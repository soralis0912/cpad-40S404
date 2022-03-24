/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _BOARD_MX6DL_SABRESD_H
#define _BOARD_MX6DL_SABRESD_H
#include <mach/iomux-mx6dl.h>

static iomux_v3_cfg_t mx6dl_bee01_pads[] = {
	/* Boot Mode Select */
	MX6DL_PAD_EIM_LBA__GPIO_2_27,
	MX6DL_PAD_EIM_RW__GPIO_2_26,
	MX6DL_PAD_EIM_A16__GPIO_2_22,
	MX6DL_PAD_EIM_A17__GPIO_2_21,
	MX6DL_PAD_EIM_A18__GPIO_2_20,
	MX6DL_PAD_EIM_A19__GPIO_2_19,
	MX6DL_PAD_EIM_A20__GPIO_2_18,
	MX6DL_PAD_EIM_A21__GPIO_2_17,
	MX6DL_PAD_EIM_A22__GPIO_2_16,
	MX6DL_PAD_EIM_EB2__GPIO_2_30,
	MX6DL_PAD_EIM_EB3__GPIO_2_31,
	MX6DL_PAD_EIM_EB0__GPIO_2_28,
	MX6DL_PAD_EIM_EB1__GPIO_2_29,
	MX6DL_PAD_EIM_DA0__GPIO_3_0,
	MX6DL_PAD_EIM_DA1__GPIO_3_1,
	MX6DL_PAD_EIM_DA2__GPIO_3_2,
	MX6DL_PAD_EIM_DA3__GPIO_3_3,
	MX6DL_PAD_EIM_DA4__GPIO_3_4,
	MX6DL_PAD_EIM_DA5__GPIO_3_5,
	MX6DL_PAD_EIM_DA6__GPIO_3_6,
	MX6DL_PAD_EIM_DA7__GPIO_3_7,
	MX6DL_PAD_EIM_DA8__GPIO_3_8,
	MX6DL_PAD_EIM_DA9__GPIO_3_9,
	MX6DL_PAD_EIM_DA10__GPIO_3_10,
	MX6DL_PAD_EIM_DA11__GPIO_3_11,
	MX6DL_PAD_EIM_DA12__GPIO_3_12,
	MX6DL_PAD_EIM_DA13__GPIO_3_13,
	MX6DL_PAD_EIM_DA14__GPIO_3_14,
	MX6DL_PAD_EIM_DA15__GPIO_3_15,
	MX6DL_PAD_EIM_WAIT__GPIO_5_0,
	MX6DL_PAD_EIM_A24__GPIO_5_4,
	MX6DL_PAD_EIM_A23__GPIO_6_6,

	/* Power */
	MX6DL_PAD_NANDF_RB0__GPIO_6_10,	/* AUX_5V_EN */
	MX6DL_PAD_EIM_D25__GPIO_3_25,	/* PWR_BTN_SNS */
	MX6DL_PAD_GPIO_18__GPIO_7_13,	/* PMIC_INT_B */

	/* Charge */
	MX6DL_PAD_ENET_RXD0__GPIO_1_27, /* UOK_B */
	MX6DL_PAD_EIM_CS1__GPIO_2_24,   /* DOK_B */
	MX6DL_PAD_EIM_A25__GPIO_5_2,  /* FLT_1_B */
	MX6DL_PAD_EIM_D23__GPIO_3_23, /* CHG_1_B */

	/* Battery power */
	MX6DL_PAD_GPIO_19__GPIO_4_5,	/* BATT_ALRT */

	/* USB */
	MX6DL_PAD_EIM_D21__USBOH3_USBOTG_OC,
	MX6DL_PAD_EIM_D22__GPIO_3_22,
#ifdef WS1_BOARD
	MX6DL_PAD_EIM_D30__USBOH3_USBH1_OC,
#endif //WS1_BOARD
	MX6DL_PAD_ENET_RX_ER__ANATOP_USBOTG_ID,
	MX6DL_PAD_ENET_TXD1__GPIO_1_29,			/* USB_H1_PWR_EN */

	/* LCD */
	MX6DL_PAD_ENET_TXD0__GPIO_1_30,			/**/

	/* LCD backlight */
	MX6DL_PAD_SD1_DAT3__PWM1_PWMO,
	MX6DL_PAD_NANDF_WP_B__GPIO_6_9,			/* LCD_BL_EN */

	/* touch panel */
	MX6DL_PAD_EIM_D24__GPIO_3_24,

	/* LED */
	MX6DL_PAD_GPIO_2__GPIO_1_2,				/* USR_DEF_RED_LED */
	MX6DL_PAD_NANDF_D2__GPIO_2_2,			/* LED_Flashing_off */
	MX6DL_PAD_NANDF_D3__GPIO_2_3,			/* LED_GREEN */

	/* WiFi */
	MX6DL_PAD_EIM_D26__UART2_TXD,				/* BT_UART_SIN */
	MX6DL_PAD_EIM_D27__UART2_RXD,				/* BT_UART_SOUT */
	MX6DL_PAD_EIM_D28__UART2_CTS,				/* BT_UART_CTS */
	MX6DL_PAD_EIM_D29__UART2_RTS,				/* BT_UART_RTS */
	MX6DL_PAD_KEY_COL0__AUDMUX_AUD5_TXC,		/* BT_PCM_CLK */
	MX6DL_PAD_KEY_ROW0__AUDMUX_AUD5_TXD,		/* BT_PCM_DIN */
	MX6DL_PAD_KEY_COL1__AUDMUX_AUD5_TXFS,		/* BT_PCM_SYNC */
	MX6DL_PAD_KEY_ROW1__AUDMUX_AUD5_RXD,		/* BT_PCM_DOUT */
	MX6DL_PAD_GPIO_8__ANATOP_ANATOP_32K_OUT,	/* REF_CLK_32K */
	MX6DL_PAD_NANDF_CS0__GPIO_6_11,				/* WIFI_32K_EN */
	MX6DL_PAD_NANDF_CS1__GPIO_6_14,				/* WLAN_RST */
	MX6DL_PAD_NANDF_CS2__GPIO_6_15,				/* WiFi/BT_PWR_EN */
	/* USDHC2 */
	MX6DL_PAD_SD2_CMD__USDHC2_CMD,
	MX6DL_PAD_SD2_CLK__USDHC2_CLK,
	MX6DL_PAD_SD2_DAT0__USDHC2_DAT0,
	MX6DL_PAD_SD2_DAT1__USDHC2_DAT1,
	MX6DL_PAD_SD2_DAT2__USDHC2_DAT2,
	MX6DL_PAD_SD2_DAT3__USDHC2_DAT3,

	/* SD card */
	MX6DL_PAD_NANDF_D0__GPIO_2_0,			/* SD3_CD_B */
	MX6DL_PAD_NANDF_D1__GPIO_2_1,			/* SD3_WP */
	/* USDHC3 */
	MX6DL_PAD_SD3_CMD__USDHC3_CMD_50MHZ,
	MX6DL_PAD_SD3_CLK__USDHC3_CLK_50MHZ,
	MX6DL_PAD_SD3_DAT0__USDHC3_DAT0_50MHZ,
	MX6DL_PAD_SD3_DAT1__USDHC3_DAT1_50MHZ,
	MX6DL_PAD_SD3_DAT2__USDHC3_DAT2_50MHZ,
	MX6DL_PAD_SD3_DAT3__USDHC3_DAT3_50MHZ,

	/* eMMC */
	/* USDHC4 */
	MX6DL_PAD_SD4_CLK__USDHC4_CLK_50MHZ,
	MX6DL_PAD_SD4_CMD__USDHC4_CMD_50MHZ,
	MX6DL_PAD_SD4_DAT0__USDHC4_DAT0_50MHZ,
	MX6DL_PAD_SD4_DAT1__USDHC4_DAT1_50MHZ,
	MX6DL_PAD_SD4_DAT2__USDHC4_DAT2_50MHZ,
	MX6DL_PAD_SD4_DAT3__USDHC4_DAT3_50MHZ,
	MX6DL_PAD_SD4_DAT4__USDHC4_DAT4_50MHZ,
	MX6DL_PAD_SD4_DAT5__USDHC4_DAT5_50MHZ,
	MX6DL_PAD_SD4_DAT6__USDHC4_DAT6_50MHZ,
	MX6DL_PAD_SD4_DAT7__USDHC4_DAT7_50MHZ,

	/* Audio Codec */
	MX6DL_PAD_CSI0_DAT4__AUDMUX_AUD3_TXC,
	MX6DL_PAD_CSI0_DAT5__AUDMUX_AUD3_TXD,
	MX6DL_PAD_CSI0_DAT6__AUDMUX_AUD3_TXFS,
	MX6DL_PAD_CSI0_DAT7__AUDMUX_AUD3_RXD,
	MX6DL_PAD_SD3_RST__GPIO_7_8,			/* HEADPHONE_DET */
	MX6DL_PAD_KEY_COL2__GPIO_4_10,			/* CODEC_PWR_EN */
	MX6DL_PAD_GPIO_9__GPIO_1_9,				/* MICROPHONE_DET */
	MX6DL_PAD_NANDF_CS3__GPIO_6_16,			/* AUDIO_CLK_EN_B */
	MX6DL_PAD_NANDF_CLE__GPIO_6_7,			/* AMP_EN */

	/* CAMERA */
	MX6DL_PAD_NANDF_ALE__GPIO_6_8,			/* CAME_EN_B */

	/* I2C1 */
	MX6DL_PAD_CSI0_DAT8__I2C1_SDA,
	MX6DL_PAD_CSI0_DAT9__I2C1_SCL,

	/* I2C2 */
	MX6DL_PAD_KEY_COL3__I2C2_SCL,
	MX6DL_PAD_KEY_ROW3__I2C2_SDA,

	/* UART1 for debug */
	MX6DL_PAD_CSI0_DAT10__UART1_TXD,
	MX6DL_PAD_CSI0_DAT11__UART1_RXD,

	/* WDOG2_B */
	MX6DL_PAD_GPIO_1__WDOG2_WDOG_B,

	/* 3-AXIS Accelerometer */
	MX6DL_PAD_SD1_CMD__GPIO_1_18,	/* ACCL_INT1_IN */
	MX6DL_PAD_SD1_CLK__GPIO_1_20,	/* ACCL_INT2_IN */
	MX6DL_PAD_NANDF_D5__GPIO_2_5,	/* SENSOR_PWR_EN */

	/* KEY */
	MX6DL_PAD_GPIO_4__GPIO_1_4,		/* KEY_VOL_UP */
	MX6DL_PAD_GPIO_5__GPIO_1_5,		/* KEY_VOL_DN */
	MX6DL_PAD_NANDF_D4__GPIO_2_4,	/* HOME */

	/* Ver PIN*/
	MX6DL_PAD_NANDF_D6__GPIO_2_6,
	MX6DL_PAD_NANDF_D7__GPIO_2_7,

	/* test pin */
	MX6DL_PAD_GPIO_3__GPIO_1_3,
	MX6DL_PAD_GPIO_6__GPIO_1_6,
	MX6DL_PAD_GPIO_7__GPIO_1_7,
	MX6DL_PAD_SD1_DAT2__GPIO_1_19,
	MX6DL_PAD_ENET_MDIO__GPIO_1_22_PD,
	MX6DL_PAD_ENET_REF_CLK__GPIO_1_23,
	MX6DL_PAD_ENET_CRS_DV__GPIO_1_25,
	MX6DL_PAD_ENET_RXD1__GPIO_1_26,
	MX6DL_PAD_ENET_TX_EN__GPIO_1_28,
	MX6DL_PAD_ENET_MDC__GPIO_1_31,
//	MX6DL_PAD_NANDF_D6__GPIO_2_6,
//	MX6DL_PAD_NANDF_D7__GPIO_2_7,
	MX6DL_PAD_EIM_OE__GPIO_2_25,
	MX6DL_PAD_EIM_CS0__GPIO_2_23,
	MX6DL_PAD_EIM_D17__GPIO_3_17,
	MX6DL_PAD_EIM_D19__GPIO_3_19,
	MX6DL_PAD_EIM_D20__GPIO_3_20,
	MX6DL_PAD_EIM_D31__GPIO_3_31,
	MX6DL_PAD_KEY_COL4__GPIO_4_14,
	MX6DL_PAD_KEY_ROW4__GPIO_4_15,
//	MX6DL_PAD_DI0_DISP_CLK__GPIO_4_16,
	MX6DL_PAD_DI0_DISP_CLK__LCDIF_CLK,
	MX6DL_PAD_DI0_PIN15__LCDIF_ENABLE,
//	MX6DL_PAD_DI0_PIN15__GPIO_4_17,
//	MX6DL_PAD_DI0_PIN2__GPIO_4_18,
	MX6DL_PAD_DI0_PIN2__LCDIF_HSYNC,
	MX6DL_PAD_DI0_PIN3__LCDIF_VSYNC,
//	MX6DL_PAD_DI0_PIN3__GPIO_4_19,
	MX6DL_PAD_DI0_PIN4__GPIO_4_20,
	MX6DL_PAD_DISP0_DAT0__GPIO_4_21,
	MX6DL_PAD_DISP0_DAT1__GPIO_4_22,
	MX6DL_PAD_DISP0_DAT2__GPIO_4_23,
	MX6DL_PAD_DISP0_DAT3__GPIO_4_24,
	MX6DL_PAD_DISP0_DAT4__GPIO_4_25,
	MX6DL_PAD_DISP0_DAT5__GPIO_4_26,
	MX6DL_PAD_DISP0_DAT6__GPIO_4_27,
	MX6DL_PAD_DISP0_DAT7__GPIO_4_28,
	MX6DL_PAD_DISP0_DAT8__GPIO_4_29,
	MX6DL_PAD_CSI0_DATA_EN__GPIO_5_20,
	MX6DL_PAD_EIM_BCLK__GPIO_6_31,
	MX6DL_PAD_SD3_DAT6__GPIO_6_18,
	MX6DL_PAD_SD3_DAT7__GPIO_6_17,
	MX6DL_PAD_SD3_DAT4__GPIO_7_1,
	MX6DL_PAD_SD3_DAT5__GPIO_7_0,
	MX6DL_PAD_GPIO_17__GPIO_7_12,
#ifndef WS1_BOARD
	MX6DL_PAD_EIM_D30__GPIO_3_30,
#endif //WS1_BOARD

	/* open pin */
	MX6DL_PAD_EIM_D16__GPIO_3_16,
	MX6DL_PAD_EIM_D18__GPIO_3_18,
	MX6DL_PAD_KEY_ROW2__GPIO_4_11,
	MX6DL_PAD_DISP0_DAT9__GPIO_4_30,
	MX6DL_PAD_DISP0_DAT10__GPIO_4_31,
	MX6DL_PAD_DISP0_DAT11__GPIO_5_5,
	MX6DL_PAD_DISP0_DAT12__GPIO_5_6,
	MX6DL_PAD_DISP0_DAT13__GPIO_5_7,
	MX6DL_PAD_DISP0_DAT14__GPIO_5_8,
	MX6DL_PAD_DISP0_DAT15__GPIO_5_9,
	MX6DL_PAD_DISP0_DAT16__GPIO_5_10,
	MX6DL_PAD_DISP0_DAT17__GPIO_5_11,
	MX6DL_PAD_DISP0_DAT18__GPIO_5_12,
	MX6DL_PAD_DISP0_DAT19__GPIO_5_13,
	MX6DL_PAD_DISP0_DAT20__GPIO_5_14,
	MX6DL_PAD_DISP0_DAT21__GPIO_5_15,
	MX6DL_PAD_DISP0_DAT22__GPIO_5_16,
	MX6DL_PAD_DISP0_DAT23__GPIO_5_17,
	MX6DL_PAD_RGMII_TXC__GPIO_6_19,
	MX6DL_PAD_RGMII_TD0__GPIO_6_20,
	MX6DL_PAD_RGMII_TD1__GPIO_6_21,
	MX6DL_PAD_RGMII_TD2__GPIO_6_22,
	MX6DL_PAD_RGMII_TD3__GPIO_6_23,
	MX6DL_PAD_RGMII_TX_CTL__GPIO_6_26,
	MX6DL_PAD_RGMII_RXC__GPIO_6_30,
	MX6DL_PAD_RGMII_RD0__GPIO_6_25,
	MX6DL_PAD_RGMII_RD1__GPIO_6_27,
	MX6DL_PAD_RGMII_RD2__GPIO_6_28,
	MX6DL_PAD_RGMII_RD3__GPIO_6_29,
	MX6DL_PAD_RGMII_RX_CTL__GPIO_6_24,
	MX6DL_PAD_GPIO_16__GPIO_7_11,
	MX6DL_PAD_GPIO_0__CCM_CLKO,			/* Audio, Camera clock  */
};

