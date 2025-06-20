#include "bridge_openharmony.h"
#include "core/variant/variant.h"

int64_t godot_add(int64_t a, int64_t b) {
    a = Variant(a);
    b = Variant(b);
    return a + b;
}