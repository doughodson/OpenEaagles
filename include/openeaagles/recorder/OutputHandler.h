//------------------------------------------------------------------------------
// Class: OutputHandler
//------------------------------------------------------------------------------
#ifndef __oe_recorder_OutputHandler_H__
#define __oe_recorder_OutputHandler_H__

#include "openeaagles/simulation/DataRecorder.h"
#include "openeaagles/base/List.h"

namespace oe {
   namespace base { class List; }

namespace recorder {
   class DataRecordHandle;

//------------------------------------------------------------------------------
// Class: OutputHandler
// Description: Generic output handler for Protocol Buffer data records
//
// Notes:
//    1) Derived class must implement processRecordImp() to process the
//    data record.
//
//    2) the addToQueue() function will save the record for later processing
//    by the processQueue() function.  This allows a time critical thread to
//    create a data record and queue it for later processing by a background
//    thread, which would call the processQueue() function.
//
//    3) Using the 'components' slot, this OutputHandler can manage as list
//    of subcomponent OutputHandlers.  The prcessRecord() function for each
//    subcomponent OutputHandler is called from our processRecord() function.
//
// Factory name: OutputHandler
//
// Overriding the Component class slot:
//    components     ! Must contain only 'OutputHandler' type objects
//
//------------------------------------------------------------------------------
class OutputHandler : public simulation::RecorderComponent
{
   DECLARE_SUBCLASS(OutputHandler, simulation::RecorderComponent)

public:
   OutputHandler();

   // Process the data record
   void processRecord(const DataRecordHandle* const handle);

   // Add the data record to a queue for later processing
   void addToQueue(const DataRecordHandle* const handle);

   // Process all data records from the queue
   void processQueue();

protected:
   // Process record implementations by derived classes
   virtual void processRecordImp(const DataRecordHandle* const handle);

   // Checks the data enabled list and returns true if the record should be processed.
   bool isDataTypeEnabled(const DataRecordHandle* const handle) const;

   virtual void processComponents(
      base::PairStream* const list,        // Source list of components
      const std::type_info& filter,        // Type filter
      base::Pair* const add = 0,           // Optional pair to add
      base::Component* const remove = 0    // Optional subcomponent to remove
   ) override;

   virtual bool shutdownNotification() override;

private:
   void initData();

   base::List queue;         // Data Record Queue
   mutable long semaphore;
};

}
}

#endif
