/*
 * Generic xmit layer for the PXA USB client function
 *
 * This code was loosely inspired by the original version which was
 * Copyright (c) Compaq Computer Corporation, 1998-1999
 * Copyright (c) 2001 by Nicolas Pitre
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * 02-May-2002
 *   Frank Becker (Intrinsyc) - derived from sa1100 usb_send.c
 *
 * TODO: Add support for DMA.
 *
 */

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/errno.h>
#include <asm/hardware.h>
#include <asm/dma.h>
#include <asm/system.h>
#include <asm/byteorder.h>

#include "pxa_usb.h"
#include "usb_ctl.h"

static void tx_callback(int flag, int size);

//#define DEBUG 2

#if DEBUG
static unsigned int usb_debug = DEBUG;
#else
#define usb_debug 0     /* gcc will remove all the debug code for us */
#endif

static char *ep_bulk_in1_buf;
static int   ep_bulk_in1_len;
static int   ep_bulk_in1_remain;
static usb_callback_t ep_bulk_in1_callback;
static int tx_pktsize;

//lw
static char *cur_tx_buf = 0;
static char *next_tx_buf = 0;
static int cur_tx_buf_len = 0;
static int next_tx_buf_len = 0;

/* device state is changing, async */
void
ep_bulk_in1_state_change_notify( int new_state )
{
}

/* set feature stall executing, async */
void
ep_bulk_in1_stall( void )
{
	UDCCS1 |= UDCCS_BI_FST;
}

static void
ep_bulk_in1_send_packet(void)
{
	int i;
	char *buf = ep_bulk_in1_buf + ep_bulk_in1_len - ep_bulk_in1_remain;
	int out_size = tx_pktsize;

//lw	if( usb_debug) 
            printk( "ep_bulk_in1_send_packet: UICR0=%x UDCCS1=%x\n", UICR0, UDCCS1);

	if( out_size > ep_bulk_in1_remain) 
	{
		out_size = ep_bulk_in1_remain;
	}

	for( i=0; i<out_size; i++)
	{
		UDDR1 = *buf++;	
	}

	UDCCS1 = UDCCS_BI_TPC;
	if( out_size < tx_pktsize)
	{
		/* short packet */
		UDCCS1 = UDCCS_BI_TSP;
	}
	ep_bulk_in1_remain -= out_size;

//lw	if( usb_debug) 
            printk( "ep_bulk_in1_send_packet: "
		"UICR0=%x UDCCS1=%x send bytes=%d left=%d\n", 
		UICR0, UDCCS1, out_size, ep_bulk_in1_remain);
}

static void
ep_bulk_in1_start(void)
{
	if (!ep_bulk_in1_len)
		return;

        UICR0 &= ~UICR0_IM1;

	ep_bulk_in1_send_packet();
}

static void
ep_bulk_in1_done(int flag)
{
	int size = ep_bulk_in1_len - ep_bulk_in1_remain;
	if (ep_bulk_in1_len) {
		ep_bulk_in1_len = 0;
		if (ep_bulk_in1_callback)
			ep_bulk_in1_callback(flag, size);
	}
}

int
ep_bulk_in1_init(int chn)
{
	desc_t * pd = pxa_usb_get_descriptor_ptr();
	tx_pktsize = __le16_to_cpu( pd->b.ep1.wMaxPacketSize );
	ep_bulk_in1_done(-EAGAIN);
	return 0;
}

void
ep_bulk_in1_reset(void)
{
	desc_t * pd = pxa_usb_get_descriptor_ptr();
	tx_pktsize = __le16_to_cpu( pd->b.ep1.wMaxPacketSize );
	UDCCS1 &= ~UDCCS_BI_FST;
	ep_bulk_in1_done(-EINTR);
}

void
ep_bulk_in1_int_hndlr(int usir0)
{
	int status = UDCCS1;

	if (ep_bulk_in1_remain != 0) {
		/* more data to go */
		ep_bulk_in1_start();
	} else {
		if( status & UDCCS_BI_TPC)
		{
			UDCCS1 = UDCCS_BI_TPC;
		}
		ep_bulk_in1_done(0);
	}
}

int
pxa_usb_send(char *buf, int len, usb_callback_t callback)
{
	int flags;

	if( usb_debug) 
            printk( "\npxa_usb_send: data len=%d state=%d blen=%d\n", 
		len, usbd_info.state, ep_bulk_in1_len);

//lw	if (usbd_info.state != USB_STATE_CONFIGURED)
//		return -ENODEV;

	if (ep_bulk_in1_len)
		return -EBUSY;

	local_irq_save(flags);
	ep_bulk_in1_buf = buf;
	ep_bulk_in1_len = len;
	ep_bulk_in1_callback = callback;
	ep_bulk_in1_remain = len;
	ep_bulk_in1_start();
	local_irq_restore(flags);

	return 0;
}

//lw
int pxa_usb_xmit(char *buf, int len)
{
    int state = 0;
    
    if (usbd_info.state != USB_STATE_CONFIGURED)
    {
        if( usbctl_next_state_on_event( kEvReset ) != kError )
        {
            ep0_reset();
            ep_bulk_in1_reset();
            usbctl_next_state_on_event( kEvConfig );
        }
    }
    if (cur_tx_buf)
    {
        if (next_tx_buf)  /* the buf is overflow */
        {
            printk("\nthe ep1 buffer is overflow!\n");
            return 0;
        }
        else
        {
            /* the data is about to send, return true here */      
            next_tx_buf = buf;
            next_tx_buf_len = len;
            return 1;
        }
    }
    else
    {
        cur_tx_buf = buf;
        cur_tx_buf_len = len;
        state = pxa_usb_send(buf, len, tx_callback);
        if (0 == state)
            return 1;
        else
            return 0;
    }

}

static void tx_callback(int flag, int size)
{
    if( cur_tx_buf ) 
    {
        cur_tx_buf_len = 0;
        cur_tx_buf = 0;
    } 
    else 
    {
        /*SerialOutputString("Oops: cur_tx_buf is NULL \n");*/
        return;
    }

    if( next_tx_buf ) 
    {   
        cur_tx_buf = next_tx_buf;
        cur_tx_buf_len = next_tx_buf_len;
        next_tx_buf = 0;
        next_tx_buf_len = 0;
        
        pxa_usb_send(cur_tx_buf, cur_tx_buf_len, tx_callback);
    }
}

void
pxa_usb_send_reset(void)
{
	ep_bulk_in1_reset();
}

int 
pxa_usb_xmitter_avail( void )
{
	if (usbd_info.state != USB_STATE_CONFIGURED)
		return -ENODEV;
	if (ep_bulk_in1_len)
		return -EBUSY;
	return 0;
}


EXPORT_SYMBOL(pxa_usb_xmitter_avail);
EXPORT_SYMBOL(pxa_usb_send);
EXPORT_SYMBOL(pxa_usb_send_reset);
