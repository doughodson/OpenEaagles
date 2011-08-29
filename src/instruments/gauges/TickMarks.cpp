#include "openeaagles/instruments/gauges/TickMarks.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Instruments {

IMPLEMENT_SUBCLASS(TickMarks, "TickMarks")
EMPTY_SERIALIZER(TickMarks)

BEGIN_SLOTTABLE(TickMarks)
    "tickLength",       // length of the tick marks
    "quantity",         // tells us how many to draw, default is 1
    "gaugeLength",      // how far are we stretching these ticks marks out?
    "flip",             // If this is set, we will flip our tick lines (ie.. if we are horizontal, we will draw our lines horizontal, etc...)
    "tickGraphic",      // sets a graphic to use instead of a line    
END_SLOTTABLE(TickMarks)       

//------------------------------------------------------------------------------
//  Map slot table to handles for Analog Dial
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(TickMarks)
    ON_SLOT(1, setSlotTickMarkLength, Basic::Number)
    ON_SLOT(2, setSlotQuantity, Basic::Number)
    ON_SLOT(3, setSlotGaugeLength, Basic::Number)
    ON_SLOT(4, setSlotFlip, Basic::Number)
    ON_SLOT(5, setTickGraphic, BasicGL::Graphic)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
TickMarks::TickMarks()
{
    STANDARD_CONSTRUCTOR()
    lengthTM = 1;
    quantity = 1;
    gaugeLength = 0;
    flip = false;
    myGraphic = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TickMarks::copyData(const TickMarks& org, const bool cc)
{
    BaseClass::copyData(org);
    
    if (cc) myGraphic = 0;

    setTickGraphic(org.myGraphic);
    lengthTM = org.lengthTM;
    quantity = org.quantity;
    gaugeLength = org.gaugeLength;
    flip = org.flip;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void TickMarks::deleteData()
{
    if (myGraphic != 0) {
        myGraphic->unref();
        myGraphic = 0;
    }
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void TickMarks::drawFunc()
{
    GLfloat currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);
    
    // determine our tick mark spacing (make sure our quantity != 0) 
    LCreal spacing = 0;
    if (quantity != 1) spacing = gaugeLength/(quantity-1);
    // if we don't have any, return
    else if (quantity == 0) return;
        
    glPushMatrix();
        // if we are vertical, draw bottom to top, else draw left to right
        if (isVertical()) {
            for (int i = 0; i < quantity; i++) {  
               if (myGraphic == 0) {
                    glBegin(GL_LINES);
                        glVertex2f(0, 0);
                        if (flip) lcVertex2(0, lengthTM);
                        else lcVertex2(lengthTM, 0);
                    glEnd();
               }
               else {
                  myGraphic->lcSaveMatrix();
                     myGraphic->draw();
                  myGraphic->lcRestoreMatrix();
               }
                // increment our spacing
                glTranslated(0, GLdouble(spacing), 0);
            }
        }
        else {
            for (int i = 0; i < quantity; i++) {  
               if (myGraphic == 0) {
                    glBegin(GL_LINES);
                        glVertex2f(0, 0);
                        if (flip) lcVertex2(lengthTM, 0);
                        else lcVertex2(0, lengthTM);
                    glEnd();
               }
               else {
                  myGraphic->lcSaveMatrix();
                     myGraphic->draw();
                  myGraphic->lcRestoreMatrix();
              }
                // increment our spacing
                glTranslated(GLdouble(spacing), 0, 0);
            }
        }
    glPopMatrix();
    
    glColor4fv(currentColor);
}

// SLOT FUNCTIONS
//------------------------------------------------------------------------------
// setSlotTickMarkLength() -- sets our tick mark length
//------------------------------------------------------------------------------
bool TickMarks::setSlotTickMarkLength(const Basic::Number* const newLength)
{
    bool ok = true;
    if (newLength != 0) ok = setTickMarkLength(newLength->getReal());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotQuantity() -- sets the number of tick marks
//------------------------------------------------------------------------------
bool TickMarks::setSlotQuantity(const Basic::Number* const newQ)
{
    bool ok = true;
    if (newQ != 0) ok = setQuantity(newQ->getInt());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotGaugeLength() 
//------------------------------------------------------------------------------
bool TickMarks::setSlotGaugeLength(const Basic::Number* const newL)
{
    bool ok = true;
    if (newL != 0) ok = setGaugeLength(newL->getReal());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotFlit() 
//------------------------------------------------------------------------------
bool TickMarks::setSlotFlip(const Basic::Number* const x)
{
    bool ok = true;
    if (x != 0) ok = setFlip(x->getBoolean());
    return ok;
}
//------------------------------------------------------------------------------
// setTickGraphic() -- sets our graphic for using as tick marks
//------------------------------------------------------------------------------
bool TickMarks::setTickGraphic(const BasicGL::Graphic* const newGraphic)
{
    bool ok = true;
    if (myGraphic != 0) {
        myGraphic->unref();
        myGraphic = 0;
    }
    if (newGraphic != 0) {
        myGraphic = (BasicGL::Graphic*) newGraphic->clone();
        myGraphic->container(this);
        ok = true;
    }
    return ok;
}


// Set function
//------------------------------------------------------------------------------
// setTickMarkLength() -- sets tick mark length
//------------------------------------------------------------------------------
bool TickMarks::setTickMarkLength(const LCreal newLength)
{
    lengthTM = newLength;
    return true;
}
//------------------------------------------------------------------------------
// setQuantity() -- sets the number of tick marks
//------------------------------------------------------------------------------
bool TickMarks::setQuantity(const int newQ)
{
    quantity = newQ; 
    return true;
}
//------------------------------------------------------------------------------
// setGaugeLength() -- sets our span to draw tick marks over
//------------------------------------------------------------------------------
bool TickMarks::setGaugeLength(const LCreal newL)
{
    gaugeLength = newL; 
    return true;
}
//------------------------------------------------------------------------------
// setFlip() -- sets our flip variables
//------------------------------------------------------------------------------
bool TickMarks::setFlip(const bool x)
{
    flip = x; 
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for TickMarks
//------------------------------------------------------------------------------
Basic::Object* TickMarks::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end Instruments namespace
}  // end Eaagles namespace
