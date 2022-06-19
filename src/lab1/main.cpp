#include <Windows.h>
#include <tchar.h>

#include <iostream>

#ifdef _UNICODE
#define CHARACTER_SET_NAME "Unicode"
#else
#define CHARACTER_SET_NAME "ASCII"
#endif

#define LOG_WARN(message)                                                   \
  std::cerr << __FILE__ << ":" << __LINE__ << " at " << __FUNCTION__ << " [WARNING] " \
            << message << '\n';

namespace lab1 {

namespace {

std::string ConvertToMultibyte(std::wstring_view string) {
  const auto sourceLength = static_cast<int>(string.size());
  int ansiLen = WideCharToMultiByte(CP_UTF8, 0, string.data(), sourceLength,
                                    nullptr, 0, nullptr, nullptr);

  std::string buf;
  buf.resize(ansiLen);

  if (WideCharToMultiByte(CP_UTF8, 0, string.data(), sourceLength, buf.data(),
                          ansiLen, nullptr, nullptr)) {
    return buf;
  }

  LOG_WARN("WideCharToMultiByte resulted with error");

  return {};
}

std::wstring ConvertToUnicode(std::string_view string) {
  const auto sourceLength = static_cast<int>(string.size());

  if (sourceLength == 0) {
    return {};
  }

  int outLength =
      MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, string.data(), sourceLength, nullptr, 0);

  std::wstring buf;
  buf.resize(outLength);

  if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, string.data(), sourceLength, buf.data(),
                          outLength)) {
    return buf;
  }

  LOG_WARN("MultiByteToWideChar resulted with error");

  return {};
}

}  // namespace
}  // namespace lab1

int main() {
  // 1) Determine encoding mode
  std::cout << "sizeof(TCHAR) == " << sizeof(TCHAR) << '\n'
            << "Default charset is " << CHARACTER_SET_NAME << '\n';
  SetConsoleOutputCP(CP_UTF8);

  auto fullName = _T("Максим Хоменко");

  const std::string res = lab1::ConvertToMultibyte(fullName);
  std::cout << "1) " << res.data() << std::endl;

  const std::wstring wRes = lab1::ConvertToUnicode("Максим Хоменко");
  std::wcout << L"2) " << wRes.data() << std::endl;
  wprintf(L"2) %s\n", wRes.data());

  MessageBox(nullptr, wRes.data(), _T("Caption"), MB_OK);

  return 0;
}
