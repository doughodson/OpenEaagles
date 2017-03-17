

#include "openeaagles/instruments/eadi3d/Eadi3DObjects.hpp"

#include "openeaagles/base/util/platform_api.hpp"
#include "openeaagles/base/units/util/angle.hpp"

#include <GL/gl.h>
#include <cmath>

#define MR2IN 0.026785714      // font was scaled by this

namespace oe {
namespace instruments {

float WHITE[3] = { 1.0, 1.0, 1.0 };
float BLACK[3] = { 0.0, 0.0, 0.0 };
float GREEN[3] = { 0.0, 1.0, 0.0 };

IMPLEMENT_ABSTRACT_SUBCLASS(Eadi3DObjects, "Eadi3DObjects")
EMPTY_SLOTTABLE(Eadi3DObjects)
EMPTY_SERIALIZER(Eadi3DObjects)

Eadi3DObjects::Eadi3DObjects(const float radius) : radius(radius)
{
}

void Eadi3DObjects::copyData(const Eadi3DObjects& org, const bool)
{
    BaseClass::copyData(org);

    base = org.base;
    radius = org.radius;
    made = org.made;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Eadi3DObjects)

//------------------------------------------------------------------------------
// drawObj() - draw the given objective
//------------------------------------------------------------------------------
void Eadi3DObjects::drawObj(EadiObj obj)
{
    if (made) glCallList(base+obj);
}

//------------------------------------------------------------------------------
// makeObjects() - make our drawing objects
//------------------------------------------------------------------------------
void Eadi3DObjects::makeObjects()
{
    if (made) return;
    base = glGenLists(__MAX_EADI_OBJECTS__);
    makeFont();
    makeGlobe();

    glNewList(base+UP_ARROW, GL_COMPILE);
        glBegin(GL_POLYGON);
            irisgl.rpmv2(0., 0.);
            irisgl.rpdr2(-0.125, -0.1875);
            irisgl.rpdr2(0.25, 0.);
        glEnd();
        glBegin(GL_POLYGON);
            irisgl.rpmv2(-0.05f, -0.1875f);
            irisgl.rpdr2(0.0f, -0.1875f);
            irisgl.rpdr2(0.10f, 0.0f);
            irisgl.rpdr2(0.0f, 0.1875f);
        glEnd();
        glEndList();

        glNewList(base+DOWN_ARROW, GL_COMPILE);
        glBegin(GL_POLYGON);
            irisgl.rpmv2(0., 0.);
            irisgl.rpdr2(-0.125, 0.1875);
            irisgl.rpdr2(0.25, 0.);
        glEnd();
        glBegin(GL_POLYGON);
            irisgl.rpmv2(-0.05f, 0.1875f);
            irisgl.rpdr2(0.0f, 0.1875f);
            irisgl.rpdr2(0.10f, 0.0f);
            irisgl.rpdr2(0.0f, -0.1875f);
        glEnd();
    glEndList();

    glNewList(base+UP_ARROW_SKINNY, GL_COMPILE);
        glBegin(GL_POLYGON);
            irisgl.rpmv2(0.0f, 0.0f);
            irisgl.rpdr2(-0.125f, -0.1875f);
            irisgl.rpdr2(0.25f, 0.0f);
        glEnd();
        glBegin(GL_POLYGON);
            irisgl.rpmv2(-0.025f, -0.1875f);
            irisgl.rpdr2(0.0f, -0.1875f);
            irisgl.rpdr2(0.05f, 0.0f);
            irisgl.rpdr2(0.0f, 0.1875f);
        glEnd();
    glEndList();

    glNewList(base+DOWN_ARROW_SKINNY, GL_COMPILE);
        glBegin(GL_POLYGON);
            irisgl.rpmv2(0.0f, 0.f);
            irisgl.rpdr2(-0.125f, 0.1875f);
            irisgl.rpdr2(0.25f, 0.0f);
        glEnd();
        glBegin(GL_POLYGON);
            irisgl.rpmv2(-0.025f, 0.1875f);
            irisgl.rpdr2(0.0f, 0.1875f);
            irisgl.rpdr2(0.05f, 0.0f);
            irisgl.rpdr2(0.0f, -0.1875f);
        glEnd();
    glEndList();

    glNewList(base+UP_ARROW_OUTLINE, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            irisgl.rmv2(0.0f, 0.0f);
            irisgl.rdr2(-0.125f, -0.1875f);
            irisgl.rdr2(0.075f, 0.0f);
            irisgl.rdr2(0.0f, -0.1875f);
            irisgl.rdr2(0.10f, 0.0f);
            irisgl.rdr2(0.0f, 0.1875f);
            irisgl.rdr2(0.075f, 0.0f);
            irisgl.rdr2(-0.125f, 0.1875f);
        glEnd();
        glEndList();

    glNewList(base+DOWN_ARROW_OUTLINE, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            irisgl.rmv2(0.0f, 0.0f);
            irisgl.rdr2(-0.125f, 0.1875f);
            irisgl.rdr2(0.075f, 0.0f);
            irisgl.rdr2(0.0f, 0.1875f);
            irisgl.rdr2(0.10f, 0.0f);
            irisgl.rdr2(0.0f, -0.1875f);
            irisgl.rdr2(0.075f, 0.0f);
            irisgl.rdr2(-0.125f, -0.1875f);
        glEnd();
    glEndList();

    glNewList(base+GROUND_POINTER, GL_COMPILE);
        glColor3fv(BLACK);
        glBegin(GL_POLYGON);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.073f, 0.125f);
            glVertex2f(-0.073f, 0.125f);
            glVertex2f(0.0f, 0.0f);
        glEnd();
        glColor3fv(WHITE);
        glBegin(GL_LINE_STRIP);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.073f, 0.125f);
            glVertex2f(-0.073f, 0.125f);
            glVertex2f(0.0f, 0.0f);
        glEnd();
    glEndList();

    glNewList(base+GROUND_POINTER_2, GL_COMPILE);
        glColor3fv(BLACK);
        glBegin(GL_POLYGON);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0876f, 0.15f);
            glVertex2f(-0.0876f, 0.15f);
            glVertex2f(0.0f, 0.0f);
        glEnd();
        glColor3fv(WHITE);
        glBegin(GL_LINE_STRIP);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0876f, 0.15f);
            glVertex2f(-0.0876f, 0.15f);
            glVertex2f(0.0f, 0.0f);
        glEnd();
    glEndList();

