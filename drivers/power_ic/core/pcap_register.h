/*
 * Copyright 2005-2006 Motorola, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 *
 *
 *  Author     Date                Comment
 * ======== ===========   =====================================================
 * Motorola 2006-Oct-25 - Update comments. 
 * Motorola 2005-Feb-28 - Full rewrite from Atlas to PCAP registers
 */

#ifndef __PCAP_REGISTER_H__
#define __PCAP_REGISTER_H__

/*!
 * @file pcap_register.h
 *
 * @brief This file contains prototypes of PCAP register access functions. 
 *
 * @ingroup poweric_core
 */ 

int pcap_reg_read (int reg, unsigned int *reg_value);
int pcap_reg_write (int reg, unsigned int reg_value);

#endif /* __PCAP_REGISTER_H__ */
