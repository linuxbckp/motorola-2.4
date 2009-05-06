/*
 * Compressed rom filesystem for Linux.
 *
 * Copyright (C) 1999 Linus Torvalds.
 *
 * This file is released under the GPL.
 */

/*
 * These are the VFS interfaces to the compressed rom filesystem.
 * The actual compression is based on zlib, see the other files.
 */

/* Linear Addressing code
 *
 * Copyright (C) 2000 Shane Nay.
 *
 * Allows you to have a linearly addressed cramfs filesystem.
 * Saves the need for buffer, and the munging of the buffer.
 * Savings a bit over 32k with default PAGE_SIZE, BUFFER_SIZE
 * etc.  Usefull on embedded platform with ROM :-).
 *
 * Downsides- Currently linear addressed cramfs partitions
 * don't co-exist with block cramfs partitions.
 *
 */

/*
 * 28-Dec-2000: XIP mode for linear cramfs
 * Copyright (C) 2000 Robert Leslie <rob@mars.org>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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
 * Copyright � 2005,2007 Motorola
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 *
 */
 
/*
 *  Author     Date                Comment
 * ======== ===========   =====================================================
 * Motorola 2005-Dec-20   Port to EzX platform
 * Motorola 2007-Jan-11   fix cramfs time issue
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/locks.h>
#include <linux/blkdev.h>
#include <linux/cramfs_fs.h>
#include <asm/semaphore.h>
#include <asm/io.h>

#include <asm/uaccess.h>

#ifdef CONFIG_ARCH_EZX

#include <linux/mtd/map.h>
#include <linux/ezx_roflash.h>


#define CRAMFS_SB_MAGIC u.cramfs_sb.magic
#define CRAMFS_SB_SIZE u.cramfs_sb.size
#define CRAMFS_SB_BLOCKS u.cramfs_sb.blocks
#define CRAMFS_SB_FILES u.cramfs_sb.files
#define CRAMFS_SB_FLAGS u.cramfs_sb.flags
#define CRAMFS_SB_LINEAR_PHYS_ADDR u.cramfs_sb.linear_phys_addr
#define CRAMFS_SB_LINEAR_VIRT_ADDR u.cramfs_sb.linear_virt_addr
 
#define CRAMFS_SB_LXB  u.cramfs_sb.l_x_b

static struct super_operations cramfs_ops;
static struct inode_operations cramfs_dir_inode_operations;
static struct file_operations cramfs_directory_operations;
static struct address_space_operations cramfs_aops;

static DECLARE_MUTEX(read_mutex);


/* These two macros may change in future, to provide better st_ino
   semantics. */
#define CRAMINO(x)	((x)->offset?(x)->offset<<2:1)
#define OFFSET(x)	((x)->i_ino)


#if defined(CONFIG_XIP_DEBUGGABLE) && defined(CONFIG_PROC_FS)
#include <linux/proc_fs.h>

static struct proc_dir_entry *proc_xip_debug;
int xip_enable_debug;

int read_xip_debug(char *page, char **start, off_t off,
			  int count, int *eof, void *data)
{
	int ret;

	ret = sprintf(page + off, "%d\n", xip_enable_debug);
	*eof = 1;
	return ret;
}

int write_xip_debug(struct file *file, const char *buffer,
			   unsigned long count, void *data)
{
	char buf[32];

	if (count > ARRAY_SIZE(buf) - 1)
		count = ARRAY_SIZE(buf) - 1;
	if (copy_from_user(buf, buffer, count))
		return -EFAULT;
	buf[count - 1] = '\0';
	if (!strcmp(buf, "0") || !strcmp(buf, "1"))
		xip_enable_debug = simple_strtoul(buf, NULL, 10);
	return count;
}

#define XIP_PROC_NAME "sys/fs/xip-debug"

static void __init create_cramfs_proc_entry(void)
{
	proc_xip_debug = create_proc_entry(XIP_PROC_NAME, 0644, NULL);

	if (proc_xip_debug) {
		proc_xip_debug->read_proc = read_xip_debug;
		proc_xip_debug->write_proc = write_xip_debug;
	}
}

static void __exit remove_cramfs_proc_entry(void)
{
	if (proc_xip_debug)
		remove_proc_entry(XIP_PROC_NAME, NULL);
}

#else

#define create_cramfs_proc_entry() do {} while (0)
#define remove_cramfs_proc_entry() do {} while (0)

#endif

static int cramfs_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long address, length;
	struct inode *inode = file->f_dentry->d_inode;
	struct super_block *sb = inode->i_sb;

	/* this is only used in the case of read-only maps for XIP */

	if (vma->vm_flags & VM_WRITE)
		return generic_file_mmap(file, vma);

	if ((vma->vm_flags & VM_SHARED) && (vma->vm_flags & VM_MAYWRITE))
		return -EINVAL;

	address  = PAGE_ALIGN(sb->CRAMFS_SB_LINEAR_PHYS_ADDR + OFFSET(inode));
	address += vma->vm_pgoff << PAGE_SHIFT;

	length = vma->vm_end - vma->vm_start;

	if (length > inode->i_size)
		length = inode->i_size;

	length = PAGE_ALIGN(length);


#if 0
	/* Doing the following makes it slower and more broken.  bdl */
	/*
	 * Accessing memory above the top the kernel knows about or
	 * through a file pointer that was marked O_SYNC will be
	 * done non-cached.
	 */
	vma->vm_page_prot =
		__pgprot((pgprot_val(vma->vm_page_prot) & ~_CACHE_MASK)
			| _CACHE_UNCACHED);
#endif

	/*
	 * Don't dump addresses that are not real memory to a core file.
	 */
	vma->vm_flags |= (VM_IO | VM_XIP);
	flush_tlb_page(vma, address);
	if (remap_page_range(vma->vm_start, address, length,
			     vma->vm_page_prot))
		return -EAGAIN;

#ifdef DEBUG_CRAMFS_XIP
	printk("cramfs_mmap: mapped %s at 0x%08lx, length %lu to vma 0x%08lx"
		", page_prot 0x%08lx\n",
		file->f_dentry->d_name.name, address, length,
		vma->vm_start, pgprot_val(vma->vm_page_prot));
#endif

	return 0;
}

static struct file_operations cramfs_linear_xip_fops = {
	read:	generic_file_read,
	mmap:	cramfs_mmap,
};

#define CRAMFS_INODE_IS_XIP(x) ((x)->i_mode & S_ISVTX)

#ifdef CONFIG_ARCH_EZXBASE
#define MKYEAR(y) \
	((((y-1)/4-(y-1)/100+(y-1)/400+367*11/12+1)+(y-1)*365-719499)*24*60*60)
static unsigned long default_time = MKYEAR(2000);
#endif

static struct inode *get_cramfs_inode(struct super_block *sb, struct cramfs_inode * cramfs_inode)
{
	struct inode * inode = new_inode(sb);

