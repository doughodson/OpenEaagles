
#ifndef __oe_graphics_Cursor_H__
#define __oe_graphics_Cursor_H__

#include "openeaagles/graphics/Field.h"

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Cursor
//
// Factory name: Cursor
//------------------------------------------------------------------------------
class Cursor : public Field
{
    DECLARE_SUBCLASS(Cursor, Field)

public:
   Cursor();
   virtual void updateData(const double dt = 0.0) override;
};

}
}

#endif
