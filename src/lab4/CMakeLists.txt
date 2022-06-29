set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab4)
set(LAB4_HDR builtinregistry.h builtincommands.h settings.h mailbox.h log.h iserialazable.h)
set(LAB4_SRC main.cpp builtinregistry.cpp builtincommands.cpp mailbox.cpp settings.cpp)

add_executable(${TARGET_NAME} ${LAB4_SRC} ${LAB4_HDR})

add_subdirectory(cli)

target_link_libraries(${TARGET_NAME} PRIVATE cli_helpers)
target_compile_definitions(${TARGET_NAME} PRIVATE _UNICODE UNICODE)