	if (inode) {
		inode->i_mode = cramfs_inode->mode;
		inode->i_uid = cramfs_inode->uid;
		inode->i_size = cramfs_inode->size;
		inode->i_blocks = (cramfs_inode->size - 1) / 512 + 1;
		inode->i_blksize = PAGE_CACHE_SIZE;
		inode->i_gid = cramfs_inode->gid;
		inode->i_ino = CRAMINO(cramfs_inode);
	#ifdef CONFIG_ARCH_EZXBASE
		inode->i_mtime = inode->i_atime = inode->i_ctime = default_time;
	#else
		inode->i_mtime = inode->i_atime = inode->i_ctime = 0;
	#endif
		/* inode->i_nlink is left 1 - arguably wrong for directories,
		   but it's the best we can do without reading the directory
	           contents.  1 yields the right result in GNU find, even
		   without -noleaf option. */
		insert_inode_hash(inode);
		if (S_ISREG(inode->i_mode)) 
		{
			switch (sb->CRAMFS_SB_LXB)
			{
				case  ROFLASH_LINEAR:
					inode->i_fop = &generic_ro_fops;
					break;
				case ROFLASH_LINEAR_XIP:
					inode->i_fop = CRAMFS_INODE_IS_XIP(inode) ? &cramfs_linear_xip_fops : &generic_ro_fops;
					break;
				case ROFLASH_BLOCK:
					inode->i_fop = &generic_ro_fops;
					break;
			}
			
			inode->i_data.a_ops = &cramfs_aops;
		} else if (S_ISDIR(inode->i_mode)) {
			inode->i_op = &cramfs_dir_inode_operations;
			inode->i_fop = &cramfs_directory_operations;
		} else if (S_ISLNK(inode->i_mode)) {
			inode->i_op = &page_symlink_inode_operations;
			inode->i_data.a_ops = &cramfs_aops;
		} else {
			inode->i_size = 0;
			init_special_inode(inode, inode->i_mode, cramfs_inode->size);
		}
	}
	return inode;
}

#if (0)  //either Linear or Block cramfs //
#ifdef CONFIG_CRAMFS_LINEAR
/*
 * Return a pointer to the block in the linearly addressed cramfs image.
 */
static void *cramfs_read(struct super_block *sb, unsigned int offset, unsigned int len)
{
	if (!len)
		return NULL;
	return (void*)(sb->CRAMFS_SB_LINEAR_VIRT_ADDR + offset);
}
#else /* Not linear addressing - aka regular block mode. */
/*
 * We have our own block cache: don't fill up the buffer cache
 * with the rom-image, because the way the filesystem is set
 * up the accesses should be fairly regular and cached in the
 * page cache and dentry tree anyway..
 *
 * This also acts as a way to guarantee contiguous areas of up to
 * BLKS_PER_BUF*PAGE_CACHE_SIZE, so that the caller doesn't need to
 * worry about end-of-buffer issues even when decompressing a full
 * page cache.
 */
#define READ_BUFFERS (2)
/* NEXT_BUFFER(): Loop over [0..(READ_BUFFERS-1)]. */
#define NEXT_BUFFER(_ix) ((_ix) ^ 1)

/*
 * BLKS_PER_BUF_SHIFT should be at least 2 to allow for "compressed"
 * data that takes up more space than the original and with unlucky
 * alignment.
 */
#define BLKS_PER_BUF_SHIFT	(2)
#define BLKS_PER_BUF		(1 << BLKS_PER_BUF_SHIFT)
#define BUFFER_SIZE		(BLKS_PER_BUF*PAGE_CACHE_SIZE)

static unsigned char read_buffers[READ_BUFFERS][BUFFER_SIZE];
static unsigned buffer_blocknr[READ_BUFFERS];
static struct super_block * buffer_dev[READ_BUFFERS];
static int next_buffer;

/*
 * Returns a pointer to a buffer containing at least LEN bytes of
 * filesystem starting at byte offset OFFSET into the filesystem.
 */
static void *cramfs_read(struct super_block *sb, unsigned int offset, unsigned int len)
{
	struct buffer_head * bh_array[BLKS_PER_BUF];
	struct buffer_head * read_array[BLKS_PER_BUF];
	unsigned i, blocknr, buffer, unread;
	unsigned long devsize;
	int major, minor;
	
	char *data;

	if (!len)
		return NULL;
	blocknr = offset >> PAGE_CACHE_SHIFT;
	offset &= PAGE_CACHE_SIZE - 1;

	/* Check if an existing buffer already has the data.. */
	for (i = 0; i < READ_BUFFERS; i++) {
		unsigned int blk_offset;

		if (buffer_dev[i] != sb)
			continue;
		if (blocknr < buffer_blocknr[i])
			continue;
		blk_offset = (blocknr - buffer_blocknr[i]) << PAGE_CACHE_SHIFT;
		blk_offset += offset;
		if (blk_offset + len > BUFFER_SIZE)
			continue;
		return read_buffers[i] + blk_offset;
	}

	devsize = ~0UL;
	major = MAJOR(sb->s_dev);
	minor = MINOR(sb->s_dev);

	if (blk_size[major])
		devsize = blk_size[major][minor] >> 2;

	/* Ok, read in BLKS_PER_BUF pages completely first. */
	unread = 0;
	for (i = 0; i < BLKS_PER_BUF; i++) {
		struct buffer_head *bh;

		bh = NULL;
		if (blocknr + i < devsize) {
			bh = sb_getblk(sb, blocknr + i);
			if (!buffer_uptodate(bh))
				read_array[unread++] = bh;
		}
		bh_array[i] = bh;
	}

	if (unread) {
		ll_rw_block(READ, unread, read_array);
		do {
			unread--;
			wait_on_buffer(read_array[unread]);
		} while (unread);
	}

	/* Ok, copy them to the staging area without sleeping. */
	buffer = next_buffer;
	next_buffer = NEXT_BUFFER(buffer);
	buffer_blocknr[buffer] = blocknr;
	buffer_dev[buffer] = sb;

	data = read_buffers[buffer];
	for (i = 0; i < BLKS_PER_BUF; i++) {
		struct buffer_head * bh = bh_array[i];
		if (bh) {
			memcpy(data, bh->b_data, PAGE_CACHE_SIZE);
			brelse(bh);
		} else
			memset(data, 0, PAGE_CACHE_SIZE);
		data += PAGE_CACHE_SIZE;
	}
	return read_buffers[buffer] + offset;
}
#endif /* !CONFIG_CRAMFS_LINEAR */
#endif

/* Both Linear and Block cramfs */

/* Not linear addressing - aka regular block mode. */
/*
 * We have our own block cache: don't fill up the buffer cache
 * with the rom-image, because the way the filesystem is set
 * up the accesses should be fairly regular and cached in the
 * page cache and dentry tree anyway..
 *
 * This also acts as a way to guarantee contiguous areas of up to
 * BLKS_PER_BUF*PAGE_CACHE_SIZE, so that the caller doesn't need to
 * worry about end-of-buffer issues even when decompressing a full
 * page cache.
 */
#define READ_BUFFERS (2)
/* NEXT_BUFFER(): Loop over [0..(READ_BUFFERS-1)]. */
#define NEXT_BUFFER(_ix) ((_ix) ^ 1)

/*
 * BLKS_PER_BUF_SHIFT should be at least 2 to allow for "compressed"
 * data that takes up more space than the original and with unlucky
 * alignment.
 */
#define BLKS_PER_BUF_SHIFT	(2)
#define BLKS_PER_BUF		(1 << BLKS_PER_BUF_SHIFT)
#define BUFFER_SIZE		(BLKS_PER_BUF*PAGE_CACHE_SIZE)

static unsigned char read_buffers[READ_BUFFERS][BUFFER_SIZE];
static unsigned buffer_blocknr[READ_BUFFERS];
static struct super_block * buffer_dev[READ_BUFFERS];
static int next_buffer;

/*
 * Return a pointer to the block in the linearly addressed cramfs image.
 */
