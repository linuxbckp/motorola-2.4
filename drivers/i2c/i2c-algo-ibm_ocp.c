/*
   -------------------------------------------------------------------------
   i2c-algo-ibm_ocp.c i2c driver algorithms for IBM PPC 405 adapters	    
   -------------------------------------------------------------------------
      
   current Mainainers
   Ian DaSilva and Armin Kuster, MontaVista Software, Inc.
   idasilva@mvista.com, Akuster@mvista.com or source@mvista.com

   Copyright 2000-2002 MontaVista Software Inc.

   Changes made to support the IIC peripheral on the IBM PPC 405

   ---------------------------------------------------------------------------
   This file was highly leveraged from i2c-algo-pcf.c, which was created
   by Simon G. Vogl and Hans Berglund:

     Copyright (C) 1995-1997 Simon G. Vogl
                   1998-2000 Hans Berglund

   With some changes from Ky�sti M�lkki <kmalkki@cc.hut.fi> and 
   Frodo Looijaard <frodol@dds.nl> ,and also from Martin Bailey
   <mbailey@littlefeet-inc.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
   ---------------------------------------------------------------------------

   History: 01/20/12 - Armin
   	akuster@mvista.com
   	ported up to 2.4.16+	

   Version 02/03/25 - Armin
       converted to ocp format
       removed commented out or #if 0 code
       added G�rard Basler's fix to iic_combined_transaction() such that it 
       returns the number of successfully completed transfers .

   Version 04/29/03 - Armin
   	General cleanups
	changes c++ style comments to c
	converted to ocp
	deleted unused printk's
	Added scan routine from Andrew May

   Version 05/25/02 - Armin
      name change for *_driver to *_dev
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <asm/io.h>

#include <linux/i2c.h>
#include <asm/ocp.h>

#include <asm/ibm4xx.h>
#include <asm/bootinfo.h>
#define IIC_VER	"2002.30.04"

/* ----- global defines ----------------------------------------------- */
#define DEB(x) if (i2c_debug>=1) x
#define DEB2(x) if (i2c_debug>=2) x
#define DEB3(x) if (i2c_debug>=3) x	/* print several statistical values */
#define DEBPROTO(x) if (i2c_debug>=9) x;
	/* debug the protocol by showing transferred bits */
#define DEF_TIMEOUT 5

#define IIC_DEV(x)	((struct ocp_dev *)adap->data)->x

/* ----- global variables ---------------------------------------------	*/

#ifdef SLO_IO
int jif;
#endif

/* module parameters:
 */
static int i2c_debug = 0;
static int iic_scan = 0;	/* have a look at what's hanging 'round         */

/* --- setting states on the bus with the right timing: ---------------	*/

#define iic_outb( reg, val) writeb(val,&(reg))
#define iic_inb( reg) readb(&(reg))

#define IICO_I2C_SDAHIGH	0x0780
#define IICO_I2C_SDALOW		0x0781
#define IICO_I2C_SCLHIGH	0x0782
#define IICO_I2C_SCLLOW		0x0783
#define IICO_I2C_LINEREAD	0x0784

#define IIC_SINGLE_XFER		0
#define IIC_COMBINED_XFER	1

#define IIC_ERR_LOST_ARB        -2
#define IIC_ERR_INCOMPLETE_XFR  -3
#define IIC_ERR_NACK            -1
#define IIC_TIMEOUT		100
#define IIC_RETRY		3

extern void iic_ibmocp_waitforpin(void *data);

/* --- other auxiliary functions --------------------------------------	*/

/*
 *Description: Puts this process to sleep for a period equal to timeout 
 */
static inline void
iic_sleep(unsigned long timeout)
{
	schedule_timeout(timeout * HZ);
}

/*
 * Description: This performs the IBM PPC 405 IIC initialization sequence
 * as described in the PPC405GP data book.
 */
