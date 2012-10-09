
#include "openeaagles/basicGL/Readouts.h"
#include "openeaagles/basicGL/Display.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Pair.h"
#include "Reformat.h"

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_SUBCLASS(AsciiText,"AsciiText")

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Cursor,"Cursor")
EMPTY_SERIALIZER(Cursor)
    
IMPLEMENT_SUBCLASS(NumericReadout,"NumericReadout")

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(HexReadout,"HexReadout")
EMPTY_SERIALIZER(HexReadout)

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(OctalReadout,"OctalReadout")
EMPTY_SERIALIZER(OctalReadout)

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TimeReadout,"TimeReadout")
EMPTY_SERIALIZER(TimeReadout)

IMPLEMENT_SUBCLASS(DirectionReadout,"DirectionReadout")
EMPTY_SERIALIZER(DirectionReadout)

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(LatitudeReadout,"LatitudeReadout")
EMPTY_SERIALIZER(LatitudeReadout)

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(LongitudeReadout,"LongitudeReadout")
EMPTY_SERIALIZER(LongitudeReadout)

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Rotary,"Rotary")
EMPTY_SERIALIZER(Rotary)

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Rotary2,"Rotary2")
EMPTY_SERIALIZER(Rotary2)

Reformat* NumericReadout::reformatter = new Reformat();

//------------------------------------------------------------------------------
// Slot tables
//------------------------------------------------------------------------------

// AsciiText --
BEGIN_SLOTTABLE(AsciiText)
    "text",             // Text String
END_SLOTTABLE(AsciiText)


// NumericReadout --
BEGIN_SLOTTABLE(NumericReadout)
    "value",            //  1: Value to be displayed
    "maxValue",         //  2: Maximum value that can be displayed
    "format",           //  3: Example format text
    "plusChar",         //  4: Positive value character
    "minusChar",        //  5: Negative value character
    "dpChar",           //  6: Decimal Point character
    "undefinedChar",    //  7: Undefined value character
    "overflowChar",     //  8: Overflow character  
    "maxValid",         //  9: Maximum valid input
    "minValid",         // 10: Minimum valid input
    "blankIfZero",      // 11: Display blanks if value is zero
END_SLOTTABLE(NumericReadout)

// DirectionReadout --
BEGIN_SLOTTABLE(DirectionReadout)
    "preConvertSymbols",            //  1: Pre convert the @ symbol to a degree symbol before drawing
END_SLOTTABLE(DirectionReadout)


//------------------------------------------------------------------------------
//  Map slot table to handles for AsciiText
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(AsciiText)
    ON_SLOT(1,setTextString, Basic::String)
    ON_SLOT(1,setTextList, Basic::List)
END_SLOT_MAP()


//------------------------------------------------------------------------------
//  Map slot table to handles for NumberRo
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(NumericReadout)
    ON_SLOT(1,setSlotFloatToBeDisplayed,Basic::Float)
    ON_SLOT(1,setSlotNumberToBeDisplayed,Basic::Number)
    ON_SLOT(2,setSlotFloatMaxValue,Basic::Float)
    ON_SLOT(2,setSlotNumberMaxValue,Basic::Number)
    ON_SLOT(3,setSlotExampleFormatText,Basic::String)
    ON_SLOT(4,setSlotPlusChar,Basic::String)
    ON_SLOT(5,setSlotMinusChar,Basic::String)
    ON_SLOT(6,setSlotDecimalPointChar,Basic::String)
    ON_SLOT(7,setSlotUndefinedChar,Basic::String)
    ON_SLOT(8,setSlotOverflowChar,Basic::String)
    ON_SLOT(9,setSlotMaxValid,Basic::Number)
    ON_SLOT(10,setSlotMinValid,Basic::Number)
    ON_SLOT(11,setSlotBlankZero,Basic::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
//  Map slot table to handles for DirectionReadout
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(DirectionReadout)
    ON_SLOT(1,setSlotPreConvertSymbols, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Macro event handlers for AsciiText events
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(AsciiText)
    ON_EVENT_OBJ(UPDATE_VALUE,setTextString,Basic::String)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Macro event handlers for NumericReadout events
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(NumericReadout)
    ON_EVENT_OBJ(UPDATE_VALUE,onUpdateValue, Basic::Float)
    ON_EVENT_OBJ(UPDATE_VALUE,onUpdateValue, Basic::Integer)    
    ON_EVENT_OBJ(UPDATE_VALUE,onUpdateValue, Basic::Number)    
END_EVENT_HANDLER()
                                        
//------------------------------------------------------------------------------
// default Macro event handler for Rotary events
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Rotary)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Macro event handler for Rotary2 events
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Rotary2)
    ON_EVENT_OBJ(SELECT,onSelect,Basic::Number)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AsciiText::AsciiText()
{
   STANDARD_CONSTRUCTOR()
}


