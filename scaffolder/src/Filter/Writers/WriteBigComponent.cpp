//
// Created by olga on 26.01.17.
//

#include "WriteBigComponent.h"

WriteBigComponent::WriteBigComponent(
        std::string fileName, int minSize, Filter *filter1) : fileName(fileName), minSize(minSize), Writer(filter1) {

}

void WriteBigComponent::write() {
    int n = filter->getVertexCount();
    int *col = new int[n];
    int cur = searcher.findComponent(col);

    std::vector< std::vector<int> > parts(cur);
    for (int i = 0; i < n; ++i) {
        if (col[i] == 0) continue;
        parts[col[i]].push_back(i);
    }

    for (int i = 1; i < cur; ++i) {
        if (parts[i].size() >= minSize) {
            std::string fn = fileName;
            std::stringstream ss;
            ss << i;
            fn += ss.str();
            dotWriter.writeVertexSet(parts[i], fn);
        }
    }
    delete col;
}