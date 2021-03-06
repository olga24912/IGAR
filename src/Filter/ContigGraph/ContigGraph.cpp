#include <sstream>
#include "ContigGraph.h"
#include "cmath"


namespace filter {
    namespace contig_graph {
        const std::string ContigGraph::Lib::typeToStr[] = {"REF", "DNA_PAIR", "RNA_PAIR", "RNA_SPLIT_50",
                                                           "RNA_SPLIT_30", "SCAFF"};

        int ContigGraph::getLibNum() {
            TRACE("get lib num: " << (int) libs.size());
            return libCnt;
        }

        int ContigGraph::getVertexCount() {
            TRACE("get vertex count: " << vertCnt);
            return vertCnt;
        }

        int ContigGraph::getTargetLen(int v) {
            //TRACE("get target " << v << " length: " << (targets[v].len));
            if (targets.count(v) == 0) return 0;
            return targets[v].len;
        }

        int ContigGraph::addVertex(int id, std::string name, int len) {
            DEBUG("add vertex id = " << id << "  vertex name = " << name << "  vertex len = " << len);

            targetId[name] = id;

            targets[id].name = name;
            targets[id].len = len;
            ++vertCnt;
            return id;
        }

        std::vector<int> ContigGraph::getEdges(int v) {
            TRACE("getEdges v=" << v);
            if (!targets.count(v)) return std::vector<int>();
            int cur = 0;
            for (int i = 0; i < (int)targets[v].edges.size(); ++i) {
                if (edges.count(targets[v].edges[i]) != 0) {
                    int u = getEdgeTo(targets[v].edges[i]);
                    if (targets.count(u) == 0) {
                        delEdge(targets[v].edges[i]);
                    } else {
                        targets[v].edges[cur] = targets[v].edges[i];
                        ++cur;
                        TRACE("cur=" << cur);
                    }
                }
            }
            targets[v].edges.resize(cur);

            TRACE("end get edge v=" << v);
            return targets[v].edges;
        }

        std::vector<int> ContigGraph::getEdgesR(int v) {
            TRACE("getEdgesR v=" << v);
            if (!targets.count(v)) return std::vector<int>();
            int cur = 0;
            for (int i = 0; i < (int)targets[v].edgesR.size(); ++i) {
                if (edges.count(targets[v].edgesR[i]) != 0) {
                    int u = getEdgeFrom(targets[v].edgesR[i]);
                    if (targets.count(u) == 0) {
                        delEdge(targets[v].edgesR[i]);
                    } else {
                        targets[v].edgesR[cur] = targets[v].edgesR[i];
                        ++cur;
                    }
                }
            }

            targets[v].edgesR.resize(cur);

            return targets[v].edgesR;
        }

        int ContigGraph::getEdgeTo(int e) {
            if (edges.count(e) == 0) return 0;
            TRACE("get v (e(" << e << "):u(" << edges[e].from << ")->V(" << edges[e].to << "))");
            return edges[e].to;
        }

        int ContigGraph::getEdgeFrom(int e) {
            if (edges.count(e) == 0) return 0;
            TRACE("get U (e(" << e << "):U(" << edges[e].from << ")->v(" << edges[e].to << "))");
            return edges[e].from;
        }

