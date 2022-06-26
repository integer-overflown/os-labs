#ifndef OSLABS_COMMANDINTERPRETER_H
#define OSLABS_COMMANDINTERPRETER_H
#include <string>
#include <memory>

#include "commandregistry.h"

namespace cli {

enum class InputParseStatus {
  NoError,
  ErrorUnknownCommand,
  ErrorInvokedCommandFailed
};

class CommandInterpreter {
 public:
  explicit CommandInterpreter(
      std::unique_ptr<ICommandRegistry> commandRegistry);
  [[nodiscard]] InputParseStatus acceptInput(std::string_view line);
  [[nodiscard]] std::string errorString() const;

 private:
  void setErrorString(std::string errorString);

  std::string _errorString;
  std::unique_ptr<ICommandRegistry> _commandRegistry;
};

}

#endif  // OSLABS_COMMANDINTERPRETER_H
