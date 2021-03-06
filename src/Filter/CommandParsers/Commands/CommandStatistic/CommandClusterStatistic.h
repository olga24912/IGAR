#ifndef SCAFFOLDER_COMMANDCLUSTERSTATISTIC_H
#define SCAFFOLDER_COMMANDCLUSTERSTATISTIC_H


#include <Filter/CommandParsers/Commands/Command.h>

namespace filter {
    namespace commands {
        class CommandClusterStatistic : public Command {
        public:
            void execute(std::string argv, State &state, ContigGraph &graph) override;
        };
    }
}


#endif //SCAFFOLDER_COMMANDCLUSTERSTATISTIC_H
