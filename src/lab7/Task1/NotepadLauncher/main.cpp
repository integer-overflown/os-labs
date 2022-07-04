#include <tchar.h>
#include <Windows.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#define CreateProcessSimple(launchLine, startUpInfo, processInfo)           \
  CreateProcess(nullptr, (launchLine), nullptr, nullptr, false, 0, nullptr, \
                nullptr, (startUpInfo), (processInfo))

namespace lab7 {
inline namespace literals {
std::basic_string<TCHAR> operator""_ts(const TCHAR *string, size_t size) {
  return {string, size};
}
}  // namespace literals

#ifdef UNICODE
constexpr auto cNotepadOpenMode = _T("/W");
#else
constexpr auto cNotepadOpenMode = _T("/A");
#endif

constexpr auto cFileDirName = _T("files");

[[noreturn]] void HandleFatalWinApiError(const char *reason) {
  std::cerr << reason << ", error code " << GetLastError() << '\n';
  std::terminate();
}

constexpr uint64_t FileTimeToUInt64(const FILETIME &ft) {
  ULARGE_INTEGER intValue{};
  intValue.LowPart = ft.dwLowDateTime;
  intValue.HighPart = ft.dwHighDateTime;
  return intValue.QuadPart;
}

}  // namespace lab7

int main() {
  using namespace lab7;
  PROCESS_INFORMATION processInfo;
  STARTUPINFO processStartUpInfo;
  SYSTEMTIME launchTime;
  FILETIME convertedTime;

  constexpr const TCHAR *cFileNames[] = {_T("first.txt"), _T("second.txt")};

  SecureZeroMemory(&processInfo, sizeof(processInfo));
  SecureZeroMemory(&processStartUpInfo, sizeof(processStartUpInfo));

  processStartUpInfo.cb = sizeof(processStartUpInfo);
  processStartUpInfo.dwFlags |= STARTF_USESTDHANDLES;

  GetLocalTime(&launchTime);
  std::cout << "Launch time (system): " << std::setfill('0') << launchTime.wYear
            << '-' << std::setw(2) << launchTime.wMonth << '-' << std::setw(2)
            << launchTime.wDay << ' ' << std::setw(2) << launchTime.wHour << ':'
            << std::setw(2) << launchTime.wMinute << ':' << std::setw(2)
            << launchTime.wSecond << '\n';

  if (!SystemTimeToFileTime(&launchTime, &convertedTime)) {
    HandleFatalWinApiError("SystemTimeToFileTime failed");
  }

  std::cout << "Launch time (filetime): " << lab7::FileTimeToUInt64(convertedTime) << '\n';

  if (!CreateDirectory(lab7::cFileDirName, nullptr)) {
    // we expect to have ERROR_ALREADY_EXISTS error
    if (GetLastError() != ERROR_ALREADY_EXISTS) {
      HandleFatalWinApiError("CreateDirectory failed");
    }
  }

  for (const TCHAR *fileName : cFileNames) {
    auto launchLine = "notepad.exe"_ts + _T(' ') + lab7::cNotepadOpenMode +
                      _T(' ') + lab7::cFileDirName + '\\' + fileName;

    std::cout << "Launching process: " << launchLine << '\n';

    if (!CreateProcessSimple(launchLine.data(), &processStartUpInfo,
                             &processInfo)) {
      HandleFatalWinApiError("CreateProcess failed");
    }

    if (WaitForSingleObject(processInfo.hProcess, INFINITE) == WAIT_FAILED) {
      HandleFatalWinApiError("WaitForSingleObject failed");
    }
  }

  return EXIT_SUCCESS;
}
