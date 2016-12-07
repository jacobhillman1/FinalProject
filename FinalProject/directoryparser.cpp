#include "directoryparser.h"

using namespace std;

DirectoryParser::DirectoryParser(char * dirName)
{
    traverse(dirName);
    numberOfFiles = paths.size();
}

void DirectoryParser::traverse(char* dirName)
{
    //a pointer to the open directory
    DIR *directory = nullptr;
    //the pointer to the individual contents of the directory
    struct dirent *file;
    //the information of each file/directory
    struct stat fileInfo;

    //open the directory
    directory = opendir(dirName);
    if (!directory)
    {
        cout << "ERROR: Directory could not be found...\n";
        return;
    }

    int filecount = 0;
    //read the files and call function if there is another directory
    while( (file = readdir(directory) ) != nullptr )
    {
        //if the file pointer doesn't point to the current directory
        if (file->d_name[0] != '.')
        {
            //create a string that represents the path (directory + file name)
            DSString fileName( file->d_name );
            DSString path = DSString( dirName ) + "/" + fileName;

            //if the file is a source code file
            if (targetFile(path))
            {
                //add the pathname to the hashtable of pathnames & files
                paths.add(path);

                //create a new File object with the pathname
                File newFile(path, fileName);

                processFiles(path.c_str(), newFile);

                //find the user defined class names
                collectClasses(path.c_str());

                files.insertValue(path, newFile);
                filecount++;
            }

            char * charPath = path.c_str();
            //read the path information into the stat object
            stat( charPath, &fileInfo );
            //if the current file is a directory
            //recursively call the traverse function for the new directory.
            if ( S_ISDIR( fileInfo.st_mode ) )
            {
                traverse( charPath );
            }
        }
    }
    //close the directory
    closedir( directory );

}


//function that checks if the file is one of the target files
//which are .cpp, .c, .hpp, or .h files.
bool DirectoryParser::targetFile(DSString& fileName)
{
    //creates a substring of the characters following the last period
    DSString sub;
    for (int i = fileName.size(); i >= 0; i--)
    {
        if (fileName[i] == '.')
            sub = fileName.substring(i + 1, fileName.size() + 1);
    }

    //if the substring is a source code file return true
    if (sub == "cpp" || sub == "c" || sub == "hpp" || sub == "h")
        return true;

    else
    {
        return false;
    }
}

//function that loops through the files and runs the metrics
void DirectoryParser::runMetrics()
{
    for (int i = 0; i < paths.size(); i++)
    {
        processMetricsForFile( files.getValue(paths[i]) );
    }
}

//function that counts the number of lines in a file to store in the file object
void DirectoryParser::processFiles(char* filePath, File& fileName)
{
    //open the file and check that it is open
    ifstream file;
    file.open(filePath);

    if (!file)
    {
        cout << "FILE COULDN'T BE OPENED" << endl;
        file.close();
    }

    //count the number of lines in each file in order to weight the score.
    char line[400];
    int counter = 0;
    while (file.getline(line, 400))
    {
        counter++;
    }

    //assign the file object with the number of lines
    fileName.setNumLines(counter);

    file.close();
}

/* File creates an array of abstract metric objects that will be assigned to
 * each metric object and then run on the file.
 */
void DirectoryParser::processMetricsForFile(File & fileObject)
{
    ifstream file;
    file.open(fileObject.getPath().c_str());

    if( !file )
    {
        cout << "FILE COULDN'T be found..." << endl;
        file.close();
    }

    //create a vector of metric info that will be copied to the file object
    DSVector<MetricInfo> vector;

    //declare objects of each metric and abstract metric pointers to point to them
    VariableMetric varMet;
    IndentationMetric indMet;
    CommentMetric comMet;
    CouplingMetric coupMet;
    AbstractMetric* metrics[4];

    metrics[0] = &varMet;
    metrics[1] = & indMet;
    metrics[2] = &comMet;
    metrics[3] = &coupMet;

    //run through each metric and collect the information for the file
    for (int i = 0; i < 4; i++)
    {
        if (i == 3)
        {
            vector += (coupMet.runMetricCoupling(file, classes));
            continue;
        }
        vector += (metrics[i]->runMetric(file));
    }

    //copy the metric info vector to the file
    fileObject.setMetricInfo(vector);
}

