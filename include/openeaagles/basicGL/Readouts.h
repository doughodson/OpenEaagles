//------------------------------------------------------------------------------
// Classes: AsciiText, NumericReadout, TimeReadout, HexReadout,
//          LatitudeReadout, LongitudeReadout, DirectionReadout, Rotary, Rotary2,
//          Cursor
//------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_Readouts_H__
#define __Eaagles_BasicGL_Readouts_H__

#include "Field.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Integer.h"

namespace Eaagles {
   namespace Basic { class Float; class Integer; }
namespace BasicGL {
   class Reformat;


//------------------------------------------------------------------------------
// Class: AsciiText
// Base class: Object > Graphic > Field > AsciiText
//
// Form name: text
// Slots:
//    text   <String>     ! Text String (default: " ")
//    text   <List>       ! Text List
//
// Public Member Functions:
//      bool isValidInputPosition(int tc)
//      Returns true if tc is a valid input position.
//
//      bool setTextString(String* stsobj)
//      Returns true if the text string was set to stsobj.
//
//      bool setTextList(List* stlobj)
//      Returns true if the text list was set to stlobj.
//
//------------------------------------------------------------------------------
class AsciiText : public Field {
    DECLARE_SUBCLASS(AsciiText, Field)

public:
   AsciiText();

   virtual char filterInputEvent(const int event, const int tc);
   virtual bool isValidInputPosition(const int tc);
   virtual bool setTextString(const Basic::String* const stsobj);
   virtual bool setTextList(const Basic::List* const stlobj);

   // Component interface
   virtual bool event(const int key, Basic::Object* const obj = 0);
};

//------------------------------------------------------------------------------
// Class: Cursor
//
// Form name: Cursor
//
//------------------------------------------------------------------------------
class Cursor : public Field {
    DECLARE_SUBCLASS(Cursor,Field)

public:
   Cursor();
   virtual void updateData(const LCreal dt = 0.0f);
};


//------------------------------------------------------------------------------
// Class: NumericReadout
//
// Form name: NumericReadout
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
class NumericReadout : public Field {
   DECLARE_SUBCLASS(NumericReadout,Field)

public:
   NumericReadout(); 

   int getInt() const               { return int(num); }
   double getFloat() const          { return num; }

   void setValue(const int v)       { num = double(v); redisplay(); }
   void setValue(const double v)    { num = v; redisplay(); }

   void setMaxValue(const int v)    { maxNum = double(v); redisplay(); }
   void setMaxValue(const double v) { maxNum = v; redisplay(); }

   virtual double getInputValue() const;
   virtual bool isInputValueValid() const;

   virtual char filterInputEvent(const int event, const int tc);

   //event handler macro functions
   virtual bool onUpdateValue(const Basic::Float* const ouvobj);
   virtual bool onUpdateValue(const Basic::Integer* const ouvobj);
   virtual bool onUpdateValue(const Basic::Number* const ouvobj);
   
   // Component interface
   virtual bool event(const int key, Basic::Object* const obj = 0);
   virtual void updateData(const LCreal dt = 0.0);

protected:
   virtual bool setSlotFloatToBeDisplayed(const Basic::Float* const msg);
   virtual bool setSlotNumberToBeDisplayed(const Basic::Number* const msg);
   virtual bool setSlotFloatMaxValue(const Basic::Float* const msg);
   virtual bool setSlotNumberMaxValue(const Basic::Number* const msg);
   virtual bool setSlotExampleFormatText(const Basic::String* const msg);
   virtual bool setSlotPlusChar(const Basic::String* const msg);
   virtual bool setSlotMinusChar(const Basic::String* const msg);
   virtual bool setSlotDecimalPointChar(const Basic::String* const msg);
   virtual bool setSlotUndefinedChar(const Basic::String* const msg);
   virtual bool setSlotOverflowChar(const Basic::String* const msg);
   virtual bool setSlotMaxValid(const Basic::Number* const msg);
   virtual bool setSlotMinValid(const Basic::Number* const msg);
   virtual bool setSlotBlankZero(const Basic::Number* const msg);
   
   virtual void makeText();
   virtual void redisplay();
   virtual void reformat(const char* const example);

   static const size_t CBUF_LENGTH = 32;   // Max length of cbuf
   static const size_t FORMAT_LENGTH = 32; // Max length of format

   char cbuf[CBUF_LENGTH];       // buffer
   char format[FORMAT_LENGTH];   // Current format string

   char plusChar;                // Positive value character
   char minusChar;               // Negative value character
   char dpChar;                  // Decimal point character
   char undefinedChar;           // Undefined value character
   char overflowChar;            // Overflow character  
   bool postSign;                // If true, sign char is at end of string

