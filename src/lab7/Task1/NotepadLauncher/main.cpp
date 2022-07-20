#include <tchar.h>
#include <Windows.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "Utils/output.h"
#include "Utils/process.h"

namespace lab7 {

#ifdef UNICODE
constexpr auto cNotepadOpenMode = _T("/W");
#else
constexpr auto cNotepadOpenMode = _T("/A");
#endif

constexpr auto cFileDirName = _T("files");

}  // namespace lab7

int main() {
  using namespace lab7;
  PROCESS_INFORMATION processInfo;
  STARTUPINFO processStartUpInfo;

  constexpr const TCHAR *cFileNames[] = {_T("first.txt"), _T("second.txt")};

  SecureZeroMemory(&processInfo, sizeof(processInfo));
  SecureZeroMemory(&processStartUpInfo, sizeof(processStartUpInfo));

  processStartUpInfo.cb = sizeof(processStartUpInfo);

  if (!CreateDirectory(lab7::cFileDirName, nullptr)) {
    // we expect to have ERROR_ALREADY_EXISTS error
    if (GetLastError() != ERROR_ALREADY_EXISTS) {
      HandleFatalWinApiError("CreateDirectory failed");
    }
  }

  for (const TCHAR *fileName : cFileNames) {
    auto launchLine = std::basic_string<TCHAR>(_T("notepad.exe")) + _T(' ') +
                      lab7::cNotepadOpenMode + _T(' ') + lab7::cFileDirName +
                      _T('\\') + fileName;

    lab7::out << "Launching process: " << launchLine << '\n';

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
