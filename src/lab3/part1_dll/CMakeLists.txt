set(RSA_LIB_TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab3)
set(LAB3_SRC rsa.cpp rsa.def)
set(LAB3_HDR rsa.h)

add_library(${RSA_LIB_TARGET_NAME} SHARED ${LAB3_SRC} ${LAB3_HDR})
target_compile_definitions(${RSA_LIB_TARGET_NAME} PRIVATE LAB3_LIB_BUILD)
target_compile_options(${RSA_LIB_TARGET_NAME} PUBLIC /EHsc)

set(CLIENT_TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab3_client)
add_executable(${CLIENT_TARGET_NAME} main.c)
target_link_libraries(${CLIENT_TARGET_NAME} PRIVATE ${RSA_LIB_TARGET_NAME})
target_compile_definitions(${CLIENT_TARGET_NAME} PRIVATE UNICODE _UNICODE)
