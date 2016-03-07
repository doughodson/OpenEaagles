#include "openeaagles/instruments/landingGear/LandingLight.h"
#include "openeaagles/base/Number.h"
#include <GL/glu.h>

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(LandingLight,"LandingLight")
EMPTY_SERIALIZER(LandingLight)

BEGIN_SLOTTABLE(LandingLight)
    "lightRadius",          // 3 Radius of our light (if not components)
END_SLOTTABLE(LandingLight)

//------------------------------------------------------------------------------
//  Map slot table to handles for LandingGear
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(LandingLight)
    ON_SLOT(1, setSlotLightRadius, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
LandingLight::LandingLight()
{
    STANDARD_CONSTRUCTOR()
    lRadius = 0;             // default to 0, means don't use it...
    gearCurrent = 0;
    selSD.empty();
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void LandingLight::copyData(const LandingLight& org, const bool)
{
    // copy base class stuff first
    BaseClass::copyData(org);

    gearCurrent = org.gearCurrent;
    lRadius = org.lRadius;
    selSD.empty();
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(LandingLight)

// set functions
//------------------------------------------------------------------------------
// setLightRadius() - how big is our light?
//------------------------------------------------------------------------------
bool LandingLight::setLightRadius(const double newLR)
{
    lRadius = newLR;
    return true;
}


//------------------------------------------------------------------------------
// setSlotLightRadius() - sets our light radius to be drawn
//------------------------------------------------------------------------------
bool LandingLight::setSlotLightRadius(const base::Number* const newLR)
{
    bool ok = false;
    if (newLR != nullptr) ok = setLightRadius(newLR->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// drawFunc()
//------------------------------------------------------------------------------
void LandingLight::drawFunc()
{
    // if the user specifies a light radius, then it will draw this way,
    if (lRadius == 0) return;
    GLfloat currentColor[4];
    GLfloat lw = 0;
    glGetFloatv(GL_CURRENT_COLOR, &currentColor[0]);
    glGetFloatv(GL_LINE_WIDTH, &lw);

    // all we need is the gear up value, and we can toggle accordingly
    double gearUpVal = getGearUpValue();
    double gearDownVal = getGearDownValue();
    double lastC = gearCurrent;
    gearCurrent = getInstValue();

    glPushMatrix();
        // determine which way we are going
        if (lastC >= gearCurrent) {
            // going towards the up position (getting closer to 0)
            if (gearCurrent <= gearUpVal) glColor3f(0, 0, 0);
            else glColor3f(0, 1, 0);
        }
        else if (lastC < gearCurrent) {
            // going towards the down position (getting closer to 1)
            if (gearCurrent >= gearDownVal) glColor3f(0, 1, 0);
            else glColor3f(0, 0, 0);
        }

        GLUquadricObj *qobj = gluNewQuadric();
        gluDisk(qobj, 0,  lRadius, 1000, 1);
        gluDeleteQuadric(qobj);
    glPopMatrix();

    glColor4fv(currentColor);
    glLineWidth(lw);
}

//------------------------------------------------------------------------------
// updateData() -
//------------------------------------------------------------------------------
void LandingLight::updateData(const double dt)
{
    BaseClass::updateData(dt);

    double gearUpVal = getGearUpValue();
    double gearDownVal = getGearDownValue();
    double lastC = gearCurrent;
    gearCurrent = getInstValue();

    int x = 0;
    // determine which way we are going
    if (lastC >= gearCurrent) {
        // going towards the up position (getting closer to 0)
        if (gearCurrent <= gearUpVal) x = 0;
        else x = 1;
    }
    else if (lastC < gearCurrent) {
        // going towards the down position (getting closer to 1)
        if (gearCurrent >= gearDownVal) x = 1;
        else x = 0;
    }

    send("index", SELECT, x, selSD);
}

//------------------------------------------------------------------------------
// getSlotByIndex() for LandingLight
//------------------------------------------------------------------------------
base::Object* LandingLight::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end instruments namespace
}  // end oe namespace