Cursor::Cursor()
{
   STANDARD_CONSTRUCTOR()
   width(1);
   setText(" ");
   setDisplayMode(reversed);
   setFlashRate(1.0f);
}

NumericReadout::NumericReadout()
{
   STANDARD_CONSTRUCTOR()
   num  = 0.0;
   maxNum = UNDEFINED_VALUE;
   cbuf[0]   = '\0';
   format[0] = '\0';
   lcStrcpy(format,FORMAT_LENGTH,"%.0f");
   justification(Basic::String::RIGHT);
   plusChar = '\0';
   minusChar = '\0';
   dpChar    = '\0';
   undefinedChar = '-';
   overflowChar  = '*';
   postSign = false;
   maxValid = UNDEFINED_VALUE;
   minValid = UNDEFINED_VALUE;
   blankZero = false;
}

HexReadout::HexReadout()
{
   STANDARD_CONSTRUCTOR()
   lcStrcpy(format,FORMAT_LENGTH,"%X");
}

OctalReadout::OctalReadout()
{
   STANDARD_CONSTRUCTOR()
   lcStrcpy(format,FORMAT_LENGTH,"%o");
}

TimeReadout::TimeReadout()
{
   STANDARD_CONSTRUCTOR()
   lcStrcpy(format,FORMAT_LENGTH,"%02d:%02d:%04.1f");
   tmode = hhmmss;
} 

DirectionReadout::DirectionReadout()
{
   STANDARD_CONSTRUCTOR()
   lcStrcpy(format,FORMAT_LENGTH,"%+07.2f");
   tmode = dd;
   // default to pre converting the symbols (most font sets have special character slots)
   preConvertSymbols = true;
}

LatitudeReadout::LatitudeReadout()
{
   STANDARD_CONSTRUCTOR()
   lcStrcpy(format,FORMAT_LENGTH,"%+03d@%04.1f");
   tmode = ddmm;
   plusChar = 'N';
   minusChar = 'S';
   postSign = false;
}

LongitudeReadout::LongitudeReadout()
{
   STANDARD_CONSTRUCTOR()
   lcStrcpy(format,FORMAT_LENGTH,"%+04d@%04.1f");
   tmode = ddmm;
   plusChar = 'E';
   minusChar = 'W';
   postSign = false;
}

Rotary::Rotary()
{
   STANDARD_CONSTRUCTOR()
   //selection = new Basic::Integer(1);    // default rotary item
   Basic::Integer* p = new Basic::Integer(1);    // default rotary item
   setSelectionName(p);
   p->unref();
   preDrawSelectList = true;
}

Rotary2::Rotary2()
{
   STANDARD_CONSTRUCTOR()
   //selection = new Basic::Integer(1);    // default rotary item
   Basic::Integer* p = new Basic::Integer(1);    // default rotary item
   setSelectionName(p);
   p->unref();
}


//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
EMPTY_COPYDATA(AsciiText)
EMPTY_COPYDATA(Cursor)


void NumericReadout::copyData(const NumericReadout& org, const bool)
{
   BaseClass::copyData(org);

   // copy the display buffer, example format, and the sprintf format
   lcStrcpy(cbuf,CBUF_LENGTH,org.cbuf);
   lcStrcpy(format,FORMAT_LENGTH,org.format);

   // copy other member variables
   plusChar  = org.plusChar;
   minusChar = org.minusChar;
   dpChar    = org.dpChar;
   undefinedChar = org.undefinedChar;
   overflowChar  = org.overflowChar;
   postSign  = org.postSign;
   num = org.num;
   maxNum = org.maxNum;
   maxValid = org.maxValid;
   minValid = org.minValid;
   blankZero = org.blankZero;
}

EMPTY_COPYDATA(HexReadout)
EMPTY_COPYDATA(OctalReadout)

