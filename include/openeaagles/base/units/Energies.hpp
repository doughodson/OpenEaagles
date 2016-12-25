//------------------------------------------------------------------------------
// Classes:  Energy, KiloWattHours, BTUs, Calories, FootPounds, Joules
//
// Base class:  Object -> Number -> Energy
//              Object -> Number -> Energy -> KiloWattHours
//              Object -> Number -> Energy -> BTUs
//              Object -> Number -> Energy -> Calories
//              Object -> Number -> Energy -> FootPounds
//              Object -> Number -> Energy -> Joules

//
// Description:  Numbers as energy -- KiloWatt-hours, BTUs, Calories, Foot Pounds, and Joules
//               Base unit for Energy derivations are Joules.
//
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
//     Conversion routines:
//        static double btusToFootPounds(const double v) { return v * 777.9f; }
//        static double btusToJoules(const double v) { return v * 1055; }
//        static double btusToKiloWattHours(const double v) { return v * 0.0002930f; }
//        static double btusToCalories(const double v) { return v * 252; }
//        static double footPoundsToBTUs(const double v) { return v * 0.001285f; }
//        static double footPoundsToJoules(const double v)  { return v * 1.356f; }
//        static double footPoundsToKiloWattHours(const double v) { return v * 0.0000006776f; }
//        static double footPoundsToCalories(const double v) { return v * 0.3239f; }
//        static double joulesToBTUs(const double v) { return v * 0.0009481f; }
//        static double joulesToFootPounds(const double v) { return v * 0.7376f; }
//        static double joulesToKiloWattHours(const double v) { return v * 0.0000002778f; }
//        static double joulesToCalories(const double v) { return v * 0.2389f; }
//        static double kiloWattHoursToBTUs(const double v) { return v * 3413; }
//        static double kiloWattHoursToFootPounds(const double v) { return v * 2655000; }
//        static double kiloWattHoursToJoules(const double v) { return v * 3600000; }
//        static double kiloWattHoursToCalories(const double v) { return v * 860100; }
//        static double caloriesToBTUs(const double v) { return v * 0.003986f; }
//        static double caloriesToFootPounds(const double v) { return v * 3.087f; }
//        static double caloriesToJoules(const double v) { return v * 4.186f; }
//        static double caloriesToKiloWattHours(const double v) { return v * 0.000001163f; }
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
//
//------------------------------------------------------------------------------

#ifndef __oe_Energies_H__
#define __oe_Energies_H__

#include "openeaagles/base/Number.hpp"
#include <iostream>

namespace oe {
namespace base {

// ----------------------------------------------------------------------------
// Define Energy Conversion Constants:
//
// These constants were obtained from the Department of Energy, and are assumed
// accurate as of 2/5/03.
//
// http://www.eia.doe.gov/kids/units.xls - Department of Energy
// Source cited for website: Monthly Energy Review, Energy Information
// Administration, Appendix A "Thermal conversion Factors" Sept. 2002, p. 161
// ----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class:  Energy
// Base class:  Object -> Number -> Energy
// Description:  Base class for energy.  Defined as a Joule which is
//               equivalent to an instance of Joules with its value equal
//               to 1.0.
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

