// Polygon

#include "openeaagles/graphics/Polygon.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/graphics/ColorGradient.h"
#include "openeaagles/basic/PairStream.h"
#include <GL/glu.h>

namespace oe {
namespace BasicGL {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Polygon,"Polygon")
EMPTY_SERIALIZER(Polygon)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Polygon::Polygon()
{
   STANDARD_CONSTRUCTOR()
   coeff.set(0,0,0,0);
   coeffValid = false;
   norm.set(1,0,0);
   layerValue = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Polygon::copyData(const Polygon& org, const bool)
{
   BaseClass::copyData(org);

   coeff = org.coeff;
   coeffValid = org.coeffValid;
   norm = org.norm;
   layerValue = org.layerValue;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Polygon)

//------------------------------------------------------------------------------
// compute the vector normal to polygon
//------------------------------------------------------------------------------
bool Polygon::calcNormal()
{
   bool ok = false;
   if (getNumberOfVertices() >= 3) {
      ok = calcNormal(norm,getVertices());
   }
   else {
      norm.set(1.0f,0.0f,0.0f);
   }

   return ok;
}

bool Polygon::calcNormal(osg::Vec3& n, const osg::Vec3 v[3])
{
   osg::Vec3 t1 = v[1] - v[0];
   t1.normalize();

   osg::Vec3 t2 = v[2] - v[1];
   t2.normalize();

   n = (t1 ^ t2); // cross product

   return true;
}

//------------------------------------------------------------------------------
// find the coefficients of the plane equation
//------------------------------------------------------------------------------
bool Polygon::calcPlaneCoeff(osg::Vec4& cc, const osg::Vec3 v[3])
{
   cc[A] =  ( v[0].y() * (v[1].z() - v[2].z())
            - v[1].y() * (v[0].z() - v[2].z())
            + v[2].y() * (v[0].z() - v[1].z()) );

   cc[B] = -( v[0].x() * (v[1].z() - v[2].z())
            - v[1].x() * (v[0].z() - v[2].z())
            + v[2].x() * (v[0].z() - v[1].z()) );

   cc[C] =  ( v[0].x() * (v[1].y() - v[2].y())
            - v[1].x() * (v[0].y() - v[2].y())
            + v[2].x() * (v[0].y() - v[1].y()) );

   cc[D] = -( v[0].x() * (v[1].y()*v[2].z() - v[2].y()*v[1].z())
            - v[1].x() * (v[0].y()*v[2].z() - v[2].y()*v[0].z())
            + v[2].x() * (v[0].y()*v[1].z() - v[1].y()*v[0].z()) );

   return true;
}

bool Polygon::calcPlaneCoeff()
{
   if (getNumberOfVertices() >= 3) {
      coeffValid = calcPlaneCoeff(coeff,getVertices());
   }
   else {
      coeffValid = false;
      coeff.set(0,0,0,0);
   }

   return coeffValid;
}

//------------------------------------------------------------------------------
// compute the z value at point p using plane coefficients c.
//------------------------------------------------------------------------------
LCreal Polygon::calcZ(const osg::Vec2& p, const osg::Vec4& cc)
{
   LCreal zz = 0.0;
   zz = (-cc._v[D] - (cc._v[A] * p.x()) - (cc._v[B] * p.y()) )/cc._v[C];
   return zz;
}

LCreal Polygon::calcZ(const osg::Vec2& p) const
{
   return calcZ(p,*getPlaneCoeff());
}

//------------------------------------------------------------------------------
// drawFunc()
//------------------------------------------------------------------------------
void Polygon::drawFunc()
{
    BEGIN_DLIST
    unsigned int nv = getNumberOfVertices();
    const osg::Vec3* vertices = getVertices();

    if (nv >= 2) {

        // Draw with texture
        unsigned int ntc = getNumberOfTextureCoords();
        if (ntc > 0 && hasTexture()) {
            const osg::Vec2* texCoord = getTextureCoord();
            unsigned int tc = 0; // texture count
            glBegin(GL_POLYGON);
            for (unsigned int i = 0; i < nv; i++) {
                if (tc < ntc)  {
                    lcTexCoord2v(texCoord[tc++].ptr());
                }
                lcVertex3v( vertices[i].ptr() );
            }
            glEnd();

        }

        // Draw without texture
        else {
            // get our color gradient, because if we have one, instead of a regular color, we will
            // override it here and set it on a per vertex level.
            ColorGradient* colGradient = dynamic_cast<ColorGradient*>(getColor());
            glBegin(GL_POLYGON);
            osg::Vec3* ptr = nullptr;
            for (unsigned int i = 0; i < nv; i++) {
                if (colGradient != nullptr) {
                    basic::Color* col = colGradient->getColorByIdx(i+1);
                    if (col != nullptr)
                       glColor4f(static_cast<GLfloat>(col->red()), static_cast<GLfloat>(col->green()),
                                 static_cast<GLfloat>(col->blue()), static_cast<GLfloat>(col->alpha()));
                }
                // if we have a material name, we will set up like we have a material
                if (getMaterialName() != nullptr) {
                    //lcVertex3v( vertices[i].ptr() );
                    ptr = const_cast<osg::Vec3*>(reinterpret_cast<const osg::Vec3*>(vertices[i].ptr()));

                    if (ptr != nullptr) {
                        calcNormal();
                        lcNormal3(norm.x(), norm.y(), -(norm.z()));
                        lcVertex3(ptr->x(), ptr->y(), ptr->z());
                    }
                }
                else {
                    lcVertex3v(vertices[i].ptr());
                }
            }
            glEnd();
        }
    }
    END_DLIST
}

} // End BasicGL namespace
} // End oe namespace

