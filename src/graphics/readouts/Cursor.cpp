
#include "openeaagles/graphics/readouts/Cursor.h"

#include "openeaagles/graphics/Display.h"

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
      setVisibility( getDisplay()->focus()->cursor(&ln, &cp) );
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
