cmake_minimum_required(VERSION 3.22)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(emt-vulkan LANGUAGES C CXX)

find_package(Vulkan REQUIRED)

if(Vulkan_LIBRARY)
    message(STATUS "[emt] found the vulkand ${Vulkan_LIBRARY}")
    message(STATUS "[emt] vulkan include : ${Vulkan_INCLUDE_DIRS}")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")

include(compile_shaders)

add_subdirectory(source/engine)

add_subdirectory(source/sample)

add_dependencies(sample copy_data)




