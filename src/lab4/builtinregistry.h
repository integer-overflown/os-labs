#ifndef OSLABS_BUILTINREGISTRY_H
#define OSLABS_BUILTINREGISTRY_H
#include <memory>

#include "cli/commandregistry.h"
#include "configuration.h"

namespace lab4 {

std::unique_ptr<cli::ICommandRegistry> builtInCommandRegistry(std::shared_ptr<lab4::IConfiguration> configuration);

}

#endif  // OSLABS_BUILTINREGISTRY_H
