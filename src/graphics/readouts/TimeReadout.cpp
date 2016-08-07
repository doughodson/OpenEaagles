
#include "openeaagles/graphics/readouts/TimeReadout.h"

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

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TimeReadout, "TimeReadout")
EMPTY_DELETEDATA(TimeReadout)
EMPTY_SERIALIZER(TimeReadout)

TimeReadout::TimeReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format, FORMAT_LENGTH, "%02d:%02d:%04.1f");
   tmode = hhmmss;
}

void TimeReadout::copyData(const TimeReadout& org, const bool)
{
   BaseClass::copyData(org);
   tmode = org.tmode;
}

//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
char TimeReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '0' || tc == 'H' || tc == 'M' || tc == 'S') {
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
double TimeReadout::getInputValue() const
{
   double value = 0.0;

   // copy string to buffer with correct sign character
   const size_t CBUFLOCAL_LEN = 100;
   char cbuf[CBUFLOCAL_LEN];
   const char* p = *this;
   base::utStrcpy(cbuf,CBUFLOCAL_LEN,p);
   if (cbuf[0] == plusChar)  cbuf[0] = '+';
   if (cbuf[0] == minusChar) cbuf[0] = '-';

   // Modify the output format statement for use with the sscanf
   char format1[32];
   int i = 0;
   int j = 0;
   bool skipIt = false;
   for (i = 0;  i < 31 && format[i] != '\0'; i++) {
      if (skipIt && format[i] == 'f') skipIt = false;
      if (format[i] == '.') {
         skipIt = true;
      }
      else if ((format[i] != '+' && format[i] != '-' && format[i] != '0')
         && !skipIt) {
            format1[j++] = format[i];
      }
   }
   format1[j] = '\0';

   switch (tmode) {

      case hhmmss : {        // Hours, Minutes, and seconds
         int   hrs = 0;
         int   min = 0;
         float sec = 0.0f;
         std::sscanf(cbuf, format1, &hrs, &min, &sec);
         value = hrs*60.0f;
         if (value >= 0.0) value += min;
         else value -= min;
         value *= 60.0;
         if (value >= 0.0) value += sec;
         else value -= sec;
      }
      break;

      case hhmm : { // Hours and minutes
         int   hrs = 0;
         float min = 0.0f;
         std::sscanf(cbuf, format1, &hrs, &min);
         value = hrs*60.0f;
         if (value >= 0.0) value += min;
         else value -= min;
         value *= 60.0;
      }
      break;

      case hh : {   // Hours only
         float hrs = 0.0f;
         std::sscanf(cbuf, format1, &hrs);
         value = hrs*3600.0f;
      }
      break;

      case mmss : { // Minutes and seconds
         int   min = 0;
         float sec = 0.0f;
         std::sscanf(cbuf, format1, &min, &sec);
         value = min*60.0f;
         if (value >= 0.0) value += sec;
         else value -= sec;
      }
      break;

      case mm : {   // Minutes only
         float min = 0.0;
         std::sscanf(cbuf, format1, &min);
         value = min*60.0f;
      }
      break;

      case ss : {   // Seconds only
         float sec = 0.0;
         std::sscanf(cbuf, format1, &sec);
         value = sec;
      }
      break;
   }
   return static_cast<double>(value);
}

//------------------------------------------------------------------------------
//  makeText() -- make the text string use the current value and formats
//------------------------------------------------------------------------------
void TimeReadout::makeText()
{
   bool neg = false;
   double seconds = getFloat();
   if (seconds < 0.0) {
      seconds = -seconds;
      neg = true;
   }

   switch (tmode) {

      case hhmmss : { // Hours, Minutes, and seconds
         double minutes = seconds/60.0f;
         int ihrs = static_cast<int>(minutes/60.0f);
         double min = minutes - static_cast<double>(ihrs*60);
         int imin = static_cast<int>(min);
         double sec = (min - static_cast<double>(imin))*60.0f;
         std::sprintf(cbuf, format, ihrs, imin, sec);
         if (neg) { /* if it was negative, swap the possible + sign to the - sign */
            bool done = false;
            for (unsigned int i = 0; !done && i < CBUF_LENGTH; i++) {
               if (cbuf[i] == '+') { cbuf[i] = '-'; done = true; }
               else if (cbuf[i] == '\0') { done = true; }
            }
         }
      }
      break;

      case hhmm : {   // Hours and minutes
         double minutes = seconds/60.0f;
         int  ihrs = static_cast<int>(minutes/60.0f);
         double min = minutes - static_cast<double>(ihrs*60);
         std::sprintf(cbuf, format, ihrs, min);
         if (neg) { /* if it was negative, swap the possible + sign to the - sign */
            bool done = false;
            for (unsigned int i = 0; !done && i < CBUF_LENGTH; i++) {
               if (cbuf[i] == '+') { cbuf[i] = '-'; done = true; }
               else if (cbuf[i] == '\0') { done = true; }
            }
         }
      }
      break;

      case hh : { // Hours only
         double hrs = getFloat()/3600.0f;
         if (neg) hrs = -hrs;
         std::sprintf(cbuf, format, hrs);
      }
      break;

      case mmss : {   // Minutes and seconds
         int  imin = static_cast<int>(seconds/60.0f);
         double sec = seconds - static_cast<double>(imin*60);
         std::sprintf(cbuf, format, imin, sec);
         if (neg) { /* if it was negative, swap the possible + sign to the - sign */
            bool done = false;
            for (unsigned int i = 0; !done && i < CBUF_LENGTH; i++) {
               if (cbuf[i] == '+') { cbuf[i] = '-'; done = true; }
               else if (cbuf[i] == '\0') { done = true; }
            }
         }
      }
      break;

      case mm : { // Minutes only
         double min = seconds/60.0f;
         if (neg) min = -min;
         std::sprintf(cbuf, format, min);
      }
      break;

      case ss : { // Seconds only
         if (neg) seconds = -seconds;
         std::sprintf(cbuf, format, seconds);
      }
      break;
   }
}

//------------------------------------------------------------------------------
// reformat() -- convert the numerical value into an ascii character string
//------------------------------------------------------------------------------
void TimeReadout::reformat(const char* const example)
{
   TimeMode results = reformatter->convertTime(example);
   if (results != invalid) {
      setExample(example);
      base::utStrcpy(format,FORMAT_LENGTH,reformatter->getFormat());
      tmode = results;
      postSign = reformatter->isPostSign();
      redisplay();
   }
}

}
}