    // Conversions between Energy
    static double btusToFootPounds(const double v)          { return (v * BTU2J) * J2FP; }
    static double btusToJoules(const double v)              { return v * BTU2J; }
    static double btusToKiloWattHours(const double v)       { return (v * BTU2J) * J2KWH; }
    static double btusToCalories(const double v)            { return (v * BTU2J) * J2C; }
    static double footPoundsToBTUs(const double v)          { return (v * FP2J) * J2BTU; }
    static double footPoundsToJoules(const double v)        { return v * FP2J; }
    static double footPoundsToKiloWattHours(const double v) { return (v * FP2J) * J2KWH; }
    static double footPoundsToCalories(const double v)      { return (v * FP2J) * J2C; }
    static double joulesToBTUs(const double v)              { return v * J2BTU; }
    static double joulesToFootPounds(const double v)        { return v * J2FP; }
    static double joulesToKiloWattHours(const double v)     { return v * J2KWH; }
    static double joulesToCalories(const double v)          { return v * J2C; }
    static double kiloWattHoursToBTUs(const double v)       { return (v * KWH2J) * J2BTU; }
    static double kiloWattHoursToFootPounds(const double v) { return (v * KWH2J) * J2FP; }
    static double kiloWattHoursToJoules(const double v)     { return v * KWH2J; }
    static double kiloWattHoursToCalories(const double v)   { return (v * KWH2J) * J2C; }
    static double caloriesToBTUs(const double v)            { return (v * C2J) * J2BTU; }
    static double caloriesToFootPounds(const double v)      { return (v * C2J) * J2FP; }
    static double caloriesToJoules(const double v)          { return v * C2J; }
    static double caloriesToKiloWattHours(const double v)   { return (v * C2J) * J2KWH; }

   // Conversion constants
   static const double J2KWH;
   static const double KWH2J;
   static const double J2BTU;
   static const double BTU2J;
   static const double J2FP;
   static const double FP2J;
   static const double J2C;
   static const double C2J;
};

inline std::ostream& operator<<(std::ostream& sout, const Energy& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class:  Joules
// Base class:  Object -> Number -> Energy -> Joules
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
// Base class:  Object -> Number -> Energy -> KiloWattHours
// Description: Joules * 0.000000277778
//------------------------------------------------------------------------------
class KiloWattHours : public Energy
{
    DECLARE_SUBCLASS(KiloWattHours, Energy)

public:
    KiloWattHours();
    KiloWattHours(const double value);
    KiloWattHours(const Energy& value);

    static double convertStatic(const Energy& n)             { return n.toEnergy() * J2KWH; }
    virtual double toEnergy() const override                 { return static_cast<double>(val * KWH2J); }
    virtual double fromEnergy(const double a) const override { return a * J2KWH; }
};


//------------------------------------------------------------------------------
// Class:  BTUs
// Base class:  Object -> Number -> Energy -> BTUs
// Description:  Joules * 9.478 x 10 -04
//------------------------------------------------------------------------------
class BTUs : public Energy
{
    DECLARE_SUBCLASS(BTUs, Energy)

public:
    BTUs();
    BTUs(const double value);
    BTUs(const Energy& value);

    static double convertStatic(const Energy& n)             { return n.toEnergy() * J2BTU; }
    virtual double toEnergy() const override                 { return static_cast<double>(val * BTU2J); }
    virtual double fromEnergy(const double a) const override { return a * J2BTU; }
};


//------------------------------------------------------------------------------
// Class:  Calories
// Base class:  Object -> Number -> Energy -> Calories
// Description:  Joules * 0.2388888888888888889
//------------------------------------------------------------------------------
class Calories : public Energy
{
    DECLARE_SUBCLASS(Calories, Energy)

public:
    Calories();
    Calories(const double value);
    Calories(const Energy& value);

    static double convertStatic(const Energy& n)               { return n.toEnergy() * J2C; }
    virtual double toEnergy() const override                   { return static_cast<double>(val * C2J); }
    virtual double fromEnergy(const double a) const override   { return a * J2C; }
};


//------------------------------------------------------------------------------
// Class:  FootPounds
// Base class:  Object -> Number -> Energy -> FootPounds
// Description:  Joules * 0.7376
//------------------------------------------------------------------------------
class FootPounds : public Energy
{
    DECLARE_SUBCLASS(FootPounds, Energy)

public:
    FootPounds();
    FootPounds(const double value);
    FootPounds(const Energy& value);

    static double convertStatic(const Energy& n)              { return n.toEnergy() * J2FP; }
    virtual double toEnergy() const override                  { return static_cast<double>(val * FP2J); }
    virtual double fromEnergy(const double a) const override  { return a * J2FP; }
};

}
}

#endif
