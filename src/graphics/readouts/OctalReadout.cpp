
#include "openeaagles/graphics/readouts/OctalReadout.h"

#include "openeaagles/base/util/str_utils.h"

#include "ReformatScanner.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace graphics {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(OctalReadout, "OctalReadout")
EMPTY_COPYDATA(OctalReadout)
EMPTY_DELETEDATA(OctalReadout)
EMPTY_SERIALIZER(OctalReadout)

OctalReadout::OctalReadout()
{
   STANDARD_CONSTRUCTOR()
   base::utStrcpy(format,FORMAT_LENGTH,"%o");
}

//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
char OctalReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '0' || tc == '#') {
      // Default numeric keys
      if ( event >= '0' && event <= '7' )
         return static_cast<char>(event);
      else
         return '\0';
   }
   else {
      return BaseClass::filterInputEvent(event,tc);
   }
}

//------------------------------------------------------------------------------
// getInputValue() -- returns the readout as a numeric value
//------------------------------------------------------------------------------
double OctalReadout::getInputValue() const
{
   int value = 0;

   // copy string to buffer with correct sign character
   const size_t CBUFLOCAL_LEN = 100;
   char cbuf[CBUFLOCAL_LEN];
   const char* p = *this;
   base::utStrcpy(cbuf,CBUFLOCAL_LEN,p);
   if (cbuf[0] == plusChar)  cbuf[0] = '+';
   if (cbuf[0] == minusChar) cbuf[0] = '-';

   std::sscanf(cbuf, format, &value);
   return static_cast<double>(value);
}

//------------------------------------------------------------------------------
//  makeText() -- make the text string use the current value and formats
//------------------------------------------------------------------------------
void OctalReadout::makeText()
{
   std::sprintf(cbuf, format, getInt());
}

//------------------------------------------------------------------------------
// reformat() -- convert the numerical value into an ascii character string
//------------------------------------------------------------------------------
void OctalReadout::reformat(const char* const example)
{
   if (reformatter->convertOctal(example) != ReformatScanner::DataType::invalid) {
      setExample(example);
      base::utStrcpy(format,FORMAT_LENGTH,reformatter->getFormat());
      postSign = reformatter->isPostSign();
      redisplay();
   }
}

}
}
