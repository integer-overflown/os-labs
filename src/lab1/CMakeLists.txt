set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab1)
set(LAB1_SRC main.cpp)

option(LAB1_USE_UNICODE "Whether to define _UNICODE/UNICODE during build, default on" ON)

add_executable(${TARGET_NAME} ${LAB1_SRC})
if (LAB1_USE_UNICODE)
    target_compile_definitions(${TARGET_NAME} PRIVATE UNICODE _UNICODE)
endif ()
target_compile_options(${TARGET_NAME} PRIVATE /utf-8)