static void *cramfs_read(struct super_block *sb, unsigned int offset, unsigned int len)
{
	if ( (sb->CRAMFS_SB_LXB == ROFLASH_LINEAR_XIP) || (sb->CRAMFS_SB_LXB == ROFLASH_LINEAR) )
	{
		if (!len)
			return NULL;
		return (void*)(sb->CRAMFS_SB_LINEAR_VIRT_ADDR + offset);
	}

	if (sb->CRAMFS_SB_LXB == ROFLASH_BLOCK)
	{
		struct buffer_head * bh_array[BLKS_PER_BUF];
		struct buffer_head * read_array[BLKS_PER_BUF];
		unsigned i, blocknr, buffer, unread;
		unsigned long devsize;
		int major, minor;
		
		char *data;

		if (!len)
			return NULL;
		blocknr = offset >> PAGE_CACHE_SHIFT;
		offset &= PAGE_CACHE_SIZE - 1;

		/* Check if an existing buffer already has the data.. */
		for (i = 0; i < READ_BUFFERS; i++) {
			unsigned int blk_offset;

			if (buffer_dev[i] != sb)
				continue;
			if (blocknr < buffer_blocknr[i])
				continue;
			blk_offset = (blocknr - buffer_blocknr[i]) << PAGE_CACHE_SHIFT;
			blk_offset += offset;
			if (blk_offset + len > BUFFER_SIZE)
				continue;
			return read_buffers[i] + blk_offset;
		}

		devsize = ~0UL;
		major = MAJOR(sb->s_dev);
		minor = MINOR(sb->s_dev);

		if (blk_size[major])
			devsize = blk_size[major][minor] >> 2;

		/* Ok, read in BLKS_PER_BUF pages completely first. */
		unread = 0;
		for (i = 0; i < BLKS_PER_BUF; i++) {
			struct buffer_head *bh;

			bh = NULL;
			if (blocknr + i < devsize) {
				bh = sb_getblk(sb, blocknr + i);
				if (!buffer_uptodate(bh))
					read_array[unread++] = bh;
			}
			bh_array[i] = bh;
		}

		if (unread) {
			ll_rw_block(READ, unread, read_array);
			do {
				unread--;
				wait_on_buffer(read_array[unread]);
			} while (unread);
		}

		/* Ok, copy them to the staging area without sleeping. */
		buffer = next_buffer;
		next_buffer = NEXT_BUFFER(buffer);
		buffer_blocknr[buffer] = blocknr;
		buffer_dev[buffer] = sb;

		data = read_buffers[buffer];
		for (i = 0; i < BLKS_PER_BUF; i++) {
			struct buffer_head * bh = bh_array[i];
			if (bh) {
				memcpy(data, bh->b_data, PAGE_CACHE_SIZE);
				brelse(bh);
			} else
				memset(data, 0, PAGE_CACHE_SIZE);
			data += PAGE_CACHE_SIZE;
		}
		return read_buffers[buffer] + offset;
	}
}


/* either LINEAR or BLOCK version */
#if (0)
static struct super_block * cramfs_read_super(struct super_block *sb, void *data, int silent)
{
#ifndef CONFIG_CRAMFS_LINEAR
	int i;
#else
	char *p;
#endif
	struct cramfs_super super;
	unsigned long root_offset;
	struct super_block * retval = NULL;

	sb->s_blocksize = PAGE_CACHE_SIZE;
	sb->s_blocksize_bits = PAGE_CACHE_SHIFT;

#ifndef CONFIG_CRAMFS_LINEAR

	set_blocksize(sb->s_dev, PAGE_CACHE_SIZE);

	/* Invalidate the read buffers on mount: think disk change.. */
	for (i = 0; i < READ_BUFFERS; i++)
		buffer_blocknr[i] = -1;

#else

	/*
	 * The physical location of the cramfs image is specified as
	 * a mount parameter.  This parameter is mandatory for obvious
	 * reasons.  Some validation is made on the phys address but this
	 * is not exhaustive and we count on the fact that someone using
	 * this feature is supposed to know what he/she's doing.
	 */
	if (!data || !(p = strstr((char *)data, "physaddr="))) {
		printk(KERN_ERR "cramfs: unknown physical address for linear cramfs image\n");
		goto out;
	}
	sb->CRAMFS_SB_LINEAR_PHYS_ADDR = simple_strtoul(p + 9, NULL, 0);
	if (sb->CRAMFS_SB_LINEAR_PHYS_ADDR & (PAGE_SIZE-1)) {
		printk(KERN_ERR "cramfs: physical address 0x%lx for linear cramfs isn't aligned to a page boundary\n",
		       sb->CRAMFS_SB_LINEAR_PHYS_ADDR);
		goto out;
	}
	if (sb->CRAMFS_SB_LINEAR_PHYS_ADDR == 0) {
		printk(KERN_ERR "cramfs: physical address for linear cramfs image can't be 0\n");
		goto out;
	}
	printk(KERN_INFO "cramfs: checking physical address 0x%lx for linear cramfs image\n",
	       sb->CRAMFS_SB_LINEAR_PHYS_ADDR);

	/* Map only one page for now.  Will remap it when fs size is known. */
	sb->CRAMFS_SB_LINEAR_VIRT_ADDR =
		ioremap(sb->CRAMFS_SB_LINEAR_PHYS_ADDR, PAGE_SIZE);
	if (!sb->CRAMFS_SB_LINEAR_VIRT_ADDR) {
		printk(KERN_ERR "cramfs: ioremap of the linear cramfs image failed\n");
		goto out;
	}
#endif

	down(&read_mutex);
	/* Read the first block and get the superblock from it */
	memcpy(&super, cramfs_read(sb, 0, sizeof(super)), sizeof(super));
	up(&read_mutex);

	/* Do sanity checks on the superblock */
	if (super.magic != CRAMFS_MAGIC) {
		printk("at offset 0, no cramfs magic: trying 512...\n");
		/* check at 512 byte offset */
		memcpy(&super, cramfs_read(sb, 512, sizeof(super)), sizeof(super));
		if (super.magic != CRAMFS_MAGIC) {
			printk(KERN_ERR "cramfs: wrong magic\n");
			goto out;
		}
	}

	/* get feature flags first */
	if (super.flags & ~CRAMFS_SUPPORTED_FLAGS) {
		printk(KERN_ERR "cramfs: unsupported filesystem features\n");
		goto out;
	}

	/* Check that the root inode is in a sane state */
	if (!S_ISDIR(super.root.mode)) {
		printk(KERN_ERR "cramfs: root is not a directory\n");
		goto out;
	}
	root_offset = super.root.offset << 2;
	if (super.flags & CRAMFS_FLAG_FSID_VERSION_2) {
		sb->CRAMFS_SB_SIZE=super.size;
		sb->CRAMFS_SB_BLOCKS=super.fsid.blocks;
		sb->CRAMFS_SB_FILES=super.fsid.files;
	} else {
		sb->CRAMFS_SB_SIZE=1<<28;
		sb->CRAMFS_SB_BLOCKS=0;
		sb->CRAMFS_SB_FILES=0;
	}
	sb->CRAMFS_SB_MAGIC=super.magic;
	sb->CRAMFS_SB_FLAGS=super.flags;
	if (root_offset == 0)
		printk(KERN_INFO "cramfs: empty filesystem");
	else if (!(super.flags & CRAMFS_FLAG_SHIFTED_ROOT_OFFSET) &&
		 ((root_offset != sizeof(struct cramfs_super)) &&
		  (root_offset != 512 + sizeof(struct cramfs_super))))
	{
		printk(KERN_ERR "cramfs: bad root offset %lu\n", root_offset);
		goto out;
	}

