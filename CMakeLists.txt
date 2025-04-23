cmake_minimum_required(VERSION 3.22)

project(emt-vulkan LANGUAGES C CXX)

find_package(Vulkan REQUIRED)

if(Vulkan_LIBRARY)
    message(STATUS "[emt] found the vulkand ${Vulkan_LIBRARY}")
    message(STATUS "[emt] vulkan include : ${Vulkan_INCLUDE_DIRS}")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

add_subdirectory(source/engine)

add_subdirectory(source/sample)
