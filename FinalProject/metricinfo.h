/* MetricInfo class is a small class that holds the score, a detailed output and id for each Metric
 *
 * Author: Will Kastner & Jacob Hillman
 */

#ifndef METRICINFO_H
#define METRICINFO_H

#include "dsstring.h"

class MetricInfo
{
public:
    MetricInfo();
    MetricInfo(double, DSString, int);
    DSString& getDetails();
    double getScore();
    int getMetricId();

private:
    DSString details;
    double metricScore = 0.0;
    int METRIC_ID = 0;

};

#endif // METRICINFO_H
