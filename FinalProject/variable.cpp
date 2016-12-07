#include "variable.h"

using namespace std;

//default constructor
Variable::Variable()
{

}

//constructor that takes the name, whether it is intitialized and the line it was declared
Variable::Variable(DSString n, DSString t, bool init, DSString value, bool parameter)
{
    name = n;
    if (name[name.size()] == ';')
    {
        name = name.substring(0, name.size());
    }
    type = t;
    initializedWhenDeclared = init;
    initializedValue = value;
    isParam = parameter;

}

//returns the name of the variable
DSString& Variable::getName()
{
    return name;
}

//returns the type of the variable
DSString Variable::getType()
{
    return type;
}

//returns if the variable is a parameter
bool Variable::getIsParam()
{
    return isParam;
}

//sets the isParam value
void Variable::setIsParam(bool value)
{
    isParam = value;
}

//checks if the variable is initialized
bool Variable::isInitializedWhenDeclared()
{
    return initializedWhenDeclared;
}

//set funciton for the boolean intialized
void Variable::setInitializedWhenDeclared(bool boolean)
{
    initializedWhenDeclared = boolean;
}

//function that returns the value of the variable when initialized
DSString& Variable::getInitializedValue()
{
    return initializedValue;
}

//overloaded greater than operator that compares variable names
bool Variable::operator >(const Variable& rhs) const
{
    if (name > rhs.name)
        return true;
    else
        return false;
}

//set name function that removes semi colons
void Variable::setName(DSString newName)
{
    name = newName;
    if (name[name.size()] == ';')
    {
        name = name.substring(0, name.size());
    }
}

//set type function
void Variable::setType(DSString newType)
{
    type = newType;
}

//set initialized value funciton
void Variable::setInitializedValue(DSString newValue)
{
    initializedValue = newValue;
}

//overloaded stream insertion operator that prints the type and name and intialized value
std::ostream& operator<< (std::ostream& output, const Variable& var)
{
    output << var.type << " " << var.name;
    if (var.initializedWhenDeclared)
        output << " = " << var.initializedValue << endl;
    else
        output << ";" << endl;

    return output;
}


