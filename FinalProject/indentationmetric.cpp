#include "indentationmetric.h"

IndentationMetric::IndentationMetric()
{

}

DSVector<MetricInfo>& IndentationMetric::runMetric(istream& fin)
{
    fin.clear();
    fin.seekg(0, ios::beg);

    DSVector<DSString> lineNums;
    int indentNum = 0;
    double lineCount = 0;
    DSString output;

    char line[400];

    while(fin.getline(line, 400))
    {
        lineCount++;
        DSString lineAsString = line;
        // everything before this bracket should not be indented
        if(lineAsString.contains("{") && !lineAsString.contains("}")
                && lineAsString.substring(0, 9) != "namespace")
            checkIndentation(fin, indentNum, lineCount, lineNums);
    }

    calculateScore(lineNums, lineCount);


    if (score == 0.0)
    {
        output = "There was no incorrect indentation in the file.";
    }
    else
    {
        output = "Incorrect indentation is found in line(s): ";
        DSString numbers("\n");
        for(int i = 0; i < lineNums.size(); i++)
        {
            numbers = numbers + lineNums.get(i) + DSString(" ");
            if ((i + 1) % 15 == 0)
                numbers = numbers + DSString("\n");
        }
        output = output + numbers + DSString("\n");
    }


    metric.add(MetricInfo(score, output, 3));

    return metric;

}

/*
 * Recursive function to check the indentation. As
 * the function finds opening brackets, it will continue to call
 * itself until it finds closing brackets. The indentNum int passed by reference
 * keeps track of the level of indentation. lineNums records the
 * location of infractions.
 */
void IndentationMetric::checkIndentation(istream &fin, int &indentNum,
                                         double& lineCount, DSVector<DSString>& lineNums)
{
    indentNum += 1;                 //stepping one level deeper in the indentation

    int requireSpace = indentNum*4; //depending on the level, this is how much
                                    //white space is required
    int spaceCount = 0;             //keeps track of spaces at the beginning of each line

    char line[400];

    DSString lineAsString;
    DSString tempString;

    int i = 0;
    bool space = true;
    bool runAlt = false;
    bool check = true;
    bool privacyLine = false;

    while(fin.getline(line, 400))
    {
        lineCount++;
        lineAsString = line;
        check = true;
        runAlt = false;
        privacyLine = false;

        //get rid of extraneous spaces
        removeSpaceAtEnd(lineAsString);

        if(lineCount > 90)
        /* checking for the '{' or '}' characters is a big part of this metric,
         * so making sure they are not found in comments is important. */
        if(lineAsString.contains("//"))
        {
            tempString = (lineAsString);
            removeSpaceAtFront(tempString);
            if(tempString.getLocOf("//") == 0)
                check = false;
            else
            {
                /* If the comment is not at the beginning of a line, make
                 * a substring of the line without the comment. Remove the
                 * space that was between the comment and last character of code.*/
                lineAsString = lineAsString.substring(0, lineAsString.getLocOf("//"));
                removeSpaceAtEnd(lineAsString);
            }
        }

        //same process for different syntax
        if(lineAsString.contains("/*"))
        {
            if(tempString.getLocOf("/*") != 0)
            {
                lineAsString = lineAsString.substring(0, lineAsString.getLocOf("/*"));
                removeSpaceAtEnd(lineAsString);
            }
            else
                check = false;

        }
        //check for brackets again...
        else if(lineAsString.contains("{") && !lineAsString.contains("}") && !lineAsString.contains("\"{\"")
                && !lineAsString.contains("'{'"))
        {
            /*if an opening bracket is found, the code that follows should be one level of
             * indentation greater */
            checkIndentation(fin, indentNum, lineCount, lineNums);
        }
        else if(lineAsString.contains("}") && !lineAsString.contains("{") && !lineAsString.contains("\"}\"")
                && !lineAsString.contains("'}'"))
        {
            /*if a closing bracket is found, the code that follows should be one level of
             * indentation less */
            indentNum--;                        //stepping out one level
            break;
        }


        /* Check for lines that do not end with a semi colon or bracket */
        else if(lineAsString[lineAsString.size() - 1] != ';'
                && lineAsString[lineAsString.size() - 1] != '{' ) //ignore the line if there's no semi colon
        {
            runAlt = true;                      //these get messy
        }

        //if it's a blank line...
        if(lineAsString.size() == 0)
        {
            runAlt = false;
            check = false;
        }

        //the following will run if the line does not end with a semi colon or bracket
        if(runAlt && check)
        {
            bool cont = true;
            tempString = lineAsString;
            removeSpaceAtFront(tempString);
            if(tempString == "private:" || tempString == "public:")
            {
                check = true;
                privacyLine = true;
                cont = false;
            }
            if(cont)
            {
                fin.getline(line, 400);
                lineCount++;
                runAlt = false;
                lineAsString = line;
                tempString = line;
                removeSpaceAtFront(tempString);
                if(tempString[0] == '{')          //if the bracket is just on the next line
                {
                    checkIndentation(fin, indentNum, lineCount, lineNums);
                }
                else
                    check = false;
            }
        }
        /* Once the code gets to this point, it can be assumed that this
         * is a "normal" line that should simply follow the indentation guidelines.
         */
        if(check)
        {
            i = 0;
            spaceCount = 0;
            space = true;
            while(space)
            {
                if(line[i] == ' ')
                {
                    spaceCount++;
                    i++;
                }
                else if(line[i] == '\t')
                {
                    spaceCount += 4;
                    i++;
                }
                else
                {
                    if(privacyLine)
                    {
                        if(spaceCount != 0)
                            lineNums.add(lineCount);
                    }
                    else if(spaceCount != requireSpace)
                    {
                        if(lineAsString != "public:" && lineAsString != "private:")
                            lineNums.add(lineCount);
                    }
                    space = false;
                }
            }
        }
    }
}

/* Calculate the score of this file based on the number of
 * incorrect indentations. Every incorrect indentation counts
 * for 5 points.
 */
void IndentationMetric::calculateScore(DSVector<DSString> lineNums, int lineCount)
{
    if (lineCount == 0)
        score = 0.0;
    else
    {
        double ratio = ((double)lineNums.size() / lineCount);
        score = 100*ratio;
        score *= 4;
    }

    if(lineNums.size() > 50)
        score = -1;
    else if(score > 100)
        score = 100;
}

/* Removes all space characters from the beginning of the String. */
void IndentationMetric::removeSpaceAtFront(DSString& in)
{
    int i = 0;
    while(in[i] == ' ' || in[i] == '\t')
        i++;

    in = in.substring(i, in.size());
}

/* Removes all space characters from the end of the String. */
void IndentationMetric::removeSpaceAtEnd(DSString& in)
{
    int spaceCount = 0;
    for(int i = in.size() - 1; i >= 0; i--)
    {
        if(in[i] == ' ' || in[i] == '\t')
            spaceCount++;
        else
            break;
    }

    in = in.substring(0, in.size() - spaceCount);
}
