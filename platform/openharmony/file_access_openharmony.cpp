#include "file_access_openharmony.h"

NativeResourceManager *FileAccessOpenHarmony::resource_manager = nullptr;

void FileAccessOpenHarmony::setup(NativeResourceManager *p_resource_manager) {
    FileAccessOpenHarmony::resource_manager = p_resource_manager;
}