#ifndef SCAFFOLDER_PACBIOGRAPHBUILDER_H
#define SCAFFOLDER_PACBIOGRAPHBUILDER_H

#include "GraphBuilder.h"
#include <seqan/seq_io.h>

namespace builder {
    namespace graph_builder {
        class PacbioGraphBuilder : public GraphBuilder {
        protected:
            std::string contigFileName;
            std::string fileName;

            struct AligInfo {
                std::string pacbioName;
                int len, cb, ce;
                char strand;
                std::string nodename;
                AligInfo(std::string pacbioName, int len, int cb, int ce, char strand, std::string nodename):
                        pacbioName(pacbioName), len(len), cb(cb), ce(ce), strand(strand), nodename(nodename) {
                }
                AligInfo(){}
            };

            std::vector<AligInfo> aligInfo;

            void readHeaderInit();
            void updateEdges(std::string s);
        public:
            PacbioGraphBuilder() = default;

            void setFileName(const std::string &fileName);
            void setContigFileName(const std::string &fileName);

            virtual void evaluate();

        private:
            DECL_LOGGER("PairReadGraphBuilder");
        };
    }
}


#endif //SCAFFOLDER_PACBIOGRAPHBUILDER_H
