// Class: IoData

#include "openeaagles/ioDevice/IoData.h"

#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace IoDevice {

//==============================================================================
// IoData
//==============================================================================
IMPLEMENT_SUBCLASS(IoData,"IoData")

// slot table for this class type
BEGIN_SLOTTABLE(IoData)
    "numAI",     // 1) Number of analog inputs (AIs)
    "numAO",     // 2) Number of analog outputs (AOs)
    "numDI",     // 3) Number of discrete inputs (DIs)
    "numDO",     // 4) Number of discrete outputs (DOs)
END_SLOTTABLE(IoData)

//  Map slot table to handles
BEGIN_SLOT_MAP(IoData)
   ON_SLOT( 1, setSlotNumAI,    Basic::Number)
   ON_SLOT( 2, setSlotNumAO,    Basic::Number)
   ON_SLOT( 3, setSlotNumDI,    Basic::Number)
   ON_SLOT( 4, setSlotNumDO,    Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IoData::IoData()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// initData() -- init member data
//------------------------------------------------------------------------------
void IoData::initData()
{
   numAI = 0;
   aiTable = 0;

   numAO = 0;
   aoTable = 0;

   numDI = 0;
   diTable = 0;

   numDO = 0;
   doTable = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IoData::copyData(const IoData& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setNumAI(org.numAI);
   if (aiTable != 0) {
      for (unsigned int i = 0; i < numAI; i++) aiTable[i] = org.aiTable[i];
   }

   setNumAO(org.numAO);
   if (aoTable != 0) {
      for (unsigned int i = 0; i < numAO; i++) aoTable[i] = org.aoTable[i];
   }

   setNumDI(org.numDI);
   if (diTable != 0) {
      for (unsigned int i = 0; i < numDI; i++) diTable[i] = org.diTable[i];
   }

   setNumDO(org.numDO);
   if (doTable != 0) {
      for (unsigned int i = 0; i < numAI; i++) doTable[i] = org.doTable[i];
   }

}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void IoData::deleteData()
{
   setNumAI(0);
   setNumAO(0);
   setNumDI(0);
   setNumDO(0);
}

// -----------------------------------------------------------------------------
// Default quantity functions
// -----------------------------------------------------------------------------
unsigned int IoData::getNumAnalogInputChannels() const     { return numAI; }
unsigned int IoData::getNumAnalogOutputChannels() const    { return numAO; }
unsigned int IoData::getNumDiscreteInputChannels() const   { return numDI; }
unsigned int IoData::getNumDiscreteOutputChannels() const  { return numDO; }

// -----------------------------------------------------------------------------
// getAnalogInput() - default
// -----------------------------------------------------------------------------
bool IoData::getAnalogInput(const unsigned int channel, LCreal* const value) const
{
   bool ok = false;
   if (value != 0 && aiTable != 0 && channel > 0 && channel <= numAI) {
      *value = aiTable[channel-1];
      ok = true;
   }
   return ok;
}

// -----------------------------------------------------------------------------
// getAnalogOutput() - 
// -----------------------------------------------------------------------------
bool IoData::getAnalogOutput(const unsigned int channel, LCreal* const value) const
{
   bool ok = false;
   if (value != 0 && aoTable != 0 && channel > 0 && channel <= numAO) {
      *value = aoTable[channel-1];
      ok = true;
   }
   return ok;
}

// -----------------------------------------------------------------------------
// getDiscreteInput() - 
// -----------------------------------------------------------------------------
bool IoData::getDiscreteInput(const unsigned int channel, bool* const value) const 
{
   bool ok = false;
   if (value != 0 && diTable != 0 && channel > 0 && channel <= numDI) {
      *value = diTable[channel-1];
      ok = true;
   }
   return ok;
}

// -----------------------------------------------------------------------------
// getDiscreteOutput() - 
// -----------------------------------------------------------------------------
bool IoData::getDiscreteOutput(const unsigned int channel, bool* const value) const
{
   bool ok = false;
   if (value != 0 && doTable != 0 && channel > 0 && channel <= numDO) {
      *value = doTable[channel-1];
      ok = true;
   }
   return ok;
}

// -----------------------------------------------------------------------------
// setAnalogInput() - 
// -----------------------------------------------------------------------------
bool IoData::setAnalogInput(const unsigned int channel, const LCreal value)
{
   bool ok = false;
   if (aiTable != 0 && channel > 0 && channel <= numAI) {
      aiTable[channel-1] = value;
      ok = true;
   }
   return ok;
}

// -----------------------------------------------------------------------------
// setAnalogOutput() - 
// -----------------------------------------------------------------------------
bool IoData::setAnalogOutput(const unsigned int channel, const LCreal value)
{
   bool ok = false;
   if (aoTable != 0 && channel > 0 && channel <= numAO) {
      aoTable[channel-1] = value;
      ok = true;
   }
   return ok;
}

// -----------------------------------------------------------------------------
// setDiscreteInput() - 
// -----------------------------------------------------------------------------
bool IoData::setDiscreteInput(const unsigned int channel, const bool value)
{
   bool ok = false;
   if (diTable != 0 && channel > 0 && channel <= numDI) {
      diTable[channel-1] = value;
      ok = true;
   }
   return ok;
}

// -----------------------------------------------------------------------------
// setDiscreteOutput() - 
// -----------------------------------------------------------------------------
bool IoData::setDiscreteOutput(const unsigned int channel, const bool value)
{
   bool ok = false;
   if (doTable != 0 && channel > 0 && channel <= numDO) {
      doTable[channel-1] = value;
      ok = true;
   }
   return ok;
}

// -----------------------------------------------------------------------------
// Default clear the data
// -----------------------------------------------------------------------------
void IoData::clear()
{
   if (aiTable != 0 && numAI > 0) {
      for (unsigned int i = 0; i < numAI; i++) {
         aiTable[i] = 0;
      }
   }

   if (aoTable != 0 && numAO > 0) {
      for (unsigned int i = 0; i < numAO; i++) {
         aoTable[i] = 0;
      }
   }

   if (diTable != 0 && numDI > 0) {
      for (unsigned int i = 0; i < numDI; i++) {
         diTable[i] = false;
      }
   }

   if (doTable != 0 && numDO > 0) {
      for (unsigned int i = 0; i < numDO; i++) {
         doTable[i] = false;
      }
   }
}

// -----------------------------------------------------------------------------
// Set functions
// -----------------------------------------------------------------------------
bool IoData::setNumAI(const unsigned int num)
{
   // Only if we're changing things ...
   if (num != numAI) {

      // Delete the old
      if (numAI > 0) {
         numAI = 0;
         delete[] aiTable;
         aiTable = 0;
      }

      // Allocate and clear the new
      if (num > 0) {
         aiTable = new LCreal[num];
         for (unsigned int i = 0; i < num; i++) aiTable[i] = 0;
         numAI = num;
      }

   }

   return true;
}

bool IoData::setNumAO(const unsigned int num)
{
   // Only if we're changing things ...
   if (num != numAO) {

      // Delete the old
      if (numAO > 0) {
         numAO = 0;
         delete[] aoTable;
         aoTable = 0;
      }

      // Allocate and clear the new
      if (num > 0) {
         aoTable = new LCreal[num];
         for (unsigned int i = 0; i < num; i++) aoTable[i] = 0;
         numAO = num;
      }

   }

   return true;
}

bool IoData::setNumDI(const unsigned int num)
{
   // Only if we're changing things ...
   if (num != numDI) {

      // Delete the old
      if (numDI > 0) {
         numDI = 0;
         delete[] diTable;
         diTable = 0;
      }

      // Allocate and clear the new
      if (num > 0) {
         diTable = new bool[num];
         for (unsigned int i = 0; i < num; i++) diTable[i] = false;
         numDI = num;
      }

   }

   return true;
}

bool IoData::setNumDO(const unsigned int num)
{
   // Only if we're changing things ...
   if (num != numDO) {

      // Delete the old
      if (numDO > 0) {
         numDO = 0;
         delete[] doTable;
         doTable = 0;
      }

      // Allocate and clear the new
      if (num > 0) {
         doTable = new bool[num];
         for (unsigned int i = 0; i < num; i++) doTable[i] = false;
         numDO = num;
      }

   }

   return true;
}

// -----------------------------------------------------------------------------
// Slot functions
// -----------------------------------------------------------------------------

// numAI: Number of analog inputs (AIs)
bool IoData::setSlotNumAI(const Basic::Number* const msg)
{
   // Null 'msg' is ok, but the value of 'msg' must
   // be greater than or equal to zero.

   bool ok = true;
   unsigned int num = 0;

   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 0) num = (unsigned int) v;
      else ok = false;
   }

   if (ok) ok = setNumAI(num);
   return ok;
}

