#ifndef DIRECTORYPARSER_H
#define DIRECTORYPARSER_H

#include <dirent.h>
#include <sys/stat.h>
#include "dsstring.h"
#include <fstream>
#include "hashtable.h"
#include "file.h"
#include "commentmetric.h"
#include "indentationmetric.h"
#include "variablemetric.h"
#include "couplingmetric.h"

using namespace std;

class DirectoryParser
{
public:
    DirectoryParser(char* );
    void runMetrics();
    void outputGoodness(char*, char* );


private:
    HashTable<DSString, File> files;
    DSVector<DSString> paths;
    DSVector<DSString> classes;
    double briefScore = 0.0;
    int numberOfFiles = 0;

    void briefOutput(ofstream& );
    void verboseOutput(ofstream& );
    void processMetricsForFile(File& );
    void processScore();
    void collectClasses(char*);
    void testClasses();
    void traverse(char* );
    bool targetFile(DSString&);
    void processFiles(char *, File &);
};

#endif // DIRECTORYPARSER_H
