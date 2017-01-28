
#ifndef __oe_graphics_Clip3D_H__
#define __oe_graphics_Clip3D_H__

#include "openeaagles/base/Object.hpp"
#include "Polygon.hpp"
#include <limits>

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Clip3D -- 3D polygon clipper
//
// Based on the following and modifed to use Performer pfVec3 and C++
//
//  /*
//   * Generic Convex Polygon Scan Conversion and Clipping
//   * by Paul Heckbert
//   * from "Graphics Gems", Academic Press, 1990
//   */
//
//  /*
//   * poly_clip.c: homogeneous 3-D convex polygon clipper
//   *
//   * Paul Heckbert 1985, Dec 1989
//   */
//
// Factory name: Clip3D
//
//------------------------------------------------------------------------------
class Clip3D : public base::Object
{
    DECLARE_SUBCLASS(Clip3D, base::Object)

public:
   Clip3D();

   //------------------------------------------------------------------------------
   // Sets the limits of the clipping box
   //------------------------------------------------------------------------------
   void setClippingBox(
         const double xmin, const double xmax,
         const double ymin, const double ymax,
         const double zmin = -std::numeric_limits<float>::max(),
         const double zmax = std::numeric_limits<float>::max()
      );

   //------------------------------------------------------------------------------
   // clip() --  Clips the convex polygon 'inPoly' to the clipping box defined by
   // setClippingBox().  The function returns a pointer to a new, clipped polygon,
   // which has been pre-ref()'d, or zero if the whole 'inPoly' polygon is outside of
   // the clipping box.
   //------------------------------------------------------------------------------
   Polygon* clip(const Polygon& inPoly) const;

   //------------------------------------------------------------------------------
   // clip() --  Clips the convex polygon 'in', which contains 'num' vertices, to
   // the clipping box defined by setBox().  The function returns the number of
   // vertices in the clipped polygon or zero if the whole polygon is outside of
   // the clipping box.  The vertices of the clipped polygon are returned in the
   // user provided output arrays, which must be of at least size 'num'.
   //------------------------------------------------------------------------------
   unsigned int clip(
         base::Vec3d* const out,            // Output polygon vertices
         base::Vec3d* const outNorm,        // Output polygon vertices
         base::Vec2d* const outTCoord,      // Output polygon vertices
         const base::Vec3d* const in,       // Input polygon vertices
         const base::Vec3d* const inNorm,   // Input polygon vertices
         const base::Vec2d* const inTCoord, // Input polygon vertices
         const unsigned int num             // Number of vertices in arrays
      ) const;

   //------------------------------------------------------------------------------
   // polyClip2Halfspace() -- clips one coordinate of a convex polygon p against
   // an upper or lower plane.  The clipped polygon is returned in q.  The number
   // of vertices in the clipped polygon is returned by the function.
   //------------------------------------------------------------------------------
   static unsigned int polyClip2Halfspace(
      base::Vec3d* const q,           // Output polygon vertices
      base::Vec3d* const qn,          // (optional) Output polygon normals
      base::Vec2d* const qt,          // (optional) Output polygon texture coordinates
      const base::Vec3d* const p,     // Input polygon vertices
      const base::Vec3d* const pn,    // (optional) Input polygon normals
      const base::Vec2d* const pt,    // (optional) Input polygon texture coordinates
      const unsigned int n,           // Number of vertices/normals
      const unsigned int index,       // Coordinate index: X -> 0; Y -> 1; and Z -> 2
      const double k,                 // Value of the clipping plane
      const bool clipUpperPlane);     // if clipping against an upper plane, 
                                      //  else against a lower plane

private:
   double x0, x1;     // left and right
   double y0, y1;     // top and bottom
   double z0, z1;     // near and far
};

}
}

#endif

