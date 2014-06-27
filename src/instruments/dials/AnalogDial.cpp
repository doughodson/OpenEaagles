#include "openeaagles/instruments/dials/AnalogDial.h"
#include "openeaagles/basic/Number.h"
#include <GL/glu.h>

namespace Eaagles {
namespace Instruments {

IMPLEMENT_SUBCLASS(AnalogDial, "AnalogDial")
EMPTY_SERIALIZER(AnalogDial)

// Event handler
BEGIN_EVENT_HANDLER(AnalogDial)
    ON_EVENT_OBJ(UPDATE_VALUE, onUpdateRadius, Basic::Number)
END_EVENT_HANDLER()

BEGIN_SLOTTABLE(AnalogDial)
    "startAngle",           // 1) angle in which we start drawing our background from 
    "sweepAngle",           // 2) sweep angle (if not drawing a full circle and not drawing a semi-circle, you can determine how far you want the outline to draw)
    "dialRadius",           // 3) how far do we want our background to extend?
    "mobile",               // 4) do we rotate if given a value, or do we just pass it down to our components?
    "slices",               // 5) number of slices to use
END_SLOTTABLE(AnalogDial)       

//------------------------------------------------------------------------------
//  Map slot table to handles for Analog Dial
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(AnalogDial)
    ON_SLOT(1, setSlotOriginAngle, Basic::Number)
    ON_SLOT(2, setSlotSweepAngle, Basic::Number)
    ON_SLOT(3, setSlotRadius, Basic::Number)
    ON_SLOT(4, setSlotMobile, Basic::Number)
    ON_SLOT(5, setSlotSlices, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AnalogDial::AnalogDial()
{
    STANDARD_CONSTRUCTOR()
    positionAngle = 0;
    originAngle = 0;
    sweepAngle = 360;
    radius = 0;
    isMobile = false;
    slices = 1000;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void AnalogDial::copyData(const AnalogDial& org, const bool)
{
    BaseClass::copyData(org);
    
    originAngle = org.originAngle;
    sweepAngle = org.sweepAngle;
    radius = org.radius;
    positionAngle = org.positionAngle;
    isMobile = org.isMobile;
    slices = org.slices;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(AnalogDial)

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void AnalogDial::drawFunc()
{
    GLfloat ocolor[4];
    glGetFloatv(GL_CURRENT_COLOR, ocolor);
    
    // get our position from our base instrument class, which handles scaling and such
    positionAngle = getInstValue();
    
    glPushMatrix();
        // move us just slightly into the background, so things will overlay us
        glTranslatef(0, 0, -0.1f);
        if (isMobile) glRotatef( static_cast<GLfloat>(-positionAngle), 0, 0, 1);
        // draw our background
        GLUquadricObj *qobj = gluNewQuadric();
        gluQuadricDrawStyle(qobj, GLU_FILL);
        // if we are a semi-circle, we ignore our start and sweep angles, and draw a semicircle
        gluPartialDisk(qobj, 0.0,  radius, slices, 1, originAngle, sweepAngle);
        gluDeleteQuadric(qobj);
    glPopMatrix();
        
    glColor4fv(ocolor);
}

//------------------------------------------------------------------------------
// onUpdateRadius() - adjust our radius
//------------------------------------------------------------------------------
bool AnalogDial::onUpdateRadius(const Basic::Number* const x)
{
    bool ok = false;
    if (x != 0) ok = setRadius(x->getReal());
    return ok;
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotOriginAngle() -- 
//------------------------------------------------------------------------------
bool AnalogDial::setSlotOriginAngle(const Basic::Number* newAngle)       
{
    bool ok = false;
    if (newAngle != 0) {
        ok = setOriginAngle(newAngle->getReal());
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotSweepAngle() -- 
//------------------------------------------------------------------------------
bool AnalogDial::setSlotSweepAngle(const Basic::Number* newSweepAngle)
{
    bool ok = false;
    if (newSweepAngle != 0) ok = setSweepAngle(newSweepAngle->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotradius() -- sets our background radius of the "dial"
//------------------------------------------------------------------------------
bool AnalogDial::setSlotRadius(const Basic::Number* newR)
{
    bool ok = false;
    if (newR != 0) ok = setRadius(newR->getReal());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotMobile() -- allows us to rotate around our dial
//------------------------------------------------------------------------------
bool AnalogDial::setSlotMobile(const Basic::Number* newM)
{
    bool ok = false;
    if (newM != 0) ok = setMobile(newM->getBoolean());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotSlices() -- how many slices to use?
//------------------------------------------------------------------------------
bool AnalogDial::setSlotSlices(const Basic::Number* const x)
{
    bool ok = false;
    if (x != 0) ok = setSlices(x->getInt());
    return ok;
}

// SET FUNCTIONS
//------------------------------------------------------------------------------
// setOriginAngle() - start angle
//------------------------------------------------------------------------------
bool AnalogDial::setOriginAngle(const LCreal na)
{
    originAngle = na;
    return true;
}

//------------------------------------------------------------------------------
// setSweepAngle() - how far to sweep (degrees)
//------------------------------------------------------------------------------
bool AnalogDial::setSweepAngle(const LCreal newSA)
{
    sweepAngle = newSA;
    return true;
}

//------------------------------------------------------------------------------
// setRadius() - set the radius of our dial 
//------------------------------------------------------------------------------
bool AnalogDial::setRadius(const LCreal newR)
{
    radius = newR;
    return true;
}

//------------------------------------------------------------------------------
// setMobile() - should the whole dial move, or just certain parts?
//------------------------------------------------------------------------------
bool AnalogDial::setMobile(const bool newM)
{
    isMobile = newM;
    return true;
}

//------------------------------------------------------------------------------
// setSlices() - determines how "smooth" our dial looks
//------------------------------------------------------------------------------
bool AnalogDial::setSlices(const int x)
{
    slices = x;
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for AnalogDial
//------------------------------------------------------------------------------
Basic::Object* AnalogDial::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end Instruments namespace
}  // end Eaagles namespace
