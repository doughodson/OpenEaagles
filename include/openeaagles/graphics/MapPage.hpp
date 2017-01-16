
#ifndef __oe_graphics_MapPage_H__
#define __oe_graphics_MapPage_H__

#include "openeaagles/graphics/MfdPage.hpp"

#include "openeaagles/base/units/unit_utils.hpp"

#include <cmath>

namespace oe {
namespace graphics {

// -------------------------------------------------------------------------------
// Class: MapPage
//
// Description: This is the generic MapPage, which can keep track of all types
// of map-specific calculations, such as range, heading, displacement, etc.  It
// performs calculations that will allow all MapPage derived classes the ability to
// easily draw and move map specific items, such as symbols and range rings.
//
//  Notes:
//    1) If a MapPage is a subcomponent of another MapPage then the subcomponent
//       MapPage will get the following attributes from the container MapPage.
//          a) reference point
//          b) heading (rotation) angle
//          c) range
//          d) centered and north-up flags
//          c) Center and de-centered outer radii
//          d) De-centered displacement
//
//    2) The reference point is usually the ownship player, but may be any point.
//
//    3) The 'heading' angle is the rotation about the reference point.
//
//    4) Conversions between lat/lon and NED use standard 'flat earth' using
//       the cosine of the reference points latitude.
//
//    5) Earth coordinates are standard NED in nautical miles
//
//    6) "Aircraft" coordinates, which actually could be any type player,
//       is X out the nose, Y right and Z down.
//
//    7) For "screen" coordinates, X is right and Y is up, and are in screen
//       'units', which can be inches, centimeters, etc, as defined by the
//       display's ortho.
//
//       The screen coordinates are still relative to the reference point and
//       do not include the de-center displacement.
//
//    8) You can bring in heading and reference lat lon as either degrees or
//       radians, using the appropriate (setHeadingDeg, setHeadingRad) functions.
//       HOWEVER, when using an event call, all of it must be sent as degrees!
//
// Factory name: MapPage
// Slots:
//
//   outerRadius           <Number> ! Radius (centered) of our outer circle (units) (default: 1.0)
//   outerRadiusDecentered <Number> ! Radius (de-centered) of our outer arc (units)
//
//   range        <Number>    ! our range, in nautical miles, of our map (default: 1.0)
//   displacement <Number>    ! how far to translate when we are decentered (default: 0.0)
//   centered     <Number>    ! are we centered, or de-centered (default: centered)
//
//   refLat       <Number>    ! reference latitude (degs) (default: 0)
//   refLon       <Number>    ! reference longitude (degs) (default: 0)
//
//   refHdg       <Number>    ! reference heading (degs) (default: 0)
//   northUp      <Number>    ! north up mode (default: true)
//
//
// Inputs for the send commands are as follows:
//      UPDATE_VALUE  = range (nautical miles)
//      UPDATE_VALUE2 = heading (degrees)
//      UPDATE_VALUE3 = reference lat (degrees)
//      UPDATE_VALUE4 = reference lon (degrees)
//      UPDATE_VALUE5 = centered
//      UPDATE_VALUE6 = outer radius (units)
//      UPDATE_VALUE7 = outer radius decentered (units)
//      UPDATE_VALUE8 = displacement
//
// -------------------------------------------------------------------------------
class MapPage : public MfdPage
{
   DECLARE_SUBCLASS(MapPage, MfdPage)

public:
   MapPage();

   // get functions
   double getReferenceLatDeg() const;  // reference latitude (degs)
   double getReferenceLonDeg() const;  // reference longitude (degs)
   double getReferenceLatRad() const;  // reference lat (rads)
   double getReferenceLonRad() const;  // reference lon (rads)
   double getCosRefLat() const;        // reference latitude cosine
   double getHeadingDeg() const;       // true heading (degrees)
   double getHeadingRad() const;       // true heading (radians)
   double getOuterRadius() const;      // outer radius (units, centered)
   double getCurrentRadius() const;    // return our current radius (dependent on centered/decentered)
   double getOuterRadiusDC() const;    // outer radius (units, decentered)
   double getRange() const;            // range (NM)
   double getScale() const;            // scale factor: screen units per NM
   double getDisplacement() const;     // de-centered displacement (units)
   bool   getCentered() const;         // centered flag
   bool   getNorthUp() const;          // north up flag

