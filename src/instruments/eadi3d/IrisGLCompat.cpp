
#include "openeaagles/instruments/eadi3d/IrisGLCompat.hpp"

#include "openeaagles/base/util/constants.hpp"
#include "openeaagles/base/util/platform_api.hpp"
#include "openeaagles/base/util/unit_utils.hpp"

#include <GL/gl.h>
#include <cmath>

#define MAX_POINTS 64

namespace oe {
namespace instruments {

IMPLEMENT_ABSTRACT_SUBCLASS(IrisGLCompat, "IrisGLCompat")
EMPTY_SLOTTABLE(IrisGLCompat)
EMPTY_SERIALIZER(IrisGLCompat)
EMPTY_DELETEDATA(IrisGLCompat)

IrisGLCompat::IrisGLCompat()
{
    STANDARD_CONSTRUCTOR()
}

void IrisGLCompat::copyData(const IrisGLCompat& org, const bool)
{
    BaseClass::copyData(org);

    vertexX = org.vertexX;
    vertexY = org.vertexY;
}

void IrisGLCompat::arc(float x, float y, float r, float startAng, float endAng)
{
    auto ang = static_cast<float>(startAng * base::angle::D2RCC);
    const auto fraction = static_cast<float>((endAng - startAng) / 360.0);
    const auto numPoints = static_cast<int>((static_cast<float>(MAX_POINTS)) * fraction);
    const auto delta = static_cast<float>((2.0 * base::PI * fraction) / static_cast<float>(numPoints));
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i < numPoints+1; i++) {
            const auto vy = static_cast<float>(std::sin(ang) * r + y);
            const auto vx = static_cast<float>(std::cos(ang) * r + x);
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

void IrisGLCompat::arcf(float x, float y, float r, float startAng, float endAng)
{
    auto ang = static_cast<float>(startAng * base::angle::D2RCC);
    const auto fraction = static_cast<float>((endAng - startAng) / 360.0);
    const auto numPoints = static_cast<int>((static_cast<float>(MAX_POINTS)) * fraction);
    const auto delta = static_cast<float>((2.0 * base::PI * fraction) / static_cast<float>(numPoints));
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

void IrisGLCompat::circ(float x, float y, float r)
{
    float ang = 0.0;
    const auto delta = static_cast<float>((2.0 * base::PI) / static_cast<float>(MAX_POINTS));
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < MAX_POINTS; i++) {
            float vy = std::sin(ang) * r + y;
            float vx = std::cos(ang) * r + x;
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

void IrisGLCompat::circf(float x, float y, float r)
{
    float ang = 0.0;
    const auto delta = static_cast<float>((2.0 * base::PI) / static_cast<float>(MAX_POINTS));
    glBegin(GL_POLYGON);
        for (int i = 0; i < MAX_POINTS; i++) {
            float vy = std::sin(ang) * r + y;
            float vx = std::cos(ang) * r + x;
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

void IrisGLCompat::washerf(float x, float y, float r1, float r2)
{
    float ang = 0.0;
    const auto delta = static_cast<float>((2.0 * base::PI) / static_cast<float>(MAX_POINTS));
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

void IrisGLCompat::rect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void IrisGLCompat::rpmv2(float x, float y)
{
    vertexX = x;
    vertexY = y;
    glVertex2f(vertexX, vertexY);
}

void IrisGLCompat::rpdr2(float x, float y)
{
    vertexX += x;
    vertexY += y;
    glVertex2f(vertexX, vertexY);
}

void IrisGLCompat::rmv2(float x, float y)
{
    vertexX = x;
    vertexY = y;
    glVertex2f(vertexX, vertexY);
}

void IrisGLCompat::rdr2(float x, float y)
{
    vertexX += x;
    vertexY += y;
    glVertex2f(vertexX, vertexY);
}

}
}
