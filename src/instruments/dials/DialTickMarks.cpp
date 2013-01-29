#include "openeaagles/instruments/dials/DialTickMarks.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Instruments {

IMPLEMENT_SUBCLASS(DialTickMarks, "DialTickMarks")
EMPTY_SERIALIZER(DialTickMarks)

BEGIN_SLOTTABLE(DialTickMarks)
    "length",           // length of the tick marks
    "quantity",         // tells us how many to draw, default is 1
    "tickGraphic",      // sets a graphic to use instead of a line
END_SLOTTABLE(DialTickMarks)       

//------------------------------------------------------------------------------
//  Map slot table to handles for Analog Dial
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(DialTickMarks)
    ON_SLOT(1, setSlotLength, Basic::Number)
    ON_SLOT(2, setSlotQuantity, Basic::Number)
    ON_SLOT(3, setTickGraphic, BasicGL::Graphic)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
DialTickMarks::DialTickMarks()
{
    STANDARD_CONSTRUCTOR()
    length = 1;
    quantity = 1;
    myGraphic = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DialTickMarks::copyData(const DialTickMarks& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) myGraphic = 0;

    length = org.length;
    quantity = org.quantity;
        setTickGraphic(org.myGraphic);
    }

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void DialTickMarks::deleteData()
{
    if (myGraphic != 0) myGraphic->unref();
    myGraphic = 0;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void DialTickMarks::drawFunc()
{
    GLfloat currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);
    
    // get our data from our baseclass (AnalogDial)
    LCreal sweepAngle = getSweepAngle();
    LCreal startAngle = getStartAngle();
    LCreal tRadius = getRadius();

    // figure our rotation angle per tick mark (deg)
    LCreal rotation = sweepAngle/quantity;
    
    glPushMatrix();
        // rotate to our start angle
        glRotatef(GLfloat(-startAngle), 0, 0, 1);
        for (int i = 0; i < quantity + 1; i++) {  
            // if we have a graphic, draw that, else we draw a line
            if (myGraphic == 0) {      
                glBegin(GL_LINES);
                    lcVertex2(0, tRadius);
                    lcVertex2(0, tRadius - length);
                glEnd();
            }
            else {
                myGraphic->lcSaveMatrix();
                    myGraphic->lcTranslate(0, tRadius);
                    myGraphic->draw();
                myGraphic->lcRestoreMatrix();
            }
            glRotatef(GLfloat(-rotation), 0, 0, 1);
        }
    glPopMatrix();
    
    glColor4fv(currentColor);
}

// SLOT FUNCTIONS
//------------------------------------------------------------------------------
// setSlotLength() -- sets our tick mark length
//------------------------------------------------------------------------------
bool DialTickMarks::setSlotLength(const Basic::Number* const newLength)
{
    bool ok = true;
    if (newLength != 0) ok = setLength(newLength->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotQuantity() -- sets the number of tick marks
//------------------------------------------------------------------------------
bool DialTickMarks::setSlotQuantity(const Basic::Number* const newQ)
{
    bool ok = true;
    if (newQ != 0) ok = setQuantity(newQ->getInt());
    return ok;
}

// Set function
//------------------------------------------------------------------------------
// setTickGraphic() -- sets our graphic for using as tick marks
//------------------------------------------------------------------------------
bool DialTickMarks::setTickGraphic(const BasicGL::Graphic* const newGraphic)
{
    bool ok = true;
    if (myGraphic != 0) {
        myGraphic->unref();
        myGraphic = 0;
    }

    if (newGraphic != 0) {
        myGraphic = newGraphic->clone();
        myGraphic->container(this);
        ok = true;
    }
    return ok;
}
//------------------------------------------------------------------------------
// setLength() -- sets tick mark length
//------------------------------------------------------------------------------
bool DialTickMarks::setLength(const LCreal newLength)
{
    length = newLength;
    return true;
}
//------------------------------------------------------------------------------
// setQuantity() -- sets the number of tick marks
//------------------------------------------------------------------------------
bool DialTickMarks::setQuantity(const int newQ)
{
    quantity = newQ; 
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for DialTickMarks
//------------------------------------------------------------------------------
Basic::Object* DialTickMarks::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end Instruments namespace
}  // end Eaagles namespace
