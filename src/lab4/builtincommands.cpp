#include "builtincommands.h"

#include <Windows.h>

#include <cassert>
#include <iomanip>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <string>

#include "log.h"
#include "mailbox.h"

namespace lab4 {
namespace {

size_t parseInt(std::string_view s) {
  char *endPtr;
  errno = 0;
  auto value = std::strtoll(s.data(), &endPtr, 10);

  if (!(errno == 0 && *endPtr == '\0')) {
    return {};
  }

  return value;
}

bool EnterEditingMode(HANDLE fileHandle) {
  std::cout << "Type '.done' to finish editing the file" << '\n';
  std::cout << " -- You're now in editing mode -- " << '\n';

  std::regex quitRegex(R"(^\s*\.done)", std::regex_constants::ECMAScript |
                                            std::regex_constants::icase);
  std::stringstream buffer;
  std::string line;

  while (std::getline(std::cin, line)) {
    if (std::regex_search(line, quitRegex)) {
      std::string contents = buffer.str();
      DWORD numBytesWritten;
      BOOL status =
          WriteFile(fileHandle, static_cast<const void *>(contents.data()),
                    contents.size(), &numBytesWritten, nullptr);
      std::cout << numBytesWritten << ' ' << "bytes written" << '\n';
      return status;
    }

    buffer << line << '\n';
  }

  return true;
}

std::string GetMailboxFolderName(std::string_view mailBoxName) {
  using namespace std::string_literals;
  return "mailbox"s + '-' + std::string(mailBoxName) + '-' + "contents";
}

std::optional<size_t> FolderFileCount(const std::string &folderName) {
  std::string wildcard = "\\*";

  if (folderName.size() > MAX_PATH - wildcard.size()) {
    LOG_WARN("Folder name is too long");
    return {};
  }

  WIN32_FIND_DATAA fileData;
  HANDLE findHandle = FindFirstFileA((folderName + wildcard).data(), &fileData);

  if (findHandle == INVALID_HANDLE_VALUE) {
    LOG_WARN("Failed to list directory contents: ") << GetLastError();
    return {};
  }

  size_t count{};

  for (;;) {
    BOOL status = FindNextFileA(findHandle, &fileData);
    if (!status) {
      if (GetLastError() == ERROR_NO_MORE_FILES) {
        break;
      } else {
        return {};
      }
    }
    // skip parent dir reference
    if (std::strcmp(fileData.cFileName, "..") != 0) {
      ++count;
    }
  }

  return count;
}

}  // namespace
}  // namespace lab4

CreateCommand::CreateCommand(
    std::shared_ptr<lab4::IConfiguration> configuration)
    : _configuration(std::move(configuration)) {
  setCommandDescription("create various entities, like emails");
  addPositionalArgument(
      "subcommand", "subcommand to invoke, valid values are: email, mailbox");
  addPositionalArgument("name", "name of entity, whether a file or a mailbox");
}

std::string CreateCommand::name() const { return cCommandName; }

bool CreateCommand::acceptInput(const std::vector<std::string_view> &tokens) {
  if (tokens.empty()) {
    setErrorString("no subcommand given");
    return false;
  }

  if (tokens[0] == "email") {
    if (tokens.size() != 3) {
      setErrorString(
          "email mailbox is a required parameter, "
          "pass it as the third value");
      return false;
    }

    return createEmailFile(tokens[1], tokens[2]);
  } else if (tokens[0] == "mailbox") {
    if (tokens.size() != 3) {
      setErrorString(
          "max size of mailbox is a required parameter,"
          " pass it as the third value");
      return false;
    }

    auto optSize = lab4::parseInt(tokens[2]);

    if (!optSize) {
      setErrorString("max size must be a valid non-negative int");
      return false;
    }

    return createMailBox(tokens[1], optSize);
  } else {
    setErrorString("unknown subcommand: " + std::string(tokens[0]));
    return false;
  }
}

