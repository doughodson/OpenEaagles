
#include "openeaagles/graphics/readouts/Cursor.h"

#include "openeaagles/graphics/Display.h"
#include "openeaagles/base/Float.h"
#include "openeaagles/base/Integer.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/util/str_utils.h"
#include "openeaagles/base/util/constants.h"

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

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Cursor, "Cursor")
EMPTY_COPYDATA(Cursor)
EMPTY_DELETEDATA(Cursor)
EMPTY_SERIALIZER(Cursor)

Cursor::Cursor()
{
   STANDARD_CONSTRUCTOR()

   width(1);
   setText(" ");
   setDisplayMode(reversed);
   setFlashRate(1.0f);
}

void Cursor::updateData(const double dt)
{
   int ln = 0;
   int cp = 0;
   if (getDisplay()->focus() != nullptr) {
      setVisibility( getDisplay()->focus()->cursor(&ln,&cp) );
   }
   else {
      setVisibility(false);
   }

   if (isVisible()) {
      line(ln);
      column(cp);
      BaseClass::updateData(dt);
   }
}

}
}
