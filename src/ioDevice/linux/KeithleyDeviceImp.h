//------------------------------------------------------------------------------
// Class: KeithleyDeviceImp -- Linux implementation
//------------------------------------------------------------------------------
#ifndef __IoDevice_Linux_KeithleyDeviceImp_H__
#define __IoDevice_Linux_KeithleyDeviceImp_H__

#include "openeaagles/ioDevice/KeithleyDevice.h"

namespace Eaagles {
namespace IoDevice {

//------------------------------------------------------------------------------
// Class:  KeithleyDeviceImp
//
// Description:  Linux version of the Keithley I/O device.
//               (See IoDevice::KeithleyDevice)
//
// Form Name: KeithleyDevice
//
//------------------------------------------------------------------------------
class KeithleyDeviceImp : public KeithleyDevice {
    DECLARE_SUBCLASS(KeithleyDeviceImp,KeithleyDevice)

public:
   KeithleyDeviceImp();

   // Basic::IoDevice functions
   virtual void processInputs(const LCreal dt, Basic::IoData* const inData);

   // Basic::Component functions
   virtual void reset();

private:
   void initData();

   enum { NUM_ANALOG_CHANNELS = 16,
          NUM_DIGITAL_CHANNELS = 23
   };

   int stream;
   bool driverOpen;
   bool openFailed;
};

} // end IoDevice
} // end Eaagles namespace

#endif
