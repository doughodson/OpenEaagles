
#include "openeaagles/recorder/OutputHandler.h"
#include "openeaagles/recorder/DataRecordHandle.h"
#include "openeaagles/recorder/DataRecord.pb.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Recorder {

//==============================================================================
// Class OutputHandler
//==============================================================================
IMPLEMENT_SUBCLASS(OutputHandler,"RecorderOutputHandler")
EMPTY_SLOTTABLE(OutputHandler)
EMPTY_SERIALIZER(OutputHandler)


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
OutputHandler::OutputHandler()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void OutputHandler::initData()
{
   semaphore = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void OutputHandler::copyData(const OutputHandler& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // Don't copy the queue
   lcLock(semaphore);
   queue.clear();
   lcUnlock(semaphore);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void OutputHandler::deleteData()
{
   // clear the queue
   lcLock(semaphore);
   queue.clear();
   lcUnlock(semaphore);
}


//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool OutputHandler::shutdownNotification()
{
   // Pass the shutdown notification to our subcomponent recorders
   Basic::PairStream* subcomponents = getComponents();
   if (subcomponents != 0) {
      for (Basic::List::Item* item = subcomponents->getFirstItem(); item != 0; item = item->getNext()) {
         Basic::Pair* pair = (Basic::Pair*)(item->getValue());
         OutputHandler* sc = (OutputHandler*) pair->object();
         sc->event(SHUTDOWN_EVENT);
      }
      subcomponents->unref();
      subcomponents = 0;
   }

   return BaseClass::shutdownNotification();
}


//------------------------------------------------------------------------------
// Pass the data record to all of our subcomponents for processing; they all
// should be of type OutputHandler (see processComponents() above)
//------------------------------------------------------------------------------
void OutputHandler::processRecord(const DataRecordHandle* const dataRecord)
{
   // Check the data filters to see if we should process this type record
   if ( isDataTypeEnabled(dataRecord) ) {

      // First, call our own implementation
      processRecordImp(dataRecord);

      // Next, pass the data record to our subcomponent OutputHandlers
      // for futher processing
      Basic::PairStream* subcomponents = getComponents();
      if (subcomponents != 0) {
         for (Basic::List::Item* item = subcomponents->getFirstItem(); item != 0; item = item->getNext()) {

            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            OutputHandler* sc = (OutputHandler*) pair->object();

            sc->processRecord(dataRecord);

         }
         subcomponents->unref();
         subcomponents = 0;
      }

   }
}


//------------------------------------------------------------------------------
// Queue the data record handle to be processed later
//------------------------------------------------------------------------------
void OutputHandler::addToQueue(const DataRecordHandle* const dataRecord)
{
   if (dataRecord != 0) {
      lcLock( semaphore );
      queue.put( (DataRecordHandle*) dataRecord);  // const cast away to put into the queue
      lcUnlock( semaphore );
   }
}


//------------------------------------------------------------------------------
// Process all data records in the queue.
//------------------------------------------------------------------------------
void OutputHandler::processQueue()
{
   // Get the first record from the queue
   lcLock( semaphore );
   const DataRecordHandle* dataRecord = (const DataRecordHandle*) queue.get();
   lcUnlock( semaphore );

   // While we have records ...
   while (dataRecord != 0) {
      // process this record, 
      processRecord(dataRecord);
      dataRecord->unref();

      // and get the next one from the queue
      lcLock( semaphore );
      dataRecord = (const DataRecordHandle*) queue.get();
      lcUnlock( semaphore );
   }
}


//------------------------------------------------------------------------------
// processRecordImp() stub
//------------------------------------------------------------------------------
void OutputHandler::processRecordImp(const DataRecordHandle* const)
{
}


//------------------------------------------------------------------------------
// Check the data filters and return true if we should process this type message
//------------------------------------------------------------------------------
bool OutputHandler::isDataTypeEnabled(const DataRecordHandle* const handle) const
{
   unsigned int id = handle->getRecord()->id();
   return isDataEnabled(id);
}


//------------------------------------------------------------------------------
//  make sure our subcomponents are all of type OutputHandler (or derived)
//------------------------------------------------------------------------------
void OutputHandler::processComponents(
      Basic::PairStream* const list,
      const std::type_info&,
      Basic::Pair* const add,
      Basic::Component* const remove
   )
{
   BaseClass::processComponents(list,typeid(OutputHandler),add,remove);
}

} // End Recorder namespace
} // End Eaagles namespace
