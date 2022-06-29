#include "settings.h"

#include <algorithm>

#include "log.h"

namespace lab4 {

Settings::Settings(std::string_view location) : _outSettingsFile() {
  _outSettingsFile.open(location.data(), std::ios_base::in |
                                             std::ios_base::out |
                                             std::ios_base::app);
  if (!_outSettingsFile.is_open()) {
    LOG_WARN("Failed to open settings file");
  }
}

bool Settings::write(const MailBox& mailBox) {
  return mailBox.writeObject(_outSettingsFile) && (_outSettingsFile << '\n');
}

bool Settings::read(MailBox& mailBox) {
  return mailBox.readObject(_outSettingsFile);
}

bool Settings::write(const std::vector<MailBox>& mailBoxes) {
  return std::all_of(mailBoxes.begin(), mailBoxes.end(),
                     [this](const MailBox& mailBox) { return write(mailBox); });
}

bool Settings::read(std::vector<MailBox>& mailBoxes) {
  for (MailBox mailBox;;) {
    if (!read(mailBox)) {
      if (_outSettingsFile.eof()) {
        break;
      } else {
        LOG_WARN("Error occurred when reading settings");
        return false;
      }
    }
    mailBoxes.emplace_back(std::move(mailBox));
  }

  return true;
}

}  // namespace lab4
