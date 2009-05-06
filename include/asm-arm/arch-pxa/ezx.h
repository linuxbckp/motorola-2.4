/*
 * Copyright (C) 2004-2006 Motorola Inc.
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

 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
 *  linux/include/asm-arm/arch-pxa/ezx.h
 *
 *  Specific macro defines for Motorola Ezx Development Platform
 *  - GPIO definitions
 *  - Memory map
 *  - HW parameter: LCD buffer strength, Keypad, LED, etc
 *  - Product name string (for storage driver): "HAINAN" or "A1200"
 *  - Macro for special code: ???
 *  - Others: ???
 *
 *  Created:    Nov 25, 2003
 *
 *    Date          	 Description of Changes
 * -------------	 ----------------------
 *  12/15/2004		 Barbados GPIO update
 *  08/03/2005		 Martinique WLAN GPIOs
 *  04/11/2005		 Change Martinique GPIO for P1 portable
 *  04/27/2005		 Martinique GPIO re-mapping - Swap GPIO21 and GPIO33 
 *  2005-07-11		 update (barbados p4) flip gpio to 15
 *  2005-12-21		 add MACAU product
 *  03/20/2006		 support 48M RAM
 *  2006-04-11		 define 16M SDRAM chip select GPIO21
 *  2006-06-15		 add Penglai product
 *  2006-07-14		 move specific macro defines from source code to here
 *  2006-08-09		 modify memory map to support multi-product
 *  2006-09-12		 change memory map for PENGLAI project
 *  2006-11-15		 camera clk change from AP to BP, use two pins to control 
 *  2006-12-05		 Add conditional compile for 13MHz clk
 *  2008-11-11           Remapping to support WebUI in rootfs
 *  2009-02-19           Remapping to support Picsel Editor in language
*/

/*
 * Flags in memory for sleep use
 */
#define FLAG_ADDR       PHYS_OFFSET
#define RESUME_ADDR     (PHYS_OFFSET + 4)
#define BPSIG_ADDR      (PHYS_OFFSET + 8)

#define USER_OFF_FLAG   0x5a5a5a5a
#define SLEEP_FLAG      0x6b6b6b6b
#define OFF_FLAG        0x7c7c7c7c
#define REFLASH_FLAG    0x0C1D2E3F
#define PASS_THRU_FLAG	0x12345678
#define CHARGE_FLAG 	0xC0C1C2C3
#define APP_REBOOT_FLAG 0x80808080
#define FACTORY_REBOOT_FLAG	0x46414354
#define DEFAULT_FLAG	0x44454641

#define WDI_FLAG        0xbb00dead
#define NO_FLAG         0xaa00dead

/*
 * GPIO control pin, have to change when hardware lock down
 */

#if defined(CONFIG_ARCH_EZX_BARBADOS)

#define EZX_PRODUCT_NAME		"Barbados"
#define EZX_PRODUCT_NAME_STORAGE	"Barbados Phone"
/* Barbados memory map definition: start */
/*****to do*******/
/* Barbados memory map definition: end */

/* audio related gpio */
#define GPIO_NSSP_SCLK2                 22
#define GPIO_NSSP_SFRM2                 14 
#define GPIO_NSSP_RXD2                  88
#define GPIO_NSSP_TXD2                  38

#define GPIO_ASSP_SCLK3                 52
#define GPIO_ASSP_TXD3                  81
#define GPIO_ASSP_RXD3                  82
#define GPIO_ASSP_SFRM3                 83

#define GPIO_BITCLK_IN_NSSP_MD  (GPIO_NSSP_SCLK2 | GPIO_ALT_FN_3_IN)
#define GPIO_SYNC_IN_NSSP_MD    (GPIO_NSSP_SFRM2 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_IN_NSSP_MD   (GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_NSSP_MD  (GPIO_NSSP_TXD2  | GPIO_ALT_FN_2_OUT)
#define GPIO_VA_NSSP_SWAP_MD	(GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_OUT)

#define GPIO_BITCLK_IN_ASSP_MD  (GPIO_ASSP_SCLK3 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_ASSP_MD  (GPIO_ASSP_TXD3  | GPIO_ALT_FN_1_OUT)
#define GPIO_SDATA_IN_ASSP_MD   (GPIO_ASSP_RXD3  | GPIO_ALT_FN_1_IN)
#define GPIO_SYNC_IN_ASSP_MD    (GPIO_ASSP_SFRM3 | GPIO_ALT_FN_1_IN)

#define GPIO_CLI_CSB 			19
#define GPIO_LCD_CM 			79
#define GPIO_USB_READY    		99
#define GPIO_EMU_INT			10
#define GPIO_TFLASH_INT			11
#define AP_13MHZ_OUTPUT_PIN             9
#define GPIO_CLI_RESETB     		49


/* AP and BP handshake signal */
#define GPIO_BP_RDY            0       /* BP_RDY    */
#define GPIO_AP_RDY	       96      /* AP_RDY*/
#define GPIO_BB_RESET          116      /* BB_RESET   */
#define GPIO_BB_FLASHMODE_EN   41      /* notify bp to enter reflash mode  ASSERT(valid)*/

#define GPIO_BB_WDI            3      /* BB_WDI   ???  */
#define GPIO_MCU_INT_SW        41      /* MCU_INT_SW  BP flash mode enable */
#define GPIO_TC_MM_EN          120      /* TC_MM_EN  no use in barbados */

/* control PCAP direct PIN */
#define GPIO_WDI_AP            4       /* WDI_AP                       */
#define GPIO_SYS_RESTART       55      /* restart PCAP power [alias:BATT_DET_IN] */

/* communicate with PCAP's PIN  */
#define GPIO_PCAP_SEC_INT      1       /* PCAP interrupt PIN to AP     */ 
#define GPIO_SPI_CLK           29      /* PCAP SPI port clock          */ 
#define GPIO_SPI_CE            24      /* PCAP SPI port SSPFRM         */  
#define GPIO_SPI_MOSI          25      /* PCAP SPI port SSPTXD         */ 
#define GPIO_SPI_MISO          26      /* PCAP SPI port SSPRXD         */ 

/*  blue tooth control PIN   */
#define GPIO_BT_WAKEUP         57      /* AP wake up bluetooth module  */
#define GPIO_BT_HOSTWAKE       13      /* AP wake up bluetooth module  */
#define GPIO_BT_RESET          37      /* AP reset bluetooth module    */



/* control LCD high - OFF low -- ON  */
#define GPIO_LCD_OFF           78     /* control LCD                */

/*  FFUART PIN              */
/*  since barbados remove the ffuart function we swap the pin to one reserved pin 120 */
/*  we will remove the code in barbados.c in later release */
#define GPIO_ICL_FFRXD_MD      (120 )
#define GPIO_ICL_FFCTS_MD      (120 )
#define GPIO_ICL_FFTXD_MD      (120 )
#define GPIO_ICL_FFRTS_MD      (120 )

/*
 * ezx platform, wake up source edge detect bit
 */
#define PEDR_INT_SEC            1

#define GPIO_FLIP_PIN           15

/* matrix key GPIO */
#define GPIO_MATRIX_KEY_ROW0    100
#define GPIO_MATRIX_KEY_ROW1    101
#define GPIO_MATRIX_KEY_ROW2    102
#define GPIO_MATRIX_KEY_ROW3    97
#define GPIO_MATRIX_KEY_ROW4    98
#define GPIO_MATRIX_KEY_COL0    103
#define GPIO_MATRIX_KEY_COL1    104
#define GPIO_MATRIX_KEY_COL2    105
#define GPIO_MATRIX_KEY_COL3    106
#define GPIO_MATRIX_KEY_COL4    107
#define GPIO_MATRIX_KEY_COL5    108

/* MMC interface */
#define GPIO_MMC_DETECT         GPIO_TFLASH_INT 
#define GPIO_MMC_CLK            32
#define GPIO_MMC_DATA0          92
//#define GPIO_MMC_WP		107
#define GPIO_MMC_DATA1          109
#define GPIO_MMC_DATA2          110
#define GPIO_MMC_DATA3          111
#define GPIO_MMC_CMD            112

/* interface function */
#define GPIO_MMC_CLK_MD         (GPIO_MMC_CLK | GPIO_ALT_FN_2_OUT)
#define GPIO_MMC_DATA0_MD       (GPIO_MMC_DATA0 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA1_MD       (GPIO_MMC_DATA1 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA2_MD       (GPIO_MMC_DATA2 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA3_MD       (GPIO_MMC_DATA3 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

#define GPIO_MMC_CMD_MD         (GPIO_MMC_CMD | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

/* EMU GPIO 119 ---MUX2 120 --- MUX1   */
#define GPIO_EMU_MUX1      120
#define GPIO_EMU_MUX2      120 
#define GPIO_SNP_INT_CTL   120
#define GPIO_SNP_INT_IN    120

/* bp status pin */
//#define GPIO_BP_STATE       41

/* define usb related pin  */
#define GPIO34_TXENB        34 
#define GPIO35_XRXD         35 
#define GPIO36_VMOUT        36 
#define GPIO39_VPOUT        39 
#define GPIO40_VPIN         40 
#define GPIO53_VMIN         53 

/* USB client 6 pin defination */
#define GPIO34_TXENB_MD     (GPIO34_TXENB | GPIO_ALT_FN_1_OUT)
#define GPIO35_XRXD_MD      (GPIO35_XRXD | GPIO_ALT_FN_2_IN )
#define GPIO36_VMOUT_MD     (GPIO36_VMOUT | GPIO_ALT_FN_1_OUT)
#define GPIO39_VPOUT_MD     (GPIO39_VPOUT | GPIO_ALT_FN_1_OUT)
#define GPIO40_VPIN_MD      (GPIO40_VPIN | GPIO_ALT_FN_3_IN )
#define GPIO53_VMIN_MD      (GPIO53_VMIN | GPIO_ALT_FN_2_IN )

#define GPIO53_FFRXD_MD     (53 | GPIO_ALT_FN_1_IN)

/* define camera and CIF related pin  */
#define GPIO_CAM_EN         50  // CAM_EN
#define GPIO_CAM_RST        28  // CAM_RESET
#define GPIO_CAM_FLASH      17  // CAM_FLASH_PWM

#define GPIO_CIF_MCLK       23  // CAM_CLKIN
#define GPIO_CIF_PCLK       54  // CAM_CLKOUT
#define GPIO_CIF_LV         85  // CAM_HSYNC
#define GPIO_CIF_FV         84  // CAM_VSYNC
#define GPIO_CIF_DD0        27  // CAM_DATA0
#define GPIO_CIF_DD1       114  // CAM_DATA1
#define GPIO_CIF_DD2        51  // CAM_DATA2
#define GPIO_CIF_DD3       115  // CAM_DATA3
#define GPIO_CIF_DD4        95  // CAM_DATA4
#define GPIO_CIF_DD5        48  // CAM_DATA5
#define GPIO_CIF_DD6        93  // CAM_DATA6
#define GPIO_CIF_DD7        12  // CAM_DATA7

#define GPIO_CAM_EN_MD      (GPIO_CAM_EN    | GPIO_OUT)
#define GPIO_CAM_RST_MD     (GPIO_CAM_RST   | GPIO_OUT)
#define GPIO_CAM_FLASH_MD   (GPIO_CAM_FLASH | GPIO_OUT)

#define GPIO_CIF_MCLK_MD    (GPIO_CIF_MCLK  | GPIO_ALT_FN_1_OUT)
#define GPIO_CIF_PCLK_MD    (GPIO_CIF_PCLK  | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_LV_MD      (GPIO_CIF_LV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_FV_MD      (GPIO_CIF_FV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD0_MD     (GPIO_CIF_DD0   | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD1_MD     (GPIO_CIF_DD1   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD2_MD     (GPIO_CIF_DD2   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD3_MD     (GPIO_CIF_DD3   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD4_MD     (GPIO_CIF_DD4   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD5_MD     (GPIO_CIF_DD5   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD6_MD     (GPIO_CIF_DD6   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD7_MD     (GPIO_CIF_DD7   | GPIO_ALT_FN_2_IN)

#endif

#if defined(CONFIG_ARCH_EZX_MARTINIQUE)

#define EZX_PRODUCT_NAME		"Martinique"
#define EZX_PRODUCT_NAME_STORAGE	"Martinique Phone"

/* Martinique memory map definition: start */
/*****to do*******/
/* Martinique memory map definition: end */
/* WLAN related gpio */
#define GPIO_WLAN_RESETB                89
#define GPIO_HOST_WLAN_WAKEB            33
#define GPIO_WLAN_HOST_WAKEB            94

#define GPIO_TFLASH_WLANB_MODE          20 
#define GPIO_TFLASH_WLANB_MODE_MD  (GPIO_TFLASH_WLANB_MODE | GPIO_OUT)


