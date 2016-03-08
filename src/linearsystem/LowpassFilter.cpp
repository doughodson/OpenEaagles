
#include "openeaagles/linearsystem/LowpassFilter.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/units/Frequencies.h"

namespace oe {
namespace linearsystem {

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
   ON_SLOT( 1, setSlotWc, base::Frequency)
   ON_SLOT( 1, setSlotWc, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
LowpassFilter::LowpassFilter() : wc(0.0)
{
   STANDARD_CONSTRUCTOR()
}

LowpassFilter::LowpassFilter(const unsigned int rate, const double w)
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

bool LowpassFilter::setWc(const double v)
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

bool LowpassFilter::setSlotWc(const base::Frequency* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      double hz = base::Hertz::convertStatic(*msg);
      double rps = static_cast<double>( hz * 2.0 * base::PI);
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

bool LowpassFilter::setSlotWc(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      double rps = msg->getReal();
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
base::Object* LowpassFilter::getSlotByIndex(const int si)
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

} // End linearsystem namespace
} // End oe namespace
