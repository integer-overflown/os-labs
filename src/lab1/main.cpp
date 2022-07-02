#include <tchar.h>
#include <Windows.h>

#ifdef UNICODE
#include <fcntl.h>
#include <io.h>
#endif

#include <algorithm>
#include <iostream>

#define LOG_WARN(message)                                            \
  std::cerr << __FILE__ << ":" << __LINE__ << " at " << __FUNCTION__ \
            << " [WARNING] " << message << '\n';

namespace lab1 {

namespace {

#ifdef UNICODE
std::wostream &out = std::wcout;
#else
std::ostream &out = std::cout;
#endif

const char *gFamilyMembers[] = {"Хоменко Максим Олександрович",
                                "Хоменко Олександр Миколайович",
                                "Рогова Валерія Олегівна"};

std::wstring ConvertToUnicode(std::string_view string) {
  const auto sourceLength = static_cast<int>(string.size());

  if (sourceLength == 0) {
    return {};
  }

  int outLength = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                      string.data(), sourceLength, nullptr, 0);

  std::wstring buf;
  buf.resize(outLength);

  if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, string.data(),
                          sourceLength, buf.data(), outLength)) {
    return buf;
  }

  LOG_WARN("MultiByteToWideChar resulted with error");

  return {};
}

void SetUpConsoleEncoding() {
#ifdef UNICODE
  // enable UTF-16 mode (wide strings encoding on Windows) for standard output
  _setmode(_fileno(stdout), _O_U16TEXT);
#else
  SetConsoleCP(CP_UTF8);        // set input code page
  SetConsoleOutputCP(CP_UTF8);  // set output code page
#endif
}

void RunTask6() {
  // Task 6
#ifndef UNICODE
  std::for_each(gFamilyMembers, gFamilyMembers + std::size(gFamilyMembers),
                [](const char *value) { std::cout << value << '\n'; });
#else
  lab1::out << _T("-- Skipping task 6: can only be run in ANSI mode")
            << _T('\n');
#endif
}

void RunTask7() {
  std::wstring wideFamily[std::size(lab1::gFamilyMembers)];

  std::transform(lab1::gFamilyMembers,
                 lab1::gFamilyMembers + std::size(lab1::gFamilyMembers),
                 wideFamily, lab1::ConvertToUnicode);

#ifdef UNICODE
  std::for_each(wideFamily, wideFamily + std::size(wideFamily),
                [](std::wstring_view s) {
                  _tprintf(_T("_tprintf: %s\n"), s.data());
                  std::wcout << "std::wcout: " << s << '\n';
                });
#else
  lab1::out << _T("-- Skipping _tprintf, std::wcout tests: ")
            << _T("can only be run in UNICODE mode") << _T('\n');
#endif

  std::for_each(wideFamily, wideFamily + std::size(wideFamily),
                [](std::wstring_view s) {
                  MessageBoxW(nullptr, s.data(), L"Output", MB_OK);
                });
}

}  // namespace
}  // namespace lab1

int main() {
  lab1::SetUpConsoleEncoding();

  // Task 1
  _tprintf(_T("sizeof(TCHAR) == %zd, encoding mode is %s\n"), sizeof(TCHAR),
           sizeof(TCHAR) > 1 ? _T("Unicode") : _T("ANSI"));

  lab1::RunTask6();
  lab1::RunTask7();
}
