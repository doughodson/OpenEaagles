
#include "openeaagles/iodevice/Controller.h"

#include "openeaagles/base/Number.h"

namespace oe {
namespace iodevice {

/* both the linux and windows version use the 'UsbJoystick' form name */
IMPLEMENT_SUBCLASS(Controller, "Controller")

// slot table for this class type
BEGIN_SLOTTABLE(Controller)
    "deviceIndex",   // 1) Device Index (default: 0)
END_SLOTTABLE(Controller)

//  Map slot table to handles
BEGIN_SLOT_MAP(Controller)
    ON_SLOT( 1, setSlotDeviceIndex,  oe::base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Controller::Controller()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// Init our data
//------------------------------------------------------------------------------
void Controller::initData()
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
void Controller::copyData(const Controller& org, const bool)
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
void Controller::deleteData()
{
   numAI = 0;
   numDI = 0;
}

//------------------------------------------------------------------------------
// Access Functions
//------------------------------------------------------------------------------

unsigned short Controller::getNumDiscreteInputChannels() const
{
   return numDI;
}

unsigned short Controller::getNumDiscreteInputPorts() const
{
   return 1;
}

bool Controller::getDiscreteInput(bool* const value, const unsigned int bit, const unsigned int) const
{
   bool ok = false;
   if (value != nullptr && bit < numDI) {
      *value = inBits[bit];
      ok = true;
   }
   return ok;
}

unsigned short Controller::getNumAnalogInputs() const
{
   return numAI;
}

bool Controller::getAnalogInput(double* const value, const unsigned int channel) const
{
   bool ok = false;
   if (value != nullptr && channel < numAI) {
      *value = inData[channel];
      ok = true;
   }
   return ok;
}

unsigned int Controller::getDeviceIndex() const
{
   return deviceIndex;
}


//------------------------------------------------------------------------------
// Set Functions
//------------------------------------------------------------------------------

// Device number
bool Controller::setDeviceIndex(const int v)
{
   bool ok = false;
   if (v >= 0 && v <= 31) {
      deviceIndex = static_cast<unsigned int>(v);
      ok = true;
   }
   return ok;
}


//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

// deviceIndex: device index
bool Controller::setSlotDeviceIndex(const oe::base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setDeviceIndex( msg->getInt() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
base::Object* Controller::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Controller::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
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

}
}


