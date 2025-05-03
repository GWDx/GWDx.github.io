---
title: "解决 Linux 上 HDMI 无法使用的问题"
date: 2022-11-19
keywords: ["Linux", "HDMI", "i915"]
description: ""
tags: ["Linux", "HDMI", "i915"]
categories: []
author: ""
summary: ""

comment: true
toc: false
autoCollapseToc: false
contentCopyright: false
reward: false
katex: false
---

前段时间整了个显示器，Windows, grub 时是正常的，但进 Linux 后就无法使用了。

<!--more-->

## 问题描述

环境：

- CPU：Intel i5-12500H
- 操作系统：Debian 12


`xrandr` 会显示

```
HDMI-1 disconnected
```

按照 Google 上的一些结果，进行了重装显卡驱动等操作，都不太行得通。

还发现 Debian 11 可以输出 HDMI 的，然而这个版本无法调节亮度。

## 更多信息

后来，想起 taoky 的博客里有一篇提到因为 i915 无法使用 HDMI 的问题：[调试一则：让 i915 在 ThinkPad T14 Gen3 上正常工作](https://blog.taoky.moe/2022-07-21/i915-debug-on-tpt14gen3.html)

尝试关闭 i915 内核模块，发现和 Debian 11 一样可以输出 HDMI 但不能调亮度。可以肯定是 i915 的问题。

然后按照博客，编译安装最新版本的内核 `6.1.0-rc5+`，新版内核还没有修复这个问题。

查看开机后的 `sudo dmesg` 发现问题不同，我的里有 3 个 `cut here`

<details open>

<summary>dmesg 中的报错信息</summary>

```
[    2.991664] i915 0000:00:02.0: [drm] [ENCODER:244:DDI B/PHY B] failed to retrieve link info, disabling eDP
[    2.992744] i915 0000:00:02.0: [drm] *ERROR* crtc 131: Can't calculate constants, dotclock = 0!
[    2.992750] ------------[ cut here ]------------
[    2.992751] i915 0000:00:02.0: drm_WARN_ON_ONCE(drm_drv_uses_atomic_modeset(dev))
[    2.992760] WARNING: CPU: 2 PID: 241 at drivers/gpu/drm/drm_vblank.c:728 drm_crtc_vblank_helper_get_vblank_timestamp_internal+0x346/0x360 [drm]
[    2.992780] Modules linked in: i915(+) usbhid i2c_algo_bit drm_buddy drm_display_helper drm_kms_helper ahci nvme libahci nvme_core libata xhci_pci cec xhci_hcd rc_core r8169 ttm t10_pi hid_generic crc64_rocksoft crc64 drm usbcore scsi_mod realtek mdio_devres libphy psmouse i2c_hid_acpi i2c_hid crc_t10dif crc32c_intel intel_lpss_pci hid i2c_i801 crct10dif_generic intel_lpss crct10dif_pclmul i2c_smbus usb_common video idma64 scsi_common crct10dif_common wmi
[    2.992792] CPU: 2 PID: 241 Comm: systemd-udevd Not tainted 6.1.0-rc5+ #13
[    2.992794] Hardware name: HASEE Computer V15x_V17xPNKPNJPNH/V15x_V17xPNKPNJPNH, BIOS 1.07.06THZX 02/15/2022
[    2.992794] RIP: 0010:drm_crtc_vblank_helper_get_vblank_timestamp_internal+0x346/0x360 [drm]
[    2.992807] Code: 48 8b 5f 50 48 85 db 75 03 48 8b 1f e8 33 15 57 fb 48 c7 c1 10 40 95 c0 48 89 da 48 c7 c7 81 68 95 c0 48 89 c6 e8 53 b2 83 fb <0f> 0b e9 c0 fd ff ff e8 4e 09 88 fb 66 66 2e 0f 1f 84 00 00 00 00
[    2.992808] RSP: 0018:ffffaa67409a3910 EFLAGS: 00010086
[    2.992809] RAX: 0000000000000000 RBX: ffff997bc1d015a0 RCX: 0000000000000000
[    2.992810] RDX: 0000000000000003 RSI: 00000000ffffefff RDI: 00000000ffffffff
[    2.992810] RBP: ffffaa67409a3980 R08: 0000000000000000 R09: ffffaa67409a3798
[    2.992811] R10: 0000000000000003 R11: ffffffffbccd1848 R12: ffffaa67409a39c8
[    2.992811] R13: ffffffffc0a7d850 R14: ffff997bcd928140 R15: ffff997be39f79d8
[    2.992812] FS:  00007fac53a28d00(0000) GS:ffff997f2f480000(0000) knlGS:0000000000000000
[    2.992813] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[    2.992813] CR2: 0000558c41e095a0 CR3: 00000004313fe006 CR4: 0000000000770ee0
[    2.992814] PKRU: 55555554
[    2.992814] Call Trace:
[    2.992816]  <TASK>
[    2.992817]  drm_get_last_vbltimestamp+0xaa/0xc0 [drm]
[    2.992830]  drm_reset_vblank_timestamp+0x61/0xd0 [drm]
[    2.992842]  drm_crtc_vblank_on+0x85/0x130 [drm]
[    2.992854]  intel_crtc_vblank_on+0x2f/0x80 [i915]
[    2.992927]  intel_modeset_setup_hw_state+0x7f7/0x1500 [i915]
[    2.992984]  ? intel_modeset_init_nogem+0x331/0x1130 [i915]
[    2.993036]  intel_modeset_init_nogem+0x331/0x1130 [i915]
[    2.993084]  ? preempt_count_add+0x6a/0xa0
[    2.993087]  ? _raw_spin_unlock_irqrestore+0x23/0x40
[    2.993089]  ? fwtable_read32+0x96/0x220 [i915]
[    2.993134]  i915_driver_probe+0x5b4/0xe20 [i915]
[    2.993175]  ? i915_pci_probe+0x97/0x1d0 [i915]
[    2.993211]  local_pci_probe+0x3e/0x80
[    2.993214]  pci_device_probe+0xc3/0x230
[    2.993216]  ? sysfs_do_create_link_sd+0x6e/0xe0
[    2.993218]  really_probe+0xdb/0x380
[    2.993220]  ? pm_runtime_barrier+0x50/0x90
[    2.993222]  __driver_probe_device+0x78/0x170
[    2.993223]  driver_probe_device+0x1f/0x90
[    2.993224]  __driver_attach+0xd1/0x1d0
[    2.993225]  ? __device_attach_driver+0x110/0x110
[    2.993225]  bus_for_each_dev+0x84/0xd0
[    2.993227]  bus_add_driver+0x1ae/0x200
[    2.993229]  driver_register+0x89/0xe0
[    2.993230]  i915_init+0x1f/0x7f [i915]
[    2.993272]  ? 0xffffffffc0d65000
[    2.993273]  do_one_initcall+0x56/0x220
[    2.993276]  do_init_module+0x4a/0x1f0
[    2.993278]  __do_sys_finit_module+0xac/0x120
[    2.993280]  do_syscall_64+0x37/0xc0
[    2.993283]  entry_SYSCALL_64_after_hwframe+0x63/0xcd
[    2.993284] RIP: 0033:0x7fac5412c5a9
[    2.993285] Code: 08 89 e8 5b 5d c3 66 2e 0f 1f 84 00 00 00 00 00 90 48 89 f8 48 89 f7 48 89 d6 48 89 ca 4d 89 c2 4d 89 c8 4c 8b 4c 24 08 0f 05 <48> 3d 01 f0 ff ff 73 01 c3 48 8b 0d 27 08 0d 00 f7 d8 64 89 01 48
[    2.993286] RSP: 002b:00007ffea2cedfb8 EFLAGS: 00000246 ORIG_RAX: 0000000000000139
[    2.993287] RAX: ffffffffffffffda RBX: 0000558c43b44a50 RCX: 00007fac5412c5a9
[    2.993288] RDX: 0000000000000000 RSI: 00007fac542beefd RDI: 0000000000000012
[    2.993289] RBP: 00007fac542beefd R08: 0000000000000000 R09: 0000000000000000
[    2.993289] R10: 0000000000000012 R11: 0000000000000246 R12: 0000000000020000
[    2.993290] R13: 0000000000000000 R14: 0000558c43b488d0 R15: 0000558c41d98e50
[    2.993291]  </TASK>
[    2.993291] ---[ end trace 0000000000000000 ]---
[    2.998078] i915 0000:00:02.0: [drm] GuC firmware i915/adlp_guc_70.bin version 70.5.1
[    2.998081] i915 0000:00:02.0: [drm] HuC firmware i915/tgl_huc.bin version 7.9.3
[    3.052258] i915 0000:00:02.0: [drm] HuC authenticated
[    3.053160] i915 0000:00:02.0: [drm] GuC submission enabled
[    3.053164] i915 0000:00:02.0: [drm] GuC SLPC enabled
[    3.053618] i915 0000:00:02.0: [drm] GuC RC: enabled
[    3.268602] ------------[ cut here ]------------
[    3.268610] i915 0000:00:02.0: drm_WARN_ON(intel_dp->pps.vdd_wakeref)
[    3.268643] WARNING: CPU: 2 PID: 241 at drivers/gpu/drm/i915/display/intel_pps.c:597 intel_pps_vdd_on_unlocked+0x2a8/0x2c0 [i915]
[    3.269065] Modules linked in: i915(+) usbhid i2c_algo_bit drm_buddy drm_display_helper drm_kms_helper ahci nvme libahci nvme_core libata xhci_pci cec xhci_hcd rc_core r8169 ttm t10_pi hid_generic crc64_rocksoft crc64 drm usbcore scsi_mod realtek mdio_devres libphy psmouse i2c_hid_acpi i2c_hid crc_t10dif crc32c_intel intel_lpss_pci hid i2c_i801 crct10dif_generic intel_lpss crct10dif_pclmul i2c_smbus usb_common video idma64 scsi_common crct10dif_common wmi
[    3.269144] CPU: 2 PID: 241 Comm: systemd-udevd Tainted: G        W          6.1.0-rc5+ #13
[    3.269154] Hardware name: HASEE Computer V15x_V17xPNKPNJPNH/V15x_V17xPNKPNJPNH, BIOS 1.07.06THZX 02/15/2022
[    3.269157] RIP: 0010:intel_pps_vdd_on_unlocked+0x2a8/0x2c0 [i915]
[    3.269483] Code: 4c 8b 67 50 4d 85 e4 75 03 4c 8b 27 e8 81 d9 2c fb 48 c7 c1 78 fd c5 c0 4c 89 e2 48 c7 c7 2d 33 c7 c0 48 89 c6 e8 a1 76 59 fb <0f> 0b e9 e7 fd ff ff e8 9c cd 5d fb 66 66 2e 0f 1f 84 00 00 00 00
[    3.269489] RSP: 0018:ffffaa67409a36c0 EFLAGS: 00010286
[    3.269496] RAX: 0000000000000000 RBX: ffff997c029ca170 RCX: 0000000000000000
[    3.269500] RDX: 0000000000000001 RSI: 00000000ffffefff RDI: 00000000ffffffff
[    3.269504] RBP: ffff997bcd928000 R08: 0000000000000000 R09: ffffaa67409a3548
[    3.269507] R10: 0000000000000003 R11: ffffffffbccd1848 R12: ffff997bc1d015a0
[    3.269510] R13: 0000000000000005 R14: ffff997bcd929c90 R15: 0000000000000000
[    3.269513] FS:  00007fac53a28d00(0000) GS:ffff997f2f480000(0000) knlGS:0000000000000000
[    3.269518] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[    3.269522] CR2: 0000558c41e095a0 CR3: 00000004313fe006 CR4: 0000000000770ee0
[    3.269527] PKRU: 55555554
[    3.269530] Call Trace:
[    3.269538]  <TASK>
[    3.269542]  ? __intel_display_power_get_domain.part.0+0x52/0x70 [i915]
[    3.269914]  ? intel_display_power_get+0x4e/0x60 [i915]
[    3.270257]  intel_dp_aux_xfer+0x127/0x7a0 [i915]
[    3.270582]  ? __schedule+0x346/0xa00
[    3.270598]  ? schedule_hrtimeout_range_clock+0xdf/0x120
[    3.270608]  intel_dp_aux_transfer+0x201/0x320 [i915]
[    3.270881]  drm_dp_dpcd_access+0xaa/0x130 [drm_display_helper]
[    3.270921]  drm_dp_dpcd_write+0x89/0xd0 [drm_display_helper]
[    3.270954]  intel_dp_set_power+0x63/0x180 [i915]
[    3.271229]  intel_ddi_post_disable+0x47b/0x4d0 [i915]
[    3.271487]  intel_encoders_post_disable+0x7b/0x90 [i915]
[    3.271778]  intel_old_crtc_state_disables+0x38/0xa0 [i915]
[    3.272039]  intel_atomic_commit_tail+0x398/0xea0 [i915]
[    3.272288]  intel_atomic_commit+0x34f/0x390 [i915]
[    3.272516]  drm_atomic_commit+0x93/0xc0 [drm]
[    3.272574]  ? drm_plane_get_damage_clips.cold+0x1c/0x1c [drm]
[    3.272634]  intel_modeset_init+0x19c/0x280 [i915]
[    3.272878]  i915_driver_probe+0x5da/0xe20 [i915]
[    3.273087]  ? i915_pci_probe+0x97/0x1d0 [i915]
[    3.273277]  local_pci_probe+0x3e/0x80
[    3.273290]  pci_device_probe+0xc3/0x230
[    3.273296]  ? sysfs_do_create_link_sd+0x6e/0xe0
[    3.273307]  really_probe+0xdb/0x380
[    3.273311]  ? pm_runtime_barrier+0x50/0x90
[    3.273319]  __driver_probe_device+0x78/0x170
[    3.273323]  driver_probe_device+0x1f/0x90
[    3.273326]  __driver_attach+0xd1/0x1d0
[    3.273330]  ? __device_attach_driver+0x110/0x110
[    3.273333]  bus_for_each_dev+0x84/0xd0
[    3.273341]  bus_add_driver+0x1ae/0x200
[    3.273349]  driver_register+0x89/0xe0
[    3.273353]  i915_init+0x1f/0x7f [i915]
[    3.273540]  ? 0xffffffffc0d65000
[    3.273543]  do_one_initcall+0x56/0x220
[    3.273552]  do_init_module+0x4a/0x1f0
[    3.273560]  __do_sys_finit_module+0xac/0x120
[    3.273570]  do_syscall_64+0x37/0xc0
[    3.273577]  entry_SYSCALL_64_after_hwframe+0x63/0xcd
[    3.273584] RIP: 0033:0x7fac5412c5a9
[    3.273588] Code: 08 89 e8 5b 5d c3 66 2e 0f 1f 84 00 00 00 00 00 90 48 89 f8 48 89 f7 48 89 d6 48 89 ca 4d 89 c2 4d 89 c8 4c 8b 4c 24 08 0f 05 <48> 3d 01 f0 ff ff 73 01 c3 48 8b 0d 27 08 0d 00 f7 d8 64 89 01 48
[    3.273591] RSP: 002b:00007ffea2cedfb8 EFLAGS: 00000246 ORIG_RAX: 0000000000000139
[    3.273596] RAX: ffffffffffffffda RBX: 0000558c43b44a50 RCX: 00007fac5412c5a9
[    3.273599] RDX: 0000000000000000 RSI: 00007fac542beefd RDI: 0000000000000012
[    3.273601] RBP: 00007fac542beefd R08: 0000000000000000 R09: 0000000000000000
[    3.273603] R10: 0000000000000012 R11: 0000000000000246 R12: 0000000000020000
[    3.273605] R13: 0000000000000000 R14: 0000558c43b488d0 R15: 0000558c41d98e50
[    3.273610]  </TASK>
[    3.273611] ---[ end trace 0000000000000000 ]---
[    3.626200] input: ImPS/2 Logitech Wheel Mouse as /devices/platform/i8042/serio1/input/input2
[    4.161000] ------------[ cut here ]------------
[    4.161009] crtc active state doesn't match with hw state (expected 0, found 1)
[    4.161039] WARNING: CPU: 0 PID: 241 at drivers/gpu/drm/i915/display/intel_modeset_verify.c:184 intel_modeset_verify_crtc+0x3d9/0x400 [i915]
[    4.161501] Modules linked in: i915(+) usbhid i2c_algo_bit drm_buddy drm_display_helper drm_kms_helper ahci nvme libahci nvme_core libata xhci_pci cec xhci_hcd rc_core r8169 ttm t10_pi hid_generic crc64_rocksoft crc64 drm usbcore scsi_mod realtek mdio_devres libphy psmouse i2c_hid_acpi i2c_hid crc_t10dif crc32c_intel intel_lpss_pci hid i2c_i801 crct10dif_generic intel_lpss crct10dif_pclmul i2c_smbus usb_common video idma64 scsi_common crct10dif_common wmi
[    4.161591] CPU: 0 PID: 241 Comm: systemd-udevd Tainted: G        W          6.1.0-rc5+ #13
[    4.161601] Hardware name: HASEE Computer V15x_V17xPNKPNJPNH/V15x_V17xPNKPNJPNH, BIOS 1.07.06THZX 02/15/2022
[    4.161605] RIP: 0010:intel_modeset_verify_crtc+0x3d9/0x400 [i915]
[    4.161976] Code: 10 1d c5 c0 e8 e1 ac 5f fb 0f 0b 4c 8b 04 24 0f b6 44 24 17 e9 a8 fd ff ff e8 cc ac 5f fb 0f 0b e9 90 fe ff ff e8 c0 ac 5f fb <0f> 0b 0f b6 85 50 01 00 00 e9 08 fd ff ff e8 ad ac 5f fb 0f 0b e9
[    4.161982] RSP: 0018:ffffaa67409a3970 EFLAGS: 00010286
[    4.161990] RAX: 0000000000000000 RBX: ffff997c02548000 RCX: 0000000000000000
[    4.161996] RDX: 0000000000000001 RSI: 00000000ffffefff RDI: 00000000ffffffff
[    4.161999] RBP: ffff997c0259c000 R08: 0000000000000000 R09: ffffaa67409a37f8
[    4.162003] R10: 0000000000000003 R11: ffffffffbccd1848 R12: ffff997bc0e7e000
[    4.162007] R13: ffff997c015c9000 R14: ffff997bcd928000 R15: ffff997c02548000
[    4.162011] FS:  00007fac53a28d00(0000) GS:ffff997f2f400000(0000) knlGS:0000000000000000
[    4.162017] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[    4.162022] CR2: 00007fac540901e6 CR3: 00000004313fe002 CR4: 0000000000770ef0
[    4.162028] PKRU: 55555554
[    4.162031] Call Trace:
[    4.162039]  <TASK>
[    4.162046]  ? _raw_spin_lock+0x13/0x40
[    4.162066]  intel_atomic_commit_tail+0x958/0xea0 [i915]
[    4.162443]  intel_atomic_commit+0x34f/0x390 [i915]
[    4.162805]  drm_atomic_commit+0x93/0xc0 [drm]
[    4.162883]  ? drm_plane_get_damage_clips.cold+0x1c/0x1c [drm]
[    4.162962]  intel_modeset_init+0x19c/0x280 [i915]
[    4.163277]  i915_driver_probe+0x5da/0xe20 [i915]
[    4.163530]  ? i915_pci_probe+0x97/0x1d0 [i915]
[    4.163748]  local_pci_probe+0x3e/0x80
[    4.163761]  pci_device_probe+0xc3/0x230
[    4.163770]  ? sysfs_do_create_link_sd+0x6e/0xe0
[    4.163782]  really_probe+0xdb/0x380
[    4.163788]  ? pm_runtime_barrier+0x50/0x90
[    4.163798]  __driver_probe_device+0x78/0x170
[    4.163803]  driver_probe_device+0x1f/0x90
[    4.163808]  __driver_attach+0xd1/0x1d0
[    4.163813]  ? __device_attach_driver+0x110/0x110
[    4.163818]  bus_for_each_dev+0x84/0xd0
[    4.163828]  bus_add_driver+0x1ae/0x200
[    4.163838]  driver_register+0x89/0xe0
[    4.163844]  i915_init+0x1f/0x7f [i915]
[    4.164077]  ? 0xffffffffc0d65000
[    4.164082]  do_one_initcall+0x56/0x220
[    4.164093]  do_init_module+0x4a/0x1f0
[    4.164104]  __do_sys_finit_module+0xac/0x120
[    4.164117]  do_syscall_64+0x37/0xc0
[    4.164127]  entry_SYSCALL_64_after_hwframe+0x63/0xcd
[    4.164136] RIP: 0033:0x7fac5412c5a9
[    4.164141] Code: 08 89 e8 5b 5d c3 66 2e 0f 1f 84 00 00 00 00 00 90 48 89 f8 48 89 f7 48 89 d6 48 89 ca 4d 89 c2 4d 89 c8 4c 8b 4c 24 08 0f 05 <48> 3d 01 f0 ff ff 73 01 c3 48 8b 0d 27 08 0d 00 f7 d8 64 89 01 48
[    4.164146] RSP: 002b:00007ffea2cedfb8 EFLAGS: 00000246 ORIG_RAX: 0000000000000139
[    4.164152] RAX: ffffffffffffffda RBX: 0000558c43b44a50 RCX: 00007fac5412c5a9
[    4.164156] RDX: 0000000000000000 RSI: 00007fac542beefd RDI: 0000000000000012
[    4.164159] RBP: 00007fac542beefd R08: 0000000000000000 R09: 0000000000000000
[    4.164161] R10: 0000000000000012 R11: 0000000000000246 R12: 0000000000020000
[    4.164164] R13: 0000000000000000 R14: 0000558c43b488d0 R15: 0000558c41d98e50
[    4.164170]  </TASK>
[    4.164172] ---[ end trace 0000000000000000 ]---
```

</details>

## 解决方案

找到了 [No HDMI signal i915 0000:00:02.0: [drm] ERROR crtc 131: Can't calculate constants, dotclock = 0!](https://gitlab.freedesktop.org/drm/intel/-/issues/6454)。

按照其中的 patch 文件更改 `drivers/gpu/drm/i915/display/intel_ddi.c` 文件中的 `intel_ddi_init` 函数。


```c
 	encoder = &dig_port->base;
 	encoder->devdata = devdata;

+	if (port_name(port) == 'B') {
+		init_dp = 0;
+		init_hdmi = 1;
+	}
+
 	if (DISPLAY_VER(dev_priv) >= 13 && port >= PORT_D_XELPD) {
 		drm_encoder_init(&dev_priv->drm, &encoder->base, &intel_ddi_funcs,
 				 DRM_MODE_ENCODER_TMDS,
```

似乎是 BIOS 对 VBT 初始化的问题

然后重新编译内核。

```bash
cp /boot/config-$(uname -r) .config
make oldconfig
make localmodconfig
make -j $(nproc)
```

安装并更新 grub。

```bash
sudo make modules_install
sudo make install
sudo update-initramfs -u -k all
sudo update-grub
```

这样可以暂时性地解决问题。只是，内核有些东西不太能用，比如显卡驱动、docker（也可能是我编译选项设置得有问题）。

<br>

## 2023-12-5 更新：仅替换 i915 内核模块

下载 `apt` 打包时的源码：

```bash
apt source linux-image-6.1.0-13-amd64-unsigned
```

在 `drivers/gpu/drm/i915/display/intel_ddi.c` 中找到 `intel_ddi_init` 函数，修改此文件。

commit 完。打包编译，大概需要编译一个小时

```bash
dpkg-source --commit
dpkg-buildpackage -us -uc
```

然后提取其中的 `i915.ko`，替换掉 `/lib/modules/$(uname -r)/kernel/drivers/gpu/drm/i915/i915.ko`。

更新 initramfs：

```bash
sudo update-initramfs -u -k all
```

这样就可以只替换 i915 内核模块，解决 HDMI 输出的问题了。
