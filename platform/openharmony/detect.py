# Build:
#   x86-64: scons platform=openharmony target=template_debug optimize=debug debug_symbols=true opengl3=false arch=x86_64 verbose=true
#   arm64: scons platform=openharmony target=template_debug optimize=debug debug_symbols=true arch=arm64 opengl3=false verbose=true
import os
import sys
from typing import TYPE_CHECKING

from methods import print_error, print_warning
from platform_methods import validate_arch

if TYPE_CHECKING:
    from SCons.Script.SConscript import SConsEnvironment


def get_name():
    return "OpenHarmony"


def can_build():
    return True

def get_tools(env: "SConsEnvironment"):
    return ["clang", "clang++", "as", "ar", "link"]

def get_opts():
    return []


def get_doc_classes():
    return []


def get_doc_path():
    return "doc_classes"


def get_flags():
    return {
        "arch": "x86_64",
        "target": "template_debug",
        "builtin_pcre2_with_jit": False,
    }


def configure(env: "SConsEnvironment"):
    # Validate arch.
    supported_arches = ["arm64", "x86_64"]
    validate_arch(env["arch"], get_name(), supported_arches)

    ## LTO

    if env["lto"] == "auto":  # Disable by default as it makes linking in Xcode very slow.
        env["lto"] = "none"

    if env["lto"] != "none":
        if env["lto"] == "thin":
            env.Append(CCFLAGS=["-flto=thin"])
            env.Append(LINKFLAGS=["-flto=thin"])
        else:
            env.Append(CCFLAGS=["-flto"])
            env.Append(LINKFLAGS=["-flto"])

    ## Compiler configuration

    # Save this in environment for use by other modules
    env["ENV"]["PATH"] = "C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/bin;" + env["ENV"]["PATH"]

    env["CC"] = "clang"
    env["CXX"] = "clang++"
    env["S_compiler"] = "clang"
    env["AR"] = "llvm-ar"
    env["AS"] = "llvm-as"
    env["LINK"] = "ld.lld"
    env["RANLIB"] = "llvm-ranlib" 

    ## Compile flags
    # "C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/bin/clang++.exe" -cc1 -triple x86_64-unknown-linux-ohos -emit-obj -mrelax-all --mrelax-relocations -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.cpp -static-define -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -target-cpu x86-64 -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning -debugger-tuning=gdb -v -fcoverage-compilation-dir=D:/Dev/GitHub/tets-harmonyos-native -resource-dir "C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/lib/clang/15.0.4" -internal-isystem "C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/bin/../include/libcxx-ohos/include/c++/v1" -internal-isystem "C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/lib/clang/15.0.4/include" -internal-externc-isystem "C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/bin/../../sysroot/usr/include/x86_64-linux-ohos" -internal-externc-isystem "C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/bin/../../sysroot/include" -internal-externc-isystem "C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/bin/../../sysroot/usr/include" -fdeprecated-macro -fdebug-compilation-dir=D:/Dev/GitHub/tets-harmonyos-native -ferror-limit 19 -fmessage-length=120 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o C:/Users/Kira/AppData/Local/Temp/main-db1a7c.o -x c++ ".\\main.cpp"

    env.Append(
        CPPPATH=[
            #"C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/include/c++/v1",
            "C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/lib/clang/15.0.4/include",
            "C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/llvm/include/libcxx-ohos/include/c++/v1",
        ]
    )
    if env["arch"] == "x86_64":
        env.Append(
            CCFLAGS=[
                "-fobjc-arc", "--target=x86_64-linux-ohos", "-fPIC",
                "-fobjc-abi-version=2", "-fobjc-legacy-dispatch", "-fmessage-length=0", "-fpascal-strings", "-fblocks",
                "-fasm-blocks", "-isysroot='C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/sysroot'",
            ]
            
        )
        env.Append(
            CPPPATH=["C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/sysroot/usr/include/x86_64-linux-ohos"]
        )
        env.Append(CXXFLAGS=[
            "--target=x86_64-linux-ohos",
            "--sysroot=C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/sysroot/",
        ])
        env.Append(LINKFLAGS=[
            "--target=x86_64-linux-ohos",
            "--sysroot=C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/sysroot/",
        ])
        env.Append(ASFLAGS=["-arch", "x86_64"])
    elif env["arch"] == "arm64":
        env.Append(
            CCFLAGS=[
                "-fobjc-arc", "--target=aarch64-linux-ohos", "-fPIC",
                "-fobjc-abi-version=2", "-fobjc-legacy-dispatch", "-fmessage-length=0", "-fpascal-strings", "-fblocks",
                "-fasm-blocks", "-isysroot='C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/sysroot'",
            ]
        )
        env.Append(
            CPPPATH=["C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/sysroot/usr/include/aarch64-linux-ohos"]
        )
        env.Append(CXXFLAGS=[
            "--target=aarch64-linux-ohos",
            "--sysroot=C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/sysroot/",
        ])
        env.Append(LINKFLAGS=[
            "--target=aarch64-linux-ohos",
            "--sysroot=C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/sysroot/",
        ])
        env.Append(ASFLAGS=["-arch", "aarch64"])

    env.Append(
        CPPPATH=["C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/sysroot/usr/include"]
    )

    env.Append(CPPPATH=["#platform/openharmony"])
    env.Append(CPPDEFINES=["UNIX_ENABLED", "__OPEN_HARMONY__", "MBEDTLS_NO_UDBL_DIVISION"])

    if env["vulkan"]:
        env.Append(CPPDEFINES=["VULKAN_ENABLED", "RD_ENABLED"])
        if not env["use_volk"]:
            env.Append(LIBS=["vulkan"])

    if env["opengl3"]:
        env.Append(CPPDEFINES=["GLES3_ENABLED"])
        env.Append(LIBS=["GLESv3"])
    
    env['ARGMAX'] = 8000
    print("CPPPATH:", env["CPPPATH"])
    print("CCFLAGS:", env["CCFLAGS"])
    print("CXXFLAGS:", env["CXXFLAGS"])
    print("LINKFLAGS:", env["LINKFLAGS"])
    print("ARGMAX", env['ARGMAX'])


    # 使用响应文件替换原始文件列表
    env['LINKCOM'] = '$LINK @${TARGET}.rsp'
    env['SHLINKFLAGS'] = '"--sysroot=C:/Program Files/Huawei/DevEco Studio/sdk/default/openharmony/native/sysroot/" -shared -soname libgodot.so '

    # 创建响应文件的 Action
    def create_rsp(target, source, env):
        rsp_file = str(target[0]) + '.rsp'
        with open(rsp_file, 'w') as f:
            # 写入所有源文件路径（每行一个）
            f.write('\n'.join(str(s) for s in source))
        return 0


    env["SHLIBSUFFIX"] = ".so"
    # 将生成响应文件的操作添加到链接前
    env['LINKCOM'] = env.Action(create_rsp, "Generating RSP: ${TARGET}.rsp") + env['LINKCOM']



    env['ARCOM'] = '$AR $ARFLAGS $TARGET @${TARGET}.rsp'

    def create_ar_rsp(target, source, env):
        # 只包含对象文件
        obj_files = [str(s) for s in source if s.get_suffix() in ['.o', '.obj']]
        rsp_file = str(target[0]) + '.rsp'
        with open(rsp_file, 'w') as f:
            f.write('\n'.join(obj_files))
        return 0

    env['ARCOM'] = env.Action(create_ar_rsp, "Generating AR RSP: ${TARGET}.rsp") + env['ARCOM']