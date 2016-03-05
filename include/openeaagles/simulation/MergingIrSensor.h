//------------------------------------------------------------------------------
// Class: MergingIrSensor
//
// Description: I/R Sensor Model that models limited IrSensor that can only distinguish targets
//              if they are not within specified az & el bin.
//              Merges targets that are within bins in receive frame.
//------------------------------------------------------------------------------
#ifndef __oe_simulation_MergingIrSensor_H__
#define __oe_simulation_MergingIrSensor_H__

#include "openeaagles/simulation/IrSensor.h"

namespace oe {

namespace base { class Integer; class Number; class String; }

namespace simulation {

class IrSeeker;
class IrQueryMsg;
class Player;
class TrackManager;

class MergingIrSensor : public IrSensor
{
    DECLARE_SUBCLASS(MergingIrSensor,IrSensor)

public:
   MergingIrSensor();

   void receive(const LCreal dt) override;

   virtual LCreal getAzimuthBin() const {return azimuthBin; };         // Returns min azimuth distance
   virtual bool setAzimuthBin(const LCreal azimuthBin);                // Sets min Azimuth Distance

   virtual LCreal getElevationBin() const {return elevationBin; };     // Returns min azimuth distance
   virtual bool setElevationBin(const LCreal elevationBin);            // Sets min Azimuth Distance

   void reset() override;

protected:
   virtual void mergeIrReturns();

   // Slot functions
   virtual bool setSlotAzimuthBin(const base::Number* const msg);     // Sets the Azimuth Bin
   virtual bool setSlotElevationBin(const base::Number* const msg);   // Sets the Elevation Bin

private:
   LCreal azimuthBin;         // minimum azimuth we can distinguish -- two signals whose
                              // azimuth differs by less than this will be merged

   LCreal elevationBin;       // minimum azimuth we can distinguish -- two signals whose
                              // azimuth differs by less than this will be merged
};

}
}

#endif

