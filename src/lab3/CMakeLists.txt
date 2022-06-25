function(AddResourceDependency target resource)
    add_dependencies(${target} ${resource})
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${resource}> $<TARGET_FILE_DIR:${target}>
            )
endfunction()

set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab3)
set(LAB3_SRC rsa.cpp rsa.def resourcelibrary.cpp resourcelibrary.h)
set(LAB3_HDR rsa.h)

add_library(${TARGET_NAME} SHARED ${LAB3_SRC} ${LAB3_HDR})
target_compile_definitions(${TARGET_NAME} PRIVATE LAB3_LIB_BUILD)
target_compile_options(${TARGET_NAME} PUBLIC /EHsc)

set(CLIENT_TARGET_NAME ${TARGET_NAME}_client)
add_executable(${CLIENT_TARGET_NAME} main.c)
target_link_libraries(${CLIENT_TARGET_NAME} PRIVATE ${TARGET_NAME})
target_compile_definitions(${CLIENT_TARGET_NAME} PRIVATE UNICODE _UNICODE)

add_subdirectory(res)

AddResourceDependency(${CLIENT_TARGET_NAME} lab3_resources_english)
AddResourceDependency(${CLIENT_TARGET_NAME} lab3_resources_ukrainian)
