#include <tchar.h>
#include <Windows.h>

#include <cerrno>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <variant>

#include "Utils/output.h"
#include "Utils/process.h"
#include "Utils/file.h"

namespace lab7 {

namespace {

enum class ParseError { ValueOutOfBounds, InvalidInteger };

#ifdef UNICODE
constexpr auto cFileReadMode = _T("rt, ccs=UNICODE");
#else
constexpr auto cFileReadMode = _T("rt");
#endif

constexpr size_t cBufferSize = 1024 * 8;  // 8Kb
constexpr auto cFilesDirectory = _T("files");


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

void DescribeFile(const WIN32_FIND_DATA &findData) {
  // large amount of data is better to allocate on heap,
  // not to exhaust stack memory
  std::unique_ptr<TCHAR[]> buffer(new TCHAR[cBufferSize]);
  FILE *inputStream;

  auto path = std::basic_string<TCHAR>(_T("")) + cFilesDirectory + _T('\\') +
              findData.cFileName;

  if (!(inputStream = _tfsopen(path.data(), cFileReadMode, _SH_DENYWR))) {
    std::cerr << "Failed to open file: " << findData.cFileName << '\n';
    std::terminate();
  }

  ULARGE_INTEGER fileSize;
  fileSize.LowPart = findData.nFileSizeLow;
  fileSize.HighPart = findData.nFileSizeHigh;

  lab7::out << "File" << ' ' << std::quoted(findData.cFileName, _T('\'')) << ' '
            << "size" << ' ' << fileSize.QuadPart << ' ' << "bytes" << '\n';

  uint64_t lineCount{};

  while (_fgetts(buffer.get(), cBufferSize, inputStream)) {
    ++lineCount;
    const size_t lineLength = _tcslen(buffer.get());
    lab7::out << "line" << ' ' << lineCount << ' ' << ':' << ' ' << lineLength
              << ' ' << "characters" << '\n';
  }

  lab7::out << "Total lines in file: " << lineCount << '\n';
}

size_t __forceinline tstrcmp(const TCHAR *a, const TCHAR *b) {
#ifdef UNICODE
  return wcscmp(a, b);
#else
  return strcmp(a, b);
#endif
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

  WIN32_FIND_DATA findData;
  HANDLE findHandle = FindFirstFile((std::basic_string<TCHAR>(_T("")) +
                                     lab7::cFilesDirectory + _T('\\') + _T("*"))
                                        .data(),
                                    &findData);

  if (!findHandle || findHandle == INVALID_HANDLE_VALUE) {
    lab7::HandleFatalWinApiError("FindFirstFile failed");
  }

  bool hasNext;
  do {
    if (lab7::tstrcmp(findData.cFileName, _T(".")) == 0 ||
        lab7::tstrcmp(findData.cFileName, _T("..")) == 0) {
      hasNext = FindNextFile(findHandle, &findData);
      continue;
    }

    // check if the file is older than editor launch time
    if (CompareFileTime(&findData.ftLastWriteTime, &birthTime) >= 0) {
      lab7::DescribeFile(findData);
      TCHAR fillChar = lab7::out.fill();
      lab7::out << std::setfill(_T('-')) << std::setw(24) << _T("")
                << std::setfill(fillChar) << '\n';
    } else {
      lab7::out << _T("File: ") << findData.cFileName << _T(" is too young: ")
                << lab7::FileTimeToUInt64(findData.ftLastWriteTime) << _T(" < ")
                << lab7::FileTimeToUInt64(birthTime) << _T('\n');
    }

    hasNext = FindNextFile(findHandle, &findData);
  } while (hasNext);
}