static iomux_v3_cfg_t mx6dl_bee01_csi0_sensor_pads[] = {
	/* IPU1 Camera */
	MX6DL_PAD_CSI0_DAT12__IPU1_CSI0_D_12,
	MX6DL_PAD_CSI0_DAT13__IPU1_CSI0_D_13,
	MX6DL_PAD_CSI0_DAT14__IPU1_CSI0_D_14,
	MX6DL_PAD_CSI0_DAT15__IPU1_CSI0_D_15,
	MX6DL_PAD_CSI0_DAT16__IPU1_CSI0_D_16,
	MX6DL_PAD_CSI0_DAT17__IPU1_CSI0_D_17,
	MX6DL_PAD_CSI0_DAT18__IPU1_CSI0_D_18,
	MX6DL_PAD_CSI0_DAT19__IPU1_CSI0_D_19,
	MX6DL_PAD_CSI0_MCLK__IPU1_CSI0_HSYNC,
	MX6DL_PAD_CSI0_PIXCLK__IPU1_CSI0_PIXCLK,
	MX6DL_PAD_CSI0_VSYNC__IPU1_CSI0_VSYNC,

	MX6DL_PAD_SD1_DAT0__GPIO_1_16,		/* camera PWDN */
	MX6DL_PAD_SD1_DAT1__GPIO_1_17,		/* camera RESET */
};


#endif
