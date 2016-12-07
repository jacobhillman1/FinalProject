#include "couplingmetric.h"

CouplingMetric::CouplingMetric()
{
    score = 0;
}

DSVector<MetricInfo>& CouplingMetric::runMetric(istream & fin)
{
    DSVector<MetricInfo> vector;
    return vector;
}

DSVector<MetricInfo>& CouplingMetric::runMetricCoupling(istream& fin, DSVector<DSString> classes)
{
    fin.clear();
    fin.seekg(0, ios::beg);

    char line[400];
    DSString lineAsString;
    DSString output;

    /* This metric only penalizes a file if it has over 9 user defined classes.
     * Therefor, if the user has not even implemented 9 classes, there is no need
     * to check the files.
     */
    if(classes.size() > 0)
    {
        while(fin.getline(line, 400))
        {
            lineAsString = line;

            for(int i = 0; i < classes.size(); i++)
            {
                if(classes.size() != 0)
                {
                    if(lineAsString.contains(classes.get(i)) &&
                            !lineAsString.contains(".h"))
                    {
                        numClasses++;
                        classes.remove(i); // don't add to the count if same object used twice
                    }
                }
            }
        }

        calculateScore(numClasses);

    }

    else
        score = 0;

    output = getDetails();

    metric.add(MetricInfo(score, output, 5));

    return metric;
}

/* If there are less than 5 classes detected, the score is zero.
 * Between 5 and 9 classes, each class is worth 5 points.
 * After 9 class, 5 points per class and a 20 point bonus.
 */
void CouplingMetric::calculateScore(int numClasses)
{
    if(numClasses < 5)
        score = 0;
    else if(numClasses < 10)
        score = (double)5*numClasses;
    else
    {
        score = 5*(double)numClasses;
        score += 20;
    }

    if(score > 100)
        score = 100;

}

DSString CouplingMetric::getDetails()
{
    DSString details("There are ");
    details = details + DSString(numClasses) + DSString(" user-defined classes used in this file.");
    return details;
}
