message(STATUS "[emt] compile shaders ...")

set(data_dir "${CMAKE_SOURCE_DIR}/data")
set(data_link_dir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data")

set(shader_dir "${data_dir}/shader")

find_program(glslang_validator NAMES glslangValidator)
find_program(glslc NAMES glslc)
if(glslang_validator AND glslc)
    message("FF")
endif()

macro(compile_spirv shader_name stage)
    add_custom_command(
        OUTPUT ${shader_dir}/${shader_name}.spv
        COMMAND ${glslang_validator} -V -S ${stage} -o ${shader_name}.spv ${shader_name} > /dev/null
    )
endmacro()



file(GLOB_RECURSE shaders ${shader_dir}/*.glsl)

foreach(shader IN LISTS shaders)
    message(STATUS "[emt] shader : ${shader}")
    set(stage "")
    if(shader MATCHES ".*(vs|vert).*")
        set(stage "vert")
    elseif(shader MATCHES ".*(ps|fs).*")
        set(stage "frag")
    else()
        message(WARNING "[emt] unkown stage type : ${shader}")
    endif()

    get_filename_component(shader_name "${shader}" NAME_WE)

    set(spv_file "${shader_dir}/${shader_name}.spv")

    add_custom_command(
        OUTPUT "${spv_file}"
        COMMAND ${glslc} -fshader-stage=${stage} "${shader}" -o "${spv_file}"
        DEPENDS "${shader}"
        COMMENT "[emt] compiling ${shader} : ${stage} to ${spv_file}"
    )

    list(APPEND compiled_shaders "${spv_file}")
endforeach()

# add_custom_target(compile_shaders ALL DEPENDS ${compiled_shaders})


set(symbol_link_support false)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_BINARY_DIR}/temp_src" "${CMAKE_BINARY_DIR}/temp_link"
    RESULT_VARIABLE link_result
    OUTPUT_QUIET ERROR_QUIET
)
if(NOT link_result EQUAL 0)
    message(STATUS "[emt] The OS does not support symbolic links due to permission restrictions, so we will use a copy command instead.")
else()
    set(symbol_link_support true)
    file(REMOVE "${CMAKE_BINARY_DIR}/temp_link")
endif()

if(symbol_link_support)
    add_custom_target(link_data ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            "${data_dir}"
            "${data_link_dir}"
    COMMENT "[emt] creating symbolic link to data directory"
    )
else()
    add_custom_target(copy_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${data_dir}" "${data_link_dir}"
    COMMENT "[emt] copying data directories"
    )

endif()

add_custom_target(compile_shaders ALL DEPENDS ${compiled_shaders})

if(NOT symbol_link_support)
    add_dependencies(copy_data compile_shaders)
endif()