#pragma once

#include <cstdint>

extern "C" {
int64_t godot_add(int64_t a, int64_t b);
int64_t godot_init(void *nativeWindow);
}