function(AddResourceDependency target resource)
    add_dependencies(${target} ${resource})
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${resource}> $<TARGET_FILE_DIR:${target}>
            )
endfunction()

add_subdirectory(res)

set(RES_CLIENT_SRC resourcelibrary.cpp)
set(RES_CLIENT_HDR resourcelibrary.h)

set(CLIENT_TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab3_resource_client)
add_executable(${CLIENT_TARGET_NAME} ${RES_CLIENT_SRC} ${RES_CLIENT_HDR})

AddResourceDependency(${CLIENT_TARGET_NAME} lab3_resources_english)
AddResourceDependency(${CLIENT_TARGET_NAME} lab3_resources_ukrainian)