#define GPIO_WLAN_RESETB_MD        (GPIO_WLAN_RESETB       | GPIO_OUT)
#define GPIO_HOST_WLAN_WAKEB_MD    (GPIO_HOST_WLAN_WAKEB   | GPIO_OUT)
#define GPIO_WLAN_HOST_WAKEB_MD    (GPIO_WLAN_HOST_WAKEB   | GPIO_IN)

#define GPIO_CLI_CSB 			19
#define GPIO_LCD_CM 			79
#define GPIO_USB_READY    		99
#define GPIO_EMU_INT			10
#define GPIO_TFLASH_INT			11
#define GPIO_CLI_RESETB     		49

/* audio related gpio */
#define GPIO_NSSP_SCLK2                 22
#define GPIO_NSSP_SFRM2                 14 
#define GPIO_NSSP_RXD2                  88
#define GPIO_NSSP_TXD2                  38

#define GPIO_ASSP_SCLK3                 52
#define GPIO_ASSP_TXD3                  81
#define GPIO_ASSP_RXD3                  82
#define GPIO_ASSP_SFRM3                 83

#define GPIO_BITCLK_IN_NSSP_MD  (GPIO_NSSP_SCLK2 | GPIO_ALT_FN_3_IN)
#define GPIO_SYNC_IN_NSSP_MD    (GPIO_NSSP_SFRM2 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_IN_NSSP_MD   (GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_NSSP_MD  (GPIO_NSSP_TXD2  | GPIO_ALT_FN_2_OUT)
#define GPIO_VA_NSSP_SWAP_MD	(GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_OUT)

#define GPIO_BITCLK_IN_ASSP_MD  (GPIO_ASSP_SCLK3 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_ASSP_MD  (GPIO_ASSP_TXD3  | GPIO_ALT_FN_1_OUT)
#define GPIO_SDATA_IN_ASSP_MD   (GPIO_ASSP_RXD3  | GPIO_ALT_FN_1_IN)
#define GPIO_SYNC_IN_ASSP_MD    (GPIO_ASSP_SFRM3 | GPIO_ALT_FN_1_IN)

#define AP_13MHZ_OUTPUT_PIN             9



/* AP and BP handshake signal */
#define GPIO_BP_RDY            0       /* BP_RDY    */
#define GPIO_AP_RDY	       96      /* AP_RDY*/
#define GPIO_BB_RESET          116      /* BB_RESET   */
#define GPIO_BB_FLASHMODE_EN   41      /* notify bp to enter reflash mode  ASSERT(valid)*/

#define GPIO_BB_WDI            3      /* BB_WDI   ???  */
#define GPIO_MCU_INT_SW        41      /* MCU_INT_SW  BP flash mode enable */
#define GPIO_TC_MM_EN          120      /* TC_MM_EN  no use in barbados */

/* control PCAP direct PIN */
#define GPIO_WDI_AP            4       /* WDI_AP                       */
#define GPIO_SYS_RESTART       55      /* restart PCAP power [alias:BATT_DET_IN] */

/* communicate with PCAP's PIN  */
#define GPIO_PCAP_SEC_INT      1       /* PCAP interrupt PIN to AP     */ 
#define GPIO_SPI_CLK           29      /* PCAP SPI port clock          */ 
#define GPIO_SPI_CE            24      /* PCAP SPI port SSPFRM         */  
#define GPIO_SPI_MOSI          25      /* PCAP SPI port SSPTXD         */ 
#define GPIO_SPI_MISO          26      /* PCAP SPI port SSPRXD         */ 

/*  blue tooth control PIN   */
#define GPIO_BT_WAKEUP         57      /* AP wake up bluetooth module  */
#define GPIO_BT_HOSTWAKE       13      /* AP wake up bluetooth module  */
#define GPIO_BT_RESET          37      /* AP reset bluetooth module    */

/* control LCD high - OFF low -- ON  */
#define GPIO_LCD_OFF           78     /* control LCD                */

/*  FFUART PIN              */
/*  since barbados remove the ffuart function we swap the pin to one reserved pin 120 */
/*  we will remove the code in barbados.c in later release */
#define GPIO_ICL_FFRXD_MD      (120 )
#define GPIO_ICL_FFCTS_MD      (120 )
#define GPIO_ICL_FFTXD_MD      (120 )
#define GPIO_ICL_FFRTS_MD      (120 )

/*
 * ezx platform, wake up source edge detect bit
 */
#define PEDR_INT_SEC            1

#define GPIO_FLIP_PIN           15

/* matrix key GPIO */
#define GPIO_MATRIX_KEY_ROW0    100
#define GPIO_MATRIX_KEY_ROW1    101
#define GPIO_MATRIX_KEY_ROW2    102
#define GPIO_MATRIX_KEY_ROW3    97
#define GPIO_MATRIX_KEY_ROW4    98
#define GPIO_MATRIX_KEY_COL0    103
#define GPIO_MATRIX_KEY_COL1    104
#define GPIO_MATRIX_KEY_COL2    105
#define GPIO_MATRIX_KEY_COL3    106
#define GPIO_MATRIX_KEY_COL4    107
#define GPIO_MATRIX_KEY_COL5    108

/* MMC interface */
#define GPIO_MMC_DETECT         GPIO_TFLASH_INT
#define GPIO_MMC_CLK            32
#define GPIO_MMC_DATA0          92
//#define GPIO_MMC_WP		107
#define GPIO_MMC_DATA1          109
#define GPIO_MMC_DATA2          110
#define GPIO_MMC_DATA3          111
#define GPIO_MMC_CMD            112

/* interface function */
#define GPIO_MMC_CLK_MD         (GPIO_MMC_CLK | GPIO_ALT_FN_2_OUT)
#define GPIO_MMC_DATA0_MD       (GPIO_MMC_DATA0 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA1_MD       (GPIO_MMC_DATA1 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA2_MD       (GPIO_MMC_DATA2 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA3_MD       (GPIO_MMC_DATA3 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

#define GPIO_MMC_CMD_MD         (GPIO_MMC_CMD | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

/* EMU GPIO 119 ---MUX2 120 --- MUX1   */
#define GPIO_EMU_MUX1      120
#define GPIO_EMU_MUX2      120 
#define GPIO_SNP_INT_CTL   120
#define GPIO_SNP_INT_IN    120

/* bp status pin */
//#define GPIO_BP_STATE       41

/* define usb related pin  */
#define GPIO34_TXENB        34 
#define GPIO35_XRXD         35 
#define GPIO36_VMOUT        36 
#define GPIO39_VPOUT        39 
#define GPIO40_VPIN         40 
#define GPIO53_VMIN         53 

/* USB client 6 pin defination */
#define GPIO34_TXENB_MD     (GPIO34_TXENB | GPIO_ALT_FN_1_OUT)
#define GPIO35_XRXD_MD      (GPIO35_XRXD | GPIO_ALT_FN_2_IN )
#define GPIO36_VMOUT_MD     (GPIO36_VMOUT | GPIO_ALT_FN_1_OUT)
#define GPIO39_VPOUT_MD     (GPIO39_VPOUT | GPIO_ALT_FN_1_OUT)
#define GPIO40_VPIN_MD      (GPIO40_VPIN | GPIO_ALT_FN_3_IN )
#define GPIO53_VMIN_MD      (GPIO53_VMIN | GPIO_ALT_FN_2_IN )

#define GPIO53_FFRXD_MD     (53 | GPIO_ALT_FN_1_IN)

/* define camera and CIF related pin  */
#define GPIO_CAM_EN         50  // CAM_EN
#define GPIO_CAM_RST        28  // CAM_RESET
#define GPIO_CAM_FLASH      17  // CAM_FLASH_PWM

#define GPIO_CIF_MCLK       23  // CAM_CLKIN
#define GPIO_CIF_PCLK       54  // CAM_CLKOUT
#define GPIO_CIF_LV         85  // CAM_HSYNC
#define GPIO_CIF_FV         84  // CAM_VSYNC
#define GPIO_CIF_DD0        27  // CAM_DATA0
#define GPIO_CIF_DD1       114  // CAM_DATA1
#define GPIO_CIF_DD2        51  // CAM_DATA2
#define GPIO_CIF_DD3       115  // CAM_DATA3
#define GPIO_CIF_DD4        95  // CAM_DATA4
#define GPIO_CIF_DD5        48  // CAM_DATA5
#define GPIO_CIF_DD6        93  // CAM_DATA6
#define GPIO_CIF_DD7        12  // CAM_DATA7

#define GPIO_CAM_EN_MD      (GPIO_CAM_EN    | GPIO_OUT)
#define GPIO_CAM_RST_MD     (GPIO_CAM_RST   | GPIO_OUT)
#define GPIO_CAM_FLASH_MD   (GPIO_CAM_FLASH | GPIO_OUT)

#define GPIO_CIF_MCLK_MD    (GPIO_CIF_MCLK  | GPIO_ALT_FN_1_OUT)
#define GPIO_CIF_PCLK_MD    (GPIO_CIF_PCLK  | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_LV_MD      (GPIO_CIF_LV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_FV_MD      (GPIO_CIF_FV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD0_MD     (GPIO_CIF_DD0   | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD1_MD     (GPIO_CIF_DD1   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD2_MD     (GPIO_CIF_DD2   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD3_MD     (GPIO_CIF_DD3   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD4_MD     (GPIO_CIF_DD4   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD5_MD     (GPIO_CIF_DD5   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD6_MD     (GPIO_CIF_DD6   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD7_MD     (GPIO_CIF_DD7   | GPIO_ALT_FN_2_IN)

#endif

#if defined(CONFIG_ARCH_EZX_HAINAN)

#define EZX_PRODUCT_NAME		"Hainan"
#define EZX_PRODUCT_NAME_STORAGE	"Phone"

/* HAINAN memory map definition: start */
#define MMAP_SECURE_HEADER_2KB	0x00000800UL
#define MMAP_SECURE_HEADER_4KB	0x00001000UL
#define MMAP_FLASH1_START_ADDR	0x00000000UL	// start addr of chip 1
#define MMAP_FLASH2_START_ADDR	0x02000000UL	// start addr of chip 2

// CADDO secondary, /dev/mtd0, 0x00000000-0x00008000
#define MMAP_CADDO_SEC_NAME		"CADDO_SECOND"
#define MMAP_CADDO_SEC_START	MMAP_FLASH1_START_ADDR	// 0x00000000UL
#define MMAP_CADDO_SEC_SIZE		0x00008000UL	// 32K

// itunes, /dev/mtd1, 0x00008000-0x00010000
#define MMAP_ITUNES_NAME		"ITUNES"
#define MMAP_ITUNES_START		(MMAP_CADDO_SEC_START + MMAP_CADDO_SEC_SIZE)	//0x00008000UL
#define MMAP_ITUNES_SIZE		0x00008000UL	// 32K

// CADDO primary, /dev/mtd2, 0x00010000-0x00018000
#define MMAP_CADDO_PRI_NAME		"CADDO_PRIMARY"
#define MMAP_CADDO_PRI_START	(MMAP_ITUNES_START + MMAP_ITUNES_SIZE)	//0x00010000UL
#define MMAP_CADDO_PRI_SIZE		0x00008000UL	// 32K

// FOTA reserved, /dev/mtd3, 0x00018000-0x00020000
#define MMAP_FATO_REV_NAME		"FOTA_REV"
#define MMAP_FATO_REV_START		(MMAP_CADDO_PRI_START + MMAP_CADDO_PRI_SIZE)	//0x00018000UL
#define MMAP_FATO_REV_SIZE		0x00008000UL	// 32K

// "mbm(Caddo data, Motorola Boot Module)", /dev/mtd4, 0x00020000-0x00060000
#define MMAP_MBM_NAME			"MBM"
#define MMAP_MBM_START			(MMAP_FATO_REV_START + MMAP_FATO_REV_SIZE)		//0x00020000UL
#define MMAP_MBM_SIZE			0x00040000UL	// 256K

// Config Descriptor Table, kernel configuration table, /dev/mtd?, 0x00060000-0x00080000
#define MMAP_CDT_KCT_NAME		"CONFIG"
#define MMAP_CDT_KCT_START		(MMAP_MBM_START + MMAP_MBM_SIZE)		//0x00060000UL
#define MMAP_CDT_KCT_SIZE		0x00020000UL	// 128K

// blob, /dev/mtd6, 0x00080000-0x000A0000
#define MMAP_BLOB_NAME			"BLOB"
#define MMAP_BLOB_START			(MMAP_CDT_KCT_START + MMAP_CDT_KCT_SIZE)	// 0x00080000UL
#define MMAP_BLOB_SIZE			0x00020000UL	// 128K

// kernel zImage, /dev/mtd7, 0x000A0000-0x001A0000
#define MMAP_KERNEL_NAME		"KERNEL"
#define MMAP_KERNEL_START		(MMAP_BLOB_START + MMAP_BLOB_SIZE)	// 0x000A0000UL
#define MMAP_KERNEL_SIZE		0x00100000UL	// 1M

