//------------------------------------------------------------------------------
// Classes:  Energy, KiloWattHours, BTUs, Calories, FootPounds, Joules
//
// Base class:  Object -> Number -> Energy
//              Object -> Number -> Energy -> KiloWattHours
//              Object -> Number -> Energy -> BTUs
//              Object -> Number -> Energy -> Calories
//              Object -> Number -> Energy -> FootPounds
//              Object -> Number -> Energy -> Joules
//------------------------------------------------------------------------------

#ifndef __oe_Energies_H__
#define __oe_Energies_H__

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/units/util/energy.hpp"
#include <iostream>

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class:  Energy
// Description:  Base class for energy.  Defined as a Joule which is
//               equivalent to an instance of Joules with its value equal
//               to 1.0.
//
// Public methods (Defined in Energy, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Energy derived instance with an double.
//
//     set(const Energy& n)
//        Sets, and converts if necessary, an Energy derived instance with
//        another Energy derived instance.
//
//     double convert(const Energy& n)
//        Converts the value of an Energy derived instance into
//        the units of another Energy derived instance.
//
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Energy& n)
//        Sends "( <the Energy derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  KiloWattHours, BTUs, Calories, Joules, FootPounds):
//
//     double convertStatic(const Energy& n)
//        Static function to convert the given Energy derived
//        instance into the units of a specific Energy derived
//        class.
//------------------------------------------------------------------------------
class Energy : public Number
{
    DECLARE_SUBCLASS(Energy, Number)

public:
    Energy();
    Energy(const double value);

    void set(const double v)  { val = v; }
    void set(const Energy& n) { val = fromEnergy(n.toEnergy()); }

    virtual double toEnergy() const = 0;
    virtual double fromEnergy(const double a) const = 0;
    double convert(const Energy& n){ return fromEnergy(n.toEnergy()); }
};

inline std::ostream& operator<<(std::ostream& sout, const Energy& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class:  Joules
// Description: An instance of Joules with its value equal to 1.0 is one
//              base unit for energy.
//------------------------------------------------------------------------------
class Joules : public Energy
{
    DECLARE_SUBCLASS(Joules, Energy)

public:
    Joules();
    Joules(const double value);
    Joules(const Energy& value);

    static double convertStatic(const Energy& n)               { return n.toEnergy(); }
    virtual double toEnergy() const override                   { return static_cast<double>(val); }
    virtual double fromEnergy(const double a) const override   { return a; }
};

//------------------------------------------------------------------------------
// Class:  KiloWattHours
// Description: Joules * 0.000000277778
//------------------------------------------------------------------------------
class KiloWattHours : public Energy
{
    DECLARE_SUBCLASS(KiloWattHours, Energy)

public:
    KiloWattHours();
    KiloWattHours(const double value);
    KiloWattHours(const Energy& value);

    static double convertStatic(const Energy& n)             { return n.toEnergy() * energy::J2KWH; }
    virtual double toEnergy() const override                 { return static_cast<double>(val * energy::KWH2J); }
    virtual double fromEnergy(const double a) const override { return a * energy::J2KWH; }
};


//------------------------------------------------------------------------------
// Class:  BTUs
// Description:  Joules * 9.478 x 10 -04
//------------------------------------------------------------------------------
class BTUs : public Energy
{
    DECLARE_SUBCLASS(BTUs, Energy)

public:
    BTUs();
    BTUs(const double value);
    BTUs(const Energy& value);

    static double convertStatic(const Energy& n)             { return n.toEnergy() * energy::J2BTU; }
    virtual double toEnergy() const override                 { return static_cast<double>(val * energy::BTU2J); }
    virtual double fromEnergy(const double a) const override { return a * energy::J2BTU; }
};


//------------------------------------------------------------------------------
// Class:  Calories
// Description:  Joules * 0.2388888888888888889
//------------------------------------------------------------------------------
class Calories : public Energy
{
    DECLARE_SUBCLASS(Calories, Energy)

public:
    Calories();
    Calories(const double value);
    Calories(const Energy& value);

    static double convertStatic(const Energy& n)               { return n.toEnergy() * energy::J2C; }
    virtual double toEnergy() const override                   { return static_cast<double>(val * energy::C2J); }
    virtual double fromEnergy(const double a) const override   { return a * energy::J2C; }
};


//------------------------------------------------------------------------------
// Class:  FootPounds
// Description:  Joules * 0.7376
//------------------------------------------------------------------------------
class FootPounds : public Energy
{
    DECLARE_SUBCLASS(FootPounds, Energy)

public:
    FootPounds();
    FootPounds(const double value);
    FootPounds(const Energy& value);

    static double convertStatic(const Energy& n)              { return n.toEnergy() * energy::J2FP; }
    virtual double toEnergy() const override                  { return static_cast<double>(val * energy::FP2J); }
    virtual double fromEnergy(const double a) const override  { return a * energy::J2FP; }
};

}
}

#endif
