//------------------------------------------------------------------------------
// Class: Field
//------------------------------------------------------------------------------
#include "openeaagles/basicGL/Field.h"
#include "openeaagles/basicGL/Display.h"
#include "openeaagles/basicGL/Page.h"
#include "openeaagles/basicGL/Display.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Rgb.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_SUBCLASS(Field,"Field")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Field)
    "position",         // 1) Starting Position ( ln cp )
    "width",            // 2) Field width
    "highLight",        // 3) Highlight text flag
    "underline",        // 4) Underlined text flag
    "reversed",         // 5) Reversed text flag
    "justification",    // 6) "none", "left", "center", or "right"
    "vertical",         // 7) vertical field flag (text displayed vertically)
    "brackets",         // 8) Bracket flag
    "linked",           // 9) Linked flag (auto step to next input field)
    "inheritColor",     //10) Inherit color of our container (instead of using default color)
    "font",             //11) Type of font to use before drawing
    "startCharPos",     //12) Our starting character position (we may want to skip!)
END_SLOTTABLE(Field)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Field)
    ON_SLOT(1, setPosition, Basic::List)
    ON_SLOT(2, setSlotWidth, Basic::Number)
    ON_SLOT(3, setSlotHighlight, Basic::Number)
    ON_SLOT(4, setSlotUnderline, Basic::Number)
    ON_SLOT(5, setSlotReversed, Basic::Number)
    ON_SLOT(6, setSlotJustification, Basic::String)
    ON_SLOT(7, setSlotVertical, Basic::Number)
    ON_SLOT(8, setSlotBrackets, Basic::Number)
    ON_SLOT(9, setSlotLinked, Basic::Number)
    ON_SLOT(10, setSlotInheritColor,Basic::Number)
    ON_SLOT(11, setSlotFont, Basic::String)
    ON_SLOT(12, setSlotStartCharPos, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Event Table
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Field) 
    if (mode == input) {
        bool kb = ( _event >= 0x20 && _event <= 0x7f );
        ON_EVENT(FORWARD_SPACE,onForwardSpace)
        ON_EVENT(BACK_SPACE,onBackSpace)
        // Keyboard Entry
        if (kb) {
            // Filter the input event -- that is, let a virtual member
            // function filter the input event using the current template
            // character.
            char nc = filterInputEvent(_event,inputExample.getChar(icp));
            if (nc != '\0') {
                setChar(nc);
                _used = true;
            }
        }
    }
    ON_EVENT_OBJ(SET_POSITION,setPosition,Basic::List)  
    ON_EVENT_OBJ(SET_LINE,onSetLine,Basic::Number)
    ON_EVENT_OBJ(SET_COLUMN,onSetColumn,Basic::Number)
    ON_EVENT_OBJ(SET_WIDTH,setSlotWidth,Basic::Number)   
    ON_EVENT_OBJ(SET_HIGHLIGHT,setSlotHighlight,Basic::Number)   
    ON_EVENT_OBJ(SET_UNDERLINE,setSlotUnderline,Basic::Number)
    ON_EVENT_OBJ(SET_REVERSED,setSlotReversed,Basic::Number)
    ON_EVENT_OBJ(SET_JUSTIFICATION,setSlotJustification,Basic::String)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Field::Field() : origStr(), inputExample(), str()
{
    STANDARD_CONSTRUCTOR()
    ln = 0;
    cp = 0;
    w  = 0;
    dmode = 0;
    jmode = Basic::String::NONE;
    mode = display;
    icp = 0;
    inpDspMode = 0;
    inpModeHold = false;
    linked = false;
    inheritColor = false;
    fontName = 0;
    startCP = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Field::copyData(const Field& org, const bool)
{
    BaseClass::copyData(org);
    origStr = org.origStr;
    inputExample = org.inputExample;
    ln      = org.ln;
    cp      = org.cp;
    w       = org.w;
    str     = org.str;
    dmode   = org.dmode;
    jmode   = org.jmode;
    mode    = org.mode;
    icp     = org.icp;
    inpDspMode = org.inpDspMode;
    inpModeHold = org.inpModeHold;
    linked = org.linked;
    inheritColor = org.inheritColor;
    startCP = org.startCP;
    setSlotFont(org.fontName);
}

//------------------------------------------------------------------------------
//deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Field::deleteData()
{
    origStr.empty();
    inputExample.empty();
    ln = 0;
    cp = 0;
    w  = 0;
    str.empty();
    dmode = 0;
    jmode = Basic::String::NONE;
    mode  = display;
    icp   = 0;
    inpDspMode = 0;
    inpModeHold = false;
    setSlotFont(0);
}

//------------------------------------------------------------------------------
// updateData() -- Update non-time critical (background) stuff here
//------------------------------------------------------------------------------
void Field::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);
    
    // ---
    // Set display mode when this field is in 'input' mode
    // ---
    if (inpDspMode != 0) {
        // Auto switch ON input display mode?
        if ( (mode == input) &&  (icp > 0 || !inpModeHold) ) {
            setDisplayMode( inpDspMode );
        }

        // Auto swtich OFF input display mode?
        if ( (mode == display) || (inpModeHold && icp == 0) ) {
            // If it wasn't previously set by a setSlot() function then turn it off.
            if ( !isDisplayMode(0x1000 & inpDspMode) ) {
                clearDisplayMode( inpDspMode );
            }
        }
    }

    // ---
    // Update readout during input mode?
    // ---
    if ( (mode == input) && inpModeHold && icp == 0 ) {
        adjust();
    }
}

