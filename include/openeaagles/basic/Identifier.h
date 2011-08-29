//------------------------------------------------------------------------------
// Class:   Identifier
//------------------------------------------------------------------------------
#ifndef __Ident_H_31ABC4CE_29BE_4cea_8F07_996A91C0C54E__
#define __Ident_H_31ABC4CE_29BE_4cea_8F07_996A91C0C54E__

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
