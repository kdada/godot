#include "display_server_openharmony.h"
#include "os_openharmony.h"
#include "rendering_context_driver_vulkan_openharmony.h"

#include "servers/rendering/renderer_rd/renderer_compositor_rd.h"
#include "servers/rendering/rendering_device.h"

void DisplayServerOpenHarmony::_dispatch_input_events(const Ref<InputEvent> &p_event) {
	DisplayServerOpenHarmony::get_singleton()->send_input_event(p_event);
}

DisplayServerOpenHarmony *DisplayServerOpenHarmony::get_singleton() {
	return static_cast<DisplayServerOpenHarmony *>(DisplayServer::get_singleton());
}

Vector<String> DisplayServerOpenHarmony::get_rendering_drivers_func() {
	Vector<String> drivers;
	drivers.push_back("vulkan");
	return drivers;
}

DisplayServer *DisplayServerOpenHarmony::create_func(const String &p_rendering_driver, DisplayServer::WindowMode p_mode, DisplayServer::VSyncMode p_vsync_mode, uint32_t p_flags, const Vector2i *p_position, const Vector2i &p_resolution, int p_screen, Context p_context, int64_t p_parent_window, Error &r_error) {
	DisplayServer *ds = memnew(DisplayServerOpenHarmony(p_rendering_driver, p_mode, p_vsync_mode, p_flags, p_position, p_resolution, p_screen, p_context, p_parent_window, r_error));
	if (r_error != OK) {
		OS::get_singleton()->alert(
				"Your device seems not to support the required Vulkan version.\n\n"
				"Unable to initialize Vulkan video driver");
	}
	return ds;
}

void DisplayServerOpenHarmony::register_openharmony_driver() {
	register_create_function("openharmony", create_func, get_rendering_drivers_func);
}

DisplayServerOpenHarmony::DisplayServerOpenHarmony(const String &p_rendering_driver, WindowMode p_mode, DisplayServer::VSyncMode p_vsync_mode, uint32_t p_flags, const Vector2i *p_position, const Vector2i &p_resolution, int p_screen, Context p_context, int64_t p_parent_window, Error &r_error) {
	rendering_driver = p_rendering_driver;

	rendering_context = nullptr;
	rendering_device = nullptr;

	if (rendering_driver != "vulkan") {
		ERR_PRINT(vformat("Failed to create %s context", rendering_driver));
		r_error = ERR_UNAVAILABLE;
	}

	rendering_context = memnew(RenderingContextDriverVulkanOpenHarmony);

	if (rendering_context->initialize() != OK) {
		memdelete(rendering_context);
		rendering_context = nullptr;
		ERR_PRINT(vformat("Failed to initialize %s context", rendering_driver));
		r_error = ERR_UNAVAILABLE;
		return;
	}
	RenderingContextDriverVulkanOpenHarmony::WindowPlatformData vulkan;
	OHNativeWindow *native_window = OS_OpenHarmony::get_singleton()->get_native_window();
	ERR_FAIL_NULL(native_window);
	vulkan.window = native_window;

	if (rendering_context->window_create(MAIN_WINDOW_ID, &vulkan) != OK) {
		ERR_PRINT(vformat("Failed to create %s window.", rendering_driver));
		memdelete(rendering_context);
		rendering_context = nullptr;
		r_error = ERR_UNAVAILABLE;
		return;
	}

	Size2i display_size = OS_OpenHarmony::get_singleton()->get_display_size();
	rendering_context->window_set_size(MAIN_WINDOW_ID, display_size.width, display_size.height);
	rendering_context->window_set_vsync_mode(MAIN_WINDOW_ID, p_vsync_mode);

	rendering_device = memnew(RenderingDevice);
	if (rendering_device->initialize(rendering_context, MAIN_WINDOW_ID) != OK) {
		rendering_device = nullptr;
		memdelete(rendering_context);
		rendering_context = nullptr;
		r_error = ERR_UNAVAILABLE;
		return;
	}
	rendering_device->screen_create(MAIN_WINDOW_ID);

	RendererCompositorRD::make_current();

	Input::get_singleton()->set_event_dispatch_function(_dispatch_input_events);

	r_error = OK;
}

DisplayServerOpenHarmony::~DisplayServerOpenHarmony() {
}

void DisplayServerOpenHarmony::_window_callback(const Callable &p_callable, const Variant &p_arg, bool p_deferred) const {
	if (p_callable.is_valid()) {
		if (p_deferred) {
			p_callable.call_deferred(p_arg);
		} else {
			p_callable.call(p_arg);
		}
	}
}

void DisplayServerOpenHarmony::send_input_event(const Ref<InputEvent> &p_event) const {
	_window_callback(input_event_callback, p_event);
}

bool DisplayServerOpenHarmony::has_feature(Feature p_feature) const {
	return false;
}

String DisplayServerOpenHarmony::get_name() const {
	return "OpenHarmony";
}

int DisplayServerOpenHarmony::get_screen_count() const {
	return 1;
}

int DisplayServerOpenHarmony::get_primary_screen() const {
	return 0;
}

Point2i DisplayServerOpenHarmony::screen_get_position(int p_screen) const {
	return Point2i(0, 0);
}

Size2i DisplayServerOpenHarmony::screen_get_size(int p_screen) const {
	return OS_OpenHarmony::get_singleton()->get_display_size();
}

Rect2i DisplayServerOpenHarmony::screen_get_usable_rect(int p_screen) const {
	Size2i display_size = OS_OpenHarmony::get_singleton()->get_display_size();
	return Rect2i(0, 0, display_size.width, display_size.height);
}

int DisplayServerOpenHarmony::screen_get_dpi(int p_screen) const {
	return 454;
}

