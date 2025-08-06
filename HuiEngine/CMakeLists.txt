set(PROJECT HuiEngine)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h src/*.hpp src/*.cpp)

# Shaders
set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

file(GLOB SHADERS
    ${SHADER_SOURCE_DIR}/*.vert
    ${SHADER_SOURCE_DIR}/*.frag
    ${SHADER_SOURCE_DIR}/*.comp
    ${SHADER_SOURCE_DIR}/*.geom
    ${SHADER_SOURCE_DIR}/*.tesc
    ${SHADER_SOURCE_DIR}/*.tese
    ${SHADER_SOURCE_DIR}/*.mesh
    ${SHADER_SOURCE_DIR}/*.task
    ${SHADER_SOURCE_DIR}/*.rgen
    ${SHADER_SOURCE_DIR}/*.rchit
    ${SHADER_SOURCE_DIR}/*.rmiss)
  
foreach(source IN LISTS SHADERS)
    get_filename_component(FILENAME ${source} NAME)
    add_custom_command(
        COMMAND
            ${glslangValidator_exe}
            -o ${SHADER_BINARY_DIR}/${FILENAME}.spv
            -V ${source}
        OUTPUT ${SHADER_BINARY_DIR}/${FILENAME}.spv
        DEPENDS ${source} #${SHADER_BINARY_DIR}
        COMMENT "Compiling ${FILENAME}"
    )
    list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv)
endforeach()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS} SOURCES ${SHADERS})

# vendor
add_subdirectory(vendor/ImGui)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/stb)
add_subdirectory(vendor/tinyobjloader)
add_subdirectory(vendor/GLFW)

# Create console app
add_executable(${PROJECT} ${SOURCES})
# Ensure shaders build first
add_dependencies(${PROJECT} shaders)

# Imgui implementation
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
target_sources(${PROJECT} PRIVATE 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
    )

target_link_vendor(${PROJECT} PUBLIC
    cpp_compiler_flags
    stb 
    tinyobjloader
    Vulkan::Vulkan
    glfw
    imgui
    )

target_include_directories(${PROJECT} PUBLIC 
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

set_target_properties(${PROJECT} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(OUTPUT_DIR "$<CONFIG>-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

set_target_properties(${PROJECT} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR}/${PROJECT}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR}/${PROJECT}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR}/${PROJECT}"
)

# Tell Visual Studio to group files by folder structure
foreach(file IN LISTS SOURCES)
    # Get relative path from source dir
    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}" "${file}")

    # Set the folder property to the relative folder of the file
    get_filename_component(folder "${rel_path}" PATH)
    set_source_files_properties(${file} PROPERTIES
        VS_FOLDER "${folder}"
    )
endforeach()