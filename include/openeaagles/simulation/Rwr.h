//------------------------------------------------------------------------------
// Class: Rwr
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_Rwr_H__
#define __oe_Simulation_Rwr_H__

#include "openeaagles/simulation/RfSensor.h"
#include "openeaagles/basic/safe_queue.h"

namespace oe {
namespace Simulation {
//------------------------------------------------------------------------------
// Class: Rwr
// Base class: Basic::Object -> ... -> RfSystem -> RfSensor -> Rwr
//
// Description: General Radar Warning Receiver (RWR) Model
// Factory name: Rwr
//------------------------------------------------------------------------------
class Rwr : public RfSensor
{
    DECLARE_SUBCLASS(Rwr,RfSensor)

public:
    Rwr();

    LCreal getDegreesPerRay() const         { return 360.0f / static_cast<LCreal>(NUM_RAYS); }
    int getNumberOfRays() const             { return NUM_RAYS; }
    LCreal getRay(const int i) const        { return (i >= 0 && i < NUM_RAYS) ? rays[1][i] : 0.0f; }

    int getRayIndex(const LCreal az) const;
    LCreal getRayAzimuth(const int idx) const;

    void clearRays(const int ibuff)
    {
       if (ibuff == 0 || ibuff == 1) {
          int i = 0;
          while (i < NUM_RAYS) rays[ibuff][i++] = 0.0;
       }
    }

    bool killedNotification(Player* const killedBy = 0) override;

protected:
   static const int MAX_EMISSIONS = 1000;
   static const int NUM_RAYS = 360;

   void receive(const LCreal dt) override;
   void process(const LCreal dt) override;

   bool shutdownNotification() override;

   void xferRays()
   {
      for (int i = 0; i < NUM_RAYS; i++) {
         rays[1][i] = rays[0][i];
      }
   }

private:
   void initData();

   Basic::safe_queue<Emission*> rptQueue;   // Report queue

   LCreal rays[2][NUM_RAYS];     // Back (sensor) buffer [0][*] and front (graphics) buffer [1][*]
};

} // End Simulation namespace
} // End oe namespace

#endif
