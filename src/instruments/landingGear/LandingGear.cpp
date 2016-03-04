#include "openeaagles/instruments/landingGear/LandingGear.h"
#include "openeaagles/base/Number.h"

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(LandingGear,"LandingGear")
EMPTY_SERIALIZER(LandingGear)

BEGIN_SLOTTABLE(LandingGear)
    "gearDownValue",  // the gear position that defines the "down" state of the gear handle
    "gearUpValue",    // gear position that defines the "up" state of the gear handle
    // Note: Everything in between these values are considered "in Transit" and will be
    // treated as such
END_SLOTTABLE(LandingGear)

//------------------------------------------------------------------------------
//  Map slot table to handles for LandingGear
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(LandingGear)
    ON_SLOT(1, setSlotGearDownValue, basic::Number)
    ON_SLOT(2, setSlotGearUpValue, basic::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
LandingGear::LandingGear()
{
    STANDARD_CONSTRUCTOR()
    gearState = 0;   // gear is in the up position
    gearDV = 1;
    gearUV = 0;
    inTransit = false;
    gearPos = 0;
    gearSelSD.empty();
    haveRotary = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void LandingGear::copyData(const LandingGear& org, const bool)
{
    BaseClass::copyData(org);
    gearState = org.gearState;
    inTransit = org.inTransit;
    gearPos = org.gearPos;
    gearDV = org.gearDV;
    gearUV = org.gearUV;
    gearSelSD.empty();
    haveRotary = org.haveRotary;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(LandingGear)

// SLOT FUNCTIONS
//------------------------------------------------------------------------------
// setSlotGearDownValue() -
//------------------------------------------------------------------------------
bool LandingGear::setSlotGearDownValue(const basic::Number* const newDV)
{
    bool ok = false;
    if (newDV != nullptr) ok = setGearDownValue(newDV->getReal());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotGearUpValue() -
//------------------------------------------------------------------------------
bool LandingGear::setSlotGearUpValue(const basic::Number* const newUV)
{
    bool ok = false;
    if (newUV != nullptr) ok = setGearUpValue(newUV->getReal());
    return ok;
}

// SET functions
//------------------------------------------------------------------------------
// setGearDownValue() - set the value at which the gear goes down
//------------------------------------------------------------------------------
bool LandingGear::setGearDownValue(const LCreal newDV)
{
    gearDV = newDV;
    return true;
}

//------------------------------------------------------------------------------
// setGearUpValue() -
//------------------------------------------------------------------------------
bool LandingGear::setGearUpValue(const LCreal newUV)
{
    gearUV = newUV;
    return true;
}

//------------------------------------------------------------------------------
// updateData() - update non time-critical stuff here
//------------------------------------------------------------------------------
void LandingGear::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);

    // this will store our last value, so we know which way we are going
    LCreal lastPos = gearPos;
    gearPos = getInstValue();

    if (gearPos == gearUV) {
        gearState = 0;
        inTransit = false;
    }
    else if (gearPos == gearDV) {
        gearState = 1;
        inTransit = false;
    }
    // if we aren't equal to either, we are in transit
    else {
        inTransit = true;
        // we are going towards the gear up value
        if (gearPos < lastPos) gearState = 0;
        // we are going towards the down value
        else if (gearPos > lastPos) gearState = 1;
        // if we are equal, we do nothing
    }

    // now send our select down based on our transition flag and gear pos
    int x = 0;
    if (gearState == 0 && !inTransit) x = 1;
    else if (gearState == 0 && inTransit) x = 2;
    else if (gearState == 1 && inTransit) x = 3;
    else if (gearState == 1 && !inTransit) x = 4;

    send("gearpos", SELECT, x, gearSelSD);

    // determine if we have a rotary
    basic::Pair* pair = (basic::Pair*)findByName("gearpos");
    if (pair != nullptr) haveRotary = true;
}

//------------------------------------------------------------------------------
// drawFunc()
//------------------------------------------------------------------------------
void LandingGear::drawFunc()
{
    // only do this if we don't have a rotary components
    if (haveRotary) return;

    GLfloat currentColor[4];
    GLfloat lw = 0;
    glGetFloatv(GL_CURRENT_COLOR, &currentColor[0]);
    glGetFloatv(GL_LINE_WIDTH, &lw);

    // draw the landing gear dependent on the gear state
    // gear is up
    if (gearState == 0) {
        glPushMatrix();
            glTranslatef(0, 1, 0);
            // the lines on the handle
            glColor3f(0,0,0);
            glBegin(GL_LINES);
                glVertex2f(-0.3f, -0.6f);
                glVertex2f(-0.3f, 0.6f);
                glVertex2f(-0.1f, -0.6f);
                glVertex2f(-0.1f, 0.6f);
                glVertex2f(0.1f, -0.6f);
                glVertex2f(0.1f, 0.6f);
                glVertex2f(0.3f, -0.6f);
                glVertex2f(0.3f, 0.6f);
            glEnd();
            if (inTransit) glColor3f(1, 0, 0);
            else glColor3f(0.9f, 0.9f, 0.9f);
            // the handle
            glBegin(GL_POLYGON);
                glVertex2f(-0.4f, -0.6f);
                glVertex2f(0.4f, -0.6f);
                glVertex2f(0.4f, 0.6f);
                glVertex2f(-0.4f, 0.6f);
            glEnd();
        glPopMatrix();
        // the arm
        glPushMatrix();
            glColor3f(0.3f, 0.3f, 0.3f);
            glBegin(GL_POLYGON);
                glVertex2f(-0.3f, 0);
                glVertex2f(0.3f, 0);
                glVertex2f(0.3f, 1);
                glVertex2f(-0.3f, 1);
            glEnd();
        glPopMatrix();
    }
    // gear is down
    if (gearState == 1) {
        glPushMatrix();
            glTranslatef(0, -1, 0);
            // the lines on the handle
            glColor3f(0,0,0);
            glBegin(GL_LINES);
                glVertex2f(-0.3f, -0.6f);
                glVertex2f(-0.3f, 0.6f);
                glVertex2f(-0.1f, -0.6f);
                glVertex2f(-0.1f, 0.6f);
                glVertex2f(0.1f, -0.6f);
                glVertex2f(0.1f, 0.6f);
                glVertex2f(0.3f, -0.6f);
                glVertex2f(0.3f, 0.6f);
            glEnd();
            if (inTransit) glColor3f(1, 0, 0);
            else glColor3f(0.9f, 0.9f, 0.9f);
            // the handle
            glBegin(GL_POLYGON);
                glVertex2f(-0.4f, -0.6f);
                glVertex2f(0.4f, -0.6f);
                glVertex2f(0.4f, 0.6f);
                glVertex2f(-0.4f, 0.6f);
            glEnd();
        glPopMatrix();
        // the arm
        glPushMatrix();
            glColor3f(0.3f, 0.3f, 0.3f);
            glBegin(GL_POLYGON);
                glVertex2f(-0.3f, 0);
                glVertex2f(0.3f, 0);
                glVertex2f(0.3f, -1);
                glVertex2f(-0.3f, -1);
            glEnd();
        glPopMatrix();
    }
    glColor4fv(currentColor);
    glLineWidth(lw);
}

//------------------------------------------------------------------------------
// getSlotByIndex() for LandingGear
//------------------------------------------------------------------------------
basic::Object* LandingGear::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end instruments namespace
}  // end oe namespace
