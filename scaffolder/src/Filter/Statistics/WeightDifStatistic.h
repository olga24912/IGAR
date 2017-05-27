#ifndef SCAFFOLDER_WEIGHTDIFSTATISTIC_H
#define SCAFFOLDER_WEIGHTDIFSTATISTIC_H

#include "Statistic.h"

class WeightDifStatistic : public Statistic {
public:
    void calculateStatistic(Filter *filter, std::string coordFile, int libNum, int stepW,
                            int mxW, int stepDif, int mxDif);
};


#endif //SCAFFOLDER_WEIGHTDIFSTATISTIC_H