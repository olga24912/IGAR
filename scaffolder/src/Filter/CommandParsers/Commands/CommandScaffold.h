#ifndef SCAFFOLDER_COMMANDMERGESIMPLEPATH_H
#define SCAFFOLDER_COMMANDMERGESIMPLEPATH_H

#include <Filter/CommandParsers/State.h>
#include "Filter/CommandParsers/Commands/Command.h"

namespace filter {
    namespace commands {
        class CommandScaffold : public Command {
        public:
            void execute(std::string argv, State &state, ContigGraph &graph) override;
        };
    }
}
#endif //SCAFFOLDER_COMMANDMERGESIMPLEPATH_H
