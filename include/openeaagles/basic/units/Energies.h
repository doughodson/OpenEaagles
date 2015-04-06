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
//     set(const LCreal v)
//        Sets an Energy derived instance with an LCreal.
//
//     set(const Energy& n)
//        Sets, and converts if necessary, an Energy derived instance with
//        another Energy derived instance.
//
//     LCreal convert(const Energy& n)
//        Converts the value of an Energy derived instance into
//        the units of another Energy derived instance.
//
//
//     Conversion routines:
//        static LCreal btusToFootPounds(const LCreal v) { return v * 777.9f; }
//        static LCreal btusToJoules(const LCreal v) { return v * 1055; }
//        static LCreal btusToKiloWattHours(const LCreal v) { return v * 0.0002930f; }
//        static LCreal btusToCalories(const LCreal v) { return v * 252; }
//        static LCreal footPoundsToBTUs(const LCreal v) { return v * 0.001285f; }
//        static LCreal footPoundsToJoules(const LCreal v)  { return v * 1.356f; }
//        static LCreal footPoundsToKiloWattHours(const LCreal v) { return v * 0.0000006776f; }
//        static LCreal footPoundsToCalories(const LCreal v) { return v * 0.3239f; }
//        static LCreal joulesToBTUs(const LCreal v) { return v * 0.0009481f; }
//        static LCreal joulesToFootPounds(const LCreal v) { return v * 0.7376f; }
//        static LCreal joulesToKiloWattHours(const LCreal v) { return v * 0.0000002778f; }
//        static LCreal joulesToCalories(const LCreal v) { return v * 0.2389f; }
//        static LCreal kiloWattHoursToBTUs(const LCreal v) { return v * 3413; }
//        static LCreal kiloWattHoursToFootPounds(const LCreal v) { return v * 2655000; }
//        static LCreal kiloWattHoursToJoules(const LCreal v) { return v * 3600000; }
//        static LCreal kiloWattHoursToCalories(const LCreal v) { return v * 860100; }
//        static LCreal caloriesToBTUs(const LCreal v) { return v * 0.003986f; }
//        static LCreal caloriesToFootPounds(const LCreal v) { return v * 3.087f; }
//        static LCreal caloriesToJoules(const LCreal v) { return v * 4.186f; }
//        static LCreal caloriesToKiloWattHours(const LCreal v) { return v * 0.000001163f; }
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Energy& n)
//        Sends "( <the Energy derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  KiloWattHours, BTUs, Calories, Joules, FootPounds):
//
//     LCreal convertStatic(const Energy& n)
//        Static function to convert the given Energy derived
//        instance into the units of a specific Energy derived
//        class.
//
//------------------------------------------------------------------------------

#ifndef __Eaagles_Energies_H__
#define __Eaagles_Energies_H__

#include "../Number.h"

namespace Eaagles {
namespace Basic {

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
    Energy(const LCreal value);

    void set(const LCreal v)  { val = v; }
    void set(const Energy& n) { val = fromEnergy(n.toEnergy()); }

    virtual LCreal toEnergy() const = 0;
    virtual LCreal fromEnergy(const LCreal a) const = 0;
    LCreal convert(const Energy& n){ return fromEnergy(n.toEnergy()); }

    // Conversions between Energy
    static LCreal btusToFootPounds(const LCreal v)          { return (v * BTU2J) * J2FP; }
    static LCreal btusToJoules(const LCreal v)              { return v * BTU2J; }
    static LCreal btusToKiloWattHours(const LCreal v)       { return (v * BTU2J) * J2KWH; }
    static LCreal btusToCalories(const LCreal v)            { return (v * BTU2J) * J2C; }
    static LCreal footPoundsToBTUs(const LCreal v)          { return (v * FP2J) * J2BTU; }
    static LCreal footPoundsToJoules(const LCreal v)        { return v * FP2J; }
    static LCreal footPoundsToKiloWattHours(const LCreal v) { return (v * FP2J) * J2KWH; }
    static LCreal footPoundsToCalories(const LCreal v)      { return (v * FP2J) * J2C; }
    static LCreal joulesToBTUs(const LCreal v)              { return v * J2BTU; }
    static LCreal joulesToFootPounds(const LCreal v)        { return v * J2FP; }
    static LCreal joulesToKiloWattHours(const LCreal v)     { return v * J2KWH; }
    static LCreal joulesToCalories(const LCreal v)          { return v * J2C; }
    static LCreal kiloWattHoursToBTUs(const LCreal v)       { return (v * KWH2J) * J2BTU; }
    static LCreal kiloWattHoursToFootPounds(const LCreal v) { return (v * KWH2J) * J2FP; }
    static LCreal kiloWattHoursToJoules(const LCreal v)     { return v * KWH2J; }
    static LCreal kiloWattHoursToCalories(const LCreal v)   { return (v * KWH2J) * J2C; }
    static LCreal caloriesToBTUs(const LCreal v)            { return (v * C2J) * J2BTU; }
    static LCreal caloriesToFootPounds(const LCreal v)      { return (v * C2J) * J2FP; }
    static LCreal caloriesToJoules(const LCreal v)          { return v * C2J; }
    static LCreal caloriesToKiloWattHours(const LCreal v)   { return (v * C2J) * J2KWH; }

   // Conversion constants
   static const LCreal J2KWH;
   static const LCreal KWH2J;
   static const LCreal J2BTU;
   static const LCreal BTU2J;
   static const LCreal J2FP;
   static const LCreal FP2J;
   static const LCreal J2C;
   static const LCreal C2J;
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
    Joules(const LCreal value);
    Joules(const Energy& value);

    static LCreal convertStatic(const Energy& n)       { return n.toEnergy(); }
    LCreal toEnergy() const override                   { return static_cast<LCreal>(val); }
    LCreal fromEnergy(const LCreal a) const override   { return a; }
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
    KiloWattHours(const LCreal value);
    KiloWattHours(const Energy& value);

    static LCreal convertStatic(const Energy& n)     { return n.toEnergy() * J2KWH; }
    LCreal toEnergy() const override                 { return static_cast<LCreal>(val * KWH2J); }
    LCreal fromEnergy(const LCreal a) const override { return a * J2KWH; }
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
    BTUs(const LCreal value);
    BTUs(const Energy& value);

    static LCreal convertStatic(const Energy& n)     { return n.toEnergy() * J2BTU; }
    LCreal toEnergy() const override                 { return static_cast<LCreal>(val * BTU2J); }
    LCreal fromEnergy(const LCreal a) const override { return a * J2BTU; }
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
    Calories(const LCreal value);
    Calories(const Energy& value);

    static LCreal convertStatic(const Energy& n)       { return n.toEnergy() * J2C; }
    LCreal toEnergy() const override                   { return static_cast<LCreal>(val * C2J); }
    LCreal fromEnergy(const LCreal a) const override   { return a * J2C; }
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
    FootPounds(const LCreal value);
    FootPounds(const Energy& value);

    static LCreal convertStatic(const Energy& n)      { return n.toEnergy() * J2FP; }
    LCreal toEnergy() const override                  { return static_cast<LCreal>(val * FP2J); }
    LCreal fromEnergy(const LCreal a) const override  { return a * J2FP; }
};

} // End Basic namespace
} // End Eaagles namespace

#endif
