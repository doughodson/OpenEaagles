// Class: AnalogOutput

#include "openeaagles/ioDevice/AnalogOutput.h"

#include "openeaagles/basic/IoData.h"
#include "openeaagles/basic/IoDevice.h"
#include "openeaagles/basic/IoHandler.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Tables.h"

namespace Eaagles {
namespace IoDevice {

//==============================================================================
// AnalogOutput
//==============================================================================

IMPLEMENT_SUBCLASS(AnalogOutput,"AnalogOutput")

// slot table for this class type
BEGIN_SLOTTABLE(AnalogOutput)
    "ao",         // 1) Analog Output location (IoData AO's channel)
    "channel",    // 2) Device channel number
    "value",      // 3) Initial value [ -1.0 ... 1.0 ] (default: 0)
    "offset",     // 4) Offset value (default: 0.0)
    "gain",       // 5) Gain value   (default: 1.0)
    "table"       // 6) Shapping function table (default: none)
END_SLOTTABLE(AnalogOutput)

//  Map slot table to handles
BEGIN_SLOT_MAP(AnalogOutput)
    ON_SLOT( 1, setSlotLocation, Basic::Number)
    ON_SLOT( 2, setSlotChannel,  Basic::Number)
    ON_SLOT( 3, setSlotValue,    Basic::Number)
    ON_SLOT( 4, setSlotOffset,   Basic::Number)
    ON_SLOT( 5, setSlotGain,     Basic::Number)
    ON_SLOT( 6, setTable,        Basic::Table1)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AnalogOutput::AnalogOutput()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void AnalogOutput::initData()
{
   devEnb = false;
   location = 0;
   channel  = 0;
   value = 0;
   gain  = 1.0f;
   offset = 0.0;
   table = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void AnalogOutput::copyData(const AnalogOutput& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   devEnb = org.devEnb;
   location = org.location;
   channel = org.channel;
   value = org.value;
   gain = org.gain;
   offset = org.offset;
   {
      const Basic::Table1* copy = 0;
      if (org.table != 0) {
         copy = (Basic::Table1*) org.table->clone();
      }
      setTable(copy);
      if (copy != 0) copy->unref();
   }
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void AnalogOutput::deleteData()
{
   setTable(0);
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
unsigned int AnalogOutput::getLocation() const
{
   return location;
}

unsigned int AnalogOutput::getChannel() const
{
   return channel;
}

LCreal AnalogOutput::getValue() const
{
   return value;
}

LCreal AnalogOutput::getOffset() const
{
   return offset;
}

LCreal AnalogOutput::getGain() const
{
   return gain;
}

const Basic::Table1* AnalogOutput::getTable() const
{
   return table;
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool AnalogOutput::setLocation(const unsigned int v)
{
   location = v;
   return true;
}

bool AnalogOutput::setChannel(const unsigned int v)
{
   channel = v;
   devEnb = true;
   return true;
}

bool AnalogOutput::setValue(const LCreal v)
{
   value = v;
   return true;
}

bool AnalogOutput::setOffset(const LCreal v)
{
   offset = v;
   return true;
}

bool AnalogOutput::setGain(const LCreal v)
{
   bool ok = false;
   if (v != 0) {
      gain = v;
      ok = true;
   }
   return ok;
}

// table: Shapping function table
bool AnalogOutput::setTable(const Basic::Table1* const msg)
{
    bool ok = true;

    // Unref() the old (if any)
    if (table != 0) { 
        table->unref();
    }

    // set our pointer to the new
    table = msg;

    // Check and ref() the new table (if any)
    if (table != 0) {
        if (table->isValid()) { 
            table->ref();
        }
        else {
            if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "AnalogOutput::setTable(): invalid table!" << std::endl;
            }
            ok = false;
            table = 0;
        }
    }

    return ok;
}

//------------------------------------------------------------------------------
// process inputs
//------------------------------------------------------------------------------
void AnalogOutput::processInputs(const LCreal, const Basic::IoDevice* const, Basic::IoData* const)
{
}

//------------------------------------------------------------------------------
// process outputs
//------------------------------------------------------------------------------
void AnalogOutput::processOutputs(const LCreal, const Basic::IoData* const outData, Basic::IoDevice* const device)
{
   // Get a value form the cockpit output handler
   if (outData != 0) {
      outData->getAnalogOutput(location,&value);
   }

   // Send the scaled data to the AO card
   if (device != 0 && devEnb) {
      LCreal vout = 0;
      if (gain != 0) {
         vout = (value / gain ) + offset;
      }
      device->setAnalogOutput(vout, channel);
   }
}

//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

// location: Output array index (location)
bool AnalogOutput::setSlotLocation(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setLocation( (unsigned int) v );
      }
   }
   return ok;
}

// channel: AI card's channel number
bool AnalogOutput::setSlotChannel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setChannel( (unsigned int) v );
      }
   }
   return ok;
}

// value: Initial value (default: 0)
bool AnalogOutput::setSlotValue(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setValue( msg->getFloat() );
   }
   return ok;
}

// offset: Offset value
bool AnalogOutput::setSlotOffset(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setOffset( msg->getFloat() );
   }
   return ok;
}

// gain: Gain value
bool AnalogOutput::setSlotGain(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setGain( msg->getFloat() );
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "AnalogOutput::setSlotGain(): ERROR -- gain can not be zero." << std::endl;
         }
      }
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Basic::Object* AnalogOutput::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& AnalogOutput::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "ao: " << getLocation() << std::endl;

   indent(sout,i+j);
   sout << "channel: " << getChannel() << std::endl;

   indent(sout,i+j);
   sout << "value: " << getValue() << std::endl;

   indent(sout,i+j);
   sout << "offset: " << getOffset() << std::endl;

   indent(sout,i+j);
   sout << "gain: " << getGain() << std::endl;

   if (table != 0) {
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

} // end IoDevice
} // end Eaagles namespace