	/* Set it all up.. */
	sb->s_op = &cramfs_ops;
	sb->s_root = d_alloc_root(get_cramfs_inode(sb, &super.root));

#ifdef CONFIG_CRAMFS_LINEAR
	/* Remap the whole filesystem now */
	iounmap(sb->CRAMFS_SB_LINEAR_VIRT_ADDR);
	printk(KERN_INFO "cramfs: linear cramfs image appears to be %lu KB in size\n",
	       sb->CRAMFS_SB_SIZE/1024);
	sb->CRAMFS_SB_LINEAR_VIRT_ADDR =
		ioremap(sb->CRAMFS_SB_LINEAR_PHYS_ADDR, sb->CRAMFS_SB_SIZE);
	if (!sb->CRAMFS_SB_LINEAR_VIRT_ADDR) {
		printk(KERN_ERR "cramfs: ioremap of the linear cramfs image failed\n");
		goto out;
	}
#endif

	retval = sb;
out:
#ifdef CONFIG_CRAMFS_LINEAR
	if (!retval && sb->CRAMFS_SB_LINEAR_VIRT_ADDR)
		iounmap(sb->CRAMFS_SB_LINEAR_VIRT_ADDR);
#endif
	return retval;
}
#endif


/* both LINEAR and BLOCK version */
static struct super_block * cramfs_read_super(struct super_block *sb, void *data, int silent)
{
	int i;
	struct cramfs_super super;
	unsigned long root_offset;
	struct super_block * retval = NULL;
	roflash_area dev_def;

	sb->s_blocksize = PAGE_CACHE_SIZE;
	sb->s_blocksize_bits = PAGE_CACHE_SHIFT;
	
	if (MAJOR(sb->s_dev) == ROFLASH_MAJOR)
	{		
		dev_def = *((roflash_area *)(roflash_get_dev(MINOR(sb->s_dev))));
		sb->CRAMFS_SB_LXB = dev_def.l_x_b;  
	}
	else  //cramfs is being mounted on DOC //
		sb->CRAMFS_SB_LXB = ROFLASH_BLOCK;

	if (sb->CRAMFS_SB_LXB == ROFLASH_CHAR)
	{
		printk(KERN_NOTICE "cramfs_read_super: mounts on CHAR\n");
		printk(KERN_NOTICE "cramfs_read_super: wrong device minor number\n");
		return retval;
	}
	if (sb->CRAMFS_SB_LXB == ROFLASH_BLOCK)  
	{
		printk(KERN_NOTICE "cramfs_read_super: mounts on(%x),BLOCK\n",sb->s_dev);
		set_blocksize(sb->s_dev, PAGE_CACHE_SIZE);

		/* Invalidate the read buffers on mount: think disk change.. */
		for (i = 0; i < READ_BUFFERS; i++)
			buffer_blocknr[i] = -1;
		sb->CRAMFS_SB_LINEAR_PHYS_ADDR = 0xffffffff;
		sb->CRAMFS_SB_LINEAR_VIRT_ADDR = (char *)0xffffffff;
	}
	if ( (sb->CRAMFS_SB_LXB == ROFLASH_LINEAR) || (sb->CRAMFS_SB_LXB == ROFLASH_LINEAR_XIP) )  // this cramfs region is linear or linear&&xip 
	{
		/*
		 * The physical location of the cramfs image is specified as
		 * a mount parameter.  This parameter is mandatory for obvious
		 * reasons.  Some validation is made on the phys address but this
		 * is not exhaustive and we count on the fact that someone using
		 * this feature is supposed to know what he/she's doing.
		 */
		#if (0)
		if (!data || !(p = strstr((char *)data, "physaddr="))) {
			printk(KERN_ERR "cramfs: unknown physical address for linear cramfs image\n");
			goto out;
		}
		sb->CRAMFS_SB_LINEAR_PHYS_ADDR = simple_strtoul(p + 9, NULL, 0);
		#endif
		
		printk(KERN_NOTICE "cramfs_read_super: mounts on(%x),LINEAR(%x)\n",sb->s_dev,sb->CRAMFS_SB_LXB);
		
		sb->CRAMFS_SB_LINEAR_PHYS_ADDR = dev_def.offset + dev_def.phys_addr;
		if (sb->CRAMFS_SB_LINEAR_PHYS_ADDR & (PAGE_SIZE-1)) {
			printk(KERN_ERR "cramfs: physical address 0x%lx for linear cramfs isn't aligned to a page boundary\n",
			       sb->CRAMFS_SB_LINEAR_PHYS_ADDR);
			goto out;
		}
		if (sb->CRAMFS_SB_LINEAR_PHYS_ADDR == 0) {
			printk(KERN_ERR "cramfs: physical address for linear cramfs image can't be 0\n");
			goto out;
		}
		printk(KERN_INFO "cramfs: checking physical address 0x%lx for linear cramfs image\n",
		       sb->CRAMFS_SB_LINEAR_PHYS_ADDR);

		#if (0)
		/* Map only one page for now.  Will remap it when fs size is known. */
		sb->CRAMFS_SB_LINEAR_VIRT_ADDR =
			ioremap(sb->CRAMFS_SB_LINEAR_PHYS_ADDR, PAGE_SIZE);
		#endif
		
		sb->CRAMFS_SB_LINEAR_VIRT_ADDR = (char *)((unsigned long)(dev_def.priv_map) + dev_def.offset); 
		if (!sb->CRAMFS_SB_LINEAR_VIRT_ADDR) {
			printk(KERN_ERR "cramfs: ioremap of the linear cramfs image failed\n");
			goto out;
		}
	}

	down(&read_mutex);
	/* Read the first block and get the superblock from it */
	memcpy(&super, cramfs_read(sb, 0, sizeof(super)), sizeof(super));
	up(&read_mutex);

	/* Do sanity checks on the superblock */
	if (super.magic != CRAMFS_MAGIC) {
		printk("at offset 0, no cramfs magic: trying 512...\n");
		/* check at 512 byte offset */
		memcpy(&super, cramfs_read(sb, 512, sizeof(super)), sizeof(super));
		if (super.magic != CRAMFS_MAGIC) {
			printk(KERN_ERR "cramfs: wrong magic\n");
			goto out;
		}
	}

	/* get feature flags first */
	if (super.flags & ~CRAMFS_SUPPORTED_FLAGS) {
		printk(KERN_ERR "cramfs: unsupported filesystem features\n");
		goto out;
	}

	/* Check that the root inode is in a sane state */
	if (!S_ISDIR(super.root.mode)) {
		printk(KERN_ERR "cramfs: root is not a directory\n");
		goto out;
	}
	root_offset = super.root.offset << 2;
	if (super.flags & CRAMFS_FLAG_FSID_VERSION_2) {
		sb->CRAMFS_SB_SIZE=super.size;
		sb->CRAMFS_SB_BLOCKS=super.fsid.blocks;
		sb->CRAMFS_SB_FILES=super.fsid.files;
	} else {
		sb->CRAMFS_SB_SIZE=1<<28;
		sb->CRAMFS_SB_BLOCKS=0;
		sb->CRAMFS_SB_FILES=0;
	}
	sb->CRAMFS_SB_MAGIC=super.magic;
	sb->CRAMFS_SB_FLAGS=super.flags;
	if (root_offset == 0)
		printk(KERN_INFO "cramfs: empty filesystem");
	else if (!(super.flags & CRAMFS_FLAG_SHIFTED_ROOT_OFFSET) &&
		 ((root_offset != sizeof(struct cramfs_super)) &&
		  (root_offset != 512 + sizeof(struct cramfs_super))))
	{
		printk(KERN_ERR "cramfs: bad root offset %lu\n", root_offset);
		goto out;
	}