// resource(preloaded contents), /dev/roflash2, 0x001A0000-0x00BA0000
#define ROFLASH_RESOURCE_NAME	"data_resource"
#define MMAP_RESOURCE_NAME		"RESOURCE"
#define MMAP_RESOURCE_START		(MMAP_KERNEL_START + MMAP_KERNEL_SIZE)	// 0x001A0000UL
#define MMAP_RESOURCE_CHIP_OFFSET	(MMAP_RESOURCE_START - MMAP_FLASH1_START_ADDR)
#define MMAP_RESOURCE_SIZE		0x00A00000UL	// 10M

// "user_db(NativeFS - phonebook, datebook, SMS)", /dev/mtd8, 0x00BA0000-0x010A0000
#define MMAP_USERDB_NAME		"USERFS_DB"
#define MMAP_USERDB_START		(MMAP_RESOURCE_START + MMAP_RESOURCE_SIZE)	// 0x00BA0000UL
#define MMAP_USERDB_SIZE		0x00500000UL	// 5M

// "user_general(user space)", /dev/mtd9, 0x010A0000-0x01800000
#define MMAP_USERGEN_NAME		"USERFS_GENERAL"
#define MMAP_USERGEN_START		(MMAP_USERDB_START + MMAP_USERDB_SIZE)	// 0x01020000UL
#define MMAP_USERGEN_SIZE		0x00760000UL	// 7.5M-128KB(for 32K mnt_record + 96K paniclog)

// reserved 32KB for mnt record, 0x01800000-0x01808000
#define MMAP_MNTRECORD_NAME		"MNT_RECORD"
#define MMAP_MNTRECORD_START	(MMAP_USERGEN_START + MMAP_USERGEN_SIZE)	// 0x01800000UL
#define MMAP_MNTRECORD_SIZE		0x00008000UL	// 32K

// reserved 96KB for paniclog, 0x01808000-0x01820000
#define MMAP_PANICLOG_NAME		"PANICLOG"
#define MMAP_PANICLOG_START		(MMAP_MNTRECORD_START + MMAP_MNTRECORD_SIZE)	// 0x01808000UL
#define MMAP_PANICLOG_SIZE		0x00018000UL	// 96K

// securesetup, /dev/roflash4, 0x01820000-0x01840000
#define ROFLASH_SECURE_SETUP_NAME	"secure_setup"
#define MMAP_SECURE_SETUP_NAME	"SECURE_SETUP"
#define MMAP_SECURE_SETUP_START	(MMAP_PANICLOG_START + MMAP_PANICLOG_SIZE)	// 0x01820000UL
#define MMAP_SECURE_SETUP_CHIP_OFFSET	(MMAP_SECURE_SETUP_START - MMAP_FLASH1_START_ADDR)
#define MMAP_SECURE_SETUP_SIZE	0x00020000UL	// 128K

// tcmd, /dev/mtd10, 0x01840000-0x01860000
#define MMAP_TCMD_NAME			"TEST_CMD"
#define MMAP_TCMD_START			(MMAP_SECURE_SETUP_START + MMAP_SECURE_SETUP_SIZE)	// 0x01840000UL
#define MMAP_TCMD_SIZE			0x00020000UL	// 128K

// logo, /dev/mtd11, 0x01860000-0x01880000
#define MMAP_LOGO_NAME			"LOGO"
#define MMAP_LOGO_START			(MMAP_TCMD_START + MMAP_TCMD_SIZE)	// 0x01860000UL
#define MMAP_LOGO_SIZE			0x00020000UL	// 128K

// "fota(FOTA Update Agent, Update Package)", /dev/mtd12, 0x01880000-0x01940000
#define MMAP_FOTA_NAME			"FOTA"
#define MMAP_FOTA_START			(MMAP_LOGO_START + MMAP_LOGO_SIZE)	// 0x01880000UL
#define MMAP_FOTA_SIZE			0x000C0000UL	// 768K

// System reserved, /dev/mtd13, 0x01940000-0x01960000
#define MMAP_SYS_REV_NAME		"RESERVE"
#define MMAP_SYS_REV_START		(MMAP_FOTA_START + MMAP_FOTA_SIZE)	// 0x01940000UL
#define MMAP_SYS_REV_SIZE		0x00020000UL	// 128K

// language, /dev/roflash1, 0x01960000-0x0215FFFF
#define ROFLASH_LANGUAGE_NAME	"language"
#define MMAP_LANGUAGE_NAME		"LANGUAGE"
#define MMAP_LANGUAGE_START		(MMAP_SYS_REV_START + MMAP_SYS_REV_SIZE)	// 0x01960000UL
#define MMAP_LANGUAGE_CHIP_OFFSET	(MMAP_LANGUAGE_START - MMAP_FLASH1_START_ADDR)
#define MMAP_LANGUAGE_SIZE		0x00800000UL	// 8M

// setup, /dev/roflash3, 0x02160000-0x02180000
#define ROFLASH_SETUP_NAME		"setup"
#define MMAP_SETUP_NAME			"SETUP"
#define MMAP_SETUP_START		(MMAP_LANGUAGE_START + MMAP_LANGUAGE_SIZE)	// 0x02160000UL
#define MMAP_SETUP_CHIP_OFFSET	(MMAP_SETUP_START - MMAP_FLASH2_START_ADDR)
#define MMAP_SETUP_SIZE			0x00020000UL	// 128K

// rootfs, /dev/roflash, 0x02180000-0x04000000
#define ROFLASH_ROOTFS_NAME		"rootfs"
#define MMAP_ROOTFS_NAME		"ROOTFS"
#define MMAP_ROOTFS_START		(MMAP_SETUP_START + MMAP_SETUP_SIZE)	// 0x01F80000UL
#define MMAP_ROOTFS_CHIP_OFFSET	(MMAP_ROOTFS_START - MMAP_FLASH2_START_ADDR)
#define MMAP_ROOTFS_SIZE		0x01E80000UL	// 30.5M

// define flash_rw_array[] for unlockdown
// flash_rw_array[0]: "CADDO2"+"ITUNES"+"CADDO1"+"FOTA_REV"
#define FLASH_RW_ARRAY1_START	(MMAP_CADDO_SEC_START)		// 0x00000000UL
#define FLASH_RW_ARRAY1_END		(MMAP_FATO_REV_START + MMAP_FATO_REV_SIZE)		// 0x00020000UL

// flash_rw_array[1]: "userfs_db"+"userfs_general" + "32+96 KB reserved for mnt_record and paniclog"
#define FLASH_RW_ARRAY2_START	(MMAP_USERDB_START)		// 0x00BA0000UL
#define FLASH_RW_ARRAY2_END		(MMAP_PANICLOG_START + MMAP_PANICLOG_SIZE)	// 0x019A0000UL

// flash_rw_array[2]: "tcmd"+"logo"+"fota"+"128KB reserve"
#define FLASH_RW_ARRAY3_START	(MMAP_TCMD_START)		// 0x019C0000UL
#define FLASH_RW_ARRAY3_END		(MMAP_SYS_REV_START + MMAP_SYS_REV_SIZE)	// 0x01AE0000UL

/* HAINAN memory map definition: end */

/* audio related gpio */
#define GPIO_NSSP_SCLK2                 22
#define GPIO_NSSP_SFRM2                 14
#define GPIO_NSSP_RXD2                  88
#define GPIO_NSSP_TXD2                  38

#define GPIO_ASSP_SCLK3                 52
#define GPIO_ASSP_TXD3                  81
#define GPIO_ASSP_RXD3                  82
#define GPIO_ASSP_SFRM3                 83

#define GPIO_BITCLK_IN_NSSP_MD  (GPIO_NSSP_SCLK2 | GPIO_ALT_FN_3_IN)
#define GPIO_SYNC_IN_NSSP_MD    (GPIO_NSSP_SFRM2 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_IN_NSSP_MD   (GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_NSSP_MD  (GPIO_NSSP_TXD2  | GPIO_ALT_FN_2_OUT)
#define GPIO_VA_NSSP_SWAP_MD    (GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_OUT)

#define GPIO_BITCLK_IN_ASSP_MD  (GPIO_ASSP_SCLK3 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_ASSP_MD  (GPIO_ASSP_TXD3  | GPIO_ALT_FN_1_OUT)
#define GPIO_SDATA_IN_ASSP_MD   (GPIO_ASSP_RXD3  | GPIO_ALT_FN_1_IN)
#define GPIO_SYNC_IN_ASSP_MD    (GPIO_ASSP_SFRM3 | GPIO_ALT_FN_1_IN)

#define GPIO_TFLASH_INT			11
#define GPIO_SD_NCD			    GPIO_TFLASH_INT
#define GPIO_USB20_NCS3			79
#define GPIO_HDD_USB20_READY		18
#define GPIO_USB20_INT			19
#define GPIO_USB20_DREQ0		20
#define GPIO_HDST_VGS_BOOST		33
#define GPIO_NPWE			49
#define GPIO_USB20_SWITCH		94
#define GPIO_EL_EN			89
#define GPIO_USB_READY                  99
#define GPIO_EMU_INT                    10
#define GPIO_CLI_RESETB     		49
#define GPIO_LCD_CM                     79

#define AP_13MHZ_OUTPUT_PIN     9

/* AP and BP handshake signal */
#define GPIO_BP_RDY            0       /* BP_RDY    */
#define GPIO_AP_RDY            96      /* AP_RDY*/
#define GPIO_BB_RESET          116      /* BB_RESET   */
#define GPIO_BB_FLASHMODE_EN   41      /* notify bp to enter reflash mode  ASSERT(valid)*/

#define GPIO_BB_WDI            3      /* BB_WDI   ???  */
#define GPIO_MCU_INT_SW        41      /* MCU_INT_SW  BP flash mode enable */
#define GPIO_TC_MM_EN          120      /* TC_MM_EN  no use in barbados */

/* control PCAP direct PIN */
#define GPIO_WDI_AP            4       /* WDI_AP                       */
#define GPIO_SYS_RESTART       55      /* restart PCAP power [alias:BATT_DET_IN] */

/* communicate with PCAP's PIN  */
#define GPIO_PCAP_SEC_INT      1       /* PCAP interrupt PIN to AP     */
#define GPIO_SPI_CLK           29      /* PCAP SPI port clock          */
#define GPIO_SPI_CE            24      /* PCAP SPI port SSPFRM         */
#define GPIO_SPI_MOSI          25      /* PCAP SPI port SSPTXD         */
#define GPIO_SPI_MISO          26      /* PCAP SPI port SSPRXD         */

/*  blue tooth control PIN   */
#define GPIO_BT_WAKEUP         57      /* AP wake up bluetooth module  */
#define GPIO_BT_HOSTWAKE       13      /* AP wake up bluetooth module  */
#define GPIO_BT_RESET          37      /* AP reset bluetooth module    */

/* control LCD high - OFF low -- ON  */
#define GPIO_LCD_OFF           78     /* control LCD                */

/*  FFUART PIN              */
/*  since barbados remove the ffuart function we swap the pin to one reserved pin 120 */
/*  we will remove the code in barbados.c in later release */
#define GPIO_ICL_FFRXD_MD      (120 )
#define GPIO_ICL_FFCTS_MD      (120 )
#define GPIO_ICL_FFTXD_MD      (120 )
#define GPIO_ICL_FFRTS_MD      (120 )

/*
 * ezx platform, wake up source edge detect bit
 */
#define PEDR_INT_SEC            1

#define GPIO_FLIP_PIN           15

/* matrix key GPIO */
#define GPIO_MATRIX_KEY_ROW0    100
#define GPIO_MATRIX_KEY_ROW1    101
#define GPIO_MATRIX_KEY_ROW2    102
#define GPIO_MATRIX_KEY_ROW3    97
#define GPIO_MATRIX_KEY_ROW4    98
#define GPIO_MATRIX_KEY_COL0    103
#define GPIO_MATRIX_KEY_COL1    104
#define GPIO_MATRIX_KEY_COL2    105
#define GPIO_MATRIX_KEY_COL3    106
#define GPIO_MATRIX_KEY_COL4    107
#define GPIO_MATRIX_KEY_COL5    108

/* MMC interface */

#define GPIO_MMC_CLK            32
#define GPIO_MMC_DATA0          92
//#define GPIO_MMC_WP           107
#define GPIO_MMC_DATA1          109
#define GPIO_MMC_DATA2          110
#define GPIO_MMC_DATA3          111
#define GPIO_MMC_CMD            112
#define GPIO_MMC_DETECT         GPIO_MMC_DATA3

/* interface function */
#define GPIO_MMC_CLK_MD         (GPIO_MMC_CLK | GPIO_ALT_FN_2_OUT)
#define GPIO_MMC_DATA0_MD       (GPIO_MMC_DATA0 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA1_MD       (GPIO_MMC_DATA1 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA2_MD       (GPIO_MMC_DATA2 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA3_MD       (GPIO_MMC_DATA3 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

