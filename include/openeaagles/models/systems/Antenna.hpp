
#ifndef __oe_models_Antenna_H__
#define __oe_models_Antenna_H__

#include "openeaagles/models/systems/ScanGimbal.hpp"

#include "openeaagles/base/safe_queue.hpp"
#include "openeaagles/base/safe_stack.hpp"
#include "openeaagles/base/util/constants.hpp"

namespace oe {
   namespace base {
      class Function;
      class Power;
   }

namespace models {

class Player;
class RfSystem;

//------------------------------------------------------------------------------
// Class: Antenna
//
// Description: Simple Antenna model: provides rate & position servo
//              control, as well as 1, 2 and 4 bar scans
//
//
// Factory name: Antenna
// Slots:
//      polarization    <base::String>  ! Antenna Polarization  { none, vertical, horizontal, slant, RHC, LHC }
//                                       ! (default: NONE)
//
//      threshold       <base::Power>   ! Antenna threshold  (default: 0.0)
//
//      gain            <base::Number>  ! Gain (default: 1.0)              (no units)
//
//      gainPattern     <base::Function> ! Gain pattern (base::Func1 or base::Func2) (db)
//                                       ! (default: 0)
//
//      gainPatternDeg  <base::Boolean> ! Gain pattern is in degrees (true) or radians (false) (default: false)
//
//      recycle         <base::Boolean> ! Recycle emissions flag (default: true)
//
//      beamWidth       <base::Angle>   ! Beam Width  (must be greater than zero) (default: 3.5 degrees)
//                      <base::Number>  ! Beam width in radians
//
//
// Note
//    1) Other defaults:
//          maxPlayersOfInterest: 200
//          playerOfInterestTypes: { "air" "weapon" }
//
//    2) When the Emission 'recycle' flag is enabled (default behavior), the
//       system will try to reuse Emission objects, which removes the overhead
//       of creating and deleting them.
//
//------------------------------------------------------------------------------
class Antenna : public ScanGimbal
{
   DECLARE_SUBCLASS(Antenna, ScanGimbal)

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
   const base::Function* gainPatternTable() const { return gainPattern; }
   bool isGainPatternDegrees() const           { return gainPatternDeg; }

   // Antenna threshold (watts)
   double getTransmitThreshold() const         { return threshold; }

   // Antenna effective area (m^2)
   static double getEffectiveArea(const double gain, const double lambda) {
      return (gain * lambda * lambda)/(4.0 * base::PI);
   }

   // Recycle emissions flag (reuse old emission structure instead of creating new ones)
   bool isEmissionRecycleEnabled() const       { return recycle; }

   // Beam width (radians)
   double getBeamWidth() const                 { return beamWidth; }

   // Member functions
   virtual bool setPolarization(const Polarization p) { polar = p; return true; }
   virtual bool setThreshold(const double th);
   virtual bool setGain(const double g);
   virtual bool setEmissionRecycleFlag(const bool enable);
   virtual bool setBeamWidth(const double radians);

   // slot functions that need public access because there is no corresponding member function
   virtual bool setSlotPolarization(base::String* const v);
   virtual bool setSlotThreshold(base::Power* const p);
   virtual bool setSlotGain(const base::Number* const g);
   virtual bool setSlotGainPattern(base::Function* const func);
   virtual bool setSlotGainPatternDeg(const base::Number* const g);
   virtual bool setSlotRecycleFlg(const base::Number* const);
   virtual bool setSlotBeamWidth(const base::Angle* const msg);
   virtual bool setSlotBeamWidth(const base::Number* const msg);

   // Event handler(s)
   virtual bool onRfEmissionReturnEventAntenna(Emission* const);

   virtual bool onStartScanEvent(base::Integer* const bar) override;
   virtual bool onEndScanEvent(base::Integer* const bar) override;

   virtual bool onRfEmissionEvent(Emission* const) override;

   virtual bool event(const int event, base::Object* const obj = nullptr) override;
   virtual void reset() override;

protected:
   void clearQueues();

   virtual void process(const double dt) override;     // Phase 3

   virtual bool shutdownNotification() override;

   base::safe_stack<Emission*> freeEmStack;  // Free emission stack
   mutable long freeEmLock;                  // Semaphore to protect 'freeEmStack'

   base::safe_queue<Emission*> inUseEmQueue; // In use emission queue
   mutable long inUseEmLock;                 // Semaphore to protect 'inUseEmQueue'

private:
   void initData();

   static const int MAX_EMISSIONS = 10000;   // Max size of emission queues and arrays

   RfSystem*    sys;               // Assigned R/F system (e.g., sensor, radio)

   // Antenna parameters
   Polarization polar;             // Polarization                 (enum)
   double      gain;               // Gain                         (no units)
   base::Function* gainPattern;    // Gain pattern                 (Function)

   double      threshold;          // Antenna threshold; don't send emission if
                                   // power is below this threshold (watts)

   double      beamWidth;          // Beamwidth                    (radians)

   bool        gainPatternDeg;     // Gain pattern is in degrees flag (else radians)

   bool        recycle;            // Recycle emissions flag
};

}
}

#endif
