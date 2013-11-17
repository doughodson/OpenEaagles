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
//------------------------------------------------------------------------------
#include "openeaagles/basicGL/Clip3D.h"

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_SUBCLASS(Clip3D,"Clip3D")
EMPTY_SLOTTABLE(Clip3D)
EMPTY_SERIALIZER(Clip3D)


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Clip3D::Clip3D()
{
   STANDARD_CONSTRUCTOR()

   setClippingBox(-FLT_MAX, FLT_MAX, -FLT_MAX, FLT_MAX, -FLT_MAX, FLT_MAX);
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Clip3D::copyData(const Clip3D& org, const bool)
{
    BaseClass::copyData(org);

    x0 = org.x0;
    x1 = org.x1;
    y0 = org.y0;
    y1 = org.y1;
    z0 = org.z0;
    z1 = org.z1;
}


//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Clip3D::deleteData()
{
    setClippingBox(-FLT_MAX, FLT_MAX, -FLT_MAX, FLT_MAX, -FLT_MAX, FLT_MAX);
}


//------------------------------------------------------------------------------
// Sets the limits of the clipping box
//------------------------------------------------------------------------------
void Clip3D::setClippingBox(
      const LCreal xmin, const LCreal xmax,
      const LCreal ymin, const LCreal ymax,
      const LCreal zmin, const LCreal zmax)
{
   x0 = xmin;
   x1 = xmax;
   y0 = ymin;
   y1 = ymax;
   z0 = zmin;
   z1 = zmax;
}

//------------------------------------------------------------------------------
// clip() --  Clips the convex polygon 'inPoly' to the clipping box defined by
// setClippingBox().  The function returns a pointer to a new, clipped polygon,
// which has been pre-ref()'d, or zero if the whole 'inPoly' polygon is outside of
// the clipping box.
//------------------------------------------------------------------------------
Polygon* Clip3D::clip(const Polygon& inPoly) const
{
   // quick out
   if (&inPoly == 0) return 0;

   // ---
   // Get the vertices and make sure we have at least two
   // ---
   const unsigned int num = inPoly.getNumberOfVertices();
   const osg::Vec3* const in = inPoly.getVertices();
   if (in != 0 && inPoly.getNumberOfVertices() < 2) return 0;

   // ---
   // count vertices "outside" with respect to each of the six planes
   // ---
   unsigned int x0Out = 0;
   unsigned int x1Out = 0;
   unsigned int y0Out = 0;
   unsigned int y1Out = 0;
   unsigned int z0Out = 0;
   unsigned int z1Out = 0;
   for (unsigned int i = 0; i < num; i++) {
      if (in[i][0] < x0) x0Out++;
      if (in[i][0] > x1) x1Out++;
      if (in[i][1] < y0) y0Out++;
      if (in[i][1] > y1) y1Out++;
      if (in[i][2] < z0) z0Out++;
      if (in[i][2] > z1) z1Out++;
   }

   // ---
   // check if all vertices are "outside" any one of the six planes
   // ---
   if ( x0Out >= num || x1Out >= num || y0Out >= num ||
        y1Out >= num || z0Out >= num || z1Out >= num ) {
      // yes -- then there's nothing to output
      return 0;
   }

   // ---
   // Clone the original polygon 
   // ---
   Polygon* outPoly = (Polygon*) inPoly.clone();

   // ---
   // check if all vertices are "inside" all of the six planes
   // ---
   if ( (x0Out+x1Out+y0Out+y1Out+z0Out+z1Out) == 0) {
      return outPoly;
   }

   // ---
   // Gather input data
   // ---
   const osg::Vec3* const inNorm   = inPoly.getNormals();
   const unsigned int numNorms     = inPoly.getNumberOfNormals();
   const osg::Vec2* const inTCoord = inPoly.getTextureCoord();
   const unsigned int numTCoords   = inPoly.getNumberOfTextureCoords();

   // ---
   // Our temporary polygons
   // ---

   // p: pre-clipped polygon
   osg::Vec3* const poly1 = new osg::Vec3[num*3];
   for (unsigned int i = 0; i < num; i++) {
      poly1[i] = in[i];
   }
   osg::Vec3* p = poly1;

   // pn: pre-clipped normals
   osg::Vec3* norm1 = 0;
   osg::Vec3* pn = 0;
   if (inNorm != 0 && numNorms == num) {
      norm1 = new osg::Vec3[numNorms*3];
      for (unsigned int i = 0; i < numNorms; i++) {
         norm1[i] = inNorm[i];
      }
      pn = norm1;
   }

   // pt: pre-clipped texture coords
   osg::Vec2* tcoord1 = 0;
   osg::Vec2* pt = 0;
   if (inTCoord != 0 && numTCoords == num) {
      tcoord1 = new osg::Vec2[numTCoords*3];
      for (unsigned int i = 0; i < numTCoords; i++) {
         tcoord1[i] = inTCoord[i];
      }
      pt = tcoord1;
   }

   // q: clipped polygon
   osg::Vec3* const poly2 = new osg::Vec3[num*3];
   osg::Vec3* q = poly2;

   // qn: clipped normals
   osg::Vec3* norm2 = 0;
   osg::Vec3* qn = 0;
   if (inNorm != 0 && numNorms == num) {
      norm2 = new osg::Vec3[numNorms*3];
      qn = norm2;
   }

   // qt: clippped texture coords
   osg::Vec2* tcoord2 = 0;
   osg::Vec2* qt = 0;
   if (inTCoord != 0 && numTCoords == num) {
      tcoord2 = new osg::Vec2[numTCoords*3];
      qt = tcoord2;
   }

   // ---
   // now clip against each of the planes that might cut the polygon,
   // at each step toggle 'p' and 'q' between polygons 'poly1' and 'poly2'
   // ---
   unsigned int n = num;
   if (x0Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 0, x0, false);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }
   if (x1Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 0, x1, true);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }
   if (y0Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 1, y0, false);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }
   if (y1Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 1, y1, true);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }
   if (z0Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 2, z0, false);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }
   if (z1Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 2, z1, true);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }

   // copy the clipped polygon to the output arrays, which after the
   // last toggle should be 'p'.
   if (n > 1) {
      outPoly->setVertices(p,n);

      if (pn != 0) outPoly->setNormals(pn,n);
      else outPoly->setNormals(0,0);

      if (pt != 0) outPoly->setTextureCoord(pt,n);
      else outPoly->setTextureCoord(0,0);
   }
   else {
      // Nothing really to output, so delete the output polygon
      outPoly->unref();
      outPoly = 0;
   }

   // cleanup
   delete[] poly1;
   delete[] poly2;
   if (norm1 != 0) delete[] norm1;
   if (norm2 != 0) delete[] norm2;
   if (tcoord1 != 0) delete[] tcoord1;
   if (tcoord2 != 0) delete[] tcoord2;

   return outPoly;
}



