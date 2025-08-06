cmake_minimum_required(VERSION 3.15)
project(HuiTracing)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

find_package(Vulkan REQUIRED)
find_program(glslangValidator_exe NAMES glslangValidator HINTS "$ENV{VULKAN_SDK}/bin")
message(STATUS "glslangValidator path: ${glslangValidator_exe}")

# specify the C++ standard
add_library(cpp_compiler_flags INTERFACE)
target_compile_features(cpp_compiler_flags INTERFACE cxx_std_17)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(cpp_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)


add_subdirectory(HuiEngine)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT HuiEngine)
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
set_target_properties(glm PROPERTIES FOLDER "Dependencies")
set_target_properties(imgui PROPERTIES FOLDER "Dependencies")
