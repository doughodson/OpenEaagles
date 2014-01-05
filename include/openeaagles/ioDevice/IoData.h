//------------------------------------------------------------------------------
// Class: IoData
//------------------------------------------------------------------------------
#ifndef __Eaagles_IoDevice_IoData_H__
#define __Eaagles_IoDevice_IoData_H__

#include "openeaagles/basic/IoData.h"

namespace Eaagles {
   namespace Basic { class Number; }

namespace IoDevice {

//------------------------------------------------------------------------------
// Class:  IoData
//
// Description:  General purpose I/O data buffer; users can specify the number
//               channels for each I/O type.
//
// Note:
//    *** Channel numbers are all one(1) based.  For example, the range of
//    AI channels is from one to getNumAnalogInputChannels(). ***
//
// Factory name: IoData
// Slots:
//    numAI    <Number>   ! Number of analog inputs (AIs)
//    numAO    <Number>   ! Number of analog outputs (AOs)
//    numDI    <Number>   ! Number of discrete inputs (DIs)
//    numDO    <Number>   ! Number of discrete outputs (DOs)
//
//------------------------------------------------------------------------------
class IoData : public Basic::IoData {
   DECLARE_SUBCLASS(IoData,Basic::IoData)

public:
   IoData();

   bool setNumAI(const unsigned int num);
   bool setNumAO(const unsigned int num);
   bool setNumDI(const unsigned int num);
   bool setNumDO(const unsigned int num);

   // Basic::IoData interface functions
   virtual unsigned int getNumAnalogInputChannels() const;
   virtual unsigned int getNumAnalogOutputChannels() const;
   virtual unsigned int getNumDiscreteInputChannels() const;
   virtual unsigned int getNumDiscreteOutputChannels() const;
   virtual bool getAnalogInput(const unsigned int channel, LCreal* const value) const;
   virtual bool getAnalogOutput(const unsigned int channel, LCreal* const value) const;
   virtual bool getDiscreteInput(const unsigned int channel, bool* const value) const;
   virtual bool getDiscreteOutput(const unsigned int channel, bool* const value) const;
   virtual bool setAnalogInput(const unsigned int channel, const LCreal value);
   virtual bool setAnalogOutput(const unsigned int channel, const LCreal value);
   virtual bool setDiscreteInput(const unsigned int channel, const bool value);
   virtual bool setDiscreteOutput(const unsigned int channel, const bool value);
   virtual void clear();

protected:
   bool setSlotNumAI(const Basic::Number* const msg);
   bool setSlotNumAO(const Basic::Number* const msg);
   bool setSlotNumDI(const Basic::Number* const msg);
   bool setSlotNumDO(const Basic::Number* const msg);

private:
   void initData();

   unsigned int numAI;  // Number of AIs
   LCreal*  aiTable;    // AIs

   unsigned int numAO;  // Number of AOs
   LCreal*  aoTable;    // AOs

   unsigned int numDI;  // Number of DIs
   bool*    diTable;    // DIs

   unsigned int numDO;  // Number of DOs
   bool*    doTable;    // DOs
};

} // end IoDevice
} // end Eaagles namespace

#endif
