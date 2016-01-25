#include "openeaagles/instruments/dials/DialArcSegment.h"
#include "openeaagles/basic/Number.h"
#include <GL/glu.h>

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(DialArcSegment, "DialArcSegment")
EMPTY_SERIALIZER(DialArcSegment)

BEGIN_SLOTTABLE(DialArcSegment)
    "dynamic",                  // do we draw around a circle according to our value?
    "outerRadius",              // this will allow us to draw nice clean arcs
    "filled",                   // fill the inside?
END_SLOTTABLE(DialArcSegment)

//------------------------------------------------------------------------------
//  Map slot table to handles for Analog Dial
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(DialArcSegment)
    ON_SLOT(1, setSlotIsDynamic, basic::Number)
    ON_SLOT(2, setSlotOuterRadius, basic::Number)
    ON_SLOT(3, setSlotFilled, basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
DialArcSegment::DialArcSegment()
{
    STANDARD_CONSTRUCTOR()
    isDynamic = false;
    outerRadius = 0.5f; // just a bit bigger than analog dial
    filled = true;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DialArcSegment::copyData(const DialArcSegment& org, const bool)
{
    // copy base stuff first
    BaseClass::copyData(org);
    isDynamic = org.isDynamic;
    outerRadius = org.outerRadius;
    filled = org.filled;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(DialArcSegment)

// SLOT FUNCTION(s)
//------------------------------------------------------------------------------
// setSlotIsDyanmic()
//------------------------------------------------------------------------------
bool DialArcSegment::setSlotIsDynamic(const basic::Number* const newD)
{
    bool ok = false;
    if (newD != nullptr) ok = setIsDynamic(newD->getBoolean());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotOuterRadius() - sets the OUTER dial radius
//------------------------------------------------------------------------------
bool DialArcSegment::setSlotOuterRadius(const basic::Number* const x)
{
    if (x != nullptr) {
        return setOuterRadius(x->getFloat());
    }
    return false;
}

//------------------------------------------------------------------------------
// setSlotFilled() - is our segment filled or not?
//------------------------------------------------------------------------------
bool DialArcSegment::setSlotFilled(const basic::Number* const x)
{
    if (x != nullptr) {
        return setFilled(x->getBoolean());
    }
    return false;
}

// SET FUNCTIONS
//------------------------------------------------------------------------------
// setIsDynamic()
//------------------------------------------------------------------------------
bool DialArcSegment::setIsDynamic(const bool newID)
{
    isDynamic = newID;
    return true;
}

//------------------------------------------------------------------------------
// setOuterRadius() -
//------------------------------------------------------------------------------
bool DialArcSegment::setOuterRadius(const LCreal x)
{
    if (x >= getRadius()) outerRadius = x;
    return true;
}

//------------------------------------------------------------------------------
// setFilled() -
//------------------------------------------------------------------------------
bool DialArcSegment::setFilled(const bool x)
{
    filled = x;
    return true;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void DialArcSegment::drawFunc()
{
    // get our data from our base class
    LCreal startAngle = getStartAngle();
    LCreal radius = getRadius();
    LCreal sweepAngle = getSweepAngle();

    GLint curSlices = getSlices();
    // our slice amount should go up as we get more of a sweep, if not, it will
    // look funny.  Pretty much one slice per degree sweep
    LCreal y = lcAbs(static_cast<LCreal>(sweepAngle));
    curSlices = curSlices + static_cast<GLint>(y * 0.05f);

    // draw our arc
    glPushMatrix();
        GLUquadric* qobj = gluNewQuadric();
        if (filled) gluQuadricDrawStyle(qobj, GL_FILL);
        else gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
        gluPartialDisk(qobj, radius, outerRadius, curSlices, 1, startAngle, sweepAngle);
        gluDeleteQuadric(qobj);
    glPopMatrix();
}

//------------------------------------------------------------------------------
// onUpdateRadius() - event function to update our radius value
//------------------------------------------------------------------------------
bool DialArcSegment::onUpdateRadius(const basic::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        // first determine our ratio from inner to outer radius
        LCreal diff = outerRadius - getRadius();
        ok = setRadius(x->getReal());
        outerRadius = getRadius() + diff;
    }
    return ok;
}

//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void DialArcSegment::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);

    if (isDynamic) setSweepAngle(getInstValue());
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Instrument
//------------------------------------------------------------------------------
basic::Object* DialArcSegment::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end of Instruments namespace
}  // end of oe namespace
