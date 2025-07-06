#include "wrapper_openharmony.h"

#include <window_manager/oh_display_manager.h>
#include <window_manager/oh_window.h>

int ohos_wrapper_get_display_dpi() {
	int32_t dpi = 0;
	OH_NativeDisplayManager_GetDefaultDisplayDensityDpi(&dpi);
	return dpi;
}

float ohos_wrapper_get_display_refresh_rate() {
	uint32_t refresh_rate = 0;
	OH_NativeDisplayManager_GetDefaultDisplayRefreshRate(&refresh_rate);
	return refresh_rate;
}

WrapperScreenOrientation ohos_wrapper_get_display_orientation() {
	NativeDisplayManager_Orientation orientation;
	OH_NativeDisplayManager_GetDefaultDisplayOrientation(&orientation);
	switch (orientation) {
		case DISPLAY_MANAGER_PORTRAIT:
			return WrapperScreenOrientation::WRAPPER_SCREEN_PORTRAIT;
		case DISPLAY_MANAGER_LANDSCAPE:
			return WrapperScreenOrientation::WRAPPER_SCREEN_LANDSCAPE;
		case DISPLAY_MANAGER_PORTRAIT_INVERTED:
			return WrapperScreenOrientation::WRAPPER_SCREEN_REVERSE_PORTRAIT;
		case DISPLAY_MANAGER_LANDSCAPE_INVERTED:
			return WrapperScreenOrientation::WRAPPER_SCREEN_REVERSE_LANDSCAPE;
		default:
			return WrapperScreenOrientation::WRAPPER_SCREEN_PORTRAIT;
	}
}

void ohos_wrapper_screen_set_keep_on(int32_t window_id, bool p_enable) {
	OH_WindowManager_SetWindowKeepScreenOn(window_id, p_enable);
}

bool ohos_wrapper_screen_is_kept_on(int32_t window_id) {
	WindowManager_WindowProperties properties;
	OH_WindowManager_GetWindowProperties(window_id, &properties);
	return properties.isKeepScreenOn;
}

int ohos_wrapper_get_keyboard_avoid_area(int32_t window_id) {
	WindowManager_AvoidArea area;
	OH_WindowManager_GetWindowAvoidArea(window_id, WINDOW_MANAGER_AVOID_AREA_TYPE_KEYBOARD, &area);
	return area.bottomRect.height;
}