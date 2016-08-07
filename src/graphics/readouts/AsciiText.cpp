
#include "openeaagles/graphics/readouts/AsciiText.h"

#include "openeaagles/base/List.h"
#include "openeaagles/base/String.h"

namespace oe {
namespace graphics {

IMPLEMENT_SUBCLASS(AsciiText, "AsciiText")
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
   ON_EVENT_OBJ(UPDATE_VALUE, setTextString, base::String)
END_EVENT_HANDLER()

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

base::Object* AsciiText::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

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

}
}
