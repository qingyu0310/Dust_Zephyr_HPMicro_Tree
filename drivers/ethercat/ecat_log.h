/*
 * Copyright (c) 2026 qingyu
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

/* C 侧诊断输出宏。C++ 侧用 ecat_log.hpp，两者转发到同一个 ecat_port_log() */

#ifndef ECAT_LOG_H
#define ECAT_LOG_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 端口层诊断输出，实现在 ecat_phy_mdio.cpp */
void ecat_port_log(const char *text);

/**
 * @brief 格式化并输出一条诊断
 */
#define ECAT_LOG(...)                                          \
	do {                                                       \
		char _ecat_text[160];                                  \
		snprintf(_ecat_text, sizeof(_ecat_text), __VA_ARGS__); \
		ecat_port_log(_ecat_text);                             \
	} while (0)

#ifdef __cplusplus
}
#endif

#endif /* ECAT_LOG_H */
