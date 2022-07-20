#ifndef OSLABS_PROCESS_H
#define OSLABS_PROCESS_H
#include <Windows.h>

#include <exception>

#include "output.h"

#define CreateProcessSimple(launchLine, startUpInfo, processInfo)           \
  CreateProcess(nullptr, (launchLine), nullptr, nullptr, false, 0, nullptr, \
                nullptr, (startUpInfo), (processInfo))

namespace lab7 {
[[noreturn]] inline void HandleFatalWinApiError(const char *reason) {
  lab7::err << reason << ", error code " << GetLastError() << '\n';
  std::terminate();
}
}

#endif  // OSLABS_PROCESS_H
