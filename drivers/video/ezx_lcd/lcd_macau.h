/*
 * linux/drivers/video/ezx_lcd/lcd_macau.h
 * Intel Bulverde/PXA250/210 LCD Controller Configuration for Moto MACAU product
 *
 * Copyright (C) 2005-2006 - Motorola
 *
 * Copyright 2003 MontaVista Software Inc.
 * Author: MontaVista Software, Inc.
 *	   source@mvista.com
 * Copyright (C) 1999 Eric A. Thomas
 * Based on acornfb.c Copyright (C) Russell King.
 *
 * 2001-08-03: Cliff Brake <cbrake@acclent.com>
 *	- ported SA1100 code to PXA
 *  
 *  This program is free software; you can redistribute	 it and/or modify it
 *  under  the terms of	 the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the	License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED	  ``AS	IS'' AND   ANY	EXPRESS OR IMPLIED
 *  WARRANTIES,	  INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO	EVENT  SHALL   THE AUTHOR  BE	 LIABLE FOR ANY	  DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED	  TO, PROCUREMENT OF  SUBSTITUTE GOODS	OR SERVICES; LOSS OF
 *  USE, DATA,	OR PROFITS; OR	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN	 CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  2005/12/30
 *	- Create to support product Macau
 *  2006/05/16
 *	- Change PWM frequence from 5k to 2.03k   
 */

#ifndef _PXA_FB_MACAU_H_
#define _PXA_FB_MACAU_H_

#include <linux/types.h>
#include "72r89803y01.h"



/* Bulverde LCD controller feature set  */
#define CONFIG_FBBASE_DOUBLE_BUFFERS 1
#define ENABLE_BASE_DB_AT_INIT 		1
#define CONFIG_OVL2_CHANNEL 1

#ifdef CONFIG_FB_PXA_24BPP
#define PXAFB_MAIN_VALID_FB     PXAFB_MAIN_OVL1
#else
#define PXAFB_MAIN_VALID_FB     PXAFB_MAIN_BASE
#endif

#define PXA_DEFAULT_PANEL       PXA_MAIN_PANEL

#define LCD_BUFFER_STRENGTH      0x2

/* Backlight dutycycle settings */
#define BKLIGHT_PRESCALE  0x33
//#define BKLIGHT_PRESCALE  0x19
#define BKLIGHT_PERIOD    0x1F3		//499 = 500HZ
//#define BKLIGHT_PERIOD    0x63
#define DEFAULT_DUTYCYCLE   0x140	//140 = 64%
//#define DEFAULT_DUTYCYCLE   0x40
#define MAX_DUTYCYCLE      (BKLIGHT_PERIOD+1)  //100
#define MIN_DUTYCYCLE      0

/* Macau related function prototype */
void pxafb_bklight_turn_on(void *pxafbinfo);
void pxafb_ezx_Backlight_turn_off(void *pxafbinfo);
void pxafb_ezx_Backlight_turn_on(void *pxafbinfo);
void pxafb_setup_gpio(void *pxafbinfo);
void pxafb_enable_controller(void *pxafbinfo);
void pxafb_disable_controller(void *pxafbinfo);
void pxafb_suspend_controller(void *pxafbinfo);

#endif /* _PXA_FB_MACAU_H_ */
