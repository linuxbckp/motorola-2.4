/*
 * linux/drivers/usbd/usbd-hotplug.h 
 *
 * Copyright (c) 2002, 2003 Belcarra
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
 */

extern int hotplug (char *name, char *interface, char *action);