#include "commentmetric.h"

CommentMetric::CommentMetric()
{
    score = 0;
}

DSVector<MetricInfo>& CommentMetric::runMetric(istream& file)
{
    file.clear();
    file.seekg(0, ios::beg);

    char line[400];
    DSString tempString;
    int commentCharacters = 0;
    int i = 0;
    int numEndofLineComments = 0;
    int endofLineChars = 0;
    double average;

    while(file.getline(line, 400))
    {
        tempString = line;
        if(tempString.contains("//"))
        {
            i = 0;
            //disregard whitespace at front of the line
            while(line[i] == ' ')
                i++;
            //if the whole line is not a comment
            if(tempString.substring(i, i + 2) != "//")
            {
                //check to see how many characters follow "//"
                commentCharacters = tempString.size() - tempString.getLocOf("//");
                endofLineChars += commentCharacters;
                numEndofLineComments++;
            }
        }

    }

    average = ((double)endofLineChars / numEndofLineComments);

    calculateScore(average);

    DSString output = getDetails(average, numEndofLineComments);

    metric.add(MetricInfo(score, output, 4));

    return metric;
}

/* Function used when determining the ideal number
 * of characters per end-of-line comment.
 * Takes in a Vector by reference to store the averages
 * for each input file.
 */
void CommentMetric::runMetricData(char* fileName, DSVector<int>& scores)
{
    ifstream file;
    file.open(fileName);
    char line[400];
    DSString tempString;
    int commentCharacters = 0;
    int i = 0;
    int numEndofLineComments = 0;
    int endofLineChars = 0;

    while(file.getline(line, 400))
    {
        tempString = line;
        if(tempString.contains("//"))
        {
            i = 0;
            //disregard whitespace at front of the line
            while(line[i] == ' ')
                i++;
            //if the whole line is not a comment
            if(tempString.substring(i, i + 2) != "//")
            {
                //check to see how many characters follow "//"
                commentCharacters = tempString.size() - tempString.getLocOf("//");

                endofLineChars += commentCharacters;
                numEndofLineComments++;
            }
        }

    }

    if(numEndofLineComments != 0)
    {
        scores.add((endofLineChars / numEndofLineComments));
    }


//    cout << "Average number of characters for each comment is "
//         << (endofLineChars / numEndofLineComments) << endl;
}

/* If the number of characters is within the standard deviation for
 * the average (34 + or - 8), then the score is zero. Otherwise,
 * the score is 5 times the number of characters away from the average
 * including the standard deviation.
 */
void CommentMetric::calculateScore(double average)
{
    if (average > 0.0)
    {
        double temp =  fabs(average - 34);  //the calculated ideal average is 34

        if(temp < 8)                        //standard deviation on the calculated average
            score = 0;
        else
            score = 5*(temp - 8);
    }
    else
        score = -1;
    if(score > 100)
        score = 100;
}

DSString CommentMetric::getDetails(double average, double numLines)
{
    DSString details;

    if (average > 0.0)
        details = DSString("The Average number of characters for in-line comments is ") + DSString::to_string(average)
            + DSString(" out of ") + DSString::to_string(numLines) + DSString(" comment(s).");

    else
        details = DSString("There were no end of line comments in this file.");

    return details;
}
