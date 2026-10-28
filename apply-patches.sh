#!/bin/bash
# apply-patches.sh — 应用所有官方仓库补丁（HPM 底层定制）
# 用法：
#   ./apply-patches.sh --zephyr <zephyr根> --sdk-env <sdk_env根> \
#                      --sdk-glue <sdk_glue根> --cherryusb <cherryusb根>
# 每个 patch 先 --check 后 apply，失败即 exit(1)，防止官方仓库"改一半"。
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PATCHES="$ROOT/zephyr-patches"

ZEPHYR=""; SDK_ENV=""; SDK_GLUE=""; CHERRYUSB=""
while [[ $# -gt 0 ]]; do
  case "$1" in
    --zephyr)    ZEPHYR="$2";    shift 2 ;;
    --sdk-env)   SDK_ENV="$2";   shift 2 ;;
    --sdk-glue)  SDK_GLUE="$2";  shift 2 ;;
    --cherryusb) CHERRYUSB="$2"; shift 2 ;;
    *) echo "未知参数: $1"; exit 1 ;;
  esac
done

for v in ZEPHYR SDK_ENV SDK_GLUE CHERRYUSB; do
  if [[ -z "${!v}" ]]; then
    echo "缺少 --${v,,} 参数"; exit 1
  fi
done

apply_one() {
  local repo="$1" patch="$2"
  echo "==> $patch"
  git -C "$repo" apply --check "$patch" || { echo "CHECK FAIL: $patch"; exit 1; }
  git -C "$repo" apply "$patch"       || { echo "APPLY FAIL: $patch"; exit 1; }
}

apply_one "$ZEPHYR"    "$PATCHES/zephyr/0001-intc_plic-spurious-ack.diff"
apply_one "$SDK_ENV"   "$PATCHES/sdk_env/0001-hpm_misc-dlm-ilm.diff"

apply_one "$SDK_GLUE"  "$PATCHES/sdk_glue/0001-drivers-serial-hw-rx-idle.diff"
apply_one "$SDK_GLUE"  "$PATCHES/sdk_glue/0002-drivers-can-mcan-fixes.diff"
apply_one "$SDK_GLUE"  "$PATCHES/sdk_glue/0003-drivers-gpio-interrupt.diff"
apply_one "$SDK_GLUE"  "$PATCHES/sdk_glue/0004-drivers-usb-udc-cherryusb.diff"
apply_one "$SDK_GLUE"  "$PATCHES/sdk_glue/0005-drivers-others.diff"
apply_one "$SDK_GLUE"  "$PATCHES/sdk_glue/0006-dts-hpm53xx-osc-clocks.diff"
apply_one "$SDK_GLUE"  "$PATCHES/sdk_glue/0007-soc-boards.diff"
apply_one "$SDK_GLUE"  "$PATCHES/sdk_glue/0008-soc-hpm5e31-wiring.diff"

apply_one "$CHERRYUSB" "$PATCHES/cherryusb/0001-usb_osal-array_size.diff"

echo "全部 10 个 patch 应用成功。"
