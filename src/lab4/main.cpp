#include <iostream>

#include "cli/commandinterpreter.h"
#include "builtinregistry.h"

int main() {
  cli::CommandInterpreter interpreter(lab4::builtInCommandRegistry());

  for (;;) {
    std::string input;
    std::cout << "> ";
    if (!std::getline(std::cin, input)) {
      break;
    }
    if (interpreter.acceptInput(input) != cli::InputParseStatus::NoError) {
      std::cout << interpreter.errorString() << '\n';
    }
  }
}