//------------------------------------------------------------------------------
// line() -- set the line number
// column() -- set the column number
//------------------------------------------------------------------------------
int Field::line(const int ll)
{
    return (ln = ll);
}

int Field::line() const
{
    return ln;
}

int Field::column(const int cc)
{
    return (cp = cc);
}

int Field::column() const
{
    return cp;
}


//------------------------------------------------------------------------------
// withinField() -- Return True/False if cp,ln is in the field space
//------------------------------------------------------------------------------
bool Field::withinField(const int l, const int c) const
{
    bool stat = false;
    if ( (l == ln) && (c >= cp) && (c <= (cp + int(w) - 1)) ) stat = true;
    return stat;
}


//------------------------------------------------------------------------------
// setText() -- set the field's text string
//------------------------------------------------------------------------------
void Field::setText(const char newString[])
{
    origStr = newString;
    if (mode == display) {
        adjust();
    }
}


//------------------------------------------------------------------------------
// justification() --
//------------------------------------------------------------------------------
Basic::String::Justify Field::justification() const
{
    return jmode;
}

Basic::String::Justify Field::justification(const Basic::String::Justify t)
{
    jmode = t;
    if (mode == display) adjust();
    return jmode;
}


//------------------------------------------------------------------------------
// setMode() -- set the mode of the field (display, input)
//------------------------------------------------------------------------------
Field::Mode Field::setMode(const Field::Mode nmode)
{
    Mode omode = mode;
    mode = nmode;

    if (nmode == input && omode == display) {

        // When we're entering the INPUT mode ...

        // Change the input focus to this field
        getDisplay()->focus(this);

        // Set the input chararcter pointer to the first character
        if (startCP > 0 && startCP < w) icp = startCP;
        else icp = 0;

    }
    else if (nmode == display && omode == input) {

        // When we're leaving the INPUT mode ...

        // Change the input focus to our container page (Page)
        if (getDisplay()->focus() == this) {
            Page* page = (Page*) findContainerByType(typeid(Page));
            if (page != 0)
                getDisplay()->focus(page);
            else
                getDisplay()->focus(0);

            // Reset text string
            adjust();

        }
    }


    return omode;
} 


//------------------------------------------------------------------------------
// input mode functions --
//------------------------------------------------------------------------------

// isValidInputPosition() -- Makes sure the position is valid for input
bool Field::isValidInputPosition(const int tc)
{
    return (tc == '+' || tc == '0' || tc == '#' ||
            tc == 'D' || tc == 'H' || tc == 'M' || tc == 'S');
}


// filterInputEvent() -- Filter input events using a template character (tc)
char Field::filterInputEvent(const int event, const int tc)
{
    if (tc == '+') {
        // Default sign keys
        if (event == '7')
            return '-';
        else if (event == '9')
            return '+';
        else
            return '\0';
    }

    else if (tc == '0' || tc == '#') {
        // Default numeric keys
        if (event >= '0' && event <= '9')
            return char(event);
        else
            return '\0';
    }

    else
        return '0';

}

int Field::setExample(const char* const example)
{
    inputExample = example;
    return int(inputExample.len());
}

void Field::advanceSpace(const int ns)
{
    if (mode != input) return;
    icp += ns;
    while ( icp < int(w) && !isValidInputPosition(inputExample.getChar(icp)) ) icp++;
    if (icp >= int(w)) {
        icp = int(w)-1;
        while ( !isValidInputPosition(inputExample.getChar(icp)) ) icp--;
        event(INPUT_RIGHT_EDGE);
    }
}

