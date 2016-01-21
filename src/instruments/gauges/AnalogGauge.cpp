#include "openeaagles/instruments/gauges/AnalogGauge.h"
#include "openeaagles/basic/Number.h"

namespace oe {
namespace Instruments {

IMPLEMENT_SUBCLASS(AnalogGauge, "AnalogGauge")
EMPTY_SERIALIZER(AnalogGauge)

BEGIN_SLOTTABLE(AnalogGauge)
    "leftBoundary",                 // the left side of the gauge (inches)
    "rightBoundary",                // the right side of the gauge (inches)
    "outlined",                     // are we an outline or a filled bar?
    "vertical",                     // are we drawing up and down?
END_SLOTTABLE(AnalogGauge)

//------------------------------------------------------------------------------
//  Map slot table to handles for Analog Dial
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(AnalogGauge)
    ON_SLOT(1, setSlotLeftBoundary, Basic::Number)
    ON_SLOT(2, setSlotRightBoundary, Basic::Number)
    ON_SLOT(3, setSlotIsOutlined, Basic::Number)
    ON_SLOT(4, setSlotIsVertical, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AnalogGauge::AnalogGauge()
{
    STANDARD_CONSTRUCTOR()
    gaugePos = 0;
    leftBoundary = -1;
    rightBoundary = 1;
    outline = false;
    vertical = true;
    drawMe = true;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void AnalogGauge::copyData(const AnalogGauge& org, const bool)
{
    // copy base class stuff first
    BaseClass::copyData(org);

    gaugePos = org.gaugePos;
    leftBoundary = org.leftBoundary;
    rightBoundary = org.rightBoundary;
    outline = org.outline;
    vertical = org.vertical;
    drawMe = org.drawMe;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(AnalogGauge)

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void AnalogGauge::drawFunc()
{
    if (drawMe) {
    glPushMatrix();
        // move us just slightly into the background, so things will overlay us
        gaugePos = getInstValue();
        //glTranslatef(0, 0, -0.1f);
        // if we are vertical, draw us growing up and down, else draw us growing left to right
        if (vertical) {
            if (outline) glBegin(GL_LINE_STRIP);
            else glBegin(GL_POLYGON);
                    lcVertex2(leftBoundary, 0);
                    lcVertex2(leftBoundary, gaugePos);
                    lcVertex2(rightBoundary, gaugePos);
                    lcVertex2(rightBoundary, 0);
                glEnd();
        }
        else {
            if (outline) glBegin(GL_LINE_STRIP);
            else glBegin(GL_POLYGON);
                    lcVertex2(0, leftBoundary);
                    lcVertex2(gaugePos, leftBoundary);
                    lcVertex2(gaugePos, rightBoundary);
                    lcVertex2(0, rightBoundary);
                glEnd();
        }
    glPopMatrix();
}
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotLeftBoundary() -- sets the left side of the gauge
//------------------------------------------------------------------------------
bool AnalogGauge::setSlotLeftBoundary(const Basic::Number* const newLB)
{
    bool ok = false;
    if (newLB != nullptr) ok = setLeftBoundary(newLB->getReal());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotRightBoundary() -- sets the right side of the gauge
//------------------------------------------------------------------------------
bool AnalogGauge::setSlotRightBoundary(const Basic::Number* const newRB)
{
    bool ok = false;
    if (newRB != nullptr) ok = setRightBoundary(newRB->getReal());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotIsOutlined() -- determines whether we are a filled bar or outlined
//------------------------------------------------------------------------------
bool AnalogGauge::setSlotIsOutlined(const Basic::Number* const newO)
{
    bool ok = false;
    if (newO != nullptr) ok = setIsOutlined(newO->getBoolean());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotIsVertical() - sets our vertical flag
//------------------------------------------------------------------------------
bool AnalogGauge::setSlotIsVertical(const Basic::Number* const newV)
{
    bool ok = false;
    if (newV != nullptr) ok = setIsVertical(newV->getBoolean());
    return ok;
}

// SET functions
//------------------------------------------------------------------------------
// setLeftBoundary() - left side of gauge
//------------------------------------------------------------------------------
bool AnalogGauge::setLeftBoundary(const LCreal newLB)
{
    leftBoundary = newLB;
    return true;
}

//------------------------------------------------------------------------------
// setRightBoundary() - right side of gauge
//------------------------------------------------------------------------------
bool AnalogGauge::setRightBoundary(const LCreal newRB)
{
    rightBoundary = newRB;
    return true;
}

//------------------------------------------------------------------------------
// setIsOutlined() - set our outline
//------------------------------------------------------------------------------
bool AnalogGauge::setIsOutlined(const bool newO)
{
    outline = newO;
    return true;
}

//------------------------------------------------------------------------------
// setIsVertical() - vertical or horizontal gauge?
//------------------------------------------------------------------------------
bool AnalogGauge::setIsVertical(const bool newV)
{
    vertical = newV;
    return true;
}

//------------------------------------------------------------------------------
// setDrawMe() - determines if we draw our drawFunc or not.
//------------------------------------------------------------------------------
bool AnalogGauge::setDrawMe(const bool x)
{
    drawMe = x;
    return true;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for AnalogGauge
//------------------------------------------------------------------------------
Basic::Object* AnalogGauge::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end Instruments namespace
}  // end oe namespace
