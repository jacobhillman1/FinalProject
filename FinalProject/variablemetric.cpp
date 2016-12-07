#include "variablemetric.h"

//the default constructor reads in the type names from a file and stores them in a vector
VariableMetric::VariableMetric()
{
    types.insert("short");
    types.insert("int");
    types.insert("long");
    types.insert("char");
    types.insert("bool");
    types.insert("float");
    types.insert("double");
    types.insert("string");
    types.insert("vector");
    types.insert("char16_t");
    types.insert("char32_t");
    types.insert("wchar_t");
}

//function that processes a file and assigns a score for the number of intialized variables
DSVector<MetricInfo>& VariableMetric::runMetric(istream &file)
{
    //string buffers that will hold individual strings
    char curr[40];
    char prev1[40] = {0};
    char prev2[40] = {0};

    //a larger string buffer to catch comments;
    char comment[250] = {0};


    while (file >> curr)
    {

        DSString currString(curr);

        //if the line is a comment line, get the rest of the comment
        //if the current string contains the double semicolon
        //the line is a function (skip line)
        if (curr[0] == '/')
        {
            file.getline(comment, 250);
            continue;
        }

        if (currString.contains("//"))
        {
            removePostCharacter(currString, file, comment, '/');
        }

        //if the current buffer is for then skip the line because the name of the control
        //variables are unimportant and often very short
        if (currString.contains( "for" ))
        {
            file.getline(comment, 250);
            continue;
        }

        //if the next string contains parentheses, open braces or semicolons
        //it is a function and should be skipped
        if (currString.contains("(") || currString.contains(")") || currString.contains("operator")
                || currString.contains("{") || currString.contains("::"))
        {
            file.getline(comment, 250);
            char* function = strcat(curr, comment);
            char functionChar = function[strlen(function) - 1];
            if (functionChar == ';')
                continue;
            else
                processParameters(function);

            continue;
        }

        if (currString.contains("//"))
        {
            removePostCharacter(currString, file, comment, '/');
        }

        //if the current buffer matches a type
        if (checkType(curr))
        {
            //FIRST READ!!!!!!!!

            //copy back the information in curr to prev1
            //which stores the type
            strcpy(prev2, prev1);
            strcpy(prev1, curr);
            file >> curr;

           //SECOND READ!!!!!!!
            bool continueLoop = secondRead(file, curr, prev1, prev2, comment);

            if (continueLoop)
                continue;

            //THIRD READ !!!!!!!!!!!!!!!!!!!!!!!!!!
            //curr holds the initialized value, prev1 holds the name, and prev2 holds the type
            thirdRead(file, curr, prev1, prev2);
        }
    }
    variableTotal = variables.size();
    runVM1();
    runVM2();

    return metrics;

}

//function that adds a single variable to the variables list and differentiates between
//initialized and non-initialized variables
void VariableMetric::addVariable(char *curr, char *prev1, bool init, char *prev2)
{
    Variable newVariable;

    //if the variable is not initialized add only the name and type
    //and increment the notInit variable
    if (init == false)
    {
        newVariable.setInitializedWhenDeclared(init);
        newVariable.setName(curr);
        newVariable.setType(prev1);
        notInit++;
    }

    //otherwise variable is initialized and the value is added to the
    //variable object
    else
    {
        newVariable.setInitializedWhenDeclared(init);
        newVariable.setName(prev1);
        newVariable.setType(prev2);
        newVariable.setInitializedValue(curr);
    }

    variables.add(newVariable);

    //increment the number of declared variables
    declared++;
}

//function that removes all characters after a semicolon (including semicolon)
void VariableMetric::removePostCharacter(DSString & str, istream& file, char* comment, char character)
{
    int position = 0;
    for (int i = str.capacity(); i > 0; i--)
    {
        if (str[i] == character)
        {
            position = i;
        }
    }
    str = str.substring(0, position);
    file.getline(comment, 250);
}

//function that adds variables that are stored in the mulitple vector to the
//variables vector and increments the declared and not initialized variables.
void VariableMetric::addVariables(char *prev2, bool init, char *curr)
{
    Variable newVariable;

    /* if the variables are not initialized loop through and add a variable object
     * to the variables vector and increment declared and notInit. If they are
     * initialized only increment declared and add the initialized value to the variable
     */
    for (int i = 0; i < multiple.size(); i++)
    {
        newVariable.setInitializedWhenDeclared(init);
        newVariable.setName(multiple[i]);
        newVariable.setType(prev2);
        if (init == false)
        {
            notInit++;
        }
        else if (init == true)
        {
            newVariable.setInitializedValue(curr);
        }
        variables.add(newVariable);
        declared++;
    }

    //clear the multiple vector at the end
    multiple.clear();
}


