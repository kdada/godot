#pragma once

#include <rawfile/raw_file_manager.h>
#include <cstdint>

extern "C" {
typedef struct GodotTouchEvent {
	uint32_t type;
	uint32_t id;
	float x;
	float y;
} GodotTouchEvent;

typedef struct GodotKeyEvent {
	uint32_t code;
	char32_t unicode;
	bool pressed;
	bool alt;
	bool ctrl;
	bool shift;
	bool meta;
} GodotKeyEvent;

typedef struct GodotMouseEvent {
	uint32_t type;
	uint32_t button;
	uint32_t mask;
	float x;
	float y;
} GodotMouseEvent;

int64_t godot_init(NativeResourceManager *p_resource_manager, void *p_native_window, int32_t window_id, int64_t window_width, int64_t window_height);
void godot_touch(GodotTouchEvent *p_event, int count);
void godot_mouse(GodotMouseEvent *p_event);
void godot_key(GodotKeyEvent *p_event);
void godot_resize(uint32_t width, uint32_t height);
void godot_window_event(int32_t event);
}