#include "bridge_openharmony.h"
#include "napi/native_api.h"
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <hilog/log.h>
#include <native_window/external_window.h>

#undef LOG_DOMAIN
#undef LOG_TAG
#define LOG_DOMAIN 0x3200
#define LOG_TAG "LIB_ENTRY"

static napi_value Add(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value args[2] = {nullptr};

    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    napi_valuetype valuetype0;
    napi_typeof(env, args[0], &valuetype0);

    napi_valuetype valuetype1;
    napi_typeof(env, args[1], &valuetype1);

    double value0;
    napi_get_value_double(env, args[0], &value0);

    double value1;
    napi_get_value_double(env, args[1], &value1);

    napi_value sum = 0;
    napi_create_double(env, godot_add(int64_t(value0), int64_t(value1)), &sum);

    return sum;
}

void OnSurfaceCreatedCB(OH_NativeXComponent *component, void *window) {
    OH_LOG_INFO(LOG_APP, "Initializing Godot");
    OHNativeWindow *nativeWindow = static_cast<OHNativeWindow *>(window);
    auto result = godot_init(nativeWindow);
    OH_LOG_INFO(LOG_APP, "Initialized Godot");
}

static OH_NativeXComponent_Callback callback;

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports) {
    napi_property_descriptor desc[] = {{"add", nullptr, Add, nullptr, nullptr, nullptr, napi_default, nullptr}};
    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);

    napi_value exportInstance = nullptr;
    OH_NativeXComponent *nativeXComponent = nullptr;

    // Get nativeXComponent
    napi_get_named_property(env, exports, OH_NATIVE_XCOMPONENT_OBJ, &exportInstance);

    napi_unwrap(env, exportInstance, reinterpret_cast<void **>(&nativeXComponent));
    // Get XComponentId
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize);

    // Register XComponent Callback
    callback.OnSurfaceCreated = OnSurfaceCreatedCB;
    int32_t ret = OH_NativeXComponent_RegisterCallback(nativeXComponent, &callback);
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
