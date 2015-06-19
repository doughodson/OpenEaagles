//==============================================================================
// Scanline --
//==============================================================================

#include "openeaagles/basicGL/Scanline.h"
#include "openeaagles/basicGL/Clip3D.h"
#include "openeaagles/basicGL/Polygon.h"
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_SUBCLASS(Scanline,"Scanline")
EMPTY_SLOTTABLE(Scanline)
EMPTY_SERIALIZER(Scanline)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Scanline::Scanline()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Scanline::initData()
{
   angle = 0.0f;

   cx = 239.5f;
   cy = 239.5f;

   sx = 479.0f;
   sy = 479.0f;

   ix = 480;
   iy = 480;

   curX = 0;
   curY = 0;
   curPoly = nullptr;

   for (unsigned int i = 0; i < MAX_POLYS; i++) {
      pt[i] = nullptr;
   }
   nPT = 0;

   for (unsigned int i = 0; i < MAX_ACTIVE_POLYS; i++) {
      apt[i] = nullptr;
   }
   nAPT = 0;

   for (unsigned int i = 0; i < MAX_EDGES; i++) {
      et[i] = nullptr;
   }
   nET = 0;
   refET = 0;

   for (unsigned int i = 0; i < MAX_ACTIVE_EDGES; i++) {
      aet[i] = nullptr;
   }
   nAET = 0;
   refAET = 0;

   setMatrix();

   clipper = new Clip3D();
   clipper->setClippingBox(0.0f, static_cast<float>(ix-1), 0.0f, static_cast<float>(iy-1));
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Scanline::copyData(const Scanline& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

}


//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Scanline::deleteData()
{
   clipper->unref();
   clipper = nullptr;
}

//------------------------------------------------------------------------------
// setMatrix() -- define the transformation matrix
//------------------------------------------------------------------------------
void Scanline::setMatrix()
{
   osg::Matrix rr;

   // start with an identity matrix
   mat.makeIdentity();

   // translate origin from center to lower left corner
   rr.makeTranslate((static_cast<LCreal>(ix-1)/2.0f), (static_cast<LCreal>(iy-1)/2.0f), 0.0f);
   mat.preMult(rr);

   // rotate
   rr.makeRotate(angle,0.0f,0.0f,1.0f);
   mat.preMult(rr);

   // scale world area size to window size
   LCreal scaleX = static_cast<LCreal>(ix-1)/sx;
   LCreal scaleY = static_cast<LCreal>(iy-1)/sy;
   rr.makeScale(scaleX, scaleY, 1.0f);
   mat.preMult(rr);

   // translate world orgin to center of display area
   rr.makeTranslate(-cx,-cy,0.0f);
   mat.preMult(rr);
}


//------------------------------------------------------------------------------
// callback() -- default handler
//------------------------------------------------------------------------------
void Scanline::callback(const PolyData* const /*p*/, const unsigned int /*x*/, const unsigned int /*y*/)
{
}

//------------------------------------------------------------------------------
// reset() -- reset the scanline alg but keep the polygon and edge tables
//------------------------------------------------------------------------------
void Scanline::reset()
{
   refET = 0;
   nAET = 0;
   refAET = 0;
   nAPT = 0;
   curY = 0.0f;
   curPoly = nullptr;
   if (nET > 0) sortEdges(et,nET,1);
}

//------------------------------------------------------------------------------
// sortEdges() -- sort the edge table; simple bubble up sort
//    index == 0     sort by x
//    index == 1     sort by lv[1]
//------------------------------------------------------------------------------
void Scanline::sortEdges(Edge* tbl[], const int n, const int index)
{
   // return if there are no edges to sort
   if (n < 2) return;

   if (index == 0) {
      for (int i = n-2; i >= 0; i--) {
         int j = i;
         while (j < (n-1) && tbl[j]->x > tbl[j+1]->x) {
            Edge* p = tbl[j];
            tbl[j] = tbl[j+1];
            tbl[j+1] = p;
            j++;
         }
      }
   }
   else if (index == 1) {
      for (int i = n-2; i >= 0; i--) {
         int j = i;
         while (j < (n-1) && tbl[j]->lv[1] > tbl[j+1]->lv[1]) {
            Edge* p = tbl[j];
            tbl[j] = tbl[j+1];
            tbl[j+1] = p;
            j++;
         }
      }
   }

}

//------------------------------------------------------------------------------
// setArea() -- set the world coord area that we are going to clip and scan convert
//------------------------------------------------------------------------------
void Scanline::setArea(
      const LCreal xCenter, const LCreal yCenter,
      const LCreal xSize, const LCreal ySize,
      const LCreal zRotDeg)
{
   cx = xCenter;
   cy = yCenter;
   sx = xSize;
   sy = ySize;
   angle = zRotDeg * static_cast<LCreal>(Basic::Angle::D2RCC);
   setMatrix();
}

//------------------------------------------------------------------------------
// setSize() -- set the size of the 'screen' we are going to scan convert into
//------------------------------------------------------------------------------
void Scanline::setSize(const unsigned int x, const unsigned int y)
{
   ix = x;
   iy = y;
   setMatrix();
   clipper->setClippingBox(0.0f, static_cast<LCreal>(ix-1), 0.0f, static_cast<LCreal>(iy-1));
}

//------------------------------------------------------------------------------
// erase() -- empty all the tables
//------------------------------------------------------------------------------
void Scanline::clear()
{
   reset();

   // empty the edge table
   while (nET > 0) {
      --nET;
      et[nET]->unref();
      et[nET] = nullptr;
   }

   // empty the polygon table
   while (nPT > 0) {
      --nPT;
      pt[nPT]->unref();
      pt[nPT] = nullptr;
   }
}

//------------------------------------------------------------------------------
// purgePolygons() -- purge the active polygon list of polygons that have
// reached the seconds edge
//------------------------------------------------------------------------------
void Scanline::purgePolygons()
{
   for (int i = nAPT-1; i >= 0; i--) {
      if (apt[i]->aptEdge2) {
         apt[i]->unref();
         for (unsigned int j = i; j < nAPT-1; j++) {
            apt[j] = apt[j+1];
         }
         nAPT--;
      }
   }
}

//------------------------------------------------------------------------------
// reduceVert() -- reduce the number of vertices by removing consecutive
// vertices that are on the same pixel.
//------------------------------------------------------------------------------
unsigned int Scanline::reduceVert(Polygon* const polygon)
{
   if (polygon == nullptr) return 0;

   unsigned int n1 = polygon->getNumberOfVertices();
   osg::Vec3* tvect  = const_cast<osg::Vec3*>(static_cast<const osg::Vec3*>(polygon->getVertices()));
   if (tvect == nullptr) return 0;

   osg::Vec3* tnorms = const_cast<osg::Vec3*>(static_cast<const osg::Vec3*>(polygon->getNormals()));
   osg::Vec2* tcoord = const_cast<osg::Vec2*>(static_cast<const osg::Vec2*>(polygon->getTextureCoord()));

   bool reduced = true;
   while (reduced && n1 > 2) {
      reduced = false;
      int ix0 = static_cast<int>( tvect[0][0] + 0.5f );
      int iy0 = static_cast<int>( tvect[0][1] + 0.5f );
      for (unsigned int i = 1; i < n1 && !reduced; i++) {
         int ix1 = static_cast<int>( tvect[i][0] + 0.5f );
         int iy1 = static_cast<int>( tvect[i][1] + 0.5f );
         if (ix0 == ix1 && iy0 == iy1) {
            reduced = true;
            n1--;
            // move the vertices down the array by one position ...
            for (unsigned int j = i; j < n1; j++) {
               tvect[j] = tvect[j+1];
               if (tnorms != nullptr) tnorms[j] = tnorms[j+1];
               if (tcoord != nullptr) tcoord[j] = tcoord[j+1];
            }
         }
         ix0 = ix1;
         iy0 = iy1;
      }
      if (!reduced) {
         // check the last point with the first point
         int ix1 = static_cast<int>( tvect[0][0] + 0.5f );
         int iy1 = static_cast<int>( tvect[0][1] + 0.5f );
         if (ix0 == ix1 && iy0 == iy1) {
            reduced = true;
            n1--;
         }
      }
   }
   return n1;
}

//------------------------------------------------------------------------------
// endPointCheck() -- end point check the edges
//------------------------------------------------------------------------------
void Scanline::endPointCheck(Edge* tbl[], const int n) const
{
   for (int i = 0; i < n; i++) {
      Edge* p = tbl[i];
      for (int j = 0; j < n; j++) {
         Edge* q = tbl[j];
         int qxmin = static_cast<int>( q->lv[0] + 0.5f );
         int qymin = static_cast<int>( q->lv[1] + 0.5f );
         int pxmax = static_cast<int>( p->uv[0] + 0.5f );
         int pymax = static_cast<int>( p->uv[1] + 0.5f );
         if (pxmax == qxmin && pymax == qymin) q->incEdgeStart();
      }
   }
}


//------------------------------------------------------------------------------
// add the edges to the edge table
//------------------------------------------------------------------------------
void Scanline::add2EdgeTable(Edge* tbl[], const int n)
{
   for (int i = 0; i < n; i++) {
      if (tbl[i]->valid) {
         tbl[i]->ref();
         et[nET++] = tbl[i];
      }
   }
}

//------------------------------------------------------------------------------
// addPolygon()
//------------------------------------------------------------------------------
bool Scanline::addPolygon(const Polygon* const polygon)
{
   // quick outs
   if (polygon == nullptr) return false;

   // number of vertices must be at least three
   unsigned int n = polygon->getNumberOfVertices();
   if (n < 3) return false;

   // Also, if there are normals and/or texture coordinates, then make sure
   // that there's one for each vertices.
   unsigned int nn = polygon->getNumberOfNormals();
   unsigned int nt = polygon->getNumberOfTextureCoords();
   if ( (nn > 0 && nn != n) || (nt > 0 && nt != n) ) return false;

   // Create a working copy of the polygon
   Polygon* tmpPolygon = polygon->clone();

   // ---
   // Transform the vertices and normals
   // ---
   {
      osg::Vec3* tv = new osg::Vec3[n];

      // Transform the vectors first
      const osg::Vec3* v = tmpPolygon->getVertices();
      for (unsigned int i = 0; i < n; i++) {
         osg::Vec4 p( v[i][0], v[i][1], v[i][2], 1.0f );
         osg::Vec4 q = p * mat;
         tv[i].set(q[0],q[1],q[2]);
      }
      tmpPolygon->setVertices(tv,n);

      // Transform the normals (if any)
      if (nn > 0) {
         const osg::Vec3* norms = tmpPolygon->getNormals();
         for (unsigned int i = 0; i < nn; i++) {
            osg::Vec4 p( norms[i][0], norms[i][1], norms[i][2], 0.0f );
            osg::Vec4 q = p * mat;
            tv[i].set(q[0],q[1],q[2]);
         }
         tmpPolygon->setNormals(tv,nn);
      }

      delete[] tv;
   }


   //// Reduce the vertices
   reduceVert(tmpPolygon);

   // Clip the polygon (creates a new 'clipped' polygon)
   Polygon* clipPolygon = clipper->clip(*tmpPolygon);
   tmpPolygon->unref();

   // Need at least three vertices after clipping
   unsigned int cn = 0;
   if (clipPolygon != nullptr) cn = clipPolygon->getNumberOfVertices();
   if (cn >= 3) {

      // Back surface removal -- Dot product of a vector in the direction
      // we're looking ( 0, 0, 1 ) and the normal of the new polygon is the
      // same as checking the z value only.
      clipPolygon->calcNormal();
      osg::Vec3 norm = *(clipPolygon->getNormal());
      if (norm[2] < 0.1f) {
         clipPolygon->unref();
         return false;
      }
      clipPolygon->calcPlaneCoeff();

      // Create a new PolyData structure for this polygon
      PolyData* newPolyData = new PolyData();
      newPolyData->polygon = clipPolygon;
      newPolyData->orig = polygon;

      // Add to the polygon table
      pt[nPT++] = newPolyData;

      // Create the edges and store them in a Temporary Edge Table
      int  nTET = 0;
      Edge* tet[100];     // temp edge table
      {
         const osg::Vec3* cvect = clipPolygon->getVertices();
         const osg::Vec3* cnorms = clipPolygon->getNormals();

         unsigned int ii = cn - 1;
         for (unsigned int j = 0; j < cn; j++) {

            Edge* newEdge = nullptr;
            if (cnorms != nullptr) {
               newEdge = new Edge( cvect[ii].ptr(), cnorms[ii], cvect[j].ptr(),  cnorms[j], newPolyData );
            }
            else {
               newEdge = new Edge( cvect[ii].ptr(), cvect[j].ptr(), newPolyData );
            }

            if (newEdge->valid) { tet[nTET++] = newEdge; }
            else { newEdge->unref(); }

            ii = j;
         }
      }

      // end point check the edges in the temporary table
      // endPointCheck(tet,nTET);

      // add the temporary edge table to the real edge table
      add2EdgeTable(tet, nTET);

      for (int i = 0; i < nTET; i++) {
         tet[i]->unref();
         tet[i] = nullptr;
      }
      nTET = 0;
   }

   return true;
}


//------------------------------------------------------------------------------
// scan() -- one pass through the scan pattern
//------------------------------------------------------------------------------
void Scanline::scan()
{
   reset();

   // ---
   // Main loop --
   //   scanlines are y = { 0 .. iy-1 }
   //   pixels in each scanline are x = { 0 .. ix-1 }
   // ---
   for (unsigned int y = 0; y < iy; y++) {
      scanline(y);
      for (unsigned int x = 0; x < ix; x++) {
         const PolyData* p = step(x);
         callback(p,x,y);
      }
   }
}


//------------------------------------------------------------------------------
// scanline() -- select the scanline and setup the AET
//------------------------------------------------------------------------------
void Scanline::scanline(const int y)
{
   // reset some values
   curPoly = nullptr;
   curX = 0.0f;
   curY = static_cast<LCreal>(y);
   refAET = 0;
   nAPT = 0;

   // Purge old edges from the AET
   for (int i = nAET-1; i >= 0; i--) {
      if (curY > aet[i]->uv[1]) {
         for (unsigned int j = i; j < nAET-1; j++) {
            aet[j] = aet[j+1];
         }
         nAET--;
      }
   }

   // Move new edges from ET and put into the AET
   while (refET < nET && curY >= et[refET]->lv[1]) {
      aet[nAET++] = et[refET++];
   }

   // Update x positions and normals of edges in the AET
   for (unsigned int i = 0; i < nAET; i++) {
      LCreal dist = curY - aet[i]->lv[1];
      aet[i]->x = static_cast<LCreal>(aet[i]->lv[0]) + aet[i]->slope*dist;
      aet[i]->cn = aet[i]->lvn + aet[i]->nslope * dist;
   }

   // Sort the AET
   sortEdges(aet, nAET, 0);
}


//------------------------------------------------------------------------------
// step() -- step down the scan line to x
//------------------------------------------------------------------------------
const Scanline::PolyData* Scanline::step(const int x)
{
   curPoly = nullptr;
   curX = static_cast<LCreal>(x);

   // Hit an edge?  Update the active polygon table.
   while (refAET < nAET && curX >= aet[refAET]->x) {
      toggleActivePolygon();
      refAET++;
   }

   // purge old polygons
   purgePolygons();

   // Choose a polygon based on Z
   if (nAPT == 1) {
      // when there is only one active polygon, we choose it
      curPoly = apt[0];
   }
   else if (nAPT > 1) {
      // when there are several active polygon, we choose the one on top
      osg::Vec2 point(curX,curY);
      LCreal zmin = apt[0]->polygon->calcZ(point);
      curPoly = apt[0];
      for (unsigned int i = 1; i < nAPT; i++) {
         LCreal z = apt[i]->polygon->calcZ(point);
         //if (z > (zmin + 0.1f)) {
         if (z > zmin) {
            // when this polygon is higher than the previous
            zmin = z;
            curPoly = apt[i];
         }
         //else if (z > (zmin - 0.1f)) {
         else if (z == zmin) {
            // use layers when this polygon is very close to the previous
            if (apt[i]->polygon->getLayer() > curPoly->polygon->getLayer()) {
               curPoly = apt[i];
               //zmin = z;
            }
         }
      }
   }

   return curPoly;    // return the top polygon
}

//------------------------------------------------------------------------------
// toggleActivePolygon() -- toggle a polygon to and from the active table
//------------------------------------------------------------------------------
void Scanline::toggleActivePolygon()
{
   // p is the polygon we're looking at
   PolyData* p = aet[refAET]->polygon;

   // If it's in the table -- remove it
   bool found = false;
   for (unsigned int i = 0; i < nAPT && !found; i++) {
      if (apt[i] == p) {
         p->aptEdge2 = true;
         found = true;
      }
   }

   // if it wasn't in the table -- add it to table iif there is a second edge
   if (!found) {

      // search for second edge
      for (unsigned int j = refAET+1; j < nAET; j++) {
         if (p == aet[j]->polygon) {
            // found second edge -- add it to table
            p->aptEdge2 = false;
            p->n0 = aet[refAET]->cn;
            p->x0 = aet[refAET]->x;
            LCreal deltaX = (aet[j]->x - p->x0);
            if (deltaX > 0.0f) {
               osg::Vec3 deltaNorm = aet[j]->cn - aet[refAET]->cn;
               p->nslope = deltaNorm * (1.0f/deltaX);
            }
            else {
               p->nslope.set(0.0f, 0.0f, 0.0f);
            }
            p->ref();
            apt[nAPT++] = p;
            break;
         }
      }

   }
}

//==============================================================================
// Scanline::PolyData class
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(Scanline::PolyData,"ScanlinePolyData")
EMPTY_SLOTTABLE(Scanline::PolyData)
EMPTY_SERIALIZER(Scanline::PolyData)

Scanline::PolyData::PolyData() : polygon(nullptr), orig(nullptr)
{
   STANDARD_CONSTRUCTOR()

   x0 = 0.0f;
   n0.set(0.0f,0.0f,1.0f);
   nslope.set(0.0f,0.0f,0.0f);
   aptEdge2 = false;
}

Scanline::PolyData::PolyData(const Scanline::PolyData& org) : polygon(nullptr), orig(nullptr)
{
   STANDARD_CONSTRUCTOR()
   copyData(org,true);
}

Scanline::PolyData::~PolyData()
{
   STANDARD_DESTRUCTOR()
}

Scanline::PolyData& Scanline::PolyData::operator=(const Scanline::PolyData& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

Scanline::PolyData* Scanline::PolyData::clone() const
{
   return new Scanline::PolyData(*this);
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Scanline::PolyData::copyData(const Scanline::PolyData& org, const bool)
{
   BaseClass::copyData(org);

   x0 = org.x0;
   n0 = org.n0;
   nslope = org.nslope;
   aptEdge2 = org.aptEdge2;

   polygon = nullptr;
   if (org.polygon != nullptr) {
      const Polygon* p = org.polygon;
      polygon = const_cast<Polygon*>(static_cast<const Polygon*>(p));
   }

   orig = nullptr;
   if (org.orig != nullptr) {
      const Polygon* p = org.orig;
      orig = p;
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Scanline::PolyData::deleteData()
{
}

//------------------------------------------------------------------------------
// Functions --
//------------------------------------------------------------------------------

void Scanline::PolyData::getNorm(osg::Vec3& cnorm, const LCreal x) const
{
   LCreal dist = x - x0;
   cnorm = n0 + nslope * dist;
}

//==============================================================================
// Edge routines
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(Scanline::Edge,"ScanlineEdge")
EMPTY_SLOTTABLE(Scanline::Edge)
EMPTY_SERIALIZER(Scanline::Edge)

Scanline::Edge::Edge() : polygon(nullptr)
{
   STANDARD_CONSTRUCTOR()

   lv.set(0,0);
   uv.set(0,0);
   x = 0;
   slope = 0;
   lvn.set(0.0f,0.0f,1.0f);
   cn.set(0.0f,0.0f,1.0f);
   nslope.set(0.0f,0.0f,0.0f);
   valid = false;
   pointLock = false;
   polygon = nullptr;
}

Scanline::Edge::Edge(
               const LCreal v0[2],
               const osg::Vec3& vn0,
               const LCreal v1[2],
               const osg::Vec3& vn1,
               PolyData* const p
            ) : polygon(nullptr)
{
   osg::Vec3 uvn;
   if (v0[1] <= v1[1]) {
      lv.set(v0[0],v0[1]);
      uv.set(v1[0],v1[1]);
      lvn = vn0;
      uvn = vn1;
   }
   else {
      lv.set(v1[0],v1[1]);
      uv.set(v0[0],v0[1]);
      lvn = vn1;
      uvn = vn0;
   }

   x = lv[0];
   cn = lvn;

   if (uv[1] > lv[1]) {
      LCreal deltaY = (uv[1] - lv[1]);
      slope = (uv[0] - lv[0]) / deltaY ;
      nslope = (uvn - lvn) * (1.0f/deltaY);
      valid = true;
   }
   else if (uv[1] == lv[1]) {
      slope = 0.0f;
      nslope.set(0.0f,0.0f,0.0f);
      valid = true;
   }
   else {
      slope = 0.0f;
      nslope.set(0.0f,0.0f,0.0f);
      valid = false;
   }

   polygon = p;
   pointLock = false;
}

Scanline::Edge::Edge(
               const LCreal v0[2],
               const LCreal v1[2],
               PolyData* const p
            ) : polygon(nullptr)
{
   if (v0[1] <= v1[1]) {
      lv.set(v0[0],v0[1]);
      uv.set(v1[0],v1[1]);
   }
   else {
      lv.set(v1[0],v1[1]);
      uv.set(v0[0],v0[1]);
   }

   x = lv[0];

   if (uv[1] > lv[1]) {
      slope = (uv[0] - lv[0]) / (uv[1] - lv[1]) ;
      valid = true;
   }
   else {
      slope = 0;
      valid = false;
   }

   polygon = p;
   pointLock = false;
   lvn.set(0.0f,0.0f,1.0f);
   cn.set(0.0f,0.0f,1.0f);
   nslope.set(0.0f,0.0f,0.0f);
}

Scanline::Edge::Edge(const Scanline::Edge& org) : polygon(nullptr)
{
   STANDARD_CONSTRUCTOR()
   copyData(org,true);
}

Scanline::Edge::~Edge()
{
   STANDARD_DESTRUCTOR()
}

Scanline::Edge& Scanline::Edge::operator=(const Scanline::Edge& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

Scanline::Edge* Scanline::Edge::clone() const
{
   return new Scanline::Edge(*this);
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Scanline::Edge::copyData(const Scanline::Edge& org, const bool)
{
   BaseClass::copyData(org);

   lv = org.lv;
   uv = org.uv;
   x = org.x;
   slope = org.slope;
   lvn = org.lvn;
   cn = org.cn;
   nslope = org.nslope;
   valid = org.valid;
   pointLock = org.pointLock;

   const PolyData* pp = org.polygon;
   polygon = const_cast<PolyData*>(static_cast<const PolyData*>(pp));
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Scanline::Edge::deleteData()
{
   polygon = nullptr;
}

//------------------------------------------------------------------------------
// v() -- increment the edge's start data.
//------------------------------------------------------------------------------
void Scanline::Edge::incEdgeStart()
{
    if (pointLock) return;

    lv[0] += slope;
    x = lv[0];
    lv[1] += 1.0f;
    if (lv[1] > uv[1]) valid = false;

    int ixmin = static_cast<int>( lv[0] + 0.5f );
    int iymin = static_cast<int>( lv[1] + 0.5f );
    int ixmax = static_cast<int>( uv[0] + 0.5f );
    int iymax = static_cast<int>( uv[1] + 0.5f );
    if (ixmax == ixmin && iymax == iymin) pointLock = true;

    lvn = lvn + nslope;

    cn = lvn;
}


} // End BasicGL namespace
} // End Eaagles namespace

