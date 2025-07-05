#pragma once

#include "audio_driver_openharmony.h"

#include "core/os/main_loop.h"
#include "drivers/unix/os_unix.h"
#include "drivers/vulkan/godot_vulkan.h"
#include "servers/audio_server.h"

class OS_OpenHarmony : public OS_Unix {
	Size2i display_size;
	OHNativeWindow *native_window = nullptr;
	MainLoop *main_loop = nullptr;
	AudioDriverOpenHarmony audio_driver_openharmony;

public:
	static const char *EXEC_PATH;
	static const char *BUNDLE_RESOURCE_DIR;
	static const char *USER_DATA_DIR;

	static OS_OpenHarmony *get_singleton();

	OS_OpenHarmony();

	void set_native_window(OHNativeWindow *p_native_window);
	OHNativeWindow *get_native_window() const;
	void set_display_size(const Size2i &p_size);
	Size2i get_display_size() const;

	virtual void initialize() override;
	virtual void initialize_joypads() override;
	virtual void set_main_loop(MainLoop *p_main_loop) override;
	virtual MainLoop *get_main_loop() const override;
	virtual void delete_main_loop() override;
	virtual void finalize() override;
	virtual bool _check_internal_feature_support(const String &p_feature) override;

	virtual String get_user_data_dir(const String &p_user_dir) const override;
	virtual String get_bundle_resource_dir() const override;
	virtual String get_executable_path() const override;

	void main_loop_begin();
	bool main_loop_iterate();
	void main_loop_end();
};

class Logger_OpenHarmony : public Logger {
public:
	virtual void logv(const char *p_format, va_list p_list, bool p_err) override;
};