    glNewList(base+BALL_FRAME, GL_COMPILE);
        glBegin(GL_LINES);
            glVertex2f(1.625f, 0.0f);        /* 90 deg */
            glVertex2f(1.9375f, 0.0f);

            glVertex2f(1.4073f, -0.8125f);   /* 60 deg */
            glVertex2f(1.5697f, -0.9063f);

            glVertex2f(1.1490f, -1.1490f);   /* 45 deg */
            glVertex2f(1.3700f, -1.3700f);

            glVertex2f(0.8125f, -1.4073f);   /* 30 deg */
            glVertex2f(0.9063f, -1.5697f);

            glVertex2f(0.5558f, -1.5270f);   /* 20 deg */
            glVertex2f(0.5985f, -1.6445f);

            glVertex2f(0.2823f, -1.6003f);   /* 10 deg */
            glVertex2f(0.3039f, -1.7234f);

            glVertex2f(0.0f, -1.625f);       /* 0 deg */
            glVertex2f(0.0f, -1.75f);

            glVertex2f(-0.2823f, -1.6003f);  /* -10 deg */
            glVertex2f(-0.3039f, -1.7234f);

            glVertex2f(-0.5558f, -1.5270f);  /* -20 deg */
            glVertex2f(-0.5985f, -1.6445f);

            glVertex2f(-0.8125f, -1.4073f);  /* -30 deg */
            glVertex2f(-0.9063f, -1.5697f);

            glVertex2f(-1.1490f, -1.1490f);  /* -45 deg */
            glVertex2f(-1.3700f, -1.3700f);

            glVertex2f(-1.4073f, -0.8125f);  /* -60 deg */
            glVertex2f(-1.5697f, -0.9063f);

            glVertex2f(-1.625f, 0.0f);       /* -90 deg */
            glVertex2f(-1.9375f, 0.0f);
        glEnd();
    glEndList();

