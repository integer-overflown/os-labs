#ifndef OSLABS_MAILBOX_H
#define OSLABS_MAILBOX_H
#include <iostream>
#include <string>

#include "iserialazable.h"

namespace lab4 {

class MailBox : public ISerializable {
 public:
  MailBox() = default;
  MailBox(std::string name, size_t maxSize);
  [[nodiscard]] bool writeObject(std::ostream& out) const override;
  [[nodiscard]] bool readObject(std::istream& in) override;
  [[nodiscard]] const std::string &getName() const;
  [[nodiscard]] size_t getMaxSize() const;

 private:
  std::string _name;
  size_t _maxSize{};
};

}  // namespace lab4

#endif  // OSLABS_MAILBOX_H
