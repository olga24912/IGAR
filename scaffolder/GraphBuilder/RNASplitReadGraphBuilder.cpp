//
// Created by olga on 08.10.16.
//

#include "RNASplitReadGraphBuilder.h"

void RNASplitReadGraphBuilder::evaluate() {
    WorkWithOtherTools wwot;
    ReadsSplitter50 rs;
    SplitterByUnmappedEnd su;

    wwot.alignmentRNA(refFileName, rnaReadsFileName, "rna.sam");
    string unmappedName;
    if (rnaReadsFileName[rnaReadsFileName.size() - 1] == 'q') {
        unmappedName = "Unmapped.fastq";
        system("mv Unmapped.out.mate1 Unmapped.fastq");
    } else {
        unmappedName = "Unmapped.fasta";
        system("mv Unmapped.out.mate1 Unmapped.fasta");
    }
    rs.splitReads(unmappedName, "cutPartReads1.fasta", "cutPartReads2.fasta");
    su.splitReads("rna.sam", "short1.fasta", "short2.fasta");

    handlingPairReads("cutPartReads1.fasta", "cutPartReads2.fasta", " 50-50");
    graph->newLib();
    handlingPairReads("short1.fasta", "short2.fasta", " long-short");

    graph->filterByContigLen(minContigLen);
}

void RNASplitReadGraphBuilder::setRefFileName(string refFileName) {
    RNASplitReadGraphBuilder::refFileName = refFileName;
}

void RNASplitReadGraphBuilder::setRnaReadFileName(string rnaReadsFileName) {
    RNASplitReadGraphBuilder::rnaReadsFileName = rnaReadsFileName;
}

void RNASplitReadGraphBuilder::handlingPairReads(string file1, string file2, string libN) {
    WorkWithOtherTools wwot;

    wwot.alignmentRNA(refFileName, file1, "rna1.sam");
    wwot.alignmentRNA(refFileName, file2, "rna2.sam");

    RNAPairReadGraphBuilder gb;

    gb.setFileName1("rna1.sam");
    gb.setFileName2("rna2.sam");
    gb.setOneSideReadFlag(true);
    gb.setGraph(graph);
    gb.setMinContigLen(minContigLen);
    gb.setMinEdgeWight(minEdgeWight);

    gb.evaluate();
    graph->filterByEdgeWeight(minEdgeWight);
    graph->setLibNum(libName + libN);
}
