#pragma once

#include <cstddef>
#include <cstdint>

enum WrapperScreenOrientation {
	WRAPPER_SCREEN_LANDSCAPE,
	WRAPPER_SCREEN_PORTRAIT,
	WRAPPER_SCREEN_REVERSE_LANDSCAPE,
	WRAPPER_SCREEN_REVERSE_PORTRAIT,
};

int ohos_wrapper_get_display_dpi();
float ohos_wrapper_get_display_refresh_rate();
WrapperScreenOrientation ohos_wrapper_get_display_orientation();
void ohos_wrapper_screen_set_keep_on(int32_t window_id, bool p_enable);
bool ohos_wrapper_screen_is_kept_on(int32_t window_id);
int ohos_wrapper_get_keyboard_avoid_area(int32_t window_id);