   static Reformat* reformatter; // Generates format statements by example

private:
   double   num;            // Value as double
   double   maxNum;         // Maximum value
   double   maxValid;       // Maximum valid input value
   double   minValid;       // Minimum valid input value
   bool     blankZero;      // Display blank instead of zero value
};


//------------------------------------------------------------------------------
// Class: HexReadout
// Base class: Object > Graphic > Field > NumericReadout > HexReadout
//
// Form name: HexReadout
//
// Example formats: 
//
//    ######    // Hex number
//    00000#    // Hex number w/leading zeros
//
//------------------------------------------------------------------------------
class HexReadout : public NumericReadout {
   DECLARE_SUBCLASS(HexReadout,NumericReadout)

public:
   HexReadout(); 
   virtual void makeText();
   virtual char filterInputEvent(const int event, const int tc);
   virtual double getInputValue() const;

protected:
    virtual void reformat(const char* const example);
};


//------------------------------------------------------------------------------
// Class: OctalReadout
// Base class: Object > Graphic > Field > NumericReadout > OctalReadout
//
// Form name: OctalReadout
//
// Example formats: 
//
//    ######    // Octal number
//    00000#    // Octal number w/leading zeros
//
//------------------------------------------------------------------------------
class OctalReadout : public NumericReadout {
   DECLARE_SUBCLASS(OctalReadout,NumericReadout)

public:
   OctalReadout(); 
   virtual void makeText();
   virtual char filterInputEvent(const int event, const int tc);
   virtual double getInputValue() const;

protected:
    virtual void reformat(const char* const example);
};


//------------------------------------------------------------------------------
// Class: TimeReadout
// Base class: Object > Graphic > Field > NumericReadout > TimeReadout
//
// Form name: TimeReadout
//
// Example formats: 
//
//    HH:MM:SS      // Hours, minutes and seconds
//    0H:MM:SS.S    // Hours (w/leading zero), minutes and seconds decimal
//   +HH:MM         // Hours (w/sign) and minutes
//    HHMM.M        // Hours (w/sign) and minutes decimal
//   +0HMM:SS.S     // Hours (w/signed and leading zero), minutes
//                  //   and seconds decimal
//    MMSS.S        // Minutes and seconds decimal
//    SSSS          // Seconds
//
// Notes: The seconds and third numbers always have leading zeros.  The first
// number requires a zero, '0', to have leading zeros.
//
//------------------------------------------------------------------------------
class TimeReadout : public NumericReadout {
   DECLARE_SUBCLASS(TimeReadout,NumericReadout)

public:
   enum TimeMode { invalid, hhmmss, hhmm, hh, mmss, mm, ss };
   TimeReadout(); 
   virtual char filterInputEvent(const int event, const int tc);
   virtual double getInputValue() const;

protected:
   virtual void makeText();
   virtual void reformat(const char* const example);
   TimeMode tmode;
};


//------------------------------------------------------------------------------
// Class: DirectionReadout
// Base class: Object > Graphic > Field > NumericReadout > DirectionReadout
//
// Form name: DirectionReadout
// Description:  This class is a default direction readout.
// Example formats: 
// 		
// 	DDD@		// Degrees
// 	DD@MM		// Degrees, minutes
// 	DD@MMSS		// Degrees, minutes, seconds		
// 
// Notes: The @ symbol by default is left in the format, and the font is reponsible
// for mapping it to a ° symbol.  This is left this way for font sets that don't have
// special characters.  However, there is a slot "setSlotPreConvertSymbols" which will manually
// convert the @ symbols to ° symbols before drawing.  
//------------------------------------------------------------------------------
class DirectionReadout : public NumericReadout {
   DECLARE_SUBCLASS(DirectionReadout,NumericReadout)

public:
   enum DirMode { invalid, ddmmss, ddmm, dd };
   DirectionReadout();
   virtual char filterInputEvent(const int event, const int tc);
   virtual double getInputValue() const;

protected:		
   // slot to map @ symbol to degree symbol
   bool setSlotPreConvertSymbols(const Basic::Number* const x);
   
  
   virtual void makeText();
   virtual void reformat(const char* const example);
   DirMode tmode;
   bool preConvertSymbols;		// convert symbols beforhand
};


//------------------------------------------------------------------------------
// Class: LatitudeReadout
// Base class: Object > Graphic > Field > NumericReadout > DirectionReadout > LatitudeReadout
//
// Form name: LatitudeReadout
//
//------------------------------------------------------------------------------
class LatitudeReadout : public DirectionReadout {
    DECLARE_SUBCLASS(LatitudeReadout,DirectionReadout)
public:
    LatitudeReadout();
    virtual char filterInputEvent(const int event, const int tc);
protected:
   virtual void makeText();
};


//------------------------------------------------------------------------------
// Class: LongitudeReadout
// Base class:  Object > Graphic > Field > NumericReadout > DirectionReadout > LongitudeReadout
//
// Form name: LongitudeReadout
//
//------------------------------------------------------------------------------
class LongitudeReadout : public DirectionReadout {
    DECLARE_SUBCLASS(LongitudeReadout,DirectionReadout)
public:
    LongitudeReadout();
    virtual char filterInputEvent(const int event, const int tc);
protected:
   virtual void makeText();
};


//------------------------------------------------------------------------------
// Class: Rotary
// Base class: Object > Graphic > Field > Rotary
//
// Description: Rotary text
//
// Form name: Rotary
//
//------------------------------------------------------------------------------
class Rotary : public Field {
    DECLARE_SUBCLASS(Rotary,Field)

public:
   Rotary();
   // BasicGL::Graphic interface
   virtual void draw();

   // Basic::Component interface
   virtual bool event(const int key, Object* const obj = 0);

private:
    // this flag tells us our components need to be pre-drawn (to avoid flicker)
    bool preDrawSelectList;
};


//------------------------------------------------------------------------------
// Class: Rotary2
// Base class: Object > Graphic > Field > Rotary > Rotary2
//
// Description: Binary rotary
//
// Form name: Rotary2
//
//------------------------------------------------------------------------------
class Rotary2 : public Rotary {
    DECLARE_SUBCLASS(Rotary2,Rotary)

public:
   Rotary2();
   virtual bool event(const int key, Basic::Object* const obj = 0);
   //macro function for event handler
   virtual bool onSelect(const Basic::Number* const osobj);
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif
