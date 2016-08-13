
#include "openeaagles/graphics/readouts/DirectionReadout.h"

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

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DirectionReadout,"DirectionReadout")
EMPTY_DELETEDATA(DirectionReadout)
EMPTY_SERIALIZER(DirectionReadout)

DirectionReadout::DirectionReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format, FORMAT_LENGTH, "%+07.2f");
   tmode = dd;
}

void DirectionReadout::copyData(const DirectionReadout& org, const bool)
{
   BaseClass::copyData(org);
   tmode = org.tmode;
}

//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
char DirectionReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '+') {
      // sign keys
      if (event == '4' && minusChar != '\0')
         return minusChar;
      else if (event == '7')
         return '-';
      else if (event == '6' && plusChar != '\0')
         return plusChar;
      else if (event == '9')
         return '+';
      else
         return '\0';
   }
   else if (tc == '0' || tc == 'D' || tc == 'M' || tc == 'S') {
      // Default numeric keys
      if ( event >= '0' && event <= '9' )
         return char(event);
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
double DirectionReadout::getInputValue() const
{
   double value = 0.0;

   // copy string to buffer with correct sign character
   const size_t CBUFLOCAL_LEN = 100;
   char cbuf[CBUFLOCAL_LEN];
   const char* p = *this;
   base::utStrcpy(cbuf,CBUFLOCAL_LEN,p);
   if (cbuf[0] == plusChar)  cbuf[0] = '+';
   if (cbuf[0] == minusChar) cbuf[0] = '-';

   switch (tmode) {

      case ddmmss : {   // Degrees, Minutes, and seconds
         double degs = 0.0;
         double min = 0.0;
         double sec = 0.0;
         std::sscanf(cbuf, "%lf@%lf'%lf", &degs, &min, &sec);
         min += sec/60.0f;
         if (degs >= 0.0) value = degs + min/60.0f;
         else value = degs - min/60.0f;
      }
      break;

      case ddmm : { // Degrees and minutes
         double degs = 0.0;
         double min = 0.0;
         std::sscanf(cbuf, "%lf@%lf", &degs, &min);
         if (degs >= 0.0) value = degs + min/60.0f;
         else value = degs - min/60.0f;
      }
      break;

      case dd : {   // Degrees only
         std::sscanf(cbuf, "%lf", &value);
      }
      break;
   }
   return static_cast<double>(value);
}

//------------------------------------------------------------------------------
//  makeText() -- make the text string use the current value and formats
//------------------------------------------------------------------------------
void DirectionReadout::makeText()
{
   bool neg = false;
   double degrees = getFloat();
   if (degrees < 0.0) {
      degrees = -degrees;
      neg = true;
   }

   switch (tmode) {

      case ddmmss : { // Degrees, Minutes, and seconds
         int     ideg = static_cast<int>(degrees);
         double  min = (degrees - static_cast<double>(ideg))*60.0f;
         int     imin = static_cast<int>(min);
         double sec = (min - static_cast<double>(imin))*60.0f;
         if (neg) ideg = -ideg;
         std::sprintf(cbuf, format, ideg, imin, sec);
      }
      break;

      case ddmm : {   // Degrees and minutes
         int     ideg = static_cast<int>(degrees);
         double  min  = (degrees - static_cast<double>(ideg))*60.0f;
         if (neg) ideg = -ideg;
         std::sprintf(cbuf, format, ideg, min);
      }
      break;

      case dd : { // Degrees only
         if (neg) degrees = -degrees;
         std::sprintf(cbuf, format, degrees);
      }
      break;

   }

   // then turn any '@' characters to degree symbols.
   {
      size_t len = std::strlen(cbuf);
      for (unsigned int i = 0; i < len; i++) {
         if (cbuf[i] == '@') cbuf[i] = static_cast<char>(0xB0);
      }
   }
}

//------------------------------------------------------------------------------
// reformat() -- convert the numerical value into an ascii character string
//------------------------------------------------------------------------------
void DirectionReadout::reformat(const char* const example)
{
   DirMode results = reformatter->convertDirection(example);
   if (results != invalid) {
      setExample(example);
      base::utStrcpy(format, FORMAT_LENGTH, reformatter->getFormat());
      tmode = results;
      postSign = reformatter->isPostSign();
      redisplay();
   }
}

}
}
