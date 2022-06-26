#include "cli/commandinterpreter.h"

namespace cli {

void CommandInterpreter::setErrorString(std::string errorString) {
  _errorString = std::move(errorString);
}

CommandInterpreter::CommandInterpreter(
    std::unique_ptr<ICommandRegistry> commandRegistry)
    : _commandRegistry(std::move(commandRegistry)) {}

InputParseStatus CommandInterpreter::acceptInput(std::string_view line) {
  using IteratorType = std::string_view::const_iterator;
  auto end = line.cend();
  IteratorType startPos = end;

  std::vector<std::string_view> tokens;

  for (auto it = line.cbegin(); it != end; ++it) {
    if (std::isspace(*it)) {
      if (startPos != end) {
        tokens.emplace_back(&startPos[0], static_cast<size_t>(it - startPos));
        startPos = end;
      }
      continue;
    } else if (startPos == end) {
      startPos = it;
    }
  }

  if (startPos != end) {
    tokens.emplace_back(&startPos[0], static_cast<size_t>(end - startPos));
  }

  if (!tokens.empty()) {
    std::string commandName(tokens.front());
    std::unique_ptr<ICommand> command =
        _commandRegistry->lookupCommand(tokens.front());

    if (!command) {
      setErrorString("Cannot recognize the command: " + commandName);
      return InputParseStatus::ErrorUnknownCommand;
    }

    tokens.erase(tokens.begin());

    if (!command->acceptInput(std::move(tokens))) {
      std::string errorString = command->errorString();
      setErrorString(
          "Failed to execute command: '" + commandName +
          "', reason: " + (errorString.empty() ? "unknown" : errorString));
      return InputParseStatus::ErrorInvokedCommandFailed;
    }
  }

  return InputParseStatus::NoError;
}

std::string CommandInterpreter::errorString() const { return _errorString; }

}  // namespace cli
