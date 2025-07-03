#pragma once

#include <rawfile/raw_file_manager.h>
#include <cstdint>

extern "C" {
typedef struct GodotTouchEvent {
	int type;
	int id;
	float x;
	float y;
} GodotTouchEvent;

int64_t godot_init(NativeResourceManager *p_resource_manager, void *p_native_window, int64_t window_width, int64_t window_height);
void godot_touch(GodotTouchEvent *p_event, int count);
}