        void ContigGraph::write(std::string fileName) {
            std::unordered_map<int, int> new_lib;
            std::unordered_map<int, int> new_vert;
            std::unordered_map<int, int> new_edge;

            INFO("Write graph to " << fileName);
            std::ofstream out(fileName);

            DEBUG("libs num=" << libCnt << " vertex num=" << vertCnt << " edegs num=" << edgeCnt);

            int cur = 0;
            for (auto l : libs) {
                new_lib[l.first] = cur;
                ++cur;
            }

            cur = 0;
            for (auto v : targets) {
                new_vert[v.first] = cur;

                if (v.first != cur) {
                    WARN("v first = " << v.first << " cur=" << cur);
                }
                ++cur;

            }

            std::vector<int> edg;
            for (auto e : edges) {
                edg.push_back(e.first);
            }

            for (int e : edg) {
                if (targets.count(edges[e].from) == 0 ||
                    targets.count(edges[e].to) == 0) {
                    delEdge(e);
                }
            }

            cur = 0;
            for (auto e : edges) {
                new_edge[e.first] = cur;
                ++cur;
            }

            out << libCnt << "\n";

            for (auto l : libs) {
                out << "l " << new_lib[l.first] << " " << l.second.color << " " << l.second.name << " " << Lib::typeToStr[l.second.type] << "\n";
            }

            out << vertCnt << "\n";
            for (auto v : targets) {
                out << "v " << new_vert[v.first] << " " << v.second.name << " " << v.second.len << "\n";
            }

            out << edgeCnt << "\n";
            for (auto e : edges) {
                out << "e " << new_edge[e.first] << " " << new_vert[e.second.from] << " " << new_vert[e.second.to] << " " <<
                    new_lib[e.second.lib] << " " << e.second.weight << " " << e.second.len;


                if (libs[e.second.lib].type == Lib::RNA_PAIR || libs[e.second.lib].type == Lib::RNA_SPLIT_30 ||
                    libs[e.second.lib].type == Lib::RNA_SPLIT_50) {
                    out << " \"coord: " << e.second.coordBegin1 << " " <<
                        e.second.coordEnd1 << " " << e.second.coordBegin2 << " " << e.second.coordEnd2;

                    if (e.second.chr_name != "") {
                        out << " chr_name: " << e.second.chr_name << "\" \n";
                    } else {
                        out << "\"\n";
                    }
                } else {
                    if (e.second.info != "") {
                        out << " \"" << e.second.info << "\"\n";
                    } else {
                        out << "\n";
                    }
                }
            }

            out.close();
            INFO("Finish write graph");
        }

        ContigGraph ContigGraph::read(std::string fileName) {
            INFO("Read graph from \"" << fileName << "\"");

            ContigGraph g;
            std::ifstream in(fileName);

            if (!in) {
                WARN("file " << fileName << " unable to open");
                return g;
            }

            in >> g.libCnt;
            DEBUG("lib num=" << g.libCnt);

            for (int i = 0; i < g.libCnt; ++i) {
                char c;
                int id;
                std::string type;
                in >> c >> id >> g.libs[i].color >> g.libs[i].name >> type;
                g.libs[i] = Lib(g.libs[i].color, g.libs[i].name, type);
            }

            in >> g.vertCnt;
            DEBUG("vertex num=" << g.vertCnt);

            for (int i = 0; i < g.vertCnt; ++i) {
                char c;
                in >> c;
                unsigned int v;
                in >> v;
                in >> g.targets[v].name;
                in >> g.targets[v].len;
                //TODO: read info
                g.targetId[g.targets[v].name] = v;
            }

            in >> g.edgeCnt;
            DEBUG("edge num=" << g.edgeCnt);
            std::string tmp;
            getline(in, tmp);

            for (int i = 0; i < g.edgeCnt; ++i) {
                char c;
                int id;
                std::string curLine;
                getline(in, curLine);
                std::stringstream ss(curLine);
                ss >> c >> id >> g.edges[i].from >> g.edges[i].to >> g.edges[i].lib >> g.edges[i].weight >> g.edges[i].len;
                g.targets[g.edges[i].from].edges.push_back(i);
                g.targets[g.edges[i].to].edgesR.push_back(i);
                std::string tmp;
                g.edges[i].coordBegin1 = 0;
                g.edges[i].coordEnd1 = 0;
                g.edges[i].coordBegin2 = 0;
                g.edges[i].coordEnd2 = 0;
                g.edges[i].chr_name = "";
                //TODO: read info
                if (ss >> tmp) {
                    ss >> g.edges[i].coordBegin1 >> g.edges[i].coordEnd1 >> g.edges[i].coordBegin2 >> g.edges[i].coordEnd2;
                }
                if (ss >> tmp) {
                    ss >> g.edges[i].chr_name;
                }

                g.mxEdge = std::max(g.mxEdge, id);
            }

            std::vector<int> vertxs = g.getVertexList();
            for (int v : vertxs) {
                std::sort(g.targets[v].edges.begin(), g.targets[v].edges.end(), [&](int v1, int v2) {
                   return g.edges[v1].to < g.edges[v2].to;
                });


                std::sort(g.targets[v].edgesR.begin(), g.targets[v].edgesR.end(), [&](int v1, int v2) {
                    return g.edges[v1].from < g.edges[v2].from;
                });
            }

            assert(g.vertCnt == g.targets.size());
            in.close();
            return g;
        }

        std::string ContigGraph::getTargetName(int v) {
            assert(vertCnt == targets.size());
            if (targets.count(v) == 0) return "";
            TRACE("getTargetName v=" << v << " : " << targets[v].name);

            return targets[v].name;
        }

