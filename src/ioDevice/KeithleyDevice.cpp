//==============================================================================
// KeithleyDevice
//==============================================================================

#include "openeaagles/ioDevice/KeithleyDevice.h"

#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace IoDevice {

/* both the linux and windows version use the 'KeithleyDevice' form name */
IMPLEMENT_SUBCLASS(KeithleyDevice,"CommonKeithleyDevice")

// slot table for this class type
BEGIN_SLOTTABLE(KeithleyDevice)
    "deviceIndex",   // 1) Device Index (default: 0)
END_SLOTTABLE(KeithleyDevice)

//  Map slot table to handles
BEGIN_SLOT_MAP(KeithleyDevice)
    ON_SLOT( 1, setSlotDeviceIndex,  Eaagles::Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
KeithleyDevice::KeithleyDevice()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// Init our data
//------------------------------------------------------------------------------
void KeithleyDevice::initData()
{
   deviceIndex = 0;

   numAI = 0;
   for (unsigned int i = 0; i < MAX_AI; i++) {
      inData[i] = 0;
   }

   numDI = 0;
   for (unsigned int i = 0; i < MAX_DI; i++) {
      inBits[i] = false;
   }
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void KeithleyDevice::copyData(const KeithleyDevice& org, const bool)
{
   BaseClass::copyData(org);

   initData();

   deviceIndex = org.deviceIndex;

   numAI = org.numAI;
   for (unsigned int i = 0; i < MAX_AI; i++) {
      inData[i] = org.inData[i];
   }

   numDI = org.numDI;
   for (unsigned int i = 0; i < MAX_DI; i++) {
      inBits[i] = org.inBits[i];
   }

}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void KeithleyDevice::deleteData()
{
   numAI = 0;
   numDI = 0; 
}

//------------------------------------------------------------------------------
// Access Functions
//------------------------------------------------------------------------------

unsigned short KeithleyDevice::getNumDiscreteInputChannels() const
{
   return numDI;
}

unsigned short KeithleyDevice::getNumDiscreteInputPorts() const
{
   return 1;
}

bool KeithleyDevice::getDiscreteInput(bool* const value, const unsigned int bit, const unsigned int) const
{
   bool ok = false;
   if (value != 0 && bit < numDI) {
      *value = inBits[bit];
      ok = true;
   }
   return ok;
}

unsigned short KeithleyDevice::getNumAnalogInputs() const
{
   return numAI;
}

bool KeithleyDevice::getAnalogInput(LCreal* const value, const unsigned int channel) const
{
   bool ok = false;
   if (value != 0 && channel < numAI) {
      *value = inData[channel];
      ok = true;
   }
   return ok;
}

unsigned int KeithleyDevice::getDeviceIndex() const
{
   return deviceIndex;
}


//------------------------------------------------------------------------------
// Set Functions
//------------------------------------------------------------------------------

// Device number
bool KeithleyDevice::setDeviceIndex(const int v)
{
   bool ok = false;
   if (v >= 0 && v <= 31) {
      deviceIndex = (unsigned int) v;
      ok = true;
   }
   return ok;
}


//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

// deviceIndex: device index
bool KeithleyDevice::setSlotDeviceIndex(const Eaagles::Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setDeviceIndex( msg->getInt() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Eaagles::Basic::Object* KeithleyDevice::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& KeithleyDevice::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "deviceIndex: " << deviceIndex << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // IoDevice namespace
} // end Eaagles namespace
