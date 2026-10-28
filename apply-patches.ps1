# apply-patches.ps1 — 应用所有官方仓库补丁（Windows 本地）
# 用法：
#   .\apply-patches.ps1 -Zephyr E:\Zephyr\zephyr -SdkEnv E:\Zephyr_HPMicro\sdk_env `
#     -SdkGlue E:\Zephyr_HPMicro\sdk_glue -CherryUsb E:\Zephyr_HPMicro\modules\lib\CherryUSB
param(
  [string]$Zephyr,
  [string]$SdkEnv,
  [string]$SdkGlue,
  [string]$CherryUsb
)
$ErrorActionPreference = "Stop"
$Root    = Split-Path -Parent $MyInvocation.MyCommand.Path
$Patches = Join-Path $Root "zephyr-patches"

foreach ($v in @("Zephyr","SdkEnv","SdkGlue","CherryUsb")) {
  if (-not (Get-Variable $v -ValueOnly)) { throw "缺少 -$v 参数" }
}

function Apply-One([string]$Repo, [string]$Patch) {
  Write-Host "==> $Patch"
  git -C $Repo apply --check $Patch
  if ($LASTEXITCODE -ne 0) { throw "CHECK FAIL: $Patch" }
  git -C $Repo apply $Patch
  if ($LASTEXITCODE -ne 0) { throw "APPLY FAIL: $Patch" }
}

Apply-One $Zephyr    (Join-Path $Patches "zephyr/0001-intc_plic-spurious-ack.diff")
Apply-One $SdkEnv    (Join-Path $Patches "sdk_env/0001-hpm_misc-dlm-ilm.diff")
Apply-One $SdkGlue   (Join-Path $Patches "sdk_glue/0001-drivers-serial-hw-rx-idle.diff")
Apply-One $SdkGlue   (Join-Path $Patches "sdk_glue/0002-drivers-can-mcan-fixes.diff")
Apply-One $SdkGlue   (Join-Path $Patches "sdk_glue/0003-drivers-gpio-interrupt.diff")
Apply-One $SdkGlue   (Join-Path $Patches "sdk_glue/0004-drivers-usb-udc-cherryusb.diff")
Apply-One $SdkGlue   (Join-Path $Patches "sdk_glue/0005-drivers-others.diff")
Apply-One $SdkGlue   (Join-Path $Patches "sdk_glue/0006-dts-hpm53xx-osc-clocks.diff")
Apply-One $SdkGlue   (Join-Path $Patches "sdk_glue/0007-soc-boards.diff")
Apply-One $SdkGlue (Join-Path $Patches "sdk_glue/0008-soc-hpm5e31-wiring.diff")
Apply-One $CherryUsb (Join-Path $Patches "cherryusb/0001-usb_osal-array_size.diff")

Write-Host "全部 10 个 patch 应用成功。"
