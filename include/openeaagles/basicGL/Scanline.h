//------------------------------------------------------------------------------
// Class: Scanline
//------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_Scanline_H__
#define __Eaagles_BasicGL_Scanline_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace BasicGL {

class Clip3D;
class Polygon;

//------------------------------------------------------------------------------
// Class: Scanline
// Base class: Object -> Scanline
// Description:  (Abstract) 2D scan line engine.
// Factory name: Scanline
//------------------------------------------------------------------------------
class Scanline : public Basic::Object
{
   DECLARE_SUBCLASS(Scanline,Basic::Object)

public:
   Scanline();

   // Generates one scan of the 'world'.  The user defined 'callback()' function (via the
   // derived class) is called at each x,y point in the virtual view port.
   void scan();

   // Defines the area that we're going to scan.
   void setArea(const LCreal xCenter, const LCreal yCenter, const LCreal xSize, const LCreal ySize, const LCreal zRotDeg);

   unsigned int getWidth() const          { return ix; }    // Viewport (image) width -- pixels
   unsigned int getHeight() const         { return iy; }    // viewport (image) height -- lines

   void setSize(const unsigned int width, const unsigned int height);   // Sets the viewport size

   // adds a polygon to the 'world'
   bool addPolygon(const Polygon* const poly);

   void clear();

protected:
   // PolyData Description
   class PolyData : public Basic::Object {
      DECLARE_SUBCLASS(PolyData,Basic::Object)
   public:
      PolyData();
      void getNorm(osg::Vec3& lnorm, const LCreal x) const;

      LCreal      x0;                    // X value at start
      osg::Vec3   n0;                    // Initial Norm
      osg::Vec3   nslope;                // Norm slope
      bool        aptEdge2;              // reached second edge
      Basic::safe_ptr<Polygon> polygon;      // Clipped (working) polygon
      Basic::safe_ptr<const Polygon> orig;   // Original polygon
   };

protected:
   // Edge Description
   class Edge : public Basic::Object {
      DECLARE_SUBCLASS(Edge,Basic::Object)
   public:
      Edge();
      Edge(const LCreal v0[2], const osg::Vec3& vn0, const LCreal v1[2], const osg::Vec3& vn1, PolyData* const p);
      Edge(const LCreal v0[2], const LCreal v1[2], PolyData* const p);

      void incEdgeStart();

      osg::Vec2   lv;         // Lower Vertex
      osg::Vec2   uv;         // Upper Vertex
      LCreal      x;          // Current X value
      LCreal      slope;      // slope of the edge
      osg::Vec3   lvn;        // Lower Vertex Norm
      osg::Vec3   cn;         // Current Norm
      osg::Vec3   nslope;     // Norm slope
      bool        valid;      // valid edge
      bool        pointLock;  // after incEdgeStart() edge became a point
      Basic::safe_ptr<PolyData> polygon;    // This edge belongs to this polygon
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

   LCreal         angle;                  // area rotation angle     (radians)
   LCreal         cx, cy;                 // area center (x,y)
   LCreal         sx, sy;                 // area size (x,y)
   unsigned int   ix,iy;                  // number of pixels (x,y)

   LCreal         curX;                   // current X value (pixel number)
   LCreal         curY;                   // current Y value (scanline number)
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


} // End BasicGL namespace
} // End Eaagles namespace

#endif
