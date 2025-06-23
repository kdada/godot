#pragma once

#include "core/io/dir_access.h"
#include "drivers/unix/dir_access_unix.h"

#include <rawfile/raw_file_manager.h>
#include <rawfile/raw_dir.h>


class DirAccessOpenHarmony : public DirAccessUnix {
   static NativeResourceManager *resource_manager; 

public:
	static void setup(NativeResourceManager *p_resource_manager);
};