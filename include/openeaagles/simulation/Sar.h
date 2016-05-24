//------------------------------------------------------------------------------
// Class: Sar
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Sar_H__
#define __oe_simulation_Sar_H__

#include "openeaagles/simulation/Radar.h"
#include "openeaagles/base/osg/Vec3"

namespace oe {
   namespace base {
      class Pair;
      class PairStream;
   }
namespace simulation {

class Image;

//------------------------------------------------------------------------------
// Class: Sar
// Base class: base::Object -> ... -> RfSensor -> Radar -> Sar
//
// Description: Generic SAR
// Factory name: Sar
// Slots:
//    chipSize      <base::Number>      ! Chip size (pixels) (default: 0)
//
//------------------------------------------------------------------------------
class Sar : public Radar
{
    DECLARE_SUBCLASS(Sar,Radar)

public:
   Sar();

   virtual bool isImagingInProgress() const;
   virtual bool isSystemReady() const;

   // Returns a list of all images (prereferenced)
   virtual base::PairStream* getImages();

   // Returns the most recent image (prereferenced)
   virtual const Image* getImage() const;

   // Get functions
   bool haveStarePoint() const             { return haveLL; }
   double getStarePointLatitude() const    { return stareLatitude; }
   double getStarePointLongitude() const   { return stareLongitude; }
   double getStarePointElevation() const   { return stareElevation; }

   // Set the stare point
   virtual bool setStarePoint(
         const double lat,               // Latitude (deg)
         const double lon,               // Longitude (deg)
         const double elev               // Elevation (meters)
      );

   // Image resolution (meters/pixel)
   double getResolution() const            { return resolution; }

   unsigned int getWidth() const { return width; }    // Image width (pixels)
   unsigned int getHeight() const { return height; }  // Image height (pixels)

   // chip size (square -- pixels)
   unsigned int getChipSize() const        { return chipSize; }
   virtual bool setChipSize(const unsigned int pixels);

   // Slot functions
   virtual bool setSlotChipSize(const base::Number* const msg);

   // Request a new image
   virtual bool  requestImage(
         const unsigned int width,       // Image width (pixels)
         const unsigned int height,      // Image height (pixels)
         const double resolution         // Image Resolution (meters/pixel)
      );

    bool addImage(base::Pair* const newImage);
    int getNextId();

   // Cancel the current SAR imaging
   virtual void cancel();

   static void xyz2AzEl(const double x, const double y, const double z, double* const az, double* const el);
   static void xyz2AzEl(const osg::Vec3& vec, double* const az, double* const el);

protected:
    virtual bool setResolution(const double res);   // SAR resolution (meters)

    void process(const double dt) override;

    double timer;                   // SAR timer

private:
    base::PairStream*   imgList; // List of SAR images

    int             nextId;         // Next image ID
    double          stareLatitude;  // Degrees
    double          stareLongitude; // Degrees
    double          stareElevation; // Meters
    double          resolution;     // Image Resolution (meters/pixel)
    unsigned int    chipSize;       // Chip Size (pixels)
    bool            busy;           // Busy generating an image
    bool            haveLL;         // Have latitude and longitude of the stare point

    unsigned int   width;           // Image width (pixels)
    unsigned int   height;          // Image height (pixels)
};

}
}

#endif
