
#include "openeaagles/base/units/Masses.hpp"
#include "openeaagles/base/osg/Math"
#include <iostream>

namespace oe {
namespace base {

//==============================================================================
// Mass() --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Mass, "AbstractMass")
EMPTY_SLOTTABLE(Mass)
EMPTY_DELETEDATA(Mass)

Mass::Mass()
{
    STANDARD_CONSTRUCTOR()
}

Mass::Mass(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Mass::copyData(const Mass& org, const bool)
{
    BaseClass::copyData(org);
    val = fromMass(org.toMass());;
}

std::ostream& Mass::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if (!slotsOnly) {
        indent(sout, i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    indent(sout, i+j);
    sout << val << std::endl;

    BaseClass::serialize(sout, i+j, true);

    if (!slotsOnly) {
        indent(sout, i);
        sout << ")" << std::endl;
    }
    return sout;
}

//==============================================================================
// KiloGrams() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloGrams, "KiloGrams")
EMPTY_SERIALIZER(KiloGrams)
EMPTY_COPYDATA(KiloGrams)
EMPTY_DELETEDATA(KiloGrams)

KiloGrams::KiloGrams() : Mass()
{
    STANDARD_CONSTRUCTOR()
}

KiloGrams::KiloGrams(const double value) : Mass(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloGrams::KiloGrams(const Mass& value) : Mass()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Grams() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Grams, "Grams")
EMPTY_SERIALIZER(Grams)
EMPTY_COPYDATA(Grams)
EMPTY_DELETEDATA(Grams)

Grams::Grams() : Mass()
{
    STANDARD_CONSTRUCTOR()
}

Grams::Grams(const double value) : Mass(value)
{
    STANDARD_CONSTRUCTOR()
}

Grams::Grams(const Mass& value) : Mass()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Slugs() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Slugs, "Slugs")
EMPTY_SERIALIZER(Slugs)
EMPTY_COPYDATA(Slugs)
EMPTY_DELETEDATA(Slugs)

Slugs::Slugs() : Mass()
{
    STANDARD_CONSTRUCTOR()
}

Slugs::Slugs(const double value) : Mass(value)
{
    STANDARD_CONSTRUCTOR()
}

Slugs::Slugs(const Mass& value) : Mass()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
