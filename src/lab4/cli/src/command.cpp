#include "cli/command.h"

#include <algorithm>

namespace cli {

void AbstractCommand::addPositionalArgument(std::string name,
                                            std::string help) {
  _positionalArguments.emplace_back(std::move(name), std::move(help));
}

size_t AbstractCommand::positionalArgumentCount() const noexcept {
  return _positionalArguments.size();
}

void AbstractCommand::setCommandDescription(std::string commandDescription) {
  _commandDescription = std::move(commandDescription);
}

void AbstractCommand::printHelp(std::ostream& out) const {
  out << name();

  if (!_positionalArguments.empty()) {
    out << ' ';
    for (size_t i = 0;; ++i) {
      out << '[' << _positionalArguments[i].first << ']';
      if (i == _positionalArguments.size() - 1) {
        out << '\n';
        break;
      }
      out << ' ';
    }
  }

  if (!_commandDescription.empty()) {
    out << '\n' << "description" << ':' << ' ' << _commandDescription << '\n';
  }

  if (_positionalArguments.empty()) {
    return;
  }

  out << "arguments" << ':' << '\n';

  std::for_each(_positionalArguments.begin(), _positionalArguments.end(),
                [&out](const auto& argInfo) {
                  out << argInfo.first;
                  if (!argInfo.second.empty()) {
                    out << ' ' << '-' << ' ' << argInfo.second;
                  }
                  out << '\n';
                });
}

}  // namespace cli
