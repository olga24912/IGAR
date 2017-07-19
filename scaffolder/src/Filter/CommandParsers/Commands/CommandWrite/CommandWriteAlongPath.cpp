#include "CommandWriteAlongPath.h"

namespace filter {
    namespace commands {
        // writeAlongPath <prefixFileName> <libNum> <dist> <minRefPathSize>
        void CommandWriteAlongPath::writeGraph(std::string argv, State &state, ContigGraph &graph) {
            std::stringstream ss(argv);
            std::string fileName;
            ss >> fileName;
            int libNum;
            int dist;
            int minSize;
            ss >> libNum >> dist >> minSize;

            INFO("write along path fileName=" << fileName << " libNum=" << libNum << " dist=" << dist << " minSize="
                                              << minSize);

            writers::WriteAlongPath writer(fileName, libNum, dist, minSize, &graph, state.validator, state.dotWriterBuilder);

            writer.write();
        }
    }
}