   // set functions
   virtual bool setReferenceLatDeg(const double newLat); // reference latitude (degs)
   virtual bool setReferenceLonDeg(const double newLon); // reference longitude (degs)
   virtual bool setReferenceLatRad(const double newLat); // reference latitude (rads)
   virtual bool setReferenceLonRad(const double newLon); // reference longitude (rads)
   virtual bool setHeadingDeg(const double newHeading);  // true heading (degrees)
   virtual bool setHeadingRad(const double newHeading);  // true heading (radians)
   virtual bool setOuterRadius(const double newRadius);  // the outer radius (centered mode) (units)
   virtual bool setOuterRadiusDC(const double newRadiusDC); // the outer radius (de-centered mode) (units)
   virtual bool setRange(const double newRange);         // the range to the outer radius (NM)
   virtual bool setDisplacement(const double newD);      // the de-centered displacement (units)
   virtual bool setCentered(const bool newC);            // the 'centered' flag
   virtual bool toggleCentered();
   virtual bool setNorthUp(const bool x);

   // converts lat/lon to screen coordinates
   virtual bool latLon2Screen(const double lat, const double lon, double* const screenX, double* const screenY) const;

   // converts screen to lat/lon coordinates
   virtual bool screen2LatLon(const double screenX, const double screenY, double* const lat, double* const lon) const;

   // converts aircraft (ie, player) coordinates (NM) to screen coordinates (units)
   // (screen coordinates do not include the map displacement)
   virtual bool aircraft2Screen(const double acX, const double acY, double* const screenX, double* const screenY) const;

   // earth coordinates (NED: NM) to aircraft (ie, player) coordinates (NM)
   virtual bool earth2Aircraft(const double earthX, const double earthY, double* const acX, double* const acY) const;

   // lat/lon (deg) to earth (NED: NM)
   virtual bool latLon2Earth(const double lat, const double lon, double* const earthX, double* const earthY) const;

   // move our reference lat/lon of our map.
   virtual void moveMap(const int startX, const int startY, const int endX, const int endY);

   // convert viewport pixel position to lat / lon
   virtual void pixelsToLatLon(const int x, const int y, double &lat, double &lon);

   virtual void updateData(const double dt = 0.0) override;
   virtual bool event(const int event, base::Object* const obj = nullptr) override;

protected:
   // slot functions
   virtual bool setSlotOuterRadius(const base::Number* const newRadius);
   virtual bool setSlotOuterRadiusDC(const base::Number* const newRadiusDC);
   virtual bool setSlotRange(const base::Number* const newR);
   virtual bool setSlotDisplacement(const base::Number* const newD);
   virtual bool setSlotCentered(const base::Number* const newC);
   virtual bool setSlotNorthUp(const base::Number* const x);

   // event functions
   virtual bool onUpdateRange(const base::Number* const newR);
   virtual bool onUpdateHeading(const base::Number* const newH);
   virtual bool onUpdateReferenceLat(const base::Number* const newOL);
   virtual bool onUpdateReferenceLon(const base::Number* const newOL);
   virtual bool onUpdateCentered(const base::Number* const newC);
   virtual bool onUpdateOuterRadius(const base::Number* const newR);
   virtual bool onUpdateOuterRadiusDC(const base::Number* const newRDC);
   virtual bool onUpdateDisplacement(const base::Number* const newD);

   // ref lat/lon heading
   virtual bool setSlotRefLat(const base::Number* const x);
   virtual bool setSlotRefLon(const base::Number* const x);
   virtual bool setSlotRefHdg(const base::Number* const x);

private:
   void initData();

   double referenceLat;        // our latitude from center reference point (degs)
   double referenceLon;        // our longitude from center reference point (degs)
   double cosineLatReference;  // cosine of our reference latitude
   double heading;             // heading (rad)
   double headingCos;          // cosine of heading
   double headingSin;          // sine of heading

