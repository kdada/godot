#include "dir_access_openharmony.h"
#include "display_server_openharmony.h"
#include "file_access_openharmony.h"
#include "os_openharmony.h"

#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "drivers/unix/dir_access_unix.h"
#include "drivers/unix/file_access_unix.h"
#include "main/main.h"
#include "scene/main/scene_tree.h"

#include <hilog/log.h>

#undef LOG_DOMAIN
#undef LOG_TAG
#define LOG_DOMAIN 0x3200
#define LOG_TAG "LIB_GODOT"

const char *OS_OpenHarmony::EXEC_PATH = "template";
const char *OS_OpenHarmony::BUNDLE_RESOURCE_DIR = "/data/storage/el1/bundle/resources/rawfile/";
const char *OS_OpenHarmony::USER_DATA_DIR = "/data/storage/el2/base/files/";

OS_OpenHarmony *OS_OpenHarmony::get_singleton() {
	return static_cast<OS_OpenHarmony *>(OS::get_singleton());
}

OS_OpenHarmony::OS_OpenHarmony() {
	Vector<Logger *> loggers;
	Logger_OpenHarmony *logger = memnew(Logger_OpenHarmony);
	loggers.push_back(logger);
	_set_logger(memnew(CompositeLogger(loggers)));

	AudioDriverManager::add_driver(&audio_driver_openharmony);
	DisplayServerOpenHarmony::register_openharmony_driver();
}

void OS_OpenHarmony::set_native_window(OHNativeWindow *p_native_window) {
	native_window = p_native_window;
}

OHNativeWindow *OS_OpenHarmony::get_native_window() const {
	return native_window;
}

void OS_OpenHarmony::set_window_id(int32_t p_window_id) {
	window_id = p_window_id;
}

int32_t OS_OpenHarmony::get_window_id() const {
	return window_id;
}

void OS_OpenHarmony::set_display_size(const Size2i &p_size) {
	display_size = p_size;
}

Size2i OS_OpenHarmony::get_display_size() const {
	return display_size;
}

void OS_OpenHarmony::initialize() {
	OS_Unix::initialize_core();

	FileAccess::make_default<FileAccessOpenHarmony>(FileAccess::ACCESS_FILESYSTEM);
	DirAccess::make_default<DirAccessOpenHarmony>(DirAccess::ACCESS_FILESYSTEM);
}

void OS_OpenHarmony::initialize_joypads() {
}

void OS_OpenHarmony::set_main_loop(MainLoop *p_main_loop) {
	main_loop = p_main_loop;
}

MainLoop *OS_OpenHarmony::get_main_loop() const {
	return main_loop;
}

void OS_OpenHarmony::delete_main_loop() {
}

void OS_OpenHarmony::finalize() {
}

bool OS_OpenHarmony::_check_internal_feature_support(const String &p_feature) {
	return false;
}

String OS_OpenHarmony::get_user_data_dir(const String &p_user_dir) const {
	return OS_OpenHarmony::USER_DATA_DIR;
}

String OS_OpenHarmony::get_bundle_resource_dir() const {
	return OS_OpenHarmony::BUNDLE_RESOURCE_DIR;
}

String OS_OpenHarmony::get_executable_path() const {
	return OS_OpenHarmony::EXEC_PATH;
}

void OS_OpenHarmony::main_loop_begin() {
	if (main_loop) {
		main_loop->initialize();
	}
}

bool OS_OpenHarmony::main_loop_iterate() {
	if (!main_loop) {
		return false;
	}
	DisplayServerOpenHarmony::get_singleton()->process_events();
	return Main::iteration();
}

void OS_OpenHarmony::main_loop_end() {
	if (main_loop) {
		SceneTree *scene_tree = Object::cast_to<SceneTree>(main_loop);
		if (scene_tree) {
			scene_tree->quit();
		}
		main_loop->finalize();
	}
}

void Logger_OpenHarmony::logv(const char *p_format, va_list p_list, bool p_err) {
	if (!should_log(p_err)) {
		return;
	}

	char buffer[4096];
	vsnprintf(&buffer[0], sizeof(buffer) - 1, p_format, p_list);

	if (p_err) {
		OH_LOG_ERROR(LOG_APP, "%{public}s", &buffer[0]);
	} else {
		OH_LOG_INFO(LOG_APP, "%{public}s", &buffer[0]);
	}
}
