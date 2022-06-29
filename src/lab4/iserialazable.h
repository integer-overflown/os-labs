#ifndef OSLABS_ISERIALAZABLE_H
#define OSLABS_ISERIALAZABLE_H
#include <iostream>

namespace lab4 {
class ISerializable {
 public:
  virtual ~ISerializable() = default;
  [[nodiscard]] virtual bool writeObject(std::ostream &out) const = 0;
  [[nodiscard]] virtual bool readObject(std::istream &in) = 0;
};
}

#endif  // OSLABS_ISERIALAZABLE_H
