#include <fcntl.h>
#include <io.h>
#include <tchar.h>
#include <Windows.h>

#include <cassert>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "resourcelibrary.h"

enum class SupportedLanguage { English, Ukrainian };

namespace {

std::optional<SupportedLanguage> ParseLanguageName(std::string_view string) {
  if (string == "en") {
    return SupportedLanguage::English;
  }
  if (string == "uk") {
    return SupportedLanguage::Ukrainian;
  }
  return {};
}

void ListSupportedLanguages(std::ostream &out) {
  out << "Supported language names" << ':' << '\n'
      << "en - English" << '\n'
      << "uk - Ukrainian" << '\n';
}

void ReportLanguageInvalid(SupportedLanguage language) {
  throw std::invalid_argument("Invalid value for language: " +
                              std::to_string(int(language)));
}

DWORD SupportedLanguageToLangID(SupportedLanguage language) {
  switch (language) {
    case SupportedLanguage::English:
      return MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
    case SupportedLanguage::Ukrainian:
      return MAKELANGID(LANG_UKRAINIAN, SUBLANG_UKRAINIAN_UKRAINE);
    default:
      ReportLanguageInvalid(language);
  }
}

std::basic_string<TCHAR> GetResourceNameForLanguage(
    SupportedLanguage language) {
  switch (language) {
    case SupportedLanguage::English:
      return _T("lab3_resources_english.dll");
    case SupportedLanguage::Ukrainian:
      return _T("lab3_resources_ukrainian.dll");
    default:
      ReportLanguageInvalid(language);
  }
}

std::optional<lab3::ResourceLibrary> LoadResourcesForLanguage(
    SupportedLanguage language) {
  return lab3::ResourceLibrary::load(GetResourceNameForLanguage(language),
                                     SupportedLanguageToLangID(language));
}

}  // namespace

int main(int argc, char *argv[]) {
  // this line sets output encoding to UTF-16, which allows outputting
  // resource string contents correctly (they're encoded in UTF-16 as well)
  _setmode(_fileno(stdout), _O_WTEXT);

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << ' ' << "[language_name]\n";
    ListSupportedLanguages(std::cerr);
    return EXIT_FAILURE;
  }

  const char *argLanguage = argv[1];
  std::optional<SupportedLanguage> optLanguage = ParseLanguageName(argLanguage);

  if (!optLanguage) {
    std::cerr << "Language is not supported: " << std::quoted(argLanguage, '\'')
              << '\n';
    ListSupportedLanguages(std::cerr);
    return EXIT_FAILURE;
  }

  std::optional<lab3::ResourceLibrary> optResource =
      LoadResourcesForLanguage(*optLanguage);

  if (!optResource) {
    std::cerr << "Failed to load resources for language: " << argLanguage;
    return EXIT_FAILURE;
  }

  std::wcout << std::setw(4) << "ID" << '|' << std::setw(10) << "Name" << '\n';
  std::wcout << std::setw(18) << std::setfill(L'-') << "" << '\n'
             << std::setfill(L' ');

  for (const int id : {101, 102, 103, 104}) {
    std::optional<std::wstring> string = optResource->GetString(id);
    if (!string) {
      std::cerr << "Cannot find string with id " << id << '\n';
      continue;
    }

    std::wcout << std::setw(4) << id << L'|' << std::setw(10) << *string
               << L'\n';

    // assure we haven't gone to error state,
    // and are good to go for next iteration
    assert(!!std::wcout);
  }

  std::wcout << std::endl;
}
