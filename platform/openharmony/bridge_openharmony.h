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

int64_t godot_init(NativeResourceManager *p_resource_manager, void *p_native_window, int32_t window_id, int64_t window_width, int64_t window_height);
void godot_touch(GodotTouchEvent *p_event, int count);
void godot_resize(uint32_t width, uint32_t height);
void godot_window_event(int32_t event);
}