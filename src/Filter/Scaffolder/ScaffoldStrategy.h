#ifndef SCAFFOLDER_SCAFFOLDSTRATEGY_H
#define SCAFFOLDER_SCAFFOLDSTRATEGY_H


#include <vector>
#include <ContigGraph/ContigGraph.h>
#include "Scaffolds.h"

namespace filter {
    namespace scaffolder {
        using namespace contig_graph;
        class ScaffoldStrategy {
        protected:
            std::vector<int> topsort;
            std::vector<int> color;
            std::vector<int> topSortPos;

            void topSortDfs(int v, ContigGraph *graph, std::vector<int> *used);

            void colorDfs(int v, int col, ContigGraph *graph);

            void topSort(ContigGraph *graph);

            void findCycle(ContigGraph *graph);

            int deg(int i, ContigGraph *pFilter, int dirIn);

        public:
            virtual void addConnection(Scaffolds *scaffolds, ContigGraph *graph) = 0;

        private:
            DECL_LOGGER("ScaffoldStrategy");
        };
    }
}

#endif //SCAFFOLDER_SCAFFOLDSTRATEGY_H
