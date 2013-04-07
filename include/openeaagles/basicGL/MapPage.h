//------------------------------------------------------------------------------
// Class: MapPage
//------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_MapPage_H__
#define __Eaagles_BasicGL_MapPage_H__

#include "openeaagles/basicGL/MfdPage.h"
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace BasicGL {

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
// Form name: MapPage
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
   LCreal getCosRefLat() const;        // reference latitude cosine
   LCreal getHeadingDeg() const;       // true heading (degrees)
   LCreal getHeadingRad() const;       // true heading (radians)
   LCreal getOuterRadius() const;      // outer radius (units, centered)
   LCreal getCurrentRadius() const;    // return our current radius (dependent on centered/decentered)
   LCreal getOuterRadiusDC() const;    // outer radius (units, decentered)
   LCreal getRange() const;            // range (NM)
   LCreal getScale() const;            // scale factor: screen units per NM
   LCreal getDisplacement() const;     // de-centered displacement (units)
   bool   getCentered() const;         // centered flag
   bool   getNorthUp() const;          // north up flag

   // set functions 
   virtual bool setReferenceLatDeg(const double newLat); // reference latitude (degs)
   virtual bool setReferenceLonDeg(const double newLon); // reference longitude (degs)
   virtual bool setReferenceLatRad(const double newLat); // reference latitude (rads)
   virtual bool setReferenceLonRad(const double newLon); // reference longitude (rads)
   virtual bool setHeadingDeg(const LCreal newHeading);  // true heading (degrees)
   virtual bool setHeadingRad(const LCreal newHeading);  // true heading (radians)
   virtual bool setOuterRadius(const LCreal newRadius);  // the outer radius (centered mode) (units)
   virtual bool setOuterRadiusDC(const LCreal newRadiusDC); // the outer radius (de-centered mode) (units)
   virtual bool setRange(const LCreal newRange);         // the range to the outer radius (NM)
   virtual bool setDisplacement(const LCreal newD);      // the de-centered displacement (units)
   virtual bool setCentered(const bool newC);            // the 'centered' flag
   virtual bool toggleCentered();
   virtual bool setNorthUp(const bool x);                  

   // converts lat/lon to screen coordinates
   virtual bool latLon2Screen(const double lat, const double lon, LCreal* const screenX, LCreal* const screenY) const;

   // converts screen to lat/lon coordinates
   virtual bool screen2LatLon(const LCreal screenX, const LCreal screenY, double* const lat, double* const lon) const;

   // converts aircraft (ie, player) coordinates (NM) to screen coordinates (units)
   // (screen coordinates do not include the map displacement)
   virtual bool aircraft2Screen(const LCreal acX, const LCreal acY, LCreal* const screenX, LCreal* const screenY) const;

   // earth coordinates (NED: NM) to aircraft (ie, player) coordinates (NM)
   virtual bool earth2Aircraft(const LCreal earthX, const LCreal earthY, LCreal* const acX, LCreal* const acY) const;

   // lat/lon (deg) to earth (NED: NM)
   virtual bool latLon2Earth(const double lat, const double lon, LCreal* const earthX, LCreal* const earthY) const;

   // move our reference lat/lon of our map.
   virtual void moveMap(const int startX, const int startY, const int endX, const int endY);

   // convert viewport pixel position to lat / lon
   virtual void pixelsToLatLon(const int x, const int y, double &lat, double &lon);

   // Basic::Component interface
   virtual void updateData(const LCreal dt = 0.0);  
   virtual bool event(const int event, Basic::Object* const obj = 0);

protected:
   // slot functions
   virtual bool setSlotOuterRadius(const Basic::Number* const newRadius);
   virtual bool setSlotOuterRadiusDC(const Basic::Number* const newRadiusDC);
   virtual bool setSlotRange(const Basic::Number* const newR);
   virtual bool setSlotDisplacement(const Basic::Number* const newD);
   virtual bool setSlotCentered(const Basic::Number* const newC);
   virtual bool setSlotNorthUp(const Basic::Number* const x);

   // event functions
   virtual bool onUpdateRange(const Basic::Number* const newR);
   virtual bool onUpdateHeading(const Basic::Number* const newH);       
   virtual bool onUpdateReferenceLat(const Basic::Number* const newOL);
   virtual bool onUpdateReferenceLon(const Basic::Number* const newOL);
   virtual bool onUpdateCentered(const Basic::Number* const newC);
   virtual bool onUpdateOuterRadius(const Basic::Number* const newR);
   virtual bool onUpdateOuterRadiusDC(const Basic::Number* const newRDC);
   virtual bool onUpdateDisplacement(const Basic::Number* const newD);

   // ref lat/lon heading
   virtual bool setSlotRefLat(const Basic::Number* const x);
   virtual bool setSlotRefLon(const Basic::Number* const x);
   virtual bool setSlotRefHdg(const Basic::Number* const x);

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

// -------------------------------------------------------------------------------
// Inline functions
// -------------------------------------------------------------------------------

inline double MapPage::getReferenceLatDeg() const  { return referenceLat; }
inline double MapPage::getReferenceLonDeg() const  { return referenceLon; }
inline double MapPage::getReferenceLatRad() const  { return referenceLat * Basic::Angle::D2RCC; }
inline double MapPage::getReferenceLonRad() const  { return referenceLon * Basic::Angle::D2RCC; }
inline LCreal MapPage::getCosRefLat() const        { return (LCreal) cosineLatReference; }
inline LCreal MapPage::getHeadingDeg() const       { return (LCreal) heading; }
inline LCreal MapPage::getHeadingRad() const       { return (LCreal)(heading * Basic::Angle::D2RCC); }
inline LCreal MapPage::getOuterRadius() const      { return (LCreal) outerRadius; }
inline LCreal MapPage::getCurrentRadius() const    { return (LCreal) ( isCentered ? outerRadius : outerRadiusDC);  }
inline LCreal MapPage::getOuterRadiusDC() const    { return (LCreal) outerRadiusDC; }
inline LCreal MapPage::getRange() const            { return (LCreal) range; }
inline LCreal MapPage::getScale() const            { return (LCreal) nm2Screen; }
inline LCreal MapPage::getDisplacement() const     { return (LCreal) displacement; }
inline bool   MapPage::getCentered() const         { return isCentered; }
inline bool   MapPage::getNorthUp() const          { return northUp; }

inline bool MapPage::setReferenceLatDeg(const double newLat)
{
   referenceLat = newLat;
   cosineLatReference = (LCreal)cos((Basic::Angle::D2RCC*referenceLat));
   return true;
}

inline bool MapPage::setReferenceLonDeg(const double newLon)
{
   referenceLon = newLon;
   return true;
}

inline bool MapPage::setReferenceLatRad(const double newLat)
{
   cosineLatReference = (LCreal)cos(newLat);
   referenceLat = newLat * Basic::Angle::R2DCC;
   return true;
}

inline bool MapPage::setReferenceLonRad(const double newLon)
{
   referenceLon = newLon * Basic::Angle::R2DCC;
   return true;
}

inline bool MapPage::setHeadingDeg(const LCreal newHeading)
{
   LCreal hdgRad = (LCreal) Basic::Angle::D2RCC * newHeading;
   heading = newHeading;  
   headingSin = (LCreal)sin(hdgRad);
   headingCos = (LCreal)cos(hdgRad);
   return true;
}

inline bool MapPage::setHeadingRad(const LCreal newHeading)
{
   heading = newHeading * (LCreal)Basic::Angle::R2DCC;
   LCreal temp = newHeading;
   if (northUp) temp = 0;
   headingSin = (LCreal)sin(temp);
   headingCos = (LCreal)cos(temp);
   return true;
}

inline bool MapPage::setOuterRadius(const LCreal newRadius)
{
   outerRadius = newRadius;
   return true;
}

inline bool MapPage::setOuterRadiusDC(const LCreal newRadiusDC)
{
   outerRadiusDC = newRadiusDC;
   return true;
}

inline bool MapPage::setRange(const LCreal newRange)
{
   range = newRange;
   return true;
}

inline bool MapPage::setDisplacement(const LCreal newD)
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

}  // End BasicGL namespace
}  // End Eaagles namespace

#endif
