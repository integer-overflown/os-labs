set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab3)
set(LAB3_SRC rsa.cpp rsa.def)
set(LAB3_HDR rsa.h)

add_library(${TARGET_NAME} SHARED ${LAB3_SRC} ${LAB3_HDR})
target_compile_definitions(${TARGET_NAME} PRIVATE LAB3_LIB_BUILD)
target_compile_options(${TARGET_NAME} PUBLIC /EHsc)

set(CLIENT_TARGET_NAME ${TARGET_NAME}_client)
add_executable(${CLIENT_TARGET_NAME} main.c)
target_link_libraries(${CLIENT_TARGET_NAME} PRIVATE ${TARGET_NAME})
target_compile_definitions(${CLIENT_TARGET_NAME} PRIVATE UNICODE _UNICODE)
