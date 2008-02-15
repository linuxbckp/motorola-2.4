#ifndef __OWIRE_H__
#define __OWIRE_H__
/*
 * /vobs/ezx_linux/code/linux/linux-2.4.17/drivers/power_ic/module/owire.h
 *
 * Description - This is the header of internal definitions of one wire bus interface.
 *
 * Copyright (C) 2006 - Motorola
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Motorola 2006-Feb-06 - File Creation
 *
 */

/*!
 * @file owire.h
 *
 * @brief This file contains prototypes to support one wire bus for SCMA11. 
 *
 * @ingroup poweric_brt
 */ 

#include <linux/types.h>

bool owire_detect(void);
u8 owire_read(u8 number_of_bits);
void owire_write(u8 value, u8 number_of_bits);
void owire_init(void);

#endif /* __OWIRE_H__ */
