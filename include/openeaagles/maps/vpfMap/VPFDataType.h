// -------------------------------------------------------------------------------
// Class: VPFDataType
//
// Description: Simple object which keeps track of data type, length, and value
// of our table.
// -------------------------------------------------------------------------------
#ifndef __Eaagles_Maps_Vpf_VPFDataType_H__
#define __Eaagles_Maps_Vpf_VPFDataType_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic { class String; }
namespace Maps {
namespace Vpf {

class VPFDataType : public Basic::Object
{
   DECLARE_SUBCLASS(VPFDataType,Basic::Object)

public:
    VPFDataType();

    enum { LONG_INT, SHORT_INT, CHAR, FIXED_TEXT, VARIABLE_TEXT, FLOAT, DOUBLE, THREE_D_COORD, DATE };

    // Set functions 
    virtual void setLength(const int x = -1);
    // Value sets
    virtual void setValue(char* x);
    virtual void setType(const int x)       { type = x; }

    // Get functions
    int getType()   { return type; }
    int getLength() { return length; }
    const char* getValue();

    float getFloat();

private:
    int type;               // Type of data we are using
    int length;             // Length of the data we are using (number of values, not byte length)
                            // A length of 1 would be an integer value of one digit, not one sizeof(int);
    Basic::String* value;   // This will hold the value type until we need it
};

} // End Vpf namespace
} // End Maps namespace 
} // End Eaagles namespace

#endif

