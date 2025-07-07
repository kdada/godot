#include "bridge_openharmony.h"
#include <cstdint>
#include <hilog/log.h>
#include <napi/native_api.h>
#include <native_window/external_window.h>
#include <rawfile/raw_file_manager.h>
#include <vector>

#undef LOG_DOMAIN
#undef LOG_TAG
#define LOG_DOMAIN 0x3200
#define LOG_TAG "LIB_ENTRY"

static NativeResourceManager *resourceManager = nullptr;
static OHNativeWindow *nativeWindow = nullptr;
static int32_t windowId = -1;
static uint32_t windowWidth = 0;
static uint32_t windowHeight = 0;
static bool initialized = false;

static napi_value NAPI_Global_setResourceManager(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    if (napi_ok != napi_get_cb_info(env, info, &argc, args, nullptr, nullptr)) {
        OH_LOG_ERROR(LOG_APP, "GetContext napi_get_cb_info failed");
        return nullptr;
    }
    resourceManager = OH_ResourceManager_InitNativeResourceManager(env, args[0]);
    return nullptr;
}

static napi_value NAPI_Global_setWindowId(napi_env env, napi_callback_info info) {
    if (windowId != -1) {
        OH_LOG_ERROR(LOG_APP, "Window id already exists");
        return nullptr;
    }
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    if (napi_ok != napi_get_cb_info(env, info, &argc, args, nullptr, nullptr)) {
        OH_LOG_ERROR(LOG_APP, "GetContext napi_get_cb_info failed");
        return nullptr;
    }
    if (napi_ok != napi_get_value_int32(env, args[0], &windowId)) {
        OH_LOG_ERROR(LOG_APP, "Get window id failed");
        return nullptr;
    }
    return nullptr;
}


static napi_value NAPI_Global_setSurfaceId(napi_env env, napi_callback_info info) {
    if (nativeWindow != nullptr) {
        OH_LOG_ERROR(LOG_APP, "Native window already exists");
        return nullptr;
    }
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    if (napi_ok != napi_get_cb_info(env, info, &argc, args, nullptr, nullptr)) {
        OH_LOG_ERROR(LOG_APP, "GetContext napi_get_cb_info failed");
        return nullptr;
    }
    int64_t surfaceId = 0;
    bool lossless = true;
    if (napi_ok != napi_get_value_bigint_int64(env, args[0], &surfaceId, &lossless)) {
        OH_LOG_ERROR(LOG_APP, "Get surface id failed");
        return nullptr;
    }
    OH_NativeWindow_CreateNativeWindowFromSurfaceId(surfaceId, &nativeWindow);
    return nullptr;
}

static napi_value NAPI_Global_changeSurface(napi_env env, napi_callback_info info) {
    if (nativeWindow == nullptr) {
        OH_LOG_ERROR(LOG_APP, "Native window does not exists");
        return nullptr;
    }
    size_t argc = 3;
    napi_value args[3] = {nullptr};

    if (napi_ok != napi_get_cb_info(env, info, &argc, args, nullptr, nullptr)) {
        OH_LOG_ERROR(LOG_APP, "GetContext napi_get_cb_info failed");
        return nullptr;
    }
    int64_t surfaceId = 0;
    bool lossless = true;
    if (napi_ok != napi_get_value_bigint_int64(env, args[0], &surfaceId, &lossless)) {
        OH_LOG_ERROR(LOG_APP, "Get surface id failed");
        return nullptr;
    }
    if (napi_ok != napi_get_value_uint32(env, args[1], &windowWidth)) {
        OH_LOG_ERROR(LOG_APP, "Get width failed");
        return nullptr;
    }
    if (napi_ok != napi_get_value_uint32(env, args[2], &windowHeight)) {
        OH_LOG_ERROR(LOG_APP, "Get height failed");
        return nullptr;
    }
    if (initialized) {
        godot_resize(windowWidth, windowHeight);
    }
    return nullptr;
}

static napi_value NAPI_Global_destroySurface(napi_env env, napi_callback_info info) {
    if (nativeWindow == nullptr) {
        return nullptr;
    }
    OH_NativeWindow_DestroyNativeWindow(nativeWindow);
    nativeWindow = nullptr;
    return nullptr;
}

static napi_value NAPI_Global_setup(napi_env env, napi_callback_info info) {
    godot_init(resourceManager, nativeWindow, windowId, windowWidth, windowHeight);
    initialized = true;
    return nullptr;
}