        int ContigGraph::getEdgeWeight(int e) {
            if (edges.count(e) == 0) {
                return 0;
            }
            TRACE("getEdgeWeight e=" << e << " : " << edges[e].weight);
            return edges[e].weight;
        }

        int ContigGraph::getEdgeLib(int e) {
            if (edges.count(e) == 0) return 0;
            TRACE("getEdgeLib e=" << e << " : " << edges[e].lib);
            return edges[e].lib;
        }

        std::string ContigGraph::getLibColor(int l) {
            TRACE("getLibColor l=" << l << " : " << libs[l].color);
            return libs[l].color;
        }

        std::string ContigGraph::getLibName(int l) {
            TRACE("getLibName l=" << l << " : " << libs[l].name);
            return libs[l].name;
        }

        int ContigGraph::getTargetId(std::string name) {
            assert(vertCnt == targets.size());
            TRACE("getTargetId name=" << name << " : " << targetId[name]);
            return targetId[name];
        }

        ContigGraph::Lib::Type ContigGraph::getLibType(int l) {
            TRACE("getLibType l=" << l << " : " << Lib::typeToStr[libs[l].type]);
            return libs[l].type;
        }

        int ContigGraph::getEdgeCoordB1(int e) {
            if (edges.count(e) == 0) return 0;
            TRACE("getEdgeCoordB1 e=" << e << " : " << edges[e].coordBegin1);
            return edges[e].coordBegin1;
        }

        int ContigGraph::getEdgeCoordE1(int e) {
            if (edges.count(e) == 0) return 0;
            TRACE("getEdgeCoordE1 e=" << e << " : " << edges[e].coordEnd1);
            return edges[e].coordEnd1;
        }

        int ContigGraph::getEdgeCoordB2(int e) {
            if (edges.count(e) == 0) return 0;
            TRACE("getEdgeCoordB2 e=" << e << " : " << edges[e].coordBegin2);
            return edges[e].coordBegin2;
        }

        int ContigGraph::getEdgeCoordE2(int e) {
            if (edges.count(e) == 0) return 0;
            TRACE("getEdgeCoordE2 e=" << e << " : " << edges[e].coordEnd2);
            return edges[e].coordEnd2;
        }

        void ContigGraph::setEdgeChr(int e, std::string name) {
            TRACE("setEdgeChr e=" << e << " name=" << name);
            edges[e].chr_name = name;
        }

        std::vector<int> ContigGraph::getLibList() {
            std::vector<int> libList;
            for (auto lib : libs) {
                libList.push_back(lib.first);
            }
            return libList;
        }

        std::string ContigGraph::getInfo(int e) {
            if (edges.count(e) == 0) return "";
            std::stringstream ss;
            if (edges[e].chr_name != "") {
                ss << edges[e].chr_name << "\n";
            }
            if (edges[e].coordBegin1 != 0 ||
                    edges[e].coordBegin2 != 0 ||
                    edges[e].coordEnd1 != 0 ||
                    edges[e].coordEnd2 != 0) {
                ss << "coord: " << edges[e].coordBegin1 << "-" << edges[e].coordEnd1 << "\n" << edges[e].coordBegin2 << "-" << edges[e].coordEnd2;
            }

            TRACE("getEdgeInfo e=" << e << " : " << ss.str());
            return ss.str();
        }

        std::vector<int> ContigGraph::getVertexList() {
            assert(vertCnt == targets.size());
            std::vector<int> vrtList;
            for (auto v : targets) {
                vrtList.push_back(v.first);
            }
            return vrtList;
        }

        int ContigGraph::addEdge(int v, int u, int lib, int w, int b1, int e1, int b2, int e2) {
            DEBUG("addEdge v=" << v << " u=" << u << " lib=" << lib << " w=" << w);
            Edge e;
            e.lib = lib;
            e.weight = w;
            e.from = v;
            e.to = u;
            e.id = mxEdge + 1;
            ++mxEdge;
            e.coordBegin1 = b1;
            e.coordEnd1 = e1;
            e.coordBegin2 = b2;
            e.coordEnd2 = e2;
            ++edgeCnt;
            targets[v].edges.push_back(e.id);
            targets[u].edgesR.push_back(e.id);
            edges[e.id] = e;

            //TODO sort edges, put edge in right place
           /* std::sort(targets[v].edges.begin(), targets[v].edges.end(), [](int v1, int v2) {
                return edges[v1].to < edges[v2].to;
            });


            std::sort(targets[u].edgesR.begin(), targets[u].edgesR.end(), [](int v1, int v2) {
                return edges[v1].from < edges[v2].from;
            });*/

/*
            e.to = v^1;
            e.from = u^1;
            e.id = mxEdge + 1;
            ++mxEdge;
            e.coordBegin1 = getTargetLen(u) - e2;
            e.coordEnd1 = getTargetLen(u) - b2;
            e.coordBegin2 = getTargetLen(v) - e1;
            e.coordEnd2 = getTargetLen(v) - b1;
            ++edgeCnt;
            targets[e.from].edges.push_back(e.id);
            targets[e.to].edges.push_back(e.id);
            edges[e.id] = e;*/

            return mxEdge - 1;
        }

