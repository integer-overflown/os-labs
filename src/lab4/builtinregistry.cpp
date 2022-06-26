#include "builtinregistry.h"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cli/command.h"

class CreateCommand : public cli::ICommand,
                      public cli::BuiltInRegister<CreateCommand> {
 public:
  static constexpr auto cCommandName = "create";
  [[nodiscard]] std::string name() const override;
  bool acceptInput(std::vector<std::string_view> tokens) override;
  std::string errorString() override;
};

std::string CreateCommand::name() const { return cCommandName; }

bool CreateCommand::acceptInput(std::vector<std::string_view> tokens) {
  return true;
}

std::string CreateCommand::errorString() { return {}; }

class BuiltInCommandRegistry : public cli::ICommandRegistry {
  using CommandSupplier = std::unique_ptr<cli::ICommand> (*)();
  using CommandMap = std::unordered_map<std::string, CommandSupplier>;

 public:
  BuiltInCommandRegistry();
  std::unique_ptr<cli::ICommand> lookupCommand(
      std::string_view commandName) override;

 private:
  static CommandMap buildInCommands();

  template <typename T>
  std::enable_if_t<std::is_base_of_v<cli::ICommand, T> &&
                       std::is_base_of_v<cli::BuiltInRegister<T>, T>,
                   CommandMap::value_type> static buildInCommandFactory() {
    return {T::commandName(), []() -> std::unique_ptr<cli::ICommand> {
              return std::make_unique<T>();
            }};
  }

  CommandMap _commandMap;
};

std::unique_ptr<cli::ICommandRegistry> lab4::builtInCommandRegistry() {
  return std::make_unique<BuiltInCommandRegistry>();
}

std::unique_ptr<cli::ICommand> BuiltInCommandRegistry::lookupCommand(
    std::string_view commandName) {
  auto it = _commandMap.find(std::string(commandName));
  return it == _commandMap.end() ? std::unique_ptr<cli::ICommand>()
                                 : it->second();
}

BuiltInCommandRegistry::CommandMap BuiltInCommandRegistry::buildInCommands() {
  // this is the place for extending built-in commands
  return {buildInCommandFactory<CreateCommand>()};
}

BuiltInCommandRegistry::BuiltInCommandRegistry()
    : _commandMap(buildInCommands()) {}
