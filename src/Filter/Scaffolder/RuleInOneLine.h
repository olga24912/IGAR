#ifndef SCAFFOLDER_RULEINONELINE_H
#define SCAFFOLDER_RULEINONELINE_H

#include <ContigGraph/ContigGraph.h>
#include "Rule.h"

namespace filter {
    namespace scaffolder {
        using namespace contig_graph;
        class RuleInOneLine : Rule {
        public:
            void simplifyGraph(ContigGraph *graph) override;

        private:
            const int DIST = 5;

            void projectEdge(ContigGraph *graph, int e);

            int getPaths(ContigGraph *graph, int u, int w, std::vector<int> &path, ContigGraph::Lib::Type type);

            std::set<int> getArea(ContigGraph *graph, int v);

            std::vector<int> topSortF(ContigGraph *graph, std::set<int> uarea);

            void topSortDfs(int v, ContigGraph *graph, std::set<int> &area, std::set<int> &used, std::vector<int> &topSort);

            std::set<int> markCycle(std::vector<int> &topSort, ContigGraph *graph, std::set<int> area);

            void
    dfsMarkCycle(int &v, ContigGraph *graph, std::set<int> area, std::set<int>& used, std::set<int>& cycle, bool inCycle);

            std::vector<int> reduceEdges(ContigGraph *graph, std::vector<int> edges, ContigGraph::Lib::Type type);

            bool sameEdges(ContigGraph *graph, int e1, int e2);
        };
    }
}


#endif //SCAFFOLDER_RULEINONELINE_H
