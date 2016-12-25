
#ifndef __oe_simulation_DataRecorder_H__
#define __oe_simulation_DataRecorder_H__

#include "openeaagles/simulation/RecorderComponent.hpp"
#include "openeaagles/simulation/recorder_macros.hpp"

namespace oe {
namespace base { class List; }
namespace simulation {
class Simulation;
class Station;

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
//    oe::recorder::DataRecorder (see "openeaagles/recorder/DataRecorder.hpp")
//
//    3) Recorded data records are defined by their "recorder event id" tokens;
//       (see openeaagles/simulation/dataRecorderTokens.hpp)
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
      const base::Object* pObjects[4],   // Sample objects
      const double values[4]              // Sample values
   );

   // Background thread processing of the data records
   virtual void processRecords();

protected:
   // Implementation of the record data function
   virtual bool recordDataImp(
      const unsigned int id,              // Recorder event Id
      const base::Object* pObjects[4],   // Sample objects
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

// Record Data function
inline bool DataRecorder::recordData(
      const unsigned int id,
      const base::Object* pObjects[4],
      const double values[4]
   )
{
   bool recorded = false;
   if (isDataEnabled(id)) {
      recorded = recordDataImp(id, pObjects, values);
      if (!recorded) processUnhandledId(id);
   }
   return recorded;
}

}
}

#endif
