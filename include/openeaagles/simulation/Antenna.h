//------------------------------------------------------------------------------
// Class: Antenna
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Antenna_H__
#define __Eaagles_Simulation_Antenna_H__

#include "openeaagles/simulation/ScanGimbal.h"

namespace Eaagles {
   namespace Basic {
      class Function;
      class Power;
   }
namespace Simulation {

class Player;
class RfSystem;
//------------------------------------------------------------------------------
// Class: Antenna
//
// Description: Simple Antenna model: provides rate & position servo
//              control, as well as 1, 2 and 4 bar scans
//
//
// Form name: Antenna
// Slots:
//      polarization    <Basic::String>  ! Antenna Polarization  { none, vertical, horizontal, slant, RHC, LHC }
//                                       ! (default: NONE)
//      threshold       <Basic::Power>   ! Antenna threshold  (default: 0.0)
//      gain            <Basic::Number>  ! Gain (default: 1.0)              (no units)
//      gainPattern     <Basic::Function> ! Gain pattern (Basic::Func1 or Basic::Func2) (db) 
//                                       ! (default: 0)
//      gainPatternDeg  <Basic::Number>  ! Gain pattern is in degrees (true) or radians (false) (default: false)
//
//
// Other defaults:
//      maxPlayersOfInterest: 200
//      playerOfInterestTypes: { "air" "weapon" } 
//
//------------------------------------------------------------------------------
class Antenna : public ScanGimbal  
{
   DECLARE_SUBCLASS(Antenna,ScanGimbal)

public:
   enum Polarization { NONE, VERTICAL, HORIZONTAL, SLANT, RHC, LHC };

public:
   Antenna();

   virtual void rfTransmit(Emission* const em);

   RfSystem* getSystem()                       { return sys; }
   const RfSystem* getSystem() const           { return sys; }
   virtual bool setSystem(RfSystem* const p);

   // System limits
   int getMaxEmissions() const                 { return MAX_EMISSIONS; }

   // Antenna polarization matching gain
   double getPolarizationGain(const Polarization p1) const;
   Polarization getPolarization() const        { return polar; }

   // Antenna gain
   virtual double getGain() const              { return gain; }

   // Gain pattern 
   const Basic::Function* gainPatternTable() const { return gainPattern; }
   bool isGainPatternDegrees() const           { return gainPatternDeg; }

   // Antenna threshold (watts)
   double getTransmitThreshold() const         { return threshold; }

   // Antenna effective area (m^2)
   static double getEffectiveArea(const double gain, const double lambda) {
      return (gain * lambda * lambda)/(4.0 * PI);
   }

   // Member functions
   virtual bool setPolarization(const Polarization p) { polar = p; return true; }
   virtual bool setThreshold(const double th);
   virtual bool setGain(double const g);

   // slot functions that need public access because there is no corresponding member function
   virtual bool setSlotPolarization(Basic::String* const v);
   virtual bool setSlotThreshold(Basic::Power* const p);
   virtual bool setSlotGain(const Basic::Number* const g);
   virtual bool setSlotGainPattern(Basic::Function* const func);
   virtual bool setSlotGainPatternDeg(const Basic::Number* const g);

   // Event handler(s)
   virtual bool onRfEmissionReturnEventAntenna(Emission* const);

   // ScanGimbal Interface -- Event handler(s)
   virtual bool onStartScanEvent(Basic::Integer* const bar);
   virtual bool onEndScanEvent(Basic::Integer* const bar);

   // Gimbal Interface -- Event handler(s)
   virtual bool onRfEmissionEvent(Emission* const);

   // Component Interface
   virtual bool event(const int event, Basic::Object* const obj = 0);
   virtual void reset();

protected:
   void clearQueues();

   // System class Interface -- phase() callbacks
   virtual void process(const LCreal dt);     // Phase 3

   // Basic::Component protected interface
   virtual bool shutdownNotification();

   QStack<Emission*> freeEmStack;  // Free emission stack
   mutable long      freeEmLock;   // Semaphore to protect 'freeEmStack'

   QQueue<Emission*> inUseEmQueue; // In use emission queue
   mutable long      inUseEmLock;  // Semaphore to protect 'inUseEmQueue'

private:
   void initData();

   static const int MAX_EMISSIONS = 10000;   // Max size of emission queues and arrays

   RfSystem*    sys;               // Assigned R/F system (e.g., sensor, radio)

   // Antenna parameters
   Polarization polar;             // Polarization                 (enum)
   double      gain;               // Gain                         (no units)
   Basic::Function* gainPattern;   // Gain pattern                 (Function)

   double      threshold;          // Antenna threshold; don't send emission if
                                   // power is below this threshold (watts)

   bool        gainPatternDeg;     // Gain pattern is in degrees flag (else radians)
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
