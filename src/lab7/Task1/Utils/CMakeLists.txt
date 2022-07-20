set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab7_utils)

add_library(${TARGET_NAME} INTERFACE)
target_include_directories(${TARGET_NAME} INTERFACE include)
target_sources(${TARGET_NAME} INTERFACE include/Utils/output.h include/Utils/process.h include/Utils/file.h)

if (LAB7_UNICODE)
    target_compile_definitions(${TARGET_NAME} INTERFACE _UNICODE UNICODE)
endif()
