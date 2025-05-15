file(GLOB_RECURSE srcs *.h *.cpp)

add_library(graphics STATIC ${srcs})

target_link_libraries(graphics PRIVATE ${Vulkan_LIBRARY})
target_include_directories(graphics PRIVATE 
    "${Vulkan_INCLUDE_DIRS}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    )

#disable warning from VMA
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(graphics PRIVATE 
        -Wno-nullability-completeness
        -Wno-nullability-extension
        )
endif()

# if(MSVC AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     message(STATUS "[emt] msvc clang-cl off the VMA warnings")
#     target_compile_options(graphics PRIVATE /Wno-nullability-completeness)
# endif()



