/* INDENTATION METRIC
 * A metric enheriting from AbstractMetric.
 * Checks to see if the indentation of the file follows
 * common indentation practice.
 * Gives a score from 0 - 100 based on the number of incorrect
 * indentations, with a score of 0 being the best.
 */

#ifndef INDENTATIONMETRIC_H
#define INDENTATIONMETRIC_H

#include "abstractmetric.h"
#include <fstream>
#include <iostream>
#include "dsvector.h"
#include "dsstring.h"
#include "metricinfo.h"


using namespace std;

class IndentationMetric : public AbstractMetric
{
public:
    IndentationMetric();
    DSVector<MetricInfo>& runMetric(istream&);
private:
    DSVector<DSString> lineNums;
    DSVector<MetricInfo> metric;
    double score;
    void checkIndentation(istream&, int&, double&, DSVector<DSString> &);
    void calculateScore(DSVector<DSString>, int);
    void removeSpaceAtFront(DSString&);
    void removeSpaceAtEnd(DSString&);

};

#endif // INDENTATIONMETRIC_H
