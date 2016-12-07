/*Purpose: Source code for a customized string class that can convert cstrings into
 * DSString objects. Has overloaded +, =, ==, >, <<, and [] operators. Has functions
 * to get size and cstring, to convert to uppercase and lowercase, and to get a substring.
 * Author: Will Kastner
 * Date: 9/4/2016
 */

#include "dsstring.h"
#include <iostream>
#include <cmath>
#include <sstream>

using namespace std;

DSString::DSString(): data(nullptr), len(0), cap(0){}

DSString::DSString(int num){
    len = 0;
    cap = 0;
    data = nullptr;

    stringstream ss;
    char number[11];
    ss << num;
    ss >> number;

    len = strlen(number);
    cap = strlen(number) + 1;
    data = new char[cap];
    strcpy(data, number);
}

DSString::DSString(const char* rhs){
    if(rhs != nullptr)
    {
        len = strlen(rhs);
        cap = len + 1; //length and capacity of cstring (null terminator taken into account)

        data = new char[cap]; //set data to array the length of capacity

        strcpy(data, rhs);
    }
}

DSString::DSString(const DSString& rhs){
    if(rhs.data != nullptr){ //if parameter string is null skip assignments
        len = rhs.len; //change values of len and cap to respective rhs values
        cap = rhs.cap;
        data = new char[rhs.cap];
        for(int i = 0; i < cap; i++)
            data[i] = rhs.data[i]; //copy elements of rhs.data to data array
    }
}

DSString& DSString::operator= (const char *rhs){

    len = strlen(rhs); //length is equal to length of cstring
    cap = len + 1;

    delete[] data; //deallocated memory data points to

    data = new char[cap]; //set data to point to an array the size of cap

    strcpy(data, rhs);

    return *this;
}


DSString& DSString::operator= (const DSString& rhs){
    if(this != &rhs){ //avoid copying to itself
        if(data != nullptr){ //make sure data is empty and pointing to null pointer
            delete[] data; //deallocate memory pointed to by data
            data = nullptr;
        }
        if(rhs.data != nullptr){ //make sure rhs.data contains characters
            data = new char[rhs.cap]; //copy the information from rhs to this object
            cap = rhs.cap;
            len = rhs.len;
            for (int i = 0; i < cap; i++)
                data[i] = rhs.data[i];
        }
    }
    return *this;
}
DSString DSString::operator+ (const DSString& rhs){

    int newcap = rhs.cap + len; //create a new capacity of lengths combined
    char* temp = nullptr;
    temp = new char[newcap]; //create temporary char * array
    int elcount = 0; //initialize a counter for the elements in the temp array
    for(int i =0; i < (len); i++){ //len-1 so that nullterminator for left side is left
        temp[i] = data[i]; //copy object's chars into temp array
        elcount++; //move to next element in temp
    }
    for(int i= 0; i< rhs.cap; i++){
        temp[elcount] = rhs.data[i]; //copy rhs's chars into temp array
        elcount++;
    }
    DSString tempString(temp);
    delete[] temp;

    return tempString;

}

bool DSString::operator== (const char* rhs){
    DSString temp(rhs);
    if (temp.len != len) //if lengths aren't equal return false
        return false;
    for(int i = 0; i< len; i++){ //if any element in the string isn't equal return false
        if (data[i] != temp.data[i])
            return false;
    }
    return true;

}
bool DSString::operator== (const DSString& rhs){
    if (len != rhs.len) // if lengths are not equal return false
        return false;
    if (cap != rhs.cap) // if capacities are not equal return false
        return false;
    for (int i=0; i < cap; i++){ //if characters in string are not equal return false
        if (data[i] != rhs.data[i])
            return false;
        }
    return true; //else return true
}

bool DSString::operator !=(const char* rhs){
    DSString temp(rhs);
    if (temp.len != len) //if lengths aren't equal return true
        return true;
    for(int i = 0; i< len; i++){ //if any element in the string isn't equal return true
        if (data[i] != temp.data[i])
            return true;
    }
    return false;

}

bool DSString::operator !=(const DSString& rhs){
    if (len != rhs.len) // if lengths are not equal return true
        return false;
    if (cap != rhs.cap) // if capacities are not equal return true
        return false;
    for (int i=0; i < cap; i++){ //if characters in string are not equal return true
        if (data[i] != rhs.data[i])
            return false;
        }
    return true; //else return false

}

bool DSString::operator> (const DSString& rhs) const
{

    int loop;
    char letter1;
    char letter2;

    //assign loop the length of the longer object
    if(rhs.len > len)
        loop = rhs.len;
    else
        loop = len;

    //loop through object arrays until one object is greater than the other
    for(int i = 0; i < loop; i++)
    {
        //ignore cases
        letter1 = data[i];
        letter2 = rhs.data[i];
        if (data[i] >= 65 && data[i] <= 90)
            letter1 += 32;
        if (rhs.data[i] >= 65 && rhs.data[i] <= 90)
            letter2 += 32;

        // compare each letter
        if(letter1 > letter2)
            return true;
        else if (letter1 < letter2)
            return false;
    }
    return false;
}
char& DSString::operator[] (const int rhs){
    //if positive element position return data at that position
    if (rhs >= 0)
        return data[rhs];
    //if negative element position return data at length minus null terminator minus that value
    else
        return data[len-1+rhs];
}

