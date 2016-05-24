//----------------------------------------------------------------------------
// Classes:  Power, KiloWatts, Watts, Horsepower
//
// Base class:  Object -> Number -> Power
//              Object -> Number -> Power -> KiloWatts
//              Object -> Number -> Power -> Watts
//              Object -> Number -> Power -> MilliWatts
//              Object -> Number -> Power -> Horsepower
//              Object -> Number -> Power -> DecibelWatts
//              Object -> Number -> Power -> DecibelMilliWatts
//
// Description:  Numbers as powers -- KiloWatts, Watts, Horsepower, DecibelWatts,
//               DecibelMilliWatts, MilliWatt
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
//     Conversion routines:
//        static double kiloWattsToWatts(const double v)       { return v * KW2W; }
//        static double kiloWattsToHorsepower(const double v)  { return (v * KW2W) * W2HP; }
//        static double kiloWattsToMilliWatts( const double v) { return (v * KW2W) * W2MW; }
//        static double wattsToKiloWatts(const double v)       { return v * W2KW; }
//        static double wattsToHorsepower(const double v)      { return v * W2HP; }
//        static double wattsToMilliWatts(const double v)      { return v * W2MW; }
//        static double horsepowerToWatts(const double v)      { return v * HP2W; }
//        static double horsepowerToKiloWatts(const double v)  { return (v * HP2W) * W2KW; }
//        static double horsepowerToMilliWatts(const double v) { return (v * HP2W) * W2MW; }
//        static double milliWattsToWatts(const double v)      { return v * MW2W }
//        static double milliWattsToKiloWatts(const double v)  { return (v * MW2W) * W2KW; }
//        static double milliWattsToHorsepower(const double v) { return (v * MW2W) * W2HP }
//
//     No decibelWatts or decibelMilliWatts conversions at this time.  We can add them later
//     if needed.
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
#ifndef __oe_base_Powers_H__
#define __oe_base_Powers_H__

#include "openeaagles/base/Number.h"

#include <cmath>

namespace oe {
namespace base {

// ---------------------------------------------------------------------------
// Defined Power Conversion Constants:
//
// These constants were obtained (and cross referenced) from the following
// websites, and are assumed accurate as of 2/5/03.
//
// http://www-cta.ornl.gov/cta/data/tedb22/Edition22_AppendixB.pdf
// - U.S. Center for Transportation Analysis
// http://www.energy.rochester.edu/units/conversions.pdf
// - Henry Manczyk, C.P.E., C.E.M.
// http://www.uwsp.edu/cnr/wcee/keep/Mod1/Whatis/energyresourcetables.htm
// - U. of Wisconsin
// http://lectureonline.cl.msu.edu/~mmp/kap5/cd129.htm
// - Michigan State University
// ---------------------------------------------------------------------------
// Since all units are converted to or from Watts, only those constants
// will be defined.
////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// Class:  Power
// Base class:  Object -> Number -> Power
// Description:  Base class for powers.  Defined as a Watt which is
//               equivalent to an instance of Watts with its value equal
//               to 1.0.
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

    // Conversions between Powers
    static double kiloWattsToWatts(const double v)       { return v * KW2W; }
    static double kiloWattsToHorsepower(const double v)  { return (v * KW2W) * W2HP; }
    static double kiloWattsToMilliWatts( const double v) { return (v * KW2W) * W2MW; }
    static double wattsToKiloWatts(const double v)       { return v * W2KW; }
    static double wattsToHorsepower(const double v)      { return v * W2HP; }
    static double wattsToMilliWatts(const double v)      { return v * W2MW; }
    static double horsepowerToWatts(const double v)      { return v * HP2W; }
    static double horsepowerToKiloWatts(const double v)  { return (v * HP2W) * W2KW; }
    static double horsepowerToMilliWatts(const double v) { return (v * HP2W) * W2MW; }
    static double milliWattsToWatts(const double v)      { return v * MW2W; }
    static double milliWattsToKiloWatts(const double v)  { return (v * MW2W) * W2KW; }
    static double milliWattsToHorsepower(const double v) { return (v * MW2W) * W2HP; }

    // Conversion constants
    static const double W2KW;  // Watts -> KiloWatts
    static const double KW2W;  // KiloWatts -> Watts
    static const double W2HP;  // Watts -> Horsepower
    static const double HP2W;  // Horsepower -> Watts
    static const double W2MW;  // Watts -> MilliWatts
    static const double MW2W;  // MilliWatts -> Watts
};


inline std::ostream& operator<<(std::ostream& sout, const Power& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }


//----------------------------------------------------------------------------
// Class:  Watts
// Base class:  Object -> Number -> Power -> Watts
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
// Class:  MilliWatts
// Base class:  Object -> Number -> Power -> MilliWatts
// Description: Watts * 0.01
//----------------------------------------------------------------------------
class MilliWatts : public Power
{
    DECLARE_SUBCLASS(MilliWatts, Power)

public:
    MilliWatts();
    MilliWatts(const double value);
    MilliWatts(const Power& org);

    static double convertStatic(const Power &n)              { return n.toPower() * W2MW; }
    virtual double toPower() const override                  { return static_cast<double>(val * MW2W); }
    virtual double fromPower(const double a) const override  { return a * W2MW; }
};

//----------------------------------------------------------------------------
// Class:  KiloWatts
// Base class:  Object -> Number -> Power -> KiloWatts
// Description: Watts * 0.001
//----------------------------------------------------------------------------
class KiloWatts : public Power
{
   DECLARE_SUBCLASS(KiloWatts, Power)

public:
    KiloWatts();
    KiloWatts(const double value);
    KiloWatts(const Power& org);

    static double convertStatic(const Power &n)              { return n.toPower() * W2KW; }
    virtual double toPower() const override                  { return static_cast<double>(val * KW2W); }
    virtual double fromPower(const double a) const override  { return a * W2KW; }
};

//----------------------------------------------------------------------------
// Class:  Horsepower
// Base class:  Object -> Number -> Power -> Horsepower
// Description: Watts * 1341
//----------------------------------------------------------------------------
class Horsepower: public Power
{
    DECLARE_SUBCLASS(Horsepower, Power)

public:
    Horsepower();
    Horsepower(const double value);
    Horsepower(const Power& org);

    static double convertStatic(const Power &n)              { return n.toPower() * W2HP; }
    virtual double toPower() const override                  { return static_cast<double>(val * HP2W); }
    virtual double fromPower(const double a) const override  { return a * W2HP; }
};

//----------------------------------------------------------------------------
// Class:  DecibelWatts
// Base class:  Object -> Number -> Power -> DecibelWatts
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
// Class:  DecibelMilliWatts
// Base class:  Object -> Number -> Power -> DecibelMilliWatts
// Description: 10 Log(Watts * 1000)
//----------------------------------------------------------------------------
class DecibelMilliWatts: public Power
{
    DECLARE_SUBCLASS(DecibelMilliWatts, Power)

public:
    DecibelMilliWatts();
    DecibelMilliWatts(const double value);
    DecibelMilliWatts(const Power& org);

    static double convertStatic(const Power &n)              { return static_cast<double>(10.0 * std::log10(n.toPower() * W2MW)); }
    virtual double toPower() const override                  { return  MW2W * std::pow(static_cast<double>(10.0), static_cast<double>(val/10.0)); }
    virtual double fromPower(const double a) const override  { return (10.0f * std::log10(a * W2MW)); }

};

}
}

#endif
