// Masses.cpp: implementation of the Mass class.
// Grams, KiloGrams, Slugs
//////////////////////////////////////////////////////////////////////

#include "openeaagles/base/units/Masses.h"
#include "openeaagles/base/osg/Math"
#include "openeaagles/base/SlotTable.h"

namespace oe {
namespace base {

//////////////////////////////////////////////////////////////////////
// Mass() --
//////////////////////////////////////////////////////////////////////
IMPLEMENT_ABSTRACT_SUBCLASS(Mass, "Mass")
EMPTY_SLOTTABLE(Mass)

// Conversion constant
const double Mass::KG2G = 1000.0;                 // KiloGrams => Grams
const double Mass::G2KG = (1 / Mass::KG2G);     // Grams => KiloGrams
const double Mass::KG2SL = 0.06852176585f;        // KiloGrams => Slugs
const double Mass::SL2KG = (1 / Mass::KG2SL);   // Slugs => KiloGrams
const double Mass::KG2PM = 2.2046f;              // KiloGrams => PoundsMass
const double Mass::PM2KG = (1 / Mass::KG2PM);   // PoundsMass => KiloGrams

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------

Mass::Mass()
{
    STANDARD_CONSTRUCTOR()
}


Mass::Mass(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- 
//------------------------------------------------------------------------------
void Mass::copyData(const Mass& org, const bool)
{
    BaseClass::copyData(org);
    val = fromMass(org.toMass());;
}

//------------------------------------------------------------------------------
// deletedDataFunc() --
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Mass)

//------------------------------------------------------------------------------
// serialize()
//------------------------------------------------------------------------------
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

EMPTY_COPYDATA(KiloGrams)
EMPTY_DELETEDATA(KiloGrams)

//==============================================================================
// Grams() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Grams, "Grams")
EMPTY_SERIALIZER(Grams)

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

EMPTY_COPYDATA(Grams)
EMPTY_DELETEDATA(Grams)

//==============================================================================
// Slugs() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Slugs, "Slugs")
EMPTY_SERIALIZER(Slugs)

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

EMPTY_COPYDATA(Slugs)
EMPTY_DELETEDATA(Slugs)

} // End base namespace
}
