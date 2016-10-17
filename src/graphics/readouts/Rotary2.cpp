
#include "openeaagles/graphics/readouts/Rotary2.hpp"

#include "openeaagles/base/Integer.hpp"

namespace oe {
namespace graphics {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Rotary2, "Rotary2")
EMPTY_COPYDATA(Rotary2)
EMPTY_DELETEDATA(Rotary2)
EMPTY_SERIALIZER(Rotary2)

// Event handler for Rotary2 events
BEGIN_EVENT_HANDLER(Rotary2)
ON_EVENT_OBJ(SELECT, onSelect, base::Number)
END_EVENT_HANDLER()

Rotary2::Rotary2()
{
   STANDARD_CONSTRUCTOR()

   base::Integer* p = new base::Integer(1);    // default rotary item
   setSelectionName(p);
   p->unref();
}

//------------------------------------------------------------------------------
// Event functions
//------------------------------------------------------------------------------

// onSelect() - Macro function for Rotary2
bool Rotary2::onSelect(const base::Number* const osobj)
{
   if (osobj != nullptr) {
      if (osobj->getBoolean()){
         //if true, select the second component
         base::Integer two(2);
         select(&two);
      }
      else {
         //if false, select the first component
         base::Integer one(1);
         select(&one);
      }
   }
   return true;
}

}
}