	/* Set it all up.. */
	sb->s_op = &cramfs_ops;
	sb->s_root = d_alloc_root(get_cramfs_inode(sb, &super.root));

#if (0)
#ifdef CONFIG_CRAMFS_LINEAR
	/* Remap the whole filesystem now */
	iounmap(sb->CRAMFS_SB_LINEAR_VIRT_ADDR);
	printk(KERN_INFO "cramfs: linear cramfs image appears to be %lu KB in size\n",
	       sb->CRAMFS_SB_SIZE/1024);
	sb->CRAMFS_SB_LINEAR_VIRT_ADDR =
		ioremap(sb->CRAMFS_SB_LINEAR_PHYS_ADDR, sb->CRAMFS_SB_SIZE);
	if (!sb->CRAMFS_SB_LINEAR_VIRT_ADDR) {
		printk(KERN_ERR "cramfs: ioremap of the linear cramfs image failed\n");
		goto out;
	}
#endif
#endif  //ss
	retval = sb;
out:
#if (0)
#ifdef CONFIG_CRAMFS_LINEAR
	if (!retval && sb->CRAMFS_SB_LINEAR_VIRT_ADDR)
		iounmap(sb->CRAMFS_SB_LINEAR_VIRT_ADDR);
#endif
#endif  //ss
	return retval;
}

static int cramfs_statfs(struct super_block *sb, struct statfs *buf)
{
	buf->f_type = CRAMFS_MAGIC;
	buf->f_bsize = PAGE_CACHE_SIZE;
	buf->f_blocks = sb->CRAMFS_SB_BLOCKS;
	buf->f_bfree = 0;
	buf->f_bavail = 0;
	buf->f_files = sb->CRAMFS_SB_FILES;
	buf->f_ffree = 0;
	buf->f_namelen = 255;
	return 0;
}

/*
 * Read a cramfs directory entry.
 */
static int cramfs_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct inode *inode = filp->f_dentry->d_inode;
	struct super_block *sb = inode->i_sb;
	unsigned int offset;
	int copied;

	/* Offset within the thing. */
	offset = filp->f_pos;
	if (offset >= inode->i_size)
		return 0;
	/* Directory entries are always 4-byte aligned */
	if (offset & 3)
		return -EINVAL;

	copied = 0;
	while (offset < inode->i_size) {
		struct cramfs_inode *de;
		unsigned long nextoffset;
		char *name;
		int namelen, error;

		down(&read_mutex);
		de = cramfs_read(sb, OFFSET(inode) + offset, sizeof(*de)+256);
		up(&read_mutex);
		name = (char *)(de+1);

		/*
		 * Namelengths on disk are shifted by two
		 * and the name padded out to 4-byte boundaries
		 * with zeroes.
		 */
		namelen = de->namelen << 2;
		nextoffset = offset + sizeof(*de) + namelen;
		for (;;) {
			if (!namelen)
				return -EIO;
			if (name[namelen-1])
				break;
			namelen--;
		}
		error = filldir(dirent, name, namelen, offset, CRAMINO(de), de->mode >> 12);
		if (error)
			break;

		offset = nextoffset;
		filp->f_pos = offset;
		copied++;
	}
	return 0;
}

/*
 * Lookup and fill in the inode data..
 */
static struct dentry * cramfs_lookup(struct inode *dir, struct dentry *dentry)
{
	unsigned int offset = 0;
	int sorted = dir->i_sb->CRAMFS_SB_FLAGS & CRAMFS_FLAG_SORTED_DIRS;

	while (offset < dir->i_size) {
		struct cramfs_inode *de;
		char *name;
		int namelen, retval;

		down(&read_mutex);
		de = cramfs_read(dir->i_sb, OFFSET(dir) + offset, sizeof(*de)+256);
		up(&read_mutex);
		name = (char *)(de+1);

		/* Try to take advantage of sorted directories */
		if (sorted && (dentry->d_name.name[0] < name[0]))
			break;

		namelen = de->namelen << 2;
		offset += sizeof(*de) + namelen;

		/* Quick check that the name is roughly the right length */
		if (((dentry->d_name.len + 3) & ~3) != namelen)
			continue;

		for (;;) {
			if (!namelen)
				return ERR_PTR(-EIO);
			if (name[namelen-1])
				break;
			namelen--;
		}
		if (namelen != dentry->d_name.len)
			continue;
		retval = memcmp(dentry->d_name.name, name, namelen);
		if (retval > 0)
			continue;
		if (!retval) {
			d_add(dentry, get_cramfs_inode(dir->i_sb, de));
			return NULL;
		}
		/* else (retval < 0) */
		if (sorted)
			break;
	}
	d_add(dentry, NULL);
	return NULL;
}

