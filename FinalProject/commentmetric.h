/* IN-LINE COMMENT METRIC
 * A metric enheriting from AbstractMetric. Examines the
 * length of comments that are found at the end of a line
 * (after some functional code that is written).
 * Gives a score from 0 - 100 based on the number of characters,
 * with a score of 0 being the best.
 */


#ifndef COMMENTMETRIC_H
#define COMMENTMETRIC_H

#include "abstractmetric.h"
#include <fstream>
#include <iostream>
#include "dsstring.h"
#include "dsvector.h"
#include "metricinfo.h"

using namespace std;

class CommentMetric : public AbstractMetric
{
public:
    CommentMetric();
    DSVector<MetricInfo>& runMetric(istream&);
    void runMetricData(char*, DSVector<int>&);
    DSString getDetails(double average, double numLines);

private:
    double score = 0;
    DSVector<MetricInfo> metric;
    void calculateScore(double average);
};

#endif // COMMENTMETRIC_H
