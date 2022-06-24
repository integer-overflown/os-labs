#ifndef OSLABS_RESOURCELIBRARY_H
#define OSLABS_RESOURCELIBRARY_H
#include <Windows.h>

#include <optional>
#include <string>

namespace lab3 {
class ResourceLibrary {
 public:
  static std::optional<ResourceLibrary> load(LPCTSTR libName);
  std::optional<std::wstring> GetString(UINT resId, UINT langId);

 private:
  explicit ResourceLibrary(HMODULE libHandle);
  HMODULE _libHandle;
};
}  // namespace lab3

#endif  // OSLABS_RESOURCELIBRARY_H
