#include <tchar.h>
#include <Windows.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "Utils/output.h"
#include "Utils/process.h"
#include "Utils/file.h"

namespace lab7 {
namespace {

template <typename T>
std::basic_string<TCHAR> __forceinline to_string(T value) {
#ifdef UNICODE
  return std::to_wstring(value);
#else
  return std::to_string(value);
#endif
}

}  // namespace
}  // namespace lab7

int _tmain(int argc, TCHAR *argv[]) {
  if (argc != 3) {
    lab7::err << _T("Usage: ") << (argc > 0 ? argv[0] : _T("[exe name]"))
              << _T(' ') << _T("[notepad launch program]") << _T(' ')
              << _T("[file reader program]") << _T('\n');
    return EXIT_FAILURE;
  }

  STARTUPINFO startUpInfo;
  PROCESS_INFORMATION processInfo;
  SYSTEMTIME launchTime;
  FILETIME convertedTime;

  SecureZeroMemory(&startUpInfo, sizeof(startUpInfo));
  SecureZeroMemory(&processInfo, sizeof(processInfo));

  startUpInfo.cb = sizeof(startUpInfo);
  startUpInfo.dwFlags |= STARTF_USESTDHANDLES;
  startUpInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  startUpInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

  GetSystemTime(&launchTime);
  lab7::out << _T("Launch time (system): ") << std::setfill(_T('0'))
            << launchTime.wYear << _T('-') << std::setw(2) << launchTime.wMonth
            << _T('-') << std::setw(2) << launchTime.wDay << _T(' ')
            << std::setw(2) << launchTime.wHour << _T(':') << std::setw(2)
            << launchTime.wMinute << _T(':') << std::setw(2)
            << launchTime.wSecond << _T('\n');

  if (!SystemTimeToFileTime(&launchTime, &convertedTime)) {
    lab7::HandleFatalWinApiError("SystemTimeToFileTime failed");
  }

  uint64_t launchFileTime = lab7::FileTimeToUInt64(convertedTime);

  lab7::out << _T("Launch time (filetime): ") << launchFileTime << _T('\n');

  lab7::out << _T("Launching: ") << argv[1] << _T('\n');

  if (!CreateProcess(nullptr, argv[1], nullptr, nullptr, true, 0, nullptr,
                     nullptr, &startUpInfo, &processInfo)) {
    lab7::HandleFatalWinApiError("CreateProcess for notepad launched failed");
  }

  if (WaitForSingleObject(processInfo.hProcess, INFINITE) == WAIT_FAILED) {
    lab7::HandleFatalWinApiError(
        "WaitForSingleObject on notepad launched failed");
  }

  auto launchLine = std::basic_string<TCHAR>(argv[2]) + _T(' ') +
                    lab7::to_string(launchFileTime);

  lab7::out << "Launching: " << launchLine << '\n';

  if (!CreateProcess(nullptr, launchLine.data(), nullptr, nullptr, true, 0,
                     nullptr, nullptr, &startUpInfo, &processInfo)) {
    lab7::HandleFatalWinApiError("CreateProcess for file reader failed");
  }

  if (WaitForSingleObject(processInfo.hProcess, INFINITE) == WAIT_FAILED) {
    lab7::HandleFatalWinApiError("WaitForSingleObject on file reader failed");
  }

  return EXIT_SUCCESS;
}
