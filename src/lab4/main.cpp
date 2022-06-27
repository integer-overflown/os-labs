#include <iostream>

#include "builtinregistry.h"
#include "cli/commandinterpreter.h"

int main() {
  cli::CommandInterpreter interpreter(lab4::builtInCommandRegistry());

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
