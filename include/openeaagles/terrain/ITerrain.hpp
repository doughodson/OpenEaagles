
#ifndef __oe_terrain_ITerrain_H__
#define __oe_terrain_ITerrain_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace terrain {

class ITerrain : public base::Component
{
   DECLARE_SUBCLASS(ITerrain, base::Component)

public:
   ITerrain();

   //
   // interface required/used by Player class
   //

   // Locates an elevation value (meters) for a given reference point and returns
   // it in 'elev'.  Function returns true if successful, otherwise 'elev' is unchanged.
   virtual bool getElevation(
         double* const elev,           // The elevation value (meters)
         const double lat,             // Reference latitude (degs)
         const double lon,             // Reference longitude (degs)
         const bool interp = false     // Interpolate between elevation posts (default: false)
      ) const = 0;

   //
   // interface required/used by Tdb class
   //

   // Returns true if a target point is occulted by the terrain as seen from the ref point
   virtual bool targetOcculting(
         const double refLat,          // Ref latitude (degs)
         const double refLon,          // Ref longitude (degs)
         const double refAlt,          // Ref altitude (meters)
         const double tgtLat,          // Target latitude (degs)
         const double tgtLon,          // Target longitude (degs)
         const double tgtAlt           // Target altitude (meters)
      ) const = 0;

   // Returns true if any terrain in the 'truBrg' direction for 'dist' meters
   // occults (or masks) a target with a look angle of atan(tanLookAng)
   virtual bool targetOcculting2(
      const double refLat,             // Ref latitude (degs)
      const double refLon,             // Ref longitude (degs)
      const double refAlt,             // Ref altitude (meters)
      const double truBrg,             // True direction angle from north to look (degs)
      const double dist,               // Distance to check (meters)
      const double tanLookAng          // Tangent of the look angle
   ) const = 0;

};

}
}

#endif
