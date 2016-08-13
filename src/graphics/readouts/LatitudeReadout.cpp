
#include "openeaagles/graphics/readouts/LatitudeReadout.h"

#include "openeaagles/base/util/str_utils.h"
#include "openeaagles/graphics/readouts/ReformatScanner.h"

namespace oe {
namespace graphics {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(LatitudeReadout, "LatitudeReadout")
EMPTY_COPYDATA(LatitudeReadout)
EMPTY_DELETEDATA(LatitudeReadout)
EMPTY_SERIALIZER(LatitudeReadout)

LatitudeReadout::LatitudeReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format, FORMAT_LENGTH, "%+03d@%04.1f");
   tmode = ddmm;
   plusChar = 'N';
   minusChar = 'S';
   postSign = false;
}

//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
char LatitudeReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '+') {
      // sign keys
      if (event == '8')
         return 'S';
      else if (event == '2')
         return 'N';
      else
         return '\0';
   }
   else {
      return BaseClass::filterInputEvent(event, tc);
   }
}

}
}