DSString DSString::to_string(int num){ //read in integer and convert to string
    stringstream ss;
    char number[11];
    ss << num;
    ss >> number;
    DSString temp(number);
    return temp;
}

DSString DSString::to_string(double num){ //read in integer and convert to string
    stringstream ss;
    char number[11];
    ss << num;
    ss >> number;
    DSString temp(number);
    return temp;
}

int DSString::to_int(){ //return the integer value of String of number characters
    stringstream ss;
    int i = 0;
    ss.str(this->c_str());
    ss >> i;
    return i;
}

int DSString::size(){
    return len; //return length minus null terminator for size
}

DSString DSString::substring(int start, int end){
    char* sub = nullptr; //substring pointer
    int size = 0; //size of substring
    int count = 0; //used to keep track of element in main string

    //set size
    if (end < 0){
        if(start < 0){
            size = abs(start) + end + 1; //if negative start and negative end get
            count = len + start; // begin reading elements from string at 'start' values back from end
        }
        else{
            size = (len + end) - start + 1; //if positives start and negative end
            count = start; //begin reading elements from string at start value
        }
    }
    else{
        size = end - start + 1; //if start and end are positive: difference between the two
        count = start; //begin reading elements from string at start value
    }
    sub = new char[size]; //sub points to array of 'size' characters
    for (int i = 0; i < size; i++){
        sub[i] = data[count]; // set elements in sub to specific elements specified by substring start and end
        count++;
    }
    sub[size - 1] = '\0'; //add the null value to the end once elements are copied
    DSString substring(sub); //create a new object and store 'sub' string that copied original string
    delete[] sub;
    return substring;

}

char* DSString::c_str()
{
    return data; //return data string (basically a get function)
}

DSString& DSString::toUpper(){
    char* temp = nullptr;
    temp = new char[cap];//creates a temporary array of same size as objects data

    for (int i = 0; i < cap; i++){
        if((data[i] <= 122) && (data[i] >= 97)){ //if the character is a capital letter, turn it lowercase
            temp[i] = (data[i] - 32);
        }
        else
            temp[i] = data[i]; //else leave the character the same
    }
    *this = temp; //create an object to return the uppercase DSString
    delete[] temp;
    return *this;
}

DSString& DSString::toLower(){
    char* temp = nullptr;
    temp = new char[cap];//creates a temporary array of same size as objects data

    for (int i = 0; i < cap; i++){
        if((data[i] <= 90) && (data[i] >= 65)){// if the character is lowercase, turn it uppercase
            temp[i] = (data[i] + 32);
        }
        else
            temp[i] = data[i]; //else leave the characters alone
    }
    *this = temp; //create an object to return a lowercase DSString
    delete[] temp;
    return *this;
}

std::ostream& operator<< (std::ostream& output, const DSString& str){
    for (int i= 0; i < (str.len); i++) //go through each element of data & store in output
        output << str.data[i];
    return output; // return 'output' to be output in terminal
}

DSString::~DSString(){
    if (data != nullptr) //if the char array isn't empty
        delete[] data;  //deallocate dynamic memory
}

/*
 * Tests to see if the string object contains
 * the word passed as input (as a char[]). Only
 * searches for exact duplicates of complete words.
 * Is case sensitive.
 */
bool DSString::contains(char input[])
{
    int inputSize = strlen(input);
    DSString inputAsString(input);
    DSString tempThis(data);

    if(inputSize <= len)
    {
        for(int i = 0; i < strlen(data) - inputSize + 1; i++)
        {
            if(tempThis.substring(i, i + inputSize) == inputAsString)
                return true;
        }
    }

    return false;
}

bool DSString::contains(DSString input)
{
    int inputSize = input.size();
    DSString tempThis(data);

    for(int i = 0; i < this->size() - inputSize + 1; i++)
    {
        if(tempThis.substring(i, i + inputSize) == input)
            return true;
    }

    return false;
}

/*
 * Returns the index of the first character
 * of the first instance of word passed to the function
 * within the String. If the String does not contian
 * the word, return the len of the String (not recommended)
 */
int DSString::getLocOf(char input[])
{
    int inputSize = strlen(input);
    DSString inputAsString(input);
    DSString tempThis(data);

    if(inputSize <= len)
    {
        for(int i = 0; i < strlen(data) - inputSize + 1; i++)
        {
            if(tempThis.substring(i, i + inputSize) == inputAsString)
                return i;
        }
    }

    return len;
}

int DSString::capacity()
{
    return cap;
}

