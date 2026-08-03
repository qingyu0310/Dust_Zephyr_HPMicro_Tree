# sdk_glue_user — HPM 侧用户自研层（与 sdk_glue 同级的小树）

> **SELF-MAINTAINED — qingyu**
> 这是与 sdk_glue / sdk_env 同级的用户自研层（`<workspace>/sdk_glue_user`，workspace 如 `E:\Zephyr_HPMicro`），
> 结构镜像 sdk_glue（`boards/`、`soc/`、`dts/`…），只包含用户自研/移植的**独立新增文件**，**独立 git 仓库管理**。
>
> **不污染官方 sdk**：sdk_glue / sdk_env 官方仓库保持干净，这里只放"新增独立文件"。
> **修改官方文件的部分**（底层 bug 修复等）走 patch（归档在 `zephyr-patches/`），apply 到官方仓库。

## 结构

```
sdk_glue_user/
├── boards/         用户自研板卡（boards/<vendor>/<name>/）
├── soc/            用户自研 SoC 支持（soc/<vendor>/<family>/）
├── dts/bindings/   用户自研 devicetree binding
└── README.md
```

## 内容清单

| 内容 | 位置 | 说明 |
| --- | --- | --- |
| hpm5361icb 板卡 | `boards/hpmicro/hpm5361icb/` | HPM5361 自制核心板（HPM5300 系列） |
| HPM5300 SoC 支持 | `soc/hpmicro/HPM5300/` | soc.c/soc.h/start.S/linker.ld/Kconfig* |
| PLLv2 binding | `dts/bindings/clock/hpmicro,hpm-pllv2.yaml` | 自研 binding |

## 引用机制

应用工程 CMakeLists 通过 `BOARD_ROOT` / `SOC_ROOT` / `DTS_ROOT` 引用本目录：

```cmake
list(APPEND BOARD_ROOT "<workspace>/sdk_glue_user")
list(APPEND SOC_ROOT   "<workspace>/sdk_glue_user")
list(APPEND DTS_ROOT   "<workspace>/sdk_glue_user")
list(APPEND DTS_ROOT   "<workspace>/sdk_glue_user/dts")
```

之后 `west build -b hpm5361icb` 可直接按板名找到。

## 管理方式

- 本目录是**独立 git 仓库**（`sdk_glue_user/.git`）。
- 新增板卡/SoC/binding 在本目录内 `git add` / `git commit`，`git push` 到用户自己的远端仓库。
- 与 sdk_glue / sdk_env 完全独立，官方仓库 git 不受影响。

## 底层修改 patch 机制

修改官方仓库的部分（不污染官方 sdk）归档在 `zephyr-patches/`，构建时 apply：

- `zephyr/`   → zephyr 官方树（intc_plic.c PLIC 伪中断）
- `sdk_env/`  → HPM sdk_env（hpm_misc.h DLM/ILM 地址换算）
- `sdk_glue/` → HPM sdk_glue（UART RX idle / CAN / GPIO / USB / dts / soc-boards 共 34 文件）
- `cherryusb/`→ CherryUSB（usb_osal_zephyr.c ARRAY_SIZE 宏冲突）

用法（Linux / CI）：`./apply-patches.sh --zephyr <zephyr> --sdk-env <sdk_env> --sdk-glue <sdk_glue> --cherryusb <cherryusb>`
用法（Windows）：`.\apply-patches.ps1 -Zephyr ... -SdkEnv ... -SdkGlue ... -CherryUsb ...`

重新生成 patch：改完官方仓库后 `git diff > zephyr-patches/<repo>/NNN-<desc>.diff` 更新对应文件。

## 备注

- 底层 bug 修复（如 hpm_sdk `hpm_misc.h` 的 DLM/ILM 地址转换、drivers 修改、dtsi 修改）**必须改官方文件**，以 patch 形式管理（见 `zephyr-patches/`）。
- 本层只放"新增独立文件"，避免别人 clone 官方后找不到用户加的东西。