static int
iic_init(struct i2c_adapter *adap)
{
	struct iic_regs *iic;
	unsigned short retval;

	iic = (struct iic_regs *) ((struct ocp_dev *) adap->data)->vaddr;

	/* Clear master low master address */
	iic_outb(iic->lmadr, 0);

	/* Clear high master address */
	iic_outb(iic->hmadr, 0);

	/* Clear low slave address */
	iic_outb(iic->lsadr, 0);

	/* Clear high slave address */
	iic_outb(iic->hsadr, 0);

	/* Clear status */
	iic_outb(iic->sts, 0x0a);

	/* Clear extended status */
	iic_outb(iic->extsts, 0x8f);

	/* Set clock division */
	iic_outb(iic->clkdiv, 0x04);

	retval = iic_inb(iic->clkdiv);
	DEB(printk("iic_init: CLKDIV register = %x\n", retval));

	/* Enable interrupts on Requested Master Transfer Complete */
	iic_outb(iic->intmsk, 0x01);

	/* Clear transfer count */
	iic_outb(iic->xfrcnt, 0x0);

	/* Clear extended control and status */
	iic_outb(iic->xtcntlss, 0xf0);

	/* Set mode control (flush master data buf, enable hold SCL, exit */
	/* unknown state.                                                 */
	iic_outb(iic->mdcntl, 0x47);

	/* Clear control register */
	iic_outb(iic->cntl, 0x0);

	DEB2(printk(KERN_DEBUG "iic_init: Initialized IIC on PPC 405\n"));
	return 0;
}

/*
 * Description: After we issue a transaction on the IIC bus, this function
 * is called.  It puts this process to sleep until we get an interrupt from
 * from the controller telling us that the transaction we requested in complete.
 */
static int
wait_for_pin(struct i2c_adapter *adap, int *status)
{

	int timeout = DEF_TIMEOUT;
	int retval;
	struct iic_regs *iic;

	iic = (struct iic_regs *) IIC_DEV(vaddr);

	*status = iic_inb(iic->sts);
#ifndef STUB_I2C

	while (timeout-- && (*status & 0x01)) {
		iic_ibmocp_waitforpin(adap->data);
		*status = iic_inb(iic->sts);
	}
#endif
	if (timeout <= 0) {
		/* Issue stop signal on the bus, and force an interrupt */
		retval = iic_inb(iic->cntl);
		iic_outb(iic->cntl, retval | 0x80);
		/* Clear status register */
		iic_outb(iic->sts, 0x0a);
		/* Exit unknown bus state */
		retval = iic_inb(iic->mdcntl);
		iic_outb(iic->mdcntl, (retval | 0x02));

		/* Check the status of the controller.  Does it still see a
		 * pending transfer, even though we've tried to stop any
		 * ongoing transaction?
		 */
		retval = iic_inb(iic->sts);
		retval = retval & 0x01;
		if (retval) {
			/* The iic controller is hosed.  It is not responding to any
			 * of our commands.  We have already tried to force it into
			 * a known state, but it has not worked.  Our only choice now
			 * is a soft reset, which will clear all registers, and force
			 * us to re-initialize the controller.
			 * Soft reset
			 */
			iic_outb(iic->xtcntlss, 0x01);
			udelay(500);
			iic_init(adap);
			/* Is the pending transfer bit in the sts reg finally cleared? */
			retval = iic_inb(iic->sts);
			retval = retval & 0x01;
			if (retval) {
				printk(KERN_CRIT
				       "The IIC Controller is hosed.  A processor reset is required\n");
			}
			/* For some reason, even though the interrupt bit in this
			 * register was set during iic_init, it didn't take.  We
			 * need to set it again.  Don't ask me why....this is just what
			 * I saw when testing timeouts.
			 */
			iic_outb(iic->intmsk, 0x01);
		}
		return (-1);
	} else
		return (0);
}

/* ------------------------------------ */
/*  Utility functions */

/*  Description: Look at the status register to see if there was an error 
 *  in the requested transaction.  If there is, look at the extended status 
 *  register and determine the exact cause. 
 */

int
analyze_status(struct i2c_adapter *adap, int *error_code)
{
	int ret;
	struct iic_regs *iic;

	iic = (struct iic_regs *) IIC_DEV(vaddr);

	ret = iic_inb(iic->sts);
	if (ret & 0x04) {
		/* Error occurred */
		ret = iic_inb(iic->extsts);
		if (ret & 0x04) {
			/* Lost arbitration */
			*error_code = IIC_ERR_LOST_ARB;
		}
		if (ret & 0x02) {
			/* Incomplete transfer */
			*error_code = IIC_ERR_INCOMPLETE_XFR;
		}
		if (ret & 0x01) {
			/* Master transfer aborted by a NACK during the transfer of the address byte */
			*error_code = IIC_ERR_NACK;
		}
		return -1;
	}
	return 0;
}

