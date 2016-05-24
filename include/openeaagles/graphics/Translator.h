//------------------------------------------------------------------------------
// Class: Translator
//------------------------------------------------------------------------------
#ifndef __oe_graphics_Translator_H__
#define __oe_graphics_Translator_H__

#include "Graphic.h"

namespace oe {
   namespace base { class Number; }
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

    double getXPos() const        { return myXPos; }    // Returns the X position (world coord)
    double getYPos() const        { return myYPos; }    // Returns the Y position (world coord)
    double getZPos() const        { return myZPos; }    // Returns the Z position (world coord)

    bool setXPos(const double xPos);                    // Sets the X position (world coord)
    bool setYPos(const double yPos);                    // Sets the Y position (world coord)
    bool setZPos(const double zPos);                    // Sets the Z position (world coord)

    virtual void draw() override;

    virtual bool event(const int event, Object* const obj = nullptr) override;

private:
    // event functions
    bool onUpdateX(const base::Number* const newX);
    bool onUpdateY(const base::Number* const newY);
    bool onUpdateZ(const base::Number* const newZ);

    double myXPos;
    double myYPos;
    double myZPos;
};

}
}

#endif
