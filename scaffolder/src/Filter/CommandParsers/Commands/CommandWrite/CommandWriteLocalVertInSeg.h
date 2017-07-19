#ifndef SCAFFOLDER_COMMANDWRITELOCALVERTINSEG_H
#define SCAFFOLDER_COMMANDWRITELOCALVERTINSEG_H

#include "CommandWrite.h"
#include <Filter/Writers/WriteLocal.h>

namespace filter {
    namespace commands {

        class CommandWriteLocalVertInSeg : public CommandWrite {
        public:
            void writeGraph(std::string argv, State &state, ContigGraph &graph) override;
        };
    }
}

#endif //SCAFFOLDER_COMMANDWRITELOCALVERTINSEG_H
