//------------------------------------------------------------------------------
// Class: Translator
//------------------------------------------------------------------------------
#include "openeaagles/basicGL/Translator.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Translator, "Translator")
EMPTY_SERIALIZER(Translator)
EMPTY_DELETEDATA(Translator)

// -----------------------------------------------------------------------------
// Event Handler (s)
// -----------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Translator)
    ON_EVENT_OBJ(UPDATE_VALUE, onUpdateX, Basic::Number)
    ON_EVENT_OBJ(UPDATE_VALUE2, onUpdateY, Basic::Number)
    ON_EVENT_OBJ(UPDATE_VALUE3, onUpdateZ, Basic::Number)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Translator::Translator()
{
    STANDARD_CONSTRUCTOR()

    myXPos = 0.0;
    myYPos = 0.0;
    myZPos = 0.0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Translator::copyData(const Translator& org, const bool)
{
    BaseClass::copyData(org);

    myXPos = org.myXPos;
    myYPos = org.myYPos;
    myZPos = org.myZPos;
}

//------------------------------------------------------------------------------
// draw() -- draws the object(s)
//------------------------------------------------------------------------------
void Translator::draw()
{
    // we are just translating here
    glPushMatrix();
        glTranslated(static_cast<double>(myXPos), static_cast<double>(myYPos), static_cast<double>(myZPos));
        Graphic::draw();
    glPopMatrix();
}

// EVENT functions
//------------------------------------------------------------------------------
// onUpdateX() - update our x position (inches)
//------------------------------------------------------------------------------
bool Translator::onUpdateX(const Basic::Number* const newX)
{
    bool ok = false;
    if (newX != nullptr) ok = setXPos(newX->getReal());
    return ok;
}
//------------------------------------------------------------------------------
// onUpdateY() - update our y position (inches)
//------------------------------------------------------------------------------
bool Translator::onUpdateY(const Basic::Number* const newY)
{
    bool ok = false;
    if (newY != nullptr) ok = setYPos(newY->getReal());
    return ok;
}
//------------------------------------------------------------------------------
// onUpdateY() - update our z position (inches)
//------------------------------------------------------------------------------
bool Translator::onUpdateZ(const Basic::Number* const newZ)
{
    bool ok = false;
    if (newZ != nullptr) ok = setZPos(newZ->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// functions for setting member variables
//------------------------------------------------------------------------------
bool Translator::setXPos(const LCreal xPos)
{
    myXPos = xPos;
    return true;
}

bool Translator::setYPos(const LCreal yPos)
{
    myYPos = yPos;
    return true;
}

bool Translator::setZPos(const LCreal zPos)
{
    myZPos = zPos;
    return true;
}

} // End BasicGL namespace
} // End Eaagles namespace
