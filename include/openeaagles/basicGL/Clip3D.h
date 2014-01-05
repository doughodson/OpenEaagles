//------------------------------------------------------------------------------
// Class: Clip3D
//------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_Clip3D_H__
#define __Eaagles_BasicGL_Clip3D_H__

#include "openeaagles/basic/Object.h"
#include "Polygon.h"

namespace Eaagles {
namespace BasicGL {

//------------------------------------------------------------------------------
// Class: Clip3D
// Base class: Object -> Clip3D
//
// Factory name: Clip3D
// Description: 3D polygon clipping
//
//------------------------------------------------------------------------------
class Clip3D : public Basic::Object {
    DECLARE_SUBCLASS(Clip3D,Basic::Object)

public:
   Clip3D();

   //------------------------------------------------------------------------------
   // Sets the limits of the clipping box
   //------------------------------------------------------------------------------
   void setClippingBox(
         const LCreal xmin, const LCreal xmax,
         const LCreal ymin, const LCreal ymax,
         const LCreal zmin = -FLT_MAX, const LCreal zmax = FLT_MAX
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
         osg::Vec3* const out,            // Output polygon vertices
         osg::Vec3* const outNorm,        // Output polygon vertices
         osg::Vec2* const outTCoord,      // Output polygon vertices
         const osg::Vec3* const in,       // Input polygon vertices
         const osg::Vec3* const inNorm,   // Input polygon vertices
         const osg::Vec2* const inTCoord, // Input polygon vertices
         const unsigned int num           // Number of vertices in arrays
      ) const;

   //------------------------------------------------------------------------------
   // polyClip2Halfspace() -- clips one coordinate of a convex polygon p against
   // an upper or lower plane.  The clipped polygon is returned in q.  The number
   // of vertices in the clipped polygon is returned by the function.
   //------------------------------------------------------------------------------
   static unsigned int polyClip2Halfspace(
      osg::Vec3* const q,           // Output polygon vertices
      osg::Vec3* const qn,          // (optional) Output polygon normals
      osg::Vec2* const qt,          // (optional) Output polygon texture coordinates
      const osg::Vec3* const p,     // Input polygon vertices
      const osg::Vec3* const pn,    // (optional) Input polygon normals
      const osg::Vec2* const pt,    // (optional) Input polygon texture coordinates
      const unsigned int n,         // Number of vertices/normals
      const unsigned int index,     // Coordinate index: X -> 0; Y -> 1; and Z -> 2
      const LCreal k,               // Value of the clipping plane
      const bool clipUpperPlane);   // if clipping against an upper plane, 
                                    //  else against a lower plane

private:
   LCreal x0, x1;     // left and right
   LCreal y0, y1;     // top and bottom
   LCreal z0, z1;     // near and far
};

} // End BasicGL namespace
} // End Eaagles namespace


#endif
