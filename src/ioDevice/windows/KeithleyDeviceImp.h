//------------------------------------------------------------------------------
// Class: KeithleyDeviceImp -- MS Windows implementation
//------------------------------------------------------------------------------
#ifndef __IoDevice_Windows_KeithleyDeviceImp_H__
#define __IoDevice_Windows_KeithleyDeviceImp_H__

#include "openeaagles/ioDevice/KeithleyDevice.h"

struct DL_ServiceRequest;

namespace Eaagles {
namespace IoDevice {

//------------------------------------------------------------------------------
// Class:  KeithleyDeviceImp
//
// Description:  MS Windows version of the Keithley I/O device.
//               (See IoDevice::KeithleyDevice)
//
// Form Name: KeithleyDevice
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

   // display native microsoft windows message box
   void showMessage(DL_ServiceRequest*);

   // create service request struct
   DL_ServiceRequest* createServiceRequest();

   void defineInitServReq(DL_ServiceRequest*);

   // analog oriented methods
   // these methods will fill out the service request struct
   // for various driver operations
   void defineAnalogServReq(DL_ServiceRequest*);

   // configure analog service request for reading device
   void cfgAnalogRead(DL_ServiceRequest*);

   // configure analog service request to convert received data
   void cfgAnalogConvert(DL_ServiceRequest*);

   // digital oriented methods
   void defineDigitalServReq(DL_ServiceRequest*);

   HINSTANCE driverInstance;

   // structures to store analog and digital service requests
   DL_ServiceRequest* analogServReq;
   DL_ServiceRequest* digitalServReq;

   float* analogValues;
   BYTE* digitalValues;

   bool driverOpen;
   bool openFailed;
};

} // end IoDevice
} // end Eaagles namespace

#endif
