#include "bridge_openharmony.h"
#include "core/config/project_settings.h"
#include "core/variant/variant.h"
#include "display_server_openharmony.h"
#include "main/main.h"
#include "os_openharmony.h"

int64_t godot_add(int64_t a, int64_t b) {
    a = Variant(a);
    b = Variant(b);
    return a + b;
}

int64_t godot_init(void *nativeWindow) {
	OHNativeWindow *window = static_cast<OHNativeWindow *>(nativeWindow);

	OS_OpenHarmony *os = memnew(OS_OpenHarmony);
	os->set_native_window(window);
	print_line("Godot Setup:", os->get_executable_path());
	Error err = Main::setup("template", 0, nullptr, true);
	print_line(vformat("Godot Setup Result: %d", err));
	return 0;
}