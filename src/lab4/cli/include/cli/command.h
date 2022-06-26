#ifndef OSLABS_COMMAND_H
#define OSLABS_COMMAND_H
#include <string>
#include <vector>

namespace cli {

class ICommand {
 public:
  virtual ~ICommand() = default;
  [[nodiscard]] virtual std::string name() const = 0;
  virtual bool acceptInput(std::vector<std::string_view> tokens) = 0;
  virtual std::string errorString() = 0;
};

template <typename Sub>
class BuiltInRegister {
 public:
  static constexpr const char* commandName() { return Sub::cCommandName; }
};

}

#endif  // OSLABS_COMMAND_H
