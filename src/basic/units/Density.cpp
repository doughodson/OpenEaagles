#include "openeaagles/basic/units/Density.h"
#include "openeaagles/basic/SlotTable.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// Density --
//==============================================================================
IMPLEMENT_SUBCLASS(Density, "Density")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Density)
    "mass",    // 1: Mass
    "volume",  // 2: Volume
END_SLOTTABLE(Density)

// Map slot table to handles 
BEGIN_SLOT_MAP(Density)
    ON_SLOT(1, setSlotMass, Mass)
    ON_SLOT(2, setSlotVolume, Volume)
END_SLOT_MAP()

// ---
// constructors
// ---
Density::Density(LCreal value, const Mass* newMass, const Volume* newVolume) : Number()
{
    STANDARD_CONSTRUCTOR()

    if (newMass != 0 && newVolume != 0) { 
        myMass = newMass;
        myVolume = newVolume;
        val = value;
    }
    else {
        std::cerr << "Density::Density() - missing a mass or volume object,"
                  << " density is default to 1.0 kilograms per cubic meter"
                  << std::endl;

        // default mass, volume and density (1 Kilogram per Cubic Meter)
        // calling default constructor here doesn't seem to work
        myMass = new KiloGrams(1);
        myVolume = new CubicMeters(1);
        val = 1;
    }
}

Density::Density() : Number()
{
    STANDARD_CONSTRUCTOR()
    
    // default mass, volume and density (1 Kilogram per Cubic Meter)
    myMass = new KiloGrams(1);
    myVolume = new CubicMeters(1);
    val = 1;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Density::copyData(const Density& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc)
    {
        // initialize pointers when called by copy constructor
        myMass = 0;
        myVolume = 0;
    }

    val = org.getReal();
    myMass = org.getMass();
    myVolume = org.getVolume();
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Density::deleteData()
{
    myMass = 0;
    myVolume = 0;
}

//------------------------------------------------------------------------------
// convert() -- converts from one mass/volume ratio to another
//------------------------------------------------------------------------------
LCreal Density::convert(const Density& n) const
{
    return myMass->convert(*n.getMass()) / myVolume->convert(*n.getVolume());
}
    
//------------------------------------------------------------------------------
// set() -- sets our density from some other density 
//------------------------------------------------------------------------------
void Density::set(const Density& n)
{
    val = convert(n);
}

//------------------------------------------------------------------------------
// getMassUnits() and getVolumeUnits() -- get units of this density object
//------------------------------------------------------------------------------
const Mass* Density::getMass() const
{
    return myMass;
}

const Volume* Density::getVolume() const
{
    return myVolume;
}

//------------------------------------------------------------------------------
// setSlotMass() -- sets our mass object.
//------------------------------------------------------------------------------
bool Density::setSlotMass(const Mass* newMass)
{
    if (newMass != 0) {
        myMass = newMass;
        return true;
    }
    else return false;
}

//------------------------------------------------------------------------------
// setSlotVolume() -- sets our volume object.
//------------------------------------------------------------------------------
bool Density::setSlotVolume(const Volume* newVolume)
{
    if (newVolume != 0) {
        myVolume = newVolume;
        return true;
    }
    else return false;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Density
//------------------------------------------------------------------------------
Object* Density::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Density::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if (!slotsOnly) {
        sout << "( " << getFormName() << std::endl;
        // tab here
        j = 4;
    }
    
    indent(sout, i+j);
    sout << "value: " << val << std::endl;
    
    if (myMass != 0) {
        indent(sout, i+j);
        sout << "mass: " << *myMass << std::endl;
    }
    if (myVolume != 0) {
        indent(sout, i+j);
        sout << "volume: " << *myVolume << std::endl;
    }
    
    if (!slotsOnly) {
        indent(sout,i);
        sout << ")" << std::endl;
    }
            
    return sout;
}

} // End Basic namespace
} // End Eaagles namespace

