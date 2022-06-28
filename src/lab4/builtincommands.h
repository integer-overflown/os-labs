#ifndef OSLABS_BUILTINCOMMANDS_H
#define OSLABS_BUILTINCOMMANDS_H
#include "cli/command.h"

class CreateCommand : public cli::AbstractCommand,
                      public cli::BuiltInRegister<CreateCommand> {
 public:
  static constexpr auto cCommandName = "create";
  CreateCommand();
  [[nodiscard]] std::string name() const override;
  bool acceptInput(const std::vector<std::string_view> &tokens) override;

 private:
  bool createEmailFile(std::string_view fileName);
};

class DeleteCommand : public cli::AbstractCommand,
                      public cli::BuiltInRegister<DeleteCommand> {
 public:
  static constexpr auto cCommandName = "delete";
  DeleteCommand();
  [[nodiscard]] std::string name() const override;
  bool acceptInput(const std::vector<std::string_view> &tokens) override;
  [[nodiscard]] std::pair<std::size_t, std::size_t> positionalArgumentCount()
      const noexcept override;

 private:
  bool deleteEmailFile(std::string_view fileName);
};

#endif  // OSLABS_BUILTINCOMMANDS_H
