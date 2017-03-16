
#include "openeaagles/iodevice/AnalogInput.hpp"

#include "openeaagles/base/io/IoData.hpp"
#include "openeaagles/base/io/IoDevice.hpp"
#include "openeaagles/base/io/IoHandler.hpp"

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/functors/Tables.hpp"
#include <iostream>

namespace oe {
namespace iodevice {

IMPLEMENT_SUBCLASS(AnalogInput, "AnalogInput")

BEGIN_SLOTTABLE(AnalogInput)
    "ai",         // 1) Analog Input location (IoData's AI channel)
    "channel",    // 2) Device channel number
    "value",      // 3) Initial value [ -1.0 ... 1.0 ] (default: 0)
    "deadband",   // 4) Deadband: [ 0 .. 1 ] (default: 0.0)
    "offset",     // 5) Offset value (default: 0.0)
    "gain",       // 6) Gain value   (default: 1.0)
    "table"       // 7) Shaping function table (default: none)
END_SLOTTABLE(AnalogInput)

BEGIN_SLOT_MAP(AnalogInput)
    ON_SLOT( 1, setSlotLocation, base::Number)
    ON_SLOT( 2, setSlotChannel,  base::Number)
    ON_SLOT( 3, setSlotValue,    base::Number)
    ON_SLOT( 4, setSlotDeadband, base::Number)
    ON_SLOT( 5, setSlotOffset,   base::Number)
    ON_SLOT( 6, setSlotGain,     base::Number)
    ON_SLOT( 7, setTable,        base::Table1)
END_SLOT_MAP()

AnalogInput::AnalogInput()
{
   STANDARD_CONSTRUCTOR()
}

void AnalogInput::copyData(const AnalogInput& org, const bool)
{
   BaseClass::copyData(org);

   devEnb = org.devEnb;
   location = org.location;
   channel = org.channel;
   value = org.value;
   deadband = org.deadband;
   gain = org.gain;
   offset = org.offset;
   {
      const base::Table1* copy = nullptr;
      if (org.table != nullptr) {
         copy = org.table->clone();
      }
      setTable(copy);
      if (copy != nullptr) copy->unref();
   }
}

void AnalogInput::deleteData()
{
   setTable(nullptr);
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
unsigned int AnalogInput::getLocation() const
{
   return location;
}

unsigned int AnalogInput::getChannel() const
{
   return channel;
}

double AnalogInput::getValue() const
{
   return value;
}

double AnalogInput::getDeadband() const
{
   return deadband;
}

double AnalogInput::getOffset() const
{
   return offset;
}

double AnalogInput::getGain() const
{
   return gain;
}

const base::Table1* AnalogInput::getTable() const
{
   return table;
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool AnalogInput::setLocation(const unsigned int v)
{
   location = v;
   return true;
}

bool AnalogInput::setChannel(const unsigned int v)
{
   devEnb = true;
   channel = v;
   return true;
}

bool AnalogInput::setValue(const double v)
{
   value = v;
   return true;
}

bool AnalogInput::setDeadband(const double v)
{
   deadband = v;
   return true;
}

bool AnalogInput::setOffset(const double v)
{
   offset = v;
   return true;
}

bool AnalogInput::setGain(const double v)
{
   gain = v;
   return true;
}

// table: Shaping function table
bool AnalogInput::setTable(const base::Table1* const msg)
{
    bool ok = true;

    // Unref() the old (if any)
    if (table != nullptr) {
        table->unref();
    }

    // set our pointer to the new
    table = msg;

    // Check and ref() the new table (if any)
    if (table != nullptr) {
        if (table->isValid()) {
            table->ref();
        }
        else {
            if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "AnalogInput::setTable(): invalid table!" << std::endl;
            }
            ok = false;
            table = nullptr;
        }
    }

    return ok;
}

//------------------------------------------------------------------------------
// process inputs
//------------------------------------------------------------------------------
void AnalogInput::processInputs(const double dt, const base::IoDevice* const device, base::IoData* const inData)
{
   // Default is our initial value
   double vin = value;


   // Get data from the AI card
   if (device != nullptr && devEnb) {
      device->getAnalogInput(&vin, channel);
   }

   // process the input value, as needed
   double vout = convert(vin,dt);

   // Set the data to the input data handler
   if (inData != nullptr) {
      inData->setAnalogInput(location,vout);
   }
}

//------------------------------------------------------------------------------
// process outputs
//------------------------------------------------------------------------------
void AnalogInput::processOutputs(const double, const base::IoData* const, base::IoDevice* const)
{
}

//------------------------------------------------------------------------------
// convert the value, as needed
//------------------------------------------------------------------------------
double AnalogInput::convert(const double vin, const double)
{
   // Deadband
   double v1 = vin;
   if (deadband != 0 && vin < deadband && vin > -deadband) {
      v1 = 0;
   }

   // Offset & Gain
   double v2 = (v1 - offset) * gain;

   // Shaping function
   double v3 = v2;
   if (table != nullptr) v3 = table->lfi(v2);

   // return final value
   return v3;
}


//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

// ai: Analog Input location
bool AnalogInput::setSlotLocation(const base::Number* const msg)
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

// channel: AI card's channel number
bool AnalogInput::setSlotChannel(const base::Number* const msg)
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

// value: Initial value (default: 0)
bool AnalogInput::setSlotValue(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setValue( msg->getReal() );
   }
   return ok;
}

// deadband: Deadband: [ 0 .. 1 ] (default: 0.0)
bool AnalogInput::setSlotDeadband(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setDeadband( msg->getReal() );
   }
   return ok;
}

// offset: Offset value
bool AnalogInput::setSlotOffset(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setOffset( msg->getReal() );
   }
   return ok;
}

// gain: Gain value
bool AnalogInput::setSlotGain(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setGain( msg->getReal() );
   }
   return ok;
}

std::ostream& AnalogInput::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "ai: " << getLocation() << std::endl;

   indent(sout,i+j);
   sout << "channel: " << getChannel() << std::endl;

   indent(sout,i+j);
   sout << "value: " << getValue() << std::endl;

   indent(sout,i+j);
   sout << "deadband: " << getDeadband() << std::endl;

   indent(sout,i+j);
   sout << "offset: " << getOffset() << std::endl;

   indent(sout,i+j);
   sout << "gain: " << getGain() << std::endl;

   if (table != nullptr) {
      indent(sout,i+j);
      sout << "table: ";
      table->serialize(sout,(i+j+4));
   }

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

}
}
