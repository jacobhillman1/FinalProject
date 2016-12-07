/* The Variable class holds the name of a variable and information on whether or not it
 * is initialized and which line it was initialized and which line it was declared.
 *
 * Date: 11/26/2016
 * Authoer: Will Kastner
 */

#ifndef VARIABLE
#define VARIABLE

#include "dsstring.h"

class Variable
{
public:
    Variable();
    Variable(DSString, DSString,  bool, DSString, bool);
    DSString& getName();
    DSString getType();
    bool isInitializedWhenDeclared();
    bool getIsParam();
    void setInitializedWhenDeclared( bool );
    void setName( DSString );
    void setType( DSString );
    void setInitializedValue(DSString);
    void setIsParam(bool);
    DSString& getInitializedValue();
    bool operator>(const Variable&) const;

    friend std::ostream& operator<< (std::ostream&, const Variable&);

private:
    DSString name = "";
    DSString type = "";
    bool initializedWhenDeclared = false;
    DSString initializedValue = "";
    bool isParam = false;
};

#endif // VARIABLE