float DisplayServerOpenHarmony::screen_get_refresh_rate(int p_screen) const {
	return 60;
}

Vector<DisplayServer::WindowID> DisplayServerOpenHarmony::get_window_list() const {
	Vector<WindowID> ret;
	ret.push_back(MAIN_WINDOW_ID);
	return ret;
}

DisplayServer::WindowID DisplayServerOpenHarmony::get_window_at_screen_position(const Point2i &p_position) const {
	return MAIN_WINDOW_ID;
}

void DisplayServerOpenHarmony::window_attach_instance_id(ObjectID p_instance, DisplayServer::WindowID p_window) {
	window_attached_instance_id = p_instance;
}

ObjectID DisplayServerOpenHarmony::window_get_attached_instance_id(DisplayServer::WindowID p_window) const {
	return window_attached_instance_id;
}

void DisplayServerOpenHarmony::window_set_window_event_callback(const Callable &p_callable, DisplayServer::WindowID p_window) {
}

void DisplayServerOpenHarmony::window_set_input_event_callback(const Callable &p_callable, DisplayServer::WindowID p_window) {
	input_event_callback = p_callable;
}

void DisplayServerOpenHarmony::window_set_input_text_callback(const Callable &p_callable, DisplayServer::WindowID p_window) {
}

void DisplayServerOpenHarmony::window_set_rect_changed_callback(const Callable &p_callable, DisplayServer::WindowID p_window) {
}

void DisplayServerOpenHarmony::window_set_drop_files_callback(const Callable &p_callable, DisplayServer::WindowID p_window) {
}

void DisplayServerOpenHarmony::window_set_title(const String &p_title, DisplayServer::WindowID p_window) {
	// Not supported on OpenHarmony.
}

int DisplayServerOpenHarmony::window_get_current_screen(DisplayServer::WindowID p_window) const {
	return SCREEN_OF_MAIN_WINDOW;
}

void DisplayServerOpenHarmony::window_set_current_screen(int p_screen, DisplayServer::WindowID p_window) {
	// Not supported on OpenHarmony.
}

Point2i DisplayServerOpenHarmony::window_get_position(DisplayServer::WindowID p_window) const {
	return Point2i();
}

Point2i DisplayServerOpenHarmony::window_get_position_with_decorations(DisplayServer::WindowID p_window) const {
	return Point2i();
}

void DisplayServerOpenHarmony::window_set_position(const Point2i &p_position, DisplayServer::WindowID p_window) {
	// Not supported on OpenHarmony.
}

void DisplayServerOpenHarmony::window_set_transient(DisplayServer::WindowID p_window, DisplayServer::WindowID p_parent) {
	// Not supported on OpenHarmony.
}

void DisplayServerOpenHarmony::window_set_max_size(const Size2i p_size, DisplayServer::WindowID p_window) {
	// Not supported on OpenHarmony.
}

Size2i DisplayServerOpenHarmony::window_get_max_size(DisplayServer::WindowID p_window) const {
	return Size2i();
}

void DisplayServerOpenHarmony::window_set_min_size(const Size2i p_size, DisplayServer::WindowID p_window) {
	// Not supported on OpenHarmony.
}

Size2i DisplayServerOpenHarmony::window_get_min_size(DisplayServer::WindowID p_window) const {
	return Size2i();
}

void DisplayServerOpenHarmony::window_set_size(const Size2i p_size, DisplayServer::WindowID p_window) {
	// Not supported on OpenHarmony.
}

Size2i DisplayServerOpenHarmony::window_get_size(DisplayServer::WindowID p_window) const {
	return OS_OpenHarmony::get_singleton()->get_display_size();
}

Size2i DisplayServerOpenHarmony::window_get_size_with_decorations(DisplayServer::WindowID p_window) const {
	return OS_OpenHarmony::get_singleton()->get_display_size();
}

void DisplayServerOpenHarmony::window_set_mode(DisplayServer::WindowMode p_mode, DisplayServer::WindowID p_window) {
	// Not supported on OpenHarmony.
}

DisplayServer::WindowMode DisplayServerOpenHarmony::window_get_mode(DisplayServer::WindowID p_window) const {
	return WINDOW_MODE_FULLSCREEN;
}

void DisplayServerOpenHarmony::window_set_vsync_mode(VSyncMode p_vsync_mode, WindowID p_window) {
	// Not supported on OpenHarmony.
}

DisplayServer::VSyncMode DisplayServerOpenHarmony::window_get_vsync_mode(WindowID p_window) const {
	return VSyncMode::VSYNC_ADAPTIVE;
}

bool DisplayServerOpenHarmony::window_is_maximize_allowed(DisplayServer::WindowID p_window) const {
	return false;
}

void DisplayServerOpenHarmony::window_set_flag(DisplayServer::WindowFlags p_flag, bool p_enabled, DisplayServer::WindowID p_window) {
	// Not supported on OpenHarmony.
}

bool DisplayServerOpenHarmony::window_get_flag(DisplayServer::WindowFlags p_flag, DisplayServer::WindowID p_window) const {
	return false;
}

void DisplayServerOpenHarmony::window_request_attention(DisplayServer::WindowID p_window) {
	// Not supported on OpenHarmony.
}

void DisplayServerOpenHarmony::window_move_to_foreground(DisplayServer::WindowID p_window) {
	// Not supported on OpenHarmony.
}

bool DisplayServerOpenHarmony::window_is_focused(WindowID p_window) const {
	return true;
}

bool DisplayServerOpenHarmony::window_can_draw(DisplayServer::WindowID p_window) const {
	return true;
}

bool DisplayServerOpenHarmony::can_any_window_draw() const {
	return true;
}

void DisplayServerOpenHarmony::process_events() {
	Input::get_singleton()->flush_buffered_events();
}