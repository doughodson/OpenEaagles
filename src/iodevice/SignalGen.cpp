// Class: SignalGen

#include "openeaagles/iodevice/SignalGen.h"

#include "openeaagles/base/IoData.h"
#include "openeaagles/base/IoDevice.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Frequencies.h"

#include <cmath>

namespace oe {
namespace iodevice {

//==============================================================================
// SignalGen
//==============================================================================

IMPLEMENT_SUBCLASS(SignalGen,"SignalGenAI")

// slot table for this class type
BEGIN_SLOTTABLE(SignalGen)
    "signal",     // 1) Signal type { SINE, COSINE, SQUARE, SAW }
    "frequency",  // 2) Signal frequency
    "phase",      // 3) Phase shift)
    "ai",         // 4) Analog Input location (IoData's AI channel)
    "channel",    // 5) Device channel number
END_SLOTTABLE(SignalGen)

//  Map slot table to handles
BEGIN_SLOT_MAP(SignalGen)
   ON_SLOT( 1, setSlotSignal,    base::String)
   ON_SLOT( 2, setSlotFrequency, base::Frequency)
   ON_SLOT( 3, setSlotPhase,     base::Angle)
   ON_SLOT( 4, setSlotLocation, base::Number)
   ON_SLOT( 5, setSlotChannel,  base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SignalGen::SignalGen()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// initData() -- init member data
//------------------------------------------------------------------------------
void SignalGen::initData()
{
   location = 0;
   channel = 0;

   signal = SINE;
   phase = 0;
   freq = 0;
   time = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SignalGen::copyData(const SignalGen& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) initData();

   signal = org.signal;
   phase = org.phase;
   freq = org.freq;
   time = org.time;
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void SignalGen::deleteData()
{
}

//------------------------------------------------------------------------------
// reset() --
//------------------------------------------------------------------------------
void SignalGen::reset()
{
   time = 0;
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
SignalGen::Signal SignalGen::getSignalType() const
{
   return signal;
}

double SignalGen::getPhase() const
{
   return phase;
}

double SignalGen::getFrequency() const
{
   return freq;
}

unsigned int SignalGen::getLocation() const
{
   return location;
}

unsigned int SignalGen::getChannel() const
{
   return channel;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
bool SignalGen::setSignalType(const Signal s)
{
   signal = s;
   return true;
}

bool SignalGen::setPhase(const double p)
{
   phase = p;
   return true;
}

bool SignalGen::setFrequency(const double f)
{
   freq = f;
   return true;
}

bool SignalGen::setLocation(const unsigned int v)
{
   location = v;
   return true;
}

bool SignalGen::setChannel(const unsigned int v)
{
   channel = v;
   return true;
}


//------------------------------------------------------------------------------
// process inputs
//------------------------------------------------------------------------------
void SignalGen::processInputs(const double dt, const base::IoDevice* const, base::IoData* const inData)
{
   double value = calc(dt);

   // Send the value to the input data buffer
   if (inData != nullptr) {
      inData->setAnalogInput(location,value);
   }
}

//------------------------------------------------------------------------------
// process outputs
//------------------------------------------------------------------------------
void SignalGen::processOutputs(const double dt, const base::IoData* const, base::IoDevice* const device)
{
   double value = calc(dt);

   // Send the value to the AO card
   if (device != nullptr) {
      device->setAnalogOutput(value, channel);
   }
}

//------------------------------------------------------------------------------
// Generate an input value (e.g., from the I/O device
//------------------------------------------------------------------------------
double SignalGen::calc(const double dt)
{
   // Default AI input value
   double value = 0;

   // Update time since reset
   time += dt;

   // Current angle (radians)
   double alpha = (2.0 * PI * freq * time) + phase;

   // Local cycle (-PI to PI)
   double beta = base::Angle::aepcdRad(alpha);

   switch (signal) {
      case SINE : {
         value = std::sin(beta);
         break;
      }
      case COSINE : {
         value = std::cos(beta);
         break;
      }
      case SQUARE : {
         if (beta > 0 && beta < PI) value = 1.0;
         else value = 0.0;
         break;
      }
      case SAW : {
         value = (beta/PI) + 1.0;
         break;
      }
   }

   if (value > 1.0) value = 1.0;
   else if (value < -1.0) value = -1.0;

   return static_cast<double>(value);
}

//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

// signal: Signal type { SINE, COSINE, SQUARE, SAW }
bool SignalGen::setSlotSignal(const base::String* const msg)
{
   bool ok = false;
   if (msg != nullptr) {

      if (*msg == "SINE" || *msg == "sine")          ok = setSignalType(SINE);
      else if (*msg == "COSINE" || *msg == "cosine") ok = setSignalType(COSINE);
      else if (*msg == "SQUARE" || *msg == "square") ok = setSignalType(SQUARE);
      else if (*msg == "SAW" || *msg == "saw")       ok = setSignalType(SAW);

      if (!ok && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "SignalGen::setSlotSignal(): Invalid signal type: " << *msg;
         std::cerr << ", use: { SINE, COSINE, SQUARE, SAW }" << std::endl;
      }

   }
   return ok;
}

// frequency: frequency
bool SignalGen::setSlotFrequency(const base::Frequency* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setFrequency( base::Hertz::convertStatic(*msg) );
   }
   return ok;
}

// phase: Phase shift
bool SignalGen::setSlotPhase(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setPhase( static_cast<double>(base::Radians::convertStatic(*msg)) );
   }
   return ok;
}

// ai: IoData's AI channel (AI signal generator only)
bool SignalGen::setSlotLocation(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setLocation( static_cast<unsigned int>(v) );
      }
   }
   return ok;
}

// channel: Output device channel number (AO signal generator only)
bool SignalGen::setSlotChannel(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setChannel( static_cast<unsigned int>(v) );
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
base::Object* SignalGen::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& SignalGen::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "signal: ";
   switch ( getSignalType() ) {
      case SINE : sout << "SINE"; break;
      case COSINE : sout << "COSINE"; break;
      case SQUARE : sout << "SQUARE"; break;
      case SAW : sout << "SAW"; break;
      default : sout << "XXXX"; break;
   }
   sout << std::endl;

   indent(sout,i+j);
   sout << "frequency: ( Hertz " << getFrequency() << " )" << std::endl;

   indent(sout,i+j);
   sout << "phase: ( Radians " << getPhase() << " )" << std::endl;

   indent(sout,i+j);
   sout << "ai: " << getLocation() << std::endl;

   indent(sout,i+j);
   sout << "channel: " << getChannel() << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // end iodevice
} // end oe namespace
