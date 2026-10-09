# SM-X700 variant (gts8wifi)

The standard 11-inch Galaxy Tab S8 Wi-Fi (SM-X700, `gts8wifi`) is a separate
build target from the 12.4-inch Tab S8+ Wi-Fi (SM-X800, `gts8pwifi`). Select it
on every Make invocation:

```sh
make VARIANT=gts8wifi pmb-config
make VARIANT=gts8wifi deps
make VARIANT=gts8wifi kernel device
make VARIANT=gts8wifi rootfs
make VARIANT=gts8wifi image
```

Initialize pmbootstrap for `samsung-gts8wifi` before `pmb-config`, and use
SM-X700 stock partition dumps under `device-facts/samsung-gts8wifi/partitions-backup`.
The regular `make` default remains the SM-X800. Kernel packages, rootfs images,
boot artifacts, and build staging are kept separate between variants.

## Hardware and kernel differences

The X700 target is based on the public
[kubierend/sm-x700-linux](https://github.com/kubierend/sm-x700-linux) port at
commit `eefeccb16ac9afec948e4906f96645cd28fc93b8`. Its kernel is pinned to the
SM8450 mainline commit `bf1d29fced6e156dd6090a9b6600a8c44259c114` (6.13-rc3);
the X800 continues to use the repository's 7.2 kernel. The X700 tree uses its
own portrait 1600×2560 framebuffer and Novatek NT36523 panel/touch support.
The X800's landscape S6TUUM1 display setup must not be used for this variant.

The included X700 device-tree selector values are reported by that source for
an EUX board-revision-8 unit (`qcom,board-id = <0x10008 0x08>`). Confirm the
revision and selector values from the target tablet's stock DTB before flashing
another regional or board revision. The included panel initialization is for
the CSOT PPA957DB1 variant; the source notes a different Tianma panel variant
that needs a separate initialization sequence. This repository has not
independently validated the X700 profile on hardware.

The first X700 boot does not include device-signed GPU/audio/sensor firmware.
After booting stock-derived firmware from the tablet:

```sh
sudo gts8wifi-firmware-setup
```

To rebuild uniLoader with the firmware-populated initramfs, copy that tablet's
updated `/boot/initramfs` to the build host and pass its path:

```sh
make VARIANT=gts8wifi boot INITRAMFS=/path/to/x700-initramfs
```

Only packages, configs, and drivers from the referenced open-source port are
included. Proprietary firmware and stock DTBs remain on the user's own device.
