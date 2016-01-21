
#include "openeaagles/basic/IoDevice.h"
#include "openeaagles/basic/IoAdapter.h"
#include "openeaagles/basic/IoHandler.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace oe {
namespace Basic {

//==============================================================================
// IoDevice
//==============================================================================

IMPLEMENT_SUBCLASS(IoDevice,"IoDevice")

// slot table for this class type
BEGIN_SLOTTABLE(IoDevice)
   "adapters",    // 1) List of IoAdapter objects (default: none)
   "devices",     // 2) List of IoDevice objects; i.e., sub-devices (default: none)
END_SLOTTABLE(IoDevice)

//  Map slot table to handles
BEGIN_SLOT_MAP(IoDevice)
   ON_SLOT(1, setSlotAdapters, PairStream)
   ON_SLOT(2, setSlotDevices,  PairStream)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IoDevice::IoDevice()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// Init member data
//------------------------------------------------------------------------------
void IoDevice::initData()
{
   adapters = nullptr;
   devices = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IoDevice::copyData(const IoDevice& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // clear our old lists of adapters and devices
   setSlotAdapters(nullptr);
   setSlotDevices(nullptr);

   // ---
   // copy the list of I/O adapters
   // ---
   if (org.adapters != nullptr) {
      PairStream* copy = static_cast<PairStream*>(org.adapters->clone());
      setSlotAdapters(copy);
      copy->unref();
   }

   // ---
   // copy the list of I/O devices
   // ---
   if (org.devices != nullptr) {
      PairStream* copy = static_cast<PairStream*>(org.devices->clone());
      setSlotDevices(copy);
      copy->unref();
   }
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void IoDevice::deleteData()
{
   setSlotAdapters(nullptr);
   setSlotDevices(nullptr);
}

//------------------------------------------------------------------------------
// reset() -- Reset the I/O handler
//------------------------------------------------------------------------------
void IoDevice::reset()
{
   BaseClass::reset();

   // Reset our I/O adapters
   if (adapters != nullptr) {
      List::Item* item = adapters->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair*>(item->getValue());
         IoAdapter* const p = static_cast<IoAdapter*>(pair->object());
         p->reset();
         item = item->getNext();
      }
   }

   // Reset our I/O devices
   if (devices != nullptr) {
      List::Item* item = devices->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair*>(item->getValue());
         IoDevice* const p = static_cast<IoDevice*>(pair->object());
         p->reset();
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool IoDevice::shutdownNotification()
{
   // Shutdown our I/O adapters
   if (adapters != nullptr) {
      List::Item* item = adapters->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair*>(item->getValue());
         IoAdapter* const p = static_cast<IoAdapter*>(pair->object());
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
   }

   // Shutdown our I/O devices
   if (devices != nullptr) {
      List::Item* item = devices->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair*>(item->getValue());
         IoDevice* const p = static_cast<IoDevice*>(pair->object());
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
   }

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// Process device input channels and components (default)
//------------------------------------------------------------------------------
void IoDevice::processInputs(const LCreal dt, IoData* const inData)
{
   // ### Since we'll process all of the input adapters, our derived I/O device
   // classes should process their device inputs BEFORE calling this base
   // class functions. ###

   // process any input adapters
   if (adapters != nullptr) {

      List::Item* item = adapters->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair*>(item->getValue());
         IoAdapter* const p = static_cast<IoAdapter*>(pair->object());
         p->processInputs(dt, this, inData);
         item = item->getNext();
      }
   }

   // process any input (sub)devices
   if (devices != nullptr) {
      List::Item* item = devices->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair*>(item->getValue());
         IoDevice* const p = static_cast<IoDevice*>(pair->object());
         p->processInputs(dt, inData);
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// Process device output channels and components (default)
//------------------------------------------------------------------------------
void IoDevice::processOutputs(const LCreal dt, const IoData* const outData)
{
   // ### Since we'll process all of the output I/O adapters, our derived I/O
   // device classes should process their device outputs AFTER calling this
   // base class functions. ###

   // process our output (sub)devices
   if (devices != nullptr) {
      List::Item* item = devices->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair*>(item->getValue());
         IoDevice* const p = static_cast<IoDevice*>(pair->object());
         p->processOutputs(dt, outData);
         item = item->getNext();
      }
   }

   // process any output adapters
   if (adapters != nullptr) {

      if (outData != nullptr) {
         List::Item* item = adapters->getFirstItem();
         while (item != nullptr) {
            Pair* const pair = static_cast<Pair*>(item->getValue());
            IoAdapter* const p = static_cast<IoAdapter*>(pair->object());
            p->processOutputs(dt, outData, this);
            item = item->getNext();
         }
      }
   }
}

//------------------------------------------------------------------------------
// Default I/O access functions
//------------------------------------------------------------------------------

unsigned short IoDevice::getNumDiscreteInputChannels() const
{
   return 0;
}

unsigned short IoDevice::getNumDiscreteInputPorts() const
{
   return 0;
}

bool IoDevice::getDiscreteInput(bool* const, const unsigned int, const unsigned int) const
{
   return false;
}

unsigned short IoDevice::getNumDiscreteOutputChannels() const
{
   return 0;
}

unsigned short IoDevice::getNumDiscreteOutputPorts() const
{
   return 0;
}

bool IoDevice::setDiscreteOutput(const bool, const unsigned int, const unsigned int)
{
   return false;
}

unsigned short IoDevice::getNumAnalogInputs() const
{
   return 0;
}

bool IoDevice::getAnalogInput(LCreal* const, const unsigned int) const
{
   return false;
}

unsigned short IoDevice::getNumAnalogOutputs() const
{
   return 0;
}

bool IoDevice::setAnalogOutput(const LCreal, const unsigned int)
{
   return false;
}

PairStream* IoDevice::getDeviceList()
{
   return devices;
}

PairStream* IoDevice::getAdapterList()
{
   return adapters;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// adapters: List of IoAdapter objects
bool IoDevice::setSlotAdapters(PairStream* const list)
{
   bool ok = true;

   if (list != nullptr) {
      // check to make sure all objects on the list are I/O adapters
      unsigned int cnt = 0;
      List::Item* item = list->getFirstItem();
      while (item != nullptr) {
         cnt++;
         Pair* const pair = static_cast<Pair*>(item->getValue());
         ok = pair->object()->isClassType(typeid(IoAdapter));
         if (ok) {
            static_cast<IoAdapter*>(pair->object())->container(this);
         }
         else {
            std::cerr << "IoDevice::setSlotAdapters(): Item number " << cnt;
            std::cerr << " on the list is a non-IoAdapter component!" << std::endl;
         }
         item = item->getNext();
      }
   }

   if (ok) adapters = list;

   return ok;
}

// devices: List of IoDevice objects
bool IoDevice::setSlotDevices(PairStream* const list)
{
   bool ok = true;

   if (list != nullptr) {
      // check to make sure all objects on the list are I/O Devices
      unsigned int cnt = 0;
      List::Item* item = list->getFirstItem();
      while (item != nullptr) {
         cnt++;
         Pair* const pair = static_cast<Pair*>(item->getValue());
         ok = pair->object()->isClassType(typeid(IoDevice));
         if (ok) {
            static_cast<IoDevice*>(pair->object())->container(this);
         }
         else {
            std::cerr << "IoDevice::setSlotDevices(): Item number " << cnt;
            std::cerr << " on the list is a non-IoDevice component!" << std::endl;
         }
         item = item->getNext();
      }
   }

   if (ok) devices = list;

   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Object* IoDevice::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& IoDevice::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   if (adapters != nullptr) {
      indent(sout,i+j);
      sout << "adapters: ";
      adapters->serialize(sout,(i+j+4));
   }

   if (devices != nullptr) {
      indent(sout,i+j);
      sout << "devices: ";
      devices->serialize(sout,(i+j+4));
   }

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // end IoDevice
} // end oe namespace
