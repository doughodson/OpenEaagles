
#ifndef __oe_base_Rgb_H__
#define __oe_base_Rgb_H__

#include "openeaagles/base/Color.hpp"

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class:  Rgb
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
//      Rgb(const double r, const double g, const double b)
//          Special constructor what initializes the object to r, g, and b
//
//------------------------------------------------------------------------------
class Rgb : public Color
{
    DECLARE_SUBCLASS(Rgb, Color)

public:
    Rgb(const double r, const double g, const double b);
    Rgb();

    // Slot methods
    virtual bool setSlotRed(Number* const msg);
    virtual bool setSlotGreen(Number* const msg);
    virtual bool setSlotBlue(Number* const msg);
    virtual bool setSlotAlpha(Number* const msg);
};

}
}

#endif