        void ContigGraph::setWeight(int e, int w) {
            edges[e].weight = w;
        }

        void ContigGraph::delEdge(int e) {
            TRACE("del edge e = " << e << " edgeCnt = " << edgeCnt << " edges.size=" << edges.size());
            if (edges.count(e) > 0) {
                edgeCnt--;
                edges.erase(e);
            }
            assert(edgeCnt == edges.size());
        }

        void ContigGraph::delLib(int l) {
            if (libs.count(l) > 0) {
                libCnt--;
                libs.erase(l);
            }
        }

        void ContigGraph::delVertex(int v) {
            assert(vertCnt == targets.size());
            if (targets.count(v) > 0 && targets.count((v^1)) > 0) {
                vertCnt--;
                vertCnt--;
                targets.erase(v);
                targets.erase((v^1));
                TRACE("Del " << v << " and " << (v^1)  << " " << targets.count(v) << " " << targets.count(v^1));
            }
            TRACE("del vertex v=" << v << "new cnt =" <<vertCnt << " " << targets.count(v));
        }

        ContigGraph::Lib ContigGraph::mergeLib(int l1, int l2, std::string lib_name, double w1, double w2) {
            if (getLibType(l1) != getLibType(l2)) {
                WARN("merge libs with different Type");
            }

            if (l2 < l1) {
                std::swap(l1, l2);
                std::swap(w1, w2);
            }
            DEBUG("merge Lib l1=" << l1 << " l2=" << l2);


            for (auto e: edges) {
                if (edges[e.first].lib == l1) {
                    edges[e.first].weight = round(edges[e.first].weight * w1);
                }
                if (edges[e.first].lib == l2) {
                    edges[e.first].lib = l1;
                    edges[e.first].weight = round(edges[e.first].weight * w2);
                }
            }

            for (auto v: targets) {
                mergeEdges(v.second, l1);
            }

            delLib(l2);
            libs[l1].name = lib_name;
            return libs[l1];
        }

        void ContigGraph::mergeEdges(ContigGraph::Vertex v, int l1) {
            for (int i = 0; i < (int)v.edges.size(); ++i) {
                if (edges.count(v.edges[i]) == 0) continue;
                if (edges[v.edges[i]].lib != l1) continue;
                for (int j = i + 1; j < (int)v.edges.size(); ++j) {
                    if (edges.count(v.edges[j]) == 0) continue;
                    if (edges[v.edges[j]].lib != l1) continue;
                    if (edges[v.edges[i]].to != edges[v.edges[j]].to) continue;

                    if (!intersec(edges[v.edges[i]].coordBegin1, edges[v.edges[i]].coordEnd1, edges[v.edges[j]].coordBegin1,edges[v.edges[j]].coordEnd1)) continue;
                    if (!intersec(edges[v.edges[i]].coordBegin2, edges[v.edges[i]].coordEnd2, edges[v.edges[j]].coordBegin2,edges[v.edges[j]].coordEnd2)) continue;

                    mergeEdge(v.edges[i], v.edges[j]);
                }
            }
        }

        void ContigGraph::mergeEdge(int &e1, int &e2) {
            assert(edges.count(e1) > 0);
            assert(edges.count(e2) > 0);
            edges[e1].weight += edges[e2].weight;
            edges[e1].coordBegin1 = std::min(edges[e1].coordBegin1, edges[e2].coordBegin1);
            edges[e1].coordBegin2 = std::min(edges[e1].coordBegin2, edges[e2].coordBegin2);
            edges[e1].coordEnd1 = std::max(edges[e1].coordEnd1, edges[e2].coordEnd1);
            edges[e1].coordEnd2 = std::max(edges[e1].coordEnd2, edges[e2].coordEnd2);
            delEdge(e2);
        }