#define GPIO_MMC_CMD_MD         (GPIO_MMC_CMD | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

/* EMU GPIO 119 ---MUX2 120 --- MUX1   */
#define GPIO_EMU_MUX1      120
#define GPIO_EMU_MUX2      120
#define GPIO_SNP_INT_CTL   120
#define GPIO_SNP_INT_IN    120
/* bp status pin */
//#define GPIO_BP_STATE       41

/* define usb related pin  */
#define GPIO34_TXENB        34
#define GPIO35_XRXD         35
#define GPIO36_VMOUT        36
#define GPIO39_VPOUT        39
#define GPIO40_VPIN         40
#define GPIO53_VMIN         53

/* USB client 6 pin defination */
#define GPIO34_TXENB_MD     (GPIO34_TXENB | GPIO_ALT_FN_1_OUT)
#define GPIO35_XRXD_MD      (GPIO35_XRXD | GPIO_ALT_FN_2_IN )
#define GPIO36_VMOUT_MD     (GPIO36_VMOUT | GPIO_ALT_FN_1_OUT)
#define GPIO39_VPOUT_MD     (GPIO39_VPOUT | GPIO_ALT_FN_1_OUT)
#define GPIO40_VPIN_MD      (GPIO40_VPIN | GPIO_ALT_FN_3_IN )
#define GPIO53_VMIN_MD      (GPIO53_VMIN | GPIO_ALT_FN_2_IN )

#define GPIO53_FFRXD_MD     (53 | GPIO_ALT_FN_1_IN)

/* define camera and CIF related pin  */
#define GPIO_CAM_EN         50  // CAM_EN
#define GPIO_CAM_RST        28  // CAM_RESET
#define GPIO_CAM_FLASH      17  // CAM_FLASH_PWM

#define GPIO_CIF_MCLK       23  // CAM_CLKIN
#define GPIO_CIF_PCLK       54  // CAM_CLKOUT
#define GPIO_CIF_LV         85  // CAM_HSYNC
#define GPIO_CIF_FV         84  // CAM_VSYNC
#define GPIO_CIF_DD0        27  // CAM_DATA0
#define GPIO_CIF_DD1       114  // CAM_DATA1
#define GPIO_CIF_DD2        51  // CAM_DATA2
#define GPIO_CIF_DD3       115  // CAM_DATA3
#define GPIO_CIF_DD4        95  // CAM_DATA4
#define GPIO_CIF_DD5        48  // CAM_DATA5
#define GPIO_CIF_DD6        93  // CAM_DATA6
#define GPIO_CIF_DD7        12  // CAM_DATA7
#define GPIO_CAM_EN_MD      (GPIO_CAM_EN    | GPIO_OUT)
#define GPIO_CAM_RST_MD     (GPIO_CAM_RST   | GPIO_OUT)
#define GPIO_CAM_FLASH_MD   (GPIO_CAM_FLASH | GPIO_OUT)

#define GPIO_CIF_MCLK_MD    (GPIO_CIF_MCLK  | GPIO_ALT_FN_1_OUT)
#define GPIO_CIF_PCLK_MD    (GPIO_CIF_PCLK  | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_LV_MD      (GPIO_CIF_LV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_FV_MD      (GPIO_CIF_FV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD0_MD     (GPIO_CIF_DD0   | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD1_MD     (GPIO_CIF_DD1   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD2_MD     (GPIO_CIF_DD2   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD3_MD     (GPIO_CIF_DD3   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD4_MD     (GPIO_CIF_DD4   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD5_MD     (GPIO_CIF_DD5   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD6_MD     (GPIO_CIF_DD6   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD7_MD     (GPIO_CIF_DD7   | GPIO_ALT_FN_2_IN)

#endif

#if defined(CONFIG_ARCH_EZX_SUMATRA)

#define EZX_PRODUCT_NAME		"Sumatra"
#define EZX_PRODUCT_NAME_STORAGE	"Phone"

/* SUMATRA memory map definition: start */
/*****to do*******/
/* SUMATRA memory map definition: end */
/* audio related gpio */
#define GPIO_NSSP_SCLK2                 22
#define GPIO_NSSP_SFRM2                 14 
#define GPIO_NSSP_RXD2                  88
#define GPIO_NSSP_TXD2                  38

#define GPIO_ASSP_SCLK3                 52
#define GPIO_ASSP_TXD3                  81
#define GPIO_ASSP_RXD3                  82
#define GPIO_ASSP_SFRM3                 83

#define GPIO_BITCLK_IN_NSSP_MD  (GPIO_NSSP_SCLK2 | GPIO_ALT_FN_3_IN)
#define GPIO_SYNC_IN_NSSP_MD    (GPIO_NSSP_SFRM2 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_IN_NSSP_MD   (GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_NSSP_MD  (GPIO_NSSP_TXD2  | GPIO_ALT_FN_2_OUT)
#define GPIO_VA_NSSP_SWAP_MD	(GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_OUT)

#define GPIO_BITCLK_IN_ASSP_MD  (GPIO_ASSP_SCLK3 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_ASSP_MD  (GPIO_ASSP_TXD3  | GPIO_ALT_FN_1_OUT)
#define GPIO_SDATA_IN_ASSP_MD   (GPIO_ASSP_RXD3  | GPIO_ALT_FN_1_IN)
#define GPIO_SYNC_IN_ASSP_MD    (GPIO_ASSP_SFRM3 | GPIO_ALT_FN_1_IN)

#define GPIO_TFLASH_INT			11
#define GPIO_SD_NCD			    GPIO_TFLASH_INT
#define GPIO_USB20_NCS3			79
#define GPIO_HDD_USB20_READY		18
#define GPIO_USB20_INT			19
#define GPIO_USB20_DREQ0		20
#define GPIO_HDST_VGS_BOOST		33
#define GPIO_NPWE			49
#define GPIO_USB20_SWITCH		94
#define GPIO_HPR_DET_STEREO_EN		89
#define GPIO_USB_READY                  99
#define GPIO_EMU_INT                    10

#define AP_13MHZ_OUTPUT_PIN     9

/* AP and BP handshake signal */
#define GPIO_BP_RDY            0       /* BP_RDY    */
#define GPIO_AP_RDY	       96      /* AP_RDY*/
#define GPIO_BB_RESET          116      /* BB_RESET   */
#define GPIO_BB_FLASHMODE_EN   41      /* notify bp to enter reflash mode  ASSERT(valid)*/

#define GPIO_BB_WDI            3      /* BB_WDI   ???  */
#define GPIO_MCU_INT_SW        41      /* MCU_INT_SW  BP flash mode enable */
#define GPIO_TC_MM_EN          120      /* TC_MM_EN  no use in barbados */

/* control PCAP direct PIN */
#define GPIO_WDI_AP            4       /* WDI_AP                       */
#define GPIO_SYS_RESTART       55      /* restart PCAP power [alias:BATT_DET_IN] */

/* communicate with PCAP's PIN  */
#define GPIO_PCAP_SEC_INT      1       /* PCAP interrupt PIN to AP     */ 
#define GPIO_SPI_CLK           29      /* PCAP SPI port clock          */ 
#define GPIO_SPI_CE            24      /* PCAP SPI port SSPFRM         */  
#define GPIO_SPI_MOSI          25      /* PCAP SPI port SSPTXD         */ 
#define GPIO_SPI_MISO          26      /* PCAP SPI port SSPRXD         */ 

/*  blue tooth control PIN   */
#define GPIO_BT_WAKEUP         57      /* AP wake up bluetooth module  */
#define GPIO_BT_HOSTWAKE       13      /* AP wake up bluetooth module  */
#define GPIO_BT_RESET          37      /* AP reset bluetooth module    */

/* control LCD high - OFF low -- ON  */
#define GPIO_LCD_OFF           78     /* control LCD                */

/*  FFUART PIN              */
/*  since barbados remove the ffuart function we swap the pin to one reserved pin 120 */
/*  we will remove the code in barbados.c in later release */
#define GPIO_ICL_FFRXD_MD      (120 )
#define GPIO_ICL_FFCTS_MD      (120 )
#define GPIO_ICL_FFTXD_MD      (120 )
#define GPIO_ICL_FFRTS_MD      (120 )

/*
 * ezx platform, wake up source edge detect bit
 */
#define PEDR_INT_SEC            1

#define GPIO_FLIP_PIN           15

/* matrix key GPIO */
#define GPIO_MATRIX_KEY_ROW0    100
#define GPIO_MATRIX_KEY_ROW1    101
#define GPIO_MATRIX_KEY_ROW2    102
#define GPIO_MATRIX_KEY_ROW3    97
#define GPIO_MATRIX_KEY_ROW4    98
#define GPIO_MATRIX_KEY_COL0    103
#define GPIO_MATRIX_KEY_COL1    104
#define GPIO_MATRIX_KEY_COL2    105
#define GPIO_MATRIX_KEY_COL3    106
#define GPIO_MATRIX_KEY_COL4    107
#define GPIO_MATRIX_KEY_COL5    108

/* MMC interface */

#define GPIO_MMC_DETECT         GPIO_TFLASH_INT 
#define GPIO_MMC_CLK            32
#define GPIO_MMC_DATA0          92
#define GPIO_MMC_WP		80 
#define GPIO_MMC_DATA1          109
#define GPIO_MMC_DATA2          110
#define GPIO_MMC_DATA3          111
#define GPIO_MMC_CMD            112

/* interface function */
#define GPIO_MMC_CLK_MD         (GPIO_MMC_CLK | GPIO_ALT_FN_2_OUT)
#define GPIO_MMC_DATA0_MD       (GPIO_MMC_DATA0 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA1_MD       (GPIO_MMC_DATA1 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA2_MD       (GPIO_MMC_DATA2 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA3_MD       (GPIO_MMC_DATA3 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

#define GPIO_MMC_CMD_MD         (GPIO_MMC_CMD | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

/* EMU GPIO 119 ---MUX2 120 --- MUX1   */
#define GPIO_EMU_MUX1      120
#define GPIO_EMU_MUX2      120 
#define GPIO_SNP_INT_CTL   120
#define GPIO_SNP_INT_IN    120


/*SDRAM*/
#define GPIO_nSDCS3        21
#define GPIO_nSDCS3_MD    (GPIO_nSDCS3 | GPIO_ALT_FN_1_OUT)

/* bp status pin */
//#define GPIO_BP_STATE       41

/* define usb related pin  */
#define GPIO34_TXENB        34 
#define GPIO35_XRXD         35 
#define GPIO36_VMOUT        36 
#define GPIO39_VPOUT        39 
#define GPIO40_VPIN         40 
#define GPIO53_VMIN         53 

/* USB client 6 pin defination */
#define GPIO34_TXENB_MD     (GPIO34_TXENB | GPIO_ALT_FN_1_OUT)
#define GPIO35_XRXD_MD      (GPIO35_XRXD | GPIO_ALT_FN_2_IN )
#define GPIO36_VMOUT_MD     (GPIO36_VMOUT | GPIO_ALT_FN_1_OUT)
#define GPIO39_VPOUT_MD     (GPIO39_VPOUT | GPIO_ALT_FN_1_OUT)
#define GPIO40_VPIN_MD      (GPIO40_VPIN | GPIO_ALT_FN_3_IN )
#define GPIO53_VMIN_MD      (GPIO53_VMIN | GPIO_ALT_FN_2_IN )

#define GPIO53_FFRXD_MD     (53 | GPIO_ALT_FN_1_IN)

/* define camera and CIF related pin  */
#define GPIO_CAM_EN         50  // CAM_EN
#define GPIO_CAM_RST        28  // CAM_RESET
#define GPIO_CAM_FLASH      17  // CAM_FLASH_PWM

#define GPIO_CIF_MCLK       23  // CAM_CLKIN
#define GPIO_CIF_PCLK       54  // CAM_CLKOUT
#define GPIO_CIF_LV         85  // CAM_HSYNC
#define GPIO_CIF_FV         84  // CAM_VSYNC
#define GPIO_CIF_DD0        27  // CAM_DATA0
#define GPIO_CIF_DD1       114  // CAM_DATA1
#define GPIO_CIF_DD2        51  // CAM_DATA2
#define GPIO_CIF_DD3       115  // CAM_DATA3
#define GPIO_CIF_DD4        95  // CAM_DATA4
#define GPIO_CIF_DD5        48  // CAM_DATA5
#define GPIO_CIF_DD6        93  // CAM_DATA6
#define GPIO_CIF_DD7        12  // CAM_DATA7

#define GPIO_CAM_EN_MD      (GPIO_CAM_EN    | GPIO_OUT)
#define GPIO_CAM_RST_MD     (GPIO_CAM_RST   | GPIO_OUT)
#define GPIO_CAM_FLASH_MD   (GPIO_CAM_FLASH | GPIO_OUT)

