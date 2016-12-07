#include "metricinfo.h"

MetricInfo::MetricInfo()
{
    metricScore == 0.0;
    details = "";
    METRIC_ID = 0;
}

MetricInfo::MetricInfo(double score, DSString string, int id)
{
    metricScore = score;
    details = string;
    METRIC_ID = id;
}

DSString& MetricInfo::getDetails()
{
    return details;
}

double MetricInfo::getScore()
{
    return metricScore;
}

int MetricInfo::getMetricId()
{
    return METRIC_ID;
}
