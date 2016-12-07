/* Base Metric class with pure firtual functions to run metrics.
 *
 * Author: Will Kastner & Jacob Hillman
 *
 */

#ifndef ABSTRACTMETRIC
#define ABSTRACTMETRIC

#include "dsvector.h"
#include "metricinfo.h"

class AbstractMetric
{
protected:
    /* "score" will hold the number representing the "goodness" of
     * the code determined by the individual metric */
//    float score;
public:
    //default constructor instantiating score to 0
    AbstractMetric()/*:score(0)*/{}
    /* Depending on the specific inherited class, runMetric will perform
     * an operation to determine the score the code deserves */
    virtual DSVector<MetricInfo>& runMetric(istream& ) = 0;
//    float getScore();

};

#endif // ABSTRACTMETRIC