#define GPIO_CIF_MCLK_MD    (GPIO_CIF_MCLK  | GPIO_ALT_FN_1_OUT)
#define GPIO_CIF_PCLK_MD    (GPIO_CIF_PCLK  | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_LV_MD      (GPIO_CIF_LV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_FV_MD      (GPIO_CIF_FV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD0_MD     (GPIO_CIF_DD0   | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD1_MD     (GPIO_CIF_DD1   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD2_MD     (GPIO_CIF_DD2   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD3_MD     (GPIO_CIF_DD3   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD4_MD     (GPIO_CIF_DD4   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD5_MD     (GPIO_CIF_DD5   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD6_MD     (GPIO_CIF_DD6   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD7_MD     (GPIO_CIF_DD7   | GPIO_ALT_FN_2_IN)

#endif

#if defined(CONFIG_ARCH_EZX_MACAU) 
#define EZX_PRODUCT_NAME		"Macau"
#define EZX_PRODUCT_NAME_STORAGE	"Phone"
/*memory map define for MACAU*/
/* MACAU memory map definition: start */

#define MMAP_SECURE_HEADER_2KB	0x00000800UL
#define MMAP_SECURE_HEADER_4KB	0x00001000UL
#define MMAP_FLASH1_START_ADDR	0x00000000UL	// start addr of chip 1
#define MMAP_FLASH2_START_ADDR	0x02000000UL	// start addr of chip 2

// CADDO secondary, /dev/mtd0, 0x00000000-0x00008000
#define MMAP_CADDO_SEC_NAME		"CADDO_SECOND"
#define MMAP_CADDO_SEC_START	MMAP_FLASH1_START_ADDR	// 0x00000000UL
#define MMAP_CADDO_SEC_SIZE		0x00008000UL	// 32K

// itunes, /dev/mtd1, 0x00008000-0x00010000
#define MMAP_ITUNES_NAME		"ITUNES"
#define MMAP_ITUNES_START		(MMAP_CADDO_SEC_START + MMAP_CADDO_SEC_SIZE)	//0x00008000UL
#define MMAP_ITUNES_SIZE		0x00008000UL	// 32K

// CADDO primary, /dev/mtd2, 0x00010000-0x00018000
#define MMAP_CADDO_PRI_NAME		"CADDO_PRIMARY"
#define MMAP_CADDO_PRI_START	(MMAP_ITUNES_START + MMAP_ITUNES_SIZE)	//0x00010000UL
#define MMAP_CADDO_PRI_SIZE		0x00008000UL	// 32K

// FOTA reserved, /dev/mtd3, 0x00018000-0x00020000
#define MMAP_FATO_REV_NAME		"FOTA_REV"
#define MMAP_FATO_REV_START		(MMAP_CADDO_PRI_START + MMAP_CADDO_PRI_SIZE)	//0x00018000UL
#define MMAP_FATO_REV_SIZE		0x00008000UL	// 32K

// "mbm(Caddo data, Motorola Boot Module)", /dev/mtd4, 0x00020000-0x00060000
#define MMAP_MBM_NAME			"MBM"
#define MMAP_MBM_START			(MMAP_FATO_REV_START + MMAP_FATO_REV_SIZE)		//0x00020000UL
#define MMAP_MBM_SIZE			0x00040000UL	// 256K

// Config Descriptor Table, kernel configuration table, /dev/mtd?, 0x00060000-0x00080000
#define MMAP_CDT_KCT_NAME		"CONFIG"
#define MMAP_CDT_KCT_START		(MMAP_MBM_START + MMAP_MBM_SIZE)		//0x00060000UL
#define MMAP_CDT_KCT_SIZE		0x00020000UL	// 128K

// blob, /dev/mtd6, 0x00080000-0x000A0000
#define MMAP_BLOB_NAME			"BLOB"
#define MMAP_BLOB_START			(MMAP_CDT_KCT_START + MMAP_CDT_KCT_SIZE)	// 0x00080000UL
#define MMAP_BLOB_SIZE			0x00020000UL	// 128K

// kernel zImage, /dev/mtd7, 0x000A0000-0x001A0000
#define MMAP_KERNEL_NAME		"KERNEL"
#define MMAP_KERNEL_START		(MMAP_BLOB_START + MMAP_BLOB_SIZE)	// 0x000A0000UL
#define MMAP_KERNEL_SIZE		0x00100000UL	// 1M

// resource(preloaded contents), /dev/roflash2, 0x001A0000-0x00BA0000
#define ROFLASH_RESOURCE_NAME	"data_resource"
#define MMAP_RESOURCE_NAME		"RESOURCE"
#define MMAP_RESOURCE_START		(MMAP_KERNEL_START + MMAP_KERNEL_SIZE)	// 0x001A0000UL
#define MMAP_RESOURCE_CHIP_OFFSET	(MMAP_RESOURCE_START - MMAP_FLASH1_START_ADDR)
#define MMAP_RESOURCE_SIZE		0x00A00000UL	// 10M

// "user_db(NativeFS - phonebook, datebook, SMS)", /dev/mtd8, 0x00BA0000-0x011A0000
#define MMAP_USERDB_NAME		"USERFS_DB"
#define MMAP_USERDB_START		(MMAP_RESOURCE_START + MMAP_RESOURCE_SIZE)	// 0x00BA0000UL
#define MMAP_USERDB_SIZE		0x00600000UL	// 6M

// "user_general(user space)", /dev/mtd9, 0x011A0000-0x01980000
#define MMAP_USERGEN_NAME		"USERFS_GENERAL"
#define MMAP_USERGEN_START		(MMAP_USERDB_START + MMAP_USERDB_SIZE)	// 0x011A0000UL
#define MMAP_USERGEN_SIZE		0x007E0000UL	// 8M-128KB(for 32K mnt_record + 96K paniclog)

// reserved 32KB for mnt record, 0x01980000-0x01988000
#define MMAP_MNTRECORD_NAME		"MNT_RECORD"
#define MMAP_MNTRECORD_START	(MMAP_USERGEN_START + MMAP_USERGEN_SIZE)	// 0x01980000UL
#define MMAP_MNTRECORD_SIZE		0x00008000UL	// 32K

// reserved 96KB for paniclog, 0x01988000-0x019A0000
#define MMAP_PANICLOG_NAME		"PANICLOG"
#define MMAP_PANICLOG_START		(MMAP_MNTRECORD_START + MMAP_MNTRECORD_SIZE)	// 0x01988000UL
#define MMAP_PANICLOG_SIZE		0x00018000UL	// 96K

// securesetup, /dev/roflash4, 0x019A0000-0x019C0000
#define ROFLASH_SECURE_SETUP_NAME	"secure_setup"
#define MMAP_SECURE_SETUP_NAME	"SECURE_SETUP"
#define MMAP_SECURE_SETUP_START	(MMAP_PANICLOG_START + MMAP_PANICLOG_SIZE)	// 0x019A0000UL
#define MMAP_SECURE_SETUP_CHIP_OFFSET	(MMAP_SECURE_SETUP_START - MMAP_FLASH1_START_ADDR)
#define MMAP_SECURE_SETUP_SIZE	0x00020000UL	// 128K

// tcmd, /dev/mtd10, 0x019C0000-0x019E0000
#define MMAP_TCMD_NAME			"TEST_CMD"
#define MMAP_TCMD_START			(MMAP_SECURE_SETUP_START + MMAP_SECURE_SETUP_SIZE)	// 0x019C0000UL
#define MMAP_TCMD_SIZE			0x00020000UL	// 128K

// logo, /dev/mtd11, 0x019E0000-0x01A00000
#define MMAP_LOGO_NAME			"LOGO"
#define MMAP_LOGO_START			(MMAP_TCMD_START + MMAP_TCMD_SIZE)	// 0x019E0000UL
#define MMAP_LOGO_SIZE			0x00020000UL	// 128K

// "fota(FOTA Update Agent, Update Package)", /dev/mtd12, 0x01A00000-0x01AC0000
#define MMAP_FOTA_NAME			"FOTA"
#define MMAP_FOTA_START			(MMAP_LOGO_START + MMAP_LOGO_SIZE)	// 0x01A00000UL
#define MMAP_FOTA_SIZE			0x000C0000UL	// 768K

// System reserved, /dev/mtd13, 0x01AC0000-0x01AE0000
#define MMAP_SYS_REV_NAME		"RESERVE"
#define MMAP_SYS_REV_START		(MMAP_FOTA_START + MMAP_FOTA_SIZE)	// 0x01AC0000UL
#define MMAP_SYS_REV_SIZE		0x00020000UL	// 128K

// language, /dev/roflash1, 0x01AE0000-0x025E0000
#define ROFLASH_LANGUAGE_NAME	"language"
#define MMAP_LANGUAGE_NAME		"LANGUAGE"
#define MMAP_LANGUAGE_START		(MMAP_SYS_REV_START + MMAP_SYS_REV_SIZE)	// 0x01AE0000UL
#define MMAP_LANGUAGE_CHIP_OFFSET	(MMAP_LANGUAGE_START - MMAP_FLASH1_START_ADDR)
#define MMAP_LANGUAGE_SIZE		0x00B00000UL	// 11M

// setup, /dev/roflash3, 0x025E0000-0x02600000
#define ROFLASH_SETUP_NAME		"setup"
#define MMAP_SETUP_NAME			"SETUP"
#define MMAP_SETUP_START		(MMAP_LANGUAGE_START + MMAP_LANGUAGE_SIZE)	// 0x025E0000UL
#define MMAP_SETUP_CHIP_OFFSET	(MMAP_SETUP_START - MMAP_FLASH2_START_ADDR)
#define MMAP_SETUP_SIZE			0x00020000UL	// 128K

// rootfs, /dev/roflash, 0x02600000-0x04000000
#define ROFLASH_ROOTFS_NAME		"rootfs"
#define MMAP_ROOTFS_NAME		"ROOTFS"
#define MMAP_ROOTFS_START		(MMAP_SETUP_START + MMAP_SETUP_SIZE)	// 0x02600000UL
#define MMAP_ROOTFS_CHIP_OFFSET	(MMAP_ROOTFS_START - MMAP_FLASH2_START_ADDR)
#define MMAP_ROOTFS_SIZE		0x01A00000UL	// 26M

// define flash_rw_array[] for unlockdown
// flash_rw_array[0]: "CADDO2"+"ITUNES"+"CADDO1"+"FOTA_REV"
#define FLASH_RW_ARRAY1_START	(MMAP_CADDO_SEC_START)		// 0x00000000UL
#define FLASH_RW_ARRAY1_END		(MMAP_FATO_REV_START + MMAP_FATO_REV_SIZE)		// 0x00020000UL

// flash_rw_array[1]: "userfs_db"+"userfs_general" + "32+96 KB reserved for mnt_record and paniclog"
#define FLASH_RW_ARRAY2_START	(MMAP_USERDB_START)		// 0x00BA0000UL
#define FLASH_RW_ARRAY2_END		(MMAP_PANICLOG_START + MMAP_PANICLOG_SIZE)	// 0x019A0000UL

// flash_rw_array[2]: "tcmd"+"logo"+"fota"+"128KB reserve"
#define FLASH_RW_ARRAY3_START	(MMAP_TCMD_START)		// 0x019C0000UL
#define FLASH_RW_ARRAY3_END		(MMAP_SYS_REV_START + MMAP_SYS_REV_SIZE)	// 0x01AE0000UL

/* MACAU memory map definition: end */


/* audio related gpio */
#define GPIO_NSSP_SCLK2                 22
#define GPIO_NSSP_SFRM2                 14 
#define GPIO_NSSP_RXD2                  88
#define GPIO_NSSP_TXD2                  38

#define GPIO_ASSP_SCLK3                 52
#define GPIO_ASSP_TXD3                  81
#define GPIO_ASSP_RXD3                  82
#define GPIO_ASSP_SFRM3                 83

#define GPIO_BITCLK_IN_NSSP_MD  (GPIO_NSSP_SCLK2 | GPIO_ALT_FN_3_IN)
#define GPIO_SYNC_IN_NSSP_MD    (GPIO_NSSP_SFRM2 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_IN_NSSP_MD   (GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_NSSP_MD  (GPIO_NSSP_TXD2  | GPIO_ALT_FN_2_OUT)
#define GPIO_VA_NSSP_SWAP_MD	(GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_OUT)

#define GPIO_BITCLK_IN_ASSP_MD  (GPIO_ASSP_SCLK3 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_ASSP_MD  (GPIO_ASSP_TXD3  | GPIO_ALT_FN_1_OUT)
#define GPIO_SDATA_IN_ASSP_MD   (GPIO_ASSP_RXD3  | GPIO_ALT_FN_1_IN)
#define GPIO_SYNC_IN_ASSP_MD    (GPIO_ASSP_SFRM3 | GPIO_ALT_FN_1_IN)

