//------------------------------------------------------------------------------
// Class: IoDevice
//------------------------------------------------------------------------------
#ifndef __oe_basic_iodevice_H__
#define __oe_basic_iodevice_H__

#include "openeaagles/basic/Component.h"

namespace oe {
namespace basic {
   class IoData;

//------------------------------------------------------------------------------
// Class:  IoDevice
//
// Description: I/O device handler.
//
//    The I/O devices handles communicate with the I/O hardware, usually via
//    device drivers, and therefore can be application and/or operating
//    system specific.  An I/O device can manage (sub) I/O devices that are
//    attached using the 'devices' slot.
//
//    Using the 'adapters' slot, an I/O device handlers can contain a list
//    of I/O adapters (see IoAdapter.h) that control the flow of I/O data
//    between the I/O device hander and I/O data buffers (see IoData.h).
//
//    The top level list of I/O devices is typically managed by an I/O
//    handler class (see IoHander.h).
//
// Notes:
//    1) All Channel (and port) numbers start with zero;
//       example: AI channels range from 0 to (getNumAnalogInputs() - 1)
//
//    2) Use the reset() function, reset(), to open the device, and the
//       deleteData() function, basic::Object::deleteData(), to close the
//       device (both as required).
//
//    3) Both processInputs() and processOutputs() are passed an I/O data
//       buffer that are application (not device) specific.  This buffer
//       should be passed to the I/O adapters.
//
//
// Factory name: IoDevice
// Slots:
//    adapters <PairStream>   ! List of IoAdapter objects (default: none)
//    devices  <PairStream>   ! List of IoDevice objects; i.e., sub-devices (default: none)
//
//------------------------------------------------------------------------------
class IoDevice : public basic::Component {
    DECLARE_SUBCLASS(IoDevice,basic::Component)

public:
   IoDevice();

   // DI functions
   virtual unsigned short getNumDiscreteInputChannels() const;
   virtual unsigned short getNumDiscreteInputPorts() const;
   virtual bool getDiscreteInput(bool* const value, const unsigned int channel, const unsigned int port) const;

   // DO functions
   virtual unsigned short getNumDiscreteOutputChannels() const;
   virtual unsigned short getNumDiscreteOutputPorts() const;
   virtual bool setDiscreteOutput(const bool value, const unsigned int channel, const unsigned int port);

   // AI functions
   virtual unsigned short getNumAnalogInputs() const;
   virtual bool getAnalogInput(LCreal* const value, const unsigned int channel) const;

   // AO functions
   virtual unsigned short getNumAnalogOutputs() const;
   virtual bool setAnalogOutput(const LCreal value, const unsigned int channel);

   // Process device input channels and components
   virtual void processInputs(const LCreal dt, IoData* const inData);

   // Process device output channels and components
   virtual void processOutputs(const LCreal dt, const IoData* const outData);

   void reset() override;

protected:
   PairStream* getDeviceList();
   PairStream* getAdapterList();

   // Slot functions
   virtual bool setSlotAdapters(PairStream* const list);
   virtual bool setSlotDevices(PairStream* const list);

   // basic::Component protected functions
   bool shutdownNotification() override;

private:
   void initData();

   safe_ptr<PairStream>  adapters;   // Adapter list
   safe_ptr<PairStream>  devices;    // Device list
};

} // end iodevice namespace
} // end oe namespace

#endif
