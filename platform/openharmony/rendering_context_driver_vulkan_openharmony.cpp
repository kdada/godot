#ifdef VULKAN_ENABLED
#include "rendering_context_driver_vulkan_openharmony.h"

const char *RenderingContextDriverVulkanOpenHarmony::_get_platform_surface_extension() const {
	return VK_OHOS_SURFACE_EXTENSION_NAME;
}

RenderingContextDriver::SurfaceID RenderingContextDriverVulkanOpenHarmony::surface_create(const void *p_platform_data) {
	const WindowPlatformData *wpd = (const WindowPlatformData *)(p_platform_data);

	VkSurfaceCreateInfoOHOS create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_SURFACE_CREATE_INFO_OHOS;
	create_info.window = wpd->window;

	VkSurfaceKHR vk_surface = VK_NULL_HANDLE;
	VkResult err = vkCreateSurfaceOHOS(instance_get(), &create_info, get_allocation_callbacks(VK_OBJECT_TYPE_SURFACE_KHR), &vk_surface);
	ERR_FAIL_COND_V(err != VK_SUCCESS, SurfaceID());

	Surface *surface = memnew(Surface);
	surface->vk_surface = vk_surface;
	return SurfaceID(surface);
}

bool RenderingContextDriverVulkanOpenHarmony::_use_validation_layers() const {
	TightLocalVector<const char *> layer_names;
	Error err = _find_validation_layers(layer_names);

	// On Android, we use validation layers automatically if they were explicitly linked with the app.
	return (err == OK) && !layer_names.is_empty();
}

#endif // VULKAN_ENABLED