/*  Description: This function is called by the upper layers to do the 
 *  grunt work for a master send transaction
 */

static int
iic_sendbytes(struct i2c_adapter *adap, const char *buf,
	      int count, int xfer_flag)
{
	struct iic_regs *iic;
	int wrcount, status, timeout;
	int loops, remainder, i, j;
	int ret, error_code;

	iic = (struct iic_regs *) IIC_DEV(vaddr);

	if (count == 0)
		return 0;
	wrcount = 0;
	loops = count / 4;
	remainder = count % 4;

	if ((loops > 1) && (remainder == 0)) {
		for (i = 0; i < (loops - 1); i++) {

			/* Write four bytes to master data buffer */

			for (j = 0; j < 4; j++) {
				iic_outb(iic->mdbuf, buf[wrcount++]);
			}

			/* Issue command to IICO device to begin transmission */

			iic_outb(iic->cntl, 0x35);

			/*    Wait for transmission to complete.  When it does,  
			 *    loop to the top of the for statement and write the  
			 *    next four bytes. 
			 */

			timeout = wait_for_pin(adap, &status);
			if (timeout < 0) {

				/* Error handling */

				return wrcount;
			}
			ret = analyze_status(adap, &error_code);
			if (ret < 0) {
				if (error_code == IIC_ERR_INCOMPLETE_XFR) {
					/* Return the number of bytes transferred */
					ret = iic_inb(iic->xfrcnt);
					ret = ret & 0x07;
					return (wrcount - 4 + ret);
				} else
					return error_code;
			}
		}
	} else if ((loops >= 1) && (remainder > 0)) {
		/*printk(KERN_DEBUG "iic_sendbytes: (loops >= 1)\n"); */
		for (i = 0; i < loops; i++) {
			/*
			 * Write four bytes to master data buffer
			 */
			for (j = 0; j < 4; j++) {
				iic_outb(iic->mdbuf, buf[wrcount++]);
			}
			/*
			 * Issue command to IICO device to begin transmission
			 */
			iic_outb(iic->cntl, 0x35);
			/*
			 * Wait for transmission to complete.  When it does,
			 * loop to the top of the for statement and write the
			 * next four bytes.
			 */
			timeout = wait_for_pin(adap, &status);
			if (timeout < 0)
				return wrcount;

			ret = analyze_status(adap, &error_code);
			if (ret < 0) {
				if (error_code == IIC_ERR_INCOMPLETE_XFR) {
					/* Return the number of bytes transferred */
					ret = iic_inb(iic->xfrcnt);
					ret = ret & 0x07;
					return (wrcount - 4 + ret);
				} else
					return error_code;
			}
		}
	}

	if (remainder == 0)
		remainder = 4;
	/* Write the remaining bytes (less than or equal to 4) */

	for (i = 0; i < remainder; i++)
		iic_outb(iic->mdbuf, buf[wrcount++]);

	if (xfer_flag == IIC_COMBINED_XFER) {
		iic_outb(iic->cntl, (0x09 | ((remainder - 1) << 4)));
	} else {
		iic_outb(iic->cntl, (0x01 | ((remainder - 1) << 4)));
	}
	DEB2(printk(KERN_DEBUG "iic_sendbytes: Waiting for interrupt\n"));
	timeout = wait_for_pin(adap, &status);
	if (timeout < 0)
		return wrcount;
	ret = analyze_status(adap, &error_code);
	if (ret < 0) {
		if (error_code == IIC_ERR_INCOMPLETE_XFR) {
			/* Return the number of bytes transferred */
			ret = iic_inb(iic->xfrcnt);
			ret = ret & 0x07;
			return (wrcount - 4 + ret);
		} else
			return error_code;
	}
	DEB2(printk(KERN_DEBUG "iic_sendbytes: Got interrupt\n"));
	return wrcount;
}

/*
 * Description: Called by the upper layers to do the grunt work for
 * a master read transaction.
 */