#define GPIO_TFLASH_INT			11
#define GPIO_SD_NCD			    GPIO_TFLASH_INT
#define GPIO_USB20_NCS3			79
#define GPIO_HDD_USB20_READY		18
#define GPIO_USB20_INT			19
#define GPIO_USB20_DREQ0		20
#define GPIO_HDST_VGS_BOOST		33
#define GPIO_NPWE			49
#define GPIO_USB20_SWITCH		94
#define GPIO_EL_EN			89
#define GPIO_USB_READY                  99
#define GPIO_EMU_INT                    10

#define AP_13MHZ_OUTPUT_PIN     9

/* AP and BP handshake signal */
#define GPIO_BP_RDY            0       /* BP_RDY    */
#define GPIO_AP_RDY	       96      /* AP_RDY*/
#define GPIO_BB_RESET          116      /* BB_RESET   */
#define GPIO_BB_FLASHMODE_EN   41      /* notify bp to enter reflash mode  ASSERT(valid)*/

#define GPIO_BB_WDI            3      /* BB_WDI   ???  */
#define GPIO_MCU_INT_SW        41      /* MCU_INT_SW  BP flash mode enable */
#define GPIO_TC_MM_EN          120      /* TC_MM_EN  no use in barbados */

/* control PCAP direct PIN */
#define GPIO_WDI_AP            4       /* WDI_AP                       */
#define GPIO_SYS_RESTART       55      /* restart PCAP power [alias:BATT_DET_IN] */

/* communicate with PCAP's PIN  */
#define GPIO_PCAP_SEC_INT      1       /* PCAP interrupt PIN to AP     */ 
#define GPIO_SPI_CLK           29      /* PCAP SPI port clock          */ 
#define GPIO_SPI_CE            24      /* PCAP SPI port SSPFRM         */  
#define GPIO_SPI_MOSI          25      /* PCAP SPI port SSPTXD         */ 
#define GPIO_SPI_MISO          26      /* PCAP SPI port SSPRXD         */ 

/*  blue tooth control PIN   */
#define GPIO_BT_WAKEUP         57      /* AP wake up bluetooth module  */
#define GPIO_BT_HOSTWAKE       13      /* AP wake up bluetooth module  */
#define GPIO_BT_RESET          37      /* AP reset bluetooth module    */

/* control LCD high - OFF low -- ON  */
#define GPIO_LCD_OFF           78     /* control LCD                */

/*  FFUART PIN              */
/*  since barbados remove the ffuart function we swap the pin to one reserved pin 120 */
/*  we will remove the code in barbados.c in later release */
#define GPIO_ICL_FFRXD_MD      (120 )
#define GPIO_ICL_FFCTS_MD      (120 )
#define GPIO_ICL_FFTXD_MD      (120 )
#define GPIO_ICL_FFRTS_MD      (120 )

/*
 * ezx platform, wake up source edge detect bit
 */
#define PEDR_INT_SEC            1

#define GPIO_FLIP_PIN           15

/* matrix key GPIO */
#define GPIO_MATRIX_KEY_ROW0    100
#define GPIO_MATRIX_KEY_ROW1    101
#define GPIO_MATRIX_KEY_ROW2    102
#define GPIO_MATRIX_KEY_ROW3    97
#define GPIO_MATRIX_KEY_ROW4    98
#define GPIO_MATRIX_KEY_COL0    103
#define GPIO_MATRIX_KEY_COL1    104
#define GPIO_MATRIX_KEY_COL2    105
#define GPIO_MATRIX_KEY_COL3    106
#define GPIO_MATRIX_KEY_COL4    107
#define GPIO_MATRIX_KEY_COL5    108

/* MMC interface */

#define GPIO_MMC_DETECT         GPIO_TFLASH_INT 
#define GPIO_MMC_CLK            32
#define GPIO_MMC_DATA0          92
#define GPIO_MMC_WP		80 
#define GPIO_MMC_DATA1          109
#define GPIO_MMC_DATA2          110
#define GPIO_MMC_DATA3          111
#define GPIO_MMC_CMD            112

/* interface function */
#define GPIO_MMC_CLK_MD         (GPIO_MMC_CLK | GPIO_ALT_FN_2_OUT)
#define GPIO_MMC_DATA0_MD       (GPIO_MMC_DATA0 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA1_MD       (GPIO_MMC_DATA1 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA2_MD       (GPIO_MMC_DATA2 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA3_MD       (GPIO_MMC_DATA3 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

#define GPIO_MMC_CMD_MD         (GPIO_MMC_CMD | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

/* EMU GPIO 119 ---MUX2 120 --- MUX1   */
#define GPIO_EMU_MUX1      120
#define GPIO_EMU_MUX2      120 
#define GPIO_SNP_INT_CTL   120
#define GPIO_SNP_INT_IN    120

/*SDRAM*/
#define GPIO_nSDCS3        21
#define GPIO_nSDCS3_MD    (GPIO_nSDCS3 | GPIO_ALT_FN_1_OUT)

/* bp status pin */
//#define GPIO_BP_STATE       41

/* define usb related pin  */
#define GPIO34_TXENB        34 
#define GPIO35_XRXD         35 
#define GPIO36_VMOUT        36 
#define GPIO39_VPOUT        39 
#define GPIO40_VPIN         40 
#define GPIO53_VMIN         53 

/* USB client 6 pin defination */
#define GPIO34_TXENB_MD     (GPIO34_TXENB | GPIO_ALT_FN_1_OUT)
#define GPIO35_XRXD_MD      (GPIO35_XRXD | GPIO_ALT_FN_2_IN )
#define GPIO36_VMOUT_MD     (GPIO36_VMOUT | GPIO_ALT_FN_1_OUT)
#define GPIO39_VPOUT_MD     (GPIO39_VPOUT | GPIO_ALT_FN_1_OUT)
#define GPIO40_VPIN_MD      (GPIO40_VPIN | GPIO_ALT_FN_3_IN )
#define GPIO53_VMIN_MD      (GPIO53_VMIN | GPIO_ALT_FN_2_IN )

#define GPIO53_FFRXD_MD     (53 | GPIO_ALT_FN_1_IN)

/* define camera and CIF related pin  */
#define GPIO_CAM_EN         50  // CAM_EN
#define GPIO_CAM_RST        28  // CAM_RESET
#define GPIO_CAM_FLASH      17  // CAM_FLASH_PWM

#define GPIO_CIF_MCLK       23  // CAM_CLKIN
#define GPIO_CIF_PCLK       54  // CAM_CLKOUT
#define GPIO_CIF_LV         85  // CAM_HSYNC
#define GPIO_CIF_FV         84  // CAM_VSYNC
#define GPIO_CIF_DD0        27  // CAM_DATA0
#define GPIO_CIF_DD1       114  // CAM_DATA1
#define GPIO_CIF_DD2        51  // CAM_DATA2
#define GPIO_CIF_DD3       115  // CAM_DATA3
#define GPIO_CIF_DD4        95  // CAM_DATA4
#define GPIO_CIF_DD5        48  // CAM_DATA5
#define GPIO_CIF_DD6        93  // CAM_DATA6
#define GPIO_CIF_DD7        12  // CAM_DATA7

#define GPIO_CAM_EN_MD      (GPIO_CAM_EN    | GPIO_OUT)
#define GPIO_CAM_RST_MD     (GPIO_CAM_RST   | GPIO_OUT)
#define GPIO_CAM_FLASH_MD   (GPIO_CAM_FLASH | GPIO_OUT)

#define GPIO_CIF_MCLK_MD    (GPIO_CIF_MCLK  | GPIO_ALT_FN_1_OUT)
#define GPIO_CIF_PCLK_MD    (GPIO_CIF_PCLK  | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_LV_MD      (GPIO_CIF_LV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_FV_MD      (GPIO_CIF_FV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD0_MD     (GPIO_CIF_DD0   | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD1_MD     (GPIO_CIF_DD1   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD2_MD     (GPIO_CIF_DD2   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD3_MD     (GPIO_CIF_DD3   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD4_MD     (GPIO_CIF_DD4   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD5_MD     (GPIO_CIF_DD5   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD6_MD     (GPIO_CIF_DD6   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD7_MD     (GPIO_CIF_DD7   | GPIO_ALT_FN_2_IN)

#endif

#if defined(CONFIG_ARCH_EZX_PENGLAI)

#define EZX_PRODUCT_NAME		"Penglai"
#define EZX_PRODUCT_NAME_STORAGE	"Phone"
/* PENGLAI memory map definition: start */

#define MMAP_SECURE_HEADER_2KB	0x00000800UL
#define MMAP_SECURE_HEADER_4KB	0x00001000UL
#define MMAP_FLASH1_START_ADDR	0x00000000UL	// start addr of chip 1
#define MMAP_FLASH2_START_ADDR	0x02000000UL	// start addr of chip 2

// CADDO secondary, /dev/mtd0, 0x00000000-0x00008000
#define MMAP_CADDO_SEC_NAME		"CADDO_SECOND"
#define MMAP_CADDO_SEC_START	MMAP_FLASH1_START_ADDR	// 0x00000000UL
#define MMAP_CADDO_SEC_SIZE		0x00008000UL	// 32K

// itunes, /dev/mtd1, 0x00008000-0x00010000
#define MMAP_ITUNES_NAME		"ITUNES"
#define MMAP_ITUNES_START		(MMAP_CADDO_SEC_START + MMAP_CADDO_SEC_SIZE)	//0x00008000UL
#define MMAP_ITUNES_SIZE		0x00008000UL	// 32K

// CADDO primary, /dev/mtd2, 0x00010000-0x00018000
#define MMAP_CADDO_PRI_NAME		"CADDO_PRIMARY"
#define MMAP_CADDO_PRI_START	(MMAP_ITUNES_START + MMAP_ITUNES_SIZE)	//0x00010000UL
#define MMAP_CADDO_PRI_SIZE		0x00008000UL	// 32K

// FOTA reserved, /dev/mtd3, 0x00018000-0x00020000
#define MMAP_FATO_REV_NAME		"FOTA_REV"
#define MMAP_FATO_REV_START		(MMAP_CADDO_PRI_START + MMAP_CADDO_PRI_SIZE)	//0x00018000UL
#define MMAP_FATO_REV_SIZE		0x00008000UL	// 32K

// "mbm(Caddo data, Motorola Boot Module)", /dev/mtd4, 0x00020000-0x00060000
#define MMAP_MBM_NAME			"MBM"
#define MMAP_MBM_START			(MMAP_FATO_REV_START + MMAP_FATO_REV_SIZE)		//0x00020000UL
#define MMAP_MBM_SIZE			0x00040000UL	// 256K

// Config Descriptor Table, kernel configuration table, /dev/mtd?, 0x00060000-0x00080000
#define MMAP_CDT_KCT_NAME		"CONFIG"
#define MMAP_CDT_KCT_START		(MMAP_MBM_START + MMAP_MBM_SIZE)		//0x00060000UL
#define MMAP_CDT_KCT_SIZE		0x00020000UL	// 128K

// blob, /dev/mtd6, 0x00080000-0x000A0000
#define MMAP_BLOB_NAME			"BLOB"
#define MMAP_BLOB_START			(MMAP_CDT_KCT_START + MMAP_CDT_KCT_SIZE)	// 0x00080000UL
#define MMAP_BLOB_SIZE			0x00020000UL	// 128K

// kernel zImage, /dev/mtd7, 0x000A0000-0x001A0000
#define MMAP_KERNEL_NAME		"KERNEL"
#define MMAP_KERNEL_START		(MMAP_BLOB_START + MMAP_BLOB_SIZE)	// 0x000A0000UL
#define MMAP_KERNEL_SIZE		0x00100000UL	// 1M

// resource(preloaded contents), /dev/roflash2, 0x001A0000-0x00BA0000
#define ROFLASH_RESOURCE_NAME	"data_resource"
#define MMAP_RESOURCE_NAME		"RESOURCE"
#define MMAP_RESOURCE_START		(MMAP_KERNEL_START + MMAP_KERNEL_SIZE)	// 0x001A0000UL
#define MMAP_RESOURCE_CHIP_OFFSET	(MMAP_RESOURCE_START - MMAP_FLASH1_START_ADDR)
#define MMAP_RESOURCE_SIZE		0x00A00000UL	// 10M

// "user_db(NativeFS - phonebook, datebook, SMS)", /dev/mtd8, 0x00BA0000-0x011A0000
#define MMAP_USERDB_NAME		"USERFS_DB"
#define MMAP_USERDB_START		(MMAP_RESOURCE_START + MMAP_RESOURCE_SIZE)	// 0x00BA0000UL
#define MMAP_USERDB_SIZE		0x00600000UL	// 6M

// "user_general(user space)", /dev/mtd9, 0x011A0000-0x01580000
#define MMAP_USERGEN_NAME		"USERFS_GENERAL"
#define MMAP_USERGEN_START		(MMAP_USERDB_START + MMAP_USERDB_SIZE)	// 0x011A0000UL
#define MMAP_USERGEN_SIZE		0x003E0000UL	// 4M-128KB(for 32K mnt_record + 96K paniclog)

