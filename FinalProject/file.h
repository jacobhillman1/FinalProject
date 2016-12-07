/* The File class is used to hold useful information about a file that is being read from the directory.
 * An object of this class will hold the full path, name, number of lines, and metric scores/information
 * of the file. There are get and set functions for each of these attributes as well as functions to
 * process the output of the metrics.
 *
 * Authors: Will Kastner & Jacob Hillman
 *
 */

#ifndef FILE_H
#define FILE_H

#include "dsstring.h"
//#include "avltree.h"
#include "variable.h"
#include "abstractmetric.h"
#include "dsvector.h"
#include "metricinfo.h"

class File
{
public:
    File();
    File(DSString, DSString);
    File(const File&);
    DSString& getPath();
    DSString& getName();
    int getNumLines();
    DSVector<MetricInfo>& getMetricInfo();
    void setNumLines(int);
    void setMetricInfo(DSVector<MetricInfo>& );
    void setFileScore(double );
    File& operator=(const File&);
    double getFileScore();
    void processVerbose();
    bool operator>(const File&rhs);

    friend std::ostream& operator<< (std::ostream&, const File&);

private:
    DSString name;
    DSString path;
    DSString verbose;
    double fileScore;
    int numLines = 0;
    DSVector<MetricInfo> metricInformation;

};

#endif // FILE_H