static int
iic_readbytes(struct i2c_adapter *adap, char *buf, int count, int xfer_type)
{
	struct iic_regs *iic;
	int rdcount = 0, i, status, timeout;
	int loops, remainder, j;
	int ret, error_code;

	iic = (struct iic_regs *) IIC_DEV(vaddr);

	if (count == 0)
		return 0;
	loops = count / 4;
	remainder = count % 4;

	if ((loops > 1) && (remainder == 0)) {
		for (i = 0; i < (loops - 1); i++) {
			/* Issue command to begin master read (4 bytes maximum) */
			iic_outb(iic->cntl, 0x37);
			/*
			 * Wait for transmission to complete.  When it does,
			 * loop to the top of the for statement and write the
			 * next four bytes.
			 */
			timeout = wait_for_pin(adap, &status);
			if (timeout < 0)
				return rdcount;

			ret = analyze_status(adap, &error_code);
			if (ret < 0) {
				if (error_code == IIC_ERR_INCOMPLETE_XFR)
					return rdcount;
				else
					return error_code;
			}

			for (j = 0; j < 4; j++) {
				/* Wait for data to shuffle to top of data buffer
				 * This value needs to optimized.
				 */
				udelay(1);
				buf[rdcount] = iic_inb(iic->mdbuf);
				rdcount++;
			}
		}
	}

	else if ((loops >= 1) && (remainder > 0)) {
		for (i = 0; i < loops; i++) {
			/* Issue command to begin master read (4 bytes maximum) */
			iic_outb(iic->cntl, 0x37);
			/*
			 * Wait for transmission to complete.  When it does,
			 * loop to the top of the for statement and write the
			 * next four bytes.
			 */
			timeout = wait_for_pin(adap, &status);
			if (timeout < 0)
				return rdcount;

			ret = analyze_status(adap, &error_code);
			if (ret < 0) {
				if (error_code == IIC_ERR_INCOMPLETE_XFR)
					return rdcount;
				else
					return error_code;
			}

			for (j = 0; j < 4; j++) {
				/* Wait for data to shuffle to top of data buffer
				 * This value needs to optimized.
				 */
				udelay(1);
				buf[rdcount] = iic_inb(iic->mdbuf);
				rdcount++;
			}
		}
	}

	if (remainder == 0)
		remainder = 4;
	DEB2(printk
	     (KERN_DEBUG "iic_readbytes: writing %x to IICO_CNTL\n",
	      (0x03 | ((remainder - 1) << 4))));

	if (xfer_type == IIC_COMBINED_XFER) {
		iic_outb(iic->cntl, (0x0b | ((remainder - 1) << 4)));
	} else {
		iic_outb(iic->cntl, (0x03 | ((remainder - 1) << 4)));
	}
	DEB2(printk(KERN_DEBUG "iic_readbytes: Wait for pin\n"));
	timeout = wait_for_pin(adap, &status);
	DEB2(printk(KERN_DEBUG "iic_readbytes: Got the interrupt\n"));
	if (timeout < 0)
		return rdcount;

	ret = analyze_status(adap, &error_code);
	if (ret < 0) {
		if (error_code == IIC_ERR_INCOMPLETE_XFR)
			return rdcount;
		else
			return error_code;
	}

	for (i = 0; i < remainder; i++) {
		buf[rdcount] = iic_inb(iic->mdbuf);
		rdcount++;
	}

	return rdcount;
}

/*
 * Description:  This function implements combined transactions.  Combined
 * transactions consist of combinations of reading and writing blocks of data.
 * Each transfer (i.e. a read or a write) is separated by a repeated start
 * condition.
 */
