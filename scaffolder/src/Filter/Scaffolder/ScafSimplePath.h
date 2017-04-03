#ifndef SCAFFOLDER_SCAFSIMPLEPATH_H
#define SCAFFOLDER_SCAFSIMPLEPATH_H

#include <seqan/seq_io.h>
#include <Filter/Filters/Filter.h>
#include "ContigGraph/SeqanUtils.h"

using namespace std;
using namespace seqan;

class ScafSimplePath {
private:
    const int GAP_SIZE = 100;

    Filter* graph;
    string contigFile;

    vector<string> contigs;
    vector<int> next;

    void saveContigs();
    void findPaths();
    void writeNewContigs(string out);

    string createRevCompl(string s);
    void dfsPath(int v, vector<int> &next, vector<int> &used, const vector<int> &countInVert, vector<int> &countOutVert);
    void initCountInOutVert(vector<int> &countInVert, vector<int>& countOutVert);
public:
    void evaluate(Filter* graph, string contigFile, string out);
};


#endif //SCAFFOLDER_SCAFSIMPLEPATH_H
