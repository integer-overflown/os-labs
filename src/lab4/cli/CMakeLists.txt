set(LAB4_HDR
        "include/cli/commandregistry.h"
        "include/cli/command.h"
        "include/cli/commandregistry.h"
        )
set(LAB4_SRC src/commandinterpreter.cpp)

# CLI stands for command-line interface
add_library(cli_helpers ${LAB4_HDR} ${LAB4_SRC})
target_include_directories(cli_helpers PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
