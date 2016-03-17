
#include "openeaagles/instruments/eadi3D/IrisGLCompat.h"
#include "openeaagles/base/units/Angles.h"

#include "openeaagles/base/util/platform_api.h"
#include <GL/gl.h>

#include <cmath>

#define MAX_POINTS 64

namespace oe {
namespace instruments {

IMPLEMENT_ABSTRACT_SUBCLASS(IrisGLCompat, "IrisGLCompat")
EMPTY_SLOTTABLE(IrisGLCompat)
EMPTY_SERIALIZER(IrisGLCompat)

IrisGLCompat::IrisGLCompat() : vertexX(0.0), vertexY(0.0)
{
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IrisGLCompat::copyData(const IrisGLCompat& org, const bool)
{
    // Copy our baseclass stuff first
    BaseClass::copyData(org);

    vertexX = org.vertexX;
    vertexY = org.vertexY;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(IrisGLCompat)

//------------------------------------------------------------------------------
// arc() -
//------------------------------------------------------------------------------
void IrisGLCompat::arc(float x, float y, float r, float startAng, float endAng)
{
    float ang = static_cast<float>(startAng * base::Angle::D2RCC);
    float fraction = static_cast<float>((endAng - startAng) / 360.0);
    int numPoints = static_cast<int>((static_cast<float>(MAX_POINTS)) * fraction);
    float delta = static_cast<float>((2.0 * base::PI * fraction) / static_cast<float>(numPoints));
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i < numPoints+1; i++) {
            float vy = static_cast<float>(std::sin(ang) * r + y);
            float vx = static_cast<float>(std::cos(ang) * r + x);
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

//------------------------------------------------------------------------------
// arcf() -
//------------------------------------------------------------------------------
void IrisGLCompat::arcf(float x, float y, float r, float startAng, float endAng)
{
    float ang = static_cast<float>(startAng * base::Angle::D2RCC);
    float fraction = static_cast<float>((endAng - startAng) / 360.0);
    int numPoints = static_cast<int>((static_cast<float>(MAX_POINTS)) * fraction);
    float delta = static_cast<float>((2.0 * base::PI * fraction) / static_cast<float>(numPoints));
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i < numPoints+1; i++) {
            float vy = std::sin(ang) * r + y;
            float vx = std::cos(ang) * r + x;
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

//------------------------------------------------------------------------------
// circ() -
//------------------------------------------------------------------------------
void IrisGLCompat::circ(float x, float y, float r)
{
    float ang = 0.0;
    float delta = static_cast<float>((2.0 * base::PI) / static_cast<float>(MAX_POINTS));
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < MAX_POINTS; i++) {
            float vy = std::sin(ang) * r + y;
            float vx = std::cos(ang) * r + x;
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

//------------------------------------------------------------------------------
// circf() -
//------------------------------------------------------------------------------
void IrisGLCompat::circf(float x, float y, float r)
{
    float ang = 0.0;
    float delta = static_cast<float>((2.0 * base::PI) / static_cast<float>(MAX_POINTS));
    glBegin(GL_POLYGON);
        for (int i = 0; i < MAX_POINTS; i++) {
            float vy = std::sin(ang) * r + y;
            float vx = std::cos(ang) * r + x;
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

//------------------------------------------------------------------------------
// washerf() -
//------------------------------------------------------------------------------
void IrisGLCompat::washerf(float x, float y, float r1, float r2)
{
    float ang = 0.0;
    float delta = static_cast<float>((2.0 * base::PI) / static_cast<float>(MAX_POINTS));
    glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < MAX_POINTS+1; i++) {
            float s= std::sin(ang);
            float c = std::cos(ang);
            float vy1 = s * r1 + y;
            float vx1 = c * r1 + x;
            float vy2 = s * r2 + y;
            float vx2 = c * r2 + x;
            glVertex2f(vx1, vy1);
            glVertex2f(vx2, vy2);
            ang += delta;
        }
    glEnd();
}

//------------------------------------------------------------------------------
// rect() -
//------------------------------------------------------------------------------
void IrisGLCompat::rect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

//------------------------------------------------------------------------------
// rpmv2() -
//------------------------------------------------------------------------------
void IrisGLCompat::rpmv2(float x, float y)
{
    vertexX = x;
    vertexY = y;
    glVertex2f(vertexX, vertexY);
}

//------------------------------------------------------------------------------
// rpdr2() -
//------------------------------------------------------------------------------
void IrisGLCompat::rpdr2(float x, float y)
{
    vertexX += x;
    vertexY += y;
    glVertex2f(vertexX, vertexY);
}

//------------------------------------------------------------------------------
// rmv2() -
//------------------------------------------------------------------------------
void IrisGLCompat::rmv2(float x, float y)
{
    vertexX = x;
    vertexY = y;
    glVertex2f(vertexX, vertexY);
}

//------------------------------------------------------------------------------
// rdr2() -
//------------------------------------------------------------------------------
void IrisGLCompat::rdr2(float x, float y)
{
    vertexX += x;
    vertexY += y;
    glVertex2f(vertexX, vertexY);
}

} // End Instruments namespace
} // End oe namespace

