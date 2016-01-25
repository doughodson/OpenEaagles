
#ifndef __oe_ioDevice_DiscreteInput_H__
#define __oe_ioDevice_DiscreteInput_H__

#include "openeaagles/basic/IoAdapter.h"

namespace oe {
   namespace basic { class Number; }

namespace ioDevice {
   class IoDevice;

//------------------------------------------------------------------------------
// Class:  DiscreteInput
//
// Description:  Manage discrete inputs (DI).
//
// Slots:
//      di           <Number>    Discrete Input location (IoData's DI channel)
//      channel      <Number>    Device channel (bit) number on the port
//      port         <Number>    Device port number (default: 0)
//      value        <Boolean>   Initial value (default: false)
//      inverted     <Boolean>   Inverted bit flag (default: false)
//      num          <Number>    Number of DIs to manage starting at 'di' and
//                               'channel'; port is unchanged (default: 1)
//
// Note: If 'num' is less than zero then the DIs are stored in reverse
//       order (i.e., 'di' location is decremented).
//------------------------------------------------------------------------------
class DiscreteInput : public basic::IoAdapter
{
   DECLARE_SUBCLASS(DiscreteInput, basic::IoAdapter)

public:
   DiscreteInput();

   unsigned int getLocation() const;
   unsigned int getPort() const;
   unsigned int getChannel() const;
   bool getValue() const;
   bool getInvertFlag() const;
   int getNumber() const;

   bool setLocation(const unsigned int);
   bool setPort(const unsigned int);
   bool setChannel(const unsigned int);
   bool setValue(const bool);
   bool setInvertFlag(const bool);
   bool setNumber(const int);

   void processInputs(const LCreal dt, const basic::IoDevice* const device, basic::IoData* const inData) override;
   void processOutputs(const LCreal dt, const basic::IoData* const outData, basic::IoDevice* const device) override;

protected:
   // Slot functions
   virtual bool setSlotLocation(const basic::Number* const msg);
   virtual bool setSlotPort(const basic::Number* const msg);
   virtual bool setSlotChannel(const basic::Number* const msg);
   virtual bool setSlotValue(const basic::Number* const msg);
   virtual bool setSlotInverted(const basic::Number* const msg);
   virtual bool setSlotNum(const basic::Number* const msg);

private:
   void initData();

   bool devEnb;            // Device enabled
   unsigned int location;  // IoData input bit location
   unsigned int port;      // Port number
   unsigned int channel;   // Port's channel (bit) number
   bool value;             // Initial value of bit.
   bool invert;            // Inverted bit flag
   int num;                // Number of DIs (neg for reverse order)
};

} // end ioDevice
} // end oe namespace

#endif