/* Function performs the second read after a type is identified. Various operations can be performed in the function.
 * Removing asterisks (pointers) from names to types, ignoring functions declarations, handling multiple variables
 * declared with commas, extracting names of built in arrays, and recognizing undeclared variables are all operations
 * handled by this function
 */
bool VariableMetric::secondRead(istream & file, char *curr, char *prev1, char *prev2, char *comment)
{
    DSString currString(curr);
    //variable acts as a continue statement that will return to the runMetric function
    bool continueLoop = true;

    removeAsterisk(file, currString, curr, prev1);

    if (currString.contains("(") || currString.contains(")") || currString.contains("operator")
            || currString.contains("{") || currString.contains("::"))
    {
        file.getline(comment, 250);
        char* function = strcat(curr, comment);
        char functionChar = function[strlen(function) - 1];
        if (functionChar == ';')
            return continueLoop;
        else
            processParameters(function);

        return continueLoop;
    }

    //else if the next string ends with (,) -> multiple variables being declared
    if (curr[strlen(curr) - 1] == ',')
    {
        //loop until all but the last new variable is in the vector
        while (curr[strlen(curr) - 1] == ',')
        {
            currString = curr;
            currString = currString.substring(0, currString.capacity() - 1);
            multiple.add(currString);
            file >> curr;
        }

        //read the last new variable and addit to the vector
        if (currString[currString.size()] == ';')
            currString = currString.substring(0, currString.capacity() - 1);
        multiple.add(currString);
    }


    //else if it is a built in array, remove the braces and extract the name
    if (currString.contains("[") || currString.contains("]"))
    {
        currString = curr;
        removeBraces(currString);
        strcpy(curr, currString.c_str());
    }

    //if the variable name ends with a semicolon, ensure it isn't a function,
    //and handle it as an unitialized variable
    if (currString.contains(";"))
    {
        removePostCharacter(currString, file, comment, ';');
        strcpy(curr, currString.c_str());
        //HERE IS WHERE AN UNINITIALIZED variable is added to the vector
        //of variable objects
        if( multiple.isEmpty())
        {
            addVariable(curr, prev1, false, "");
        }
        else
        {
            addVariables(prev2, false, curr);
        }

        return continueLoop;
    }

    //if the variable was not an unitialized variable
    //return false so that the program will read in the value
    //that the variable was initialized to.
    return false;
}



/* function that performs the third read if the variable is being initialized when declared
 * if the variable is declared and doesn't end statement
 * copy back the information to hold the type, name, and = sign
 */
void VariableMetric::thirdRead(istream & file, char *curr, char *prev1, char *prev2)
{
    //copy back the strings to the previous buffers
    //and read in the next string
    strcpy(prev2, prev1);
    strcpy(prev1, curr);
    file >> curr;

    //skip the = to get initialization
    file >> curr;

    //if the multiple vector is empty add a single variable
    //otherwise add multiple that were declared.
    if (multiple.isEmpty())
        addVariable(curr, prev1, true, prev2);
    else
        addVariables(prev2, true, curr);
}



/* Function that checks whether a cstring matches one of the types
 * specified in the types vector.
 */
bool VariableMetric::checkType(char* cstr)
{
    if (cstr == nullptr)
        return false;
    //convert to a string to use string functions
    DSString compare(cstr);
    int asteriskCount = 0;

    //count the position where the asterisks start
    //if there are none, nothing will happen
    for (int i = compare.capacity(); i >= 0; i--)
    {
        if (compare[i] == '*')
            asteriskCount++;
    }

    //remove the asterisks
    compare = compare.substring(0, compare.capacity() - asteriskCount);

    //compare the string (w/o asterisks) to the vector of types
    if (types.contains(compare))
        return true;

    return false;
}



//function that removes asterisk from the front of a string
void VariableMetric::removeAsterisk(istream& file, DSString& currString, char* curr, char* prev1)
{
    //while the curr has an asterisk at the front of the string
    //add the asterisk to the type and remove it from the string
    while (curr[0] == '*')
    {
        //attach an asterisk to the type
        strcat(prev1, "*");

        //if only the only the asterisk, read in the next
        if(strcmp(curr, "*") == 0)
        {
            file >> curr;
        }

        //else if asterisk is attacked to the front of the name
        //convert to a string and call the remove asterisk function
        else
        {
            currString = currString.substring(1, currString.capacity());
            strcpy(curr, currString.c_str());
        }
    }
}

