/*
 * /vobs/ezx_linux/code/linux/linux-2.4.17/drivers/power_ic/module/owire.c
 *
 * Description - One Wire bus interface.
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
 * @file owire.c
 *
 * @brief This file contains functions for one wire bus driver on SCM-A11. 
 *
 * @ingroup poweric_brt
 */

#include <asm/arch/clock.h>
#include <asm/arch/platform.h>
#include <asm/div64.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/types.h>
#include "owire.h"

#define LSB_BIT_MASK       0x0001
#define RDST_BIT           3
#define MILLION            1000000 /* 1 million */
#define CTRL_RPP           0x0080 /* Reset Pulled and Presence Detect (RPP) */
#define CTRL_PST           0x0040 /* Presence Status (PST) */
#define CTRL_WR0           0x0020 /* Write 0 (WR0) */
#define CTRL_WR1RD         0x0010 /* Write 1 or Read (WR1/RD) */
#define CTRL_RDST          0x0008 /* Read Status (RDST) */
#define RST_RESET          0x0001 /* Software Reset of Module (rst) */
#define MUX_OW_MASK        0x00FFFFFF /* mask for owire in mux */
#define MUX_OW_IO          0x12000000 /* Enable owire mux as io */

#define REG_CTRL           IO_ADDRESS(OWIRE_BASE_ADDR + 0x00)
#define REG_CLKDIV         IO_ADDRESS(OWIRE_BASE_ADDR + 0x02)
#define REG_RST            IO_ADDRESS(OWIRE_BASE_ADDR + 0x04)
#define REG_MUXAP_OW       IO_ADDRESS(IOMUX_AP_BASE_ADDR + 0x10)

/*!
 * @brief owire_detect - detect one-wire device 
 *
 * This function implements a detection of one wire bus device.  
 *
 * @return 1 if owire device is detected
 */
bool owire_detect(void)
{
    u16 reg_val;

    reg_val = readw(REG_CTRL);
    reg_val |= CTRL_RPP;
    /* Set the RPP bit */
    writew(reg_val, REG_CTRL);

    /* Wait until the RPP bit returns low */
    while((bool)(readw(REG_CTRL) & CTRL_RPP) != 0)
    {
        /* Wait */
    }

    /* Return the state of the PST BIT */
    reg_val = readw(REG_CTRL) & CTRL_PST;

    return ((bool)reg_val);
}

/*!
 * @brief owire_read - read one-wire device 
 *
 * This function implements a read function from one wire bus device.  
 *
 * @param number_of_bits number of bits to read
 *
 * @return read result
 */
u8 owire_read(u8 number_of_bits)
{
    u8 index;
    u8 read_value = 0;
    u16 reg_val;

    for (index=0; index<number_of_bits; index++)
    {
        /* Initiate the read operation */
        reg_val = readw(REG_CTRL);
        reg_val |= CTRL_WR1RD;
        writew(reg_val, REG_CTRL);
        /* Wait until WR1/RD bit returns low */
        while((bool)(readw(REG_CTRL) & CTRL_WR1RD) != 0)
        {
            /* Wait */
        }
        /* Place read bit into the current bit positon of the pattern being read */
        read_value |= ((readw(REG_CTRL) & CTRL_RDST)>>RDST_BIT)<<index;
    }

    return (read_value);
}

/*!
 * @brief owire_write - write to one-wire device 
 *
 * This function implements a write function to one wire bus device.  
 *
 * @param value          value to write
 * @param number_of_bits number of bits to write
 */
void owire_write(u8 value, u8 number_of_bits)
{
    u8 index;
    u16 reg_val;
   
    for (index=0; index<number_of_bits; index++)
    {
        reg_val = readw(REG_CTRL);
        /* if current bit is a one then set WR1/RD else set WR0 */
        (bool)((value >> index) & LSB_BIT_MASK) ? (reg_val |= CTRL_WR1RD) : (reg_val |= CTRL_WR0);
        writew(reg_val, REG_CTRL);
        /* Wait until the the WR0 bit or the WR1/RD bit return low */
        while (((bool)(readw(REG_CTRL) & CTRL_WR0) != 0) || 
                ((bool)(readw(REG_CTRL) & CTRL_WR1RD) != 0))
        {
            /* Wait */
        }
    }      
}

/*!
 * @brief owire_init - initial one-wire device 
 *
 * This function implements the initial function on one wire module.
 */
void owire_init(void)
{
    u16 reg_val;
    u32 rem; 
    u32 mux;
    
    /* Read clock used by owire module, in scma11, i2c and owire uses same clock */
    u32 clk = mxc_get_clocks(I2C_CLK);

    /* Determine the value for clock divider register,
       SCMA11 uses clock/(divider+1) to generate 1MHz internal clock */
    rem = do_div(clk, MILLION);
    if (rem < (MILLION/2)){
        clk--;
    }

    /* Set mux as io */
    mux = readl(REG_MUXAP_OW);
    mux &= MUX_OW_MASK;
    mux |= MUX_OW_IO;
    writel(mux, REG_MUXAP_OW);

    /* Set bit 0 to put 1 wire module in reset */
    reg_val = readw(REG_RST);
    reg_val |= RST_RESET;
    writew(reg_val, REG_RST);

    /* Wait for 700us, defined by scma11 spec */
    udelay(700);

    /* Clear reset bit to pull 1 wire module out of reset */
    reg_val &= ~RST_RESET;
    writew(reg_val, REG_RST);
 
    /* Set up the one-wire clock divider */
    writew((u16)clk, REG_CLKDIV);
}

