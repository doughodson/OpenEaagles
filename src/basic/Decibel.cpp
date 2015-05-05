//------------------------------------------------------------------------------
// Decibel
//------------------------------------------------------------------------------
#include "openeaagles/basic/Decibel.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Decibel,"dB")

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
Decibel::Decibel()
{
   db = 0.0;
   STANDARD_CONSTRUCTOR()
}

Decibel::Decibel(const LCreal value)
{
   STANDARD_CONSTRUCTOR()
   setValueDB(value);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Decibel::copyData(const Decibel& org, const bool)
{
   BaseClass::copyData(org);
   db = org.db;
}

EMPTY_DELETEDATA(Decibel)


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
void Decibel::setValueDB(const LCreal v)
{
   db = v;
   val = lcPow( static_cast<LCreal>(10.0), static_cast<LCreal>(db/10.0) );
}

void Decibel::setValue(const double v)
{
   BaseClass::setValue(v);
   db = lcLog10(static_cast<LCreal>(v)) * 10.0f;
}

//------------------------------------------------------------------------------
// slot functions
//------------------------------------------------------------------------------
bool Decibel::setSlotValue(const Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setValueDB( msg->getReal() );
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Decibel::serialize(std::ostream& sout, const int, const bool) const
{
   sout << db;
   return sout;
}

} // End Basic namespace
} // End Eaagles namespace
