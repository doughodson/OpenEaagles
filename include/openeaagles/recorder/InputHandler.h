//------------------------------------------------------------------------------
// Class: InputHandler
//------------------------------------------------------------------------------
#ifndef __Eaagles_Recorder_InputHandler_H__
#define __Eaagles_Recorder_InputHandler_H__

#include "openeaagles/simulation/DataRecorder.h"

namespace Eaagles {
namespace Recorder {
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

} // End Recorder namespace
} // End Eaagles namespace

#endif
