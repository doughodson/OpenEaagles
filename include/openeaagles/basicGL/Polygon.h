//------------------------------------------------------------------------------
// Class: Polygon
//------------------------------------------------------------------------------
#ifndef	__Eaagles_BasicGL_Polygon_H__
#define __Eaagles_BasicGL_Polygon_H__

#include "Graphic.h"

namespace Eaagles {
namespace BasicGL {

//------------------------------------------------------------------------------
// Class:       Polygon
// Base class:  Object -> Component -> Graphic -> Polygon
//
// Description: General purpose polygon
//
// Form name: Polygon
// Public methods: Base class public methods, plus ...
//
//    const osg::Vec3* getNormal() const
//       Returns the vector normal to this polygon.  Calling calcNormal() will
//       compute the normal vector for this polygon.
//
//    bool isNormalVectorValid() const
//       Returns true if the vector normal to this polygon has been computed.
//       Calling calcNormal() will compute the normal vector for this polygon.
//       
//    bool calcNormal()
//       Computes the vector normal to this polygon.
//
//
//    const osg::Vec4* getPlaneCoeff() const
//       Returns the plane equation coefficients for the plane
//       containing this polygon.  Calling calcPlaneCoeff() will
//       compute these coefficients for this polygon
//
//    bool arePlaneCoeffValid() const 
//       Returns true if the plane equation coefficients for this
//       polygon are valid.  Calling calcPlaneCoeff() will compute
//       these coefficients for this polygon.
//
//    bool calcPlaneCoeff()
//       Computes the plane equation coefficients for this polygon.
//
//
//    LCreal calcZ(const osg::Vec2& p) const
//       Computes the Z value for a point, 'p', on this polygon.
//       The plane equation coefficients must be valid (see arePlaneCoeffValid()).
//       Calling calcPlaneCoeff() will compute these coefficients for this polygon
//
//
// Static, public methods:
//
//    bool calcNormal(osg::Vec3& norm, const osg::Vec3 v[3])
//       Computes the normal vector, 'norm', for the plane define by
//       the three vectices, v.  Returns true if 'norm' is valid.
//
//    bool calcPlaneCoeff(osg::Vec4& coeff, const osg::Vec3 v[3])
//       Computes the plane equation coefficients for the plane defined by
//       the three vectices, v.  Returns true if 'coeff' is valid.
//
//    LCreal calcZ(const osg::Vec2& p, const osg::Vec4& coeff)
//       Computes the Z value for a point, 'p', located on a plane, which
//       is defined by the plane equation coefficients, 'coeff'.
//
//------------------------------------------------------------------------------
class Polygon : public Graphic {
    DECLARE_SUBCLASS(Polygon,Graphic)

public:
      enum { A=0, B=1, C=2, D=3 };

public:
   Polygon();

   const osg::Vec3* getNormal() const        { return &norm; }
   void setNormal(const osg::Vec3& nn)       { norm = nn; }
   bool calcNormal();
   static bool calcNormal(osg::Vec3& norm, const osg::Vec3 v[3]);

   // Coefficients of the plane equation
   bool arePlaneCoeffValid() const           { return coeffValid; }
   const osg::Vec4* getPlaneCoeff() const    { return &coeff; }
   bool calcPlaneCoeff();
   static bool calcPlaneCoeff(osg::Vec4& cc, const osg::Vec3 v[3]);

   // Compute the Z value of the polygon
   LCreal calcZ(const osg::Vec2& point) const;
   static LCreal calcZ(const osg::Vec2& point, const osg::Vec4& coeff);

   unsigned int getLayer() const             { return layerValue; }
   void setLayer(const unsigned int newLayer);

   // Graphic Interface
   virtual void drawFunc(); 

private:
   osg::Vec4      coeff;      // Coefficients of the plane equation
   bool           coeffValid; // 'coeff' numbers are valid
   osg::Vec3      norm;       // normal to the polygon
   unsigned int   layerValue; // Layer value
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif
