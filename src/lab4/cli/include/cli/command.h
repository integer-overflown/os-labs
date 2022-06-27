#ifndef OSLABS_COMMAND_H
#define OSLABS_COMMAND_H
#include <iostream>
#include <string>
#include <vector>

namespace cli {

class ICommand {
 public:
  virtual ~ICommand() noexcept = default;
  [[nodiscard]] virtual std::string name() const = 0;
  virtual bool acceptInput(const std::vector<std::string_view> &tokens) = 0;
  [[nodiscard]] virtual std::string errorString() const = 0;
  [[nodiscard]] virtual std::pair<size_t, size_t> positionalArgumentCount()
      const noexcept = 0;
  virtual void printHelp(std::ostream &out) const = 0;
};

template <typename Sub>
class BuiltInRegister {
 public:
  static constexpr const char *commandName() { return Sub::cCommandName; }
};

class AbstractCommand : public ICommand {
 public:
  [[nodiscard]] std::string errorString() const override {
    return _errorString;
  }
  [[nodiscard]] std::pair<std::size_t, std::size_t> positionalArgumentCount() const noexcept override;
  void printHelp(std::ostream &out) const override;

 protected:
  void setErrorString(std::string errorString) {
    _errorString = std::move(errorString);
  }

  void addPositionalArgument(std::string name, std::string help);

  void setCommandDescription(std::string commandDescription);

 private:
  std::string _errorString;
  std::string _commandDescription;
  std::vector<std::pair<std::string, std::string>> _positionalArguments;
};

}  // namespace cli

#endif  // OSLABS_COMMAND_H
