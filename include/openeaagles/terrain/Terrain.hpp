
#ifndef __oe_terrain_Terrain_H__
#define __oe_terrain_Terrain_H__

#include "openeaagles/simulation/environment/ITerrain.hpp"

#include "openeaagles/base/osg/Vec2d"
#include "openeaagles/base/osg/Vec3d"

namespace oe {
namespace base { class Hsva; class String; }
namespace terrain {

//------------------------------------------------------------------------------
// Class: Terrain
//
// Description: Abstract class for managing the terrain elevation databases.
//              Database specific, derived classes will be used to handle the
//              various database formats (e.g., DTED, DED, SRTM).
//
// Slots:
//    file  <String>   ! Data file name (default: 0)
//    path  <String>   ! Data path name (default: 0)
//
// Notes:
//    1) the first point [0] of all arrays is at the reference point
//    2) the final point [n-1] is at the maximum range
//    3) The size of all arrays, n, must contain at least 2 points (ref point & max range)
//------------------------------------------------------------------------------
class Terrain : public simulation::ITerrain
{
    DECLARE_SUBCLASS(Terrain, simulation::ITerrain)

public:
   Terrain();

   const char* getFilename() const;                            // Returns the name of the datafile
   virtual bool setFilename(const base::String* const msg);    // Sets the name of the datafile

   const char* getPathname() const;                            // Returns the path to the datafiles
   virtual bool setPathname(const base::String* const msg);    // Sets the path to the datafiles

   double getMinElevation() const { return minElev; }    // Minimum elevation in this database (meters)
   double getMaxElevation() const { return maxElev; }    // Maximum elevation in this database (meters)
   double getLatitudeSW() const   { return swLat;   }    // Southwest corner latitude of this database (degs)
   double getLongitudeSW() const  { return swLon;   }    // Southwest corner longitude of this database (degs)
   double getLatitudeNE() const   { return neLat;   }    // Northeast corner latitude of this database (degs)
   double getLongitudeNE() const  { return neLon;   }    // Northeast corner longitude of this database (degs)

   // Has the data been loaded
   virtual bool isDataLoaded() const = 0;

   // Locates an array of (at least two) elevation points (and sets valid flags if found)
   // returns the number of points found within this DataFile
   virtual unsigned int getElevations(
         double* const elevations,     // The elevation array (meters)
         bool* const validFlags,       // Valid elevation flag array (true if elevation was found)
         const unsigned int n,         // Size of elevation and valdFlags arrays
         const double lat,             // Starting latitude (degs)
         const double lon,             // Starting longitude (degs)
         const double direction,       // True direction (heading) angle of the data (degs)
         const double maxRng,          // Range to last elevation point (meters)
         const bool   interp = false   // Interpolate between elevation posts (default: false)
      ) const = 0;

   // Locates an elevation value (meters) for a given reference point and returns
   // it in 'elev'.  Function returns true if successful, otherwise 'elev' is unchanged.
   virtual bool getElevation(
         double* const elev,           // The elevation value (meters)
         const double lat,             // Reference latitude (degs)
         const double lon,             // Reference longitude (degs)
         const bool interp = false     // Interpolate between elevation posts (default: false)
      ) const override = 0;

   // Returns true if a target point is occulted by the terrain as seen from the ref point
   virtual bool targetOcculting(
         const double refLat,          // Ref latitude (degs)
         const double refLon,          // Ref longitude (degs)
         const double refAlt,          // Ref altitude (meters)
         const double tgtLat,          // Target latitude (degs)
         const double tgtLon,          // Target longitude (degs)
         const double tgtAlt           // Target altitude (meters)
      ) const override;

   // Returns true if any terrain in the 'truBrg' direction for 'dist' meters
   // occults (or masks) a target with a look angle of atan(tanLookAng)
   virtual bool targetOcculting2(
      const double refLat,             // Ref latitude (degs)
      const double refLon,             // Ref longitude (degs)
      const double refAlt,             // Ref altitude (meters)
      const double truBrg,             // True direction angle from north to look (degs)
      const double dist,               // Distance to check (meters)
      const double tanLookAng          // Tangent of the look angle
   ) const override;

