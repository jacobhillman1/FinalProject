/* COUPLING METRIC
 * A metric enheriting from AbstractMetric.
 * Coupling metric determines the number of user-defined
 * classes used by each file.
 * Gives a score from 0 - 100 based on the number of classes,
 * with a score of 0 being the best.
 */


#ifndef COUPLINGMETRIC_H
#define COUPLINGMETRIC_H

#include "abstractmetric.h"
#include "dsstring.h"
#include "dsvector.h"
#include <fstream>
#include "metricinfo.h"

using namespace std;


class CouplingMetric : public AbstractMetric
{
public:
    CouplingMetric();
    DSVector<MetricInfo>& runMetric(istream&);
    DSVector<MetricInfo>& runMetricCoupling(istream&, DSVector<DSString>);
    DSString getDetails();

private:
    DSVector<MetricInfo> metric;
    double score = 0;
    int numClasses = 0;
    void calculateScore(int numClasses);

};

#endif // COUPLINGMETRIC_H

