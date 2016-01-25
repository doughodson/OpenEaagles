//------------------------------------------------------------------------------
// Class: InputHandler
//------------------------------------------------------------------------------
#ifndef __oe_recorder_InputHandler_H__
#define __oe_recorder_InputHandler_H__

#include "openeaagles/simulation/DataRecorder.h"

namespace oe {
namespace recorder {
   class DataRecordHandle;

//------------------------------------------------------------------------------
// Class:   InputHandler
// Description: Read and parse data records from an input source
//------------------------------------------------------------------------------
class InputHandler : public Simulation::RecorderComponent
{
    DECLARE_SUBCLASS(InputHandler, Simulation::RecorderComponent)

public:
   InputHandler();

   // Read one data record; returns zero if no record is available
   const DataRecordHandle* readRecord();

protected:
   // Read one record from our data source
   // -- Must be implemented by our derived classes
   virtual const DataRecordHandle* readRecordImp() =0;
};

} // End recorder namespace
} // End oe namespace

#endif