   double nm2Screen;           // nautical miles to screen units
   double range;               // range scale of the system (NM)
   bool northUp;               // flag to determine north up

   // (we keep these separate because we need both of them)
   double outerRadius;         // radius of the outer circle's ring (screen units)
   double outerRadiusDC;       // radius if we are de-centered (screen units)

   double displacement;        // how far to translate up or down when centered/decentered (display units)
   bool   isCentered;          // flag for centering our map page
};

inline double MapPage::getReferenceLatDeg() const  { return referenceLat; }
inline double MapPage::getReferenceLonDeg() const  { return referenceLon; }
inline double MapPage::getReferenceLatRad() const  { return referenceLat * base::angle::D2RCC; }
inline double MapPage::getReferenceLonRad() const  { return referenceLon * base::angle::D2RCC; }
inline double MapPage::getCosRefLat() const        { return static_cast<double>(cosineLatReference); }
inline double MapPage::getHeadingDeg() const       { return static_cast<double>(heading); }
inline double MapPage::getHeadingRad() const       { return static_cast<double>(heading * base::angle::D2RCC); }
inline double MapPage::getOuterRadius() const      { return static_cast<double>(outerRadius); }
inline double MapPage::getCurrentRadius() const    { return static_cast<double>(isCentered ? outerRadius : outerRadiusDC);  }
inline double MapPage::getOuterRadiusDC() const    { return static_cast<double>(outerRadiusDC); }
inline double MapPage::getRange() const            { return static_cast<double>(range); }
inline double MapPage::getScale() const            { return static_cast<double>(nm2Screen); }
inline double MapPage::getDisplacement() const     { return static_cast<double>(displacement); }
inline bool   MapPage::getCentered() const         { return isCentered; }
inline bool   MapPage::getNorthUp() const          { return northUp; }

inline bool MapPage::setReferenceLatDeg(const double newLat)
{
   referenceLat = newLat;
   cosineLatReference = static_cast<double>(std::cos((base::angle::D2RCC*referenceLat)));
   return true;
}

inline bool MapPage::setReferenceLonDeg(const double newLon)
{
   referenceLon = newLon;
   return true;
}

inline bool MapPage::setReferenceLatRad(const double newLat)
{
   cosineLatReference = static_cast<double>(std::cos(newLat));
   referenceLat = newLat * base::angle::R2DCC;
   return true;
}

inline bool MapPage::setReferenceLonRad(const double newLon)
{
   referenceLon = newLon * base::angle::R2DCC;
   return true;
}

inline bool MapPage::setHeadingDeg(const double newHeading)
{
   double hdgRad = static_cast<double>(base::angle::D2RCC * newHeading);
   heading = newHeading;
   headingSin = static_cast<double>(std::sin(hdgRad));
   headingCos = static_cast<double>(std::cos(hdgRad));
   return true;
}

inline bool MapPage::setHeadingRad(const double newHeading)
{
   heading = newHeading * static_cast<double>(base::angle::R2DCC);
   double temp = newHeading;
   if (northUp) temp = 0;
   headingSin = static_cast<double>(std::sin(temp));
   headingCos = static_cast<double>(std::cos(temp));
   return true;
}

inline bool MapPage::setOuterRadius(const double newRadius)
{
   outerRadius = newRadius;
   return true;
}

inline bool MapPage::setOuterRadiusDC(const double newRadiusDC)
{
   outerRadiusDC = newRadiusDC;
   return true;
}

inline bool MapPage::setRange(const double newRange)
{
   range = newRange;
   return true;
}

inline bool MapPage::setDisplacement(const double newD)
{
   displacement = newD;
   return true;
}

inline bool MapPage::setCentered(const bool newC)
{
   isCentered = newC;
   return true;
}

inline bool MapPage::setNorthUp(const bool flg)
{
   northUp = flg;
   return true;
}

inline bool MapPage::toggleCentered()
{
   isCentered = !isCentered;
   return true;
}

}
}

#endif
