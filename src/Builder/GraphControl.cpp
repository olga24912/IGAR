#include <Builder/GraphBuilder/RNASplitGraphBuilder.h>
#include <Builder/GraphBuilder/ConnectionGraphBuilder.h>
#include <Builder/GraphBuilder/DNAPairReadsGraphBuilder.h>
#include <Builder/GraphBuilder/LongGraphBuilder.h>
#include <Builder/GraphBuilder/FASTGGraphBuilder.h>
#include <Builder/GraphBuilder/GFAGraphBuilder.h>
#include <Builder/GraphBuilder/GFA2GraphBuilder.h>
#include "GraphControl.h"

namespace builder {
    void GraphControl::evaluate(int argc, char **argv) {
        using namespace graph_builder;
        using namespace contig_graph;

        int max_output_edges=500000;

        if (std::string(argv[1]) == "RNA_PAIR") {
            PairReadGraphBuilder* gb = new PairReadGraphBuilder;

            gb->setFileName1(argv[2]);
            gb->setFileName2(argv[3]);
            gb->setLibName(argv[4]);
            gb->setLibType(ContigGraph::Lib::Type::RNA_PAIR);

            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[5]);
        } else if (std::string(argv[1]) == "DNA_PAIR_FR") {
            DNAPairReadsGraphBuilder* gb = new DNAPairReadsGraphBuilder;

            gb->setFileName1(argv[2]);
            gb->setFileName2(argv[3]);

            gb->setLibName(argv[4]);
            gb->setLibType(ContigGraph::Lib::Type::DNA_PAIR);

            gb->setRevFirstFlag(0);
            gb->setRevSecondFlag(1);
            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[5]);
        } else if (std::string(argv[1]) == "DNA_PAIR_RF") {
            DNAPairReadsGraphBuilder* gb = new DNAPairReadsGraphBuilder;

            gb->setFileName1(argv[2]);
            gb->setFileName2(argv[3]);

            gb->setLibName(argv[4]);
            gb->setLibType(ContigGraph::Lib::Type::DNA_PAIR);

            gb->setRevFirstFlag(1);
            gb->setRevSecondFlag(0);
            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[5]);
        } else if (std::string(argv[1]) == "DNA_PAIR_FF") {
            DNAPairReadsGraphBuilder* gb = new DNAPairReadsGraphBuilder;

            gb->setFileName1(argv[2]);
            gb->setFileName2(argv[3]);

            gb->setLibName(argv[4]);
            gb->setLibType(ContigGraph::Lib::Type::DNA_PAIR);

            gb->setRevFirstFlag(0);
            gb->setRevSecondFlag(0);
            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[5]);
        } else if (std::string(argv[1]) == "LONG") {
            LongGraphBuilder* gb = new LongGraphBuilder;

            gb->setFileName(argv[2]);
            gb->setContigFile(argv[3]);

            gb->setLibName(argv[4]);
            gb->setLibType(ContigGraph::Lib::Type::LONG);

            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[5]);
        } else if (std::string(argv[1]) == "RNA_SPLIT_50") {
            RNASplitGraphBuilder* gb = new RNASplitGraphBuilder;

            gb->setFileName1(argv[2]);
            gb->setFileName2(argv[3]);

            gb->setLibName(argv[4]);
            gb->setLibType(ContigGraph::Lib::Type::RNA_SPLIT_50);

            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[5]);
        } else if ( std::string(argv[1]) == "RNA_SPLIT_30") {
            RNASplitGraphBuilder* gb = new RNASplitGraphBuilder;

            gb->setFileName1(argv[2]);
            gb->setFileName2(argv[3]);

            gb->setLibName(argv[4]);
            gb->setLibType(ContigGraph::Lib::Type::RNA_SPLIT_30);

            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[5]);
        } else if (std::string(argv[1]) == "CONNECTION") {
            ConnectionGraphBuilder* gb = new ConnectionGraphBuilder;

            gb->setConnectionFile(argv[2]);
            gb->setContigFile(argv[3]);

            gb->setLibName(argv[4]);
            gb->setLibType(ContigGraph::Lib::Type::CONNECTION);

            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[5]);
        } else if (std::string(argv[1]) == "FASTG") {
            FASTGGraphBuilder* gb = new FASTGGraphBuilder;

            gb->setFASTGFile(argv[2]);
            gb->setContigFile(argv[3]);

            gb->setLibName(argv[4]);
            gb->setLibType(ContigGraph::Lib::Type::FASTG);

            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[5]);
        } else if (std::string(argv[1]) == "GFA") {
            GFAGraphBuilder* gb = new GFAGraphBuilder;

            gb->setGFAFile(argv[2]);

            gb->setLibName(argv[3]);
            gb->setLibType(ContigGraph::Lib::Type::GFA);

            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[4]);
        } else if (std::string(argv[1]) == "GFA2") {
            GFA2GraphBuilder* gb = new GFA2GraphBuilder;

            gb->setGFAFile(argv[2]);
            gb->setContigFile(argv[3]);

            gb->setLibName(argv[4]);
            gb->setLibType(ContigGraph::Lib::Type::GFA2);

            gb->setGraph(&graph);
            gb->evaluate();

            max_output_edges=atoi(argv[5]);
        } else {
            ERROR("unkonwn connection type: " << argv[1]);
        }

        std::string path = ".";
        graph.write(path + "/graph.gr", max_output_edges);
    }
}