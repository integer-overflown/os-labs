#ifndef OSLABS_CONFIGURATION_H
#define OSLABS_CONFIGURATION_H
#include <vector>
#include <optional>

#include "mailbox.h"

namespace lab4 {

class IConfiguration {
 public:
  virtual ~IConfiguration() = default;
  [[nodiscard]] virtual std::optional<std::vector<MailBox>> availableMailBoxes() const = 0;
  [[nodiscard]] virtual bool addMailBox(const MailBox &mailBox) = 0;
};

class Configuration : public IConfiguration {
 public:
  [[nodiscard]] std::optional<std::vector<MailBox>> availableMailBoxes() const override;
  [[nodiscard]] bool addMailBox(const MailBox &mailBox) override;
};

}  // namespace lab4

#endif  // OSLABS_CONFIGURATION_H
