#include "builtincommands.h"

#include <Windows.h>

CreateCommand::CreateCommand() {
  setCommandDescription("create various entities, like emails");
  addPositionalArgument("subcommand",
                        "subcommand to invoke, valid values are: email");
  addPositionalArgument("filename", "email file to create");
}

std::string CreateCommand::name() const { return cCommandName; }

bool CreateCommand::acceptInput(const std::vector<std::string_view> &tokens) {
  if (tokens.empty()) {
    setErrorString("no subcommand given");
    return false;
  }

  if (tokens[0] == "email") {
    return createEmailFile(tokens[1]);
  } else {
    setErrorString("unknown subcommand: " + std::string(tokens[0]));
    return false;
  }
}

bool CreateCommand::createEmailFile(std::string_view fileName) {
  if (fileName.length() > MAX_PATH) {
    setErrorString("filename is too long");
    return false;
  }

  HANDLE fileHandle = CreateFileA(
      fileName.data(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_NEW,
      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_POSIX_SEMANTICS, nullptr);

  if (fileHandle == INVALID_HANDLE_VALUE) {
    DWORD errorCode = GetLastError();
    if (errorCode == ERROR_FILE_EXISTS) {
      setErrorString("file already exists: " + std::string(fileName));
    } else {
      setErrorString("unknown error, code: " + std::to_string(errorCode));
    }
    return false;
  }

  CloseHandle(fileHandle);
  return true;
}

DeleteCommand::DeleteCommand() {
  setCommandDescription("delete particular email, or all of them");
  addPositionalArgument("subcommand",
                        "subcommand to invoke, valid values are: email, all");
}

std::string DeleteCommand::name() const { return cCommandName; }

bool DeleteCommand::acceptInput(const std::vector<std::string_view> &tokens) {
  if (tokens[0] == "email") {
    if (tokens.size() != 2) {
      setErrorString("missing filename");
      return false;
    }

    std::string_view filename = tokens[1];
    // check if file is a path
    auto it = std::find_if(filename.begin(), filename.end(),
                           [](char c) { return c == '/' || c == '\\'; });

    if (it != filename.end()) {
      setErrorString("filename cannot be a path");
      return false;
    }

    return deleteEmailFile(filename);
  } else {
    setErrorString("unknown subcommand: " + std::string(tokens[0]));
    return false;
  }
}

bool DeleteCommand::deleteEmailFile(std::string_view fileName) {
  if (fileName.length() > MAX_PATH) {
    setErrorString("filename is too long");
    return false;
  }

  if (!DeleteFileA(fileName.data())) {
    if (auto error = GetLastError(); error == ERROR_FILE_NOT_FOUND) {
      setErrorString("file not found");
    } else if (error == ERROR_ACCESS_DENIED) {
      setErrorString("access denied");
    } else {
      setErrorString("unexpected error, code " + std::to_string(error));
    }
    return false;
  }

  return true;
}
std::pair<std::size_t, std::size_t> DeleteCommand::positionalArgumentCount()
    const noexcept {
  size_t argCount = AbstractCommand::positionalArgumentCount().first;
  return {argCount, argCount + 1};
}