bool CreateCommand::createEmailFile(std::string_view fileName,
                                    std::string_view mailBoxName) {
  using namespace std::string_literals;
  auto optMailBoxes = _configuration->availableMailBoxes();

  if (!optMailBoxes) {
    setErrorString("failed to read configuration");
    return false;
  }

  lab4::MailBox mailBox;

  if (auto it = std::find_if(optMailBoxes->begin(), optMailBoxes->end(),
                             [mailBoxName](const lab4::MailBox &mb) {
                               return mb.getName() == mailBoxName;
                             });
      it == optMailBoxes->end()) {
    setErrorString("mailbox"s + ' ' + '\'' + std::string(mailBoxName) + '\'' +
                   ' ' + "doesn't exist"s);
    return false;
  } else {
    mailBox = *it;
  }

  std::string folderName = lab4::GetMailboxFolderName(mailBoxName);

  if (fileName.length() > MAX_PATH - folderName.size()) {
    setErrorString("filename is too long");
    return false;
  }

  if (!CreateDirectoryA(folderName.c_str(), nullptr)) {
    if (GetLastError() != ERROR_ALREADY_EXISTS) {
      setErrorString(
          "unexpected error occurred when created mailbox directory, error "
          "code " +
          std::to_string(GetLastError()));
      return false;
    }
  }

  if (auto optFileCount = lab4::FolderFileCount(folderName); !optFileCount) {
    setErrorString("failed to access directory " + folderName);
    return false;
  } else if (*optFileCount == mailBox.getMaxSize()) {
    setErrorString("maximum size for mailbox has been reached");
    return false;
  }

  HANDLE fileHandle =
      CreateFileA((folderName + '\\' + std::string(fileName)).data(),
                  GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_NEW,
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

  bool success = lab4::EnterEditingMode(fileHandle);

  if (!success) {
    setErrorString("failed to write file, code: " +
                   std::to_string(GetLastError()));
  }

  CloseHandle(fileHandle);
  return success;
}

bool CreateCommand::createMailBox(std::string_view name, size_t size) {
  using namespace std::string_literals;
  lab4::MailBox mailBox(std::string(name), size);

  auto optAvailable = _configuration->availableMailBoxes();

  if (!optAvailable) {
    setErrorString("failed to read configuration");
    return false;
  }

  auto existingIt = std::find_if(
      optAvailable->begin(), optAvailable->end(),
      [name](const lab4::MailBox &mb) { return mb.getName() == name; });

  if (existingIt != optAvailable->end()) {
    setErrorString("mailbox"s + ' ' + std::string(name) + ' ' +
                   "already exists"s);
    return false;
  }

  bool success = _configuration->addMailBox(mailBox);

  if (!success) {
    setErrorString("failed to save mail box");
    return false;
  }

  return success;
}

std::pair<std::size_t, std::size_t> CreateCommand::positionalArgumentCount()
    const noexcept {
  auto sizeRange = AbstractCommand::positionalArgumentCount();
  return {sizeRange.first, sizeRange.first + 1};
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

ListCommand::ListCommand(std::shared_ptr<lab4::IConfiguration> configuration)
    : _configuration(std::move(configuration)) {
  setCommandDescription("List various available instances, like mailboxes");
  addPositionalArgument("instanceType",
                        "instances to list, supported values: mailbox");
}

std::pair<std::size_t, std::size_t> DeleteCommand::positionalArgumentCount()
    const noexcept {
  size_t argCount = AbstractCommand::positionalArgumentCount().first;
  return {argCount, argCount + 1};
}

std::string ListCommand::name() const { return cCommandName; }

bool ListCommand::acceptInput(const std::vector<std::string_view> &tokens) {
  if (tokens[0] == "mailbox") {
    return listMailBoxes();
  } else {
    setErrorString("unknown subcommand: " + std::string(tokens[0]));
    return false;
  }
}

bool ListCommand::listMailBoxes() {
  auto optMailBoxes = _configuration->availableMailBoxes();

  if (!optMailBoxes) {
    setErrorString("failed to read configuration for available mailboxes");
    return false;
  }

  std::for_each(optMailBoxes->begin(), optMailBoxes->end(),
                [num = size_t(1)](const lab4::MailBox &mb) mutable {
                  std::cout << num << ')' << ' '
                            << std::quoted(mb.getName(), '\'') << ',' << ' '
                            << "max size" << ' ' << mb.getMaxSize() << '\n';
                  ++num;
                });

  return true;
}