static int
iic_combined_transaction(struct i2c_adapter *i2c_adap, struct i2c_msg msgs[],
			 int num)
{
	int i;
	struct i2c_msg *pmsg;
	int ret;

	DEB2(printk(KERN_DEBUG "Beginning combined transaction\n"));
	for (i = 0; i < num; i++) {
		pmsg = &msgs[i];
		if (pmsg->flags & I2C_M_RD) {

			/* Last read or write segment needs to be terminated with a stop */
			if (i < num - 1) {
				DEB2(printk(KERN_DEBUG "This one is a read\n"));
			} else {
				DEB2(printk
				     (KERN_DEBUG "Doing the last read\n"));
			}
			ret =
			    iic_readbytes(i2c_adap, pmsg->buf, pmsg->len,
					  (i <
					   num -
					   1) ? IIC_COMBINED_XFER :
					  IIC_SINGLE_XFER);

			if (ret != pmsg->len) {
				DEB2(printk("i2c-algo-ppc405.o: fail: "
					    "only read %d bytes.\n", ret));
				return i;
			} else {
				DEB2(printk
				     ("i2c-algo-ppc405.o: read %d bytes.\n",
				      ret));
			}
		} else if (!(pmsg->flags & I2C_M_RD)) {

			/* Last read or write segment needs to be terminated with a stop */
			if (i < num - 1) {
				DEB2(printk
				     (KERN_DEBUG "This one is a write\n"));
			} else {
				DEB2(printk
				     (KERN_DEBUG "Doing the last write\n"));
			}
			ret =
			    iic_sendbytes(i2c_adap, pmsg->buf, pmsg->len,
					  (i <
					   num -
					   1) ? IIC_COMBINED_XFER :
					  IIC_SINGLE_XFER);

			if (ret != pmsg->len) {
				DEB2(printk("i2c-algo-ppc405.o: fail: "
					    "only wrote %d bytes.\n", ret));
				return i;
			} else {
				DEB2(printk
				     ("i2c-algo-ppc405.o: wrote %d bytes.\n",
				      ret));
			}
		}
	}

	return num;
}

/*
 * Description: Whenever we initiate a transaction, the first byte clocked
 * onto the bus after the start condition is the address (7 bit) of the
 * device we want to talk to.  This function manipulates the address specified
 * so that it makes sense to the hardware when written to the IIC peripheral.
 *
 * Note: 10 bit addresses are not supported in this driver, although they are
 * supported by the hardware.  This functionality needs to be implemented.
 */
static inline int
iic_doAddress(struct i2c_adapter *adap, struct i2c_msg *msg, int retries)
{
	struct iic_regs *iic;
	unsigned short flags = msg->flags;
	unsigned char addr;

	iic = (struct iic_regs *) IIC_DEV(vaddr);

/*
 * The following segment for 10 bit addresses needs to be ported
 */
#if 0
	/* Ten bit addresses not supported right now */
	if ((flags & I2C_M_TEN)) {
		/* a ten bit address */
		addr = 0xf0 | ((msg->addr >> 7) & 0x03);
		DEB2(printk(KERN_DEBUG "addr0: %d\n", addr));
		/* try extended address code... */
		ret = try_address(adap, addr, retries);
		if (ret != 1) {
			printk(KERN_ERR
			       "iic_doAddress: died at extended address code.\n");
			return -EREMOTEIO;
		}
		/* the remaining 8 bit address */
		iic_outb(msg->addr & 0x7f);
		/* Status check comes here */
		if (ret != 1) {
			printk(KERN_ERR
			       "iic_doAddress: died at 2nd address code.\n");
			return -EREMOTEIO;
		}
		if (flags & I2C_M_RD) {
			i2c_repstart(adap);
			/* okay, now switch into reading mode */
			addr |= 0x01;
			ret = try_address(adap, addr, retries);
			if (ret != 1) {
				printk(KERN_ERR
				       "iic_doAddress: died at extended address code.\n");
				return -EREMOTEIO;
			}
		}
	} else
		/* ---------->  normal 7 bit address */
		/*Ten bit addresses not supported yet */
#endif
		addr = (msg->addr << 1);
	if (flags & I2C_M_RD)
		addr |= 1;
	if (flags & I2C_M_REV_DIR_ADDR)
		addr ^= 1;
	/*
	 * Write to the low slave address
	 */
	iic_outb(iic->lmadr, addr);
	/*
	 * Write zero to the high slave register since we are
	 * only using 7 bit addresses
	 */
	iic_outb(iic->hmadr, 0);

	return 0;
}

/*
 * Description: Prepares the controller for a transaction (clearing status
 * registers, data buffers, etc), and then calls either iic_readbytes or
 * iic_sendbytes to do the actual transaction.
 */