        bool ContigGraph::intersec(int b1, int e1, int b2, int e2) {
            return  (b1 - COORD_DIST <= e2 && e2 <= e1 + COORD_DIST) ||
                    (b1 - COORD_DIST <= b2 && b2 <= e1 + COORD_DIST) ||
                    (b2 - COORD_DIST <= e1 && e1 <= e2 + COORD_DIST) ||
                    (b2 - COORD_DIST <= b1 && b1 <= e2 + COORD_DIST);

            return false;
        }

        int ContigGraph::getMaxVertId() {
            int mx = 0;
            for (auto vert : targets) {
                mx = std::max(mx, vert.first);
            }
            return mx;
        }

        int ContigGraph::addLib(std::string color, std::string name, ContigGraph::Lib::Type type) {
            int mxLib = 0;
            for (auto lib : libs) {
                mxLib = std::max(mxLib, lib.first);
            }
            mxLib += 1;

            libs[mxLib] = Lib(color, name, type);
            ++libCnt;

            return mxLib;
        }

        void ContigGraph::addExonBlock(std::string fileName) {
            if (fileName.substr(fileName.size() - 3) == "gff") {
                addExonBlockFromGffFile(fileName);
                return;
            }

            std::ifstream in(fileName);
            std::string line;
            while (getline(in, line)) {
                std::stringstream ss(line);
                std::string contigName;
                ss >> contigName;
                int v = targetId[contigName];

                if (targets.count(v) == 0) continue;

                int len = targets[v].len;
                int u = (v ^ 1);

                char c;
                while (ss >> c) {
                    Exon ex;
                    ss >> ex.b >> ex.e >> ex.cov >> ex.id;
                    targets[v].exonsStr1.push_back(ex);
                    int e = ex.e, b = ex.b;
                    ex.b = len - e;
                    ex.e = len - b;
                    targets[u].exonsStr1.push_back(ex);
                    ss >> c;
                }

                std::reverse(targets[u].exonsStr1.begin(), targets[u].exonsStr1.end());
            }
        }

        std::vector<ContigGraph::Exon> ContigGraph::getExons(int v, int strand) {
            if (strand == 1) {
                return targets[v].exonsStr1;
            } else {
                return targets[v].exonsStr2;
            }
        }

        void ContigGraph::addExonBlockFromGffFile(std::string fileName) {
            seqan::GffFileIn gffIn(fileName.c_str());

            seqan::GffRecord record;
            std::vector<int> rev;
            while (!seqan::atEnd(gffIn)) {
                try {
                    seqan::readRecord(record, gffIn);
                    if (std::string(seqan::toCString(record.type)) != "gene") continue;
                    //std::cerr << seqan::toCString(record.ref) << " "
                    //          << seqan::toCString(record.type) << " "
                    //          << record.beginPos << " " << record.endPos << "\n";
                    int b = record.beginPos, e = record.endPos;
                    std::string contigName = std::string(seqan::toCString(record.ref));

                    int v = targetId[contigName];
                    if (targets.count(v) == 0) continue;
                    int len = targets[v].len;

                    Exon ex;
                    ex.b = b;
                    ex.e = e;
                    if (record.strand == '+') {
                        ex.id = (int) targets[v].exonsStr1.size();
                        targets[v].exonsStr1.push_back(ex);
                    } else {
                        ex.id = (int) targets[v].exonsStr2.size();
                        targets[v].exonsStr2.push_back(ex);
                    }

                    ex.b = len - e;
                    ex.e = len - b;
                    if (record.strand == '+') {
                        targets[v ^ 1].exonsStr1.push_back(ex);
                    } else {
                        targets[v ^ 1].exonsStr1.push_back(ex);
                    }
                    rev.push_back(v ^ 1);

                } catch (seqan::Exception const & e) {
                    WARN(e.what());
                }
            }

            sort(rev.begin(), rev.end());
            rev.resize(std::unique(rev.begin(), rev.end()) - rev.begin());

            for (int v : rev) {
                std::reverse(targets[v].exonsStr1.begin(), targets[v].exonsStr1.end());
                std::reverse(targets[v].exonsStr2.begin(), targets[v].exonsStr2.end());
            }
        }

        void ContigGraph::setCoord(int e, int b1, int e1, int b2,  int e2) {
            if (edges.count(e) == 0) return;
            edges[e].coordBegin1 = b1;
            edges[e].coordEnd1 = e1;
            edges[e].coordBegin2 = b2;
            edges[e].coordEnd2 = e2;

        }
    }
}