    glNewList(base+AC_REF, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2f(0.40625f, 0.0f);      /* aircraft reference symbol */
            glVertex2f(0.25f, 0.0f);
            glVertex2f(0.125f, -0.21875f);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(-0.125f, -0.21875f);
            glVertex2f(-0.25f, 0.0f);
            glVertex2f(-0.40625f, 0.0f);
        glEnd();
    glEndList();

    glNewList(base+AC_REF2, GL_COMPILE);
        glLineWidth(6.0);
        glColor3fv(GREEN);
        glBegin(GL_LINE_STRIP);
            glVertex2f(0.40625f, 0.0f);     /* aircraft reference symbol */
            glVertex2f(0.25f, 0.0f);
            glVertex2f(0.125f, -0.21875f);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(-0.125f, -0.21875f);
            glVertex2f(-0.25f, 0.0f);
            glVertex2f(-0.40625f, 0.0f);
        glEnd();
        glLineWidth(2.0);
        glColor3fv(BLACK);
        glBegin(GL_LINE_STRIP);
            glVertex2f(0.40625, 0.);        /* aircraft reference symbol */
            glVertex2f(0.25, 0.);
            glVertex2f(0.125, -0.21875);
            glVertex2f(0., 0.);
            glVertex2f(-0.125, -0.21875);
            glVertex2f(-0.25, 0.);
            glVertex2f(-0.40625, 0.);
        glEnd();
    glEndList();

    glNewList(base+BORDER, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2f(-2.5, -2.625);
            glVertex2f(-2.5, 2.625);
            glVertex2f(2.5, 2.625);
            glVertex2f(2.5, -2.625);
            glVertex2f(-2.5, -2.625);
        glEnd();
    glEndList();

    glNewList(base+HEADING_SCALE, GL_COMPILE);
        glBegin(GL_LINES);
            glVertex2f(-1.6f, 2.1875f);    /* real start */
            glVertex2f(-1.6f, 2.25f);
            glVertex2f(-1.4f, 2.1875f);
            glVertex2f(-1.4f, 2.25f);
            glVertex2f(-1.2f, 2.1875f);
            glVertex2f(-1.2f, 2.25f);
            glVertex2f(-1.0f, 2.1875f);
            glVertex2f(-1.0f, 2.25f);
            glVertex2f(-0.8f, 2.1875f);
            glVertex2f(-0.8f, 2.25f);
            glVertex2f(-0.6f, 2.1875f);
            glVertex2f(-0.6f, 2.25f);
            glVertex2f(-0.4f, 2.1875f);
            glVertex2f(-0.4f, 2.25f);
            glVertex2f(-0.2f, 2.1875f);
            glVertex2f(-0.2f, 2.25f);
            glVertex2f(0.0f, 2.1875f);
            glVertex2f(0.0f, 2.25f);
            glVertex2f(0.2f, 2.1875f);
            glVertex2f(0.2f, 2.25f);
            glVertex2f(0.4f, 2.1875f);
            glVertex2f(0.4f, 2.25f);
            glVertex2f(0.6f, 2.1875f);
            glVertex2f(0.6f, 2.25f);
            glVertex2f(0.8f, 2.1875f);
            glVertex2f(0.8f, 2.25f);
            glVertex2f(1.0f, 2.1875f);
            glVertex2f(1.0f, 2.25f);
            glVertex2f(1.2f, 2.1875f);
            glVertex2f(1.2f, 2.25f);
            glVertex2f(1.4f, 2.1875f);
            glVertex2f(1.4f, 2.25f);
            glVertex2f(1.6f, 2.1875f);
            glVertex2f(1.6f, 2.25f);
            glVertex2f(1.8f, 2.1875f);    /* extra */
            glVertex2f(1.8f, 2.25f);
            glVertex2f(2.0f, 2.1875f);
            glVertex2f(2.0f, 2.25f);
            glVertex2f(2.2f, 2.1875f);
            glVertex2f(2.2f, 2.25f);
            glVertex2f(2.4f, 2.1875f);
            glVertex2f(2.4f, 2.25f);
            glVertex2f(2.6f, 2.1875f);
            glVertex2f(2.6f, 2.25f);
        glEnd();
    glEndList();

    glNewList(base+LEFT_CARET, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2f(-0.125f, -0.125f);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(-0.125f, 0.125f);
        glEnd();
    glEndList();

    glNewList(base+UP_CARET, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2f(-0.125f, -0.125f);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.125f, -0.125f);
        glEnd();
    glEndList();

    glNewList(base+UP_TRIANGLE, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2f(-0.125f, -0.145f);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.125f, -0.145f);
            glVertex2f(-0.125f, -0.145f);
        glEnd();
    glEndList();

    glNewList(base+TURN_RATE, GL_COMPILE);
        glBegin(GL_LINES);
            glVertex2f(-0.8125f, -2.125f);   /* turn rate */
            glVertex2f(0.8125f, -2.125f);
            glVertex2f(-0.8125f, -2.125f);
            glVertex2f(-0.8125f, -2.25f);
            glVertex2f(-0.40625f, -2.125f);
            glVertex2f(-0.40625f, -2.25f);
            glVertex2f(0.0f, -2.125f);
            glVertex2f(0.0f, -2.25f);
            glVertex2f(0.40625f, -2.125f);
            glVertex2f(0.40625f, -2.25f);
            glVertex2f(0.8125f, -2.125f);
            glVertex2f(0.8125f, -2.25f);
        glEnd();
    glEndList();

    glNewList(base+SLIP_IND, GL_COMPILE);
        irisgl.rect(-0.40625f, -2.5f, 0.40625f, -2.375f);    /* slip ind */
        glBegin(GL_LINES);
            glVertex2f(-0.203125f, -2.375f);
            glVertex2f(-0.203125f, -2.5f);
            glVertex2f(0.0f, -2.375f);
            glVertex2f(0.0f, -2.5f);
            glVertex2f(0.203125f, -2.375f);
            glVertex2f(0.203125f, -2.5f);
        glEnd();
    glEndList();

    glNewList(base+SLIP_BALL, GL_COMPILE);
        glBegin(GL_POLYGON);
            glVertex2f(0.0f, 0.0625f);
            glVertex2f(0.0625f, 0.0f);
            glVertex2f(0.0f, -0.0625f);
            glVertex2f(-0.0625f, 0.0f);
            glVertex2f(0.0f, 0.0625f);
        glEnd();
    glEndList();

    glNewList(base+GLIDESLOPE, GL_COMPILE);
        glPushMatrix();
            glTranslatef(-2.0f, 1.0f, 0.0f);             /* glideslope */
            irisgl.rect(-0.02f, -0.02f, 0.02f, 0.02f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-2.0f, 0.5f, 0.0f);
            irisgl.rect(-0.02f, -0.02f, 0.02f, 0.02f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-2.0f, 0.0f, 0.0f);
            irisgl.rect(-0.02f, -0.02f, 0.02f, 0.02f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-2.0f, -0.5f, 0.0f);
            irisgl.rect(-0.02f, -0.02f, 0.02f, 0.02f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-2.0f, -1.0f, 0.0f);
            irisgl.rect(-0.02f, -0.02f, 0.02f, 0.02f);
        glPopMatrix();
    glEndList();

    glNewList(base+LOCALIZER, GL_COMPILE);
        glPushMatrix();
            glTranslatef(-1.0f, 2.0f, 0.0f);              /* localizer */
            irisgl.rect(-0.02f, -0.02f, 0.02f, 0.02f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-0.5f, 2.0f, 0.0f);
            irisgl.rect(-0.02f, -0.02f, 0.02f, 0.02f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, 2.0f, 0.0f);
            irisgl.rect(-0.02f, -0.02f, 0.02f, 0.02f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.5f, 2.0f, 0.0f);
            irisgl.rect(-0.02f, -0.02f, 0.02f, 0.02f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.0f, 2.0f, 0.0f);
            irisgl.rect(-0.02f, -0.02f, 0.02f, 0.02f);
        glPopMatrix();
    glEndList();

    glNewList(base+ALPHA_SYMB, GL_COMPILE);             /* greek alpha */
        glPushMatrix();
            glScalef(static_cast<float>(MR2IN), static_cast<float>(MR2IN), 1.0f);
            glBegin(GL_LINE_STRIP);
                glVertex2f(4.0f, 5.0f);
                glVertex2f(1.0f, 2.0f);
                glVertex2f(0.0f, 3.0f);
                glVertex2f(0.0f, 4.0f);
                glVertex2f(1.0f, 5.0f);
                glVertex2f(4.0f, 2.0f);
            glEnd();
        glPopMatrix();
    glEndList();

    glNewList(base+GS_TEXT1, GL_COMPILE);
        glCallList(base+LEFT_CARET);
        glTranslatef(-0.25f, -0.09375f, 0.0f);
        glCallList(base+FONT_S);
        glTranslatef(-0.22f, 0.0f, 0.0f);
        glCallList(base+FONT_G);
        glTranslatef(0.11f, 0.0f, 0.0f);
        glCallList(base+PUNC07);
    glEndList();

    glNewList(base+GS_TEXT2, GL_COMPILE);
        glPushMatrix();
            glTranslatef(-2.25f, -0.09375f, 0.0f);
            glCallList(base+FONT_F);
            glTranslatef(0.0f, 0.25f, 0.0f);
            glCallList(base+FONT_O);
            glTranslatef(0.0f, -0.5f, 0.0f);
            glCallList(base+FONT_F);
        glPopMatrix();
    glEndList();

    glNewList(base+LOC_TEXT1, GL_COMPILE);
        glCallList(base+UP_CARET);
        glTranslatef(-0.05357f, -0.325f, 0.0f);
        glCallList(base+FONT_O);
        glTranslatef(-0.16071f, 0.0f, 0.0f);
        glCallList(base+FONT_L);
        glTranslatef(0.32142f, 0.0f, 0.0f);
        glCallList(base+FONT_C);
    glEndList();

    glNewList(base+LOC_TEXT2, GL_COMPILE);
        glPushMatrix();
            glTranslatef(-0.05357f, 1.725f, 0.0f);
            glCallList(base+FONT_F);
            glTranslatef(-0.16071f, 0.0f, 0.0f);
            glCallList(base+FONT_O);
            glTranslatef(0.32142f, 0.0f, 0.0f);
            glCallList(base+FONT_F);
        glPopMatrix();
    glEndList();

    made = true;
}

//------------------------------------------------------------------------------
// makeGlobe data -
//------------------------------------------------------------------------------
float globeArrowV1[3] = {0.0, 0.0, 0.0};
float globeArrowV2[3] = {0.0, 0.0, 0.0};
float globeArrowV3[3] = {0.0, 0.0, 0.0};
float globeArrowV4[3] = {0.0, 0.0, 0.0};
float globeArrowV5[3] = {0.0, 0.0, 0.0};
float globeArrowV6[3] = {0.0, 0.0, 0.0};


//------------------------------------------------------------------------------
// drawArrow() - draw our arrow objects at the given angle and position
//------------------------------------------------------------------------------
void Eadi3DObjects::drawArrow(float bAngle, float s, float c)
{
    glPushMatrix();
        glTranslatef(c * radius, s * radius, 0.0);
        glRotatef(bAngle, 0.0, 0.0, 1.0);
        glBegin(GL_POLYGON);
            glVertex3fv(globeArrowV1);
            glVertex3fv(globeArrowV2);
            glVertex3fv(globeArrowV3);
        glEnd();
        glLineWidth(2.0);
        glBegin(GL_LINES);
            glVertex3fv(globeArrowV1);
            glVertex3fv(globeArrowV4);
        glEnd();
        glLineWidth(1.0);
    glPopMatrix();
}

//------------------------------------------------------------------------------
// drawArrow2() - second arrow
//------------------------------------------------------------------------------
void Eadi3DObjects::drawArrow2(float bAngle, float s, float c)
{
    glPushMatrix();
        glTranslatef(c * radius, s * radius, 0.0);
        glRotatef(bAngle, 0.0, 0.0, 1.0);
        glBegin(GL_POLYGON);
            glVertex3fv(globeArrowV1);
            glVertex3fv(globeArrowV5);
            glVertex3fv(globeArrowV6);
        glEnd();
    glPopMatrix();
}

//------------------------------------------------------------------------------
// makeGlobe() - make our 3d globe
//------------------------------------------------------------------------------
void Eadi3DObjects::makeGlobe()
{
    float h = static_cast<float>(std::sin(5.0 * base::angle::D2RCC) * radius);
    globeArrowV1[1] = h + h;
    globeArrowV2[2] = -h;
    globeArrowV3[2] = h;
    globeArrowV4[1] = -h;
    globeArrowV5[2] = static_cast<float>(-h / 2.0);
    globeArrowV6[2] = static_cast<float>(h / 2.0);

    glNewList(base+GLOBE, GL_COMPILE);
    glPushMatrix();
        for (int i = 0; i < 6; i++) {
            glColor3fv(WHITE);
            glRotatef(30.0, 0.0, 1.0, 0.0);
            irisgl.arc(0.0, 0.0, radius, 0.0, 180.0);

            glColor3fv(WHITE);
            for (float a = -20.0; a >= -60.0; a -= 20.0) {
                float bAngle = a;
                float s = static_cast<float>(std::sin(a * base::angle::D2RCC));
                float c = static_cast<float>(std::cos(a * base::angle::D2RCC));
                drawArrow(bAngle, s, c);
                drawArrow(-bAngle, s, -c);
            }
            float bAngle = -80.0;
            float s = static_cast<float>(std::sin(-80.0 * base::angle::D2RCC));
            float c = static_cast<float>(std::cos(-80.0 * base::angle::D2RCC));
            drawArrow2(bAngle, s, c);
            drawArrow2(-bAngle, s, -c);
        }
        glPopMatrix();

        glColor3fv(WHITE);
        glPushMatrix();
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            float s = static_cast<float>(std::sin(-90.0 * base::angle::D2RCC));
            float c = static_cast<float>(std::cos(-85.0 * base::angle::D2RCC));
            glPushMatrix();
        glTranslatef(0.0f, 0.0f, -s * radius);
        irisgl.circf(0.0f, 0.0f, c * radius);
        glPopMatrix();
        glLineWidth(2.0);
        for (float a = -80.0; a <= 80.0; a += 10.0) {
            s = static_cast<float>(std::sin(a * base::angle::D2RCC));
            c = static_cast<float>(std::cos(a * base::angle::D2RCC));
            glPushMatrix();
            glTranslatef(0.0, 0.0, -s * radius);
            glColor3fv(WHITE);
            if (std::fabs(a) == 60.0 || std::fabs(a) == 30.0 || a == 0.0) {
                glLineWidth(4.0);
                irisgl.circ(0.0, 0.0, c * radius);
                if (a != 0.0) {
                    glColor3fv(BLACK);
                    /* glColor3fv(WHITE);     * temp */
                    glPushMatrix();
                    glRotatef(-90.0, 1.0, 0.0, 0.0);

                    for (int j = 0; j < 4; j++) {
                        glPushMatrix();
                        glTranslatef(-0.15f, static_cast<float>(-0.09375 * c), static_cast<float>(c * radius + 0.09375 * s));
                        glRotatef(-a, 1.0, 0.0, 0.0);
                        glCallList(base + ( static_cast<unsigned int>(std::fabs(a) / 10.)));
                        glTranslatef(0.1929f, 0.0f, 0.0f);
                        glCallList(base+FONT_0);
                        glPopMatrix();
                        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
                    }
                    glPopMatrix();
                }
                glLineWidth(2.0);
            }
            else {
                irisgl.circ(0.0, 0.0, c * radius);
            }
            glPopMatrix();
        }
        s = static_cast<float>(std::sin(90.0 * base::angle::D2RCC));
        c = static_cast<float>(std::cos(85.0 * base::angle::D2RCC));
        glPushMatrix();
            glTranslatef(0., 0., -s * radius);
            irisgl.circf(0.0, 0.0, c * radius);
        glPopMatrix();
    glPopMatrix();

    glEndList();
}

//------------------------------------------------------------------------------
// makeFTZRO() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTZRO(unsigned int obj)
{
    float vec1[2] = {0.013393f, 0.013393f};
    float vec2[2] = {0.000000f, 0.040179f};
    float vec3[2] = {0.000000f, 0.147321f};
    float vec4[2] = {0.013393f, 0.174107f};
    float vec5[2] = {0.040179f, 0.187500f};
    float vec6[2] = {0.066964f, 0.187500f};
    float vec7[2] = {0.093750f, 0.174107f};
    float vec8[2] = {0.107143f, 0.147321f};
    float vec9[2] = {0.107143f, 0.040179f};
    float vec10[2] = {0.093750f, 0.013393f};
    float vec11[2] = {0.066964f, 0.000000f};
    float vec12[2] = {0.040179f, 0.000000f};
    float vec13[2] = {0.013393f, 0.013393f};
    float vec14[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
            glVertex2fv(vec10);
            glVertex2fv(vec11);
            glVertex2fv(vec12);
            glVertex2fv(vec13);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec14);
        glEnd();
    glEndList();
}


//------------------------------------------------------------------------------
// makeFT1() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFT1(unsigned int obj)
{
    float vec1[2] = {0.026786f, 0.000000f};
    float vec2[2] = {0.080357f, 0.000000f};
    float vec3[2] = {0.053571f, 0.000000f};
    float vec4[2] = {0.053571f, 0.187500f};
    float vec5[2] = {0.026786f, 0.133929f};
    float vec6[2] = {0.080357f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec6);
        glEnd();
    glEndList();
}


//------------------------------------------------------------------------------
// makeFT2() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFT2(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.107143f, 0.000000f};
    float vec3[2] = {0.000000f, 0.000000f};
    float vec4[2] = {0.093750f, 0.107143f};
    float vec5[2] = {0.107143f, 0.133929f};
    float vec6[2] = {0.107143f, 0.147321f};
    float vec7[2] = {0.093750f, 0.174107f};
    float vec8[2] = {0.066964f, 0.187500f};
    float vec9[2] = {0.040179f, 0.187500f};
    float vec10[2] = {0.013393f, 0.174107f};
    float vec11[2] = {0.000000f, 0.147321f};
    float vec12[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
            glVertex2fv(vec10);
            glVertex2fv(vec11);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec12);
        glEnd();
    glEndList();
}


//------------------------------------------------------------------------------
// makeFT3() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFT3(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.040179f};
    float vec2[2] = {0.013393f, 0.013393f};
    float vec3[2] = {0.040179f, 0.000000f};
    float vec4[2] = {0.066964f, 0.000000f};
    float vec5[2] = {0.093750f, 0.013393f};
    float vec6[2] = {0.107143f, 0.040179f};
    float vec7[2] = {0.107143f, 0.066964f};
    float vec8[2] = {0.093750f, 0.093750f};
    float vec9[2] = {0.066964f, 0.107143f};
    float vec10[2] = {0.053571f, 0.107143f};
    float vec11[2] = {0.107143f, 0.187500f};
    float vec12[2] = {0.000000f, 0.187500f};
    float vec13[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
        glVertex2fv(vec1);
        glVertex2fv(vec2);
        glVertex2fv(vec3);
        glVertex2fv(vec4);
        glVertex2fv(vec5);
        glVertex2fv(vec6);
        glVertex2fv(vec7);
        glVertex2fv(vec8);
        glVertex2fv(vec9);
        glVertex2fv(vec10);
        glVertex2fv(vec11);
        glVertex2fv(vec12);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2fv(vec13);
    glEnd();
    glEndList();
}


//------------------------------------------------------------------------------
// makeFT4() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFT4(unsigned int obj)
{
    float vec1[2] = {0.093750f, 0.000000f};
    float vec2[2] = {0.093750f, 0.187500f};
    float vec3[2] = {0.066964f, 0.174107f};
    float vec4[2] = {0.000000f, 0.066964f};
    float vec5[2] = {0.107143f, 0.066964f};
    float vec6[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
        glVertex2fv(vec1);
        glVertex2fv(vec2);
        glVertex2fv(vec3);
        glVertex2fv(vec4);
        glVertex2fv(vec5);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2fv(vec6);
    glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFT5() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFT5(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.026786f};
    float vec2[2] = {0.013393f, 0.013393f};
    float vec3[2] = {0.040179f, 0.000000f};
    float vec4[2] = {0.066964f, 0.000000f};
    float vec5[2] = {0.093750f, 0.013393f};
    float vec6[2] = {0.107143f, 0.040179f};
    float vec7[2] = {0.107143f, 0.080357f};
    float vec8[2] = {0.080357f, 0.107143f};
    float vec9[2] = {0.026786f, 0.107143f};
    float vec10[2] = {0.000000f, 0.080357f};
    float vec11[2] = {0.000000f, 0.187500f};
    float vec12[2] = {0.107143f, 0.187500f};
    float vec13[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
        glVertex2fv(vec1);
        glVertex2fv(vec2);
        glVertex2fv(vec3);
        glVertex2fv(vec4);
        glVertex2fv(vec5);
        glVertex2fv(vec6);
        glVertex2fv(vec7);
        glVertex2fv(vec8);
        glVertex2fv(vec9);
        glVertex2fv(vec10);
        glVertex2fv(vec11);
        glVertex2fv(vec12);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2fv(vec13);
    glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFT6() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFT6(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.040179f};
    float vec2[2] = {0.013393f, 0.013393f};
    float vec3[2] = {0.040179f, 0.000000f};
    float vec4[2] = {0.066964f, 0.000000f};
    float vec5[2] = {0.093750f, 0.013393f};
    float vec6[2] = {0.107143f, 0.040179f};
    float vec7[2] = {0.107143f, 0.053571f};
    float vec8[2] = {0.093750f, 0.080357f};
    float vec9[2] = {0.066964f, 0.093750f};
    float vec10[2] = {0.040179f, 0.093750f};
    float vec11[2] = {0.013393f, 0.080357f};
    float vec12[2] = {0.000000f, 0.053571f};
    float vec13[2] = {0.000000f, 0.040179f};
    float vec14[2] = {0.000000f, 0.093750f};
    float vec15[2] = {0.026786f, 0.160714f};
    float vec16[2] = {0.066964f, 0.187500f};
    float vec17[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
        glVertex2fv(vec1);
        glVertex2fv(vec2);
        glVertex2fv(vec3);
        glVertex2fv(vec4);
        glVertex2fv(vec5);
        glVertex2fv(vec6);
        glVertex2fv(vec7);
        glVertex2fv(vec8);
        glVertex2fv(vec9);
        glVertex2fv(vec10);
        glVertex2fv(vec11);
        glVertex2fv(vec12);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2fv(vec13);
        glVertex2fv(vec14);
        glVertex2fv(vec15);
        glVertex2fv(vec16);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2fv(vec17);
    glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFT7() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFT7(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.107143f, 0.187500f};
    float vec3[2] = {0.000000f, 0.187500f};
    float vec4[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec4);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFT8() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFT8(unsigned int obj)
{
    float vec1[2] = {0.013393f, 0.013393f};
    float vec2[2] = {0.040179f, 0.000000f};
    float vec3[2] = {0.066964f, 0.000000f};
    float vec4[2] = {0.093750f, 0.013393f};
    float vec5[2] = {0.107143f, 0.040179f};
    float vec6[2] = {0.107143f, 0.053571f};
    float vec7[2] = {0.093750f, 0.080357f};
    float vec8[2] = {0.066964f, 0.093750f};
    float vec9[2] = {0.040179f, 0.093750f};
    float vec10[2] = {0.013393f, 0.107143f};
    float vec11[2] = {0.000000f, 0.133929f};
    float vec12[2] = {0.000000f, 0.147321f};
    float vec13[2] = {0.013393f, 0.174107f};
    float vec14[2] = {0.040179f, 0.187500f};
    float vec15[2] = {0.066964f, 0.187500f};
    float vec16[2] = {0.093750f, 0.174107f};
    float vec17[2] = {0.107143f, 0.147321f};
    float vec18[2] = {0.107143f, 0.133929f};
    float vec19[2] = {0.093750f, 0.107143f};
    float vec20[2] = {0.066964f, 0.093750f};
    float vec21[2] = {0.040179f, 0.093750f};
    float vec22[2] = {0.013393f, 0.080357f};
    float vec23[2] = {0.000000f, 0.053571f};
    float vec24[2] = {0.000000f, 0.040179f};
    float vec25[2] = {0.013393f, 0.013393f};
    float vec26[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
            glVertex2fv(vec10);
            glVertex2fv(vec11);
            glVertex2fv(vec12);
            glVertex2fv(vec13);
            glVertex2fv(vec14);
            glVertex2fv(vec15);
            glVertex2fv(vec16);
            glVertex2fv(vec17);
            glVertex2fv(vec18);
            glVertex2fv(vec19);
            glVertex2fv(vec20);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec21);
            glVertex2fv(vec22);
            glVertex2fv(vec23);
            glVertex2fv(vec24);
            glVertex2fv(vec25);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec26);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFT9() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFT9(unsigned int obj)
{
    float vec1[2] = {0.040179f, 0.000000f};
    float vec2[2] = {0.080357f, 0.026786f};
    float vec3[2] = {0.107143f, 0.093750f};
    float vec4[2] = {0.107143f, 0.147321f};
    float vec5[2] = {0.093750f, 0.174107f};
    float vec6[2] = {0.066964f, 0.187500f};
    float vec7[2] = {0.040179f, 0.187500f};
    float vec8[2] = {0.013393f, 0.174107f};
    float vec9[2] = {0.000000f, 0.147321f};
    float vec10[2] = {0.000000f, 0.133929f};
    float vec11[2] = {0.013393f, 0.107143f};
    float vec12[2] = {0.040179f, 0.093750f};
    float vec13[2] = {0.066964f, 0.093750f};
    float vec14[2] = {0.093750f, 0.107143f};
    float vec15[2] = {0.107143f, 0.133929f};
    float vec16[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
            glVertex2fv(vec10);
            glVertex2fv(vec11);
            glVertex2fv(vec12);
            glVertex2fv(vec13);
            glVertex2fv(vec14);
            glVertex2fv(vec15);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec16);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTA() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTA(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.000000f, 0.120536f};
    float vec3[2] = {0.026786f, 0.174107f};
    float vec4[2] = {0.053571f, 0.187500f};
    float vec5[2] = {0.080357f, 0.174107f};
    float vec6[2] = {0.107143f, 0.120536f};
    float vec7[2] = {0.107143f, 0.000000f};
    float vec8[2] = {0.107143f, 0.093750f};
    float vec9[2] = {0.000000f, 0.093750f};
    float vec10[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec10);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTB() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTB(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.080357f, 0.000000f};
    float vec3[2] = {0.093750f, 0.013393f};
    float vec4[2] = {0.107143f, 0.040179f};
    float vec5[2] = {0.107143f, 0.053571f};
    float vec6[2] = {0.093750f, 0.080357f};
    float vec7[2] = {0.080357f, 0.093750f};
    float vec8[2] = {0.093750f, 0.107143f};
    float vec9[2] = {0.107143f, 0.133929f};
    float vec10[2] = {0.107143f, 0.147321f};
    float vec11[2] = {0.093750f, 0.174107f};
    float vec12[2] = {0.080357f, 0.187500f};
    float vec13[2] = {0.000000f, 0.187500f};
    float vec14[2] = {0.013393f, 0.187500f};
    float vec15[2] = {0.013393f, 0.000000f};
    float vec16[2] = {0.013393f, 0.093750f};
    float vec17[2] = {0.080357f, 0.093750f};
    float vec18[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
            glVertex2fv(vec10);
            glVertex2fv(vec11);
            glVertex2fv(vec12);
            glVertex2fv(vec13);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec14);
            glVertex2fv(vec15);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec16);
            glVertex2fv(vec17);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec18);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTC() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTC(unsigned int obj)
{
    float vec1[2] = {0.013393f, 0.013393f};
    float vec2[2] = {0.040179f, 0.000000f};
    float vec3[2] = {0.066964f, 0.000000f};
    float vec4[2] = {0.093750f, 0.013393f};
    float vec5[2] = {0.107143f, 0.040179f};
    float vec6[2] = {0.107143f, 0.147321f};
    float vec7[2] = {0.093750f, 0.174107f};
    float vec8[2] = {0.066964f, 0.187500f};
    float vec9[2] = {0.040179f, 0.187500f};
    float vec10[2] = {0.013393f, 0.174107f};
    float vec11[2] = {0.000000f, 0.133929f};
    float vec12[2] = {0.000000f, 0.053571f};
    float vec13[2] = {0.013393f, 0.013393f};
    float vec14[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
            glVertex2fv(vec10);
            glVertex2fv(vec11);
            glVertex2fv(vec12);
            glVertex2fv(vec13);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec14);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTD() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTD(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.066964f, 0.000000f};
    float vec3[2] = {0.093750f, 0.013393f};
    float vec4[2] = {0.107143f, 0.040179f};
    float vec5[2] = {0.107143f, 0.147321f};
    float vec6[2] = {0.093750f, 0.174107f};
    float vec7[2] = {0.066964f, 0.187500f};
    float vec8[2] = {0.000000f, 0.187500f};
    float vec9[2] = {0.013393f, 0.187500f};
    float vec10[2] = {0.013393f, 0.000000f};
    float vec11[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec9);
            glVertex2fv(vec10);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec11);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTE() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTE(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.000000f, 0.187500f};
    float vec3[2] = {0.107143f, 0.187500f};
    float vec4[2] = {0.080357f, 0.093750f};
    float vec5[2] = {0.000000f, 0.093750f};
    float vec6[2] = {0.000000f, 0.000000f};
    float vec7[2] = {0.107143f, 0.000000f};
    float vec8[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec8);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTF() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTF(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.000000f, 0.187500f};
    float vec3[2] = {0.107143f, 0.187500f};
    float vec4[2] = {0.066964f, 0.107143f};
    float vec5[2] = {0.000000f, 0.107143f};
    float vec6[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec6);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTG() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTG(unsigned int obj)
{
    float vec1[2] = {0.013393f, 0.013393f};
    float vec2[2] = {0.040179f, 0.000000f};
    float vec3[2] = {0.066964f, 0.000000f};
    float vec4[2] = {0.093750f, 0.013393f};
    float vec5[2] = {0.107143f, 0.040179f};
    float vec6[2] = {0.107143f, 0.080357f};
    float vec7[2] = {0.053571f, 0.080357f};
    float vec8[2] = {0.013393f, 0.013393f};
    float vec9[2] = {0.000000f, 0.053571f};
    float vec10[2] = {0.000000f, 0.133929f};
    float vec11[2] = {0.013393f, 0.174107f};
    float vec12[2] = {0.040179f, 0.187500f};
    float vec13[2] = {0.066964f, 0.187500f};
    float vec14[2] = {0.093750f, 0.174107f};
    float vec15[2] = {0.107143f, 0.147321f};
    float vec16[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
            glVertex2fv(vec10);
            glVertex2fv(vec11);
            glVertex2fv(vec12);
            glVertex2fv(vec13);
            glVertex2fv(vec14);
            glVertex2fv(vec15);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec16);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTH() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTH(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.000000f, 0.187500f};
    float vec3[2] = {0.000000f, 0.093750f};
    float vec4[2] = {0.107143f, 0.093750f};
    float vec5[2] = {0.107143f, 0.187500f};
    float vec6[2] = {0.107143f, 0.000000f};
    float vec7[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec7);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTI() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTI(unsigned int obj)
{
    float vec1[2] = {0.026786f, 0.000000f};
    float vec2[2] = {0.080357f, 0.000000f};
    float vec3[2] = {0.053571f, 0.000000f};
    float vec4[2] = {0.053571f, 0.187500f};
    float vec5[2] = {0.026786f, 0.187500f};
    float vec6[2] = {0.080357f, 0.187500f};
    float vec7[2] = {0.080357f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec7);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTJ() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTJ(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.040179f};
    float vec2[2] = {0.013393f, 0.013393f};
    float vec3[2] = {0.040179f, 0.000000f};
    float vec4[2] = {0.066964f, 0.013393f};
    float vec5[2] = {0.080357f, 0.040179f};
    float vec6[2] = {0.080357f, 0.187500f};
    float vec7[2] = {0.080357f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec7);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTK() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTK(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.000000f, 0.187500f};
    float vec3[2] = {0.107143f, 0.187500f};
    float vec4[2] = {0.013393f, 0.093750f};
    float vec5[2] = {0.000000f, 0.093750f};
    float vec6[2] = {0.013393f, 0.093750f};
    float vec7[2] = {0.107143f, 0.000000f};
    float vec8[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec8);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTL() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTL(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.107143f, 0.000000f};
    float vec3[2] = {0.000000f, 0.000000f};
    float vec4[2] = {0.000000f, 0.187500f};
    float vec5[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec5);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTM() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTM(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.000000f, 0.187500f};
    float vec3[2] = {0.053571f, 0.107143f};
    float vec4[2] = {0.107143f, 0.187500f};
    float vec5[2] = {0.107143f, 0.000000f};
    float vec6[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec6);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTN() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTN(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.000000f, 0.187500f};
    float vec3[2] = {0.107143f, 0.000000f};
    float vec4[2] = {0.107143f, 0.187500f};
    float vec5[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec5);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTO() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTO(unsigned int obj)
{
    float vec1[2] = {0.026786f, 0.000000f};
    float vec2[2] = {0.080357f, 0.000000f};
    float vec3[2] = {0.107143f, 0.053571f};
    float vec4[2] = {0.107143f, 0.133929f};
    float vec5[2] = {0.080357f, 0.187500f};
    float vec6[2] = {0.026786f, 0.187500f};
    float vec7[2] = {0.000000f, 0.133929f};
    float vec8[2] = {0.000000f, 0.053571f};
    float vec9[2] = {0.026786f, 0.000000f};
    float vec10[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec10);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTP() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTP(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.000000f, 0.187500f};
    float vec3[2] = {0.066964f, 0.187500f};
    float vec4[2] = {0.093750f, 0.174107f};
    float vec5[2] = {0.107143f, 0.147321f};
    float vec6[2] = {0.107143f, 0.133929f};
    float vec7[2] = {0.093750f, 0.107143f};
    float vec8[2] = {0.066964f, 0.093750f};
    float vec9[2] = {0.000000f, 0.093750f};
    float vec10[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec10);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTQ() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTQ(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.040179f};
    float vec2[2] = {0.013393f, 0.013393f};
    float vec3[2] = {0.040179f, 0.000000f};
    float vec4[2] = {0.066964f, 0.000000f};
    float vec5[2] = {0.093750f, 0.013393f};
    float vec6[2] = {0.107143f, 0.040179f};
    float vec7[2] = {0.107143f, 0.147321f};
    float vec8[2] = {0.093750f, 0.174107f};
    float vec9[2] = {0.066964f, 0.187500f};
    float vec10[2] = {0.040179f, 0.187500f};
    float vec11[2] = {0.013393f, 0.174107f};
    float vec12[2] = {0.000000f, 0.147321f};
    float vec13[2] = {0.000000f, 0.040179f};
    float vec14[2] = {0.053571f, 0.053571f};
    float vec15[2] = {0.107143f, 0.000000f};
    float vec16[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
            glVertex2fv(vec10);
            glVertex2fv(vec11);
            glVertex2fv(vec12);
            glVertex2fv(vec13);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec14);
            glVertex2fv(vec15);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec16);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTR() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTR(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.000000f, 0.187500f};
    float vec3[2] = {0.066964f, 0.187500f};
    float vec4[2] = {0.093750f, 0.174107f};
    float vec5[2] = {0.107143f, 0.147321f};
    float vec6[2] = {0.107143f, 0.133929f};
    float vec7[2] = {0.093750f, 0.107143f};
    float vec8[2] = {0.066964f, 0.093750f};
    float vec9[2] = {0.000000f, 0.093750f};
    float vec10[2] = {0.053571f, 0.093750f};
    float vec11[2] = {0.107143f, 0.000000f};
    float vec12[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec10);
            glVertex2fv(vec11);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec12);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTS() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTS(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.040179f};
    float vec2[2] = {0.013393f, 0.013393f};
    float vec3[2] = {0.040179f, 0.000000f};
    float vec4[2] = {0.066964f, 0.000000f};
    float vec5[2] = {0.093750f, 0.013393f};
    float vec6[2] = {0.107143f, 0.040179f};
    float vec7[2] = {0.107143f, 0.053571f};
    float vec8[2] = {0.093750f, 0.080357f};
    float vec9[2] = {0.066964f, 0.093750f};
    float vec10[2] = {0.040179f, 0.093750f};
    float vec11[2] = {0.013393f, 0.107143f};
    float vec12[2] = {0.000000f, 0.133929f};
    float vec13[2] = {0.000000f, 0.147321f};
    float vec14[2] = {0.013393f, 0.174107f};
    float vec15[2] = {0.040179f, 0.187500f};
    float vec16[2] = {0.066964f, 0.187500f};
    float vec17[2] = {0.093750f, 0.174107f};
    float vec18[2] = {0.107143f, 0.147321f};
    float vec19[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
            glVertex2fv(vec10);
            glVertex2fv(vec11);
            glVertex2fv(vec12);
            glVertex2fv(vec13);
            glVertex2fv(vec14);
            glVertex2fv(vec15);
            glVertex2fv(vec16);
            glVertex2fv(vec17);
            glVertex2fv(vec18);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec19);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTT() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTT(unsigned int obj)
{
    float vec1[2] = {0.053571f, 0.000000f};
    float vec2[2] = {0.053571f, 0.187500f};
    float vec3[2] = {0.000000f, 0.187500f};
    float vec4[2] = {0.107143f, 0.187500f};
    float vec5[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec5);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTU() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTU(unsigned int obj)
{
    float vec1[2] = {0.013393f, 0.013393f};
    float vec2[2] = {0.000000f, 0.040179f};
    float vec3[2] = {0.000000f, 0.187500f};
    float vec4[2] = {0.107143f, 0.187500f};
    float vec5[2] = {0.107143f, 0.040179f};
    float vec6[2] = {0.093750f, 0.013393f};
    float vec7[2] = {0.066964f, 0.000000f};
    float vec8[2] = {0.040179f, 0.000000f};
    float vec9[2] = {0.013393f, 0.013393f};
    float vec10[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
            glVertex2fv(vec8);
            glVertex2fv(vec9);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec10);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTV() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTV(unsigned int obj)
{
    float vec1[2] = {0.053571f, 0.000000f};
    float vec2[2] = {0.000000f, 0.120536f};
    float vec3[2] = {0.000000f, 0.187500f};
    float vec4[2] = {0.107143f, 0.187500f};
    float vec5[2] = {0.107143f, 0.120536f};
    float vec6[2] = {0.053571f, 0.000000f};
    float vec7[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec7);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTW() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTW(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.000000f, 0.187500f};
    float vec3[2] = {0.107143f, 0.187500f};
    float vec4[2] = {0.107143f, 0.000000f};
    float vec5[2] = {0.053571f, 0.093750f};
    float vec6[2] = {0.000000f, 0.000000f};
    float vec7[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
            glVertex2fv(vec6);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec7);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTX() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTX(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.107143f, 0.187500f};
    float vec3[2] = {0.000000f, 0.187500f};
    float vec4[2] = {0.107143f, 0.000000f};
    float vec5[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec5);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTY() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTY(unsigned int obj)
{
    float vec1[2] = {0.053571f, 0.000000f};
    float vec2[2] = {0.053571f, 0.093750f};
    float vec3[2] = {0.000000f, 0.187500f};
    float vec4[2] = {0.107143f, 0.187500f};
    float vec5[2] = {0.053571f, 0.093750f};
    float vec6[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec6);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTZ() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTZ(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.107143f, 0.187500f};
    float vec3[2] = {0.000000f, 0.187500f};
    float vec4[2] = {0.013393f, 0.093750f};
    float vec5[2] = {0.093750f, 0.093750f};
    float vec6[2] = {0.107143f, 0.000000f};
    float vec7[2] = {0.000000f, 0.000000f};
    float vec8[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec4);
            glVertex2fv(vec5);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec6);
            glVertex2fv(vec7);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec8);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTMIN() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTMIN(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.093750f};
    float vec2[2] = {0.107143f, 0.093750f};
    float vec3[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTPER() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTPER(unsigned int obj)
{
    float vec1[2] = {0.040179f, 0.026786f};
    float vec2[2] = {0.053571f, 0.053571f};
    float vec3[2] = {0.066964f, 0.026786f};
    float vec4[2] = {0.040179f, 0.026786f};
    float vec5[2] = {0.066964f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
            glVertex2fv(vec3);
            glVertex2fv(vec4);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec5);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFTSLH() -
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFTSLH(unsigned int obj)
{
    float vec1[2] = {0.000000f, 0.000000f};
    float vec2[2] = {0.107143f, 0.187500f};
    float vec3[2] = {0.107143f, 0.093750f};

    glNewList(obj, GL_COMPILE);
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec1);
            glVertex2fv(vec2);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2fv(vec3);
        glEnd();
    glEndList();
}

//------------------------------------------------------------------------------
// makeFont() - make all of the above font letters
//------------------------------------------------------------------------------
void Eadi3DObjects::makeFont() {
    makeFTZRO(base+FONT_0);
    makeFT1(base+FONT_1);
    makeFT2(base+FONT_2);
    makeFT3(base+FONT_3);
    makeFT4(base+FONT_4);
    makeFT5(base+FONT_5);
    makeFT6(base+FONT_6);
    makeFT7(base+FONT_7);
    makeFT8(base+FONT_8);
    makeFT9(base+FONT_9);
    makeFTMIN(base+PUNC05);
    makeFTPER(base+PUNC06);
    makeFTSLH(base+PUNC07);
    makeFTA(base+FONT_A);
    makeFTB(base+FONT_B);
    makeFTC(base+FONT_C);
    makeFTD(base+FONT_D);
    makeFTE(base+FONT_E);
    makeFTF(base+FONT_F);
    makeFTG(base+FONT_G);
    makeFTH(base+FONT_H);
    makeFTI(base+FONT_I);
    makeFTJ(base+FONT_J);
    makeFTK(base+FONT_K);
    makeFTL(base+FONT_L);
    makeFTM(base+FONT_M);
    makeFTN(base+FONT_N);
    makeFTO(base+FONT_O);
    makeFTP(base+FONT_P);
    makeFTQ(base+FONT_Q);
    makeFTR(base+FONT_R);
    makeFTS(base+FONT_S);
    makeFTT(base+FONT_T);
    makeFTU(base+FONT_U);
    makeFTV(base+FONT_V);
    makeFTW(base+FONT_W);
    makeFTX(base+FONT_X);
    makeFTY(base+FONT_Y);
    makeFTZ(base+FONT_Z);
}

}
}