// numAO: Number of analog outputs (AOs)
bool IoData::setSlotNumAO(const Basic::Number* const msg)
{
   // Null 'msg' is ok, but the value of 'msg' must
   // be greater than or equal to zero.

   bool ok = true;
   unsigned int num = 0;

   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 0) num = (unsigned int) v;
      else ok = false;
   }

   if (ok) ok = setNumAO(num);
   return ok;
}

// numDI: Number of discrete inputs (DIs)
bool IoData::setSlotNumDI(const Basic::Number* const msg)
{
   // Null 'msg' is ok, but the value of 'msg' must
   // be greater than or equal to zero.

   bool ok = true;
   unsigned int num = 0;

   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 0) num = (unsigned int) v;
      else ok = false;
   }

   if (ok) ok = setNumDI(num);
   return ok;
}

// numDO: Number of discrete outputs (DOs)
bool IoData::setSlotNumDO(const Basic::Number* const msg)
{
   // Null 'msg' is ok, but the value of 'msg' must
   // be greater than or equal to zero.

   bool ok = true;
   unsigned int num = 0;

   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 0) num = (unsigned int) v;
      else ok = false;
   }

   if (ok) ok = setNumDO(num);
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Basic::Object* IoData::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& IoData::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   if (numAI > 0) {
      indent(sout,i+j);
      sout << "numAI: " << numAI << std::endl;
   }

   if (numAO > 0) {
      indent(sout,i+j);
      sout << "numAO: " << numAO << std::endl;
   }

   if (numDI > 0) {
      indent(sout,i+j);
      sout << "numDI: " << numDI << std::endl;
   }

   if (numDO > 0) {
      indent(sout,i+j);
      sout << "numDO: " << numDO << std::endl;
   }

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // end IoDevice
} // end Eaagles namespace
