/* Purpose: This is the header file for a customized String class
 * Author: Will Kastner
 * Date: 9/4/2016
 */

#ifndef STRING
#define STRING

#include <iostream>
#include <cstring>

class DSString{
    private:
        char* data = nullptr;//the char pointer used to point to char array
        int len;// the number of chars the string is using
        int cap;//the total number of char the string can hold
    public:

        DSString();
        DSString(int);
        DSString(const char*);
        DSString(const DSString&);

        DSString& operator= (const char*);
        DSString& operator= (const DSString&);
        DSString operator+ (const DSString&);
        bool operator== (const char*);
        bool operator== (const DSString&);
        bool operator!= (const char*);
        bool operator!= (const DSString&);
        bool operator> (const DSString&) const;
        char& operator[] (const int);
        static DSString to_string(int);
        static DSString to_string(double);
        int to_int();

        int size();
        int capacity();
        DSString substring(int, int);
        char* c_str();
        DSString& toUpper();
        DSString& toLower();
        bool contains(char[]);
        bool contains(DSString);
        int getLocOf(char[]);

        friend std::ostream& operator<< (std::ostream&, const DSString&);

        ~DSString();

};


#endif

