
#ifndef __oe_models_MergingIrSensor_H__
#define __oe_models_MergingIrSensor_H__

#include "openeaagles/models/systems/IrSensor.hpp"

namespace oe {

namespace base { class Integer; class Number; class String; }

namespace models {

class IrSeeker;
class IrQueryMsg;
class Player;
class TrackManager;

//------------------------------------------------------------------------------
// Class: MergingIrSensor
//
// Description: I/R Sensor Model that models limited IrSensor that can only distinguish targets
//              if they are not within specified az & el bin.
//              Merges targets that are within bins in receive frame.
//------------------------------------------------------------------------------
class MergingIrSensor : public IrSensor
{
    DECLARE_SUBCLASS(MergingIrSensor, IrSensor)

public:
   MergingIrSensor();

   virtual void receive(const double dt) override;

   virtual double getAzimuthBin() const {return azimuthBin; };         // Returns min azimuth distance
   virtual bool setAzimuthBin(const double azimuthBin);                // Sets min Azimuth Distance

   virtual double getElevationBin() const {return elevationBin; };     // Returns min azimuth distance
   virtual bool setElevationBin(const double elevationBin);            // Sets min Azimuth Distance

   virtual void reset() override;

protected:
   virtual void mergeIrReturns();

   // Slot functions
   virtual bool setSlotAzimuthBin(const base::Number* const msg);     // Sets the Azimuth Bin
   virtual bool setSlotElevationBin(const base::Number* const msg);   // Sets the Elevation Bin

private:
   double azimuthBin;         // minimum azimuth we can distinguish -- two signals whose
                              // azimuth differs by less than this will be merged

   double elevationBin;       // minimum azimuth we can distinguish -- two signals whose
                              // azimuth differs by less than this will be merged
};

}
}

#endif

