
#include "openeaagles/recorder/InputHandler.h"
#include "openeaagles/recorder/DataRecordHandle.h"
#include "openeaagles/recorder/DataRecord.pb.h"

namespace Eaagles {
namespace Recorder {

//==============================================================================
// Class InputHandler
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(InputHandler,"RecorderInputHandler")
EMPTY_SLOTTABLE(InputHandler)
EMPTY_SERIALIZER(InputHandler)
EMPTY_CONSTRUCTOR(InputHandler);
EMPTY_COPYDATA(InputHandler)
EMPTY_DELETEDATA(InputHandler)

//------------------------------------------------------------------------------
// Read one data record; returns zero if no record is available
//------------------------------------------------------------------------------
const DataRecordHandle* InputHandler::readRecord()
{
   const DataRecordHandle* p = 0;
   bool finished = false;

   while (!finished) {

      // Get a record from our derived class
      p = readRecordImp();

      // Did we get one?
      if (p != 0) {
         // Yes -- now check to see if this data record is enabled
         unsigned int id = p->getRecord()->id();
         bool enabled = isDataEnabled(id);

         if (enabled) finished = true;    // If it is then we're done
         else { p->unref(); p = 0; }      // If itis disabled then we'll try again
      }

      // When we didn't receive a record then we're done for now and they
      // can try again later
      else finished = true;

   }

   return p;
}

} // End Recorder namespace
} // End Eaagles namespace
