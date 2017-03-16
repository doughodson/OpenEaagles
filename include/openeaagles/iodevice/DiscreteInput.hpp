
#ifndef __oe_iodevice_DiscreteInput_H__
#define __oe_iodevice_DiscreteInput_H__

#include "openeaagles/base/io/IoAdapter.hpp"

namespace oe {
namespace base { class Number; }
namespace iodevice {
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
class DiscreteInput : public base::IoAdapter
{
   DECLARE_SUBCLASS(DiscreteInput, base::IoAdapter)

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
   bool devEnb {};            // Device enabled
   unsigned int location {};  // IoData input bit location
   unsigned int port {};      // Port number
   unsigned int channel {};   // Port's channel (bit) number
   bool value {};             // Initial value of bit.
   bool invert {};            // Inverted bit flag
   int num {1};               // Number of DIs (neg for reverse order)
};

}
}

#endif
