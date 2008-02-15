/*
 * linux/drivers/usb/device/serial_fd/crc10.c 
 *
 * Copyright (c) 2002 Belcarra
 * Copyright (c) 2000, 2001, 2002 Lineo
 * Copyright (c) 2001 Hewlett Packard
 *
 * By: 
 *      Stuart Lynne <sl@belcarra.com>, 
 *      Tom Rushworth <tbr@belcarra.com>, 
 *      Bruce Balden <balden@belcarra.com>
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 */

#include <linux/types.h>

__u16 crc10_table[256] = {
	0x000, 0x233, 0x255, 0x066, 0x299, 0x0aa, 0x0cc, 0x2ff, 0x301, 0x132, 0x154, 0x367, 0x198, 0x3ab, 0x3cd, 0x1fe,
	0x031, 0x202, 0x264, 0x057, 0x2a8, 0x09b, 0x0fd, 0x2ce, 0x330, 0x103, 0x165, 0x356, 0x1a9, 0x39a, 0x3fc, 0x1cf,
	0x062, 0x251, 0x237, 0x004, 0x2fb, 0x0c8, 0x0ae, 0x29d, 0x363, 0x150, 0x136, 0x305, 0x1fa, 0x3c9, 0x3af, 0x19c,
	0x053, 0x260, 0x206, 0x035, 0x2ca, 0x0f9, 0x09f, 0x2ac, 0x352, 0x161, 0x107, 0x334, 0x1cb, 0x3f8, 0x39e, 0x1ad,
	0x0c4, 0x2f7, 0x291, 0x0a2, 0x25d, 0x06e, 0x008, 0x23b, 0x3c5, 0x1f6, 0x190, 0x3a3, 0x15c, 0x36f, 0x309, 0x13a,
	0x0f5, 0x2c6, 0x2a0, 0x093, 0x26c, 0x05f, 0x039, 0x20a, 0x3f4, 0x1c7, 0x1a1, 0x392, 0x16d, 0x35e, 0x338, 0x10b,
	0x0a6, 0x295, 0x2f3, 0x0c0, 0x23f, 0x00c, 0x06a, 0x259, 0x3a7, 0x194, 0x1f2, 0x3c1, 0x13e, 0x30d, 0x36b, 0x158,
	0x097, 0x2a4, 0x2c2, 0x0f1, 0x20e, 0x03d, 0x05b, 0x268, 0x396, 0x1a5, 0x1c3, 0x3f0, 0x10f, 0x33c, 0x35a, 0x169,
	0x188, 0x3bb, 0x3dd, 0x1ee, 0x311, 0x122, 0x144, 0x377, 0x289, 0x0ba, 0x0dc, 0x2ef, 0x010, 0x223, 0x245, 0x076,
	0x1b9, 0x38a, 0x3ec, 0x1df, 0x320, 0x113, 0x175, 0x346, 0x2b8, 0x08b, 0x0ed, 0x2de, 0x021, 0x212, 0x274, 0x047,
	0x1ea, 0x3d9, 0x3bf, 0x18c, 0x373, 0x140, 0x126, 0x315, 0x2eb, 0x0d8, 0x0be, 0x28d, 0x072, 0x241, 0x227, 0x014,
	0x1db, 0x3e8, 0x38e, 0x1bd, 0x342, 0x171, 0x117, 0x324, 0x2da, 0x0e9, 0x08f, 0x2bc, 0x043, 0x270, 0x216, 0x025,
	0x14c, 0x37f, 0x319, 0x12a, 0x3d5, 0x1e6, 0x180, 0x3b3, 0x24d, 0x07e, 0x018, 0x22b, 0x0d4, 0x2e7, 0x281, 0x0b2,
	0x17d, 0x34e, 0x328, 0x11b, 0x3e4, 0x1d7, 0x1b1, 0x382, 0x27c, 0x04f, 0x029, 0x21a, 0x0e5, 0x2d6, 0x2b0, 0x083,
	0x12e, 0x31d, 0x37b, 0x148, 0x3b7, 0x184, 0x1e2, 0x3d1, 0x22f, 0x01c, 0x07a, 0x249, 0x0b6, 0x285, 0x2e3, 0x0d0,
	0x11f, 0x32c, 0x34a, 0x179, 0x386, 0x1b5, 0x1d3, 0x3e0, 0x21e, 0x02d, 0x04b, 0x278, 0x087, 0x2b4, 0x2d2, 0x0e1,
};