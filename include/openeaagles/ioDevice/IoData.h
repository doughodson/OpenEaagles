
#ifndef __oe_IoDevice_IoData_H__
#define __oe_IoDevice_IoData_H__

#include "openeaagles/basic/IoData.h"

namespace oe {

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
class IoData : public Basic::IoData
{
   DECLARE_SUBCLASS(IoData,Basic::IoData)

public:
   IoData();

   bool setNumAI(const unsigned int num);
   bool setNumAO(const unsigned int num);
   bool setNumDI(const unsigned int num);
   bool setNumDO(const unsigned int num);

   unsigned int getNumAnalogInputChannels() const override;
   unsigned int getNumAnalogOutputChannels() const override;
   unsigned int getNumDiscreteInputChannels() const override;
   unsigned int getNumDiscreteOutputChannels() const override;
   bool getAnalogInput(const unsigned int channel, LCreal* const value) const override;
   bool getAnalogOutput(const unsigned int channel, LCreal* const value) const override;
   bool getDiscreteInput(const unsigned int channel, bool* const value) const override;
   bool getDiscreteOutput(const unsigned int channel, bool* const value) const override;
   bool setAnalogInput(const unsigned int channel, const LCreal value) override;
   bool setAnalogOutput(const unsigned int channel, const LCreal value) override;
   bool setDiscreteInput(const unsigned int channel, const bool value) override;
   bool setDiscreteOutput(const unsigned int channel, const bool value) override;
   void clear() override;

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
} // end oe namespace

#endif