static int cramfs_readpage(struct file *file, struct page * page)
{
	struct inode *inode = page->mapping->host;
	u32 maxblock, bytes_filled;
	void *pgdata;

	maxblock = (inode->i_size + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	bytes_filled = 0;
	if (page->index < maxblock)
	{
		struct super_block *sb = inode->i_sb;
		u32 blkptr_offset;
		u32 start_offset, compr_len;

		if ( (sb->CRAMFS_SB_LXB == ROFLASH_LINEAR_XIP) && (CRAMFS_INODE_IS_XIP(inode)) )
		{
			blkptr_offset = PAGE_ALIGN(OFFSET(inode)) + page->index * PAGE_CACHE_SIZE;
			down(&read_mutex);
			memcpy(page_address(page),cramfs_read(sb, blkptr_offset, PAGE_CACHE_SIZE),PAGE_CACHE_SIZE);
			up(&read_mutex);
			bytes_filled = PAGE_CACHE_SIZE;
			pgdata = kmap(page);
		} 
		else 
		{
			blkptr_offset = OFFSET(inode) + page->index*4;
			start_offset = OFFSET(inode) + maxblock*4;
			down(&read_mutex);
			if (page->index)
				start_offset = *(u32 *) cramfs_read(sb, blkptr_offset-4, 4);
			compr_len = (*(u32 *) cramfs_read(sb, blkptr_offset, 4) - start_offset);
			up(&read_mutex);
			pgdata = kmap(page);
			if (compr_len == 0)
				; /* hole */
			else 
			{
				down(&read_mutex);
				bytes_filled = cramfs_uncompress_block(pgdata,
					 PAGE_CACHE_SIZE,
					 cramfs_read(sb, start_offset, compr_len),
					 compr_len);
				up(&read_mutex);
			}
		}
	} 
	else
		pgdata = kmap(page);
	memset(pgdata + bytes_filled, 0, PAGE_CACHE_SIZE - bytes_filled);
	kunmap(page);
	flush_dcache_page(page);
	SetPageUptodate(page);
	UnlockPage(page);
	return 0;
}

static struct address_space_operations cramfs_aops = {
	readpage: cramfs_readpage
};

/*
 * Our operations:
 */

/*
 * A directory can only readdir
 */
static struct file_operations cramfs_directory_operations = {
	read:		generic_read_dir,
	readdir:	cramfs_readdir,
};

static struct inode_operations cramfs_dir_inode_operations = {
	lookup:		cramfs_lookup,
};

static struct super_operations cramfs_ops = {
	statfs:		cramfs_statfs,
};

//ss  #ifndef CONFIG_CRAMFS_LINEAR
static DECLARE_FSTYPE_DEV(cramfs_fs_type, "cramfs", cramfs_read_super);
//ss  #else
//ss  static DECLARE_FSTYPE(cramfs_fs_type, "cramfs", cramfs_read_super, 0);
//ss  #endif

static int __init init_cramfs_fs(void)
{
	cramfs_uncompress_init();
	create_cramfs_proc_entry();
	return register_filesystem(&cramfs_fs_type);
}

static void __exit exit_cramfs_fs(void)
{
	remove_cramfs_proc_entry();
	cramfs_uncompress_exit();
	unregister_filesystem(&cramfs_fs_type);
}

module_init(init_cramfs_fs)
module_exit(exit_cramfs_fs)
MODULE_LICENSE("GPL");


#else

#define CRAMFS_SB_MAGIC u.cramfs_sb.magic
#define CRAMFS_SB_SIZE u.cramfs_sb.size
#define CRAMFS_SB_BLOCKS u.cramfs_sb.blocks
#define CRAMFS_SB_FILES u.cramfs_sb.files
#define CRAMFS_SB_FLAGS u.cramfs_sb.flags
#define CRAMFS_SB_LINEAR_PHYS_ADDR u.cramfs_sb.linear_phys_addr
#define CRAMFS_SB_LINEAR_VIRT_ADDR u.cramfs_sb.linear_virt_addr

static struct super_operations cramfs_ops;
static struct inode_operations cramfs_dir_inode_operations;
static struct file_operations cramfs_directory_operations;
static struct address_space_operations cramfs_aops;

static DECLARE_MUTEX(read_mutex);


/* These two macros may change in future, to provide better st_ino
   semantics. */
#define CRAMINO(x)	((x)->offset?(x)->offset<<2:1)
#define OFFSET(x)	((x)->i_ino)


#ifdef CONFIG_CRAMFS_LINEAR_XIP

static int cramfs_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long address, length;
	struct inode *inode = file->f_dentry->d_inode;
	struct super_block *sb = inode->i_sb;

	/* this is only used in the case of read-only maps for XIP */

	if (vma->vm_flags & VM_WRITE)
		return generic_file_mmap(file, vma);

	if ((vma->vm_flags & VM_SHARED) && (vma->vm_flags & VM_MAYWRITE))
		return -EINVAL;

	address  = PAGE_ALIGN(sb->CRAMFS_SB_LINEAR_PHYS_ADDR + OFFSET(inode));
	address += vma->vm_pgoff << PAGE_SHIFT;

	length = vma->vm_end - vma->vm_start;

	if (length > inode->i_size)
		length = inode->i_size;

	length = PAGE_ALIGN(length);


#if 0
	/* Doing the following makes it slower and more broken.  bdl */
	/*
	 * Accessing memory above the top the kernel knows about or
	 * through a file pointer that was marked O_SYNC will be
	 * done non-cached.
	 */
	vma->vm_page_prot =
		__pgprot((pgprot_val(vma->vm_page_prot) & ~_CACHE_MASK)
			| _CACHE_UNCACHED);
#endif

	/*
	 * Don't dump addresses that are not real memory to a core file.
	 */
	vma->vm_flags |= (VM_IO | VM_XIP);
	flush_tlb_page(vma, address);
	if (remap_page_range(vma->vm_start, address, length,
			     vma->vm_page_prot))
		return -EAGAIN;

#ifdef DEBUG_CRAMFS_XIP
	printk("cramfs_mmap: mapped %s at 0x%08lx, length %lu to vma 0x%08lx"
		", page_prot 0x%08lx\n",
		file->f_dentry->d_name.name, address, length,
		vma->vm_start, pgprot_val(vma->vm_page_prot));
#endif

	return 0;
}

static struct file_operations cramfs_linear_xip_fops = {
	read:	generic_file_read,
	mmap:	cramfs_mmap,
};

#define CRAMFS_INODE_IS_XIP(x) ((x)->i_mode & S_ISVTX)

#endif

static struct inode *get_cramfs_inode(struct super_block *sb, struct cramfs_inode * cramfs_inode)
{
	struct inode * inode = new_inode(sb);

	if (inode) {
		inode->i_mode = cramfs_inode->mode;
		inode->i_uid = cramfs_inode->uid;
		inode->i_size = cramfs_inode->size;
		inode->i_blocks = (cramfs_inode->size - 1) / 512 + 1;
		inode->i_blksize = PAGE_CACHE_SIZE;
		inode->i_gid = cramfs_inode->gid;
		inode->i_ino = CRAMINO(cramfs_inode);
		/* inode->i_nlink is left 1 - arguably wrong for directories,
		   but it's the best we can do without reading the directory
	           contents.  1 yields the right result in GNU find, even
		   without -noleaf option. */
		insert_inode_hash(inode);
		if (S_ISREG(inode->i_mode)) {
#ifdef CONFIG_CRAMFS_LINEAR_XIP
			inode->i_fop = CRAMFS_INODE_IS_XIP(inode) ? &cramfs_linear_xip_fops : &generic_ro_fops;
#else
			inode->i_fop = &generic_ro_fops;
#endif
			inode->i_data.a_ops = &cramfs_aops;
		} else if (S_ISDIR(inode->i_mode)) {
			inode->i_op = &cramfs_dir_inode_operations;
			inode->i_fop = &cramfs_directory_operations;
		} else if (S_ISLNK(inode->i_mode)) {
			inode->i_op = &page_symlink_inode_operations;
			inode->i_data.a_ops = &cramfs_aops;
		} else {
			inode->i_size = 0;
			init_special_inode(inode, inode->i_mode, cramfs_inode->size);
		}
	}
	return inode;
}

#ifdef CONFIG_CRAMFS_LINEAR
/*
 * Return a pointer to the block in the linearly addressed cramfs image.
 */
static void *cramfs_read(struct super_block *sb, unsigned int offset, unsigned int len)
{
	if (!len)
		return NULL;
	return (void*)(sb->CRAMFS_SB_LINEAR_VIRT_ADDR + offset);
}
#else /* Not linear addressing - aka regular block mode. */
/*
 * We have our own block cache: don't fill up the buffer cache
 * with the rom-image, because the way the filesystem is set
 * up the accesses should be fairly regular and cached in the
 * page cache and dentry tree anyway..
 *
 * This also acts as a way to guarantee contiguous areas of up to
 * BLKS_PER_BUF*PAGE_CACHE_SIZE, so that the caller doesn't need to
 * worry about end-of-buffer issues even when decompressing a full
 * page cache.
 */
#define READ_BUFFERS (2)
/* NEXT_BUFFER(): Loop over [0..(READ_BUFFERS-1)]. */
#define NEXT_BUFFER(_ix) ((_ix) ^ 1)

/*
 * BLKS_PER_BUF_SHIFT should be at least 2 to allow for "compressed"
 * data that takes up more space than the original and with unlucky
 * alignment.
 */
#define BLKS_PER_BUF_SHIFT	(2)
#define BLKS_PER_BUF		(1 << BLKS_PER_BUF_SHIFT)
#define BUFFER_SIZE		(BLKS_PER_BUF*PAGE_CACHE_SIZE)

static unsigned char read_buffers[READ_BUFFERS][BUFFER_SIZE];
static unsigned buffer_blocknr[READ_BUFFERS];
static struct super_block * buffer_dev[READ_BUFFERS];
static int next_buffer;

/*
 * Returns a pointer to a buffer containing at least LEN bytes of
 * filesystem starting at byte offset OFFSET into the filesystem.
 */
