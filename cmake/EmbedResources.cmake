function (embed_resources)
    set (
        oneValueArgs
        TARGET
    )

    set (
        multiValueArgs
        RESOURCES
    )

    cmake_parse_arguments (
        ER "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN}
    )

    function (embedded_resource input_file as_text resulting_file resource_name)
        if (EXISTS "${input_file}")
            get_filename_component(file_name ${input_file} NAME_WLE)

            file (MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/resources")
            set (output_file "${CMAKE_CURRENT_BINARY_DIR}/resources/${file_name}.c")

            string (MAKE_C_IDENTIFIER "${file_name}" variable_name)
            set (variable_name "resource_${variable_name}")

            set ("${resulting_file}" "${output_file}" PARENT_SCOPE)
            set ("${resource_name}" "${variable_name}" PARENT_SCOPE)
        else ()
            message (FATAL_ERROR "[${PROJECT_ID}] Could not find resource \"${input_file}\"")
        endif ()

        if (EXISTS "${output_file}")
            if ("${output_file}" IS_NEWER_THAN "${input_file}")
                return ()
            endif ()
        endif ()

        file (READ "${input_file}" hex_content HEX)
        if (as_text)
            set (hex_content "${hex_content}00")
        endif ()

        string (REPEAT "[0-9a-f]" 32 pattern)
        string (REGEX REPLACE "(${pattern})" "\\1\n" content "${hex_content}")
        string (REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " content "${content}")
        string (REGEX REPLACE ", $" "" content "${content}")

        message (STATUS "[${PROJECT_ID}] Generating ${output_file} from ${input_file}")

        set (array_definition "const uint8_t ${variable_name}[] =\n{\n${content}\n};")
        set (array_size "const size_t ${variable_name}_size = sizeof(${variable_name});")
        set (source "/* Auto generated file. DO NOT EDIT! */\n\n#include <stdint.h>\n#include <string.h>\n\n${array_definition}\n${array_size}\n")
        file (WRITE "${output_file}" "${source}")
    endfunction()

    set (RESOURCE_SOURCES "")
    set (RESOURCE_HEADER_CONTENT "/* Auto generated file. DO NOT EDIT! */\n\n#include <stdint.h>\n#include <string.h>\n\n#ifdef __cplusplus\nextern \"C\" {\n#endif\n")
    list (LENGTH ER_RESOURCES ER_RESOURCES_COUNT)
    set (counter 1)
    foreach (resource IN LISTS ER_RESOURCES)
        if (NOT (${resource} STREQUAL "TEXT"))
            if (counter LESS ER_RESOURCES_COUNT)
                list (GET ER_RESOURCES ${counter} next_value)
                if (next_value STREQUAL "TEXT")
                    embedded_resource ("${resource}" 1 resulting_file variable_name)
                else ()
                    embedded_resource ("${resource}" 0 resulting_file variable_name)
                endif ()
            else ()
                embedded_resource ("${resource}" 0 resulting_file variable_name)
            endif ()

            list (APPEND RESOURCE_SOURCES "${resulting_file}")
            set (RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}\nextern const uint8_t ${variable_name}[];\nextern const size_t ${variable_name}_size;\n")
        endif ()

        math (EXPR counter "${counter}+1")
    endforeach ()
    set (RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}\n#ifdef __cplusplus\n}\n#endif\n")
    file (WRITE "${CMAKE_CURRENT_BINARY_DIR}/resources/resources.h" "${RESOURCE_HEADER_CONTENT}")

    add_library (
        ${ER_TARGET}_resources STATIC
        ${RESOURCE_SOURCES}
    )
    set_property (TARGET ${ER_TARGET}_resources PROPERTY POSITION_INDEPENDENT_CODE ON)

    target_include_directories (
        ${ER_TARGET}_resources SYSTEM INTERFACE
        ${CMAKE_CURRENT_BINARY_DIR}/resources/
    )

    target_link_libraries (
        ${ER_TARGET} PUBLIC
        ${ER_TARGET}_resources
    )
endfunction ()
