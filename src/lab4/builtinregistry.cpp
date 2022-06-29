#include "builtinregistry.h"

#include <cassert>
#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "builtincommands.h"
#include "cli/command.h"

class BuiltInCommandRegistry : public cli::ICommandRegistry {
  using CommandSupplier = std::function<std::unique_ptr<cli::ICommand>()>;
  using CommandMap = std::unordered_map<std::string, CommandSupplier>;

 public:
  explicit BuiltInCommandRegistry(
      std::shared_ptr<lab4::IConfiguration> configuration);

  std::unique_ptr<cli::ICommand> lookupCommand(
      std::string_view commandName) override;

 private:
  template <typename T>
  std::enable_if_t<std::is_base_of_v<cli::ICommand, T> &&
                       std::is_base_of_v<cli::BuiltInRegister<T>, T>,
                   CommandMap::value_type>
  builtInCommandFactory() {
    return {
        T::commandName(),
        [configuration = _configuration]() -> std::unique_ptr<cli::ICommand> {
          if constexpr (std::is_constructible_v<
                            T, std::shared_ptr<lab4::IConfiguration>>) {
            return std::make_unique<T>(configuration);
          } else {
            return std::make_unique<T>();
          }
        }};
  }

  CommandMap _commandMap;
  std::shared_ptr<lab4::IConfiguration> _configuration;
};

std::unique_ptr<cli::ICommandRegistry> lab4::builtInCommandRegistry(
    std::shared_ptr<lab4::IConfiguration> configuration) {
  return std::make_unique<BuiltInCommandRegistry>(std::move(configuration));
}

std::unique_ptr<cli::ICommand> BuiltInCommandRegistry::lookupCommand(
    std::string_view commandName) {
  auto it = _commandMap.find(std::string(commandName));
  return it == _commandMap.end() ? std::unique_ptr<cli::ICommand>()
                                 : it->second();
}

BuiltInCommandRegistry::BuiltInCommandRegistry(
    std::shared_ptr<lab4::IConfiguration> configuration)
    : _configuration(std::move(configuration)) {
  assert(_configuration);
  _commandMap = {builtInCommandFactory<CreateCommand>(),
                 builtInCommandFactory<DeleteCommand>(),
                 builtInCommandFactory<ListCommand>()};
}
