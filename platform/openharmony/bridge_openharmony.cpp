#include "bridge_openharmony.h"
#include "dir_access_openharmony.h"
#include "file_access_openharmony.h"
#include "os_openharmony.h"

#include "core/config/project_settings.h"
#include "core/variant/variant.h"
#include "display_server_openharmony.h"
#include "main/main.h"

#include <native_vsync/native_vsync.h>

OS_OpenHarmony *os_openharmony = nullptr;
OH_NativeVSync *native_vsync = nullptr;
int step = 0;

enum GodotStartupStep {
	STEP_TERMINATED = -1,
	STEP_SETUP,
	STEP_SHOW_LOGO,
	STEP_STARTED
};

void godot_finalize() {
	if (step == STEP_TERMINATED) {
		return;
	}
	step = STEP_TERMINATED;

	if (os_openharmony) {
		os_openharmony->main_loop_end();
		Main::cleanup();
		memdelete(os_openharmony);
		os_openharmony = nullptr;
	}

	OH_NativeVSync_Destroy(native_vsync);
	native_vsync = nullptr;
}

void godot_step(long long timestamp, void *data) {
	if (step == STEP_TERMINATED) {
		return;
	}

	switch (step) {
		case STEP_SETUP:
			// Since Godot is initialized on the UI thread, main_thread_id was set to that thread's id,
			// but for Godot purposes, the main thread is the one running the game loop
			Main::setup2(false); // The logo is shown in the next frame otherwise we run into rendering issues
			step++;
			break;
		case STEP_SHOW_LOGO:
			Main::setup_boot_logo();
			step++;

			break;
		case STEP_STARTED:
			if (Main::start() != EXIT_SUCCESS) {
				return; // should exit instead and print the error
			}
			os_openharmony->main_loop_begin();
			step++;
			break;
		default:
			if (os_openharmony->main_loop_iterate()) {
				// If the main loop iteration returns true, it means we should exit.
				// In this case, we do not request another frame.
				godot_finalize();
				return;
			}
			break;
	}

	// Request the next frame
	OH_NativeVSync_RequestFrame(native_vsync, godot_step, nullptr);
}

int64_t godot_init(NativeResourceManager *p_resource_manager, void *p_native_window, int64_t window_width, int64_t window_height) {
	OHNativeWindow *window = static_cast<OHNativeWindow *>(p_native_window);

	FileAccessOpenHarmony::setup(p_resource_manager);
	DirAccessOpenHarmony::setup(p_resource_manager);
	os_openharmony = memnew(OS_OpenHarmony);
	os_openharmony->set_native_window(window);
	os_openharmony->set_display_size(Size2i(window_width, window_height));

	Error err = Main::setup(OS_OpenHarmony::EXEC_PATH, 0, nullptr, false);
	if (err != OK) {
		return err;
	}

	const char *connection_name = "godot";
	native_vsync = OH_NativeVSync_Create(connection_name, strlen(connection_name));
	return OH_NativeVSync_RequestFrame(native_vsync, godot_step, nullptr);
}

void godot_touch(GodotTouchEvent *p_event, int count) {
	static Vector<GodotTouchEvent> last_touch_events;
	for (int i = 0; i < count; i++) {
		GodotTouchEvent &event = p_event[i];
		if (event.id >= last_touch_events.size()) {
			last_touch_events.resize(event.id + 1);
		}
		switch (event.type) {
			case 0: { // Touch begin
				Ref<InputEventScreenTouch> ev;
				ev.instantiate();
				ev->set_index(event.id);
				ev->set_pressed(true);
				ev->set_position(Vector2(event.x, event.y));
				Input::get_singleton()->parse_input_event(ev);
			} break;
			case 1: { // Touch up
				Ref<InputEventScreenTouch> ev;
				ev.instantiate();
				ev->set_index(event.id);
				ev->set_pressed(false);
				ev->set_position(Vector2(event.x, event.y));
				Input::get_singleton()->parse_input_event(ev);
			} break;
			case 2: { // Touch move
				Ref<InputEventScreenDrag> ev;
				ev.instantiate();
				ev->set_index(event.id);
				ev->set_position(Vector2(event.x, event.y));
				ev->set_relative(Vector2(event.x - last_touch_events[event.id].x, event.y - last_touch_events[event.id].y));
				ev->set_relative_screen_position(ev->get_relative());
				Input::get_singleton()->parse_input_event(ev);
			} break;
			case 3: { // Touch cancel
				Ref<InputEventScreenTouch> ev;
				ev.instantiate();
				ev->set_index(event.id);
				ev->set_canceled(true);
				ev->set_position(Vector2(event.x, event.y));
				Input::get_singleton()->parse_input_event(ev);
			} break;
		}
		last_touch_events.set(event.id, event);
	}
}