//------------------------------------------------------------------------------
// clip() --  Clips the convex polygon 'in', which contains 'num' vertices, to
// the clipping box defined by setClippingBox().  The function returns the number
// of vertices in the clipped polygon or zero if the whole polygon is outside of
// the clipping box.  The vertices of the clipped polygon are returned in the
// user provided array, 'out', which must be at least of size 'num'.
//------------------------------------------------------------------------------
unsigned int Clip3D::clip(
      osg::Vec3* const out,            // Output polygon vertex array
      osg::Vec3* const outNorm,        // (optional) Output polygon normals (or zero)
      osg::Vec2* const outTCoord,      // (optional) Output polygon texture coordinates (or zero)
      const osg::Vec3* const in,       // Input polygon vertex array
      const osg::Vec3* const inNorm,   // (optional) Input polygon normals (or zero)
      const osg::Vec2* const inTCoord, // (optional) Input polygon texture coordinates (or zero)
      const unsigned int num           // Number of vertices in arrays
   ) const
{
   // quick out
   if (out == 0 || in == 0 || num < 2) return 0;

   // ---
   // count vertices "outside" with respect to each of the six planes
   // ---
   unsigned int x0Out = 0;
   unsigned int x1Out = 0;
   unsigned int y0Out = 0;
   unsigned int y1Out = 0;
   unsigned int z0Out = 0;
   unsigned int z1Out = 0;
   for (unsigned int i = 0; i < num; i++) {
      if (in[i][0] < x0) x0Out++;
      if (in[i][0] > x1) x1Out++;
      if (in[i][1] < y0) y0Out++;
      if (in[i][1] > y1) y1Out++;
      if (in[i][2] < z0) z0Out++;
      if (in[i][2] > z1) z1Out++;
   }

   // ---
   // check if all vertices are "outside" any one of the six planes
   // ---
   if ( x0Out >= num || x1Out >= num || y0Out >= num ||
        y1Out >= num || z0Out >= num || z1Out >= num ) {
      // yes -- then there's nothing to output
      return 0;
   }

   // ---
   // check if all vertices are "inside" all of the six planes
   // ---
   if ( (x0Out+x1Out+y0Out+y1Out+z0Out+z1Out) == 0) {
      // yes -- then just pass the polygon through ...
      for (unsigned int i = 0; i < num; i++) {
         out[i] = in[i];
      }
      return num;
   }

   // ---
   // Our temporary polygons
   // ---

   // p: pre-clipped polygon
   osg::Vec3* const poly1 = new osg::Vec3[num*3];
   for (unsigned int i = 0; i < num; i++) {
      poly1[i] = in[i];
   }
   osg::Vec3* p = poly1;

   // pn: pre-clipped normals
   osg::Vec3* norm1 = 0;
   osg::Vec3* pn = 0;
   if (inNorm != 0 && outNorm != 0) {
      norm1 = new osg::Vec3[num*3];
      for (unsigned int i = 0; i < num; i++) {
         norm1[i] = inNorm[i];
      }
      pn = norm1;
   }

   // pt: pre-clipped texture coords
   osg::Vec2* tcoord1 = 0;
   osg::Vec2* pt = 0;
   if (inTCoord != 0 && outTCoord != 0) {
      tcoord1 = new osg::Vec2[num*3];
      for (unsigned int i = 0; i < num; i++) {
         tcoord1[i] = inTCoord[i];
      }
      pt = tcoord1;
   }

   // q: clipped polygon
   osg::Vec3* const poly2 = new osg::Vec3[num*3];
   osg::Vec3* q = poly2;

   // qn: clipped normals
   osg::Vec3* norm2 = 0;
   osg::Vec3* qn = 0;
   if (inNorm != 0 && outNorm != 0) {
      norm2 = new osg::Vec3[num*3];
      qn = norm2;
   }

   // qt: clippped texture coords
   osg::Vec2* tcoord2 = 0;
   osg::Vec2* qt = 0;
   if (inTCoord != 0 && outTCoord != 0) {
      tcoord2 = new osg::Vec2[num*3];
      qt = tcoord2;
   }

   // ---
   // now clip against each of the planes that might cut the polygon,
   // at each step toggle 'p' and 'q' between polygons 'poly1' and 'poly2'
   // ---
   unsigned int n = num;
   if (x0Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 0, x0, false);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }
   if (x1Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 0, x1, true);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }
   if (y0Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 1, y0, false);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }
   if (y1Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 1, y1, true);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }
   if (z0Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 2, z0, false);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }
   if (z1Out > 0 && n > 1) {
      n = polyClip2Halfspace(q, qn, qt, p, pn, pt, n, 2, z1, true);
      { osg::Vec3* r = p; p = q; q = r; }
      if (pn != 0) { osg::Vec3* r = pn; pn = qn; qn = r; }
      if (pt != 0) { osg::Vec2* r = pt; pt = qt; qt = r; }
   }

   // copy the clipped polygon to the output arrays, which after the
   // last toggle should be 'p'.
   if (n > 1) {
      for (unsigned int i = 0; i < n; i++) {
         out[i] = p[i];
      }
      if (pn != 0) {
         for (unsigned int i = 0; i < n; i++) {
            outNorm[i] = pn[i];
         }
      }
      if (pt != 0) {
         for (unsigned int i = 0; i < n; i++) {
            outTCoord[i] = pt[i];
         }
      }
   }
   else {
      n = 0;
   }

   // cleanup
   delete[] poly1;
   delete[] poly2;
   if (norm1 != 0) delete[] norm1;
   if (norm2 != 0) delete[] norm2;
   if (tcoord1 != 0) delete[] tcoord1;
   if (tcoord2 != 0) delete[] tcoord2;

   return n;
}


