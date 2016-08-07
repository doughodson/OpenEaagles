
#include "openeaagles/graphics/readouts/Rotary.h"



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

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Rotary, "Rotary")
EMPTY_COPYDATA(Rotary)
EMPTY_DELETEDATA(Rotary)
EMPTY_SERIALIZER(Rotary)

Rotary::Rotary()
{
   STANDARD_CONSTRUCTOR()

   base::Integer* p = new base::Integer(1);    // default rotary item
   setSelectionName(p);
   p->unref();
   preDrawSelectList = true;
}

// Rotary - we do this "pre-draw" of all of our possible selections, that will
// eliminate the "flicker" on the first selection of the item, because we will have
// already drawn the item one time before (here).
void Rotary::draw()
{
   if (preDrawSelectList) {
      int start = 1;
      base::Pair* p = findByIndex(start);
      while (p != nullptr) {
         graphics::Graphic* g = dynamic_cast<graphics::Graphic*>(p->object());
         if (g != nullptr) g->draw();
         p = findByIndex(++start);
      }
      preDrawSelectList = false;
   }

   BaseClass::draw();
}

}
}
