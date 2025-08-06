# Create an INTERFACE library to only expose headers and link flags
add_library(glfw INTERFACE)

target_include_directories(glfw INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib)
target_link_libraries(glfw INTERFACE ${GLFW_LIB_PATH})