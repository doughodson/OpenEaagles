
// fields
#include "openeaagles/graphics/readouts/AsciiText.h"
#include "openeaagles/graphics/readouts/Cursor.h"
#include "openeaagles/graphics/readouts/Rotary.h"
#include "openeaagles/graphics/readouts/Rotary2.h"

// readouts
#include "openeaagles/graphics/readouts/DirectionReadout.h"
#include "openeaagles/graphics/readouts/HexReadout.h"
#include "openeaagles/graphics/readouts/LatitudeReadout.h"
#include "openeaagles/graphics/readouts/LongitudeReadout.h"
#include "openeaagles/graphics/readouts/NumericReadout.h"
#include "openeaagles/graphics/readouts/OctalReadout.h"
#include "openeaagles/graphics/readouts/TimeReadout.h"



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
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace graphics {

//==============================================================================
// class HexReadout
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(HexReadout,"HexReadout")
EMPTY_COPYDATA(HexReadout)
EMPTY_DELETEDATA(HexReadout)
EMPTY_SERIALIZER(HexReadout)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
HexReadout::HexReadout()
{
   STANDARD_CONSTRUCTOR()
   base::utStrcpy(format,FORMAT_LENGTH,"%X");
}

//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
char HexReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '0' || tc == '#') {
      // Default numeric keys
      if ( (event >= '0' && event <= '9') || (event >= 'A' && event <= 'F') )
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
double HexReadout::getInputValue() const
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
void HexReadout::makeText()
{
   std::sprintf(cbuf, format, getInt());
}

//------------------------------------------------------------------------------
// reformat() -- convert the numerical value into an ascii character string
//------------------------------------------------------------------------------
void HexReadout::reformat(const char* const example)
{
   if (reformatter->convertHex(example) != ReformatScanner::DataType::invalid) {
      setExample(example);
      base::utStrcpy(format,FORMAT_LENGTH,reformatter->getFormat());
      postSign = reformatter->isPostSign();
      redisplay();
   }
}


//==============================================================================
// class OctalReadout
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(OctalReadout,"OctalReadout")
EMPTY_COPYDATA(OctalReadout)
EMPTY_DELETEDATA(OctalReadout)
EMPTY_SERIALIZER(OctalReadout)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
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


//==============================================================================
// class TimeReadout
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TimeReadout,"TimeReadout")
EMPTY_DELETEDATA(TimeReadout)
EMPTY_SERIALIZER(TimeReadout)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
TimeReadout::TimeReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format,FORMAT_LENGTH,"%02d:%02d:%04.1f");
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


//==============================================================================
// class DirectionReadout
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DirectionReadout,"DirectionReadout")
EMPTY_DELETEDATA(DirectionReadout)
EMPTY_SERIALIZER(DirectionReadout)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
DirectionReadout::DirectionReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format,FORMAT_LENGTH,"%+07.2f");
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
      base::utStrcpy(format,FORMAT_LENGTH,reformatter->getFormat());
      tmode = results;
      postSign = reformatter->isPostSign();
      redisplay();
   }
}


//==============================================================================
// class LatitudeReadout
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(LatitudeReadout,"LatitudeReadout")
EMPTY_COPYDATA(LatitudeReadout)
EMPTY_DELETEDATA(LatitudeReadout)
EMPTY_SERIALIZER(LatitudeReadout)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
LatitudeReadout::LatitudeReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format,FORMAT_LENGTH,"%+03d@%04.1f");
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
      return BaseClass::filterInputEvent(event,tc);
   }
}


//==============================================================================
// class LongitudeReadout
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(LongitudeReadout,"LongitudeReadout")
EMPTY_COPYDATA(LongitudeReadout)
EMPTY_DELETEDATA(LongitudeReadout)
EMPTY_SERIALIZER(LongitudeReadout)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
LongitudeReadout::LongitudeReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format,FORMAT_LENGTH,"%+04d@%04.1f");
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
      return BaseClass::filterInputEvent(event,tc);
   }
}

//==============================================================================
// class Rotary
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Rotary,"Rotary")
EMPTY_COPYDATA(Rotary)
EMPTY_DELETEDATA(Rotary)
EMPTY_SERIALIZER(Rotary)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Rotary::Rotary()
{
   STANDARD_CONSTRUCTOR()

   base::Integer* p = new base::Integer(1);    // default rotary item
   setSelectionName(p);
   p->unref();
   preDrawSelectList = true;
}

//------------------------------------------------------------------------------
// draw() -- draw our graphics
//------------------------------------------------------------------------------

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


//==============================================================================
// class Rotary2
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Rotary2,"Rotary2")
EMPTY_COPYDATA(Rotary2)
EMPTY_DELETEDATA(Rotary2)
EMPTY_SERIALIZER(Rotary2)

// Event handler for Rotary2 events
BEGIN_EVENT_HANDLER(Rotary2)
ON_EVENT_OBJ(SELECT,onSelect,base::Number)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
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
