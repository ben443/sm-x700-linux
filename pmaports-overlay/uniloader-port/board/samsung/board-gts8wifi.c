/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Samsung Galaxy Tab S8 Wi-Fi (SM-X700), codename "gts8wifi".
 * SM8450 (Waipio). Derived from board-gts8pwifi.c (Galaxy Tab S8+, same SoC).
 *
 * The Samsung bootloader leaves the panel scanning the cont_splash
 * framebuffer, so simplefb here gives uniLoader (and then the kernel)
 * a visible console on the panel.
 *
 * Geometry comes from the stock DTS (gts8wifi_eur_open_w00_r08.dts):
 *   splash_region reg = <0x0 0xb8000000 0x0 0x2b00000> (45 MiB) -- identical
 *   address/size to the X800's cont_splash_region, confirming this is a
 *   platform-wide (not panel-specific) memory map.
 *
 * Scanout is PORTRAIT 1600x2560, NOT rotated to landscape. Confirmed by two
 * independent lines of evidence:
 *   - Physical panel size (qcom,mdss-pan-physical-{width,height}-dimension
 *     = 0x94/0xec = 148mm/236mm): 148/236 = 0.627 matches 1600/2560 = 0.625,
 *     i.e. width-dimension (148mm, the SHORT axis) corresponds to the
 *     1600px pixel width, not the long axis. Unlike the X800's panel
 *     (S6TUUM1), whose width-dimension property IS the long/landscape axis,
 *     this panel's DTS properties are authored in portrait.
 *   - Empirically confirmed on-device: an earlier build using 2560x1600
 *     (landscape, by wrongly assuming the long axis = width like X800)
 *     rendered uniLoader's boot log as badly sheared diagonal columns on
 *     the real panel -- the classic symptom of a stride/geometry mismatch,
 *     see the X800 board's bug #2. 1600x2560 fixed it.
 * The per-DSI-link timing block values (qcom,mdss-dsi-panel-width/height =
 * 0x320/0xa00 = 800/2560) are HALF the real width (dual-DSI column split);
 * combined full-frame width is 800*2 = 1600, matching the above.
 */

#include <board.h>
#include <util.h>
#include <drivers/framework.h>
#include <lib/simplefb.h>

static struct video_info gts8wifi_fb = {
	.format = FB_FORMAT_ARGB8888,
	.width = 1600,
	.height = 2560,
	.stride = 4,
	.address = (void *)0xb8000000
};

static const struct device gts8wifi_devices[] = {
	{ "simplefb", &gts8wifi_fb, "fb" },
};

struct board_data board_ops = {
	.name = "samsung-gts8wifi",
	.ops = {
	},
	.devices = gts8wifi_devices,
	.num_devices = ARRAY_SIZE(gts8wifi_devices),
	.quirks = 0
};
