/*
 * linux/drivers/usbd/usb-module.h 
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

/*
 *
 * USBD_MODULE_INFO(info) 
 *
 *
 * The info parameter should take the form:
 *
 *      "usbdcore 0.1-beta"
 *
 *      name    usbdcore
 *      major   0
 *      minor   1
 *      status  beta
 *
 * Status should be one of:
 *
 *      alpha           work in progress, preview only
 *      beta            initial testing, not for deployment
 *
 *      early           suitable for production use where new features are needed
 *      limited         suitable for new installations
 *      production      most stable
 *
 *      deprectated     no longer maintained
 *      abandoned       no longer available
 *
 * Additional information added from usbd-export.h and usbd-build.h will
 * record the build number and date exported.
 */

#if defined(MODULE)
#define USBD_MODULE_INFO(info) \
static const char __usbd_module_info[] = info " " USBD_BUILD " 03/03/25 sl_enposte_net@whiskey.enposte.net";
#else
#define USBD_MODULE_INFO(info) \
static const char __usbd_module_info[] = info " " USBD_BUILD " 03/03/25 sl_enposte_net@whiskey.enposte.net";
#endif
/*
#if defined(MODULE)
#define USBD_MODULE_INFO(info) \
static const char __usbd_module_info[] = info " " USBD_BUILD " " USBD_EXPORT_DATE;
#else
#define USBD_MODULE_INFO(info) \
static const char __usbd_module_info[] = info " " USBD_BUILD " " USBD_EXPORT_DATE;
#endif
*/

#if 0
/*
 * modname should be defined by USBD_MODULE_NAME in the including file:
 * E.g. #define USBD_MODULE_NAME usbdcore
 */
#define USBD_MODULE_VARNAME(modname) __usbd_module_info_##modname
#define USBD_MODULE_VERSION(modname,vers) \
const char USBD_MODULE_VARNAME(modname)[] = __MODULE_STRING(modname) " " vers " " USBD_BUILD " " USBD_EXPORT_DATE;
#endif
