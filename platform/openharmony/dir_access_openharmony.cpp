#include "dir_access_openharmony.h"

NativeResourceManager *DirAccessOpenHarmony::resource_manager = nullptr;

void DirAccessOpenHarmony::setup(NativeResourceManager *p_resource_manager) {
    DirAccessOpenHarmony::resource_manager = p_resource_manager;
}