static void *cramfs_read(struct super_block *sb, unsigned int offset, unsigned int len)
{
	struct buffer_head * bh_array[BLKS_PER_BUF];
	struct buffer_head * read_array[BLKS_PER_BUF];
	unsigned i, blocknr, buffer, unread;
	unsigned long devsize;
	int major, minor;
	
	char *data;

	if (!len)
		return NULL;
	blocknr = offset >> PAGE_CACHE_SHIFT;
	offset &= PAGE_CACHE_SIZE - 1;

	/* Check if an existing buffer already has the data.. */
	for (i = 0; i < READ_BUFFERS; i++) {
		unsigned int blk_offset;

		if (buffer_dev[i] != sb)
			continue;
		if (blocknr < buffer_blocknr[i])
			continue;
		blk_offset = (blocknr - buffer_blocknr[i]) << PAGE_CACHE_SHIFT;
		blk_offset += offset;
		if (blk_offset + len > BUFFER_SIZE)
			continue;
		return read_buffers[i] + blk_offset;
	}

	devsize = ~0UL;
	major = MAJOR(sb->s_dev);
	minor = MINOR(sb->s_dev);

	if (blk_size[major])
		devsize = blk_size[major][minor] >> 2;

	/* Ok, read in BLKS_PER_BUF pages completely first. */
	unread = 0;
	for (i = 0; i < BLKS_PER_BUF; i++) {
		struct buffer_head *bh;

		bh = NULL;
		if (blocknr + i < devsize) {
			bh = sb_getblk(sb, blocknr + i);
			if (!buffer_uptodate(bh))
				read_array[unread++] = bh;
		}
		bh_array[i] = bh;
	}

	if (unread) {
		ll_rw_block(READ, unread, read_array);
		do {
			unread--;
			wait_on_buffer(read_array[unread]);
		} while (unread);
	}

	/* Ok, copy them to the staging area without sleeping. */
	buffer = next_buffer;
	next_buffer = NEXT_BUFFER(buffer);
	buffer_blocknr[buffer] = blocknr;
	buffer_dev[buffer] = sb;

	data = read_buffers[buffer];
	for (i = 0; i < BLKS_PER_BUF; i++) {
		struct buffer_head * bh = bh_array[i];
		if (bh) {
			memcpy(data, bh->b_data, PAGE_CACHE_SIZE);
			brelse(bh);
		} else
			memset(data, 0, PAGE_CACHE_SIZE);
		data += PAGE_CACHE_SIZE;
	}
	return read_buffers[buffer] + offset;
}
#endif

static struct super_block * cramfs_read_super(struct super_block *sb, void *data, int silent)
{
#ifndef CONFIG_CRAMFS_LINEAR
	int i;
#else
	char *p;
#endif
	struct cramfs_super super;
	unsigned long root_offset;
	struct super_block * retval = NULL;

	sb->s_blocksize = PAGE_CACHE_SIZE;
	sb->s_blocksize_bits = PAGE_CACHE_SHIFT;

#ifndef CONFIG_CRAMFS_LINEAR

	set_blocksize(sb->s_dev, PAGE_CACHE_SIZE);

	/* Invalidate the read buffers on mount: think disk change.. */
	for (i = 0; i < READ_BUFFERS; i++)
		buffer_blocknr[i] = -1;

#else

	/*
	 * The physical location of the cramfs image is specified as
	 * a mount parameter.  This parameter is mandatory for obvious
	 * reasons.  Some validation is made on the phys address but this
	 * is not exhaustive and we count on the fact that someone using
	 * this feature is supposed to know what he/she's doing.
	 */
	if (!data || !(p = strstr((char *)data, "physaddr="))) {
		printk(KERN_ERR "cramfs: unknown physical address for linear cramfs image\n");
		goto out;
	}
	sb->CRAMFS_SB_LINEAR_PHYS_ADDR = simple_strtoul(p + 9, NULL, 0);
	if (sb->CRAMFS_SB_LINEAR_PHYS_ADDR & (PAGE_SIZE-1)) {
		printk(KERN_ERR "cramfs: physical address 0x%lx for linear cramfs isn't aligned to a page boundary\n",
		       sb->CRAMFS_SB_LINEAR_PHYS_ADDR);
		goto out;
	}
	if (sb->CRAMFS_SB_LINEAR_PHYS_ADDR == 0) {
		printk(KERN_ERR "cramfs: physical address for linear cramfs image can't be 0\n");
		goto out;
	}
	printk(KERN_INFO "cramfs: checking physical address 0x%lx for linear cramfs image\n",
	       sb->CRAMFS_SB_LINEAR_PHYS_ADDR);

	/* Map only one page for now.  Will remap it when fs size is known. */
	sb->CRAMFS_SB_LINEAR_VIRT_ADDR =
		ioremap(sb->CRAMFS_SB_LINEAR_PHYS_ADDR, PAGE_SIZE);
	if (!sb->CRAMFS_SB_LINEAR_VIRT_ADDR) {
		printk(KERN_ERR "cramfs: ioremap of the linear cramfs image failed\n");
		goto out;
	}
#endif

	down(&read_mutex);
	/* Read the first block and get the superblock from it */
	memcpy(&super, cramfs_read(sb, 0, sizeof(super)), sizeof(super));
	up(&read_mutex);

	/* Do sanity checks on the superblock */
	if (super.magic != CRAMFS_MAGIC) {
		printk("at offset 0, no cramfs magic: trying 512...\n");
		/* check at 512 byte offset */
		memcpy(&super, cramfs_read(sb, 512, sizeof(super)), sizeof(super));
		if (super.magic != CRAMFS_MAGIC) {
			printk(KERN_ERR "cramfs: wrong magic\n");
			goto out;
		}
	}

	/* get feature flags first */
	if (super.flags & ~CRAMFS_SUPPORTED_FLAGS) {
		printk(KERN_ERR "cramfs: unsupported filesystem features\n");
		goto out;
	}

	/* Check that the root inode is in a sane state */
	if (!S_ISDIR(super.root.mode)) {
		printk(KERN_ERR "cramfs: root is not a directory\n");
		goto out;
	}
	root_offset = super.root.offset << 2;
	if (super.flags & CRAMFS_FLAG_FSID_VERSION_2) {
		sb->CRAMFS_SB_SIZE=super.size;
		sb->CRAMFS_SB_BLOCKS=super.fsid.blocks;
		sb->CRAMFS_SB_FILES=super.fsid.files;
	} else {
		sb->CRAMFS_SB_SIZE=1<<28;
		sb->CRAMFS_SB_BLOCKS=0;
		sb->CRAMFS_SB_FILES=0;
	}
	sb->CRAMFS_SB_MAGIC=super.magic;
	sb->CRAMFS_SB_FLAGS=super.flags;
	if (root_offset == 0)
		printk(KERN_INFO "cramfs: empty filesystem");
	else if (!(super.flags & CRAMFS_FLAG_SHIFTED_ROOT_OFFSET) &&
		 ((root_offset != sizeof(struct cramfs_super)) &&
		  (root_offset != 512 + sizeof(struct cramfs_super))))
	{
		printk(KERN_ERR "cramfs: bad root offset %lu\n", root_offset);
		goto out;
	}

	/* Set it all up.. */
	sb->s_op = &cramfs_ops;
	sb->s_root = d_alloc_root(get_cramfs_inode(sb, &super.root));

#ifdef CONFIG_CRAMFS_LINEAR
	/* Remap the whole filesystem now */
	iounmap(sb->CRAMFS_SB_LINEAR_VIRT_ADDR);
	printk(KERN_INFO "cramfs: linear cramfs image appears to be %lu KB in size\n",
	       sb->CRAMFS_SB_SIZE/1024);
#ifdef CONFIG_ARM
	sb->CRAMFS_SB_LINEAR_VIRT_ADDR =
		__ioremap(sb->CRAMFS_SB_LINEAR_PHYS_ADDR, sb->CRAMFS_SB_SIZE,
			  L_PTE_CACHEABLE);
#else
	sb->CRAMFS_SB_LINEAR_VIRT_ADDR =
		ioremap(sb->CRAMFS_SB_LINEAR_PHYS_ADDR, sb->CRAMFS_SB_SIZE);
#endif
	if (!sb->CRAMFS_SB_LINEAR_VIRT_ADDR) {
		printk(KERN_ERR "cramfs: ioremap of the linear cramfs image failed\n");
		goto out;
	}
#endif

