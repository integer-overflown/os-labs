#include "configuration.h"

#include "settings.h"

namespace lab4 {

std::optional<std::vector<MailBox>> Configuration::availableMailBoxes() const {
  Settings settings;
  std::vector<MailBox> mailBoxes;

  if (!settings.read(mailBoxes)) {
    return {};
  }

  return mailBoxes;
}

bool Configuration::addMailBox(const MailBox& mailBox) {
  Settings settings;
  return settings.write(mailBox);
}

}  // namespace lab4
