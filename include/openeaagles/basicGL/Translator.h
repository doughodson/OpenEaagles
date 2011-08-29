//------------------------------------------------------------------------------
// Class:	Graphic -> Translator
//------------------------------------------------------------------------------
#ifndef	__Translator_H_ADDFFC31_ED9E_427c_95AD_886CEBD0EAE0
#define __Translator_H_ADDFFC31_ED9E_427c_95AD_886CEBD0EAE0

#include "Graphic.h"

namespace Eaagles {
   namespace Basic { class Number; }
namespace BasicGL {

//------------------------------------------------------------------------------
// Class:	Graphic -> Translator
//
// Description:  This is a basic graphic object which will translate any graphic
// given to it according to X and Y positioning.  Z is default to 0.0.
//
// Form name: Translator
// Inputs:
//      UPDATE_VALUE  -> moves x
//      UPDATE_VALUE2 -> moves y
//      UPDATE_VALUE3 -> moves z
//------------------------------------------------------------------------------
class Translator : public Graphic {
    DECLARE_SUBCLASS(Translator,Graphic)
public:
    Translator();

    LCreal getXPos() const        { return myXPos; }    // Returns the X position (world coord)
    LCreal getYPos() const        { return myYPos; }    // Returns the Y position (world coord)
    LCreal getZPos() const        { return myZPos; }    // Returns the Z position (world coord)

    // set functions
    bool setXPos(const LCreal xPos);                    // Sets the X position (world coord)
    bool setYPos(const LCreal yPos);                    // Sets the Y position (world coord)
    bool setZPos(const LCreal zPos);                    // Sets the Z position (world coord)
    
    // Graphic interface
    virtual void draw();
    
    // Component interface
    virtual bool event(const int event, Object* const obj = 0);  
    
private:
    // event functions
    bool onUpdateX(const Basic::Number* const newX);
    bool onUpdateY(const Basic::Number* const newY);
    bool onUpdateZ(const Basic::Number* const newZ);
    
    LCreal myXPos;
    LCreal myYPos;
    LCreal myZPos;
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif	/* __Translator_H_ADDFFC31_ED9E_427c_95AD_886CEBD0EAE0 */
