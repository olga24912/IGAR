#ifndef SCAFFOLDER_COMMAND_H
#define SCAFFOLDER_COMMAND_H

#include <string>
#include "Filter/CommandParsers/State.h"
#include <sstream>

namespace filter {
    namespace commands {
        using namespace contig_graph;
        class Command {
        public:
            virtual void execute(std::string argv, State &state, ContigGraph &graph) = 0;

        protected:
            DECL_LOGGER("Command");
        };
    }
}

#endif //SCAFFOLDER_COMMAND_H
