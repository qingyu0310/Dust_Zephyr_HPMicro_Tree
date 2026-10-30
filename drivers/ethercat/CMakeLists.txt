# Copyright (c) 2026
# SPDX-License-Identifier: Apache-2.0

# HPMicro EtherCAT 厂商 C 代码：ESC Zephyr 设备驱动 + SII Flash 模拟。
# 本文件只列 C 源；C++ 适配层在 zephyr_user/drivers/ethercat/hal/。

zephyr_library()

zephyr_library_sources(
    esc_hpmicro.c
    ecat_e2p_emulation.c
)

zephyr_library_include_directories(
    .
    include
)

# SII Flash 模拟依赖的 HPM SDK 组件
zephyr_library_sources_ifdef(CONFIG_HPM_ESC_EEPROM_EMULATION
    ${HPM_SDK_DIR}/components/eeprom_emulation/eeprom_emulation.c
)

zephyr_library_include_directories(
    ${HPM_SDK_DIR}/components/eeprom_emulation
    ${HPM_SDK_DIR}/components/eeprom_emulation/port
)
