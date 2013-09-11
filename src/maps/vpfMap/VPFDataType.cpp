// -------------------------------------------------------------------------------
// Class: VPFDataType
// -------------------------------------------------------------------------------

#include "openeaagles/maps/vpfMap/VPFDataType.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Maps {
namespace Vpf {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(VPFDataType,"VPFDataType")
EMPTY_SERIALIZER(VPFDataType)

VPFDataType::VPFDataType()
{
    STANDARD_CONSTRUCTOR()
    value = new Basic::String();
    length = 0;
}

void VPFDataType::copyData(const VPFDataType& org, const bool)
{
    BaseClass::copyData(org);
}

void VPFDataType::deleteData()
{
    if (value != 0) {
        value->unref();
        value = 0;
    }
}

void VPFDataType::setValue(char* x)
{
    value->setStr(x);
    if (length != -1) type = FIXED_TEXT;
    else type = VARIABLE_TEXT;
}

const char* VPFDataType::getValue()
{
    if (value != 0 && !value->isEmpty()) return value->getString();
    else return "";
}

// returns a float value (if we are a float!)
float VPFDataType::getFloat()
{
    float x = (float)atof(value->getString());
    return x;
}

void VPFDataType::setLength(const int x)
{
    length = x;
}

} // end Vpf namespace
} // end Maps namespace
} // end Eaagles namespace

