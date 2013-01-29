#include "openeaagles/instruments/maps/BearingPointer.h"

namespace Eaagles {
namespace Instruments {

IMPLEMENT_SUBCLASS(BearingPointer, "BearingPointer")
EMPTY_SERIALIZER(BearingPointer)

BEGIN_SLOTTABLE(BearingPointer)
    "headGraphic",  // the graphic we will draw for the head
    "tailGraphic",  // the graphic we will draw for the tail
END_SLOTTABLE(BearingPointer)       

//  Map slot table to handles for Bearing Pointer
BEGIN_SLOT_MAP(BearingPointer)
    ON_SLOT(1, setSlotHeadGraphic, BasicGL::Graphic)
    ON_SLOT(2, setSlotTailGraphic, BasicGL::Graphic)
END_SLOT_MAP()

// Macro event handlers for Bearing Pointer events
BEGIN_EVENT_HANDLER(BearingPointer)
    ON_EVENT_OBJ(UPDATE_VALUE7,onUpdateRadBearingPointer, Basic::Angle)          // Sets bearing to this Basic::Angle 
    ON_EVENT_OBJ(UPDATE_VALUE7,onUpdateRadBearingPointer, Basic::Number)         // Sets bearing to this angle in radians 
    ON_EVENT_OBJ(UPDATE_VALUE8, onUpdateDegBearingPointer, Basic::Number)        // Sets bearing to this angle in degrees 
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
BearingPointer::BearingPointer()
{
    STANDARD_CONSTRUCTOR()

    bearing = 0;
    myRotation = 0.0;    
    myRadius = 0;
    head = 0;
    tail = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void BearingPointer::copyData(const BearingPointer& org, const bool cc)
{
    BaseClass::copyData(org);
    
    if (cc) {
        head = 0;
        tail = 0;
    }

    setSlotHeadGraphic(org.head);
    setSlotTailGraphic(org.tail);
    
    bearing = org.bearing;
    myRotation = org.myRotation;
    myRadius = org.myRadius;
    
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void BearingPointer::deleteData()
{
    setSlotHeadGraphic(0);
    setSlotTailGraphic(0);
}

//------------------------------------------------------------------------------
// draw() 
//------------------------------------------------------------------------------
void BearingPointer::draw()
{
    bool c = isCentered();
    LCreal dis = getDisplacement();
    lcSaveMatrix();
        if (!c) lcTranslate(0, dis);
        lcRotate(myRotation);
        BasicGL::Graphic::draw();
    lcRestoreMatrix();
}


// Event handlers
//------------------------------------------------------------------------------
//  onUpdateRadBearingPointer() - update bearing angle
//------------------------------------------------------------------------------
bool BearingPointer::onUpdateRadBearingPointer(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        setBearingRad( (LCreal) Basic::Radians::convertStatic( *msg ) );
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  onUpdateRadBearingPointer() - update bearing angle by number
//------------------------------------------------------------------------------
bool BearingPointer::onUpdateRadBearingPointer(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        setBearingRad(msg->getReal());  // radians
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  onUpdateDegBearingPointer() - update bearing angle (degrees)
//------------------------------------------------------------------------------
bool BearingPointer::onUpdateDegBearingPointer(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        setBearingDeg(msg->getReal());  // degrees
        ok = true;
    }
    return ok;
}

// SET functions
//------------------------------------------------------------------------------
//  setBearingDeg() - 
//------------------------------------------------------------------------------
bool BearingPointer::setBearingDeg(const LCreal newB)
{
    bearing = newB * (LCreal) Basic::Angle::D2RCC;
    return true;
}

//------------------------------------------------------------------------------
//  setBearingRad() - 
//------------------------------------------------------------------------------
bool BearingPointer::setBearingRad(const LCreal newB)
{
    bearing = newB;
    return true;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s) using openGL
//------------------------------------------------------------------------------
void BearingPointer::drawFunc()
{
    GLfloat ocolor[4];
    GLfloat lw;
    glGetFloatv(GL_CURRENT_COLOR, ocolor);
    glGetFloatv(GL_LINE_WIDTH, &lw);
    
    // draw a default head and tail graphic, unless one is given to us, then draw it
    myRadius = 0;
    bool amICentered = isCentered();
    if (amICentered) myRadius = getCenteredRadius();
    else myRadius = getDeCenteredRadius();
    
    // now draw the head graphic
    glPushMatrix();
        // translate to the top of our radius
        glTranslated(0, GLdouble(myRadius), 0);
        if (head != 0) {
            head->container(this);
            head->draw();
        }
        else {
            glBegin(GL_POLYGON);
                glVertex2f(-0.5, 0);
                glVertex2f(0.5, 0);
                glVertex2f(0, 0.5);
            glEnd();
        }
    glPopMatrix();
    // now draw the tail graphic
    glPushMatrix();
        // translate to the bottom of our radius
        glTranslated(0, GLdouble(-myRadius), 0);
        if (tail != 0) {
            tail->container(this);
            tail->draw();
        }
        else {
            glLineWidth(3);
            glBegin(GL_LINES);
                glVertex2f(0, 0);
                glVertex2f(0, -0.5);
            glEnd();
        }
    glPopMatrix();
    glColor4fv(ocolor);
    glLineWidth(lw);
}

//------------------------------------------------------------------------------
// updateData() - updates our non time-critical threads here
//------------------------------------------------------------------------------
void BearingPointer::updateData(const LCreal dt)
{
    // update our base class first
    BaseClass::updateData(dt);
    
    // get our heading and bearing (hopefully they are in radians, if not, the
    // calculation will be skewed
    LCreal hdg = getRotationRad();
    
    // stay between +- 3.14 radians
    bearing = lcAepcRad(bearing - hdg);
    LCreal dbrg = lcAepcRad(myRotation - bearing);
    
    // if we are over the max, rotate the other way
    LCreal dd0 = dbrg * dt;
    LCreal maxdd0 = (90.0f * (LCreal) Basic::Angle::D2RCC) * dt;      // Limit to 90 degs/sec
    if (dd0 < -maxdd0) dd0 = -maxdd0;
    if (dd0 > maxdd0) dd0 = maxdd0;
    bearing += dd0;
    
    myRotation = bearing;
}


// SLOT FUNCTIONS
//------------------------------------------------------------------------------
// setSlotHeadGraphic() - sets the head graphic for the bearing pointer
//------------------------------------------------------------------------------
bool BearingPointer::setSlotHeadGraphic(const BasicGL::Graphic* const newH)
{
    bool ok = false;
    if (head != 0) {
        head->unref();
        head = 0;
    }
    if (newH != 0) {
        head = newH->clone();
        ok = true;
    }
    return ok;
}
//------------------------------------------------------------------------------
// setSlotTailGraphic() - sets the tail graphic for the bearing pointer
//------------------------------------------------------------------------------
bool BearingPointer::setSlotTailGraphic(const BasicGL::Graphic* const newT)
{
    bool ok = false;
    if (tail != 0) {
        tail->unref();
        tail = 0;
    }
    if (newT != 0) {
        tail = newT->clone();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for BearingPointer
//------------------------------------------------------------------------------
Basic::Object* BearingPointer::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end Instruments namespace
}  // end Eaagles namespace
