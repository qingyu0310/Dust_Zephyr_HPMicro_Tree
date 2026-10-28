/*
 * Copyright (c) 2025 HPMicro
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <zephyr/devicetree.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <hpm_common.h>
#include <hpm_soc.h>
#include "hpm_clock_drv.h"
#include "hpm_pcfg_drv.h"
#include "hpm_pllctlv2_drv.h"
#ifdef CONFIG_NOCACHE_MEMORY
#include <zephyr/linker/linker-defs.h>
#include "hpm_pmp_drv.h"
#endif
#ifdef CONFIG_XIP
#include "hpm_bootheader.h"
#endif

#ifdef CONFIG_XIP
#if DT_HAS_CHOSEN(zephyr_flash)
#define HPM_XPI_FLASH_NODE DT_CHOSEN(zephyr_flash)
#define HPM_NOR_CFG_OPT_HDR DT_PROP_OR(HPM_XPI_FLASH_NODE, nor_cfg_opt_hdr, 0xfcf90002)
#define HPM_NOR_CFG_OPT_OPT0 DT_PROP_OR(HPM_XPI_FLASH_NODE, nor_cfg_opt_opt0, 0x00000005)
#define HPM_NOR_CFG_OPT_OPT1 DT_PROP_OR(HPM_XPI_FLASH_NODE, nor_cfg_opt_opt1, 0x00001000)
#else
#define HPM_NOR_CFG_OPT_HDR 0xfcf90002
#define HPM_NOR_CFG_OPT_OPT0 0x00000005
#define HPM_NOR_CFG_OPT_OPT1 0x00001000
#endif

__attribute__((section(".nor_cfg_option"), used)) const uint32_t option[4] = {
    HPM_NOR_CFG_OPT_HDR,
    HPM_NOR_CFG_OPT_OPT0,
    HPM_NOR_CFG_OPT_OPT1,
    0x0
};
__attribute__((section(".last_section"))) const uint32_t rom_marker = CONFIG_LINKER_LAST_SECTION_ID_PATTERN;
#endif
__attribute__((weak)) void c_startup(void)
{
}

static void soc_init_clock(void)
{
    uint32_t cpu0_freq = clock_get_frequency(clock_cpu0);
    if (cpu0_freq == PLLCTL_SOC_PLL_REFCLK_FREQ) {
        /* Configure the External OSC ramp-up time: ~9ms */
        pllctlv2_xtal_set_rampup_time(HPM_PLLCTLV2, 32ul * 1000ul * 9u);

        /* select clock setting preset1 */
        sysctl_clock_set_preset(HPM_SYSCTL, 2);
    }
    /* Add Clocks to group 0 */
    clock_add_to_group(clock_cpu0, 0);
    clock_add_to_group(clock_mchtmr0, 0);
    clock_add_to_group(clock_ahb0, 0);
    clock_add_to_group(clock_axif, 0);
    clock_add_to_group(clock_axis, 0);
    clock_add_to_group(clock_axic, 0);
    clock_add_to_group(clock_rom0, 0);
    clock_add_to_group(clock_xpi0, 0);
    clock_add_to_group(clock_lmm0, 0);
    clock_add_to_group(clock_ram0, 0);
    clock_add_to_group(clock_hdma, 0);
    clock_add_to_group(clock_xdma, 0);
    clock_add_to_group(clock_gpio, 0);
    clock_add_to_group(clock_ptpc, 0);
    /* Motor Related */
    clock_add_to_group(clock_qei0, 0);
    clock_add_to_group(clock_plb0, 0);
    clock_add_to_group(clock_qei1, 0);
    clock_add_to_group(clock_qeo0, 0);
    clock_add_to_group(clock_qeo1, 0);
    clock_add_to_group(clock_pwm0, 0);
    clock_add_to_group(clock_pwm1, 0);
    clock_add_to_group(clock_emds, 0);
    /* Connect Group0 to CPU0 */
    clock_connect_group_to_cpu(0, 0);

    /* Bump up DCDC voltage to 1275mv */
    pcfg_dcdc_set_voltage(HPM_PCFG, 1275);
    pcfg_dcdc_switch_to_dcm_mode(HPM_PCFG);

    /* Configure PLL0 Post Divider */
    pllctlv2_set_postdiv(HPM_PLLCTLV2, pllctlv2_pll0, pllctlv2_clk0, pllctlv2_div_1p0);    /* PLL0CLK0: 480MHz */
    pllctlv2_set_postdiv(HPM_PLLCTLV2, pllctlv2_pll0, pllctlv2_clk1, pllctlv2_div_1p2);    /* PLL0CLK1: 400MHz */
    /* Configure PLL0 Frequency to 480MHz */
    pllctlv2_init_pll_with_freq(HPM_PLLCTLV2, pllctlv2_pll0, 480000000UL);
    /* CPU clock use clk_src_pll0_clk0 */
    clock_set_source_divider(clock_cpu0, clk_src_pll0_clk0, 1);
    clock_update_core_clock();

    /* Configure mchtmr to 24MHz */
    clock_set_source_divider(clock_mchtmr0, clk_src_osc24m, 1);
}


#ifdef CONFIG_NOCACHE_MEMORY
static void soc_init_pma(void)
{
    volatile uint32_t start_addr = (uint32_t) &_nocache_ram_start;
    volatile uint32_t length = (uint32_t) &_nocache_ram_size;

    if (length == 0) {
        return;
    }

    /* Ensure the address and the length are power of 2 aligned */
    assert((length & (length - 1U)) == 0U);
    assert((start_addr & (length - 1U)) == 0U);

    pmp_attr_t pmp_attrs[1] = { 0 };
    pmp_attrs[0].pmp_addr = PMP_NAPOT_ADDR(start_addr, length);
    pmp_attrs[0].pmp_cfg.val = PMP_CFG(READ_EN, WRITE_EN, EXECUTE_DIS, ADDR_MATCH_NAPOT, REG_UNLOCK);
    pmp_config_attributes(&pmp_attrs[0], ARRAY_SIZE(pmp_attrs));
}
#endif

static int hpmicro_soc_init(void)
{
	uint32_t key;

	key = irq_lock();
	soc_init_clock();
#ifdef CONFIG_NOCACHE_MEMORY
	soc_init_pma();
#endif
	irq_unlock(key);

	return 0;
}

SYS_INIT(hpmicro_soc_init, PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
