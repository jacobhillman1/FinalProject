#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <dsstring.h>
#include <iostream>

using namespace std;

void printFirstLine(char* fileName)
{
    ifstream file;
    file.open(fileName);
    char line[400];
    int counter = 0;
    while (file.getline(line, 400))
    {
        counter++;
    }
    cout << "# of separate lines: " << counter << endl;
    cout << endl;
}

//function that checks if the file is one of the target files
//which are .cpp, .c, .hpp, or .h files.
bool targetFile(DSString& fileName)
{
    DSString sub;
    for (int i = fileName.size() - 1; i >= 0; i--)
    {
        if (fileName[i] == '.')
            sub = fileName.substring(i + 1, fileName.size());
    }
    if (sub == "cpp" || sub == "c" || sub == "hpp" || sub == "h")
        return true;
    else
        return false;

}

void traverse(char* dirName)
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
            DSString path = DSString( dirName ) + "/" + DSString( file->d_name );
            if (targetFile(path))
            {
                cout << "Path & file = " << path << endl;
                printFirstLine(path.c_str());
                filecount++;
            }

            char * charPath = path.c_str();
            //read the path information into the stat object
            stat( charPath, &fileInfo );
            //if the current file is a directory
            //recursively call the traverse function for the new directory.
            bool direct = S_ISDIR( fileInfo.st_mode );
            if ( S_ISDIR( fileInfo.st_mode ) )
            {
                traverse( charPath );
            }
        }
    }
    //close the directory
    closedir( directory );

    cout << "\n\nFile Count: " << filecount << endl;

}

using namespace std;

int main( int argc, char* const argv[] )
{
    traverse(argv[1]);
    return 0;
}