// reserved 32KB for mnt record, 0x01580000-0x01588000
#define MMAP_MNTRECORD_NAME		"MNT_RECORD"
#define MMAP_MNTRECORD_START	(MMAP_USERGEN_START + MMAP_USERGEN_SIZE)	// 0x01580000UL
#define MMAP_MNTRECORD_SIZE		0x00008000UL	// 32K

// reserved 96KB for paniclog, 0x01588000-0x015A0000
#define MMAP_PANICLOG_NAME		"PANICLOG"
#define MMAP_PANICLOG_START		(MMAP_MNTRECORD_START + MMAP_MNTRECORD_SIZE)	// 0x01588000UL
#define MMAP_PANICLOG_SIZE		0x00018000UL	// 96K

// securesetup, /dev/roflash4, 0x015A0000-0x015C0000
#define ROFLASH_SECURE_SETUP_NAME	"secure_setup"
#define MMAP_SECURE_SETUP_NAME	"SECURE_SETUP"
#define MMAP_SECURE_SETUP_START	(MMAP_PANICLOG_START + MMAP_PANICLOG_SIZE)	// 0x015A0000UL
#define MMAP_SECURE_SETUP_CHIP_OFFSET	(MMAP_SECURE_SETUP_START - MMAP_FLASH1_START_ADDR)
#define MMAP_SECURE_SETUP_SIZE	0x00020000UL	// 128K

// tcmd, /dev/mtd10, 0x015C0000-0x015E0000
#define MMAP_TCMD_NAME			"TEST_CMD"
#define MMAP_TCMD_START			(MMAP_SECURE_SETUP_START + MMAP_SECURE_SETUP_SIZE)	// 0x015C0000UL
#define MMAP_TCMD_SIZE			0x00020000UL	// 128K

// logo, /dev/mtd11, 0x015E0000-0x01600000
#define MMAP_LOGO_NAME			"LOGO"
#define MMAP_LOGO_START			(MMAP_TCMD_START + MMAP_TCMD_SIZE)	// 0x015E0000UL
#define MMAP_LOGO_SIZE			0x00020000UL	// 128K

// "fota(FOTA Update Agent, Update Package)", /dev/mtd12, 0x01600000-0x016C0000
#define MMAP_FOTA_NAME			"FOTA"
#define MMAP_FOTA_START			(MMAP_LOGO_START + MMAP_LOGO_SIZE)	// 0x01600000UL
#define MMAP_FOTA_SIZE			0x000C0000UL	// 768K

// System reserved, /dev/mtd13, 0x016C0000-0x016E0000
#define MMAP_SYS_REV_NAME		"RESERVE"
#define MMAP_SYS_REV_START		(MMAP_FOTA_START + MMAP_FOTA_SIZE)	// 0x016C0000UL
#define MMAP_SYS_REV_SIZE		0x00020000UL	// 128K

// language, /dev/roflash1, 0x016E0000-0x021E0000
#define ROFLASH_LANGUAGE_NAME	"language"
#define MMAP_LANGUAGE_NAME		"LANGUAGE"
#define MMAP_LANGUAGE_START		(MMAP_SYS_REV_START + MMAP_SYS_REV_SIZE)	// 0x016E0000UL
#define MMAP_LANGUAGE_CHIP_OFFSET	(MMAP_LANGUAGE_START - MMAP_FLASH1_START_ADDR)
#define MMAP_LANGUAGE_SIZE		0x00B00000UL	// 11M

// setup, /dev/roflash3, 0x021E0000-0x02200000
#define ROFLASH_SETUP_NAME		"setup"
#define MMAP_SETUP_NAME			"SETUP"
#define MMAP_SETUP_START		(MMAP_LANGUAGE_START + MMAP_LANGUAGE_SIZE)	// 0x021E0000UL
#define MMAP_SETUP_CHIP_OFFSET	(MMAP_SETUP_START - MMAP_FLASH2_START_ADDR)
#define MMAP_SETUP_SIZE			0x00020000UL	// 128K

// rootfs, /dev/roflash, 0x02200000-0x04000000
#define ROFLASH_ROOTFS_NAME		"rootfs"
#define MMAP_ROOTFS_NAME		"ROOTFS"
#define MMAP_ROOTFS_START		(MMAP_SETUP_START + MMAP_SETUP_SIZE)	// 0x02200000UL
#define MMAP_ROOTFS_CHIP_OFFSET	(MMAP_ROOTFS_START - MMAP_FLASH2_START_ADDR)
#define MMAP_ROOTFS_SIZE		0x01E00000UL	// 30M

// define flash_rw_array[] for unlockdown
// flash_rw_array[0]: "CADDO2"+"ITUNES"+"CADDO1"+"FOTA_REV"
#define FLASH_RW_ARRAY1_START	(MMAP_CADDO_SEC_START)		// 0x00000000UL
#define FLASH_RW_ARRAY1_END		(MMAP_FATO_REV_START + MMAP_FATO_REV_SIZE)		// 0x00020000UL

// flash_rw_array[1]: "userfs_db"+"userfs_general" + "32+96 KB reserved for mnt_record and paniclog"
#define FLASH_RW_ARRAY2_START	(MMAP_USERDB_START)		// 0x00BA0000UL
#define FLASH_RW_ARRAY2_END		(MMAP_PANICLOG_START + MMAP_PANICLOG_SIZE)	// 0x015A0000UL

// flash_rw_array[2]: "tcmd"+"logo"+"fota"+"128KB reserve"
#define FLASH_RW_ARRAY3_START	(MMAP_TCMD_START)		// 0x015C0000UL
#define FLASH_RW_ARRAY3_END		(MMAP_SYS_REV_START + MMAP_SYS_REV_SIZE)	// 0x016E0000UL

/* PENGLAI memory map definition: end */


/* audio related gpio */
#define GPIO_NSSP_SCLK2                 22
#define GPIO_NSSP_SFRM2                 14 
#define GPIO_NSSP_RXD2                  88
#define GPIO_NSSP_TXD2                  38

#define GPIO_ASSP_SCLK3                 52
#define GPIO_ASSP_TXD3                  81
#define GPIO_ASSP_RXD3                  82
#define GPIO_ASSP_SFRM3                 83

#define GPIO_BITCLK_IN_NSSP_MD  (GPIO_NSSP_SCLK2 | GPIO_ALT_FN_3_IN)
#define GPIO_SYNC_IN_NSSP_MD    (GPIO_NSSP_SFRM2 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_IN_NSSP_MD   (GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_NSSP_MD  (GPIO_NSSP_TXD2  | GPIO_ALT_FN_2_OUT)
#define GPIO_VA_NSSP_SWAP_MD	(GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_OUT)

#define GPIO_BITCLK_IN_ASSP_MD  (GPIO_ASSP_SCLK3 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_ASSP_MD  (GPIO_ASSP_TXD3  | GPIO_ALT_FN_1_OUT)
#define GPIO_SDATA_IN_ASSP_MD   (GPIO_ASSP_RXD3  | GPIO_ALT_FN_1_IN)
#define GPIO_SYNC_IN_ASSP_MD    (GPIO_ASSP_SFRM3 | GPIO_ALT_FN_1_IN)

#define GPIO_TFLASH_INT			11
#define GPIO_SD_NCD			    GPIO_TFLASH_INT
#define GPIO_DVBH_NCS3			79
#define GPIO_DVBH_READY		18
#define GPIO_DVBH_INT			19
//#define GPIO_USB20_DREQ0                20
#define GPIO_DVBH_RESET                 20
#define GPIO_DVBH_1_2_POWER             33
#define GPIO_NPWE			49
//#define GPIO_USB20_SWITCH               94
#define GPIO_DVBH_ANTENNA_SWITCH        94


#define GPIO_EL_EN			89
#define GPIO_USB_READY                  99
#define GPIO_EMU_INT                    10

#define AP_13MHZ_OUTPUT_PIN     9

/* AP and BP handshake signal */
#define GPIO_BP_RDY            0       /* BP_RDY    */
#define GPIO_AP_RDY	       96      /* AP_RDY*/
#define GPIO_BB_RESET          116      /* BB_RESET   */
#define GPIO_BB_FLASHMODE_EN   41      /* notify bp to enter reflash mode  ASSERT(valid)*/

#define GPIO_BB_WDI            3      /* BB_WDI   ???  */
#define GPIO_MCU_INT_SW        41      /* MCU_INT_SW  BP flash mode enable */
#define GPIO_TC_MM_EN          120      /* TC_MM_EN  no use in barbados */

/* control PCAP direct PIN */
#define GPIO_WDI_AP            4       /* WDI_AP                       */
#define GPIO_SYS_RESTART       55      /* restart PCAP power [alias:BATT_DET_IN] */

/* communicate with PCAP's PIN  */
#define GPIO_PCAP_SEC_INT      1       /* PCAP interrupt PIN to AP     */ 
#define GPIO_SPI_CLK           29      /* PCAP SPI port clock          */ 
#define GPIO_SPI_CE            24      /* PCAP SPI port SSPFRM         */  
#define GPIO_SPI_MOSI          25      /* PCAP SPI port SSPTXD         */ 
#define GPIO_SPI_MISO          26      /* PCAP SPI port SSPRXD         */ 

/*  blue tooth control PIN   */
#define GPIO_BT_WAKEUP         57      /* AP wake up bluetooth module  */
#define GPIO_BT_HOSTWAKE       13      /* AP wake up bluetooth module  */
#define GPIO_BT_RESET          37      /* AP reset bluetooth module    */

/* control LCD high - OFF low -- ON  */
#define GPIO_LCD_OFF           78     /* control LCD                */

/*  FFUART PIN              */
/*  since barbados remove the ffuart function we swap the pin to one reserved pin 120 */
/*  we will remove the code in barbados.c in later release */
#define GPIO_ICL_FFRXD_MD      (120 )
#define GPIO_ICL_FFCTS_MD      (120 )
#define GPIO_ICL_FFTXD_MD      (120 )
#define GPIO_ICL_FFRTS_MD      (120 )

/*
 * ezx platform, wake up source edge detect bit
 */
#define PEDR_INT_SEC            1

#define GPIO_FLIP_PIN           15

/* matrix key GPIO */
#define GPIO_MATRIX_KEY_ROW0    100
#define GPIO_MATRIX_KEY_ROW1    101
#define GPIO_MATRIX_KEY_ROW2    102
#define GPIO_MATRIX_KEY_ROW3    97
#define GPIO_MATRIX_KEY_ROW4    98
#define GPIO_MATRIX_KEY_COL0    103
#define GPIO_MATRIX_KEY_COL1    104
#define GPIO_MATRIX_KEY_COL2    105

#ifdef CONFIG_CAMERA_CLK13M
/*These two pins reserved to support camera clk change*/
//#define GPIO_MATRIX_KEY_COL3    106
//#define GPIO_MATRIX_KEY_COL4    107
#else
#define GPIO_MATRIX_KEY_COL3    106
#define GPIO_MATRIX_KEY_COL4    107
#endif
#define GPIO_MATRIX_KEY_COL5    108

#ifdef CONFIG_CAMERA_CLK13M
/* For camera clk change from AP to BP 13Mhz */
#define GPIO_CAM_APBP           106
#define GPIO_CAM_BPCLK_EN       107
#define GPIO_CAM_APBP_MD         (GPIO_CAM_APBP | GPIO_OUT)
#define GPIO_CAM_BPCLK_MD        (GPIO_CAM_BPCLK_EN | GPIO_OUT)
#endif

/* MMC interface */

#define GPIO_MMC_DETECT         GPIO_TFLASH_INT 
#define GPIO_MMC_CLK            32
#define GPIO_MMC_DATA0          92
#define GPIO_MMC_WP		80 
#define GPIO_MMC_DATA1          109
#define GPIO_MMC_DATA2          110
#define GPIO_MMC_DATA3          111
#define GPIO_MMC_CMD            112

/* interface function */
#define GPIO_MMC_CLK_MD         (GPIO_MMC_CLK | GPIO_ALT_FN_2_OUT)
#define GPIO_MMC_DATA0_MD       (GPIO_MMC_DATA0 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA1_MD       (GPIO_MMC_DATA1 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA2_MD       (GPIO_MMC_DATA2 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA3_MD       (GPIO_MMC_DATA3 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

#define GPIO_MMC_CMD_MD         (GPIO_MMC_CMD | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

/* EMU GPIO 119 ---MUX2 120 --- MUX1   */
#define GPIO_EMU_MUX1      120
#define GPIO_EMU_MUX2      120 
#define GPIO_SNP_INT_CTL   120
#define GPIO_SNP_INT_IN    120


#define GPIO_DVBH_1_8_POWER             21
/* bp status pin */
//#define GPIO_BP_STATE       41

/* define usb related pin  */
#define GPIO34_TXENB        34 
#define GPIO35_XRXD         35 
#define GPIO36_VMOUT        36 
#define GPIO39_VPOUT        39 
#define GPIO40_VPIN         40 
#define GPIO53_VMIN         53 

