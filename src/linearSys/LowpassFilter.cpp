
#include "openeaagles/linearSys/LowpassFilter.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/units/Frequencies.h"

namespace Eaagles {
namespace LinearSystem {

//==============================================================================
// Class LowpassFilter
//==============================================================================
IMPLEMENT_SUBCLASS(LowpassFilter,"LowpassFilter")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(LowpassFilter)
    "wc",      //  1: Cutoff Frequency 
    "wc",      //  1: Filer time constant (radians/sec)
END_SLOTTABLE(LowpassFilter)

// Map slot table to handles 
BEGIN_SLOT_MAP(LowpassFilter)
   ON_SLOT( 1, setSlotWc, Basic::Frequency)
   ON_SLOT( 1, setSlotWc, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
LowpassFilter::LowpassFilter()
{
   STANDARD_CONSTRUCTOR()
}

LowpassFilter::LowpassFilter(const unsigned int rate, const LCreal w)
                        : FirstOrderTf(rate, 0.0f, w, 1.0f, w)
{
   STANDARD_CONSTRUCTOR()

   wc = w;
}

EMPTY_COPYDATA(LowpassFilter)
EMPTY_DELETEDATA(LowpassFilter)

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool LowpassFilter::setWc(const LCreal v)
{
   wc = v;

   setN1(0.0f);
   setN2(wc);
   setD1(1.0f);
   setD2(wc);

   initialize();
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool LowpassFilter::setSlotWc(const Basic::Frequency* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal hz = Basic::Hertz::convertStatic(*msg);
      LCreal rps = (LCreal)( hz * 2.0 * PI);
      if (rps > 0) {
         setWc( rps );
         ok = true;
      }
      else {
         if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LowpassFilter::setSlotWc() -- Wc must be greater than zero!" << std::endl;
      }
   }
   }
   return ok;
}

bool LowpassFilter::setSlotWc(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal rps = msg->getReal();
      if (rps > 0) {
         setWc( rps );
         ok = true;
      }
      else {
         if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LowpassFilter::setSlotWc() -- Wc must be greater than zero!" << std::endl;
         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for LowpassFilter
//------------------------------------------------------------------------------
Basic::Object* LowpassFilter::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& LowpassFilter::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "wc:   " << getWc() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End LinearSystem namespace
} // End Eaagles namespace
