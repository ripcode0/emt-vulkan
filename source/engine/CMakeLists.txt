file(GLOB_RECURSE srcs *.h *.cpp)

add_library(graphics STATIC ${srcs})

target_link_libraries(graphics PRIVATE ${Vulkan_LIBRARY})
target_include_directories(graphics PRIVATE 
    "${Vulkan_INCLUDE_DIRS}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    )

