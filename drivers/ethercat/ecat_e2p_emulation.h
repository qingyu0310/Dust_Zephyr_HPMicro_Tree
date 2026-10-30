/*
 * Copyright (c) 2024 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef ECAT_E2P_EMULATION_H
#define ECAT_E2P_EMULATION_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "hpm_common.h"

/* hpm_common.h 和 Zephyr 的 sys/util.h 都定义 ARRAY_SIZE，两者语义相同。
 * 厂商头在前，先去掉它，让后面 ecat_config.h 引入的 Zephyr 版本生效，
 * 避免 "ARRAY_SIZE redefined" 警告。 */
#ifdef ARRAY_SIZE
#undef ARRAY_SIZE
#endif

#include "eeprom_emulation.h"
#include "ecat_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* SII 存储容量（字节） */
#ifndef ESC_EEPROM_SIZE
#define ESC_EEPROM_SIZE 0x800
#endif

/* 每次 SII 擦写的字节数 */
#ifndef EEPROM_WRITE_SIZE
#define EEPROM_WRITE_SIZE 8
#endif

/* Flash 模拟的变量条目数上限 */
#ifndef EEPROM_MAX_VAR_CNT
#define EEPROM_MAX_VAR_CNT (ESC_EEPROM_SIZE)
#endif

/* eeprom_emulation 组件的调试等级 */
#ifndef E2P_DEBUG_LEVEL_WARN
#define E2P_DEBUG_LEVEL_WARN 2
#endif
#ifndef E2P_DEBUG_LEVEL
#define E2P_DEBUG_LEVEL E2P_DEBUG_LEVEL_WARN
#endif

/* SII 中产品码与版本号所在的字索引 */
#define ESC_EEPROM_PRODUCT_CODE_WORD_INDEX (0xA)
#define ESC_EEPROM_REVISION_NUM_WORD_INDEX (0xC)

/* 计算 EEPROM 配置区校验值 */
uint8_t ecat_calculate_eeprom_config_data_checksum(const uint8_t *data, size_t size);

hpm_stat_t ecat_flash_eeprom_init(void);
hpm_stat_t ecat_flash_eeprom_write(uint32_t addr, uint8_t *data);
hpm_stat_t ecat_flash_eeprom_read(uint32_t addr, uint8_t *data);
void ecat_flash_eeprom_format(void);
void ecat_flash_eeprom_flush(void);
void ecat_flash_eeprom_show_info(void);

#ifdef __cplusplus
}
#endif

#endif /* ECAT_E2P_EMULATION_H */
