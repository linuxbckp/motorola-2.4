/*
 *  
 * (c) Copyright � 2003-2006, Marvell International Ltd. 
 *
 * This software file (the "File") is distributed by Marvell International 
 * Ltd. under the terms of the GNU General Public License Version 2, June 1991 
 * (the "License").  You may use, redistribute and/or modify this File in 
 * accordance with the terms and conditions of the License, a copy of which 
 * is available along with the File in the license.txt file or by writing to 
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 
 * 02111-1307 or on the worldwide web at http://www.gnu.org/licenses/gpl.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE 
 * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about 
 * this warranty disclaimer.
 *
 */

#ifndef _OS_HEADERS_H
#define _OS_HEADERS_H

#ifndef __ATTRIB_ALIGN__
#define __ATTRIB_ALIGN__ __attribute__((aligned(4)))
#endif

#ifndef __ATTRIB_PACK__
#define __ATTRIB_PACK__ __attribute__ ((packed))
#endif

/* Linux header files */
#include        <linux/kernel.h>
#include        <linux/module.h>
#include        <linux/init.h>
#include        <linux/version.h>
#include        <linux/param.h>
#include        <linux/delay.h>
#include        <linux/slab.h>
#include        <linux/mm.h>
#include        <linux/types.h>
#include        <linux/sched.h>
#include        <linux/timer.h>
#include        <linux/types.h>
#include        <linux/ioport.h>
#include        <linux/pci.h>
#include        <linux/ctype.h>
#include        <linux/proc_fs.h>
#include	<linux/ptrace.h>
#include	<linux/string.h>
#include	<linux/config.h>
#include	<linux/ioport.h>

#ifdef USB83xx
#include	<linux/usb.h>
#endif /* USB83xx */

/* New Code to synchronize between IEEE Power save and PM*/
#ifdef CONFIG_PM
#include <linux/pm.h>
#endif

/* ASM files */
#include        <asm/semaphore.h>
#include        <asm/byteorder.h>
#include        <asm/irq.h>
#include        <asm/uaccess.h>
#include        <asm/io.h>
#include	<asm/system.h>

/* Net header files */
#include        <linux/wireless.h>
#include        <linux/netdevice.h>
#include        <linux/net.h>
#include        <linux/ip.h>
#include        <linux/skbuff.h>
#include        <linux/if_arp.h>
#include        <linux/if_ether.h>
#include        <linux/etherdevice.h>
#include        <net/arp.h>
#include        <linux/rtnetlink.h>

/* Wireless header */
#include        <net/iw_handler.h>
#include        <linux/wireless.h>

#endif