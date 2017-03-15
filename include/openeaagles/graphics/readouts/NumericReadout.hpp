
#ifndef __oe_graphics_NumericReadout_H__
#define __oe_graphics_NumericReadout_H__

#include "openeaagles/graphics/Field.hpp"

#include "openeaagles/base/util/constants.hpp"

namespace oe {
namespace base { class Object; class Float; class Integer; class Number; }

namespace graphics {

class ReformatScanner;

//------------------------------------------------------------------------------
// Class: NumericReadout
//
// Factory name: NumericReadout
//
// Slots:
//   value           <Float>     ! Value to be displayed (default: 0.0)
//   value           <Number>    ! Value to be displayed
//   maxValue        <Float>     ! Maximum value that can be displayed (default: UNDEFINED_VALUE)
//   maxValue        <Number>    ! Maximum value that can be displayed
//   format          <String>    ! Example format text (default: '\0')
//   plusChar        <String>    ! Positive value character (default: '\0')
//   minusChar       <String>    ! Negative value character (default: '\0')
//   dpChar          <String>    ! Decimal Point character (default: '\0')
//   undefinedChar   <String>    ! Undefined value character (default: '-')
//   overflowChar    <String>    ! Overflow character (default: '+')
//   maxValid        <Number>    ! Maximum valid input (default: UNDEFINED_VALUE)
//   minValid        <Number>    ! Minimum valid input (default: UNDEFINED_VALUE)
//   blankIfZero     <Boolean>   ! Display blanks if value is zero
//
// Example formats:
//
//    ######    // Integer
//    +#####    // Integer w/plus sign
//    00000#    // Integer w/leading zeros
//    +0000#    // Integer w/plus sign & leading zeros
//    ###.##    // Float w/2 right of decimal point
//    +##.##    // Float w/plus sign, 2 right of decimal point
//    00#.##    // Float w/2 right of decimal point & leading zeros
//    +0#.##    // Float w/plus sign, 2 right of decimal point, & leading zeros
//
// Public Member Functions:
//  int getInt()
//      Returns num as an int.
//
//  double getFloat()
//      Returns num. (native)
//
//  setValue(int v)
//  setValue(double v)
//      Sets num to v as an double (in both cases) then redisplays the value.
//
//  setMaxValue(int v)
//  setMaxValue(double v)
//      Sets maxNum to v as an double (in both cases) then redisplays the value.
//
//  double getInputValue()
//      Gets the input value.
//
//  bool isInputValueValid()
//      Checks the input value and returns true if it is valid.
//
//------------------------------------------------------------------------------
class NumericReadout : public Field
{
   DECLARE_SUBCLASS(NumericReadout, Field)

public:
   NumericReadout();

   int getInt() const               { return int(num); }
   double getFloat() const          { return num; }

   void setValue(const int v)       { num = static_cast<double>(v); redisplay(); }
   void setValue(const double v)    { num = v; redisplay(); }

   void setMaxValue(const int v)    { maxNum = static_cast<double>(v); redisplay(); }
   void setMaxValue(const double v) { maxNum = v; redisplay(); }

   virtual double getInputValue() const override;
   virtual bool isInputValueValid() const override;
   virtual char filterInputEvent(const int event, const int tc) override;
   virtual bool event(const int key, base::Object* const obj = nullptr) override;
   virtual void updateData(const double dt = 0.0) override;

   //event handler macro functions
   virtual bool onUpdateValue(const base::Float* const ouvobj);
   virtual bool onUpdateValue(const base::Integer* const ouvobj);
   virtual bool onUpdateValue(const base::Number* const ouvobj);

protected:
   virtual bool setSlotFloatToBeDisplayed(const base::Float* const msg);
   virtual bool setSlotNumberToBeDisplayed(const base::Number* const msg);
   virtual bool setSlotFloatMaxValue(const base::Float* const msg);
   virtual bool setSlotNumberMaxValue(const base::Number* const msg);
   virtual bool setSlotExampleFormatText(const base::String* const msg);
   virtual bool setSlotPlusChar(const base::String* const msg);
   virtual bool setSlotMinusChar(const base::String* const msg);
   virtual bool setSlotDecimalPointChar(const base::String* const msg);
   virtual bool setSlotUndefinedChar(const base::String* const msg);
   virtual bool setSlotOverflowChar(const base::String* const msg);
   virtual bool setSlotMaxValid(const base::Number* const msg);
   virtual bool setSlotMinValid(const base::Number* const msg);
   virtual bool setSlotBlankZero(const base::Number* const msg);

   virtual void makeText();
   virtual void redisplay();
   virtual void reformat(const char* const example);

   static const size_t CBUF_LENGTH = 32;    // Max length of cbuf
   static const size_t FORMAT_LENGTH = 32;  // Max length of format

   char cbuf[CBUF_LENGTH];       // buffer
   char format[FORMAT_LENGTH];   // Current format string

   char plusChar {};                // Positive value character
   char minusChar {};               // Negative value character
   char dpChar {};                  // Decimal point character
   char undefinedChar {'-'};        // Undefined value character
   char overflowChar {'*'};         // Overflow character
   bool postSign {};                // If true, sign char is at end of string

   static ReformatScanner* reformatter; // Generates format statements by example

private:
   double num {};                            // Value as double
   double maxNum {base::UNDEFINED_VALUE};    // Maximum value
   double maxValid {base::UNDEFINED_VALUE};  // Maximum valid input value
   double minValid {base::UNDEFINED_VALUE};  // Minimum valid input value
   bool blankZero {};                        // Display blank instead of zero value
};

}
}

#endif
