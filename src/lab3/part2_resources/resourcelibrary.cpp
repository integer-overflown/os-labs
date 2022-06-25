#include "resourcelibrary.h"

#include <Windows.h>

namespace lab3 {

namespace {
LPCWSTR FindStringResourceEx(HINSTANCE moduleHandle, UINT resId, UINT langId) {
  LPCWSTR resourceData = nullptr;
  HRSRC hrsrc = FindResourceEx(moduleHandle, RT_STRING,
                               MAKEINTRESOURCE(resId / 16 + 1), langId);
  if (hrsrc) {
    HGLOBAL resourceHandle = LoadResource(moduleHandle, hrsrc);
    if (resourceHandle) {
      resourceData = reinterpret_cast<LPCWSTR>(LockResource(resourceHandle));
      if (resourceData) {
        // okay now walk the string table
        for (int i = 0; i < (resId & 15); i++) {
          resourceData += 1 + (UINT)*resourceData;
        }
        UnlockResource(resourceData);
      }
      FreeResource(resourceHandle);
    }
  }
  return resourceData;
}
}  // namespace

ResourceLibrary::ResourceLibrary(HMODULE libHandle, UINT langId)
    : _libHandle(libHandle), _langId(langId) {}

std::optional<std::wstring> ResourceLibrary::GetString(UINT resId) {
  LPCWSTR stringResStartPtr = FindStringResourceEx(_libHandle, resId, _langId);

  if (!stringResStartPtr) {
    return {};
  }

  std::wstring result(stringResStartPtr + 1, size_t(*stringResStartPtr));
  return result;
}

std::optional<ResourceLibrary> ResourceLibrary::load(
    std::basic_string_view<TCHAR> libName, UINT langId) {
  HMODULE dllHandle =
      LoadLibraryEx(libName.data(), nullptr, LOAD_LIBRARY_AS_DATAFILE);
  return dllHandle ? std::make_optional(ResourceLibrary{dllHandle, langId})
                   : std::nullopt;
}
}  // namespace lab3
