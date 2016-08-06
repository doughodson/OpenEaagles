//------------------------------------------------------------------------------
// Class: Field
//------------------------------------------------------------------------------
#ifndef __oe_graphics_Field_H__
#define __oe_graphics_Field_H__

#include "Graphic.h"
#include "openeaagles/base/String.h"

namespace oe {

   namespace base {
      class List;
   }

namespace graphics {

//------------------------------------------------------------------------------
// Class: Field
//
// Description: Abstract Field class
//
// Factory name: Field
// Slots:
//      position       <List>   ! Starting position ( Line Column ) (default: 0)
//      width          <Number> ! Field width (default: 0)
//      highLight      <Number> ! Highlight text flag  (Display mode = 0x1001, default 0)
//      underline      <Number> ! Underlined text flag  (Display mode = 0x1002, default 0)
//      reversed       <Number> ! Reversed video flag  (Display mode = 0x1004, default 0)
//      justification  <String> ! "none", "left", "center", or "right" (default: "none")
//      vertical       <Number> ! vertical field flag (text displayed vertically) (Display mode = 0x1010, default 0)
//      brackets       <Number> ! Bracket the field with a character just left
//                              ! and right of this field. (Display mode = 0x1020, default = 0)
//      linked         <Number> ! Linked flag (auto step to next input field) (default = false)
//      inheritColor   <Number> ! inherit color of our container (default: false)
//      font           <String> ! specific font name we wish to use (default: 0)
//      startCharPos   <Number> ! Our starting character position (we may want to skip!) (default: 0)
//
//
// Public member functions:
//
//  operator char*() { return str; }
//      Conversion function to allow str to be returned as a char*.
//
//  int line(int ll)
//      Sets the line number.
//
//  int line()
//      Sets the line number.
//
//  int column(int cc)
//      Sets the column number.
//
//  int column()
//      Sets the column number.
//
//  size_t width()
//      Returns the field width.
//
//  size_t width(size_t t)
//      Sets and returns the field width to size t.
//
//  Mode getMode()
//      Returns the mode of the field.
//
//  Mode setMode(Mode newMode);
//      Sets the mode to newMode.
//
//  isHighLighted()
//      Returns true if field is highlighted.
//
//  isUnderlined()
//      Returns true if field is underlined.
//
//  isReversed()
//      Returns true if field is reversed.
//
//  isSpecial()
//      Returns true if field is special.
//
//  isVertical()
//      Returns true if field is vertical.
//
//  areBracketsOn()
//      Returns true if brackets are on.
//
//  isDisplayMode(int t)
//      Returns true if display mode == t.
//
//  setDisplayMode(int t)
//      Adds display mode t to the current display mode. (combination, not addition)
//
//  clearDisplayMode(int t)
//      Removes display mode t from the current display mode.
//
//  clearAllDisplayModes()
//      Removes all special display modes.
//
//  String::Justify justification()
//      Returns the current justification mode.
//
//  String::Justify justification(String::Justify t)
//      Sets justification mode to t.
//
//  setPosition(List* spobj)
//      Sets the Position to [Line Column] specified by spobj.
//
//  withinField(int ln, int cp)
//      Returns true if location specified is within the current field.
//
//  setText(char newString[])
//      Sets the field's text string.
//
//  int setExample(char* example)
//      Sets the input example to example, and returns the length of example.
//
//  char getChar()
//      Gets the character if in input mode.
//
//  setChar(char c)
//      Sets the character to c if in input mode.
//
//  backSpace(int ns = 1)
//      Backs up one space if in input mode.
//
//  advanceSpace(int ns = 1)
//      Advance one space if in input mode.
//
//  onForwardSpace()
//      Calls advanceSpace() and returns true.
//
//  onBackSpace()
//      Calls backSpace() and returns true.
//
//  char filterInputEvent(int event, int tc)
//      Filters input event 'event' using template character 'tc'.
//
//  isValidInputPosition(int tc)
//      Makes sure the current input position is valid.
//
//  double getInputValue()
//      Returns 0.0. /* This will need revision once the function does something */
//
//  isInputValueValid()
//      Returns true. /* This will need revision once the function does something */
//
//  int getInputCharacterPosition()
//      Returns the current input character position.
//
//  isLinked()
//      Returns true if linked.
//
//  setLinked(bool f)
//      Sets linked to f.
//
//  isInheritColor()
//      Returns true if inheritColor is on.
//
//  setInheritColor(bool i)
//      Sets the inheritColor to i, then returns true.
//
//  cursor(int* ln, int* cp)
//      Returns true if text cursor should be seen within this
//        object and the position of the cursor.
//
//  onSetLine(Number* oslobj)
//      Sets the line number and returns true.
//
//  onSetColumn(Number* oscobj)
//      Sets the column number and returns true.
//
//
//Comment section last modified: 2004.10.13 by MJK
//------------------------------------------------------------------------------
class Field : public Graphic
{
   DECLARE_SUBCLASS(Field,Graphic)

public:
   enum Mode { display, input };

