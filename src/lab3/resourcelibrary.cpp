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

ResourceLibrary::ResourceLibrary(HMODULE libHandle) : _libHandle(libHandle) {}

std::optional<std::wstring> ResourceLibrary::GetString(UINT resId,
                                                       UINT langId) {
  LPCWSTR stringResStartPtr = FindStringResourceEx(_libHandle, resId, langId);

  if (!stringResStartPtr) {
    return {};
  }

  std::wstring result(stringResStartPtr + 1, size_t(*stringResStartPtr));
  return result;
}

std::optional<ResourceLibrary> ResourceLibrary::load(LPCTSTR libName) {
  HMODULE dllHandle = LoadLibraryEx(libName, nullptr, LOAD_LIBRARY_AS_DATAFILE);
  return dllHandle ? std::make_optional(ResourceLibrary{dllHandle})
                   : std::nullopt;
}
}  // namespace lab3
