#ifndef OSLABS_FILE_H
#define OSLABS_FILE_H
#include <Windows.h>

#include <cstdint>

namespace lab7 {

constexpr uint64_t FileTimeToUInt64(const FILETIME &ft) {
  ULARGE_INTEGER intValue{};
  intValue.LowPart = ft.dwLowDateTime;
  intValue.HighPart = ft.dwHighDateTime;
  return intValue.QuadPart;
}

constexpr void UInt64ToFileTime(uint64_t value, FILETIME &ft) {
  // this assumes the machine is little-endian,
  // which is true for most Windows versions out there
  ft.dwLowDateTime = value & UINT64_C(0xFFFF'FFFF);
  ft.dwHighDateTime = value >> 32;
}

}

#endif  // OSLABS_FILE_H
