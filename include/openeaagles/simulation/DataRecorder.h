//------------------------------------------------------------------------------
// Classes:
//    DataRecorder      -- Abstract data recorder class
//    RecorderComponent -- Base class for all data recorder components
//
// Macros used to sample data
//
//       BEGIN_RECORD_DATA_SAMPLE(pRecorder, token)
//          Starts a record data sample section
//          'pRecorder' is a pointer to the data recorder
//          'token' is the recorder event token (see openeaagles/simulation/recorderTokens.h)
//
//       END_RECORD_DATA_SAMPLE()
//          Completes the record data sample section
//
//       SAMPLE_1_OBJECT(  P1 )
//       SAMPLE_2_OBJECTS( P1, P2 )
//       SAMPLE_3_OBJECTS( P1, P2, P3 )
//       SAMPLE_4_OBJECTS( P1, P2, P3, P4 )
//          Sample data from 1 to 4 objects
//
//       SAMPLE_1_VALUE(  V1 )
//       SAMPLE_2_VALUES( V1, V2 )
//       SAMPLE_3_VALUES( V1, V2, V3 )
//       SAMPLE_4_VALUES( V1, V2, V3, V4 )
//          From 1 to 4 sample values
//
// Examples:
//
//    Simple event
//          BEGIN_RECORD_DATA_SAMPLE( getDataRecorder(), REID_SIMPLE_EVENT )
//          END_RECORD_DATA_SAMPLE()
//
//    Event with a single object
//          BEGIN_RECORD_DATA_SAMPLE( getDataRecorder(), REID_SMALL_EVENT )
//             SAMPLE_1_OBJECT( newPlayer )
//          END_RECORD_DATA_SAMPLE()
//
//    Event with several objects and values
//          BEGIN_RECORD_DATA_SAMPLE( getDataRecorder(), REID_BIG_EVENT )
//             SAMPLE_2_OBJECTS( ownship, tgtPlayer )
//             SAMPLE_2_VALUES( someValue, anotherValue )
//          END_RECORD_DATA_SAMPLE()
//
//  Notes:
//    1) If the pointer to the Data Recorder is zero (null)
//       then no data is recorded
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_DataRecorder_H__
#define __Eaagles_Simulation_DataRecorder_H__

#include "openeaagles/basic/Component.h"
#include "openeaagles/simulation/dataRecorderTokens.h"

namespace Eaagles {
   namespace Basic { class List; }
namespace Simulation {
   class Simulation;
   class Station;

//------------------------------------------------------------------------------
// Class: RecorderComponent
// Description: Base class for all data recorder components
//
// Notes:
//    1) Use the setEnabledList() function to limit the processing to only
//    DataRecords with matching recorder event IDs.  Default is to process
//    all DataRecords.
//
//
// Slots:
//    enabledList <list>         ! List of data records that are enabled for processing
//                               ! Overrides the disabledList!
//                               ! (default: all records are enabled -- except those
//                               !  listed in 'disabledList')
//
//    disabledList <list>        ! List of data records that are disabled from processing
//                               ! Only valid if 'enabledList' is NOT set!
//                               ! (default: no records are disabled)
//
//------------------------------------------------------------------------------
class RecorderComponent : public Basic::Component
{
   DECLARE_SUBCLASS(RecorderComponent, Basic::Component)

public:
   RecorderComponent();

   // Checks the data filters and returns true if the record should be processed.
   bool isDataEnabled(const unsigned int id) const;

   // Set a list of 'n' of data records enabled for processing,
   // or set 'n' to zero to enable all data records.
   bool setEnabledList(const unsigned int* const list, const unsigned int n);

   // Set a list of 'n' of data records disabled from being processed
   bool setDisabledList(const unsigned int* const list, const unsigned int n);

protected:
   // Slot functions
   bool setSlotEnabledList(const Basic::List* const list);
   bool setSlotDisabledList(const Basic::List* const list);

private:
   void initData();

   unsigned int* enabledList;       // List of data records enabled for processing (default: all)
   unsigned int numEnabled;         // Number of enabled record IDs, or zero for all records enabled

   unsigned int* disabledList;      // List of data records disabled from being processed (default: none)
   unsigned int numDisabled;        // Number of disabled record IDs
};


//------------------------------------------------------------------------------
// Class: DataRecorder
// Description: Abstract data recorder
//
// Slots:
//
// Notes:
//    1) This as an abstract class for the data record that acts as a stub class
//    for implementing the recording 'hooks' in the simulation code.
//
//    2) The actual data recorder is implemented by the derived class
//    Eaagles::Recorder::DataRecorder (see "openeaagles/recorder/DataRecorder.h")
//
//    3) Recorded data records are defined by their "recorder event id" tokens;
//       (see openeaagles/simulation/dataRecorderTokens.h)
//------------------------------------------------------------------------------
class DataRecorder : public RecorderComponent
{
   DECLARE_SUBCLASS(DataRecorder, RecorderComponent)

public:
   DataRecorder();

   Station* getStation();                    // Our parent station
   const Station* getStation() const;        // Our parent station (const version)

   Simulation* getSimulation();              // The simulation
   const Simulation* getSimulation() const;  // The simulation (const version)

   // Record Data function
   bool recordData(
      const unsigned int id,              // Recorder event Id
      const Basic::Object* pObjects[4],   // Sample objects
      const double values[4]              // Sample values
   );

   // Background thread processing of the data records
   virtual void processRecords();

protected:
   // Implementation of the record data function
   virtual bool recordDataImp(
      const unsigned int id,              // Recorder event Id
      const Basic::Object* pObjects[4],   // Sample objects
      const double values[4]              // Sample values
   );

   // Process the unhandled or unknown recorder event IDs
   virtual bool processUnhandledId(const unsigned int id) =0;

private:
   void initData();
   Station* getStationImp();
   Simulation* getSimulationImp();

   Station* sta;     // The Station that owns us (not ref()'d)
   Simulation* sim;     // The simulation system (not ref()'d)
};

#include "openeaagles/simulation/DataRecorder.inl"

} // End Simulation namespace
} // End Eaagles namespace

#endif

