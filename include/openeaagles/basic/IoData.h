//------------------------------------------------------------------------------
// Class: IoData
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_IoData_H__
#define __Eaagles_Basic_IoData_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:  IoData
// Description:  I/O data buffer
//
//    Common interface to I/O channels that are used to hold data coming from
//    and going to our I/O devices.  These I/O channels contain applications
//    specific I/O items that are independent of the actual I/O device channels.
//    Derived classes will handle the application specific details.
//
//    For a standard I/O data buffer, see "openeaagles/ioDevice/IoData.h".
//
//    The main input and output data buffers are typically managed by an I/O
//    handler class (see IoHander.h).
//
// Notes:
//    1) Inputs from hardware are called Analog Inputs (AI) and Discrete Inputs (DI)
//
//    2) Outputs to hardware are called Analog Outputs (AO) and Discrete Outputs (DO)
//
//    3) Channel numbering is application dependent; some applications may start
//       numbering at one(1) and some may start at zero(0).
//
//    4) All functions return true if successful.
//
//
// Form name: GenericIoData
//
//------------------------------------------------------------------------------
class IoData : public Object {
   DECLARE_SUBCLASS(IoData,Object)

public:
   IoData();

   // ---
   // Input channels
   // ---
   virtual unsigned int getNumAnalogInputChannels() const;
   virtual unsigned int getNumDiscreteInputChannels() const;

   // Returns the value of the analog input 'channel' in 'value'
   virtual bool getAnalogInput(const unsigned int channel, LCreal* const value) const;

   // Returns the state of the discrete input bit 'channel' in 'value'
   virtual bool getDiscreteInput(const unsigned int channel, bool* const value) const;

   // Sets an analog input 'channel' to 'value'.
   virtual bool setAnalogInput(const unsigned int channel, const LCreal value);

   // Sets the discrete input 'channel' to 'value'.
   virtual bool setDiscreteInput(const unsigned int channel, const bool value);

   // Process the input channels, if needed (application specific).
   // Called by the IoHandler just after the input devices are processed.
   virtual bool processInputs();

   // ---
   // Output channels
   // ---

   virtual unsigned int getNumDiscreteOutputChannels() const;
   virtual unsigned int getNumAnalogOutputChannels() const;

   // Returns the value of the analog output 'channel' in 'value'
   virtual bool getAnalogOutput(const unsigned int channel, LCreal* const value) const;

   // Returns the state of the output bit 'channel' in 'value'
   virtual bool getDiscreteOutput(const unsigned int channel, bool* const value) const;

   // Sets an analog output 'channel' to 'value'.
   virtual bool setAnalogOutput(const unsigned int channel, const LCreal value);

   // Sets the discrete output 'channel' to 'value'.
   virtual bool setDiscreteOutput(const unsigned int channel, const bool value);

   // Process the output channels, if needed (application specific).
   // Called by the IoHandler just before the output devices are processed.
   virtual bool processOutputs();

   // ---
   // Support functions
   // ---

   // Initialize the network (if any)
   virtual bool initNetwork();

   // Clear our data buffers
   virtual void clear();
};

} // end Basic namespace
} // end Eaagles namespace

#endif
