# Copyright (c) 2026
# SPDX-License-Identifier: Apache-2.0

# 厂商 C 代码：EtherCAT 的 Zephyr 设备驱动与 SII Flash 模拟。
# C++ 适配层（EcatEsc / EcatSii 的实现）在 zephyr_user/drivers/ethercat/hal/。

add_subdirectory_ifdef(CONFIG_DUST_ECAT_HAL_HPM ethercat)
