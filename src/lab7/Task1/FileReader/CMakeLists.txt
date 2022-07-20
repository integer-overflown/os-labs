set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab7_file_reader)
set(TASK1_HDR)
set(TASK1_SRC "main.cpp")

add_executable(${TARGET_NAME} ${TASK1_HDR} ${TASK1_SRC})
set_property(TARGET ${TARGET_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${LAB7_BIN_OUTPUT_DIR}/$<CONFIG>)

target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_EXE_NAME_PREFIX}lab7_utils)

if (LAB7_UNICODE)
    target_compile_definitions(${TARGET_NAME} PRIVATE _UNICODE UNICODE)
endif()
