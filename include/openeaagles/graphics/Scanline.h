//------------------------------------------------------------------------------
// Class: Scanline
//------------------------------------------------------------------------------
#ifndef __oe_graphics_Scanline_H__
#define __oe_graphics_Scanline_H__

#include "openeaagles/base/Object.h"
#include "openeaagles/base/osg/Vec2"
#include "openeaagles/base/osg/Vec3"
#include "openeaagles/base/osg/Matrix"
#include "openeaagles/base/safe_ptr.h"

namespace oe {
namespace graphics {

class Clip3D;
class Polygon;

//------------------------------------------------------------------------------
// Class: Scanline
// Base class: Object -> Scanline
// Description:  (Abstract) 2D scan line engine.
// Factory name: Scanline
//------------------------------------------------------------------------------
class Scanline : public base::Object
{
   DECLARE_SUBCLASS(Scanline,base::Object)

public:
   Scanline();

   // Generates one scan of the 'world'.  The user defined 'callback()' function (via the
   // derived class) is called at each x,y point in the virtual view port.
   void scan();

   // Defines the area that we're going to scan.
   void setArea(const double xCenter, const double yCenter, const double xSize, const double ySize, const double zRotDeg);

   unsigned int getWidth() const          { return ix; }    // Viewport (image) width -- pixels
   unsigned int getHeight() const         { return iy; }    // viewport (image) height -- lines

   void setSize(const unsigned int width, const unsigned int height);   // Sets the viewport size

   // adds a polygon to the 'world'
   bool addPolygon(const Polygon* const poly);

   void clear();

protected:
   // PolyData Description
   class PolyData : public base::Object {
      DECLARE_SUBCLASS(PolyData,base::Object)
   public:
      PolyData();
      void getNorm(osg::Vec3& lnorm, const double x) const;

      double      x0;                    // X value at start
      osg::Vec3   n0;                    // Initial Norm
      osg::Vec3   nslope;                // Norm slope
      bool        aptEdge2;              // reached second edge
      base::safe_ptr<Polygon> polygon;      // Clipped (working) polygon
      base::safe_ptr<const Polygon> orig;   // Original polygon
   };

protected:
   // Edge Description
   class Edge : public base::Object {
      DECLARE_SUBCLASS(Edge,base::Object)
   public:
      Edge();
      Edge(const double v0[2], const osg::Vec3& vn0, const double v1[2], const osg::Vec3& vn1, PolyData* const p);
      Edge(const double v0[2], const double v1[2], PolyData* const p);

      void incEdgeStart();

      osg::Vec2   lv;         // Lower Vertex
      osg::Vec2   uv;         // Upper Vertex
      double      x;          // Current X value
      double      slope;      // slope of the edge
      osg::Vec3   lvn;        // Lower Vertex Norm
      osg::Vec3   cn;         // Current Norm
      osg::Vec3   nslope;     // Norm slope
      bool        valid;      // valid edge
      bool        pointLock;  // after incEdgeStart() edge became a point
      base::safe_ptr<PolyData> polygon;    // This edge belongs to this polygon
   };

protected:
   virtual void callback(const PolyData* const p, const unsigned int x, const unsigned int y);

   virtual void reset();
   virtual void scanline(const int y);
   virtual const PolyData* step(const int x);
   virtual void toggleActivePolygon();

   virtual unsigned int reduceVert(Polygon* const poly);
   virtual void purgePolygons();
   virtual void setMatrix();
   virtual void sortEdges(Edge* tbl[], const int n, const int index);
   virtual void endPointCheck(Edge* tbl[], const int n) const;
   virtual void add2EdgeTable(Edge* tbl[], const int n);

private:
   void initData();

   static const unsigned int MAX_EDGES = 4000;
   static const unsigned int MAX_ACTIVE_EDGES = 1000;
   static const unsigned int MAX_POLYS = 500;
   static const unsigned int MAX_ACTIVE_POLYS = 100;

   Clip3D*        clipper;                // clipping object

   double         angle;                  // area rotation angle     (radians)
   double         cx, cy;                 // area center (x,y)
   double         sx, sy;                 // area size (x,y)
   unsigned int   ix,iy;                  // number of pixels (x,y)

   double         curX;                   // current X value (pixel number)
   double         curY;                   // current Y value (scanline number)
   PolyData*       curPoly;               // Current polygon (one on top)

   osg::Matrix    mat;                    // matrix used by scanner

   PolyData*       pt[MAX_POLYS];         // Polygons Table (PT)
   unsigned int   nPT;                    // Number of polygons in PT

   PolyData*       apt[MAX_ACTIVE_POLYS]; // Active Polygon Table (APT)
   unsigned int   nAPT;                   // Number of polygons in APT

   Edge*          et[MAX_EDGES];          // Edge Table (ET)
   unsigned int   nET;                    // Number of edges in ET
   unsigned int   refET;                  // Ref index for ET

   Edge*          aet[MAX_ACTIVE_EDGES];  // Active Edge Table (AET)
   unsigned int   nAET;                   // Number of edges in AET
   unsigned int   refAET;                 // Ref index for AET
};


} // End graphics namespace
} // End oe namespace

#endif
