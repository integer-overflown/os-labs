set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab2)
set(LAB2_SRC rsa.cpp)
set(LAB2_HDR rsa.h)

add_library(${TARGET_NAME} SHARED ${LAB2_SRC} ${LAB2_HDR})
target_compile_definitions(${TARGET_NAME} PRIVATE LAB2_LIB_BUILD)

add_executable(lab2_client main.cpp)
target_link_libraries(lab2_client PRIVATE ${TARGET_NAME})
