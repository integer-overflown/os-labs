set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab2)
set(LAB2_SRC rsa.cpp)
set(LAB2_HDR rsa.h)

add_library(${TARGET_NAME} SHARED ${LAB2_SRC} ${LAB2_HDR})
target_compile_definitions(${TARGET_NAME} PRIVATE LAB2_LIB_BUILD)
target_compile_options(${TARGET_NAME} PUBLIC /EHsc)

set(CLIENT_TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab2_client)
add_executable(${CLIENT_TARGET_NAME} main.cpp)
target_link_libraries(${CLIENT_TARGET_NAME} PRIVATE ${TARGET_NAME})
