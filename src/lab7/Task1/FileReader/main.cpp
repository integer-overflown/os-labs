#include <Windows.h>

#include <cerrno>
#include <iostream>
#include <string_view>
#include <variant>

namespace lab7 {
namespace {

enum class ParseError { ValueOutOfBounds, InvalidInteger };

constexpr void UInt64ToFileTime(uint64_t value, FILETIME &ft) {
  // this assumes the machine is little-endian,
  // which is true for most Windows versions out there
  ft.dwLowDateTime = value & UINT64_C(0xFFFF'FFFF);
  ft.dwHighDateTime = value >> 32;
}

constexpr uint64_t FileTimeToUInt64(const FILETIME &ft) {
  ULARGE_INTEGER intValue{};
  intValue.LowPart = ft.dwLowDateTime;
  intValue.HighPart = ft.dwHighDateTime;
  return intValue.QuadPart;
}

std::variant<uint64_t, ParseError> ParseUInt64(std::string_view string) {
  char *endPtr;
  errno = 0;
  uint64_t value = std::strtoull(string.data(), &endPtr, 10);

  if (errno == ERANGE) {
    return ParseError::ValueOutOfBounds;
  }

  if (*endPtr != '\0') {
    return ParseError::InvalidInteger;
  }

  return value;
}

}  // namespace
}  // namespace lab7

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << (argc > 0 ? argv[0] : "[exe file]") << ' '
              << "[file time]" << '\n';
    return EXIT_FAILURE;
  }

  std::variant<uint64_t, lab7::ParseError> fileTimeResult =
      lab7::ParseUInt64(argv[1]);

  if (std::holds_alternative<lab7::ParseError>(fileTimeResult)) {
    switch (std::get<lab7::ParseError>(fileTimeResult)) {
      case lab7::ParseError::ValueOutOfBounds:
        std::cerr << "The argument is too large or too small" << '\n';
        break;
      case lab7::ParseError::InvalidInteger:
      default:
        std::cerr << "The argument must be a valid integer" << '\n';
        break;
    }
    return EXIT_FAILURE;
  }

  FILETIME birthTime;
  lab7::UInt64ToFileTime(std::get<uint64_t>(fileTimeResult), birthTime);

  std::cout << lab7::FileTimeToUInt64(birthTime) << '\n';
}
