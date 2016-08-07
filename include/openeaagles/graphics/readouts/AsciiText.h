
#ifndef __oe_graphics_AsciiText_H__
#define __oe_graphics_AsciiText_H__

#include "openeaagles/graphics/Field.h"

namespace oe {
namespace base { class Object; class List; class String; }

namespace graphics {

//------------------------------------------------------------------------------
// Class: AsciiText
//
// Factory name: text
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
class AsciiText : public Field
{
    DECLARE_SUBCLASS(AsciiText, Field)

public:
   AsciiText();

   virtual char filterInputEvent(const int event, const int tc) override;
   virtual bool isValidInputPosition(const int tc) override;
   virtual bool event(const int key, base::Object* const obj = nullptr) override;

   virtual bool setTextString(const base::String* const stsobj);
   virtual bool setTextList(const base::List* const stlobj);
};

}
}

#endif
