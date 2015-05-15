//==============================================================================
// class Terrain --
//==============================================================================

#include "openeaagles/basic/Terrain.h"

#include "openeaagles/basic/Color.h"
#include "openeaagles/basic/Hsva.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Rgba.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_ABSTRACT_SUBCLASS(Terrain,"TerrainDatabase")

// slot table
BEGIN_SLOTTABLE(Terrain)
   "file",        // 1) Data file name
   "path",        // 2) Data path name
END_SLOTTABLE(Terrain)

// slot map
BEGIN_SLOT_MAP(Terrain)
   ON_SLOT(1, setFilename, String)
   ON_SLOT(2, setPathname, String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Terrain::Terrain()
{
   STANDARD_CONSTRUCTOR()

   path = nullptr;
   file = nullptr;

   neLat = 0;
   neLon = 0;
   swLat = 0;
   swLon = 0;
   minElev = 0;
   maxElev = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Terrain::copyData(const Terrain& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      path = nullptr;
      file = nullptr;
   }

   clearData();

   if (org.path != nullptr) {
      setPathname( static_cast<String*>(org.path->clone()) );
   }
   else {
      setPathname( nullptr );
   }

   if (org.file != nullptr) {
      setFilename( static_cast<String*>(org.file->clone()) );
   }
   else {
      setFilename( nullptr );
   }

   neLat = org.neLat;
   neLon = org.neLon;
   swLat = org.swLat;
   swLon = org.swLon;

   minElev = org.minElev;
   maxElev = org.maxElev;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Terrain::deleteData()
{
   clearData();

   setPathname(nullptr);
   setFilename(nullptr);
}

//------------------------------------------------------------------------------
// reset() -- Reset the simulation & players
//------------------------------------------------------------------------------
void Terrain::reset()
{
   if ( !isDataLoaded() ) {
      loadData();
   }

   BaseClass::reset();
}

//------------------------------------------------------------------------------
// clear our data
//------------------------------------------------------------------------------
void Terrain::clearData()
{
}

//------------------------------------------------------------------------------
// Access (get) functions
//------------------------------------------------------------------------------

// Returns the name of the datafile
const char* Terrain::getFilename() const
{
   const char* p = nullptr;
   if (file != nullptr) {
      p = *file;
   }
   return p;
}

// Returns the path to the datafiles
const char* Terrain::getPathname() const
{
   const char* p = nullptr;
   if (path != nullptr) {
      // Our path
      p = *path;
   }
   else {
      // See if we have a contain "Terrain" object that has a path set
      const Terrain* q = static_cast<const Terrain*>(findContainerByType(typeid(Terrain)));
      if (q != nullptr) p = q->getPathname();
   }
   return p;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the name of the datafile
bool Terrain::setFilename(const String* const msg)
{
   if (file != nullptr) file->unref();
   file = msg;
   if (file != nullptr) file->ref();
   return true;
}

// Sets the path to the datafiles
bool Terrain::setPathname(const String* const msg)
{
   if (path != nullptr) path->unref();
   path = msg;
   if (path != nullptr) path->ref();
   return true;
}

// Minimum elevation in this database (meters)
bool Terrain::setMinElevation(const LCreal v)
{
   minElev = v;
   return true;
}

// Maximum elevation in this database (meters)
bool Terrain::setMaxElevation(const LCreal v)
{
   maxElev = v;
   return true;
}

// Southwest corner latitude of this database (degs)
bool Terrain::setLatitudeSW(const double v)
{
   bool ok = false;
   if (v >= -90.0 && v <= 90.0) {
      swLat = v;
      ok = true;
   }
   return ok;
}

// Southwest corner longitude of this database (degs)
bool Terrain::setLongitudeSW(const double v)
{
   bool ok = false;
   if (v >= -180.0 && v <= 180.0) {
      swLon = v;
      ok = true;
   }
   return ok;
}

// Northeast corner latitude of this database (degs)
bool Terrain::setLatitudeNE(const double v)
{
   bool ok = false;
   if (v >= -90.0 && v <= 90.0) {
      neLat = v;
      ok = true;
   }
   return ok;
}

// Northeast corner longitude of this database (degs)
bool Terrain::setLongitudeNE(const double v)
{
   bool ok = false;
   if (v >= -180.0 && v <= 180.0) {
      neLon = v;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Target occulting: returns true if a target point [ tgtLat tgtLon tgtAlt ] is
// occulted by the terrain as seen from the ref point [ refLat refLon refAlt ].
//------------------------------------------------------------------------------
bool Terrain::targetOcculting(
      const double refLat,    // Ref latitude (degs)
      const double refLon,    // Ref longitude (degs)
      const LCreal refAlt,    // Ref altitude (meters)
      const double tgtLat,    // Target latitude (degs)
      const double tgtLon,    // Target longitude (degs)
      const LCreal tgtAlt     // Target altitude (meters)
   ) const
{
   // 1200 points gives us 100 meter data up to a distance
   // of one degree at the equator
   static const unsigned int MAX_POINTS = 1200;

   bool occulted = false;

   // Compute bearing and distance to target (flat earth)
   double brgDeg = 0.0;
   double distNM = 0.0;
   Nav::fll2bd(refLat, refLon, tgtLat, tgtLon, &brgDeg, &distNM);
   double dist = (distNM * Distance::NM2M);

   // Number of points (default: 100M data)
   unsigned int numPts = static_cast<unsigned int>((dist / 100.0f) + 0.5f);
   if (numPts > MAX_POINTS) numPts = MAX_POINTS;

   // Get the elevations and check for target occulting
   if (numPts > 1) {

      // Arrays for the elevations
      LCreal elevations[MAX_POINTS];

      // Valid flags
      bool validFlags[MAX_POINTS];
      for (unsigned int i = 0; i < numPts; i++) { validFlags[i] = false; }

      // Get the elevations
      unsigned int num = getElevations(elevations, validFlags, numPts, refLat, refLon,
                                       static_cast<LCreal>(brgDeg), static_cast<LCreal>(dist), false);

      // And check occulting
      if (num > 0) {
         occulted = occultCheck(elevations, validFlags, numPts, static_cast<LCreal>(dist),
                                refAlt, tgtAlt);
      }
   }

   return occulted;
}

//------------------------------------------------------------------------------
// Target occulting #2: returns true if any terrain in the 'truBrg' direction
// for 'dist' meters occults (or masks) a target with a look angle of atan(tanLookAng)
//------------------------------------------------------------------------------
bool Terrain::targetOcculting2(
      const double refLat,    // Ref latitude (degs)
      const double refLon,    // Ref longitude (degs)
      const double refAlt,    // Ref altitude (meters)
      const double truBrg,    // True direction angle from north to look (degs)
      const double dist,      // Distance to check (meters)
      const double tanLookAng // Tangent of the look angle
   ) const
{
   // 1200 points gives us 100 meter data up to a distance
   // of one degree at the equator
   static const unsigned int MAX_POINTS = 1200;

   bool occulted = false;

   // Number of points (default: 100M data)
   unsigned int numPts = static_cast<unsigned int>((dist / 100.0f) + 0.5f);
   if (numPts > MAX_POINTS) numPts = MAX_POINTS;

   // Get the elevations and check for target occulting
   if (numPts > 1) {

      // Arrays for the elevations
      LCreal elevations[MAX_POINTS];

      // Valid flags
      bool validFlags[MAX_POINTS];
      for (unsigned int i = 0; i < numPts; i++) { validFlags[i] = false; }

      // Get the elevations
      unsigned int num = getElevations(elevations, validFlags, numPts, refLat, refLon,
                                       static_cast<LCreal>(truBrg), static_cast<LCreal>(dist), false);

      // And check occulting
      if (num > 0) {
         occulted = occultCheck2(elevations, validFlags, numPts, dist, refAlt, tanLookAng);
      }
   }

   return occulted;
}

//------------------------------------------------------------------------------
// Occulting check: returns true if a target at the altitude 'tgtAlt' and
// range 'range' is occulted by the elevation points as seen from the
// reference altitude 'refAlt'.
//------------------------------------------------------------------------------
bool Terrain::occultCheck(
      const LCreal* const elevations,  // The elevation array (meters)
      const bool* const validFlags,    // Valid elevation flag array (true if elevation was found)
      const unsigned int n,            // Size of the arrays
      const LCreal range,              // Range (meters)
      const LCreal refAlt,             // Ref altitude (meters)
      const LCreal tgtAlt)             // Target altitude (meters)
{
   bool occulted = false;

   // Early out checks
   if (  elevations == nullptr ||    // The elevation array wasn't provided, or
         n < 2 ||              // there are too few points, or
         range <= 0            // the range is less than or equal to zero
         ) return occulted;

   // Tangent of the angle to the target point --
   // If the angle to any terrain point is greater than this
   // angle then the target is occulted by the terrain point
   LCreal tgtTan = (tgtAlt - refAlt) / range;

   // Loop through all elevation points looking for an angle
   // that's greater than our ref angle
   LCreal deltaRng = (range / (n - 1));
   LCreal currentRange = 0;
   if (validFlags != nullptr) {
      // with valid flags
      for (unsigned int i = 1; i < (n-1) && !occulted; i++) {
         currentRange += deltaRng;
         if (validFlags[i]) {
            const LCreal tstTan = (elevations[i] - refAlt) / currentRange;
            if (tstTan >= tgtTan) {
               occulted = true;
            }
         }
      }
   }
   else {
      // without valid flags
      for (unsigned int i = 1; i < (n-1) && !occulted; i++) {
         currentRange += deltaRng;
         const LCreal tstTan = (elevations[i] - refAlt) / currentRange;
         if (tstTan >= tgtTan) {
            occulted = true;
         }
      }
   }

   return occulted;
}

//------------------------------------------------------------------------------
// Occulting check #2: returns true if any of the tangents of the angles (from
// level) to each elevation point, as seen from the ref altitude, refAlt, is
// greater than the tangent of the 'look' angle, 'tanLookAng'.
//------------------------------------------------------------------------------
bool Terrain::occultCheck2(
      const LCreal* const elevations, // The elevation array (meters)
      const bool* const validFlags, // Valid elevation flag array (true if elevation was found)
      const unsigned int n,         // Size of the arrays
      const double range,           // Range (meters)
      const double refAlt,          // Ref altitude (meters)
      const double tanLookAng)      // Tangent of the look angle
{
   bool occulted = false;

   // Early out checks
   if (  elevations == nullptr ||   // The elevation array wasn't provided, or
         n < 2 ||                   // there are too few points, or
         range <= 0                 // the range is less than or equal to zero
         ) return occulted;

   // Loop through all elevation points looking for an angle
   // that's greater than our ref angle
   double deltaRng = (range / (n - 1));
   double currentRange = 0;
   if (validFlags != nullptr) {
      // with valid flags
      for (unsigned int i = 1; i < (n-1) && !occulted; i++) {
         currentRange += deltaRng;
         if (validFlags[i]) {
            const double tstTan = (elevations[i] - refAlt) / currentRange;
            if (tstTan >= tanLookAng) {
               occulted = true;
            }
         }
      }
   }
   else {
      // without valid flags
      for (unsigned int i = 1; i < (n-1) && !occulted; i++) {
         currentRange += deltaRng;
         const double tstTan = (elevations[i] - refAlt) / currentRange;
         if (tstTan >= tanLookAng) {
            occulted = true;
         }
      }
   }

   return occulted;
}


//------------------------------------------------------------------------------
// Vertical Beam Width and Shadow Check --
// Sets an array of mask flags; the flags are set true if the point
// is masked (in shadow or out of beam) as seen from the reference
// altitude over the first point.  Returns true if successful
//------------------------------------------------------------------------------
bool Terrain::vbwShadowChecker(
      bool* const maskFlags,          // The array of mask flags
      const LCreal* const elevations, // The elevation array (meters)
      const bool* const validFlags,   // (Optional) Valid elevation flag array (true if elevation was found)
      const unsigned int n,           // Size of the arrays
      const LCreal range,             // Range (meters)
      const LCreal refAlt,            // Ref altitude (meters)
      const LCreal beamAngle,         // Center beam elevation angle (degs)
      const LCreal beamWidth)         // Total beam width angle (degs)
{
   // Early out checks
   if (  maskFlags == nullptr ||      // The mask flag array wasn't provided, or
         elevations == nullptr ||     // the elevation array wasn't provided, or
         n < 2 ||                     // there are too few points, or
         range <= 0                   // the range is less than or equal to zero
         ) return false;

   // Compute the upper and lower edges of the beam
   LCreal beamUpper = beamAngle + beamWidth/2.0f;
   if (beamUpper >  89.9999f) beamUpper =  89.9999f;
   LCreal beamLower = beamAngle - beamWidth/2.0f;
   if (beamLower < -89.9999f) beamLower = -89.9999f;

   // tangents of the upper and lower edges of the beam
   LCreal tanUpper = lcTan(beamUpper * static_cast<LCreal>(Angle::D2RCC));
   LCreal tanLower = lcTan(beamLower * static_cast<LCreal>(Angle::D2RCC));

   // Loop through all other elevation points -- keep track of the current max
   // tangent value and flag as terrain masked all points with tangent
   // values less than the current.

   LCreal deltaRng = (range / (n - 1));
   LCreal currentRange = 0;
   if (validFlags != nullptr) {
      // with valid flags
      LCreal tanLookAngle = 0;
      for (unsigned int i = 0; i < n; i++) {
         if (validFlags[i]) {
            // Valid data -- compute the tangent of the look angle to the point and
            // see if it's within the upper and lower tangent boundaries
            if (currentRange > 0) {
               tanLookAngle = (elevations[i] - refAlt) / currentRange;
            }
            else {
               tanLookAngle = (elevations[i] - refAlt) / 1.0f;
            }
            if (tanLookAngle >= tanLower && tanLookAngle <= tanUpper) {
               // In beam and not in shadow
               maskFlags[i] = false;
               tanLower = tanLookAngle;
            }
            else {
               // Out of beam or in shadow
               maskFlags[i] = true;
            }
         }
         else {
            // Set the mask flag for all invalid data
            maskFlags[i] = true;
         }
         currentRange += deltaRng;
      }
   }

   else {
      // without valid flags
      LCreal tanLookAngle = 0;
      for (unsigned int i = 0; i < n; i++) {
         // Compute the tangent of the look angle to the point and
         // see if it's within the upper and lower tangent boundaries
         if (currentRange > 0) {
            tanLookAngle = (elevations[i] - refAlt) / currentRange;
         }
         else {
            tanLookAngle = (elevations[i] - refAlt) / 1.0f;
         }
         if (tanLookAngle >= tanLower && tanLookAngle <= tanUpper) {
            // In beam and not in shadow
            maskFlags[i] = false;
            tanLower = tanLookAngle;
         }
         else {
            // Out of beam or in shadow
            maskFlags[i] = true;
         }
         currentRange += deltaRng;
      }
   }

   return true;
}

//------------------------------------------------------------------------------
// aac() -- Compute Aspect Angle Cosines; computes the cosine of the angle
// inwhich the beam hits the terrain.
//------------------------------------------------------------------------------
bool Terrain::aac(
      LCreal* const aacData,        // The array for the aspect angle cosines
      const LCreal* const elevData, // The elevation array (meters)
      const bool* const maskFlags,  // (Optional) The array of mask flags
      const unsigned int n,         // Size of the arrays
      const LCreal range,           // Range (meters)
      const LCreal refAlt           // Ref altitude (meters)
   )
{
   // Early out checks
   if (  aacData == nullptr ||      // The AAC data array wasn't provided, or
         elevData == nullptr ||     // the elevation array wasn't provided, or
         n < 3 ||                   // there are too few points, or
         range <= 0                 // the range is less than or equal to zero
         ) return false;


   // m1 will be used as a normalized vector along a line from the
   // ref point to the current terrain point.
   osg::Vec2 m1;

   // m2 will be used is a normalized vector along a line perpendicular
   // to the current terrain slope.
   osg::Vec2 m2;

   // ---
   // For all elevation points, except the end points, compute the dot product
   // of a vector in the direction of the beam with a vector perpendicular to
   // the terrain.
   // ---

   LCreal deltaRng = (range / (n - 1));
   LCreal deltaRng2 = deltaRng * 2.0f;
   LCreal currentRange = deltaRng;

   if (maskFlags != nullptr) {
      // with mask flags
      for (unsigned int i = 1; i < (n-1); i++) {
         if (!maskFlags[i]) {
            m1.set(currentRange, elevData[i] - refAlt);
            m1.normalize();
            m2.set((elevData[i+1] - elevData[i-1]),-deltaRng2);
            m2.normalize();
            LCreal v = m1 * m2;
            if (v < 0.0) v = 0;
            aacData[i] = v;
         }
         else {
            aacData[i] = 0.0f;
         }
         currentRange += deltaRng;
      }
   }

   else {
      // without mask flags
      for (unsigned int i = 1; i < (n-1); i++) {
         m1.set(currentRange, elevData[i] - refAlt);
         m1.normalize();
         m2.set((elevData[i+1] - elevData[i-1]),-deltaRng2);
         m2.normalize();
         LCreal v = m1 * m2;
         if (v < 0.0) v = 0;
         aacData[i] = v;
         currentRange += deltaRng;
      }
   }

   // ---
   // Set the endpoints to the next to endpoints values;
   // ---
   aacData[0]   = aacData[1];
   aacData[n-1] = aacData[n-2];

   return true;
}

//------------------------------------------------------------------------------
// cLight() -- Computes the columnated lighting effect for each point
//------------------------------------------------------------------------------
bool Terrain::cLight(
      LCreal* const ldata,          // The array for the lighting factors
      const LCreal* const elevData, // The elevation array (meters)
      const bool* const maskFlags,  // (Optional) The array of mask flags
      const unsigned int n,         // Size of the arrays
      const LCreal range,           // Range (meters)
      const osg::Vec2& lv           // Columnated lighting vector
   )
{
   // Early out checks
   if (  ldata == nullptr ||        // The lighting data array wasn't provided, or
         elevData == nullptr ||     // the elevation array wasn't provided, or
         n < 3 ||                   // there are too few points, or
         range <= 0                 // the range is less than or equal to zero
         ) return false;


   // m2 will be used is a normalized vector along a line perpendicular
   // to the current terrain slope.
   osg::Vec2 m2;

   // ---
   // For all elevation points, except the end points, compute the dot product
   // of a vector in the direction of the beam with a vector perpendicular to
   // the terrain.
   // ---

   LCreal deltaRng = (range / (n - 1));
   LCreal deltaRng2 = deltaRng * 2.0f;

   if (maskFlags != nullptr) {
      // with mask flags
      for (unsigned int i = 1; i < (n-1); i++) {
         if (!maskFlags[i]) {
            m2.set((elevData[i+1] - elevData[i-1]),-deltaRng2);
            m2.normalize();
            LCreal v = lv * m2;
            if (v < 0.0) v = 0;
            ldata[i] = v;
         }
         else {
            ldata[i] = 0.0f;
         }
      }
   }

   else {
      // without mask flags
      for (unsigned int i = 1; i < (n-1); i++) {
         m2.set((elevData[i+1] - elevData[i-1]),-deltaRng2);
         m2.normalize();
         LCreal v = lv * m2;
         if (v < 0.0) v = 0;
         ldata[i] = v;
      }
   }

   // ---
   // Set the endpoints to the next to endpoints values;
   // ---
   ldata[0]   = ldata[1];
   ldata[n-1] = ldata[n-2];

   return true;
}

//------------------------------------------------------------------------------
// Converts an elevation to a color (or gray scale)
//------------------------------------------------------------------------------
bool Terrain::getElevationColor(
      const LCreal elevation,          // Elevation
      const LCreal minz,               // Min elevation
      const LCreal maxz,               // Max elevation
      const Hsva** colorTable, // Color table
      const unsigned int numColors,    // Number of colors
      osg::Vec3& rgb)                  // Color
{

   // early out test
   if ( colorTable == nullptr || // no color table, or
        numColors < 1      // no colors, or
         ) return false;

   // HSV color
   Hsva colorHsv;

   if ( elevation < minz ) {
      // too low is the first color
      colorHsv = *colorTable[0];
   }

   else if ( elevation >= maxz ) {
      // too high is the last color
      colorHsv = *colorTable[numColors-1];
   }

   else if (numColors == 2) {
      // Only two, that's easy
      colorHsv.colorInterpolate(elevation, minz, maxz, *colorTable[0], *colorTable[1]);
   }
   else {

      // Full range: min to max
      LCreal elevRange = maxz - minz;

      // elevation steps between colors
      LCreal elevSteps = elevRange / (numColors-1);

      // delta elevation between min to ref elevations
      LCreal deltaElev = elevation - minz;

      // Lower color table index
      unsigned int idx = static_cast<unsigned int>(deltaElev/elevSteps);
      if (idx > (numColors - 2)) {
         idx = numColors - 2;
      }

      // Lower & upper elevation limits
      LCreal lowLimit = idx * elevSteps;
      LCreal highLimit = (idx+1) * elevSteps;

      // Interpolate between colors
      colorHsv.colorInterpolate(deltaElev, lowLimit, highLimit, *colorTable[idx], *colorTable[(idx+1)]);
   }

   // Return the RGB value of this new color
   const osg::Vec3* p = colorHsv.getRGB();
   if (p != nullptr) rgb = *p;

   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Object* Terrain::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() --
//------------------------------------------------------------------------------
std::ostream& Terrain::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
      indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
    }

    if (path != nullptr) {
      indent(sout,i+j);
      sout << "path:  " << path << std::endl;
    }

    if (file != nullptr) {
      indent(sout,i+j);
      sout << "file:  " << file << std::endl;
    }

    if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
    }

    return sout;
}

} // End Basic namespace
} // End Eaagles namespace
