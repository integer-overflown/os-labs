#include "mailbox.h"

#include "log.h"

namespace lab4 {

MailBox::MailBox(std::string name, size_t maxSize)
    : _name(std::move(name)), _maxSize(maxSize) {}

bool MailBox::writeObject(std::ostream& out) const {
  if (!out.good()) {
    LOG_WARN("Stream has failed state");
    return false;
  }

  return bool(out << _name << ' ' << _maxSize);
}

bool MailBox::readObject(std::istream& in) {
  return (in >> _name) && (in >> _maxSize);
}

const std::string& MailBox::getName() const { return _name; }

size_t MailBox::getMaxSize() const { return _maxSize; }

}  // namespace lab4
