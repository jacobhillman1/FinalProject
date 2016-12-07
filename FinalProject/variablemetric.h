/* The VariableMetric class parses a file and extracts all of the variable information
 * and calculates the scores and outputs for both the Variable Naming metric and the
 * Initialized Variable metric. Inherits the runMetric function from the abstract metric class.
 *
 * Author: Will Kastner & Jacob Hillman
 *
 */

#ifndef INITIALIZED_VAR_METRIC_H
#define INITIALIZED_VAR_METRIC_H

#include <fstream>
#include "dsvector.h"
#include "dsstring.h"
#include "variable.h"
#include "metricinfo.h"
#include "abstractmetric.h"
#include "avltree.h"
#include <iomanip>

using namespace std;

class VariableMetric: public AbstractMetric
{
public:

    VariableMetric();
    DSVector<MetricInfo> &runMetric(istream&);
    void printVariables();

private:

    bool checkType(char*);
    int getDeclared();
    int getNotInit();
    void removeAsterisk(istream&, DSString&, char* , char *);
    DSString& removeBraces(DSString &);
    bool secondRead(istream&, char*, char*, char*, char*);
    void thirdRead(istream&, char*, char*, char*);
    void addVariable(char*, char*, bool, char*);
    void addVariables(char*, bool, char*);
    void removePostCharacter(DSString&, istream&, char *, char);
    void processParameters(char* );
    void tokenize(char* );
    void manageString(DSString&);

    void processVM1Score();
    DSString getVM1Details();
    double getVM1Score();
    void processVM2Score();
    DSString getVM2Details();
    double getVM2Score();
    void getAvgLength();
    void checkForNumbers();
    bool findNumber(DSString&);

    void runVM1();
    void runVM2();

    double fileVM1Score = 0;
    int declared = 0;
    int notInit = 0;

    int variableTotal = 0;
    int numParams = 0;

    double fileVM2Score = 0;
    int shortVars = 0;
    int avgVars = 0;
    int longVars = 0;
    int variablesWithNumbers = 0;
    double avgLength = 0;

    AvlTree<DSString> types;
    DSVector<DSString> multiple;
    DSVector<DSString> functionStrings;
    DSVector<Variable> variables;
    DSVector<MetricInfo> metrics;
};

#endif // INITIALIZED_VAR_METRIC_H
