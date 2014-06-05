// Class: Ai2DiSwitch

#include "openeaagles/ioDevice/Ai2DiSwitch.h"

#include "openeaagles/basic/IoData.h"
#include "openeaagles/basic/IoDevice.h"
#include "openeaagles/basic/IoHandler.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace IoDevice {

//==============================================================================
// Ai2DiSwitch
//==============================================================================

IMPLEMENT_SUBCLASS(Ai2DiSwitch,"Ai2DiSwitch")

// slot table for this class type
BEGIN_SLOTTABLE(Ai2DiSwitch)
    "di",         // 1) Discrete Input location (IoData's DI channel)
    "channel",    // 2) Device's AI channel number
    "level",      // 3) Level to switch DI (default: 0)
    "inverted"    // 4) Inverted bit flag (default: false)
END_SLOTTABLE(Ai2DiSwitch)

//  Map slot table to handles
BEGIN_SLOT_MAP(Ai2DiSwitch)
    ON_SLOT( 1, setSlotLocation, Basic::Number)
    ON_SLOT( 2, setSlotChannel,  Basic::Number)
    ON_SLOT( 3, setSlotLevel,    Basic::Number)
    ON_SLOT( 4, setSlotInverted, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Ai2DiSwitch::Ai2DiSwitch()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// initData() -- init member data
//------------------------------------------------------------------------------
void Ai2DiSwitch::initData()
{
   devEnb = false;
   location = 0;
   channel = 0;
   level = false;
   invert = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Ai2DiSwitch::copyData(const Ai2DiSwitch& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) initData();

   devEnb = org.devEnb;
   location = org.location;
   channel = org.channel;
   level = org.level;
   invert = org.invert;
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void Ai2DiSwitch::deleteData()
{
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
unsigned int Ai2DiSwitch::getLocation() const
{
   return location;
}

unsigned int Ai2DiSwitch::getChannel() const
{
   return channel;
}

LCreal Ai2DiSwitch::getLevel() const
{
   return level;
}

bool Ai2DiSwitch::getInvertFlag() const
{
   return invert;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool Ai2DiSwitch::setLocation(const unsigned int v)
{
   location = v;
   return true;
}

bool Ai2DiSwitch::setChannel(const unsigned int v)
{
   channel = v;
   devEnb = true;
   return true;
}

bool Ai2DiSwitch::setLevel(const LCreal v)
{
   level = v;
   return true;
}

bool Ai2DiSwitch::setInvertFlag(const bool f)
{
   invert = f;
   return true;
}

//------------------------------------------------------------------------------
// process inputs
//------------------------------------------------------------------------------
void Ai2DiSwitch::processInputs(const LCreal, const Basic::IoDevice* const device, Basic::IoData* const inData)
{
   // Default is our initial value
   LCreal vin(0);

   // Get data from the AI device
   if (device != 0 && devEnb) {
      device->getAnalogInput(&vin, channel);
   }

   // Set the bit to the cockpit input handler
   if (inData != 0) {
      bool flag = (vin >= level);
      if (invert) flag = !flag;
      inData->setDiscreteInput(location,flag);
   }
}

//------------------------------------------------------------------------------
// process outputs
//------------------------------------------------------------------------------
void Ai2DiSwitch::processOutputs(const LCreal, const Basic::IoData* const, Basic::IoDevice* const)
{
}

//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

// location: Input array index (location)
bool Ai2DiSwitch::setSlotLocation(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setLocation( static_cast<unsigned int>(v) );
      }
   }
   return ok;
}

// channel: Device's AI channel number
bool Ai2DiSwitch::setSlotChannel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setChannel( static_cast<unsigned int>(v) );
      }
   }
   return ok;
}

// level: Level to switch DI (default: 0)
bool Ai2DiSwitch::setSlotLevel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setLevel( msg->getReal() );
   }
   return ok;
}

// invert: Inverted bit flag (default: false)
bool Ai2DiSwitch::setSlotInverted(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setInvertFlag( msg->getBoolean() );
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Basic::Object* Ai2DiSwitch::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Ai2DiSwitch::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "di: " << getLocation() << std::endl;

   indent(sout,i+j);
   sout << "channel: " << getChannel() << std::endl;

   indent(sout,i+j);
   sout << "level: " << getLevel() << std::endl;

   indent(sout,i+j);
   sout << "inverted: " << getInvertFlag() << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // end IoDevice
} // end Eaagles namespace