void TimeReadout::copyData(const TimeReadout& org, const bool)
{
   BaseClass::copyData(org);
   tmode = org.tmode;
}

void DirectionReadout::copyData(const DirectionReadout& org, const bool)
{
   BaseClass::copyData(org);
   tmode = org.tmode;
   preConvertSymbols = org.preConvertSymbols;
}

EMPTY_COPYDATA(LatitudeReadout)
EMPTY_COPYDATA(LongitudeReadout)
EMPTY_COPYDATA(Rotary)
EMPTY_COPYDATA(Rotary2)


//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(AsciiText)
EMPTY_DELETEDATA(Cursor)

void NumericReadout::deleteData()
{
   cbuf[0]   = '\0';
   lcStrcpy(format,FORMAT_LENGTH,"%.0f");
   plusChar  = '\0';
   minusChar = '\0';
   dpChar    = '\0';
   undefinedChar = '-';
   overflowChar  = '*';
   postSign = false;
   num  = 0.0;
   blankZero = false;
}

EMPTY_DELETEDATA(HexReadout)
EMPTY_DELETEDATA(OctalReadout)
EMPTY_DELETEDATA(TimeReadout)
EMPTY_DELETEDATA(DirectionReadout)
EMPTY_DELETEDATA(LatitudeReadout)
EMPTY_DELETEDATA(LongitudeReadout)
EMPTY_DELETEDATA(Rotary)
EMPTY_DELETEDATA(Rotary2)

//------------------------------------------------------------------------------
// updateData() -- Update non-time critical (background) stuff here
//------------------------------------------------------------------------------
void NumericReadout::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);
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
        Basic::Pair* p = findByIndex(start);
        while (p != 0) {
            BasicGL::Graphic* g = dynamic_cast<BasicGL::Graphic*>(p->object());
            if (g != 0) g->draw();
            p = findByIndex(++start);
        }
        preDrawSelectList = false;
    }

    BaseClass::draw();
}

//------------------------------------------------------------------------------
// input mode functions --
//   filterInputEvent() -- Filter input events using a template character (tc)
//   isValidInputPosition() -- Makes sure the position is valid for input
//------------------------------------------------------------------------------

bool AsciiText::isValidInputPosition(const int)
{
   return true;     // AsciiText fields do not have template characters
}

char AsciiText::filterInputEvent(const int event, const int)
{
    return char(event); // All characters are valid for AsciiText fields
}

char NumericReadout::filterInputEvent(const int event, const int tc)
{
    if (tc == '+') {
        // sign keys
        if (event == '7') {
            if (minusChar != '\0')
                return minusChar;
            else
                return '-';
        }
        else if (event == '9') {
            if (plusChar != '\0')
                return plusChar;
            else
                return '+';
        }
        else
            return '\0';
    }
    else
        return BaseClass::filterInputEvent(event,tc);
}

char HexReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '0' || tc == '#') {
      // Default numeric keys
      if ( (event >= '0' && event <= '9') || (event >= 'A' && event <= 'F') )
         return char(event);
      else
         return '\0';
   }
   else
      return BaseClass::filterInputEvent(event,tc);
}

char OctalReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '0' || tc == '#') {
      // Default numeric keys
      if ( event >= '0' && event <= '7' )
         return char(event);
      else
         return '\0';
   }
   else
      return BaseClass::filterInputEvent(event,tc);
}

char TimeReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '0' || tc == 'H' || tc == 'M' || tc == 'S') {
      // Default numeric keys
      if ( event >= '0' && event <= '9' )
         return char(event);
      else
         return '\0';
   }
   else
      return BaseClass::filterInputEvent(event,tc);
}

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
   else
      return BaseClass::filterInputEvent(event,tc);
}

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
   else
      return BaseClass::filterInputEvent(event,tc);
}

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
   else
      return BaseClass::filterInputEvent(event,tc);
}


