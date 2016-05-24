
#include "openeaagles/linearsystem/ScalerFunc.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/units/Frequencies.h"

namespace oe {
namespace linearsystem {

//==============================================================================
// Class ScalerFunc
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(ScalerFunc,"ScalerFunc")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(ScalerFunc)
    "rate",   //  1: Master update rate
    "x0",     //  2: Initial (previous) input value: X(0)
    "y0",     //  3: Initial (previous) output value: Y(0)
END_SLOTTABLE(ScalerFunc)

// Map slot table to handles
BEGIN_SLOT_MAP(ScalerFunc)
   ON_SLOT( 1, setSlotRate, base::Frequency)
   ON_SLOT( 1, setSlotRate, base::Number)
   ON_SLOT( 2, setSlotX0,  base::Number)
   ON_SLOT( 3, setSlotY0, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
ScalerFunc::ScalerFunc()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

ScalerFunc::ScalerFunc(const unsigned int r)
{
   STANDARD_CONSTRUCTOR()

   initData();
   rate = r;
}

//------------------------------------------------------------------------------
// initData() -- init member data
//------------------------------------------------------------------------------
void ScalerFunc::initData()
{
   rate = 0;
   x0 = 0;
   y0 = 0;

   px = nullptr;
   py = nullptr;
   n = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ScalerFunc::copyData(const ScalerFunc& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   rate = org.rate;
   x0 = org.x0;
   y0 = org.y0;

   // copy data array -- derived classes must have called allocateMemory() prior
   // to calling this copyData() function.
   allocateMemory( org.n );
   if (n > 0) {
      for (unsigned int i = 0; i < n; i++) {
         px[i] = org.px[i];
         py[i] = org.py[i];
      }
   }
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void ScalerFunc::deleteData()
{
   // We clean up memory so that our derived classes don't need to.
   allocateMemory(0);
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool ScalerFunc::isValid() const
{
   return (px != nullptr && py != nullptr && n > 0 && BaseClass::isValid());
}

//------------------------------------------------------------------------------
// Initialize this control/filter
//------------------------------------------------------------------------------
void ScalerFunc::initialize()
{
}

//------------------------------------------------------------------------------
// Allocate memory arrays and free any old arrays
//------------------------------------------------------------------------------
void ScalerFunc::allocateMemory(unsigned int n0)
{
   if (n0 != n) {
      // Free the old memory arrays
      if (n > 0) {
         delete[] px;  px = nullptr;
         delete[] py;  py = nullptr;
         n = 0;
      }
      // Allocate the new space
      if (n0 > 0) {
         px = new double[n0];
         py = new double[n0];
         n = n0;
      }
   }
}

//------------------------------------------------------------------------------
// Clear the memory arrays
//------------------------------------------------------------------------------
void ScalerFunc::clearMemory()
{
   for (unsigned int i = 0; i < n; i++) {
      px[i] = 0;
      py[i] = 0;
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool ScalerFunc::setX0(const double v)
{
   x0 = v;
   if (px != nullptr) {
      for (unsigned int i = 0; i < n; i++) {
         px[i] = x0;
      }
   }
   return true;
}

bool ScalerFunc::setY0(const double v)
{
   y0 = v;
   if (py != nullptr) {
      for (unsigned int i = 0; i < n; i++) {
         py[i] = y0;
      }
   }
   return true;
}

bool ScalerFunc::setRate(const unsigned int v)
{
   bool ok = false;
   if (v > 0) {
      rate = v;
      initialize();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool ScalerFunc::setSlotRate(const base::Frequency* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = static_cast<int>( base::Hertz::convertStatic( *msg ) + 0.5f );
      if (v > 0) {
         setRate( static_cast<unsigned int>(v) );
         ok = true;
      }
   }
   return ok;
}

bool ScalerFunc::setSlotRate(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v > 0) {
         setRate( static_cast<unsigned int>(v) );
         ok = true;
      }
   }
   return ok;
}

bool ScalerFunc::setSlotX0(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setX0( msg->getReal() );
      ok = true;
   }
   return ok;
}

bool ScalerFunc::setSlotY0(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setY0( msg->getReal() );
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for ScalerFunc
//------------------------------------------------------------------------------
base::Object* ScalerFunc::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& ScalerFunc::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    if (rate > 0) {
       indent(sout,i+j);
       sout << "rate:   " << rate << std::endl;
    }

    if (x0 != 0) {
       indent(sout,i+j);
       sout << "x0:   " << x0 << std::endl;
    }

    if (y0 != 0) {
       indent(sout,i+j);
       sout << "y0:   " << y0 << std::endl;
    }

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