static int
iic_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[], int num)
{
	struct iic_regs *iic;
	struct i2c_msg *pmsg;
	int i = 0;
	int ret;

	iic = (struct iic_regs *) IIC_DEV(vaddr);

	pmsg = &msgs[i];

	/*
	 * Clear status register
	 */
	DEB2(printk
	     (KERN_DEBUG "iic_xfer: iic_xfer: Clearing status register\n"));
	iic_outb(iic->sts, 0x0a);

	/*
	 * Wait for any pending transfers to complete
	 */
	DEB2(printk
	     (KERN_DEBUG
	      "iic_xfer: Waiting for any pending transfers to complete\n"));
	while ((ret = iic_inb(iic->sts)) == 0x01) {
		;
	}

	/*
	 * Flush master data buf
	 */
	DEB2(printk(KERN_DEBUG "iic_xfer: Clearing master data buffer\n"));
	ret = iic_inb(iic->mdcntl);
	iic_outb(iic->mdcntl, ret | 0x40);

	/*
	 * Load slave address
	 */
	DEB2(printk(KERN_DEBUG "iic_xfer: Loading slave address\n"));
	ret = iic_doAddress(adap, pmsg, adap->retries);

	/*
	 * Check to see if the bus is busy
	 */
	ret = iic_inb(iic->extsts);
	/* Mask off the irrelevent bits */
	ret = ret & 0x70;
	/* When the bus is free, the BCS bits in the EXTSTS register are 0b100 */
	if (ret != 0x40)
		return IIC_ERR_LOST_ARB;

	/*
	 * Combined transaction (read and write)
	 */
	if (num > 1) {
		DEB2(printk
		     (KERN_DEBUG "iic_xfer: Call combined transaction\n"));
		ret = iic_combined_transaction(adap, msgs, num);
	}
	/*
	 * Read only
	 */
	else if ((num == 1) && (pmsg->flags & I2C_M_RD)) {
		/*
		 * Tell device to begin reading data from the  master data 
		 */
		DEB2(printk(KERN_DEBUG "iic_xfer: Call adapter's read\n"));
		ret =
		    iic_readbytes(adap, pmsg->buf, pmsg->len, IIC_SINGLE_XFER);
	}
	/*
	 * Write only
	 */
	else if ((num == 1) && (!(pmsg->flags & I2C_M_RD))) {
		/*
		 * Write data to master data buffers and tell our device
		 * to begin transmitting
		 */
		DEB2(printk(KERN_DEBUG "iic_xfer: Call adapter's write\n"));
		ret =
		    iic_sendbytes(adap, pmsg->buf, pmsg->len, IIC_SINGLE_XFER);
	}

	return ret;
}

/*
 * Description: Implements device specific ioctls.  Higher level ioctls can
 * be found in i2c-core.c and are typical of any i2c controller (specifying
 * slave address, timeouts, etc).  These ioctls take advantage of any hardware
 * features built into the controller for which this algorithm-adapter set
 * was written.  These ioctls allow you to take control of the data and clock
 * lines on the IBM PPC 405 IIC controller and set the either high or low,
 * similar to a GPIO pin.
 */
static int
algo_control(struct i2c_adapter *adap, unsigned int cmd, unsigned long arg)
{
	struct iic_regs *iic;
	int ret = 0;
	int lines;

	iic = (struct iic_regs *) IIC_DEV(vaddr);

	lines = iic_inb(iic->directcntl);

	if (cmd == IICO_I2C_SDAHIGH) {
		lines = lines & 0x01;
		if (lines)
			lines = 0x04;
		else
			lines = 0;
		iic_outb(iic->directcntl, (0x08 | lines));
	} else if (cmd == IICO_I2C_SDALOW) {
		lines = lines & 0x01;
		if (lines)
			lines = 0x04;
		else
			lines = 0;
		iic_outb(iic->directcntl, (0x00 | lines));
	} else if (cmd == IICO_I2C_SCLHIGH) {
		lines = lines & 0x02;
		if (lines)
			lines = 0x08;
		else
			lines = 0;
		iic_outb(iic->directcntl, (0x04 | lines));
	} else if (cmd == IICO_I2C_SCLLOW) {
		lines = lines & 0x02;
		if (lines)
			lines = 0x08;
		else
			lines = 0;
		iic_outb(iic->directcntl, (0x00 | lines));
	} else if (cmd == IICO_I2C_LINEREAD) {
		ret = lines;
	}
	return ret;
}