void Field::backSpace(const int ns)
{
   if (mode != input) return;
   // if we are backspacing, and we are at the starting character position that was set, we stay there!
   if (startCP > 0 && icp == startCP) {
      event(INPUT_LEFT_EDGE);
   }
   else {
      icp -= ns;
      while ( icp >= 0 && !isValidInputPosition(inputExample.getChar(icp)) ) icp--;
      if (icp < 0) {
         icp = 0;
         while ( !isValidInputPosition(inputExample.getChar(icp)) ) icp++;
         event(INPUT_LEFT_EDGE);
      }
   }
}

bool Field::setInputCharacterPosition(const unsigned int ii)
{
    if (startCP > 0) {
        if (ii >= startCP) icp = ii;
    }
    else if (ii < w) icp = ii;
    return true;
}
  
                                       
char Field::getChar()
{
    if (mode == input)
        return str.getChar(icp);
    else
        return '\0';
}

void Field::setChar(const char c)
{
    if (mode != input) return;
    str.setChar(icp,c);
    advanceSpace();
}

double Field::getInputValue() const
{
    return 0.0;
}

bool Field::isInputValueValid() const
{
    return true;
}


//------------------------------------------------------------------------------
// onForwardSpace() -- 
//------------------------------------------------------------------------------
bool Field::onForwardSpace()
{
    advanceSpace();
    return true;
}

//------------------------------------------------------------------------------
// onBackSpace() -- 
//------------------------------------------------------------------------------
bool Field::onBackSpace()
{
    backSpace();
    return true;
}


//------------------------------------------------------------------------------
// cursor() -- Returns true if text cursor should be seen within this
//	       object and the position of the cursor. 
//------------------------------------------------------------------------------
bool Field::cursor(int* l, int* c) const
{
    if (mode == input) {
        *l = ln;
        *c = cp + icp;
        return true;
    }
    else {
        *l = 0;
        *c = 0;
        return false;
    }
}


//------------------------------------------------------------------------------
// drawFunc -- draw this text field
//------------------------------------------------------------------------------
void Field::drawFunc()
{
    // Get a pointer to the current display
    BasicGL::Display* dsp = getDisplay();
    if (dsp == 0) return;

    // ---
    // When our container is also a Field, get a pointer to it.
    // ---
    BasicGL::Field* parent = 0;
    if (container() != 0) {
        BasicGL::Field* fp = dynamic_cast<BasicGL::Field*>(container());
        if (fp != 0) parent = fp;
    }

    // ---
    // If we don't have a position, try to get one from our container
    // ---
    int ll = line();
    int cc = column();
    if (ll == 0 && parent != 0) {
        ll = parent->line();
        cc = parent->column();
    }

    // ---
    // Select the correct font based on font name if there is one, and if not, then do it normally
    // ---
    if (fontName != 0) dsp->selectFont(isReversed(), isUnderlined(), dsp->getFont(fontName->getString()));    
    else dsp->selectFont(isReversed(), isUnderlined());
    
    
    // ---
    // Set the color
    // ---
    bool restoreColor = false;
    osg::Vec4 ocolor = dsp->getCurrentColor();
    // only use default colors if we aren't inheriting our container's colors

    if (!isInheritColor()) {
        if (getColorName() == 0 && getColor() == 0) {
            const Basic::Color* cc = 0;
            if (isHighLighted()) cc = dsp->getHighlightColor();
            else cc = dsp->getNormColor();
            if (cc != 0) {
                const osg::Vec4* p = cc->getRGBA();
                dsp->setColor(*p);
                restoreColor = true;
            }
        }
    }

    // ---
    // draw the string
    // ---
    
    if (str.len() > 0) {
        // Draw the text string
        const char* sp = str;
        if (ll > 0 && cc > 0)
            dsp->outputTextLC(ll, cc, sp, int(width()), isVertical());
        else
            dsp->outputText(sp, int(width()), isVertical());
    }

    // ---
    // draw the brackets
    // ---
    if (areBracketsOn() && ll > 0 && cc > 0) {
        if (isVertical()) {
            // Position for vertical text
            dsp->drawLeftBracket(ll-1, cc);
            dsp->drawRightBracket(ll+int(width()), cc);
        }
        else {
            // Position for normal text
            dsp->drawLeftBracket(ll, cc-1);
            dsp->drawRightBracket(ll, cc+int(width()));
        }
    }

    // ---
    // If we used default colors, restore the old value
    // ---
    if (restoreColor) dsp->setColor(ocolor);

}




