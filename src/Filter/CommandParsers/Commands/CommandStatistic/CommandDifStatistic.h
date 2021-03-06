#ifndef SCAFFOLDER_COMMANDDIFSTATISTIC_H
#define SCAFFOLDER_COMMANDDIFSTATISTIC_H

#include <Filter/CommandParsers/Commands/Command.h>

namespace filter {
    namespace commands {

        class CommandDifStatistic : public Command {
        public:
            void execute(std::string argv, State &state, ContigGraph &graph) override;
        };
    }
}

#endif //SCAFFOLDER_COMMANDDIFSTATISTIC_H
