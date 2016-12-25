#include "openeaagles/instruments/dials/DialPointer.hpp"
#include "openeaagles/base/units/Angles.hpp"

namespace oe {
namespace instruments {

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
    double pointerPos = getInstValue();
    double startAngle = getStartAngle();
    lcSaveMatrix();
        lcRotate((-pointerPos - startAngle) * static_cast<double>(base::Angle::D2RCC));
        graphics::Graphic::draw();
    lcRestoreMatrix();
}

}
}
