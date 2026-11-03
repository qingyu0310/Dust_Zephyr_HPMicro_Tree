/*
 * Copyright (c) 2024 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef ECAT_CONFIG_H
#define ECAT_CONFIG_H

/* SII 介质：1=Flash 分区模拟，0=板载 I2C EEPROM。
 * SSC 原生宏名，ecat_e2p_emulation.c 与 ecat_port_hw.h 共用这一个开关。
 * 必须先于 ecat_port_hw.h 定义，否则该头按"未定义"裁剪掉 EEPROM 引脚函数。 */
#if defined(CONFIG_DUST_ECAT_SII_FLASH)
#define ESC_EEPROM_EMULATION (1)
#elif defined(CONFIG_DUST_ECAT_SII_EEPROM)
#define ESC_EEPROM_EMULATION (0)
#endif

#include "ecat_port_hw.h"

#define FLASH_SECTOR_SIZE        (0x1000U) /* 4096 */

/* Please allocate appropriate flash space for FLASH EEPROM to avoid conflicts with other flash content */
#define FLASH_EEPROM_ADDR        (BOARD_ECAT_FLASH_EMULATE_EEPROM_ADDR)
#define FLASH_EEPROM_SIZE        (0x10000)

/* ecat flash emulate eeprom function */
#define ECAT_EEPROM_FLASH_OFFSET      (FLASH_EEPROM_ADDR)
#define ECAT_EEPROM_FLASH_SECTOR_SIZE (FLASH_SECTOR_SIZE)
#define ECAT_EEPROM_FLASH_SECTOR_CNT  (FLASH_EEPROM_SIZE / FLASH_SECTOR_SIZE) /* suitable for 2K bytes eeprom space */

/* check Product Code and Revision Number in EEPROM, if not match, will update eeprom data by included data in eeprom.h */
#define ECAT_EEPROM_CHECK_PRODUCT_CODE_AND_REVISION      (1)

/* EEPROM size in bytes, indicate the size of the actual connected eeprom */
#define ECAT_EEPROM_SIZE_BYTE         (16 * 1024)  /* 16K bytes = 128K bits */


/* ecat reset function */
#define ECAT_RESET_ESC_PERIPHERAL  0    /* ESC reset request will reset ethercat slave controller only, not suggest */
#define ECAT_RESET_ESC_WITH_MCU    1    /* ESC reset request will reset ethercat slave controller and mcu */


#endif /* ECAT_CONFIG_H */
