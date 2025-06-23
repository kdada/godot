#pragma once

#include "core/io/file_access.h"
#include "drivers/unix/file_access_unix.h"

#include <rawfile/raw_file_manager.h>
#include <rawfile/raw_dir.h>

class FileAccessOpenHarmony : public FileAccessUnix {
   static NativeResourceManager *resource_manager; 

public:
	static void setup(NativeResourceManager *p_resource_manager);
};