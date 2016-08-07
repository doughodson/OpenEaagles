
#ifndef __oe_graphics_Rotary_H__
#define __oe_graphics_Rotary_H__

#include "openeaagles/graphics/Field.h"

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Rotary
// Base class: Object > Graphic > Field > Rotary
//
// Description: Rotary text
//
// Factory name: Rotary
//
//------------------------------------------------------------------------------
class Rotary : public Field
{
    DECLARE_SUBCLASS(Rotary, Field)

public:
   Rotary();
   virtual void draw() override;

private:
    // this flag tells us our components need to be pre-drawn (to avoid flicker)
    bool preDrawSelectList;
};

}
}

#endif
