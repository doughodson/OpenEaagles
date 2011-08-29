#include "openeaagles/instruments/dials/DialPointer.h"
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace Instruments {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DialPointer, "DialPointer")
EMPTY_SERIALIZER(DialPointer)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
DialPointer::DialPointer()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
EMPTY_COPYDATA(DialPointer)

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(DialPointer)

//------------------------------------------------------------------------------
// draw() -- draws the object(s)
//------------------------------------------------------------------------------
void DialPointer::draw()
{
    LCreal pointerPos = getInstValue();
    LCreal startAngle = getStartAngle();
    lcSaveMatrix();
        lcRotate((-pointerPos - startAngle) * (LCreal) Basic::Angle::D2RCC);
        BasicGL::Graphic::draw();
    lcRestoreMatrix();
}

}  // end of Instruments namespace
}  // end of Eaagles namespace