   // Returns true if the target at the altitude 'tgtAlt' and range 'range' is
   // occulted by the elevation points as seen from the reference altitude, 'refAlt'.
   static bool occultCheck(
         const double* const elevations, // The elevation array (meters)
         const bool* const validFlags,   // (Optional) Valid elevation flag array (true if elevation was found)
         const unsigned int n,           // Size of elevation and valdFlags arrays
         const double range,             // Range (meters)
         const double refAlt,            // Ref altitude (meters)
         const double tgtAlt             // Target altitude (meters)
      );

   // Returns true if any of the tangents of the angles (from level) to each
   // elevation point, as seen from the ref altitude, refAlt, is greater than
   // the tangent of the 'look' angle, 'tanLookAng'.
   static bool occultCheck2(
         const double* const elevations, // The elevation array (meters)
         const bool* const validFlags,   // Valid elevation flag array (true if elevation was found)
         const unsigned int n,           // Size of the arrays
         const double range,             // Range (meters)
         const double refAlt,            // Ref altitude (meters)
         const double tanLookAng         // Tangent of the look angle
      );

   // Vertical Beam Width and Shadow Check --
   // Sets an array of mask flags; the flags are set true if the point
   // is masked (in shadow or out of beam) as seen from the reference
   // altitude over the first point.  Returns true if successful
   static bool vbwShadowChecker(
         bool* const maskFlags,          // The array of mask flags
         const double* const elevations, // The elevation array (meters)
         const bool* const validFlags,   // (Optional) Valid elevation flag array (true if elevation was found)
         const unsigned int n,           // Size of elevation and valdFlags arrays
         const double range,             // Range (meters)
         const double refAlt,            // Ref altitude (meters)
         const double beamAngle = 0,     // (optional) Center beam elevation angle (degs)
         const double beamWidth = 180    // (optional) Total beam width angle (degs)
      );

   // aac() -- Compute Aspect Angle Cosines; computes the cosine of the angle
   // inwhich the beam hits the terrain.
   static bool aac(
         double* const aacData,        // The array for the aspect angle cosines
         const double* const elevData, // The elevation array (meters)
         const bool* const maskFlags,  // (Optional) The array of mask flags
         const unsigned int n,         // Size of the arrays
         const double range,           // Range (meters)
         const double refAlt           // Ref altitude (meters)
      );

   static bool cLight(
      double* const ldata,          // The array for the lighting factors
      const double* const elevData, // The elevation array (meters)
      const bool* const maskFlags,  // (Optional) The array of mask flags
      const unsigned int n,         // Size of the arrays
      const double range,           // Range (meters)
      const osg::Vec2d& lv          // Lighting vector
   );

   // Converts an elevation to a color (or gray scale)
   //  gray scale (colorScale flag = false):
   //       black @ minz;
   //       white @ maxz;
   //
   //  color scale (colorScale flag = true):
   //       blue     @ <= minz
   //       cyan     @ 1/6 of maxz
   //       green    @ 2/6 of maxz
   //       yellow   @ 3/6 of maxz
   //       red      @ 4/6 of maxz
   //       megenta  @ 5/6 of maxz
   //       white    @ >= maxz
   //
   static bool getElevationColor(
      const double elevation,           // Elevation
      const double minz,                // Min elevation (units: same as elevation)
      const double maxz,                // Max elevtion  (units: same as elevation)
      const base::Hsva** colorTable,    // Color table
      const unsigned int numColors,     // Number of colors
      osg::Vec3d& rgb);                 // Color

   virtual void reset() override;

protected:
   virtual void clearData();                       // Clear the data arrays

   virtual bool setMinElevation(const double v);   // Minimum elevation in this database (meters)
   virtual bool setMaxElevation(const double v);   // Maximum elevation in this database (meters)
   virtual bool setLatitudeSW(const double v);     // Southwest corner latitude of this database (degs: +/-90)
   virtual bool setLongitudeSW(const double v);    // Southwest corner longitude of this database (degs: +/-180)
   virtual bool setLatitudeNE(const double v);     // Northeast corner latitude of this database (degs: +/-90)
   virtual bool setLongitudeNE(const double v);    // Northeast corner longitude of this database (degs: +/-180)

private:
   virtual bool loadData() =0;      // Load the data file

   const base::String* path;        // Data path name
   const base::String* file;        // Data file name
   double neLat, neLon;             // Northeast lat/lon (degs)
   double swLat, swLon;             // Southwest lat/lon (degs)
   double minElev;                  // Minimum elevation (m)
   double maxElev;                  // Maximum elevation (m)
};

}
}

#endif
