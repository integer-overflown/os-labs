set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab3)
set(LAB3_SRC rsa.cpp rsa.def)
set(LAB3_HDR rsa.h)

add_library(${TARGET_NAME} SHARED ${LAB3_SRC} ${LAB3_HDR})
target_compile_definitions(${TARGET_NAME} PRIVATE LAB3_LIB_BUILD)
target_compile_options(${TARGET_NAME} PUBLIC /EHsc)

add_executable(${TARGET_NAME}_client main.c)
target_link_libraries(${TARGET_NAME}_client PRIVATE ${TARGET_NAME})
