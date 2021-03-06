#include "ReadsSplitter.h"

namespace reads_splitter {
    void ReadsSplitter::splitRead(std::string readName, std::string seq, int len, seqan::SeqFileOut &out1,
                                  seqan::SeqFileOut &out2) {
        TRACE("splitRead readName=" << readName << " seq=" << seq << " len=" << len);
        std::string readName1 = readName;
        readName1 += "/1";
        std::string readName2 = readName;
        readName2 += "/2";

        TRACE("readName1=" << readName1 << " readName2=" << readName2);

        seqan::StringSet<seqan::CharString> ids;
        seqan::appendValue(ids, seqan::CharString(readName1.c_str()));

        seqan::StringSet<seqan::CharString> seqs;
        seqan::appendValue(seqs, seqan::Dna5String(seq.substr(0, len)));

        seqan::writeRecords(out1, ids, seqs);
        TRACE("seq1=" << seq.substr(0, len));

        clear(ids);
        clear(seqs);

        seqan::appendValue(ids, seqan::CharString(readName2.c_str()));
        seqan::appendValue(seqs, seqan::Dna5String(seq.substr(len, seq.size() - len)));

        seqan::writeRecords(out2, ids, seqs);
        TRACE("seq2=" << seq.substr(len, seq.size() - len));
    }
}
