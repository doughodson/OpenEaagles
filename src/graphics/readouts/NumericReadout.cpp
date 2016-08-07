
#include "openeaagles/graphics/readouts/NumericReadout.h"

#include "openeaagles/base/Float.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/Integer.h"

#include "openeaagles/base/util/str_utils.h"
#include "openeaagles/base/util/constants.h"

#include "openeaagles/graphics/readouts/ReformatScanner.h"

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
   ON_SLOT(1, setSlotFloatToBeDisplayed, base::Float)
   ON_SLOT(1, setSlotNumberToBeDisplayed, base::Number)
   ON_SLOT(2, setSlotFloatMaxValue, base::Float)
   ON_SLOT(2, setSlotNumberMaxValue, base::Number)
   ON_SLOT(3, setSlotExampleFormatText, base::String)
   ON_SLOT(4, setSlotPlusChar, base::String)
   ON_SLOT(5, setSlotMinusChar, base::String)
   ON_SLOT(6, setSlotDecimalPointChar, base::String)
   ON_SLOT(7, setSlotUndefinedChar, base::String)
   ON_SLOT(8, setSlotOverflowChar, base::String)
   ON_SLOT(9, setSlotMaxValid, base::Number)
   ON_SLOT(10, setSlotMinValid, base::Number)
   ON_SLOT(11, setSlotBlankZero, base::Number)
END_SLOT_MAP()

// Event handlers for NumericReadout events
BEGIN_EVENT_HANDLER(NumericReadout)
ON_EVENT_OBJ(UPDATE_VALUE,onUpdateValue, base::Float)
ON_EVENT_OBJ(UPDATE_VALUE,onUpdateValue, base::Integer)
ON_EVENT_OBJ(UPDATE_VALUE,onUpdateValue, base::Number)
END_EVENT_HANDLER()

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

base::Object* NumericReadout::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

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

}
}
