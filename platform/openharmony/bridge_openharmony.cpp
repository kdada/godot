#include "bridge_openharmony.h"
#include "core/config/project_settings.h"
#include "core/variant/variant.h"
#include "display_server_openharmony.h"
#include "os_openharmony.h"

int64_t godot_add(int64_t a, int64_t b) {
    a = Variant(a);
    b = Variant(b);
    return a + b;
}

int64_t godot_init(void *nativeWindow) {
	auto window = static_cast<OHNativeWindow *>(nativeWindow);

	auto os = memnew(OS_OpenHarmony);
	print_line("Godot init 1");
	os->set_native_window(window);
	print_line("Godot init 2");
	auto proj = memnew(ProjectSettings);
	print_line("Godot init 3");
	Error err = OK;
	auto display_server = memnew(DisplayServerOpenHarmony("vulkan", DisplayServer::WINDOW_MODE_FULLSCREEN, DisplayServer::VSYNC_ADAPTIVE, 0, nullptr, Size2i(0, 0), 0, DisplayServer::CONTEXT_ENGINE, 0, err));
	print_line("Godot init END");
	return 0;
}