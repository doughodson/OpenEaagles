
#include "openeaagles/graphics/Readouts.h"

#include "openeaagles/graphics/Display.h"
#include "openeaagles/base/Float.h"
#include "openeaagles/base/Integer.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/util/str_utils.h"
#include "openeaagles/base/util/constants.h"

#include "../ReformatScanner.h"

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
// class AsciiText
//==============================================================================
IMPLEMENT_SUBCLASS(AsciiText,"AsciiText")
EMPTY_COPYDATA(AsciiText)
EMPTY_DELETEDATA(AsciiText)

// Slot table
BEGIN_SLOTTABLE(AsciiText)
   "text",             // Text String
END_SLOTTABLE(AsciiText)

BEGIN_SLOT_MAP(AsciiText)
   ON_SLOT(1,setTextString, base::String)
   ON_SLOT(1,setTextList, base::List)
END_SLOT_MAP()

// Event handlers for AsciiText events
BEGIN_EVENT_HANDLER(AsciiText)
   ON_EVENT_OBJ(UPDATE_VALUE,setTextString,base::String)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
AsciiText::AsciiText()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// input mode functions --
//   filterInputEvent() -- Filter input events using a template character (tc)
//   isValidInputPosition() -- Makes sure the position is valid for input
//------------------------------------------------------------------------------
char AsciiText::filterInputEvent(const int event, const int)
{
   return static_cast<char>(event); // All characters are valid for AsciiText fields
}

