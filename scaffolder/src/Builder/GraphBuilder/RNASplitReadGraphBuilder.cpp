//
// Created by olga on 08.10.16.
//

#include "RNASplitReadGraphBuilder.h"
#include "Builder/SamFileWriter/SamFileWriteEdge.h"

void RNASplitReadGraphBuilder::evaluate() {
    SystemAlignmentTools wwot;
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

    handlingPairReads("cutPartReads1.fasta", "cutPartReads2.fasta", "-50-50");

    graph->newLib(libName + "-long-short", getLibColor());

    handlingPairReads("short1.fasta", "short2.fasta", "-long-short");
}

void RNASplitReadGraphBuilder::setRefFileName(string refFileName) {
    RNASplitReadGraphBuilder::refFileName = refFileName;
}

void RNASplitReadGraphBuilder::setRnaReadFileName(string rnaReadsFileName) {
    RNASplitReadGraphBuilder::rnaReadsFileName = rnaReadsFileName;
}

void RNASplitReadGraphBuilder::handlingPairReads(string file1, string file2, string libN) {
    SystemAlignmentTools wwot;

    wwot.alignmentRNA(refFileName, file1, "rna1.sam");
    wwot.alignmentRNA(refFileName, file2, "rna2.sam");

    RNAPairReadGraphBuilder gb;

    gb.setFileName1("rna1.sam");
    gb.setFileName2("rna2.sam");
    gb.setOneSideReadFlag(true);
    gb.setGraph(graph);

    gb.setMinContigLen(minContigLen);

    SamFileWriteEdge writeEdge("reads_" + libName + libN);
    gb.setSamFileWriter(writeEdge);

    gb.evaluate();
}

string RNASplitReadGraphBuilder::getLibColor() {
    int cntRB = 100 + rand()%150;
    int color[3] = {cntRB, rand()%(cntRB/2), cntRB};
    return colorToString(color);
}
