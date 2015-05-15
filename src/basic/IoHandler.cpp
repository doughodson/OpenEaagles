
#include "openeaagles/basic/IoHandler.h"
#include "openeaagles/basic/IoData.h"
#include "openeaagles/basic/IoDevice.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Thread.h"
#include "openeaagles/basic/units/Frequencies.h"

namespace Eaagles {
namespace Basic {

//=============================================================================
// Declare the I/O thread
//=============================================================================

class IoThread : public ThreadPeriodicTask {
   DECLARE_SUBCLASS(IoThread,ThreadPeriodicTask)
   public: IoThread(Component* const parent, const LCreal priority, const LCreal rate);
   private: virtual unsigned long userFunc(const LCreal dt);
};

//==============================================================================
// IoHandler
//==============================================================================

IMPLEMENT_SUBCLASS(IoHandler,"IoHandler")

// slot table for this class type
BEGIN_SLOTTABLE(IoHandler)
   "ioData",      // 1) Combined input/output data
   "inputData",   // 2) Individual input data (default: none)
   "outputData",  // 3) Individual output data (default: none)
   "devices",     // 4) List of I/O device handlers (IoDevice objects) (default: none)
   "rate",        // 5) Thread rate (default: 50hz)
   "priority"     // 6) Thread priority (zero(0) is lowest, one(1) is highest) (Number, default: 0.5 )
END_SLOTTABLE(IoHandler)

//  Map slot table to handles
BEGIN_SLOT_MAP(IoHandler)
   ON_SLOT(1, setSlotIoData,     IoData)
   ON_SLOT(2, setSlotInputData,  IoData)
   ON_SLOT(3, setSlotOutputData, IoData)
   ON_SLOT(4, setSlotDevices,    PairStream)
   ON_SLOT(5, setSlotRate,       Frequency)
   ON_SLOT(6, setSlotPriority,   Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IoHandler::IoHandler()
{
   STANDARD_CONSTRUCTOR()
   initData();
}


//------------------------------------------------------------------------------
// Init member data
//------------------------------------------------------------------------------
void IoHandler::initData()
{
   inData = nullptr;
   outData = nullptr;
   devices = nullptr;

   netInitialized = false;
   netInitFailed = false;

   rate = 50;
   pri = 0.5f;
   thread = nullptr;
}


//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IoHandler::copyData(const IoHandler& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // clear the I/O buffers and list of devices
   setSlotIoData(nullptr);
   setSlotDevices(nullptr);

   // ---
   // copy the I/O buffers
   // ---
   if (org.inData != nullptr && org.inData == org.outData) {
      // Common input/output buffer
      IoData* copy = static_cast<IoData*>(org.inData->clone());
      setSlotIoData(copy);
      copy->unref();
   }
   else {
      // Separate input/output buffers
      if (org.inData != nullptr) {
         IoData* copy = static_cast<IoData*>(org.inData->clone());
         setSlotInputData(copy);
         copy->unref();
      }
      if (org.outData != nullptr) {
         IoData* copy = static_cast<IoData*>(org.outData->clone());
         setSlotOutputData(copy);
         copy->unref();
      }
   }

   // ---
   // copy the list of I/O devices
   // ---
   if (org.devices != nullptr) {
      PairStream* copy = static_cast<PairStream*>(org.devices->clone());
      setSlotDevices(copy);
      copy->unref();
   }

   netInitialized = false;
   netInitFailed = false;

   rate = 50;
   pri = 0.0;

   if (thread != nullptr) {
      thread->terminate();
      thread = nullptr;
   }
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void IoHandler::deleteData()
{
   inData = nullptr;
   outData = nullptr;
   devices = nullptr;

   if (thread != nullptr) {
      thread->terminate();
      thread = nullptr;
   }
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
IoData* IoHandler::getInputData()               { return inData; }
const IoData* IoHandler::getInputData() const   { return inData; }

IoData* IoHandler::getOutputData()              { return outData; }
const IoData* IoHandler::getOutputData() const  { return outData; }

LCreal IoHandler::getPriority() const           { return pri; }
LCreal IoHandler::getRate() const               { return rate; }

//------------------------------------------------------------------------------
// reset() -- Reset the I/O handler
//------------------------------------------------------------------------------
void IoHandler::reset()
{
   BaseClass::reset();

   // Reset our I/O devices
   if (devices != nullptr) {
      List::Item* item = devices->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair* const>(item->getValue());
         IoDevice* const p = static_cast<IoDevice* const>(pair->object());
         p->reset();
         item = item->getNext();
      }
   }

   // Initialize the networks
   if (!netInitialized && !netInitFailed) {
      netInitialized = initNetworks();
      netInitFailed = !netInitialized;
   }

   clear();
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool IoHandler::shutdownNotification()
{
   // Shutdown our I/O devices
   if (devices != nullptr) {
      List::Item* item = devices->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair* const>(item->getValue());
         IoDevice* const p = static_cast<IoDevice* const>(pair->object());
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
   }

   // Zero (unref) our thread object (of any).  The thread's function has ref()'d
   // this object, so it won't be deleted until the thread terminates, which it
   // will based on our BaseClass::isShutdown() function.  But at least we won't
   // mistakenly think that it's still around.
   if (thread != nullptr) {
      thread->terminate();
      thread = nullptr;
   }

   return BaseClass::shutdownNotification();
}

// -----------------------------------------------------------------------------
// Clear the data
// -----------------------------------------------------------------------------
void IoHandler::clear()
{
   if (inData != nullptr) inData->clear();
   if (outData != nullptr) outData->clear();
}

//------------------------------------------------------------------------------
// initNetworks() -- Init the networks
//------------------------------------------------------------------------------
bool IoHandler::initNetworks()
{
   bool ok1 = false;
   bool ok2 = false;

   if (inData != nullptr) ok1 = inData->initNetwork();
   if (outData != nullptr) ok2 = outData->initNetwork();

   return (ok1 && ok2);
}

//------------------------------------------------------------------------------
// Handle input devices
//------------------------------------------------------------------------------
void IoHandler::inputDevices(const LCreal dt)
{
   if (thread == nullptr) inputDevicesImp(dt);
}

//------------------------------------------------------------------------------
// Handle output devices
//------------------------------------------------------------------------------
void IoHandler::outputDevices(const LCreal dt)
{
   if (thread == nullptr) outputDevicesImp(dt);
}

//------------------------------------------------------------------------------
// Implementation of the input devices handler
//------------------------------------------------------------------------------
void IoHandler::inputDevicesImp(const LCreal dt)
{
   // process our I/O devices
   if (devices != nullptr) {
      List::Item* item = devices->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair* const>(item->getValue());
         IoDevice* const p = static_cast<IoDevice* const>(pair->object());
         p->processInputs(dt, inData);
         item = item->getNext();
      }
   }

   // update the input data buffers after the input devices
   if (inData != nullptr) inData->processInputs();
}

//------------------------------------------------------------------------------
// Implementation of the output devices handler
//------------------------------------------------------------------------------
void IoHandler::outputDevicesImp(const LCreal dt)
{
   // update the output data buffers before the output devices
   if (outData != nullptr) outData->processOutputs();

   // process our I/O devices
   if (devices != nullptr) {
      List::Item* item = devices->getFirstItem();
      while (item != nullptr) {
         Pair* const pair = static_cast<Pair* const>(item->getValue());
         IoDevice* const p = static_cast<IoDevice* const>(pair->object());
         p->processOutputs(dt, outData);
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// createDataThread() -- Create the data acq thread
//------------------------------------------------------------------------------
void IoHandler::createDataThread()
{
   if ( thread == nullptr ) {
      thread = new IoThread(this, getPriority(), getRate());
      thread->unref(); // 'thread' is a safe_ptr<>

      bool ok = thread->create();
      if (!ok) {
         thread = nullptr;
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IoHandler::createDataThread(): ERROR, failed to create the thread!" << std::endl;
         }
      }
   }
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool IoHandler::setSlotIoData(IoData* const msg)
{
   inData = msg;
   outData = msg;
   return true;
}

bool IoHandler::setSlotInputData(IoData* const msg)
{
   inData = msg;
   return true;
}

bool IoHandler::setSlotOutputData(IoData* const msg)
{
   outData = msg;
   return true;
}

bool IoHandler::setSlotDevices(PairStream* const list)
{
   bool ok = true;

   if (list != nullptr) {
      // check to make sure all objects on the list are I/O Devices
      unsigned int cnt = 0;
      List::Item* item = list->getFirstItem();
      while (item != nullptr) {
         cnt++;
         Pair* const pair = static_cast<Pair* const>(item->getValue());
         ok = pair->object()->isClassType(typeid(IoDevice));
         if (ok) {
            static_cast<IoDevice*>(pair->object())->container(this);
         }
         else {
            std::cerr << "IoHandler::setSlotDevices(): Item number " << cnt;
            std::cerr << " on the list is a non-IoDevice component!" << std::endl;
         }
         item = item->getNext();
      }
   }

   if (ok) devices = list;

   return ok;
}

bool IoHandler::setSlotRate(const Frequency* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        LCreal x = Hertz::convertStatic(*msg);
        if (x > 0) {
            rate = x;
            ok = true;
        }
        else {
            std::cerr << "IoHandler::setSlotRate: Rate is invalid; must be greater than zero" << std::endl;
        }
    }
    return ok;
}

bool IoHandler::setSlotPriority(const Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
       LCreal x = num->getReal();
        if (x >= 0 && x <= 1.0f) {
            pri = x;
            ok = true;
        }
        else {
            std::cerr << "IoHandler::setSlotPriority: Priority is invalid, range: [0 .. 1]" << std::endl;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Object* IoHandler::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& IoHandler::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   if (inData != nullptr || outData != nullptr) {

      // combined data
      if (inData == outData) {
         indent(sout,i+j);
         sout << "ioData: ";
         inData->serialize(sout,(i+j+4));
      }

      // individual data
      else {
         if (inData != nullptr) {
            indent(sout,i+j);
            sout << "inputData: ";
            inData->serialize(sout,(i+j+4));
         }
         if (outData != nullptr) {
            indent(sout,i+j);
            sout << "outputData: ";
            outData->serialize(sout,(i+j+4));
         }
      }

   }

   if (devices != nullptr) {
      indent(sout,i+j);
      sout << "devices: ";
      devices->serialize(sout,(i+j+4));
   }

   indent(sout,i+j);
   sout << "frequency: ( Hertz " << rate << " )" << std::endl;

   indent(sout,i+j);
   sout << "priority: " << pri << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

//=============================================================================
// IoThread: Data I/O thread
//=============================================================================
IMPLEMENT_SUBCLASS(IoThread,"IoThread")
EMPTY_SLOTTABLE(IoThread)
EMPTY_COPYDATA(IoThread)
EMPTY_DELETEDATA(IoThread)
EMPTY_SERIALIZER(IoThread)

IoThread::IoThread(Component* const parent, const LCreal priority, const LCreal rate)
      : ThreadPeriodicTask(parent, priority, rate)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long IoThread::userFunc(const LCreal dt)
{
   IoHandler* ioHandler = static_cast<IoHandler*>(getParent());
   ioHandler->inputDevicesImp( static_cast<LCreal>(dt) );
   ioHandler->outputDevicesImp( static_cast<LCreal>(dt) );
   return 0;
}

} // End Basic
} // end Eaagles namespace
