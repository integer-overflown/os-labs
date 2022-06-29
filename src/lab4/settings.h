#ifndef OSLABS_SETTINGS_H
#define OSLABS_SETTINGS_H
#include <vector>
#include <fstream>

#include "mailbox.h"

namespace lab4 {

inline constexpr auto cDefaultLocation = "settings.txt";

class Settings {
 public:
  explicit Settings(std::string_view location = cDefaultLocation);
  [[nodiscard]] bool write(const MailBox &mailBox);
  [[nodiscard]] bool read(MailBox &mailBox);
  [[nodiscard]] bool write(const std::vector<MailBox> &mailBoxes);
  [[nodiscard]] bool read(std::vector<MailBox> &mailBoxes);

 private:
  std::fstream _outSettingsFile;
};
}

#endif  // OSLABS_SETTINGS_H