//------------------------------------------------------------------------------
// getSlotByIndex() for Field
//------------------------------------------------------------------------------
Basic::Object* Field::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// setPosition() -- set position: [ Line Column ]
//------------------------------------------------------------------------------
bool Field::setPosition(const Basic::List* const spobj)
{
    bool ok = true;
    if (spobj != 0) {
        int values[2];
        int n = spobj->getNumberList(values, 2);
            if (n >= 2) {
                line(values[0]);
                column(values[1]);
            }
            else {
                 if (isMessageEnabled(MSG_ERROR)) {
                std::cerr << "Field::setPosition: not enough data to process list" << std::endl;
                 }
                ok = false;   
            }
    }
    return ok;
}


//------------------------------------------------------------------------------
// setLine() -- 
//------------------------------------------------------------------------------
bool Field::onSetLine(const Basic::Number* const oslobj)
{
    if (oslobj != 0) line(oslobj->getInt());
    return true;
}

//------------------------------------------------------------------------------
// setColumn() -- 
//------------------------------------------------------------------------------                          
bool Field::onSetColumn(const Basic::Number* const oscobj)
{
   if (oscobj != 0) column(oscobj->getInt());
   return true;
}
                                   
//------------------------------------------------------------------------------
// setSlotWidth() -- 
//------------------------------------------------------------------------------                          
bool Field::setSlotWidth(const Basic::Number* const swobj)
{
    
    if (swobj != 0) width(swobj->getInt());
    return true;
}

//------------------------------------------------------------------------------
// setSlotHighlight() -- 
//------------------------------------------------------------------------------                          
bool Field::setSlotHighlight(const Basic::Number* const shobj)
{
    if (shobj != 0) {
        // Set our mode
        if (shobj->getBoolean()) {
            setDisplayMode(highlight);
            setDisplayMode(highlight1);
        }
        else {
            setDisplayMode(highlight);
            setDisplayMode(highlight1);
        }

        Basic::PairStream* subcomponents = getComponents();
        if (subcomponents != 0) {

            const Basic::List::Item* item = subcomponents->getFirstItem();
            while (item != 0) {
                Basic::Pair* p = (Basic::Pair*) item->getValue();
                Field* child = dynamic_cast<Field*>(p->object());
                if (child != 0) child->setSlotHighlight(shobj); //changed from obj
                item = item->getNext();
            }

            subcomponents->unref();
            subcomponents = 0;
        }
    }
    return true;
}   