   enum {
          // display modes
          highlight  = 0x0001,
          underline  = 0x0002,
          reversed   = 0x0004,
          special    = 0x0008,
          vertical   = 0x0010,
          brackets   = 0x0020,

          // Internal use: display mode was set by the setSlotXXXX()
          highlight1 = 0x1001,
          underline1 = 0x1002,
          reversed1  = 0x1004,
          special1   = 0x1008,
          vertical1  = 0x1010,
          brackets1  = 0x1020
        };

public:
   Field();

   operator char*()                     { return str; }
   operator const char*() const         { return str; }

   virtual int line(const int ll);
   virtual int line() const;

   virtual int column(const int cc);
   virtual int column() const;

   size_t width() const                   { return w; }
   size_t width(const size_t t)           { w = t; adjust(); return w; }

   Mode getMode() const                   { return mode; }
   Mode setMode(const Mode newMode);

   bool isHighLighted() const             { return (dmode & highlight) != 0; }
   bool isUnderlined() const              { return (dmode & underline) != 0; }
   bool isReversed() const                { return (dmode & reversed) != 0;  }
   bool isSpecial() const                 { return (dmode & special) != 0;   }
   bool isVertical() const                { return (dmode & vertical) != 0;  }
   bool areBracketsOn() const             { return (dmode & brackets) != 0;  }
   bool isDisplayMode(const int t) const  { return (dmode & t) != 0; }
   void setDisplayMode(const int t)       { dmode |= t; }
   void clearDisplayMode(const int t)     { dmode &= ~t; }
   void clearAllDisplayModes()            { dmode = 0; }

   base::String::Justify justification() const;
   base::String::Justify justification(const base::String::Justify t);

   virtual bool setPosition(const base::List* const spobj);

   virtual bool withinField(const int ln, const int cp) const;
   virtual void setText(const char newString[]);
   virtual int setExample(const char* const example);
   virtual char getChar();
   virtual void setChar(const char c);

   virtual void backSpace(const int ns = 1);
   virtual void advanceSpace(const int ns = 1);
   virtual bool onForwardSpace();
   virtual bool onBackSpace();

   virtual char filterInputEvent(const int event, const int tc);
   virtual bool isValidInputPosition(const int tc);

   virtual double getInputValue() const;
   virtual bool isInputValueValid() const;
   int getInputCharacterPosition() const            { return icp; }

   bool setInputCharacterPosition(const unsigned int);

   bool isLinked() const                            { return linked; }
   void setLinked(const bool f)                     { linked = f; }

   bool isInheritColor() const                      { return inheritColor; }
   bool setInheritColor(const bool i)               { inheritColor = i; return true; }

   virtual bool onSetLine(const base::Number* const oslobj);
   virtual bool onSetColumn(const base::Number* const oscobj);

   const base::String& getInputExample() const  { return inputExample; }
   int getCharacterPos() const                   { return icp; }

   virtual bool cursor(int* ln, int* cp) const override;
   virtual void drawFunc() override;
   virtual bool event(const int event, Object* const obj = nullptr) override;

   virtual void updateData(const double dt = 0.0) override;

protected:
   //slot table macro functions
   virtual bool setSlotWidth(const base::Number* const swobj);
   virtual bool setSlotHighlight(const base::Number* const shobj);
   virtual bool setSlotUnderline(const base::Number* const suobj);
   virtual bool setSlotReversed(const base::Number* const srobj);
   virtual bool setSlotJustification(const base::String* const sjobj);
   virtual bool setSlotVertical(const base::Number* const ssobj);
   virtual bool setSlotBrackets(const base::Number* const obj);
   virtual bool setSlotLinked(const base::Number* const msg);
   virtual bool setSlotInheritColor(const base::Number* const ic);
   virtual bool setSlotFont(const base::String* const font);
   virtual bool setSlotStartCharPos(const base::Number* const x);

   base::String origStr;        // Original text saved by setText
   base::String inputExample;   // Input Template String
   base::String str;            // Text stored in field
   Mode     mode;                // Current mode
   int      icp;                 // Input character pointer
   int      inpDspMode;          // Auto switches to this display mode during input mode
   bool     inpModeHold;         // Hold input mode until after first input character

private:
   void adjust()  { str.setString(origStr,w,jmode); }

   int      ln;                     // Line this field is on
   int      cp;                     // Starting character position of field
   size_t   w;                      // Width of the field
   bool     linked;                 // Linked field (on input)
   unsigned int startCP;            // our starting character position that we "write" from

   int      dmode;                  // Display mode flags
   base::String::Justify jmode;    // Justification mode
   bool     inheritColor;           // Inherit color instead of using a default color
   base::String* fontName;         // name of the font we want our display to use (if overridden)
};

}
}

#endif