//function that removes the braces for a built in array to isolated the name
DSString& VariableMetric::removeBraces(DSString& str)
{
    //placeholder for first brace
    int firstBrace = 0;
    int secondBrace = 0;
    DSString end("");

    //set index location of each brace
    for (int i = str.capacity(); i > 0; i--)
    {
        if (str[i] == '[')
            firstBrace = i;

        if (str[i] == ']')
            secondBrace = i;
    }

    //if the closing brace was in the string
    if (secondBrace > 0)
         end = str.substring(secondBrace + 1, str.capacity());

    //remove the braces and information;
    str = str.substring(0, firstBrace);
    str = str + end;

    return str;
}

//function that prints the vector of variables, variables declared, not initialized, and the score
void VariableMetric::printVariables()
{
    for (int i = 0; i < variables.size(); i++)
    {
        cout << variables[i];
    }

//    variables.printTree();

//    cout << endl << endl << "Number of Declared variables = " << declared << endl
//         << "# of unintialized variables = " << notInit << endl;

    cout << endl << "File VM1 Score = " << setprecision(4) << fileVM1Score << endl;

    cout << endl << "Total # of variables = " << variableTotal << endl;
//    cout << endl << "# of parameters = " << numParams << endl;
}

//function returns the number of variables found in the file
int VariableMetric::getDeclared()
{
    return declared;
}

//function returns the number of variables that were declared
//but not initialized immediately in the file
int VariableMetric::getNotInit()
{
    return notInit;
}

//This function is called when a function is found to read in all paramenters
//and store their names and types
void VariableMetric::processParameters(char *line)
{
    DSString type;
    DSString name;

    tokenize(line);

    for (int i = 0; i < functionStrings.size() - 1; i++)
    {
        if (checkType(functionStrings[i].c_str()))
        {
            type = functionStrings[i];
            if (checkType(functionStrings[i + 1].c_str()))
                return;
            name = functionStrings[i+1];

            Variable newVariable(name, type, false, "", true);
            variables.add(newVariable);
            numParams++;
        }
    }

    functionStrings.clear();
}

//function that is designed to accept cstring and separate the string into multiple
//smaller strings, storing them in vector
void VariableMetric::tokenize(char * line)
{
    char* word = strtok(line, " ");
    DSString str;

    while (word != nullptr)
    {
        str = word;
        manageString(str);
        word = strtok(nullptr, " ");
    }
}

//function that accepts a string and removes punctuation,
//separating a string into two if necessary
void VariableMetric::manageString(DSString& str)
{
    //variable for index of parentheses;
    int index = 0;


    while (str[0] == '*')
    {
        str = str.substring(1, str.capacity());
        DSString newString (functionStrings[functionStrings.size() - 1]);
        newString = newString + "*";
        if(!functionStrings.isEmpty())
            functionStrings[functionStrings.size() - 1] = newString;
    }

    //if the string contains an open parentheses get the index
    //and separate into two strings
    if (str.contains("("))
    {
        //find the index number of the opening parentheses
        for (int i = 0; i < str.capacity(); i++)
        {
            if (str[i] == '(')
                index = i;
        }

        //if the opening brace is the first character skip splitting the string
        if (index > 0)
        {
            //get the first half of the funciton call
            DSString string1( str.substring(0, index));
            functionStrings.add(string1);
        }

        //get the string following the parantheses
        DSString string2( str.substring(index + 1, str.capacity()));
        functionStrings.add(string2);
    }

    //else if the string has a closing parantheses or a comma (remove it) and split the strings
    else if (str.contains(")") || str.contains(","))
    {
        //find the index
        for (int i = 0; i < str.capacity(); i++)
        {
            if (str[i] == ')' || str[i] == ',')
                index = i;
        }

        //split the strings into two if the parentheses or braces isn't the last one
        DSString string1( str.substring(0, index));
        functionStrings.add(string1);

        if (index < str.capacity() - 1 && str[index + 1] != '{')
        {
            DSString string2( str.substring(index + 1, str.capacity()));
            functionStrings.add(string2);
        }
    }

    //otherwise its its own string and add it
    else
        functionStrings.add(str);
}


//function that runs the first variable metric (initialized when declared)
void VariableMetric::runVM1()
{
    //process the score and get the output and add it to the metrics vector
    processVM1Score();
    DSString details(getVM1Details());
    MetricInfo metricinfo(fileVM1Score, details, 1);

    metrics.add(metricinfo);
}


//function returns the score for the first Variable Metric (Initialized when declared)
double VariableMetric::getVM1Score()
{
    return fileVM1Score;
}

/* Function that produces a string of the details of a file. In this case it shows how many
 * variables are declared vs how many are initialized.
 */
DSString VariableMetric::getVM1Details()
{
    DSString details("There were ");

    details = details + DSString::to_string(notInit) + DSString(" variables uninitialized out of ")
            + DSString::to_string(declared) + DSString(" variables declared.");

    return details;
}

/* Function that processes the score for the file that is calculated by dividing the number
 * of variables initialized when declared by the overall number declared.
 * If no variables were declared in the file, the score is set to -1 and will be ignored.
 */
void VariableMetric::processVM1Score()
{
    double percentage = (static_cast<double>(notInit) / static_cast<double>(declared));

    if (declared == 0)
        fileVM1Score = -1;
    else
        fileVM1Score = percentage * 100;
}

//function runs the second variable metric (variable naming)
void VariableMetric::runVM2()
{
    //process the score and output and add it to the metrics vector
    processVM2Score();
    DSString details(getVM2Details());
    MetricInfo metric2(fileVM2Score, details, 2);

    metrics.add(metric2);
}

/* Function loops through all of the variables from the file and runs a series
 * of tests on the variable names to produce a score for naming conventions
 *
 */
void VariableMetric::processVM2Score()
{
    //process the average length and number of variables of each size
    getAvgLength();
    //process the variables with numbers in them
    checkForNumbers();

    //get an even amount of points to subtract from the score;
    double magnitude = ( 1.0 / variableTotal * 100) * 0.5;

    //for every variable with a number multiply by magnitude and add to score
    fileVM2Score = 0.00;
    fileVM2Score = fileVM2Score + (magnitude * static_cast< double > (variablesWithNumbers));

    //if the average length is outside of the preferred average length add 15
    if (avgLength > 8 && avgLength < 20)
        fileVM2Score;
    else
        fileVM2Score = fileVM2Score + 15.0;

    //for every short and long variable add Magnitude to score
    fileVM2Score = fileVM2Score + (magnitude * static_cast<double>(shortVars));
    fileVM2Score = fileVM2Score + (magnitude * static_cast<double>(longVars));

    //score will be no greater than 100
    if (fileVM2Score > 100)
        fileVM2Score = 100.0;

    //no variables produces a null score
    if (declared == 0)
        fileVM2Score = -1;
}

//produces a string of a verbose version of what the file contained.
DSString VariableMetric::getVM2Details()
{
    DSString details;

    DSString first("The average length of a variable is ");
    first = first + DSString::to_string(avgLength) + " ";

    DSString second("and the number of variables\ncontaining numbers is ");
    second = second + DSString::to_string(variablesWithNumbers) + DSString(".");


    DSString third(" The total number of variables in the file is ");
    third = third + DSString::to_string(variableTotal) + DSString(".");

    //combine the strings to get the string of output
    details = first + second + third;

    return details;
}

//function that returns the file score
double VariableMetric::getVM2Score()
{
    return fileVM2Score;
}

//a function that compiles an average length
void VariableMetric::getAvgLength()
{
    //an integer to store total length
    int totalLength = 0;

    //loop through the variables vector and count the number of
    //small, average, and long variables and keep a running count
    //of their total length
    for (int i = 0; i < variables.size(); i++)
    {
        if (variables[i].getName().size() < 8)
            shortVars++;
        else if (variables[i].getName().size() >= 20)
            longVars++;
        else
            avgVars++;
        totalLength = totalLength + variables[i].getName().size();
    }

    //set the average of the string lengths by dividing total lenght by number of variables
    avgLength = static_cast<double>(totalLength) / static_cast<double>(variables.size());

}


//a function that checks each variable for numbers and counts the number of variables
//that contain numbers
void VariableMetric::checkForNumbers()
{
    for (int i = 0; i < variables.size(); i++)
    {

            if (findNumber(variables[i].getName()))
                variablesWithNumbers++;
    }
}

//function that loops through a string to see if it has a number
bool VariableMetric::findNumber(DSString& str)
{
    //for each character 0 - 9 check if it is in the string and return boolean
    for (char c = 48; c <= 57; c++)
    {
        for (int i = 0; i < str.capacity(); i++)
        {
            if (str[i] == c)
                return true;
        }
    }

    return false;
}
