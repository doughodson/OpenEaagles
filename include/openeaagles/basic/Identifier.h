//------------------------------------------------------------------------------
// Class: Identifier
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Identifier_H__
#define __Eaagles_Basic_Identifier_H__

#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class: Identifier
// Description: A single unquoted word (usually from the input file), which
//              can contain upper and lower case letters, numbers, special
//              characters, but not spaces.
//
// Note: When setting the identifier, any spaces will be replaced
//       with underscores.
//
// Factory name: Identifier
//------------------------------------------------------------------------------
class Identifier : public String {
    DECLARE_SUBCLASS(Identifier,String)

public:
    Identifier(const char* string);
    Identifier();

    // Eaagles::Basic::String functions
    virtual void setStr(const char* string);        // Sets us to a copy of 'string'
    virtual void catStr(const char* string);        // Append a copy of 'string' to our text string
};

} // End Basic namespace
} // End Eaagles namespace

#endif
