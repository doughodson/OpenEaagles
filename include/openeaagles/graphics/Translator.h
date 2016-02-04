//------------------------------------------------------------------------------
// Class: Translator
//------------------------------------------------------------------------------
#ifndef __oe_graphics_Translator_H__
#define __oe_graphics_Translator_H__

#include "Graphic.h"

namespace oe {
   namespace basic { class Number; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: Translator
//
// Description:  This is a basic graphic object which will translate any graphic
// given to it according to X and Y positioning.  Z is default to 0.0.
//
// Factory name: Translator
// Inputs:
//      UPDATE_VALUE  -> moves x
//      UPDATE_VALUE2 -> moves y
//      UPDATE_VALUE3 -> moves z
//------------------------------------------------------------------------------
class Translator : public Graphic
{
    DECLARE_SUBCLASS(Translator,Graphic)
public:
    Translator();

    LCreal getXPos() const        { return myXPos; }    // Returns the X position (world coord)
    LCreal getYPos() const        { return myYPos; }    // Returns the Y position (world coord)
    LCreal getZPos() const        { return myZPos; }    // Returns the Z position (world coord)

    bool setXPos(const LCreal xPos);                    // Sets the X position (world coord)
    bool setYPos(const LCreal yPos);                    // Sets the Y position (world coord)
    bool setZPos(const LCreal zPos);                    // Sets the Z position (world coord)

    void draw() override;

    bool event(const int event, Object* const obj = nullptr) override;

private:
    // event functions
    bool onUpdateX(const basic::Number* const newX);
    bool onUpdateY(const basic::Number* const newY);
    bool onUpdateZ(const basic::Number* const newZ);

    LCreal myXPos;
    LCreal myYPos;
    LCreal myZPos;
};

} // End graphics namespace
} // End oe namespace

#endif
