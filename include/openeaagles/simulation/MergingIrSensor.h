//------------------------------------------------------------------------------
// Class: MergingIrSensor
//
// Description: I/R Sensor Model that models limited IrSensor that can only distinguish targets
//              if they are not within specified az & el bin. 
//              Merges targets that are within bins in receive frame.
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_MergingIrSensor_H__
#define __Eaagles_Simulation_MergingIrSensor_H__

#include "openeaagles/simulation/IrSensor.h"

namespace Eaagles {

namespace Basic { class Integer; class Number; class String; }

namespace Simulation {

class IrSeeker;
class IrQueryMsg;
class Player;
class TrackManager;

class MergingIrSensor : public IrSensor
{
    DECLARE_SUBCLASS(MergingIrSensor,IrSensor)

public:
   MergingIrSensor();

   // System class -- phase callbacks
   virtual void receive(const LCreal dt);

   virtual LCreal getAzimuthBin() const {return azimuthBin; };         // Returns min azimuth distance
   virtual bool setAzimuthBin(const LCreal azimuthBin);                // Sets min Azimuth Distance

   virtual LCreal getElevationBin() const {return elevationBin; };     // Returns min azimuth distance
   virtual bool setElevationBin(const LCreal elevationBin);            // Sets min Azimuth Distance

   virtual void reset();
protected:
   virtual void mergeIrReturns();

   // Slot functions
   virtual bool setSlotAzimuthBin(const Basic::Number* const msg);     // Sets the Azimuth Bin 
   virtual bool setSlotElevationBin(const Basic::Number* const msg);   // Sets the Elevation Bin 

private:
   LCreal azimuthBin;         // minimum azimuth we can distinguish -- two signals whose
                              // azimuth differs by less than this will be merged

   LCreal elevationBin;       // minimum azimuth we can distinguish -- two signals whose
                              // azimuth differs by less than this will be merged
};

}
}

#endif

