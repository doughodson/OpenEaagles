
#include "openeaagles/graphics/readouts/Rotary2.h"



#include "openeaagles/graphics/Display.h"
#include "openeaagles/base/Float.h"
#include "openeaagles/base/Integer.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/util/str_utils.h"
#include "openeaagles/base/util/constants.h"

#include "ReformatScanner.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
//#if(_MSC_VER>=1400)   // VC8+
//# pragma warning(disable: 4996)
//#endif

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
