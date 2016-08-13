
#include "openeaagles/graphics/readouts/LongitudeReadout.h"

#include "openeaagles/base/util/str_utils.h"
#include "openeaagles/graphics/readouts/ReformatScanner.h"

namespace oe {
namespace graphics {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(LongitudeReadout, "LongitudeReadout")
EMPTY_COPYDATA(LongitudeReadout)
EMPTY_DELETEDATA(LongitudeReadout)
EMPTY_SERIALIZER(LongitudeReadout)

LongitudeReadout::LongitudeReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format, FORMAT_LENGTH, "%+04d@%04.1f");
   tmode = ddmm;
   plusChar = 'E';
   minusChar = 'W';
   postSign = false;
}


//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
char LongitudeReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '+') {
      // sign keys
      if (event == '4')
         return 'W';
      else if (event == '6')
         return 'E';
      else
         return '\0';
   }
   else {
      return BaseClass::filterInputEvent(event, tc);
   }
}

}
}
