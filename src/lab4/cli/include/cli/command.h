#ifndef OSLABS_COMMAND_H
#define OSLABS_COMMAND_H
#include <string>
#include <vector>

namespace cli {

class ICommand {
 public:
  virtual ~ICommand() noexcept = default;
  [[nodiscard]] virtual std::string name() const = 0;
  virtual bool acceptInput(const std::vector<std::string_view> &tokens) = 0;
  virtual std::string errorString() = 0;
};

template <typename Sub>
class BuiltInRegister {
 public:
  static constexpr const char* commandName() { return Sub::cCommandName; }
};

class AbstractCommand : public ICommand {
 public:
  std::string errorString() override { return _errorString; }

 protected:
  void setErrorString(std::string errorString) {
    _errorString = std::move(errorString);
  }

 private:
  std::string _errorString;
};

}  // namespace cli

#endif  // OSLABS_COMMAND_H