//------------------------------------------------------------------------------
// getInputValue() -- returns the readout as a numeric value
//------------------------------------------------------------------------------
double NumericReadout::getInputValue() const
{
    // copy string to buffer with correct sign character
    const size_t CBUFLOCAL_LEN = 100;
    char cbuf[CBUFLOCAL_LEN];
    const char* p = *this;
    lcStrcpy(cbuf,CBUFLOCAL_LEN,p);
    if (cbuf[0] == plusChar)  cbuf[0] = '+';
    if (cbuf[0] == minusChar) cbuf[0] = '-';

    // Check decimal point character
    if (dpChar != '\0') {
        int i;
        for (i = 0; cbuf[i] != '\0'; i++) {
            if (cbuf[i] == dpChar)  cbuf[i] = '.';
        }
    }
   
    // Remove spaces
    {
        int i = 0;
        while (cbuf[i] != '\0') {
            if (cbuf[i] == ' ') {
                int j = i + 1;
                while (cbuf[j] != '\0') { cbuf[j-1] = cbuf[j]; j++; }
                cbuf[j-1] = '\0';
            }
            else i++;
        }
    }
   
   return atof(cbuf);
}

double HexReadout::getInputValue() const
{
   int value = 0;

   // copy string to buffer with correct sign character
   const size_t CBUFLOCAL_LEN = 100;
   char cbuf[CBUFLOCAL_LEN];
   const char* p = *this;
   lcStrcpy(cbuf,CBUFLOCAL_LEN,p);
   if (cbuf[0] == plusChar)  cbuf[0] = '+';
   if (cbuf[0] == minusChar) cbuf[0] = '-';

   sscanf(cbuf, format, &value);
   return double(value);
}

double OctalReadout::getInputValue() const
{
   int value = 0;

   // copy string to buffer with correct sign character
   const size_t CBUFLOCAL_LEN = 100;
   char cbuf[CBUFLOCAL_LEN];
   const char* p = *this;
   lcStrcpy(cbuf,CBUFLOCAL_LEN,p);
   if (cbuf[0] == plusChar)  cbuf[0] = '+';
   if (cbuf[0] == minusChar) cbuf[0] = '-';

   sscanf(cbuf, format, &value);
   return double(value);
}

double TimeReadout::getInputValue() const
{
   double value = 0.0;

   // copy string to buffer with correct sign character
   const size_t CBUFLOCAL_LEN = 100;
   char cbuf[CBUFLOCAL_LEN];
   const char* p = *this;
   lcStrcpy(cbuf,CBUFLOCAL_LEN,p);
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
         sscanf(cbuf, format1, &hrs, &min, &sec);
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
         sscanf(cbuf, format1, &hrs, &min);
         value = hrs*60.0f;
         if (value >= 0.0) value += min;
         else value -= min;
         value *= 60.0;
      }
      break;
      case hh : {   // Hours only
         float hrs = 0.0f;
         sscanf(cbuf, format1, &hrs);
         value = hrs*3600.0f;
      }
      break;
      case mmss : { // Minutes and seconds
         int   min = 0;
         float sec = 0.0f;
         sscanf(cbuf, format1, &min, &sec);
         value = min*60.0f;
         if (value >= 0.0) value += sec;
         else value -= sec;
      }
      break;
      case mm : {   // Minutes only
         float min = 0.0;
         sscanf(cbuf, format1, &min);
         value = min*60.0f;
      }
      break;
      case ss : {   // Seconds only
         float sec = 0.0;
         sscanf(cbuf, format1, &sec);
         value = sec;
      }
      break;
   }
   return double(value);
}

double DirectionReadout::getInputValue() const
{
   double value = 0.0;

   // copy string to buffer with correct sign character
   const size_t CBUFLOCAL_LEN = 100;
   char cbuf[CBUFLOCAL_LEN];
   const char* p = *this;
   lcStrcpy(cbuf,CBUFLOCAL_LEN,p);
   if (cbuf[0] == plusChar)  cbuf[0] = '+';
   if (cbuf[0] == minusChar) cbuf[0] = '-';

   switch (tmode) {
      case ddmmss : {   // Degrees, Minutes, and seconds
         double degs = 0.0;
         double min = 0.0;
         double sec = 0.0;
         sscanf(cbuf, "%lf@%lf'%lf", &degs, &min, &sec);
         min += sec/60.0f;
         if (degs >= 0.0) value = degs + min/60.0f;
         else value = degs - min/60.0f;
      }
      break;
      case ddmm : { // Degrees and minutes
         double degs = 0.0;
         double min = 0.0;
         sscanf(cbuf, "%lf@%lf", &degs, &min);
         if (degs >= 0.0) value = degs + min/60.0f;
         else value = degs - min/60.0f;
      }
      break;
      case dd : {   // Degrees only
         sscanf(cbuf, "%lf", &value);
      }
      break;
   }
   return double(value);
}

