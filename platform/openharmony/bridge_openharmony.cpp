#include "bridge_openharmony.h"
#include "dir_access_openharmony.h"
#include "file_access_openharmony.h"
#include "os_openharmony.h"

#include "core/config/project_settings.h"
#include "core/variant/variant.h"
#include "display_server_openharmony.h"
#include "main/main.h"

int64_t godot_init(NativeResourceManager *p_resource_manager, void *nativeWindow, int64_t window_width, int64_t window_height) {
	OHNativeWindow *window = static_cast<OHNativeWindow *>(nativeWindow);

	FileAccessOpenHarmony::setup(p_resource_manager);
	DirAccessOpenHarmony::setup(p_resource_manager);
	OS_OpenHarmony *os = memnew(OS_OpenHarmony);
	os->set_native_window(window);
	os->set_display_size(Size2i(window_width, window_height));

	print_line("Godot Setup:", os->get_executable_path());
	Error err = Main::setup(OS_OpenHarmony::EXEC_PATH, 0, nullptr, true);
	print_line(vformat("Godot Setup Result: %d", err));
	return 0;
}