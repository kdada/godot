#include "bridge_openharmony.h"
#include <cstdint>
#include <hilog/log.h>
#include <napi/native_api.h>
#include <native_window/external_window.h>
#include <rawfile/raw_file_manager.h>

#undef LOG_DOMAIN
#undef LOG_TAG
#define LOG_DOMAIN 0x3200
#define LOG_TAG "LIB_ENTRY"

static NativeResourceManager *resourceManager = nullptr;
static OHNativeWindow *nativeWindow = nullptr;
static int64_t windowWidth = 0;
static int64_t windowHeight = 0;

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
    if (napi_ok != napi_get_value_bigint_int64(env, args[1], &windowWidth, &lossless)) {
        OH_LOG_ERROR(LOG_APP, "Get width failed");
        return nullptr;
    }
    if (napi_ok != napi_get_value_bigint_int64(env, args[2], &windowHeight, &lossless)) {
        OH_LOG_ERROR(LOG_APP, "Get height failed");
        return nullptr;
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
    godot_init(resourceManager, nativeWindow, windowWidth, windowHeight);
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
        {"setup", nullptr, NAPI_Global_setup, nullptr, nullptr, nullptr, napi_default, nullptr}};
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
