#pragma once

#include <rawfile/raw_file_manager.h>
#include <cstdint>

extern "C" {
int64_t godot_init(NativeResourceManager *p_resource_manager, void *nativeWindow, int64_t window_width, int64_t window_height);
}