//------------------------------------------------------------------------------
// isInputValueValid() -- returns true if the input value is valid.
//------------------------------------------------------------------------------
bool NumericReadout::isInputValueValid() const
{
    bool ok = true;
    double val = getInputValue();
    if ( (minValid != UNDEFINED_VALUE && val < minValid) ||
         (maxValid != UNDEFINED_VALUE && val > maxValid) ) {
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// updateData --
//------------------------------------------------------------------------------
void Cursor::updateData(const LCreal dt)
{
   int ln = 0;
   int cp = 0;
   if (getDisplay()->focus() != 0) {
      setVisibility( getDisplay()->focus()->cursor(&ln,&cp) );
   }
   else
      setVisibility(false);

   if (isVisible()) {
      line(ln);
      column(cp);
      BaseClass::updateData(dt);
   }
}


//------------------------------------------------------------------------------
// redisplay() -- redisplay the value 
//------------------------------------------------------------------------------
void NumericReadout::redisplay()
{


   // Check if we are displaying blank for zero
   if ((num == 0) && blankZero) {
      size_t i;
      for (i = 0; i < width(); i++) {
         cbuf[i] = ' ';
      }
      cbuf[width()] = '\0';
      setText(cbuf);
      return;
   }

   // Check if we have an undefined value and should place dashes in the display
   if (num == UNDEFINED_VALUE) {
      size_t i;
      for (i = 0; i < width(); i++) {
         cbuf[i] = undefinedChar;
      }
      cbuf[width()] = '\0';
      setText(cbuf);
      return;
   }

   // Check if we have an undefined value and should place dashes in the display
   if (maxNum > UNDEFINED_VALUE && num > maxNum) {
      size_t i;
      for (i = 0; i < width(); i++) {
         cbuf[i] = overflowChar;
      }
      cbuf[width()] = '\0';
      setText(cbuf);
      return;
   }

   // Create the readout text string
   makeText();

   size_t len = strlen(cbuf);

   // When we have a replacement char for the sign character
   if (plusChar != '\0' || minusChar != '\0') {

      // Find the sign character (if any)
      size_t i = 0;
      while (i < len) {
         if (cbuf[i] == '+' || cbuf[i] == '-') break;
         i++;
      }

      // When we have a sign character
      if (i < len) {
         if (cbuf[i] == '+' && plusChar  != '\0')  cbuf[i] = plusChar;
         if (cbuf[i] == '-' && minusChar != '\0')  cbuf[i] = minusChar;
         if (postSign) {
            // move sign character to the end of the string
            char cc = cbuf[i];
            while (i < (len-1)) {
               cbuf[i] = cbuf[i+1];
               i++;
            }
            cbuf[i] = cc;
         }
      }
   }

   // When we have a replacement char for the decimal point
   if (dpChar != '\0') {

      // Find the decimal point (if any)
      size_t i = 0;
      while (i < len) {
         if (cbuf[i] == '.') break;
         i++;
      }

      // when we have a decimal point
      if (i < len) cbuf[i] = dpChar;
   }

   // Have Field display this text
   setText(cbuf);
}


//------------------------------------------------------------------------------
//  makeText() -- make the text string use the current value and formats
//------------------------------------------------------------------------------
void NumericReadout::makeText()
{
   std::sprintf(cbuf, format, getFloat());
}

void HexReadout::makeText()
{
   std::sprintf(cbuf, format, getInt());
}

void OctalReadout::makeText()
{
   std::sprintf(cbuf, format, getInt());
}

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
            int ihrs = int(minutes/60.0f);
            double min = minutes - double(ihrs*60);
            int imin = int(min);
            double sec = (min - double(imin))*60.0f;
            std::sprintf(cbuf, format, ihrs, imin, sec);
            if (neg) { /* if it was negative, swap the the posible + sign to the - sign */
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
            int  ihrs = int(minutes/60.0f);
            double min = minutes - double(ihrs*60);
            std::sprintf(cbuf, format, ihrs, min);
            if (neg) { /* if it was negative, swap the the posible + sign to the - sign */
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
            int  imin = int(seconds/60.0f);
            double sec = seconds - double(imin*60);
            std::sprintf(cbuf, format, imin, sec);
            if (neg) { /* if it was negative, swap the the posible + sign to the - sign */
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
            int     ideg = int(degrees);
            double min = (degrees - double(ideg))*60.0f;
            int     imin = int(min);
            double sec = (min - double(imin))*60.0f;
            if (neg) ideg = -ideg;
            std::sprintf(cbuf, format, ideg, imin, sec);
        }
        break;
        case ddmm : {   // Degrees and minutes
            int     ideg = int(degrees);
            double  min  = (degrees - double(ideg))*60.0f;
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
    // now convert the symbols if we are supposed so
    if (preConvertSymbols) {
       // then turn any '@' characters to degree symbols.
       size_t len = strlen(cbuf);
       for (unsigned int i = 0; i < len; i++) {
          if (cbuf[i] == '@') cbuf[i] = '°';
       }
    }
}

void LatitudeReadout::makeText()
{
   // Let our base class do its thing
   BaseClass::makeText();

   if (preConvertSymbols) {
      // then turn any '@' characters to degree symbols.
      size_t len = strlen(cbuf);

      for (unsigned int i = 0; i < len; i++) {
         if (cbuf[i] == '@') cbuf[i] = '°';
      }
   }
}

void LongitudeReadout::makeText()
{
   // Let our base class do its thing
   BaseClass::makeText();

   if (preConvertSymbols) {
      // then turn any '@' characters to degree symbols.
      size_t len = strlen(cbuf);

      for (unsigned int i = 0; i < len; i++) {
         if (cbuf[i] == '@') cbuf[i] = '°';
      }
   }
}

//------------------------------------------------------------------------------
// reformat() -- convert the numerical value into an ascii character string
//------------------------------------------------------------------------------
void NumericReadout::reformat(const char* const example)
{
   if (reformatter->convertNumber(example) != Reformat::invalid) {
      setExample(example);
      lcStrcpy(format,FORMAT_LENGTH,reformatter->getFormat());
      postSign = reformatter->isPostSign();
      redisplay();
   }
}


void HexReadout::reformat(const char* const example)
{
   if (reformatter->convertHex(example) != Reformat::invalid) {
      setExample(example);
      lcStrcpy(format,FORMAT_LENGTH,reformatter->getFormat());
      postSign = reformatter->isPostSign();
      redisplay();
   }
}


void OctalReadout::reformat(const char* const example)
{
   if (reformatter->convertOctal(example) != Reformat::invalid) {
      setExample(example);
      lcStrcpy(format,FORMAT_LENGTH,reformatter->getFormat());
      postSign = reformatter->isPostSign();
      redisplay();
   }
}


void TimeReadout::reformat(const char* const example)
{
   TimeMode results = reformatter->convertTime(example);
   if (results != invalid) {
      setExample(example);
      lcStrcpy(format,FORMAT_LENGTH,reformatter->getFormat());
      tmode = results;
      postSign = reformatter->isPostSign();
      redisplay();
   }
}


void DirectionReadout::reformat(const char* const example)
{
   DirMode results = reformatter->convertDirection(example);
   if (results != invalid) {
      setExample(example);
      lcStrcpy(format,FORMAT_LENGTH,reformatter->getFormat());
      tmode = results;
      postSign = reformatter->isPostSign();
      redisplay();
   }
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* AsciiText::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

Basic::Object* NumericReadout::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

Basic::Object* DirectionReadout::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print functions
//------------------------------------------------------------------------------
std::ostream& AsciiText::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "text: " << origStr << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }
   return sout;
}


std::ostream& NumericReadout::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   if (num != 0.0) {
      indent(sout,i+j);
      sout << "value: " << num << std::endl;
   }

   if (maxNum != UNDEFINED_VALUE) {
      indent(sout,i+j);
      sout << "maxValue: " << maxNum << std::endl;
   }

   if (inputExample.len() > 0) {
      indent(sout,i+j);
      sout << "format: " << inputExample << std::endl;
   }

   if (plusChar > 0) {
      indent(sout,i+j);
      sout << "plusChar: " << '\"' << plusChar << '\"' << std::endl;
   }

   if (minusChar > 0) {
      indent(sout,i+j);
      sout << "minusChar: " << '\"' << minusChar << '\"' << std::endl;
   }

   if (dpChar > 0) {
      indent(sout,i+j);
      sout << "dpChar: " << '\"' << dpChar << '\"' << std::endl;
   }

   if (undefinedChar > 0) {
      indent(sout,i+j);
      sout << "undefinedChar: " << '\"' << undefinedChar << '\"' << std::endl;
   }

   if (overflowChar > 0) {
      indent(sout,i+j);
      sout << "overflowChar: " << '\"' << overflowChar << '\"' << std::endl;
   }

   if (maxValid != UNDEFINED_VALUE) {
      indent(sout,i+j);
      sout << "maxValid: " << maxValid << std::endl;
   }

   if (minValid != UNDEFINED_VALUE) {
      indent(sout,i+j);
      sout << "minValid: " << minValid << std::endl;
   }

   if (blankZero) {
      indent(sout,i+j);
      sout << "blankZero: " << blankZero << std::endl;
   }

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }
   return sout;
}

//------------------------------------------------------------------------------
// setTextString() -- takes in a Basic::String and sets it
//------------------------------------------------------------------------------
bool AsciiText::setTextString(const Basic::String* const stsobj)
{
    bool ok = true;
    if (stsobj != 0) {
        if (width() == 0) width(stsobj->len());
        setText(*stsobj);
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "AsciiText::setTextString: \"text\" must be a string or a list of (ASCII) numbers!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setTextString() -- takes in a Basic::String and sets it
//------------------------------------------------------------------------------
bool AsciiText::setTextList(const Basic::List* const stlobj)
{
    bool ok = true;
    if (stlobj != 0) {
        float values[256];
        int n = stlobj->getNumberList(values, 256);
        if (n > 0) {
            char cbuf[258];
            int j;
            for (j = 0; j < n; j++) {
                cbuf[j] = char(values[j]);
            }
        cbuf[j] = '\0';
        setText(cbuf);
        ok = true;
        }
        else {
              if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "AsciiText::setTextList: \"text\" must be a string or a list of (ASCII) numbers!" << std::endl;
              }
        ok = false;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotFloatToBeDisplayed() --
//------------------------------------------------------------------------------
bool NumericReadout::setSlotFloatToBeDisplayed(const Basic::Float* const sftbdobj)
{
    bool ok = true;
    if (sftbdobj != 0) {
        setValue(sftbdobj->getReal());
        ok = true;
    }
    else {
           if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setFloatToBeDisplayed: \"value\" must be a number!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotNumberToBeDisplayed() --
//------------------------------------------------------------------------------
bool NumericReadout::setSlotNumberToBeDisplayed(const Basic::Number* const sntbdobj)
{
    bool ok = true;
    if (sntbdobj != 0) {
        setValue(int(sntbdobj->getInt()));
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setNumberToBeDisplayed: \"value\" must be a number!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotFloatMaxValue() --
//------------------------------------------------------------------------------
bool NumericReadout::setSlotFloatMaxValue(const Basic::Float* const sfmvobj)
{
    bool ok = true;
    if (sfmvobj != 0) {
        setMaxValue(sfmvobj->getReal());
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setFloatMaxValue: \"maxValue\" must be a number!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotNumberMaxValue() --
//------------------------------------------------------------------------------
bool NumericReadout::setSlotNumberMaxValue(const Basic::Number* const snmvobj)
{
    bool ok = true;
    if (snmvobj != 0) {
        setMaxValue(int(snmvobj->getInt()));
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setNumberMaxValue: \"maxValue\" must be a number!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotExampleFormatText() --
//------------------------------------------------------------------------------
bool NumericReadout::setSlotExampleFormatText(const Basic::String* const seftobj)
{
    bool ok = true;
    if (seftobj != 0) {
        if (width() == 0) width(seftobj->len());
        reformat(*seftobj);
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setExampleFormatText: \"format\" must be a string!" << std::endl;
          }
    ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotPlusChar() -- positive value character
//------------------------------------------------------------------------------
bool NumericReadout::setSlotPlusChar(const Basic::String* const spcobj)
{
    bool ok = true;
    if (spcobj != 0) {
        const char* str = *spcobj;
        plusChar = str[0];
        redisplay();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setPlusChar \"plusChar\" must be a character!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotMinusChar() -- negative value character
//------------------------------------------------------------------------------
bool NumericReadout::setSlotMinusChar(const Basic::String* const smcobj)
{
    bool ok = true;
    if (smcobj != 0) {
        const char* str = *smcobj;
        minusChar = str[0];
        redisplay();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setMinusChar \"minusChar\" must be a character!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotDecimalPointChar() -- decimal point character
//------------------------------------------------------------------------------
bool NumericReadout::setSlotDecimalPointChar(const Basic::String* const sdpcobj)
{
    bool ok = true;
    if (sdpcobj != 0) {
        const char* str = *sdpcobj;
        dpChar = str[0];
        redisplay();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setDecimalPointChar \"dpChar\" must be a character!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotUndefinedChar() -- undefined value character
//------------------------------------------------------------------------------
bool NumericReadout::setSlotUndefinedChar(const Basic::String* const sucobj)
{
    bool ok = true;
    if (sucobj != 0) {
        const char* str = *sucobj;
        undefinedChar = str[0];
        redisplay();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setUndefinedChar \"undefinedChar\" must be a character!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotMaxValid() -- Maximum valid value
//------------------------------------------------------------------------------
bool NumericReadout::setSlotMaxValid(const Basic::Number* const msg)
{
    bool ok = true;
    if (msg != 0) {
        maxValid = msg->getReal();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setSlotMaxValid: \"maxValid\" must be a number!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotMinValid() -- Minimum valid value
//------------------------------------------------------------------------------
bool NumericReadout::setSlotMinValid(const Basic::Number* const msg)
{
    bool ok = true;
    if (msg != 0) {
        minValid = msg->getReal();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setSlotMinValid: \"minValid\" must be a number!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotBlankZero() -- Display blank if value is zero
//------------------------------------------------------------------------------
bool NumericReadout::setSlotBlankZero(const Basic::Number* const msg)
{
    bool ok = true;
    if (msg != 0) {
       blankZero = msg->getBoolean();
    }
    else {
       if (isMessageEnabled(MSG_ERROR)) {
          std::cerr << "NumericReadout::setSlotMinValid: \"blankZero\" must be a number!" << std::endl;
       }
       ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotOverflowChar() --overflow character
//------------------------------------------------------------------------------
bool NumericReadout::setSlotOverflowChar(const Basic::String* const socobj)
{
    bool ok = true;
    if (socobj != 0) {
        const char* str = *socobj;
        overflowChar = str[0];
        redisplay();
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NumericReadout::setOverflowChar \"overflowChar\" must be a character!" << std::endl;
          }
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotPreConvertSymbols() -- convert @ symbols to degree symbol beforehand
//------------------------------------------------------------------------------
bool DirectionReadout::setSlotPreConvertSymbols(const Basic::Number* const x)
{
    bool ok = false;
    if (x != 0) {
      ok = true;
      preConvertSymbols = x->getBoolean();
    }
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateValue() - for Basic::Float
//------------------------------------------------------------------------------
bool NumericReadout::onUpdateValue(const Basic::Float* const ouvobj)
{
    if (ouvobj != 0) {
        setValue(ouvobj->getDouble());
    }
    return true;
}

//------------------------------------------------------------------------------
// onUpdateValue() - for Basic::Integer
//------------------------------------------------------------------------------
bool NumericReadout::onUpdateValue(const Basic::Integer* const ouvobj)
{
    if (ouvobj != 0) {
        setValue(ouvobj->getInt());
    }
    return true;
}

//------------------------------------------------------------------------------
// onUpdateValue() - for Basic::Number
//------------------------------------------------------------------------------
bool NumericReadout::onUpdateValue(const Basic::Number* const ouvobj)
{
    if (ouvobj != 0) {
        setValue(ouvobj->getDouble());
    }
    return true;
}

//------------------------------------------------------------------------------
// onSelect() - Macro function for Rotary2
//------------------------------------------------------------------------------
bool Rotary2::onSelect(const Basic::Number* const osobj)
{
    if (osobj != 0) {
        if (osobj->getBoolean()){
            //if true, select the second component
            Basic::Integer two(2);
            select(&two);
        }
        else {
            //if false, select the first component
            Basic::Integer one(1);
            select(&one);
        }
    }
    return true;
}

} // End BasicGL namespace
} // End Eaagles namespace
