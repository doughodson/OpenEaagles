//------------------------------------------------------------------------------
// Class: Identifier
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Identifier_H__
#define __Eaagles_Basic_Identifier_H__

#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:   Identifier
// Base class:  Object -> String -> Identifier
//
// Form name: Identifier
// Description: Identifier.
//              A single unquoted word, which starts with a letter.
//              Usually from the input file.
//------------------------------------------------------------------------------
class Identifier : public String {
    DECLARE_SUBCLASS(Identifier,String)

public:
    Identifier(const char* string) : String(string)  { STANDARD_CONSTRUCTOR() }
    Identifier()                   : String()        { STANDARD_CONSTRUCTOR() }
};

} // End Basic namespace
} // End Eaagles namespace

#endif
