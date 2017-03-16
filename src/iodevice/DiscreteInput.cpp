
#include "openeaagles/iodevice/DiscreteInput.hpp"

#include "openeaagles/base/io/IoData.hpp"
#include "openeaagles/base/io/IoDevice.hpp"
#include "openeaagles/base/io/IoHandler.hpp"
#include "openeaagles/base/Number.hpp"

#include <iostream>

namespace oe {
namespace iodevice {

IMPLEMENT_SUBCLASS(DiscreteInput, "DiscreteInput")
EMPTY_DELETEDATA(DiscreteInput)

BEGIN_SLOTTABLE(DiscreteInput)
    "di",         // 1) Discrete Input location (IoData's DI channel)
    "port",       // 2) Device port number (default: 0)
    "channel",    // 3) Device channel (bit) number on the port
    "value",      // 4) Initial value (default: false)
    "inverted",   // 5) Inverted bit flag (default: false)
    "num"         // 6) Number of DIs to manage starting at 'di' and 'channel'
END_SLOTTABLE(DiscreteInput)

BEGIN_SLOT_MAP(DiscreteInput)
    ON_SLOT( 1, setSlotLocation, base::Number)
    ON_SLOT( 2, setSlotPort,     base::Number)
    ON_SLOT( 3, setSlotChannel,  base::Number)
    ON_SLOT( 4, setSlotValue,    base::Number)
    ON_SLOT( 5, setSlotInverted, base::Number)
    ON_SLOT( 6, setSlotNum,      base::Number)
END_SLOT_MAP()

DiscreteInput::DiscreteInput()
{
   STANDARD_CONSTRUCTOR()
}

void DiscreteInput::copyData(const DiscreteInput& org, const bool)
{
   BaseClass::copyData(org);

   devEnb = org.devEnb;
   location = org.location;
   port = org.port;
   channel = org.channel;
   value = org.value;
   invert = org.invert;
   num = org.num;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
unsigned int DiscreteInput::getLocation() const
{
   return location;
}

unsigned int DiscreteInput::getPort() const
{
   return port;
}

unsigned int DiscreteInput::getChannel() const
{
   return channel;
}

bool DiscreteInput::getValue() const
{
   return value;
}

bool DiscreteInput::getInvertFlag() const
{
   return invert;
}

int DiscreteInput::getNumber() const
{
   return num;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool DiscreteInput::setLocation(const unsigned int v)
{
   location = v;
   return true;
}

bool DiscreteInput::setPort(const unsigned int v)
{
   devEnb = true;
   port = v;
   return true;
}

bool DiscreteInput::setChannel(const unsigned int v)
{
   devEnb = true;
   channel = v;
   return true;
}

bool DiscreteInput::setValue(const bool f)
{
   value = f;
   return true;
}

bool DiscreteInput::setInvertFlag(const bool f)
{
   invert = f;
   return true;
}

bool DiscreteInput::setNumber(const int n)
{
   num = n;
   return true;
}

//------------------------------------------------------------------------------
// process inputs
//------------------------------------------------------------------------------
void DiscreteInput::processInputs(const double dt, const base::IoDevice* const device, base::IoData* const inData)
{
   if (inData != nullptr) {
      unsigned int chan = channel;
      unsigned int loc  = location;
      unsigned int n = ((num >= 0) ? num : -num);

      for (unsigned int i = 0; i < n; i++) {

         // Get the bit from the DI card
         if (device != nullptr && devEnb) {
            device->getDiscreteInput(&value, chan, port);
         }

         // Set the bit to the cockpit input handler
         bool value0 = value;
         if (invert) value0 = !value;
         inData->setDiscreteInput(loc,value0);

         chan++;
         if (num >= 0) loc++;
         else if (loc > 0) loc--;
      }
   }
}

//------------------------------------------------------------------------------
// process outputs
//------------------------------------------------------------------------------
void DiscreteInput::processOutputs(const double, const base::IoData* const, base::IoDevice* const)
{
}


//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

// location: Input array index (location)
bool DiscreteInput::setSlotLocation(const base::Number* const msg)
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

// port: DiHandler's port number
bool DiscreteInput::setSlotPort(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setPort( static_cast<unsigned int>(v) );
      }
   }
   return ok;
}

// channel: DiHandler's channel (bit) number on the port
bool DiscreteInput::setSlotChannel(const base::Number* const msg)
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

// value: Initial value (default: false)
bool DiscreteInput::setSlotValue(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setValue( msg->getBoolean() );
   }
   return ok;
}

// invert: Inverted bit flag (default: false)
bool DiscreteInput::setSlotInverted(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setInvertFlag( msg->getBoolean() );
   }
   return ok;
}

bool DiscreteInput::setSlotNum(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setNumber( msg->getInt() );
   }
   return ok;
}

std::ostream& DiscreteInput::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "di: " << getLocation() << std::endl;

   indent(sout,i+j);
   sout << "port: " << getPort() << std::endl;

   indent(sout,i+j);
   sout << "channel: " << getChannel() << std::endl;

   indent(sout,i+j);
   sout << "value: " << getValue() << std::endl;

   indent(sout,i+j);
   sout << "inverted: " << getInvertFlag() << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

}
}
