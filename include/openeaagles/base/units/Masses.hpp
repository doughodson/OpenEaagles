//------------------------------------------------------------------------------
// Classes:  Mass, Grams, KiloGrams, Slugs
//
// Base class:  Object -> Number -> Mass
//              Object -> Number -> Mass -> Grams
//              Object -> Number -> Mass -> KiloGrams
//              Object -> Number -> Mass -> Slugs
//------------------------------------------------------------------------------
#ifndef __oe_base_Masses_H__
#define __oe_base_Masses_H__

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/units/util/mass.hpp"

#include <iostream>

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: Mass
// Base class:  Object -> Number -> Mass
// Description:  Base class for Masses.  Defined as a KiloGram which is
//               equivalent to an instance of KiloGrams with its value equal
//               to 1.0.
//
// Public methods (Defined in Mass, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Mass derived instance with an double.
//
//     set(const Mass& n)
//        Sets, and converts if necessary, an Mass derived instance with
//        another Mass derived instance.
//
//     double convert(const Mass& n)
//        Converts the value of an Mass derived instance into
//        the units of another Mass derived instance.
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Mass& n)
//        Sends "( <the Mass derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  Grams, KiloGrams, Slugs):
//
//     double convertStatic(const Mass& n)
//        Static function to convert the given Mass derived instance
//        into the units of a specific Mass derived class.
//
//------------------------------------------------------------------------------
class Mass : public Number
{
    DECLARE_SUBCLASS(Mass, Number)

public:
    Mass();
    Mass(const double value);

    void set(const double v) { val = v; }
    void set(const Mass& n)  { val = fromMass(n.toMass()); }

    //this goes to another mass (kilograms)
    virtual double toMass() const = 0;
    //this is coming from another mass (kilograms)
    virtual double fromMass(const double a) const = 0;
    double convert(const Mass& n) const { return fromMass(n.toMass()); }
};

inline std::ostream& operator<<(std::ostream& sout, const Mass& n)
   { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class:  KiloGrams
// Base class:  Object -> Number -> Mass -> KiloGrams
// Description:  An instance of KiloGrams with its value equal to 1.0 is one
//               base unit for mass.
//------------------------------------------------------------------------------
class KiloGrams : public Mass
{
    DECLARE_SUBCLASS(KiloGrams, Mass)

public:
    KiloGrams();
    KiloGrams(const double value);
    KiloGrams(const Mass& value);

    static double convertStatic(const Mass &n)               { return n.toMass(); }
    virtual double toMass() const override                   { return static_cast<double>(val); }
    virtual double fromMass(const double a) const override   { return a; }
};


//------------------------------------------------------------------------------
// Class:  Grams
// Base class:  Object -> Number -> Mass -> Grams
// Description:  KiloGrams * 1000
//------------------------------------------------------------------------------
class Grams : public Mass
{
    DECLARE_SUBCLASS(Grams, Mass)

public:
    Grams();
    Grams(const double value);
    Grams(const Mass& value);

    static double convertStatic(const Mass &n)               { return n.toMass() * mass::KG2G; }
    virtual double toMass() const override                   { return static_cast<double>(val * mass::G2KG); }
    virtual double fromMass(const double a) const override   { return a * mass::KG2G; }
};


//------------------------------------------------------------------------------
// Class:  Slugs
// Base class:  Object -> Number -> Mass -> Slugs
// Description:  KiloGram * 0.06852176585
//------------------------------------------------------------------------------
class Slugs : public Mass
{
    DECLARE_SUBCLASS(Slugs, Mass)

public:
    Slugs();
    Slugs(const double value);
    Slugs(const Mass& value);

    static double convertStatic(const Mass &n)               { return n.toMass() * mass::KG2SL; }
    virtual double toMass() const override                   { return static_cast<double>(val * mass::SL2KG); }
    virtual double fromMass(const double a) const override   { return a * mass::KG2SL; }
};

}
}

#endif
