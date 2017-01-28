//----------------------------------------------------------------------------
// Classes: Power, KiloWatts, Watts, Horsepower
//
// Base class:  Object -> Number -> Power
//              Object -> Number -> Power -> KiloWatts
//              Object -> Number -> Power -> Watts
//              Object -> Number -> Power -> MilliWatts
//              Object -> Number -> Power -> Horsepower
//              Object -> Number -> Power -> DecibelWatts
//              Object -> Number -> Power -> DecibelMilliWatts
//----------------------------------------------------------------------------
#ifndef __oe_base_Powers_H__
#define __oe_base_Powers_H__

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/util/power_utils.hpp"

#include <cmath>
#include <iostream>

namespace oe {
namespace base {

//----------------------------------------------------------------------------
// Class: Power
// Description:  Base class for powers.  Defined as a Watt which is
//               equivalent to an instance of Watts with its value equal
//               to 1.0.
//
//               Base unit for Power derivations are Watts.
//
// Public methods (Defined in Power, and inherited by all derived classes):
//
//     set(const double v)
//        Sets a Power derived instance with an double.
//
//     set(const Power& n)
//        Sets, and converts if necessary, a Power derived instance with
//        another Power derived instance.
//
//     double convert(const Power& n)
//        Converts the value of a Power derived instance into
//        the units of another Power derived instance.
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Power& n)
//        Sends "( <the Power derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  KiloWatts, Watts, Horsepower, DecibelWatts,
//                 DecibelMilliWatts):
//
//     double convertStatic(const Power& n)
//        Static function to convert the given Power derived
//        instance into the units of another Power derived class.
//
//----------------------------------------------------------------------------
class Power : public Number
{
    DECLARE_SUBCLASS(Power, Number)

public:
    Power();
    Power(const double value);

    void set(const double v) { val = v; }
    void set(const Power& n) { val = fromPower(n.toPower()); }

    virtual double toPower() const = 0;
    virtual double fromPower(const double a) const = 0;
    double convert(const Power& n) { return fromPower(n.toPower()); }
};

inline std::ostream& operator<<(std::ostream& sout, const Power& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }


//----------------------------------------------------------------------------
// Class: Watts
// Description: An instance of KiloWatts with its value equal to 1.0 is one base
// unit for power.
//----------------------------------------------------------------------------
class Watts : public Power
{
    DECLARE_SUBCLASS(Watts, Power)

public:
    Watts();
    Watts(const double value);
    Watts(const Power& org);

    static double convertStatic(const Power &n)              { return n.toPower(); }
    virtual double toPower() const override                  { return static_cast<double>(val); }
    virtual double fromPower(const double a) const override  { return a; }
};

//----------------------------------------------------------------------------
// Class: MilliWatts
// Description: Watts * 0.01
//----------------------------------------------------------------------------
class MilliWatts : public Power
{
    DECLARE_SUBCLASS(MilliWatts, Power)

public:
    MilliWatts();
    MilliWatts(const double value);
    MilliWatts(const Power& org);

    static double convertStatic(const Power &n)              { return n.toPower() * power::W2MW; }
    virtual double toPower() const override                  { return static_cast<double>(val * power::MW2W); }
    virtual double fromPower(const double a) const override  { return a * power::W2MW; }
};

//----------------------------------------------------------------------------
// Class: KiloWatts
// Description: Watts * 0.001
//----------------------------------------------------------------------------
class KiloWatts : public Power
{
   DECLARE_SUBCLASS(KiloWatts, Power)

public:
    KiloWatts();
    KiloWatts(const double value);
    KiloWatts(const Power& org);

    static double convertStatic(const Power &n)              { return n.toPower() * power::W2KW; }
    virtual double toPower() const override                  { return static_cast<double>(val * power::KW2W); }
    virtual double fromPower(const double a) const override  { return a * power::W2KW; }
};

//----------------------------------------------------------------------------
// Class:  Horsepower
// Description: Watts * 1341
//----------------------------------------------------------------------------
class Horsepower: public Power
{
    DECLARE_SUBCLASS(Horsepower, Power)

public:
    Horsepower();
    Horsepower(const double value);
    Horsepower(const Power& org);

    static double convertStatic(const Power &n)              { return n.toPower() * power::W2HP; }
    virtual double toPower() const override                  { return static_cast<double>(val * power::HP2W); }
    virtual double fromPower(const double a) const override  { return a * power::W2HP; }
};

//----------------------------------------------------------------------------
// Class: DecibelWatts
// Description: 10 Log(Watts)
//----------------------------------------------------------------------------
class DecibelWatts: public Power
{
    DECLARE_SUBCLASS(DecibelWatts, Power)

public:
    DecibelWatts();
    DecibelWatts(const double value);
    DecibelWatts(const Power& org);

    static double convertStatic(const Power &n)              { return static_cast<double>(10.0 * std::log10(n.toPower())); }
    virtual double toPower() const override                  { return  std::pow(static_cast<double>(10.0), static_cast<double>(val/10.0)); }
    virtual double fromPower(const double a) const override  { return 10.0f * std::log10(a) ; }

};

//----------------------------------------------------------------------------
// Class: DecibelMilliWatts
// Description: 10 Log(Watts * 1000)
//----------------------------------------------------------------------------
class DecibelMilliWatts: public Power
{
    DECLARE_SUBCLASS(DecibelMilliWatts, Power)

public:
    DecibelMilliWatts();
    DecibelMilliWatts(const double value);
    DecibelMilliWatts(const Power& org);

    static double convertStatic(const Power &n)              { return static_cast<double>(10.0 * std::log10(n.toPower() * power::W2MW)); }
    virtual double toPower() const override                  { return  power::MW2W * std::pow(static_cast<double>(10.0), static_cast<double>(val/10.0)); }
    virtual double fromPower(const double a) const override  { return (10.0f * std::log10(a * power::W2MW)); }
};

}
}

#endif
