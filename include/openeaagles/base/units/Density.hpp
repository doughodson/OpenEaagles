//------------------------------------------------------------------------------
// Class: Density
//------------------------------------------------------------------------------
#ifndef __oe_base_Density_H__
#define __oe_base_Density_H__

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/safe_ptr.hpp"
#include "openeaagles/base/units/Masses.hpp"
#include "openeaagles/base/units/Volumes.hpp"

namespace oe {
namespace base {
//------------------------------------------------------------------------------
// Classes: Density
//
// Description:  Density as a function of mass and volume.

// Factory name: Density
// Slots:
//    mass    <Mass>    ! Mass (default: 1 kg)
//    volume  <Volume>  ! Volume (default: 1 m^3)
//
// getMassUnits() -- get units of this density object
//     const Mass* Density::getMass() const

// getVolumeUnits() -- get units of this density object
//     const Volume* Density::getVolume() const

// setSlotMass() -- sets our mass object.
//     bool Density::setSlotMass(const Mass* newMass)

// setSlotVolume() -- sets our volume object.
//     bool Density::setSlotVolume(const Volume* newVolume)

// getSlotByIndex() for Density
//     Object* Density::getSlotByIndex(const int si)

// set() -- sets our density from some other density
//     void Density::set(const Density& n)

// convert() -- converts from one mass/volume ratio to another
//     double Density::convert(const Density& n) const

// serialize() -- print the value of this object to the output stream sout.
//     std::ostream& Density::serialize(std::ostream& sout, const int i, const bool slotsOnly) const

// NOTE: The = operator and copy constructor WILL change the units
// of the target density object.  To perform a conversion, use set().
//------------------------------------------------------------------------------
class Density : public Number
{
    DECLARE_SUBCLASS(Density, Number)

public:
    Density(double newDensity, const Mass* newMass, const Volume* newVolume);
    Density();

    double convert(const Density& n) const;
    void set(const Density& n);
    const Mass* getMass() const;
    const Volume* getVolume() const;

    // slot table functions
    bool setSlotMass(const Mass* newMass);
    bool setSlotVolume(const Volume* newVolume);

    private:
    safe_ptr<const Mass>     myMass;         // holds our mass type
    safe_ptr<const Volume>   myVolume;       // holds our volume type
};

inline std::ostream& operator<<(std::ostream& sout, const Density& n)
{
    const Mass* m = n.getMass();
    const Volume* v = n.getVolume();

    sout << "( " << n.getFactoryName() << " " << n.getReal();
    if ( m != 0 ) sout << " " << *m;
    if ( v != 0 ) sout << " " << *v;
    sout << " )";
    return sout;
}

}
}

#endif
