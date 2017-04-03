#include "ScafSimplePath.h"

void ScafSimplePath::evaluate(Filter* graph, string contigFile, string out) {
    this->graph = graph;
    this->contigFile = contigFile;

    cerr << "save" << endl;
    saveContigs();
    cerr << "find path" << endl;
    findPaths();
    cerr << "write new contig" << endl;
    writeNewContigs(out);
}

void ScafSimplePath::saveContigs() {
    SeqFileIn seqFileIn(contigFile.c_str());
    StringSet<CharString> ids;
    StringSet<Dna5String> seqs;

    readRecords(ids, seqs, seqFileIn);

    for (unsigned i = 0; i < length(ids); ++i) {
        string seq = SeqanUtils::dna5ToString(toCString(seqs[i]), length(seqs[i]));
        contigs.push_back(seq);
        contigs.push_back(createRevCompl(seq));
    }
}

void ScafSimplePath::findPaths() {
    int n = graph->getVertexCount();
    vector<int> verts = graph->getVertexList();

    vector<int> used(n, 0);
    next.resize(n, -1);
    vector<int> countInVert(n, 0);
    vector<int> countOutVert(n, 0);

    initCountInOutVert(countInVert, countOutVert);

    for (int i : verts) {
        if (used[i] == 0) {
            dfsPath(i, next, used, countInVert, countOutVert);
        }
        cerr << used[i] << " " << next[i] << endl;
    }
}

void ScafSimplePath::writeNewContigs(string fileName) {
    SeqFileOut out(fileName.c_str());
    ofstream outInfo("out.info");

    StringSet<seqan::CharString> ids;
    StringSet<seqan::CharString> seqs;

    int n = graph->getVertexCount();
    vector<int> used(n, 0);
    vector<int> first(n, 1);
    for (int i = 0; i < n; ++i) {
        if (next[i] != -1) {
            first[next[i]] = 0;
        }
    }

    for (int i = 0; i < n; ++i) {
        if (first[i] && used[i] == 0) {
            stringstream ss;
            ss << i;
            string readName = "path" + ss.str();
            outInfo << ">" + readName + " ";

            string seq = contigs[i];
            used[i] = 1;
            used[i ^ 1] = 1;
            if ((i&1) == 0) {
                outInfo << "(" << graph->getTargetName(i) << " "  << i/2 << " +) ";
            } else {
                outInfo << "(" << graph->getTargetName(i^1) << " " << i/2 << " -) ";
            }

            int x = next[i];
            if (x != -1) cerr << i << " ";
            while (x != -1) {
                cerr << x << " ";
                for (int j = 0; j < GAP_SIZE; ++j) {
                    seq += 'N';
                }
                if ((x&1) == 0) {
                    outInfo << "(" << graph->getTargetName(x) << " "  << x/2 << " +) ";
                } else {
                    outInfo << "(" << graph->getTargetName(x^1) << " " << x/2 << " -) ";
                }
                seq += contigs[x];
                used[x] = 1;
                used[x^1] = 1;
                x = next[x];
            }
            if (next[i] != -1) cerr << endl;
            outInfo << "\n";
            appendValue(ids, CharString(readName.c_str()));
            appendValue(seqs, Dna5String(seq));
        }
    }

    outInfo.close();
    writeRecords(out, ids, seqs);
}

string ScafSimplePath::createRevCompl(string s) {
    string res = s;
    reverse(res.begin(), res.end());
    for (int i = 0; i < (int)res.size(); ++i) {
        if (res[i] == 'A') {
            res[i] = 'T';
        } else if (res[i] == 'T') {
            res[i] = 'A';
        } else if (res[i] == 'C') {
            res[i] = 'G';
        } else if (res[i] == 'G') {
            res[i] = 'C';
        }
    }
    return res;
}


void ScafSimplePath::dfsPath(int v, vector<int> &next, vector<int> &used,
                             const vector<int> &countInVert, vector<int> &countOutVert) {
    used[v] = 1;
    if (countOutVert[v] != 1) {
        return;
    }

    int tu = -1;
    vector<int> edges = graph->getEdges(v);
    tu = graph->getEdgeTo(graph->getEdges(v)[0]);
    if (countInVert[tu] != 1) {
        return;
    }

    next[v] = tu;

    if (tu != -1 && used[tu] == 0) dfsPath(tu, next, used, countInVert, countOutVert);
}

void ScafSimplePath::initCountInOutVert(vector<int> &countInVert, vector<int> &countOutVert) {
    for (int i = 0; i < (int)countOutVert.size(); ++i) {
        vector<int> edges = graph->getEdges(i);
        vector<int> vertOut;
        for (int e : edges) {
            int u = graph->getEdgeTo(e);
            vertOut.push_back(u);
        }
        sort(vertOut.begin(), vertOut.end());
        vertOut.resize(unique(vertOut.begin(), vertOut.end()) - vertOut.begin());
        countOutVert[i] = vertOut.size();
    }
    for (int i = 0; i < (int)countOutVert.size(); ++i) {
        vector<int> edges = graph->getEdgesR(i);
        vector<int> vertIn;
        for (int e : edges) {
            int u = graph->getEdgeFrom(e);
            vertIn.push_back(u);
        }
        sort(vertIn.begin(), vertIn.end());
        vertIn.resize(unique(vertIn.begin(), vertIn.end()) - vertIn.begin());
        countInVert[i] = vertIn.size();
    }
}
