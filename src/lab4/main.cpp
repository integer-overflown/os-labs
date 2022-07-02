#include <Windows.h>

#include <iostream>

#include "builtinregistry.h"
#include "cli/commandinterpreter.h"
#include "configuration.h"

int main() {
  SetConsoleCP(CP_UTF8);        // set input mode to UTF-8
  SetConsoleOutputCP(CP_UTF8);  // set output mode to UTF-8

  cli::CommandInterpreter interpreter(
      lab4::builtInCommandRegistry(std::make_shared<lab4::Configuration>()));

  for (;;) {
    std::string input;
    std::cout << "> ";
    if (!std::getline(std::cin, input)) {
      break;
    }
    if (cli::InputParseStatus status = interpreter.acceptInput(input);
        status == cli::InputParseStatus::NoError) {
      continue;
    } else if (status == cli::InputParseStatus::ErrorQuitRequested) {
      break;
    } else {
      std::cout << interpreter.errorString() << '\n';
    }
  }
}