//------------------------------------------------------------------------------
// polyClip2Halfspace() -- clips one coordinate of a convex polygon p against
// an upper or lower plane.  The clipped polygon is returned in q.  The number
// of vertices in the clipped polygon is returned by the function.
//------------------------------------------------------------------------------
unsigned int Clip3D::polyClip2Halfspace(
   osg::Vec3* const q,           // Output polygon vertex array
   osg::Vec3* const qn,          // (optional) Output polygon normals
   osg::Vec2* const qt,          // (optional) Output polygon texture coordinates
   const osg::Vec3* const p,     // Input polygon vertex array
   const osg::Vec3* const pn,    // (optional) Input polygon normals
   const osg::Vec2* const pt,    // (optional) Input polygon texture coordinates
   const unsigned int n,         // Number of vertices/normals
   const unsigned int index,     // Coordinate index: X -> 0; Y -> 1; and Z -> 2
   const LCreal k,               // Value of the clipping plane
   const bool clipUpperPlane)    // if clipping against an upper plane, 
                                 // else against a lower plane
{
   unsigned int m = 0;  // Number of clipper vertices generated

   // quick out: No input/output vertex arrays or too few vertices
   if ( p == 0 || q == 0 || n < 2 ) return 0;

   // Which clipping plane (upper or lower)
   LCreal sign = -1.0f;
   if (clipUpperPlane) sign = 1.0f;

   // Loop for all vertices; start with v = 0 and u = n-1 
   //  (on original polygon (p), 'v' is current vertex and 'u' is previous vertex)
   unsigned int u = n-1;
   LCreal tu = sign * (p[u][index] - k);
   for (unsigned int v = 0; v < n; v++) {

      LCreal tv = sign * (p[v][index] - k);

      // tu and tv are negative if the vertexes are IN 
      bool uf = tu <= 0.0f;
      bool vf = tv <= 0.0f;

      // Exclusive OR to see if the edge crosses the plane;
      if ( (uf && !vf) || (!uf && vf) ) {
         // yes, compute and add the intersection point to q
         LCreal t = tu/(tu-tv);
         q[m]  =  p[u] + (p[v] - p[u]) * t;
         if (pn != 0 && qn != 0) {
            // Interpolate between the normals
            qn[m] = pn[u] + (pn[v] - pn[u]) * t;
         }
         if (pt != 0 && qt != 0) {
            // Interpolate between the texture coordinates
            qt[m] = pt[u] + (pt[v] - pt[u]) * t;
         }
         m++;
      }

      // and if the vertex v is IN, copy it to q
      if (vf) {
         q[m]  = p[v];
         if (pn != 0 && qn != 0) {
            qn[m] = pn[v];
         }
         if (pt != 0 && qt != 0) {
            qt[m] = pt[v];
         }
         m++;
      }

      // update pointers
      u = v;
      tu = tv;
   }

   return m;
}


} // End BasicGL namespace
} // End Eaagles namespace
