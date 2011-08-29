//------------------------------------------------------------------------------
// Class:	Rwr
//------------------------------------------------------------------------------
#ifndef __RWR_H_88B6FD96_1ABB_414c_B188_A65E6F9F0999__
#define __RWR_H_88B6FD96_1ABB_414c_B188_A65E6F9F0999__

#include "openeaagles/simulation/RfSensor.h"

namespace Eaagles {
namespace Simulation {
//------------------------------------------------------------------------------
// Class: Rwr
// Base class: Basic::Object -> ... -> RfSystem -> RfSensor -> Rwr
//
// Description: General Radar Warning Receiver (RWR) Model
// Form name: Rwr
//------------------------------------------------------------------------------
class Rwr : public RfSensor  
{
    DECLARE_SUBCLASS(Rwr,RfSensor)

public:
    Rwr();

    LCreal getDegreesPerRay() const         { return 360.0f/LCreal(NUM_RAYS); }
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

    // System Interface -- Event handler(s)
    virtual bool killedNotification(Player* const killedBy = 0);

protected:
   static const int MAX_EMISSIONS = 1000;
   static const int NUM_RAYS = 360;

   // System Interface -- phase callbacks
   virtual void receive(const LCreal dt);
   virtual void process(const LCreal dt);

   void xferRays()
   {
      for (int i = 0; i < NUM_RAYS; i++) {
         rays[1][i] = rays[0][i];
      }
   }

private:
   void initData();

   QQueue<Emission*> rptQueue;   // Report queue 

   LCreal rays[2][NUM_RAYS];     // Back (sensor) buffer [0][*] and front (graphics) buffer [1][*]
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // __RWR_H_88B6FD96_1ABB_414c_B188_A65E6F9F0999__
