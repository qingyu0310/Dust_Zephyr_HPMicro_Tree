# Copyright (c) 2026
# SPDX-License-Identifier: Apache-2.0
#
# 用户层 HPM 覆盖模块：只补充本架构需要的驱动，不重复 sdk_glue 的 soc/boards/dts。
# boards/dts/soc 仍由 project/CMakeLists.txt 的 BOARD_ROOT/SOC_ROOT/DTS_ROOT 引入。

add_subdirectory(drivers)
