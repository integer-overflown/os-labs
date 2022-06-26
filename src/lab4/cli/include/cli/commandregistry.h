#ifndef OSLABS_COMMANDREGISTRY_H
#define OSLABS_COMMANDREGISTRY_H
#include <memory>
#include <string_view>

#include "command.h"

namespace cli {
class ICommandRegistry {
 public:
  virtual ~ICommandRegistry() = default;
  virtual std::unique_ptr<ICommand> lookupCommand(
      std::string_view commandName) = 0;
};
}  // namespace cli

#endif  // OSLABS_COMMANDREGISTRY_H
