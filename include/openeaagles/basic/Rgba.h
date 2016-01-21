//------------------------------------------------------------------------------
// Class:  Rgba
//------------------------------------------------------------------------------
#ifndef __oe_basic_Rgba_H__
#define __oe_basic_Rgba_H__

#include "openeaagles/basic/Rgb.h"

namespace oe {
namespace basic {

//------------------------------------------------------------------------------
// Class:  Rgba
// Base class:  Object -> Color -> Rgb -> Rgba
//
// Description:  Defines a color by Red, Green, Blue and Alpha
//
// Factory name: rgba
// Slots:
//    alpha <Number>  ! Alpha component (0.0 to 1.0) (default: 1.0)
//
// Events: None
//
//
// Public methods: Base class public methods, plus ...
//
//      Rgba(const LCreal r, const LCreal g, const LCreal b, const LCreal a)
//          Special constructor what initializes the object to r, g, b and a
//
//------------------------------------------------------------------------------
class Rgba : public Rgb {
    DECLARE_SUBCLASS(Rgba,Rgb)

public:
    Rgba(const LCreal r, const LCreal g, const LCreal b, const LCreal a);
    Rgba();

};

} // End basic namespace
} // End oe namespace

#endif
