// Class: DiscreteOutput

#include "openeaagles/iodevice/DiscreteOutput.h"

#include "openeaagles/basic/IoData.h"
#include "openeaagles/basic/IoDevice.h"
#include "openeaagles/basic/IoHandler.h"
#include "openeaagles/basic/Number.h"

namespace oe {
namespace iodevice {

//==============================================================================
// DiscreteOutput
//==============================================================================

IMPLEMENT_SUBCLASS(DiscreteOutput,"DiscreteOutput")

// slot table for this class type
BEGIN_SLOTTABLE(DiscreteOutput)
    "do",         // 1) Discrete Output location (IoData's DO channel)
    "port",       // 2) Device port number (default: 0)
    "channel",    // 3) Device channel (bit) number on the port
    "value",      // 4) Initial value (default: false)
    "inverted",   // 5) Inverted bit flag (default: false)
    "num"         // 6) Number of DOs to manage starting at 'do' and 'channel'
END_SLOTTABLE(DiscreteOutput)

//  Map slot table to handles
BEGIN_SLOT_MAP(DiscreteOutput)
    ON_SLOT( 1, setSlotLocation, basic::Number)
    ON_SLOT( 2, setSlotPort,     basic::Number)
    ON_SLOT( 3, setSlotChannel,  basic::Number)
    ON_SLOT( 4, setSlotValue,    basic::Number)
    ON_SLOT( 5, setSlotInverted, basic::Number)
    ON_SLOT( 6, setSlotNum,      basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
DiscreteOutput::DiscreteOutput()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void DiscreteOutput::initData()
{
   devEnb = false;
   location = 0;
   port = 0;
   channel = 0;
   value = false;
   invert = false;
   num = 1;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DiscreteOutput::copyData(const DiscreteOutput& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   devEnb = org.devEnb;
   location = org.location;
   port = org.port;
   channel = org.channel;
   value = org.value;
   invert = org.invert;
   num = org.num;
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void DiscreteOutput::deleteData()
{
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
unsigned int DiscreteOutput::getLocation() const
{
   return location;
}

unsigned int DiscreteOutput::getPort() const
{
   return port;
}

unsigned int DiscreteOutput::getChannel() const
{
   return channel;
}

bool DiscreteOutput::getValue() const
{
   return value;
}

bool DiscreteOutput::getInvertFlag() const
{
   return invert;
}

int DiscreteOutput::getNumber() const
{
   return num;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool DiscreteOutput::setLocation(const unsigned int v)
{
   location = v;
   return true;
}

bool DiscreteOutput::setPort(const unsigned int v)
{
   port = v;
   devEnb = true;
   return true;
}

bool DiscreteOutput::setChannel(const unsigned int v)
{
   channel = v;
   devEnb = true;
   return true;
}

bool DiscreteOutput::setValue(const bool f)
{
   value = f;
   return true;
}

bool DiscreteOutput::setInvertFlag(const bool f)
{
   invert = f;
   return true;
}

bool DiscreteOutput::setNumber(const int n)
{
   num = n;
   return true;
}

//------------------------------------------------------------------------------
// process inputs
//------------------------------------------------------------------------------
void DiscreteOutput::processInputs(const LCreal, const basic::IoDevice* const, basic::IoData* const)
{
}

//------------------------------------------------------------------------------
// process outputs
//------------------------------------------------------------------------------
void DiscreteOutput::processOutputs(const LCreal, const basic::IoData* const outData, basic::IoDevice* const device)
{
   if (device != nullptr && devEnb) {
      unsigned int chan = channel;
      unsigned int loc  = location;
      unsigned int n = ((num >= 0) ? num : -num);

      for (unsigned int i = 0; i < n; i++) {

         // Get the bit from the cockpit output data handler
         if (outData != nullptr) {
            outData->getDiscreteOutput(loc, &value);
         }

         // Send the bit to the DO card
         bool value0 = value;
         if (invert) value0 = !value;
         device->setDiscreteOutput(value0, chan, port);

         chan++;
         if (num >= 0) loc++;
         else if (loc > 0) loc--;
      }
   }
}


//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

// location: Output array index (location)
bool DiscreteOutput::setSlotLocation(const basic::Number* const msg)
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
bool DiscreteOutput::setSlotPort(const basic::Number* const msg)
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
bool DiscreteOutput::setSlotChannel(const basic::Number* const msg)
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
bool DiscreteOutput::setSlotValue(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setValue( msg->getBoolean() );
   }
   return ok;
}

// invert: Inverted bit flag (default: false)
bool DiscreteOutput::setSlotInverted(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setInvertFlag( msg->getBoolean() );
   }
   return ok;
}

bool DiscreteOutput::setSlotNum(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setNumber( msg->getInt() );
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
basic::Object* DiscreteOutput::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& DiscreteOutput::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "do: " << getLocation() << std::endl;

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

} // end iodevice
} // end oe namespace
