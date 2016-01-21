//------------------------------------------------------------------------------
// Class: Identifier
//------------------------------------------------------------------------------
#ifndef __oe_basic_Identifier_H__
#define __oe_basic_Identifier_H__

#include "openeaagles/basic/String.h"

namespace oe {
namespace basic {

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

    void setStr(const char* string) override;
    void catStr(const char* string) override;
};

} // End basic namespace
} // End oe namespace

#endif
