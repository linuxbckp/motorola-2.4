/*
 * Copyright (C) 2002--2006 Motorola Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
 * drivers/mtd/maps/ezx_parts.c
 *
 * Parse multiple flash partitions in Ezx project into mtd_table or into 
 * act_roflash_table
 *
 * history:
 * 2002/10/22  Create for EzX 
 * 2005/12/21  support macau product
 * 2006/03/20  Change roflash to support linear read 
 * 2006/06/06  change memory layout for macau (Add 1M space to resource area, and reduce 1M from language space)
 * 2006/06/15  support penglai product
 * 2006/08/09  modify memory map to support multi-product
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/ezx_roflash.h>

roflash_area *act_roflash_table[MAX_ROFLASH] = {0};
unsigned short roflash_partitions = 0;

extern int roflash_init(void);

extern int cfi_intelext_read_roflash(void *priv_map, unsigned long from, size_t len, size_t *retlen, u_char *buf);
extern int cfi_intelext_read_roflash_c(void *priv_map, unsigned long from, size_t len, size_t *retlen, u_char *buf);
roflash_area **roflash_table_pptr = NULL;

int linear_roflash_read(void *priv_map, unsigned long from, size_t len, size_t *retlen, u_char *buf)
{
	unsigned char *map_offset = (unsigned char *)(priv_map + from);
	unsigned long region_start, region_end;
	int i;
	
	if (!roflash_partitions) 
		return -EFAULT;
	for (i = 0; i < roflash_partitions; i++) {
		if(!act_roflash_table[i])
			return -EFAULT;
		region_start = (unsigned long)(act_roflash_table[i]->priv_map 
				+ act_roflash_table[i]->offset);
		region_end = (unsigned long)(act_roflash_table[i]->priv_map 
				+ act_roflash_table[i]->offset 
				+ act_roflash_table[i]->size);
		if ((unsigned long)map_offset >= region_start 
			&& (unsigned long)map_offset < region_end) {
			if ((unsigned long)(map_offset + len) <= region_end)
				*retlen = len;
			else 
				*retlen = region_end-(unsigned long)map_offset;
			goto right_add;
		}	
	}
	return -EFAULT;

right_add:
	memcpy(buf, map_offset, *retlen);
	return 0;	
}

static DECLARE_MUTEX(roflash_table_mutex);

/* I know what I am doing, so setup l_x_b flag */
#ifdef CONFIG_ARCH_EZXBASE
static roflash_area  fix_roflash_table[]= 
{
	{
		name:		ROFLASH_ROOTFS_NAME,
		size:		(MMAP_ROOTFS_SIZE - MMAP_SECURE_HEADER_4KB),
		offset:		(MMAP_ROOTFS_CHIP_OFFSET + MMAP_SECURE_HEADER_4KB),
		roflash_read:   linear_roflash_read,  /* force read-only in cacheable mapping*/
		l_x_b:		ROFLASH_LINEAR,
		phys_addr:	MMAP_FLASH2_START_ADDR,
	},{
		name:		ROFLASH_LANGUAGE_NAME,
		size:		MMAP_LANGUAGE_SIZE,
		offset:		MMAP_LANGUAGE_CHIP_OFFSET,
		roflash_read:   cfi_intelext_read_roflash,
		l_x_b:		ROFLASH_BLOCK,
		phys_addr:      0xffffffff, // not use for block cramfs mount 
	},{
		name:		ROFLASH_RESOURCE_NAME,  /* resource.img */
		size:		MMAP_RESOURCE_SIZE,
		offset:		MMAP_RESOURCE_CHIP_OFFSET,
		roflash_read:   cfi_intelext_read_roflash,
		l_x_b:		ROFLASH_BLOCK,
		phys_addr:      0xffffffff, // not use for block cramfs mount
	},{
		name:		ROFLASH_SETUP_NAME,	/* setup.img */
		size:		MMAP_SETUP_SIZE,
		offset:         MMAP_SETUP_CHIP_OFFSET,
		roflash_read:   linear_roflash_read,
		l_x_b:		ROFLASH_LINEAR,
		phys_addr:      MMAP_FLASH2_START_ADDR,
	},{
		name:		ROFLASH_SECURE_SETUP_NAME,  /* securesetup.img */
		size:		(MMAP_SECURE_SETUP_SIZE - MMAP_SECURE_HEADER_4KB),
		offset:		(MMAP_SECURE_SETUP_CHIP_OFFSET + MMAP_SECURE_HEADER_4KB),
		roflash_read:   cfi_intelext_read_roflash,
		l_x_b:		ROFLASH_BLOCK,
		phys_addr:      0xffffffff, // not use for block cramfs mount
	}
};
#endif