//function is determines the code bases overall score across all files
//as a weighted average based on the approximate number of lines in the file
void DirectoryParser::processScore()
{
    double totalScore = 0.0;
    int total_number_of_lines = 0;

    //loop through each file and get the total number of lines.
    for (int i = 0; i < paths.size(); i++)
    {
        total_number_of_lines = total_number_of_lines + files.getValue(paths[i]).getNumLines();
    }

    //get the score from every file in order to average the score
    for (int i = 0; i < paths.size(); i++)
    {
        //get the weight of the file across all files by dividing the number of lines in the file
        //by the total lines across all files.
        double multiplier1 = static_cast <double> (files.getValue(paths[i]).getNumLines());
        double multiplier2 = static_cast<double> (total_number_of_lines);

        double weight = multiplier1 / multiplier2;

        //get the weighted score using the weight from above times the fileScore
        double weightedScore = (weight * files.getValue(paths[i]).getFileScore());

        //add the weighted score to the total score
        totalScore += weightedScore;
    }

    //get an average score (total / # of files)
    briefScore = totalScore;
}

/*
 * Function is passed every file. If it is a .h file, it
 * extracts the name of the class it is defining and
 * stores it in a Vector passed by reference.
 * To be used with the coupling metric.
 */
void DirectoryParser::collectClasses(char* fileName)
{

    ifstream fin;
    fin.open(fileName);
    DSString fileAsString = fileName;

    if(fileAsString.contains(".h") && !fileAsString.contains("catch.hpp")) // if it's a .h file...
    {
        char temp[99];
        char buffer[99];
        bool foundClassName = false;
        DSString tempAsString;

        while(!foundClassName && fin)
        {
            fin >> temp;

            if(!strcmp(temp, "class"))
            {
                fin >> temp;                    // read in the next word, which could be the name of the class
                tempAsString = temp;
                fin >> buffer;
                if(tempAsString.contains("{"))  //if there isn't a space between name and {
                {
                    tempAsString = tempAsString.substring(0, tempAsString.size() - 1);
                    classes.add(tempAsString);

                    foundClassName = true;
                }
                else if(!strcmp(buffer, "{") || !strcmp(buffer, ":")) // will signify that it's the class name
                {
                    classes.add(tempAsString);

                    foundClassName = true;
                }
            }
        }
    }
}

void DirectoryParser::testClasses()
{
    for(int i = 0; i < classes.size(); i++)
    {
        cout << classes.get(i) << endl;
    }
}

//function that determines which type of output the program will produce
void DirectoryParser::outputGoodness(char* outputType, char* outputFile)
{
    //open the file and process the score of the overall file
    ofstream fout(outputFile);
    processScore();

    //determine which type of output to produce.
    if (strcmp(outputType, "-v") == 0)
        verboseOutput(fout);
    else
        briefOutput(fout);
}

//the brief output outputs the score and number of files.
void DirectoryParser::briefOutput(ofstream& fout)
{
    fout << setprecision(4) << "The overall score of the directory containing " << numberOfFiles
         << " files was " << briefScore << "." << endl;
}

/* The verbose output outputs the brief output and then the 10 highest scoring
 * fileas and all of the details that go with them.
 */
void DirectoryParser::verboseOutput(ofstream& fout)
{
    briefOutput(fout);

    //set variables for keeping track of the ouput count, the number of
    //files tou output, and the scores being searched for.
    int count = 0;
    int limit = 0;
    double top = 100;
    double bottom = 99;

    //if the number of files is greater than 10, print the highest 10 scores.
    if (numberOfFiles > 10)
        limit = 10;
    else
        limit = numberOfFiles;


    fout << "\nBelow are the details for the highest 10 file scores: \n\n";

    //while the limit hasn't been reached loop through the paths and find the highest score
    while (count < limit)
    {

        //the loop checks for scores between the limits
        for (int i = 0; i < paths.size(); i++)
        {
            //check for scores between the limits and print if true
            if (count > 10)
                continue;
            else if (files.getValue(paths[i]).getFileScore() >= bottom && files.getValue(paths[i]).getFileScore() < top)
            {
                fout << files.getValue(paths[i]) << endl;
                count++;
            }
        }

        //lower the limits
        top--;
        bottom--;
    }

}

