#ifndef SCAFFOLDER_DOTWRITERBUILDER_H
#define SCAFFOLDER_DOTWRITERBUILDER_H


#include "DotWriter.h"

class DotWriterBuilder {
protected:
    Filter* filter;
    FileValidator* validator;
    int maxVert = 20;
    int maxEdge = 40;
public:
    virtual DotWriter build() {
        return DotWriter(filter, validator, maxVert, maxEdge);
    }

    void setFilter(Filter* filter) {
        this->filter = filter;
    }

    void setValidator(FileValidator* validator) {
        this->validator = validator;
    }

    void setMaxVert(int maxVert) {
        DotWriterBuilder::maxVert = maxVert;
    }

    void setMaxEdge(int maxEdge) {
        DotWriterBuilder::maxEdge = maxEdge;
    }
};


#endif //SCAFFOLDER_DOTWRITERBUILDER_H