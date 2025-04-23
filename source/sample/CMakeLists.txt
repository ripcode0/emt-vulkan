
add_executable(sample main.cpp)

target_link_libraries(sample PRIVATE graphics)
target_include_directories(sample PRIVATE
    ${CMAKE_SOURCE_DIR}/source/engine
    "${Vulkan_INCLUDE_DIRS}"
)