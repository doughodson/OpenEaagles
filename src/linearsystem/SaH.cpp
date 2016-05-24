
#include "openeaagles/linearsystem/SaH.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/units/Frequencies.h"

namespace oe {
namespace linearsystem {

//==============================================================================
// Class SaH
//==============================================================================
IMPLEMENT_SUBCLASS(SaH,"SaH")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(SaH)
    "sampleRate",   //  1: Sample update rate
END_SLOTTABLE(SaH)

// Map slot table to handles
BEGIN_SLOT_MAP(SaH)
   ON_SLOT( 1, setSlotSampleRate, base::Frequency)
   ON_SLOT( 1, setSlotSampleRate, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SaH::SaH()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

SaH::SaH(const unsigned int r, const unsigned int sr) : ScalerFunc(r)
{
   STANDARD_CONSTRUCTOR()

   initData();
   sampleRate = sr;

   initialize();
}

//------------------------------------------------------------------------------
// initData() -- init member data
//------------------------------------------------------------------------------
void SaH::initData()
{
   sampleRate = 0;
   time = 0;
   stime = 0;

   allocateMemory(ORDER);
   clearMemory();
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SaH::copyData(const SaH& org, const bool cc)
{
   if (cc) {
      // allocate memory before calling BaseClass::copyData()
      allocateMemory(ORDER);
   }

   BaseClass::copyData(org);

   sampleRate = org.sampleRate;
   time = org.time;
   stime = org.stime;

   initialize();
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void SaH::deleteData()
{
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool SaH::isValid() const
{
   // Valid if we have a master rate and a sample rate
   bool valid = (
         n == ORDER &&
         rate > 0 &&
         sampleRate > 0
      );

   return valid && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// g() is one iteration of the difference equation.
//------------------------------------------------------------------------------
double SaH::g(const double xn)
{
   if (isValid()) {

      // Current input
      px[0] = xn;

      // Update sample timer
      time += 1.0f/rate;

      // Take a sample
      if (time >= stime) {
         py[0] = px[0];
         time = 0;
      }

      return py[0];
   }
   else {
      // If invalid transfer function, just return the input value
      return xn;
   }
}

//------------------------------------------------------------------------------
// initialize the system
//------------------------------------------------------------------------------
void SaH::initialize()
{
   BaseClass::initialize();

   if (isValid()) {
      // Init the past values
      px[0] = x0;
      py[0] = y0;

      // Compute sample time
      stime = 1.0f/sampleRate;

      // Age of the sample data: set beyond max to force a sample at first call to g()
      time = stime + 1.0f;
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool SaH::setSampleRate(const unsigned int v)
{
   sampleRate = v;
   initialize();
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool SaH::setSlotSampleRate(const base::Frequency* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = static_cast<int>( base::Hertz::convertStatic( *msg ) + 0.5f );
      if (v > 0) {
         setSampleRate( static_cast<unsigned int>(v) );
         ok = true;
      }
   }
   return ok;
}

bool SaH::setSlotSampleRate(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v > 0) {
         setSampleRate( static_cast<unsigned int>(v) );
         ok = true;
      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for SaH
//------------------------------------------------------------------------------
base::Object* SaH::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& SaH::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    //indent(sout,i+j);
    //sout << "N1:   " << n1 << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