	retval = sb;
out:
#ifdef CONFIG_CRAMFS_LINEAR
	if (!retval && sb->CRAMFS_SB_LINEAR_VIRT_ADDR)
		iounmap(sb->CRAMFS_SB_LINEAR_VIRT_ADDR);
#endif
	return retval;
}

static int cramfs_statfs(struct super_block *sb, struct statfs *buf)
{
	buf->f_type = CRAMFS_MAGIC;
	buf->f_bsize = PAGE_CACHE_SIZE;
	buf->f_blocks = sb->CRAMFS_SB_BLOCKS;
	buf->f_bfree = 0;
	buf->f_bavail = 0;
	buf->f_files = sb->CRAMFS_SB_FILES;
	buf->f_ffree = 0;
	buf->f_namelen = 255;
	return 0;
}

/*
 * Read a cramfs directory entry.
 */
static int cramfs_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct inode *inode = filp->f_dentry->d_inode;
	struct super_block *sb = inode->i_sb;
	unsigned int offset;
	int copied;

	/* Offset within the thing. */
	offset = filp->f_pos;
	if (offset >= inode->i_size)
		return 0;
	/* Directory entries are always 4-byte aligned */
	if (offset & 3)
		return -EINVAL;

	copied = 0;
	while (offset < inode->i_size) {
		struct cramfs_inode *de;
		unsigned long nextoffset;
		char *name;
		int namelen, error;

		down(&read_mutex);
		de = cramfs_read(sb, OFFSET(inode) + offset, sizeof(*de)+256);
		up(&read_mutex);
		name = (char *)(de+1);

		/*
		 * Namelengths on disk are shifted by two
		 * and the name padded out to 4-byte boundaries
		 * with zeroes.
		 */
		namelen = de->namelen << 2;
		nextoffset = offset + sizeof(*de) + namelen;
		for (;;) {
			if (!namelen)
				return -EIO;
			if (name[namelen-1])
				break;
			namelen--;
		}
		error = filldir(dirent, name, namelen, offset, CRAMINO(de), de->mode >> 12);
		if (error)
			break;

		offset = nextoffset;
		filp->f_pos = offset;
		copied++;
	}
	return 0;
}

/*
 * Lookup and fill in the inode data..
 */
static struct dentry * cramfs_lookup(struct inode *dir, struct dentry *dentry)
{
	unsigned int offset = 0;
	int sorted = dir->i_sb->CRAMFS_SB_FLAGS & CRAMFS_FLAG_SORTED_DIRS;

	while (offset < dir->i_size) {
		struct cramfs_inode *de;
		char *name;
		int namelen, retval;

		down(&read_mutex);
		de = cramfs_read(dir->i_sb, OFFSET(dir) + offset, sizeof(*de)+256);
		up(&read_mutex);
		name = (char *)(de+1);

		/* Try to take advantage of sorted directories */
		if (sorted && (dentry->d_name.name[0] < name[0]))
			break;

		namelen = de->namelen << 2;
		offset += sizeof(*de) + namelen;

		/* Quick check that the name is roughly the right length */
		if (((dentry->d_name.len + 3) & ~3) != namelen)
			continue;

		for (;;) {
			if (!namelen)
				return ERR_PTR(-EIO);
			if (name[namelen-1])
				break;
			namelen--;
		}
		if (namelen != dentry->d_name.len)
			continue;
		retval = memcmp(dentry->d_name.name, name, namelen);
		if (retval > 0)
			continue;
		if (!retval) {
			d_add(dentry, get_cramfs_inode(dir->i_sb, de));
			return NULL;
		}
		/* else (retval < 0) */
		if (sorted)
			break;
	}
	d_add(dentry, NULL);
	return NULL;
}

static int cramfs_readpage(struct file *file, struct page * page)
{
	struct inode *inode = page->mapping->host;
	u32 maxblock, bytes_filled;
	void *pgdata;

	maxblock = (inode->i_size + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	bytes_filled = 0;
	if (page->index < maxblock) {
		struct super_block *sb = inode->i_sb;
		u32 blkptr_offset;
		u32 start_offset, compr_len;

#ifdef CONFIG_CRAMFS_LINEAR_XIP
		if(CRAMFS_INODE_IS_XIP(inode)) {
			blkptr_offset = 
				PAGE_ALIGN(OFFSET(inode)) + 
				page->index * PAGE_CACHE_SIZE;
			down(&read_mutex);
			memcpy(page_address(page),
				cramfs_read(sb, blkptr_offset, PAGE_CACHE_SIZE),
				PAGE_CACHE_SIZE);
			up(&read_mutex);
			bytes_filled = PAGE_CACHE_SIZE;
			pgdata = kmap(page);
		} else {
#endif
			blkptr_offset = OFFSET(inode) + page->index*4;
			start_offset = OFFSET(inode) + maxblock*4;
			down(&read_mutex);
			if (page->index)
				start_offset = *(u32 *) cramfs_read(sb, blkptr_offset-4, 4);
			compr_len = (*(u32 *) cramfs_read(sb, blkptr_offset, 4) - start_offset);
			up(&read_mutex);
			pgdata = kmap(page);
			if (compr_len == 0)
				; /* hole */
			else {
				down(&read_mutex);
				bytes_filled = cramfs_uncompress_block(pgdata,
					 PAGE_CACHE_SIZE,
					 cramfs_read(sb, start_offset, compr_len),
					 compr_len);
				up(&read_mutex);
			}
#ifdef CONFIG_CRAMFS_LINEAR_XIP
		}
#endif
	} else
		pgdata = kmap(page);
	memset(pgdata + bytes_filled, 0, PAGE_CACHE_SIZE - bytes_filled);
	kunmap(page);
	flush_dcache_page(page);
	SetPageUptodate(page);
	UnlockPage(page);
	return 0;
}

static struct address_space_operations cramfs_aops = {
	readpage: cramfs_readpage
};

/*
 * Our operations:
 */

/*
 * A directory can only readdir
 */
static struct file_operations cramfs_directory_operations = {
	read:		generic_read_dir,
	readdir:	cramfs_readdir,
};

static struct inode_operations cramfs_dir_inode_operations = {
	lookup:		cramfs_lookup,
};

static struct super_operations cramfs_ops = {
	statfs:		cramfs_statfs,
};

#ifndef CONFIG_CRAMFS_LINEAR
static DECLARE_FSTYPE_DEV(cramfs_fs_type, "cramfs", cramfs_read_super);
#else
static DECLARE_FSTYPE(cramfs_fs_type, "cramfs", cramfs_read_super, 0);
#endif

static int __init init_cramfs_fs(void)
{
	cramfs_uncompress_init();
	create_cramfs_proc_entry();
	return register_filesystem(&cramfs_fs_type);
}

static void __exit exit_cramfs_fs(void)
{
	remove_cramfs_proc_entry();
	cramfs_uncompress_exit();
	unregister_filesystem(&cramfs_fs_type);
}

module_init(init_cramfs_fs)
module_exit(exit_cramfs_fs)
MODULE_LICENSE("GPL");

#endif

