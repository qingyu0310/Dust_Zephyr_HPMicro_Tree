/*
 * Copyright (c) 2024 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* eeprom_emulation 组件的配置，由 HPM SDK 的 eeprom_emulation.h 引用 */

#ifndef _USER_CONFIG_H
#define _USER_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* SII 存储容量（字节），与 ecat_def.h 的 ESC_EEPROM_SIZE 一致 */
#define ESC_EEPROM_SIZE    0x800
#define EEPROM_MAX_VAR_CNT (ESC_EEPROM_SIZE)

#ifdef __cplusplus
}
#endif

#endif
