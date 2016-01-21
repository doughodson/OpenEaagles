// Class: SignalGen

#include "openeaagles/ioDevice/SignalGen.h"

#include "openeaagles/basic/IoData.h"
#include "openeaagles/basic/IoDevice.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Frequencies.h"

#include <cmath>

namespace oe {
namespace IoDevice {

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
   ON_SLOT( 1, setSlotSignal,    Basic::String)
   ON_SLOT( 2, setSlotFrequency, Basic::Frequency)
   ON_SLOT( 3, setSlotPhase,     Basic::Angle)
   ON_SLOT( 4, setSlotLocation, Basic::Number)
   ON_SLOT( 5, setSlotChannel,  Basic::Number)
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

LCreal SignalGen::getPhase() const
{
   return phase;
}

LCreal SignalGen::getFrequency() const
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

bool SignalGen::setPhase(const LCreal p)
{
   phase = p;
   return true;
}

bool SignalGen::setFrequency(const LCreal f)
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
void SignalGen::processInputs(const LCreal dt, const Basic::IoDevice* const, Basic::IoData* const inData)
{
   LCreal value = calc(dt);

   // Send the value to the input data buffer
   if (inData != nullptr) {
      inData->setAnalogInput(location,value);
   }
}

//------------------------------------------------------------------------------
// process outputs
//------------------------------------------------------------------------------
void SignalGen::processOutputs(const LCreal dt, const Basic::IoData* const, Basic::IoDevice* const device)
{
   LCreal value = calc(dt);

   // Send the value to the AO card
   if (device != nullptr) {
      device->setAnalogOutput(value, channel);
   }
}

//------------------------------------------------------------------------------
// Generate an input value (e.g., from the I/O device
//------------------------------------------------------------------------------
LCreal SignalGen::calc(const LCreal dt)
{
   // Default AI input value
   double value = 0;

   // Update time since reset
   time += dt;

   // Current angle (radians)
   double alpha = (2.0 * PI * freq * time) + phase;

   // Local cycle (-PI to PI)
   double beta = Basic::Angle::aepcdRad(alpha);

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

   return static_cast<LCreal>(value);
}

//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

// signal: Signal type { SINE, COSINE, SQUARE, SAW }
bool SignalGen::setSlotSignal(const Basic::String* const msg)
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
bool SignalGen::setSlotFrequency(const Basic::Frequency* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setFrequency( Basic::Hertz::convertStatic(*msg) );
   }
   return ok;
}

// phase: Phase shift
bool SignalGen::setSlotPhase(const Basic::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setPhase( static_cast<LCreal>(Basic::Radians::convertStatic(*msg)) );
   }
   return ok;
}

// ai: IoData's AI channel (AI signal generator only)
bool SignalGen::setSlotLocation(const Basic::Number* const msg)
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
bool SignalGen::setSlotChannel(const Basic::Number* const msg)
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
Basic::Object* SignalGen::getSlotByIndex(const int si)
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

} // end IoDevice
} // end oe namespace
