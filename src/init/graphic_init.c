/**
 * @file
 *
 * @date Feb 4, 2013
 * @author: Anton Bondarev
 */
#include <errno.h>
#include <assert.h>

#include <drivers/video/fb.h>
#include <drivers/video/fb_videomodes.h>

#include <embox/unit.h>

#if !OPTION_GET(BOOLEAN, manual_settings)
#include <module/embox/arch/x86/boot/multiboot.h>

#define MBOOTMOD embox__arch__x86__boot__multiboot

#define SET_X   OPTION_MODULE_GET(MBOOTMOD,NUMBER,video_width)
#define SET_Y   OPTION_MODULE_GET(MBOOTMOD,NUMBER,video_height)
#define SET_BPP OPTION_MODULE_GET(MBOOTMOD,NUMBER,video_depth)

#else

#define SET_X   OPTION_GET(NUMBER, set_x)
#define SET_Y   OPTION_GET(NUMBER, set_y)
#define SET_BPP OPTION_GET(NUMBER, set_bpp)

#endif

EMBOX_UNIT_INIT(mode_init);

static int mode_init(void) {
	struct fb_info *fbinfo;
	const struct fb_videomode *mode;
	struct fb_var_screeninfo var;
	int ret;

	fbinfo = fb_lookup(0);
	if (fbinfo == NULL) {
		return -ENODEV;
	}

	mode = fb_find_videomode(SET_X, SET_Y, SET_BPP);
	if (mode == NULL) {
		return -EINVAL;
	}

	fb_videomode_to_var(&var, mode);
	var.bits_per_pixel = SET_BPP;
	var.fmt = SET_BPP == 16 ? BGR565 : RGBA8888;
	ret = fb_set_var(fbinfo, &var);
	if (ret != 0) {
		return -EIO;
	}

	return 0;

}
