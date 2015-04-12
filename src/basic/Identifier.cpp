
#include "openeaagles/basic/Identifier.h"
#include <cstring>

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(Identifier,"Identifier")
EMPTY_SLOTTABLE(Identifier)
EMPTY_COPYDATA(Identifier)
EMPTY_DELETEDATA(Identifier)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Identifier::Identifier() : String()
{
   STANDARD_CONSTRUCTOR()
}

Identifier::Identifier(const char* string) : String()
{
   STANDARD_CONSTRUCTOR()
   setStr(string);
}


//------------------------------------------------------------------------------
// Replace spaces with underscores, and then call our BaseClass::setStr()
//------------------------------------------------------------------------------
void Identifier::setStr(const char* string)
{
   size_t len = 0;
   if (string != nullptr) len = std::strlen(string);

   // copy the string and replace any spaces
   if (len > 0) {
      char* newStr = new char[len+1];
      for (unsigned int i = 0; i < len; i++) {
         if (string[i] == ' ') newStr[i] = '_';
         else newStr[i] = string[i];
      }
      newStr[len] = '\0';
      BaseClass::setStr(newStr);
      delete[] newStr;
   }

   // empty string ---
   else {
      BaseClass::setStr(string);
   }
}

//------------------------------------------------------------------------------
// Replace spaces with underscores, and then call our BaseClass::catStr()
//------------------------------------------------------------------------------
void Identifier::catStr(const char* s)
{
   // Early out if nothing to append
   if (s == nullptr) return;

   // If this string was empty then we're really just setStr()
   if ( isEmpty() ) {
      setStr(s);
      return;
   }

   // replace any spaces
   size_t len = std::strlen(s);
   char* newStr = new char[len+1];
   for (unsigned int i = 0; i < len; i++) {
      if (s[i] == ' ') newStr[i] = '_';
      else newStr[i] = s[i];
   }
   newStr[len] = '\0';
   BaseClass::catStr(newStr);
   delete[] newStr;
}

//------------------------------------------------------------------------------
// output stream operator
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& sout, const Identifier& ident)
{
    const char* s = ident;
    sout << s;
    return sout;
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Identifier::serialize(std::ostream& sout, const int, const bool) const
{
    const char* s = *this;
    sout << s;
    return sout;
}

} // End Basic namespace
} // End Eaagles namespace
