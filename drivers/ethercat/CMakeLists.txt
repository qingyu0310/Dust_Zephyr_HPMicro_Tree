# SPDX-License-Identifier: Apache-2.0
#
# HPMicro ESC 设备驱动 + SSC 端口层
zephyr_library()
zephyr_library_sources_ifdef(CONFIG_HPM_ESC esc_hpmicro.c)
zephyr_library_sources_ifdef(CONFIG_HPM_ESC ${HPM_SDK_DIR}/drivers/src/hpm_esc_drv.c)
zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# SSC 端口层：HW_* 宏 -> hpm_esc 驱动
if(CONFIG_DUST_COM_ETHERCAT AND CONFIG_HPM_ESC)
    # SSC 编在 app target 里，它 include 的 eeprom_emulation.h 必须对 app 可见
    zephyr_include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${HPM_SDK_DIR}/components/eeprom_emulation
        ${HPM_SDK_DIR}/components/eeprom_emulation/port
        ${HPM_SDK_DIR}/utils)
    zephyr_library_sources(
        hpm_ecat_hw.c
        hpm_ecat_e2p_emulation.c
        hpm5e31_phy.c
        ecat_zephyr_port.c
        ${HPM_SDK_DIR}/components/eeprom_emulation/eeprom_emulation.c
        ${HPM_SDK_DIR}/utils/hpm_crc32.c)
    zephyr_library_include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${FW_ROOT}/drivers/communication/ethercat/ssc)
endif()
