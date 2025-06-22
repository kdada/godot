#include "display_server_openharmony.h"
#include "os_openharmony.h"
#include <hilog/log.h>

#undef LOG_DOMAIN
#undef LOG_TAG
#define LOG_DOMAIN 0x3200
#define LOG_TAG "LIB_GODOT"

OS_OpenHarmony *OS_OpenHarmony::get_singleton() {
	return static_cast<OS_OpenHarmony *>(OS::get_singleton());
}

OS_OpenHarmony::OS_OpenHarmony() {
	Vector<Logger *> loggers;
	Logger_OpenHarmony *logger = memnew(Logger_OpenHarmony);
	loggers.push_back(logger);
	_set_logger(memnew(CompositeLogger(loggers)));

	DisplayServerOpenHarmony::register_openharmony_driver();
}

void OS_OpenHarmony::set_native_window(OHNativeWindow *p_native_window) {
	native_window = p_native_window;
}

OHNativeWindow *OS_OpenHarmony::get_native_window() const {
	return native_window;
}

void OS_OpenHarmony::set_display_size(const Size2i &p_size) {
	display_size = p_size;
}

Size2i OS_OpenHarmony::get_display_size() const {
	return display_size;
}

void OS_OpenHarmony::initialize() {
	OS_Unix::initialize_core();
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
	return "/data/storage/el2/base/files/" + p_user_dir;
}

String OS_OpenHarmony::get_resource_dir() const {
	return "/data/storage/el1/bundle/";
}

String OS_OpenHarmony::get_bundle_resource_dir() const {
	return "/data/storage/el1/bundle/";
}

String OS_OpenHarmony::get_executable_path() const {
	return "template";
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
