set(TARGET_NAME ${PROJECT_EXE_NAME_PREFIX}lab7_threaded_matrix)
set(TASK3_HDR matrix.h)
set(TASK3_SRC main.cpp matrix.cpp)

add_executable(${TARGET_NAME} ${TASK3_HDR} ${TASK3_SRC})
