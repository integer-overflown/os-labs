set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab1)
set(LAB1_SRC main.cpp)

add_executable(${TARGET_NAME} ${LAB1_SRC})
target_compile_definitions(${TARGET_NAME} PRIVATE UNICODE _UNICODE)
target_compile_options(${TARGET_NAME} PRIVATE /utf-8)