static u32
iic_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_SMBUS_EMUL | I2C_FUNC_10BIT_ADDR |
	    I2C_FUNC_PROTOCOL_MANGLING;
}

/* -----exported algorithm data: -------------------------------------	*/

static struct i2c_algorithm iic_algo = {
	"IBM on-chip IIC algorithm",
	I2C_ALGO_OCP,
	iic_xfer,
	NULL,
	NULL,			/* slave_xmit           */
	NULL,			/* slave_recv           */
	algo_control,		/* ioctl                */
	iic_func,		/* functionality        */
};

/* 
 * registering functions to load algorithms at runtime 
 */

/*
 * Description: Register bus structure
 */
int
i2c_iic_add_bus(struct i2c_adapter *adap)
{

	DEB2(printk
	     (KERN_DEBUG "i2c-algo-iic.o: hw routines for %s registered.\n",
	      adap->name));
	adap->id |= iic_algo.id;
	adap->algo = &iic_algo;
	adap->timeout = IIC_TIMEOUT;
	adap->retries = IIC_RETRY;

#ifdef MODULE
	MOD_INC_USE_COUNT;
#endif

	iic_init(adap);
	i2c_add_adapter(adap);
	/* scan the I2C bus for valid 7 bit addresses
	 * (ie things that ACK on 1byte read)
	 * if i2c_debug is off we print everything on one line.
	 * if i2c_debug is on we do a newline per print so we don't
	 * clash too much with printk's in the other functions.
	 * TODO: check for 10-bit mode and never run as a slave.
	 */

	if (iic_scan) {

		int found = 0;
		int i;
		printk(KERN_INFO " i2c-algo-iic.o: scanning bus %s. Found ",
		       adap->name);
		if (i2c_debug)
			printk("\n");
		for (i = 0; i < 0x7f; i++) {
			int ret;
			struct i2c_msg msg;
			char data[1];
			msg.addr = i;
			msg.buf = data;
			msg.len = 1;
			msg.flags = I2C_M_RD;
			if ((ret = iic_xfer(adap, &msg, 1)) == 1) {
				if (i2c_debug)
					printk
					    ("I2C Found 0x%02x ret %d Data 0x%02x\n",
					     i, ret, data[0]);
				else
					printk("0x%02x ", i);
				found++;
			}
		}
		if (i2c_debug) {
			if (!found)
				printk("I2C Found Nothing\n");
		} else {
			if (found)
				printk("\n");
			else
				printk("Nothing\n");
		}

	}
	return 1;
}

int
i2c_iic_del_bus(struct i2c_adapter *adap)
{
	int res;
	if ((res = i2c_del_adapter(adap)) < 0)
		return res;
	DEB2(printk
	     (KERN_DEBUG "i2c-algo-iic.o: adapter unregistered: %s\n",
	      adap->name));

#ifdef MODULE
	MOD_DEC_USE_COUNT;
#endif
	return 1;
}

int __init
i2c_algo_iic_init(void)
{
	printk(KERN_INFO "IBM On-chip iic (i2c) algorithm module %s\n",
	       IIC_VER);

	return 0;
}

void
i2c_algo_iic_exit(void)
{
	return;
}

EXPORT_SYMBOL(i2c_iic_add_bus);
EXPORT_SYMBOL(i2c_iic_del_bus);

/*
 * The MODULE_* macros resolve to nothing if MODULES is not defined
 * when this file is compiled.
 */
MODULE_AUTHOR("MontaVista Software <www.mvista.com>");
MODULE_DESCRIPTION("PPC 405 iic algorithm");

MODULE_PARM(iic_scan, "i");
MODULE_PARM(i2c_debug, "i");

MODULE_PARM_DESC(iic_scan, "Scan for active chips on the bus");
MODULE_PARM_DESC(i2c_debug,
		 "debug level - 0 off; 1 normal; 2,3 more verbose; 9 iic-protocol");

MODULE_LICENSE("GPL");

module_init(i2c_algo_iic_init);
module_exit(i2c_algo_iic_exit);
