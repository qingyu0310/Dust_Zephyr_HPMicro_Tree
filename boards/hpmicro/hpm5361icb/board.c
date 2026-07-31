/*
 * Copyright 2025 HPMicro
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>

static int board_init(void)
{
	return 0;
}

SYS_INIT(board_init, PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);