bool AsciiText::isValidInputPosition(const int)
{
   return true;     // AsciiText fields do not have template characters
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// setTextString() -- takes in a base::String and sets it
bool AsciiText::setTextString(const base::String* const stsobj)
{
   bool ok = true;
   if (stsobj != nullptr) {
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

// setTextList() -- takes in alist of ascii numbers
bool AsciiText::setTextList(const base::List* const stlobj)
{
   bool ok = true;
   if (stlobj != nullptr) {
      float values[256];
      int n = stlobj->getNumberList(values, 256);
      if (n > 0) {
         char cbuf[258];
         int j;
         for (j = 0; j < n; j++) {
            cbuf[j] = char(values[j]);
         }
         cbuf[j] = '\0';
         if (width() == 0) width(j);
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
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* AsciiText::getSlotByIndex(const int si)
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
      sout << "( " << getFactoryName() << std::endl;
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


//==============================================================================
// class Cursor
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Cursor,"Cursor")
EMPTY_COPYDATA(Cursor)
EMPTY_DELETEDATA(Cursor)
EMPTY_SERIALIZER(Cursor)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Cursor::Cursor()
{
   STANDARD_CONSTRUCTOR()

   width(1);
   setText(" ");
   setDisplayMode(reversed);
   setFlashRate(1.0f);
}

//------------------------------------------------------------------------------
// updateData --
//------------------------------------------------------------------------------
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


//==============================================================================
// class NumericReadout
//==============================================================================
IMPLEMENT_SUBCLASS(NumericReadout,"NumericReadout")

ReformatScanner* NumericReadout::reformatter = new ReformatScanner();

// Slot table
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

BEGIN_SLOT_MAP(NumericReadout)
   ON_SLOT(1,setSlotFloatToBeDisplayed,base::Float)
   ON_SLOT(1,setSlotNumberToBeDisplayed,base::Number)
   ON_SLOT(2,setSlotFloatMaxValue,base::Float)
   ON_SLOT(2,setSlotNumberMaxValue,base::Number)
   ON_SLOT(3,setSlotExampleFormatText,base::String)
   ON_SLOT(4,setSlotPlusChar,base::String)
   ON_SLOT(5,setSlotMinusChar,base::String)
   ON_SLOT(6,setSlotDecimalPointChar,base::String)
   ON_SLOT(7,setSlotUndefinedChar,base::String)
   ON_SLOT(8,setSlotOverflowChar,base::String)
   ON_SLOT(9,setSlotMaxValid,base::Number)
   ON_SLOT(10,setSlotMinValid,base::Number)
   ON_SLOT(11,setSlotBlankZero,base::Number)
END_SLOT_MAP()

// Event handlers for NumericReadout events
BEGIN_EVENT_HANDLER(NumericReadout)
ON_EVENT_OBJ(UPDATE_VALUE,onUpdateValue, base::Float)
ON_EVENT_OBJ(UPDATE_VALUE,onUpdateValue, base::Integer)
ON_EVENT_OBJ(UPDATE_VALUE,onUpdateValue, base::Number)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
NumericReadout::NumericReadout()
{
   STANDARD_CONSTRUCTOR()

   num  = 0.0;
   maxNum = base::UNDEFINED_VALUE;
   cbuf[0]   = '\0';
   format[0] = '\0';
   base::utStrcpy(format,FORMAT_LENGTH,"%.0f");
   justification(base::String::RIGHT);
   plusChar = '\0';
   minusChar = '\0';
   dpChar    = '\0';
   undefinedChar = '-';
   overflowChar  = '*';
   postSign = false;
   maxValid = base::UNDEFINED_VALUE;
   minValid = base::UNDEFINED_VALUE;
   blankZero = false;
}

void NumericReadout::copyData(const NumericReadout& org, const bool)
{
   BaseClass::copyData(org);

   // copy the display buffer, example format, and the sprintf format
   base::utStrcpy(cbuf,CBUF_LENGTH,org.cbuf);
   base::utStrcpy(format,FORMAT_LENGTH,org.format);

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

void NumericReadout::deleteData()
{
   cbuf[0]   = '\0';
   base::utStrcpy(format,FORMAT_LENGTH,"%.0f");
   plusChar  = '\0';
   minusChar = '\0';
   dpChar    = '\0';
   undefinedChar = '-';
   overflowChar  = '*';
   postSign = false;
   num  = 0.0;
   blankZero = false;
}

//------------------------------------------------------------------------------
// updateData() -- Update non-time critical (background) stuff here
//------------------------------------------------------------------------------
void NumericReadout::updateData(const double dt)
{
   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// isInputValueValid() -- returns true if the input value is valid.
//------------------------------------------------------------------------------
bool NumericReadout::isInputValueValid() const
{
   bool ok = true;
   const double val = getInputValue();
   if ( (minValid != base::UNDEFINED_VALUE && val < minValid) ||
        (maxValid != base::UNDEFINED_VALUE && val > maxValid) ) {
      ok = false;
   }
   return ok;
}

//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
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
      else {
         return '\0';
      }
   }
   else {
      return BaseClass::filterInputEvent(event,tc);
   }
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
   base::utStrcpy(cbuf,CBUFLOCAL_LEN,p);
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

   return std::atof(cbuf);
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
   if (num == base::UNDEFINED_VALUE) {
      size_t i;
      for (i = 0; i < width(); i++) {
         cbuf[i] = undefinedChar;
      }
      cbuf[width()] = '\0';
      setText(cbuf);
      return;
   }

   // Check if we have an undefined value and should place dashes in the display
   if (maxNum != base::UNDEFINED_VALUE && num > maxNum) {
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

   size_t len = std::strlen(cbuf);

   // When we have a replacement char for the sign character
   if (plusChar != '\0' || minusChar != '\0' || postSign) {

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

//------------------------------------------------------------------------------
// reformat() -- convert the numerical value into an ascii character string
//------------------------------------------------------------------------------
void NumericReadout::reformat(const char* const example)
{
   if (reformatter->convertNumber(example) != ReformatScanner::DataType::invalid) {
      setExample(example);
      base::utStrcpy(format,FORMAT_LENGTH,reformatter->getFormat());
      postSign = reformatter->isPostSign();
      redisplay();
   }
}

//------------------------------------------------------------------------------
// Event functions
//------------------------------------------------------------------------------

// onUpdateValue() - for base::Float
bool NumericReadout::onUpdateValue(const base::Float* const ouvobj)
{
   if (ouvobj != nullptr) {
      setValue(ouvobj->getDouble());
   }
   return true;
}

// onUpdateValue() - for base::Integer
bool NumericReadout::onUpdateValue(const base::Integer* const ouvobj)
{
   if (ouvobj != nullptr) {
      setValue(ouvobj->getInt());
   }
   return true;
}

// onUpdateValue() - for base::Number
bool NumericReadout::onUpdateValue(const base::Number* const ouvobj)
{
   if (ouvobj != nullptr) {
      setValue(ouvobj->getDouble());
   }
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

//  setSlotFloatToBeDisplayed() --
bool NumericReadout::setSlotFloatToBeDisplayed(const base::Float* const sftbdobj)
{
   bool ok = true;
   if (sftbdobj != nullptr) {
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

//  setSlotNumberToBeDisplayed() --
bool NumericReadout::setSlotNumberToBeDisplayed(const base::Number* const sntbdobj)
{
   bool ok = true;
   if (sntbdobj != nullptr) {
      setValue(static_cast<int>(sntbdobj->getInt()));
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "NumericReadout::setNumberToBeDisplayed: \"value\" must be a number!" << std::endl;
      }
      ok = false;
   }
   return ok;
}

//  setSlotFloatMaxValue() --
bool NumericReadout::setSlotFloatMaxValue(const base::Float* const sfmvobj)
{
   bool ok = true;
   if (sfmvobj != nullptr) {
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

//  setSlotNumberMaxValue() --
bool NumericReadout::setSlotNumberMaxValue(const base::Number* const snmvobj)
{
   bool ok = true;
   if (snmvobj != nullptr) {
      setMaxValue(static_cast<int>(snmvobj->getInt()));
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "NumericReadout::setNumberMaxValue: \"maxValue\" must be a number!" << std::endl;
      }
      ok = false;
   }
   return ok;
}

//  setSlotExampleFormatText() --
bool NumericReadout::setSlotExampleFormatText(const base::String* const seftobj)
{
   bool ok = true;
   if (seftobj != nullptr) {
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

//  setSlotPlusChar() -- positive value character
bool NumericReadout::setSlotPlusChar(const base::String* const spcobj)
{
   bool ok = true;
   if (spcobj != nullptr) {
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

//  setSlotMinusChar() -- negative value character
bool NumericReadout::setSlotMinusChar(const base::String* const smcobj)
{
   bool ok = true;
   if (smcobj != nullptr) {
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

//  setSlotDecimalPointChar() -- decimal point character
bool NumericReadout::setSlotDecimalPointChar(const base::String* const sdpcobj)
{
   bool ok = true;
   if (sdpcobj != nullptr) {
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

//  setSlotUndefinedChar() -- undefined value character
bool NumericReadout::setSlotUndefinedChar(const base::String* const sucobj)
{
   bool ok = true;
   if (sucobj != nullptr) {
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

//  setSlotMaxValid() -- Maximum valid value
bool NumericReadout::setSlotMaxValid(const base::Number* const msg)
{
   bool ok = true;
   if (msg != nullptr) {
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

//  setSlotMinValid() -- Minimum valid value
bool NumericReadout::setSlotMinValid(const base::Number* const msg)
{
   bool ok = true;
   if (msg != nullptr) {
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

//  setSlotBlankZero() -- Display blank if value is zero
bool NumericReadout::setSlotBlankZero(const base::Number* const msg)
{
   bool ok = true;
   if (msg != nullptr) {
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

//  setSlotOverflowChar() --overflow character
bool NumericReadout::setSlotOverflowChar(const base::String* const socobj)
{
   bool ok = true;
   if (socobj != nullptr) {
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
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* NumericReadout::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print functions
//------------------------------------------------------------------------------
std::ostream& NumericReadout::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   if (num != 0.0) {
      indent(sout,i+j);
      sout << "value: " << num << std::endl;
   }

   if (maxNum != base::UNDEFINED_VALUE) {
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

   if (maxValid != base::UNDEFINED_VALUE) {
      indent(sout,i+j);
      sout << "maxValid: " << maxValid << std::endl;
   }

   if (minValid != base::UNDEFINED_VALUE) {
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
