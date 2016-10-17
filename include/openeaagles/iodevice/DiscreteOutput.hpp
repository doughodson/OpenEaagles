
#ifndef __oe_iodevice_DiscreteOutput_H__
#define __oe_iodevice_DiscreteOutput_H__

#include "openeaagles/base/IoAdapter.hpp"

namespace oe {

namespace base { class NetHandler; class Number; }

namespace iodevice {

class IoDevice;

//------------------------------------------------------------------------------
// Class:  DiscreteOutput
//
// Description:  Manage discrete outputs (DO).
//
// Slots:
//      do           <Number>    Discrete Output location (IoData's DO channel)
//      port         <Number>    Device port number (default: 0)
//      channel      <Number>    Device channel (bit) number on the port
//      value        <Boolean>   Initial value (default: false)
//      inverted     <Boolean>   Inverted bit flag (default: false)
//      num          <Number>    Number of DOs to manage starting at 'do' and
//                               'channel'; port is unchanged (default: 1)
//
// Note: If 'num' is less than zero then the DOs are stored in reverse
//       order (i.e., 'do' location is decremented).
//------------------------------------------------------------------------------
class DiscreteOutput : public base::IoAdapter
{
   DECLARE_SUBCLASS(DiscreteOutput, base::IoAdapter)

public:
   DiscreteOutput();

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

   virtual void processInputs(const double dt, const base::IoDevice* const device, base::IoData* const inData) override;
   virtual void processOutputs(const double dt, const base::IoData* const outData, base::IoDevice* const device) override;

protected:
   // Slot functions
   virtual bool setSlotLocation(const base::Number* const msg);
   virtual bool setSlotPort(const base::Number* const msg);
   virtual bool setSlotChannel(const base::Number* const msg);
   virtual bool setSlotValue(const base::Number* const msg);
   virtual bool setSlotInverted(const base::Number* const msg);
   virtual bool setSlotNum(const base::Number* const msg);

private:
   void initData();

   bool devEnb;            // Device enabled
   unsigned int location;  // IoData output bit location
   unsigned int port;      // Port number
   unsigned int channel;   // Port's channel (bit) number
   bool value;             // Initial value of bit.
   bool invert;            // Inverted bit flag
   int num;                // Number of DIs (neg for reverse order)
};

}
}

#endif
