//------------------------------------------------------------------------------
// Class:  Rgb
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Rgb_H__
#define __Eaagles_Basic_Rgb_H__

#include "openeaagles/basic/Color.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:  Rgb
// Base class:  Object -> Color -> Rgb
//
// Description:  Defines a color by Red, Green, and Blue
//
// Factory name: rgb
// Slots:
//     red   <Number>  ! Red component (0.0 to 1.0) (default: 0)
//     green <Number>  ! Green component (0.0 to 1.0) (default: 0)
//     blue  <Number>  ! Blue component (0.0 to 1.0) (default: 0)
//
// Events: None
//
//
// Public methods: Base class public methods, plus ...
//
//      Rgb(const LCreal r, const LCreal g, const LCreal b)
//          Special constructor what initializes the object to r, g, and b
//
//------------------------------------------------------------------------------
class Rgb : public Color {
    DECLARE_SUBCLASS(Rgb,Color)

public:
    Rgb(const LCreal r, const LCreal g, const LCreal b);
    Rgb();

    // Slot methods
    virtual bool setSlotRed(Number* const msg);
    virtual bool setSlotGreen(Number* const msg);
    virtual bool setSlotBlue(Number* const msg);
    virtual bool setSlotAlpha(Number* const msg);
};

} // End Basic namespace
} // End Eaagles namespace

#endif
