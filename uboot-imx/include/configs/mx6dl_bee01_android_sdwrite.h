/*
 * Copyright (C) 2010-2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the MX6DL SabreSD Freescale board.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef MX6Q_SABRESD_ANDROID_H
#define MX6Q_SABRESD_ANDROID_H

#include <asm/arch/mx6.h>
#include "mx6dl_bee01.h"

//#define CONFIG_USB_DEVICE
//#define CONFIG_IMX_UDC		       1
//#define CONFIG_FASTBOOT		       1
#define CONFIG_FASTBOOT_STORAGE_EMMC_SATA
#define CONFIG_FASTBOOT_VENDOR_ID      0x18d1
#define CONFIG_FASTBOOT_PRODUCT_ID     0x0d02
#define CONFIG_FASTBOOT_BCD_DEVICE     0x311
#define CONFIG_FASTBOOT_MANUFACTURER_STR  "Freescale"
#define CONFIG_FASTBOOT_PRODUCT_NAME_STR "i.mx6dl Sabre SmartDevice"
#define CONFIG_FASTBOOT_INTERFACE_STR	 "Android fastboot"
#define CONFIG_FASTBOOT_CONFIGURATION_STR  "Android fastboot"
#define CONFIG_FASTBOOT_SERIAL_NUM	"12345"
#define CONFIG_FASTBOOT_SATA_NO		 0
#define CONFIG_BATTERY_GUARD_SOC      0
#define CONFIG_BATTERY_GUARD_SOC_AC   -1
#define POWERBTN_DELAY_TIME           500000
#define RECOVERY_BTN_HOLD_TIME        5000000

/*  For system.img growing up more than 256MB, more buffer needs
*   to receive the system.img*/
#define CONFIG_FASTBOOT_TRANSFER_BUF	0x2c000000
#define CONFIG_FASTBOOT_TRANSFER_BUF_SIZE 0x14000000 /* 320M byte */


#define CONFIG_CMD_BOOTI
#define CONFIG_ANDROID_RECOVERY
/* which mmc bus is your main storage ? */
#define CONFIG_ANDROID_MAIN_MMC_BUS 3

#define CONFIG_ANDROID_BOOT_PARTITION_MMC 7
#define CONFIG_ANDROID_SYSTEM_PARTITION_MMC 5
#define CONFIG_ANDROID_RECOVERY_PARTITION_MMC 2
#define CONFIG_ANDROID_CACHE_PARTITION_MMC 6
#define CONFIG_ANDROID_FACTORY 8


#define CONFIG_ANDROID_RECOVERY_BOOTARGS_MMC NULL
#define CONFIG_ANDROID_RECOVERY_BOOTCMD_MMC  \
	"booti mmc3 recovery"
#define CONFIG_ANDROID_RECOVERY_CMD_FILE "/recovery/command"
#define CONFIG_ANDROID_FACTORY_SDTESTCARD_FILE "sdtestcard"
#define CONFIG_ANDROID_FACTORY_BOOTCMD_SD "booti mmc2"
#define CONFIG_INITRD_TAG

#undef CONFIG_LOADADDR
#undef CONFIG_RD_LOADADDR
#undef CONFIG_EXTRA_ENV_SETTINGS


#define CONFIG_LOADADDR		0x10800000	/* loadaddr env var */
#define CONFIG_RD_LOADADDR      0x11000000

#define CONFIG_INITRD_TAG
#ifdef WS1
#define	CONFIG_EXTRA_ENV_SETTINGS					\
		"netdev=eth0\0"						\
		"ethprime=FEC0\0"					\
		"fastboot_dev=mmc3\0"					\
		"bootcmd=booti mmc3\0"					\
		"splashimage=0x30000000\0"				\
		"splashpos=m,m\0"					\
		"autorun=/system/bin/install_to_emmc_autorun\0"	\
		"lvds_num=0\0"
#else
#define	CONFIG_EXTRA_ENV_SETTINGS					\
		"netdev=eth0\0"						\
		"ethprime=FEC0\0"					\
		"fastboot_dev=mmc3\0"					\
		"bootcmd=booti mmc3\0"					\
		"splashimage=0x30000000\0"				\
		"splashpos=m,m\0"					\
		"autorun=/system/bin/install_to_emmc_autorun\0"	\
		"lvds_num=1\0"
#endif
#undef CONFIG_MD5
#undef CONFIG_RSA
#undef CONFIG_STOP_AUTH_ERROR
#define CONFIG_SDWRITE
#endif
