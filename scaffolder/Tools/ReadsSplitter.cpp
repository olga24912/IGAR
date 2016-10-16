//
// Created by olga on 16.10.16.
//

#include "ReadsSplitter.h"
#include "FastaToolsIn.h"
#include "FastaToolsOut.h"

void ReadsSplitter::findAndSplitNotAlignmentReads(string rnaReadsFileName, string alignmentFileName, string resFileName) {
    cerr << "start split reads" << endl;
    unordered_set<string> alignmentReads = findAlignmentReads(alignmentFileName);

    FastaToolsIn ftin;
    ftin.parse(rnaReadsFileName);

    FastaToolsOut ftout;
    ftout.putFileName(resFileName);

    cerr << "start rewrite reads" << endl;

    while (ftin.next()) {
        string readName = ftin.currentName();

        if (alignmentReads.count(readName) == 0) {
            string seq = ftin.currentRef();

            int len = (int) seq.size() / 2;
            string readName1 = readName;
            readName1 += "_1";
            string readName2 = readName;
            readName2 += "_2";

            ftout.write(readName1, seq.substr(0, len));
            ftout.write(readName2, seq.substr(len, seq.size() - len));
        }
    }

    ftin.close();
    ftout.close();
}

unordered_set<string> ReadsSplitter::findAlignmentReads(string fileName) {
    cerr << "start read sam file" << endl;
    unordered_set<string> usedReads;

    BamFileIn bamFile;
    open(bamFile, fileName.c_str());

    BamHeader samHeader;
    readHeader(samHeader, bamFile);

    BamAlignmentRecord read;
    while (!atEnd(bamFile)) {
        readRecord(read, bamFile);
        string name = string(toCString(read.qName));
        cerr << name << "\n";
        if (read.rID != -1) {
            usedReads.insert(name);
        }
    }
    close(bamFile);
    return usedReads;
}

string ReadsSplitter::getName(string headerLine) {
    string name;
    for (int i = 1; i < (int)headerLine.size() && headerLine[i] != ' '; ++i) {
        name += headerLine[i];
    }

    return name;
}