static napi_value NAPI_Global_input(napi_env env, napi_callback_info info) {
    if (!initialized) {
        return nullptr;
    }
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    if (napi_ok != napi_get_cb_info(env, info, &argc, args, nullptr, nullptr)) {
        OH_LOG_ERROR(LOG_APP, "GetContext napi_get_cb_info failed");
        return nullptr;
    }
    uint32_t array_length = 0;
    if (napi_ok != napi_get_array_length(env, args[0], &array_length)) {
        OH_LOG_ERROR(LOG_APP, "Get array length failed");
        return nullptr;
    }

    std::vector<GodotTouchEvent> events;

    for (int i = 0; i < array_length; i++) {
        napi_value element;
        if (napi_ok != napi_get_element(env, args[0], i, &element)) {
            OH_LOG_ERROR(LOG_APP, "Get array element failed");
            return nullptr;
        }

        napi_value event_type;
        if (napi_ok != napi_get_named_property(env, element, "type", &event_type)) {
            OH_LOG_ERROR(LOG_APP, "Get event type failed");
            return nullptr;
        }

        int32_t event_type_int;
        if (napi_ok != napi_get_value_int32(env, event_type, &event_type_int)) {
            OH_LOG_ERROR(LOG_APP, "Get event type int failed");
            return nullptr;
        }

        napi_value event_id;
        if (napi_ok != napi_get_named_property(env, element, "id", &event_id)) {
            OH_LOG_ERROR(LOG_APP, "Get event id failed");
            return nullptr;
        }

        int32_t event_id_int;
        if (napi_ok != napi_get_value_int32(env, event_id, &event_id_int)) {
            OH_LOG_ERROR(LOG_APP, "Get event id int failed");
            return nullptr;
        }

        napi_value event_x;
        if (napi_ok != napi_get_named_property(env, element, "x", &event_x)) {
            OH_LOG_ERROR(LOG_APP, "Get event x failed");
            return nullptr;
        }

        double event_x_double;
        if (napi_ok != napi_get_value_double(env, event_x, &event_x_double)) {
            OH_LOG_ERROR(LOG_APP, "Get event x double failed");
            return nullptr;
        }

        napi_value event_y;
        if (napi_ok != napi_get_named_property(env, element, "y", &event_y)) {
            OH_LOG_ERROR(LOG_APP, "Get event y failed");
            return nullptr;
        }

        double event_y_double;
        if (napi_ok != napi_get_value_double(env, event_y, &event_y_double)) {
            OH_LOG_ERROR(LOG_APP, "Get event y double failed");
            return nullptr;
        }

        GodotTouchEvent event;
        event.type = event_type_int;
        event.id = event_id_int;
        event.x = event_x_double;
        event.y = event_y_double;
        events.push_back(event);
    }
    godot_touch(&events[0], events.size());
    return nullptr;
}

static napi_value NAPI_Global_sendWindowEvent(napi_env env, napi_callback_info info) {
    if (!initialized) {
        return nullptr;
    }

    size_t argc = 1;
    napi_value args[1] = {nullptr};

    if (napi_ok != napi_get_cb_info(env, info, &argc, args, nullptr, nullptr)) {
        OH_LOG_ERROR(LOG_APP, "GetContext napi_get_cb_info failed");
        return nullptr;
    }
    int32_t event = 0;
    if (napi_ok != napi_get_value_int32(env, args[0], &event)) {
        OH_LOG_ERROR(LOG_APP, "Get event id failed");
        return nullptr;
    }
    godot_window_event(event);
    return nullptr;
}

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports) {
    napi_property_descriptor desc[] = {
        {"setResourceManager", nullptr, NAPI_Global_setResourceManager, nullptr, nullptr, nullptr, napi_default,
         nullptr},
        {"setSurfaceId", nullptr, NAPI_Global_setSurfaceId, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"changeSurface", nullptr, NAPI_Global_changeSurface, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"destroySurface", nullptr, NAPI_Global_destroySurface, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"setup", nullptr, NAPI_Global_setup, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"input", nullptr, NAPI_Global_input, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"setWindowId", nullptr, NAPI_Global_setWindowId, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"sendWindowEvent", nullptr, NAPI_Global_sendWindowEvent, nullptr, nullptr, nullptr, napi_default, nullptr}};
    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    return exports;
}
EXTERN_C_END

static napi_module demoModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "entry",
    .nm_priv = ((void *)0),
    .reserved = {0},
};

extern "C" __attribute__((constructor)) void RegisterEntryModule(void) { napi_module_register(&demoModule); }
