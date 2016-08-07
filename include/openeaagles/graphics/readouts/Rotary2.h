
#ifndef __oe_graphics_Rotary2_H__
#define __oe_graphics_Rotary2_H__

#include "Rotary.h"

namespace oe {
namespace base { class Object; class Number; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: Rotary2
// Base class: Object > Graphic > Field > Rotary > Rotary2
//
// Description: Binary rotary
//
// Factory name: Rotary2
//
//------------------------------------------------------------------------------
class Rotary2 : public Rotary
{
    DECLARE_SUBCLASS(Rotary2, Rotary)

public:
   Rotary2();
   virtual bool event(const int key, base::Object* const obj = nullptr) override;
   //macro function for event handler
   virtual bool onSelect(const base::Number* const osobj);
};

}
}

#endif
