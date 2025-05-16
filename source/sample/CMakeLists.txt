
file(GLOB_RECURSE srcs *.h *.cpp)

add_executable(sample ${srcs})

target_link_libraries(sample PRIVATE graphics)

target_include_directories(sample PRIVATE
    ${CMAKE_SOURCE_DIR}/source/engine
    "${Vulkan_INCLUDE_DIRS}"
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(sample PRIVATE 
        -Wno-nullability-completeness
        -Wno-nullability-extension
        )
endif()

set_target_properties(sample PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
# add_custom_command(
#     TARGET sample POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${data_dir}"
#     "${data_dir_dst}"
#     COMMENT "creating symbolic link to data directory"
# )

