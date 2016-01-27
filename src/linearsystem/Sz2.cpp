
#include "openeaagles/linearSys/Sz2.h"
#include "openeaagles/basic/Number.h"

namespace oe {
namespace linearSys {

//==============================================================================
// Class Sz2
//==============================================================================
IMPLEMENT_SUBCLASS(Sz2,"Sz2")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Sz2)
    "n1",      //  1: N1
    "N1",      //  2: N1
    "n2",      //  3: N2
    "N2",      //  4: N2
    "n3",      //  5: N2
    "N3",      //  6: N2
    "d1",      //  7: D1
    "D1",      //  8: D1
    "d2",      //  9: D2
    "D2",      // 10: D2
    "d3",      // 11: D2
    "D3",      // 12: D2
END_SLOTTABLE(Sz2)

// Map slot table to handles
BEGIN_SLOT_MAP(Sz2)
   ON_SLOT( 1, setSlotN1,   basic::Number)
   ON_SLOT( 2, setSlotN1,   basic::Number)
   ON_SLOT( 3, setSlotN2,   basic::Number)
   ON_SLOT( 4, setSlotN2,   basic::Number)
   ON_SLOT( 5, setSlotN3,   basic::Number)
   ON_SLOT( 6, setSlotN3,   basic::Number)
   ON_SLOT( 7, setSlotD1,   basic::Number)
   ON_SLOT( 8, setSlotD1,   basic::Number)
   ON_SLOT( 9, setSlotD2,   basic::Number)
   ON_SLOT(10, setSlotD2,   basic::Number)
   ON_SLOT(11, setSlotD3,   basic::Number)
   ON_SLOT(12, setSlotD3,   basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Sz2::Sz2()
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Sz2)
EMPTY_DELETEDATA(Sz2)


//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool Sz2::setSlotN1(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setN1( msg->getReal() );
      ok = true;
   }
   return ok;
}

bool Sz2::setSlotN2(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setN2( msg->getReal() );
      ok = true;
   }
   return ok;
}

bool Sz2::setSlotN3(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setN3( msg->getReal() );
      ok = true;
   }
   return ok;
}

bool Sz2::setSlotD1(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setD1( msg->getReal() );
      ok = true;
   }
   return ok;
}

bool Sz2::setSlotD2(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setD2( msg->getReal() );
      ok = true;
   }
   return ok;
}

bool Sz2::setSlotD3(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setD3( msg->getReal() );
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Sz2
//------------------------------------------------------------------------------
basic::Object* Sz2::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Sz2::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "N1:   " << getN1() << std::endl;

    indent(sout,i+j);
    sout << "N2:   " << getN2() << std::endl;

    indent(sout,i+j);
    sout << "N3:   " << getN3() << std::endl;

    indent(sout,i+j);
    sout << "D1:   " << getD1() << std::endl;

    indent(sout,i+j);
    sout << "D2:   " << getD2() << std::endl;

    indent(sout,i+j);
    sout << "D3:   " << getD3() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End linearSys namespace
} // End oe namespace
