#ifndef OSLABS_RESOURCELIBRARY_H
#define OSLABS_RESOURCELIBRARY_H
#include <Windows.h>

#include <optional>
#include <string>
#include <string_view>

namespace lab3 {
class ResourceLibrary {
 public:
  static std::optional<ResourceLibrary> load(std::basic_string_view<TCHAR> libName, UINT langId);
  std::optional<std::wstring> GetString(UINT resId);
  ~ResourceLibrary();

 private:
  explicit ResourceLibrary(HMODULE libHandle, UINT langId);
  HMODULE _libHandle;
  UINT _langId;
};
}  // namespace lab3

#endif  // OSLABS_RESOURCELIBRARY_H
