#ifndef OSLABS_BUILTINREGISTRY_H
#define OSLABS_BUILTINREGISTRY_H
#include <memory>

#include "cli/commandregistry.h"

namespace lab4 {

std::unique_ptr<cli::ICommandRegistry> builtInCommandRegistry();

}

#endif  // OSLABS_BUILTINREGISTRY_H