/* USB client 6 pin defination */
#define GPIO34_TXENB_MD     (GPIO34_TXENB | GPIO_ALT_FN_1_OUT)
#define GPIO35_XRXD_MD      (GPIO35_XRXD | GPIO_ALT_FN_2_IN )
#define GPIO36_VMOUT_MD     (GPIO36_VMOUT | GPIO_ALT_FN_1_OUT)
#define GPIO39_VPOUT_MD     (GPIO39_VPOUT | GPIO_ALT_FN_1_OUT)
#define GPIO40_VPIN_MD      (GPIO40_VPIN | GPIO_ALT_FN_3_IN )
#define GPIO53_VMIN_MD      (GPIO53_VMIN | GPIO_ALT_FN_2_IN )

#define GPIO53_FFRXD_MD     (53 | GPIO_ALT_FN_1_IN)

/* define camera and CIF related pin  */
#define GPIO_CAM_EN         50  // CAM_EN
#define GPIO_CAM_RST        28  // CAM_RESET
#define GPIO_CAM_FLASH      17  // CAM_FLASH_PWM

#define GPIO_CIF_MCLK       23  // CAM_CLKIN
#define GPIO_CIF_PCLK       54  // CAM_CLKOUT
#define GPIO_CIF_LV         85  // CAM_HSYNC
#define GPIO_CIF_FV         84  // CAM_VSYNC
#define GPIO_CIF_DD0        27  // CAM_DATA0
#define GPIO_CIF_DD1       114  // CAM_DATA1
#define GPIO_CIF_DD2        51  // CAM_DATA2
#define GPIO_CIF_DD3       115  // CAM_DATA3
#define GPIO_CIF_DD4        95  // CAM_DATA4
#define GPIO_CIF_DD5        48  // CAM_DATA5
#define GPIO_CIF_DD6        93  // CAM_DATA6
#define GPIO_CIF_DD7        12  // CAM_DATA7

#define GPIO_CAM_EN_MD      (GPIO_CAM_EN    | GPIO_OUT)
#define GPIO_CAM_RST_MD     (GPIO_CAM_RST   | GPIO_OUT)
#define GPIO_CAM_FLASH_MD   (GPIO_CAM_FLASH | GPIO_OUT)

#define GPIO_CIF_MCLK_MD    (GPIO_CIF_MCLK  | GPIO_ALT_FN_1_OUT)
#define GPIO_CIF_PCLK_MD    (GPIO_CIF_PCLK  | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_LV_MD      (GPIO_CIF_LV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_FV_MD      (GPIO_CIF_FV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD0_MD     (GPIO_CIF_DD0   | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD1_MD     (GPIO_CIF_DD1   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD2_MD     (GPIO_CIF_DD2   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD3_MD     (GPIO_CIF_DD3   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD4_MD     (GPIO_CIF_DD4   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD5_MD     (GPIO_CIF_DD5   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD6_MD     (GPIO_CIF_DD6   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD7_MD     (GPIO_CIF_DD7   | GPIO_ALT_FN_2_IN)

#endif //penglai



#if defined(CONFIG_ARCH_EZX_A780) || defined(CONFIG_ARCH_EZX_E680)

/* audio related gpio */
#define GPIO_NSSP_SCLK2                 22
#define GPIO_NSSP_SFRM2                 37
#define GPIO_NSSP_RXD2                  88
#define GPIO_NSSP_TXD2                  38

#define GPIO_ASSP_SCLK3                 52
#define GPIO_ASSP_TXD3                  81
#define GPIO_ASSP_RXD3                  89
#define GPIO_ASSP_SFRM3                 83

#define GPIO_BITCLK_IN_NSSP_MD  (GPIO_NSSP_SCLK2 | GPIO_ALT_FN_3_IN)
#define GPIO_SYNC_IN_NSSP_MD    (GPIO_NSSP_SFRM2 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_IN_NSSP_MD   (GPIO_NSSP_RXD2  | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_NSSP_MD  (GPIO_NSSP_TXD2  | GPIO_ALT_FN_2_OUT)

#define GPIO_BITCLK_IN_ASSP_MD  (GPIO_ASSP_SCLK3 | GPIO_ALT_FN_2_IN)
#define GPIO_SDATA_OUT_ASSP_MD  (GPIO_ASSP_TXD3  | GPIO_ALT_FN_1_OUT)
#define GPIO_SDATA_IN_ASSP_MD   (GPIO_ASSP_RXD3  | GPIO_ALT_FN_1_IN)
#define GPIO_SYNC_IN_ASSP_MD    (GPIO_ASSP_SFRM3 | GPIO_ALT_FN_1_IN)

#define AP_13MHZ_OUTPUT_PIN  9

#ifdef CONFIG_ARCH_EZX_E680
#define GPIO_VA_SEL_BUL     79
#define GPIO_FLT_SEL_BUL    80		/* out filter select pin */
#define GPIO_MIDI_RESET    78		/* GPIO used by MIDI chipset */
#define GPIO_MIDI_CS       33
#define GPIO_MIDI_IRQ      15
#define GPIO_MIDI_NPWE     49
#define GPIO_MIDI_RDY      18
#endif

#ifdef CONFIG_ARCH_EZX_A780
#define GPIO_HW_ATTENUATE_A780	96	/* hw noise attenuation be used or bypassed, for receiver or louderspeaker mode */
#endif



/* shakehand  with BP's PIN  */
#define GPIO_BP_RDY            0       /* BP_RDY     */
#define GPIO_BB_WDI            13      /* BB_WDI     */
#define GPIO_BB_WDI2           3       /* BB_WDI2    */
#define GPIO_BB_RESET          82      /* BB_RESET   */
#define GPIO_MCU_INT_SW        57      /* MCU_INT_SW */
#define GPIO_TC_MM_EN          99      /* TC_MM_EN   */

/* control PCAP direct PIN */
#define GPIO_WDI_AP            4       /* WDI_AP                       */
#define GPIO_SYS_RESTART       55      /* restart PCAP power           */
//#define GPIO_AP_STANDBY        28      /* make pcap enter standby mode */ 

/* communicate with PCAP's PIN  */
#define GPIO_PCAP_SEC_INT      1       /* PCAP interrupt PIN to AP     */ 
#define GPIO_SPI_CLK           29      /* PCAP SPI port clock          */ 
#define GPIO_SPI_CE            24      /* PCAP SPI port SSPFRM         */  
#define GPIO_SPI_MOSI          25      /* PCAP SPI port SSPTXD         */ 
#define GPIO_SPI_MISO          26      /* PCAP SPI port SSPRXD         */ 

/*  blue tooth control PIN   */
#define GPIO_BT_WAKEUP         28      /* AP wake up bluetooth module  */
#define GPIO_BT_HOSTWAKE       14      /* AP wake up bluetooth module  */
#define GPIO_BT_RESET          48      /* AP reset bluetooth module    */

/* control LCD high - OFF low -- ON  */
#define GPIO_LCD_OFF           116     /* control LCD                */

/*  FFUART PIN              */
#define GPIO_ICL_FFRXD_MD      (53 | GPIO_ALT_FN_1_IN)
#define GPIO_ICL_FFCTS_MD      (35 | GPIO_ALT_FN_1_IN)
#define GPIO_ICL_FFTXD_MD      (39 | GPIO_ALT_FN_2_OUT)
#define GPIO_ICL_FFRTS_MD      (41 | GPIO_ALT_FN_2_OUT)

/*
 * ezx platform, wake up source edge detect bit
 */
#define PEDR_INT_SEC            1

#define GPIO_FLIP_PIN          12
/*E680 screen lock button*/

#define GPIO_LOCK_SCREEN_PIN    GPIO_FLIP_PIN

/* MMC interface */
#define GPIO_MMC_DETECT         11
#define GPIO_MMC_CLK            32
#define GPIO_MMC_DATA0          92
#define GPIO_MMC_WP		107
#define GPIO_MMC_DATA1          109
#define GPIO_MMC_DATA2          110
#define GPIO_MMC_DATA3          111
#define GPIO_MMC_CMD            112

/* interface function */
#define GPIO_MMC_CLK_MD         (GPIO_MMC_CLK | GPIO_ALT_FN_2_OUT)
#define GPIO_MMC_DATA0_MD       (GPIO_MMC_DATA0 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA1_MD       (GPIO_MMC_DATA1 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA2_MD       (GPIO_MMC_DATA2 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)
#define GPIO_MMC_DATA3_MD       (GPIO_MMC_DATA3 | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

#define GPIO_MMC_CMD_MD         (GPIO_MMC_CMD | GPIO_ALT_FN_1_IN | GPIO_ALT_FN_1_OUT)

/* EMU GPIO 119 ---MUX2 120 --- MUX1   */
#define GPIO_EMU_MUX1      120
#define GPIO_EMU_MUX2      119
#define GPIO_SNP_INT_CTL   86
#define GPIO_SNP_INT_IN    87



/* define camera and CIF related pin  */
#define GPIO_CAM_EN         50  // CAMERA_PD
#define GPIO_CAM_RST        19  // CAMERA_RST

#define GPIO_CIF_MCLK       23  // CIF_MCLK
#define GPIO_CIF_PCLK       54  // CIF_PCLK
#define GPIO_CIF_LV         85  // CIF_HSYNC
#define GPIO_CIF_FV         84  // CIF_VSYNC
#define GPIO_CIF_DD0        27  // CIF_DATA_0
#define GPIO_CIF_DD1       114  // CIF_DATA_1
#define GPIO_CIF_DD2        51  // CIF_DATA_2
#define GPIO_CIF_DD3       115  // CIF_DATA_3
#define GPIO_CIF_DD4        95  // CIF_DATA_4
#define GPIO_CIF_DD5        94  // CIF_DATA_5
#define GPIO_CIF_DD6        17  // CIF_DATA_6
#define GPIO_CIF_DD7       108  // CIF_DATA_7

#define GPIO_CAM_EN_MD      (GPIO_CAM_EN    | GPIO_OUT)
#define GPIO_CAM_RST_MD     (GPIO_CAM_RST   | GPIO_OUT)

#define GPIO_CIF_MCLK_MD    (GPIO_CIF_MCLK  | GPIO_ALT_FN_1_OUT)
#define GPIO_CIF_PCLK_MD    (GPIO_CIF_PCLK  | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_LV_MD      (GPIO_CIF_LV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_FV_MD      (GPIO_CIF_FV    | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD0_MD     (GPIO_CIF_DD0   | GPIO_ALT_FN_3_IN)
#define GPIO_CIF_DD1_MD     (GPIO_CIF_DD1   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD2_MD     (GPIO_CIF_DD2   | GPIO_ALT_FN_1_IN)
#define GPIO_CIF_DD3_MD     (GPIO_CIF_DD3   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD4_MD     (GPIO_CIF_DD4   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD5_MD     (GPIO_CIF_DD5   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD6_MD     (GPIO_CIF_DD6   | GPIO_ALT_FN_2_IN)
#define GPIO_CIF_DD7_MD     (GPIO_CIF_DD7   | GPIO_ALT_FN_1_IN)

#endif


#if defined(CONFIG_ARCH_EZX_A780) || defined(CONFIG_ARCH_EZX_E680)

/* bp status pin */
#define GPIO_BP_STATE       41

/* define usb related pin  */
#define GPIO34_TXENB        34 
#define GPIO35_XRXD         35 
#define GPIO36_VMOUT        36 
#define GPIO39_VPOUT        39 
#define GPIO40_VPIN         40 
#define GPIO53_VMIN         53 

/* USB client 6 pin defination */
#define GPIO34_TXENB_MD     (GPIO34_TXENB | GPIO_ALT_FN_1_OUT)
#define GPIO35_XRXD_MD      (GPIO35_XRXD | GPIO_ALT_FN_2_IN )
#define GPIO36_VMOUT_MD     (GPIO36_VMOUT | GPIO_ALT_FN_1_OUT)
#define GPIO39_VPOUT_MD     (GPIO39_VPOUT | GPIO_ALT_FN_1_OUT)
#define GPIO40_VPIN_MD      (GPIO40_VPIN | GPIO_ALT_FN_3_IN )
#define GPIO53_VMIN_MD      (GPIO53_VMIN | GPIO_ALT_FN_2_IN )

#define GPIO53_FFRXD_MD     (53 | GPIO_ALT_FN_1_IN)


#endif

#define BT_HOSTWAKE	GPIO_2_x_TO_IRQ(GPIO_BT_HOSTWAKE)    /* irq for host wake up */

#ifndef EZX_PRODUCT_NAME
#define EZX_PRODUCT_NAME	"EZX PRODUCT"
#endif

#ifndef EZX_PRODUCT_NAME_STORAGE
#define EZX_PRODUCT_NAME_STORAGE	"Motorola EZX Phone"
#endif	// 

