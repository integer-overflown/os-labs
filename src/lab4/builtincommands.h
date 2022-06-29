#ifndef OSLABS_BUILTINCOMMANDS_H
#define OSLABS_BUILTINCOMMANDS_H
#include "cli/command.h"
#include "configuration.h"

class CreateCommand : public cli::AbstractCommand,
                      public cli::BuiltInRegister<CreateCommand> {
 public:
  static constexpr auto cCommandName = "create";
  explicit CreateCommand(std::shared_ptr<lab4::IConfiguration> configuration);
  [[nodiscard]] std::string name() const override;
  bool acceptInput(const std::vector<std::string_view> &tokens) override;
  [[nodiscard]] std::pair<std::size_t, std::size_t> positionalArgumentCount()
      const noexcept override;

 private:
  bool createEmailFile(std::string_view fileName, std::string_view mailBoxName);
  bool createMailBox(std::string_view name, size_t size);

  std::shared_ptr<lab4::IConfiguration> _configuration;
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

class ListCommand : public cli::AbstractCommand,
                    public cli::BuiltInRegister<ListCommand> {
 public:
  static constexpr auto cCommandName = "list";
  explicit ListCommand(std::shared_ptr<lab4::IConfiguration> configuration);
  [[nodiscard]] std::string name() const override;
  bool acceptInput(const std::vector<std::string_view> &tokens) override;

 private:
  bool listMailBoxes();

  std::shared_ptr<lab4::IConfiguration> _configuration;
};

#endif  // OSLABS_BUILTINCOMMANDS_H
