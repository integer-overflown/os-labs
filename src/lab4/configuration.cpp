#include "configuration.h"

#include "settings.h"

namespace lab4 {

std::vector<MailBox> Configuration::availableMailBoxes() const {
  Settings settings;
  std::vector<MailBox> mailBoxes;
  return settings.read(mailBoxes) ? mailBoxes : std::vector<MailBox>();
}

bool Configuration::addMailBox(const MailBox& mailBox) {
  Settings settings;
  return settings.write(mailBox);
}

}
