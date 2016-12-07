#include "file.h"

File::File()
{
    path = "";
    name = "";
}

File::File(DSString filePath, DSString fileName)
{
    path = filePath;
    name = fileName;
}

File::File(const File & copy)
{
    path = copy.path;
    name = copy.name;
    numLines = copy.numLines;
}

int File::getNumLines()
{
    return numLines;
}

DSString& File::getPath()
{
    return path;
}

DSString& File::getName()
{
    return name;
}

File& File::operator =( const File& rhs )
{
    if (this != &rhs)
    {
        path = rhs.path;
        name = rhs.name;
        numLines = rhs.numLines;
    }

    return *this;

}

void File::setNumLines(int new_number_of_lines)
{
    numLines = new_number_of_lines;
}

//Function that reads in a vector of Metric info and sets the average
//score of the file.
void File::setMetricInfo(DSVector<MetricInfo> & vector)
{
    metricInformation = vector;

    double avgScore = 0.0;
    int ignoreCount = 0;

    for (int i = 0; i < metricInformation.size(); i++)
    {
        if (metricInformation[i].getScore() == -1.0)
        {
            ignoreCount++;
            continue;
        }
        avgScore += metricInformation[i].getScore();
    }

    //the average score is equal to the total divided by the number of metrics
    //counted in the total score (if -1 was the score it was ignored)
    avgScore /= (metricInformation.size() - ignoreCount);

    setFileScore(avgScore);

    //process the verbose output after the score is calculated
    processVerbose();
}

void File::setFileScore(double score)
{
    fileScore = score;
}

double File::getFileScore()
{
    return fileScore;
}

DSVector<MetricInfo>& File::getMetricInfo()
{
    return metricInformation;
}

//Function processes the verbose output of the file by adding a title and information from each metric
//to the verbose string variable.
void File::processVerbose()
{
    //add the file header to the verbose output
    verbose = DSString ("------------------------------------------------------------------------\n")
            + DSString("For file \"") + name + DSString("\", the file score was ")
            + DSString::to_string(fileScore) + DSString(". The details are shown below:\n")
            + DSString ("------------------------------------------------------------------------\n");

    //add the Metric details and score to the verbose output
    for (int i = 0; i < metricInformation.size(); i++)
        verbose = verbose + metricInformation.get(i).getDetails() + DSString("\n    Metric ")
                + metricInformation.get(i).getMetricId() + DSString(" Score: ")
                + metricInformation.get(i).getScore() + DSString("\n\n");
}

//the greater than operator compares the scores of the file.
bool File::operator >(const File& rhs)
{
    if (fileScore > rhs.fileScore)
        return true;
    else
        return false;
}

//the overloaded stream insertion operator outputs the verbose string
std::ostream& operator<< (std::ostream& output, const File& fileObject)
{
    output << fileObject.verbose;

    return output;
}