#ifdef CONFIG_ARCH_EZX_A780
static roflash_area  fix_roflash_table[]= 
{
	{
		name:		"rootfs",
		size:		0x018E0000,  /* rootfs size is 24.875MB */
		offset:		0x00120000,
		roflash_read:   NULL,  /* force read-only in cacheable mapping*/
		l_x_b:		ROFLASH_LINEAR,
		phys_addr:	0x00000000,
	},{
		name:		"NO-USE",  /* language package is moved to MDOC */
		size:		0x00000000,
		offset:		0x01A00000,  // correspond to the beginning of the second flash chip //
		roflash_read:  cfi_intelext_read_roflash,
		l_x_b:		ROFLASH_BLOCK,
		phys_addr:  0xffffffff, // not use for block cramfs mount
	},{
		name:		"setup",
		size:		0x00020000,
		offset:         0x01FA0000,
		roflash_read:  cfi_intelext_read_roflash,
		l_x_b:		ROFLASH_BLOCK,
		phys_addr:  0xffffffff, // not use for block cramfs mount
	}
};
#if (0)
static roflash_area  fix_roflash_table[]= 
{
	{
		name:		"rootfs",
		size:		0x00f00000,  /* rootfs size is 15MB */
		offset:		0x00100000,
		roflash_read:  NULL,  /* force read-only in cacheable mapping*/
		l_x_b:		ROFLASH_LINEAR_XIP,
		phys_addr:	0x00000000,
	},{
		name:		"nxipapp",  /* old language area is 0xAA0000 */
		size:		0x00AA0000,
		offset:		0x00500000,
		roflash_read:  cfi_intelext_read_roflash,
		l_x_b:		ROFLASH_BLOCK,
		phys_addr:  0xffffffff, // not use for block cramfs mount
	},{
		name:		"setup",     /* setup stuff is 1MB */
		size:		0x00020000,
		offset:         0x00FA0000,
		roflash_read:  cfi_intelext_read_roflash,
		l_x_b:		ROFLASH_BLOCK,
		phys_addr:  0xffffffff, //phys_addr is not useful for block cramfs mount
	}
};
#endif

#endif

#ifdef CONFIG_ARCH_EZX_E680
static roflash_area  fix_roflash_table[]= 
{
	{
		name:		"rootfs",
		size:		0x018E0000,  /* rootfs size is 15MB */
		offset:		0x00120000,
		roflash_read:   NULL,  /* force read-only in cacheable mapping*/
		l_x_b:		ROFLASH_LINEAR,
		phys_addr:	0x00000000,
	},{
		name:		"NO-USE",  /* language package is 2MB */
		size:		0x00000000,
		offset:		0x01A00000,  // correspond to the beginning of the second flash chip //
		roflash_read:  cfi_intelext_read_roflash,
		l_x_b:		ROFLASH_BLOCK,
		phys_addr:  0xffffffff, // not use for block cramfs mount
	},{
		name:		"setup",     /* setup stuff is 1MB */
		size:		0x00020000,
		offset:         0x01FA0000,  // correspond to the beginning of the second flash chip //
		roflash_read:  cfi_intelext_read_roflash,
		l_x_b:		ROFLASH_BLOCK,
		phys_addr:  0xffffffff, // not use for block cramfs mount
	}
};
#endif

/* If we decide to use flash partition parse table, this function will parse the fpt - flash partition table,
*  the beginning address of fpt is defined in CONFIG_FPT_OFFSET.  This function will parser out all roflash areas
*  and animation area( or re-fresh code area).   
*  For roflash areas, registered them into act_roflash_table, for animation region, register it into mtd_table[6].
*  Note 
*  6 is fixed for animation region -- /dev/mtd6 c 90 12
*  /dev/roflash b 62 0  -- root file system
*  /dev/roflash1 b 62 1 -- language package
*  /dev/roflash2 b 62 2 -- setup stuff
*/  
int ezx_parse(struct map_info *chip_map)
{
	return 0;  // depends on discussion about DC tool
}

int ezx_parts_parser(void)
{
	int ret = 0;
	int nrparts = 0;
	int i = 0;

//#ifdef CONFIG_EZX_PARSER_TABLE
//	ret = ezx_parse(chip_map);
//#else
	down(&roflash_table_mutex);

	nrparts = sizeof(fix_roflash_table) / sizeof(roflash_area);

#ifdef ROFLASH_DEBUG_ERR
	printk("ezx_parts_parser:  nrparts is %d\n", nrparts);
#endif
	for ( i = 0; i < nrparts; i ++ )
	{
		if (act_roflash_table[i])
			return -1;
		act_roflash_table[i] = (roflash_area *)(&(fix_roflash_table[i]));
#ifdef ROFLASH_DEBUG_ERR
		printk(KERN_ERR "ezx_parts_parser: act_roflash_table[%d] = %d\n",i,(unsigned long)(act_roflash_table[i]));
#endif
	}
	roflash_partitions = i;

#ifdef ROFLASH_DEBUG_ERR
	printk("ezx_parts_parser:  roflash_partitions is %d\n", roflash_partitions);
#endif
	
	up(&roflash_table_mutex);
	
	return ret;
}
			
int ezx_partition_init(void)
{
	int ret = 0;

	
#ifdef ROFLASH_DEBUG_ERR
	printk(KERN_ERR "ezx_partition_init: roflash_table_pptr = %d\n",(unsigned long)(act_roflash_table));
#endif
	roflash_table_pptr = (roflash_area **)(act_roflash_table);

	ret = ezx_parts_parser();
	if ( ret )
	{
		printk(KERN_NOTICE "invoke ezx_parts_parser, return %d\n", ret);
		return -EIO;
	}

#ifdef ROFLASH_DEBUG_ERR
	printk("ezx_partition_init:  ret of ezx_parts_parser is %d\n", ret);
#endif

	ret = roflash_init();
	if ( ret )
	{
		printk(KERN_NOTICE "invoke roflash_init, return %d\n", ret);
		return -EIO;
	}
	return 0 ;
}