//------------------------------------------------------------------------------
// setSlotUnderline() -- 
//------------------------------------------------------------------------------                          
bool Field::setSlotUnderline(const Basic::Number* const suobj)
{
    if (suobj != 0) {

        // Set our mode
        if (suobj->getBoolean()) {
            setDisplayMode(underline);
            setDisplayMode(underline1);
        }
        else {
            clearDisplayMode(underline);
            clearDisplayMode(underline1);
        }

        // Set our children's mode
        Basic::PairStream* subcomponents = getComponents();
        if (subcomponents != 0) {

            const Basic::List::Item* item = subcomponents->getFirstItem();
            while (item != 0) {
                Basic::Pair* p = (Basic::Pair*) item->getValue();
                Field* child = dynamic_cast<Field*>(p->object());
                if (child != 0) child->setSlotUnderline(suobj);
                item = item->getNext();
            }

            subcomponents->unref();
            subcomponents = 0;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotReversed() -- 
//------------------------------------------------------------------------------                          
bool Field::setSlotReversed(const Basic::Number* const srobj)
{
   
    if (srobj != 0) {

        // Set our mode
        if (srobj->getBoolean()) {
            setDisplayMode(reversed);
            setDisplayMode(reversed1);
        }
        else {
            clearDisplayMode(reversed);
            clearDisplayMode(reversed1);
        }

        // Set our children's mode
        Basic::PairStream* subcomponents = getComponents();
        if (subcomponents != 0) {

            const Basic::List::Item* item = subcomponents->getFirstItem();
            while (item != 0) {
                Basic::Pair* p = (Basic::Pair*) item->getValue();
                Field* child = dynamic_cast<Field*>(p->object());
                if (child != 0) child->setSlotReversed(srobj);
                item = item->getNext();
            }

            subcomponents->unref();
            subcomponents = 0;
        }
    }
    return true;
}


//------------------------------------------------------------------------------
// setSlotVertical() -- 
//------------------------------------------------------------------------------                          
bool Field::setSlotVertical(const Basic::Number* const ssobj)
{
    if (ssobj != 0) {
        // Set our mode
        if (ssobj->getBoolean()) {
            setDisplayMode(vertical);
            setDisplayMode(vertical1);
        }
        else {
            clearDisplayMode(vertical);
            clearDisplayMode(vertical1);
        }
    }
    return true;
}


//------------------------------------------------------------------------------
// setSlotBrackets() -- 
//------------------------------------------------------------------------------                          
bool Field::setSlotBrackets(const Basic::Number* const ssobj)
{
    if (ssobj != 0) {
        // Set our mode
        if (ssobj->getBoolean()) {
            setDisplayMode(brackets);
            setDisplayMode(brackets1);
        }
        else {
            clearDisplayMode(brackets);
            clearDisplayMode(brackets1);
        }
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotLinked() -- 
//------------------------------------------------------------------------------                          
bool Field::setSlotLinked(const Basic::Number* const msg)
{
    if (msg != 0) {
        setLinked( msg->getBoolean() );
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotInheritColor() -- 
//------------------------------------------------------------------------------                          
bool Field::setSlotInheritColor(const Basic::Number* const ic)
{
    bool ok = false;
    if (ic != 0) {
        ok = setInheritColor(ic->getBoolean());
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotJustification() -- 
//------------------------------------------------------------------------------                          
bool Field::setSlotJustification(const Basic::String* const sjobj)
{
    bool ok = true;
    if (sjobj != 0) {

        // Set our justification
        if ( *sjobj == "none" )
            justification(Basic::String::NONE);
        else if ( *sjobj == "left" )
            justification(Basic::String::LEFT);
        else if ( *sjobj == "center" )
            justification(Basic::String::CENTER);
        else if ( *sjobj == "right" )
            justification(Basic::String::RIGHT);
        else {
              if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Field::setJustification: No proper inputs" << std::endl;
              }
            ok = false;
        }

        // Set our children's justification
        Basic::PairStream* subcomponents = getComponents();
        if (subcomponents != 0) {

            const Basic::List::Item* item = subcomponents->getFirstItem();
            while (item != 0) {
                Basic::Pair* p = (Basic::Pair*) item->getValue();
                Field* child = dynamic_cast<Field*>(p->object());
                if (child != 0) child->setSlotJustification(sjobj);
                item = item->getNext();
            }

            subcomponents->unref();
            subcomponents = 0;
        }
    }
    return ok;
}

bool Field::setSlotFont(const Basic::String* const font)
{
    bool ok = false;
    if (fontName != 0) fontName->unref();
    fontName = 0;
    if (font != 0) {
        fontName = font->clone();
        ok = true;
    }
    
    return ok;
}

bool Field::setSlotStartCharPos(const Basic::Number* const msg) {
    bool ok = false;
    if (msg != 0) {
        int ii = msg->getInt();
        if (ii > 0) {
           // come in as 1 based, convert to 0 based
           startCP = (unsigned int) (ii - 1);
           ok = true;
        }
    }
    
    return ok;
}

//------------------------------------------------------------------------------
// serialize() -- print functions
//------------------------------------------------------------------------------
std::ostream& Field::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    if (line() > 0 && column() > 0) {
        indent(sout,i+j);
        sout << "position: [ " << line() << " " << column() << " ]" << std::endl;
    }

    if ( width() > 0) {
        indent(sout,i+j);
        sout << "width: "	<< (unsigned int)width()	<< std::endl;
    }

    if ( isHighLighted() ) {
        indent(sout,i+j);
        sout << "highLight: " << isHighLighted() << std::endl;
    }

    if ( isUnderlined() ) {
        indent(sout,i+j);
        sout << "underline: " << isUnderlined() << std::endl;
    }

    if ( isReversed() ) {
        indent(sout,i+j);
        sout << "reversed: " << isReversed() << std::endl;
    }

    if ( isVertical() ) {
        indent(sout,i+j);
        sout << "vertical: " << isVertical() << std::endl;
    }

    if ( areBracketsOn() ) {
        indent(sout,i+j);
        sout << "brackets: " << areBracketsOn() << std::endl;
    }

    indent(sout,i+j);
    sout << "justification: ";
    switch (jmode) {
        case Basic::String::NONE   : sout << "\"none\"";    break;
        case Basic::String::LEFT   : sout << "\"left\"";    break;
        case Basic::String::CENTER : sout << "\"center\"";  break;
        case Basic::String::RIGHT  : sout << "\"right\"";   break;
    }
    sout << std::endl;

    indent(sout,i+j);
    sout << "startCharPos: " << startCP << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }
    return sout;
}

} // End BasicGL namespace
} // End Eaagles namespace
