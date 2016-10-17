//
// Created by olga on 16.10.16.
//

#include "SeqanUtils.h"

string SeqanUtils::cutReadName(BamAlignmentRecord read) {
    string readName = string(toCString(read.qName));
    if (readName.size() > 1) {
        if ((readName[readName.size() - 2] == '/' ||
             readName[readName.size() - 2] == '_') &&
            (readName[readName.size() - 1] == '2' ||
             readName[readName.size() - 1] == '1')) {
            readName.resize(readName.size() - 2);
        }
    }
    return readName;
}