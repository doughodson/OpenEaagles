//------------------------------------------------------------------------------
// Class: Radar
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Radar_H__
#define __Eaagles_Simulation_Radar_H__

#include "openeaagles/simulation/RfSensor.h"

namespace Eaagles {
namespace Simulation {

//------------------------------------------------------------------------------
// Class: Radar
// Description: Generic Radar Model
//
// Default R/F sensor type ID is "RADAR"
//
// Factory name: Radar
// Slots:
//    igain    <Basic::Number>     ! Integrator gain (no units; default: 1.0f)
//             <Basic::Decibel>    ! Integrator gain (dB)
//
//------------------------------------------------------------------------------
class Radar : public RfSensor
{
   DECLARE_SUBCLASS(Radar,RfSensor)

public:
   // Max number of reports (per scan)
   static const unsigned int MAX_REPORTS = EAAGLES_CONFIG_MAX_REPORTS;

   static const unsigned int NUM_SWEEPS = 121;          // Number of sweeps in Real-Beam display
   static const unsigned int PTRS_PER_SWEEP = 128;      // Number of points per sweep in RB display

public:
   Radar();

   const LCreal* getSweep(const unsigned int n) const     { return (n < NUM_SWEEPS ?  sweeps[n] : 0); }
   const LCreal* getClosure(const unsigned int n) const   { return (n < NUM_SWEEPS ?  vclos[n]  : 0); }
   unsigned int getNumSweeps() const                      { return NUM_SWEEPS; }
   unsigned int getPtrsPerSweep() const                   { return PTRS_PER_SWEEP; }

   unsigned int getMaxReports() const                     { return MAX_REPORTS; }
   unsigned int getNumReports() const                     { return numReports; }

   // Returns the number of emission reports, up to 'max', that are loaded into the 'list'
   // Emission pointers are pre-ref()'d, so unref() when finished.
   unsigned int getReports(const Emission** list, const unsigned int max) const;

   // For debugging purposes
   // returns the amount of jamming signal to be considered, 0 if no jamming
   LCreal getRecvJamSignal() const                 { return currentJamSignal; }
   LCreal getRecvJamSignalDb() const               { return 10.0f * lcLog10(currentJamSignal); }

   // Returns integration gain
   LCreal getIGain() const                         { return rfIGain; }

   // return the current number of emissions that have been jammed.
   int getNumberOfJammedEmissions() const          { return numberOfJammedEmissions; }

   // Sets integration gain
   virtual bool setIGain(const LCreal);

   // Slot functions
   virtual bool setSlotIGain(Basic::Number* const msg);

   // System Interface -- Event handler(s)
   bool killedNotification(Player* const killedBy = 0) override;

   // Basic::Component interface
   void updateData(const LCreal dt = 0.0) override;
   void reset() override;

protected:
   virtual bool onEndScanEvent(const Basic::Integer* const bar);

   // System class -- phase callbacks
   void transmit(const LCreal dt) override;
   void receive(const LCreal dt) override;
   void process(const LCreal dt) override;

   // Basic::Component protected interface
   bool shutdownNotification() override;

protected: // (#temporary#) allow subclasses to access and use report queue

   // Semaphore to protect 'rptQueue', 'rptSnQueue', 'reports' and 'rptMaxSn'
   mutable long myLock;

   // Queues
   QQueue<Emission*>   rptQueue;       // Reporting emission queue
   QQueue<LCreal>      rptSnQueue;     // Reporting Signal/Nose queue  (dB)

   // Reports
   Emission*   reports[MAX_REPORTS];   // Best emission for this report
   LCreal      rptMaxSn[MAX_REPORTS];  // Signal/Nose value            (dB)
   unsigned int numReports;            // Number of reports this sweep

private:
   void initData();
   void clearTracksAndQueues();
   void clearSweep(const unsigned int i);
   void ageSweeps();
   unsigned int computeSweepIndex(const LCreal az);
   unsigned int computeRangeIndex(const LCreal rng);

   bool        endOfScanFlg;           // End of scan flag

   LCreal      sweeps[NUM_SWEEPS][PTRS_PER_SWEEP];
   LCreal      vclos[NUM_SWEEPS][PTRS_PER_SWEEP];
   int         csweep;                     // Current sweep

   LCreal      currentJamSignal;
   int         numberOfJammedEmissions;

   LCreal      rfIGain;                // Integrator gain (default: 1.0) (no units)
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
