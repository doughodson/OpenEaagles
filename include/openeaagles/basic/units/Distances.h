//------------------------------------------------------------------------------
// Classes: Distance
// Base class:  Object -> Number -> Distance
//              Object -> Number -> Distance -> Meters
//              Object -> Number -> Distance -> CentiMeters
//              Object -> Number -> Distance -> KiloMeters
//              Object -> Number -> Distance -> Inches
//              Object -> Number -> Distance -> Feet
//              Object -> Number -> Distance -> NauticalMiles
//              Object -> Number -> Distance -> StatuteMiles
//
// Description:  Numbers as distances -- Meters, CentiMeters, KiloMeters,
//                                       Inches, Feet, NauticalMiles,
//                                       or StatuteMiles
//               Base unit for Distance derivations is a meter.
//
//
// Public methods (Defined in Distance, and inherited by all derived classes):
//
//     set(const LCreal v)
//        Sets a Distance derived instance with an LCreal.
//
//     set(const Distance& n)
//        Sets, and converts if necessary, a Distance derived instance with
//        another Distance derived instance.
//
//     LCreal convert(const Distance& n)
//        Converts the value of a Distance derived instance into
//        the units of another Distance derived instance.
//
//
// Conversion routines:
//       LCreal MetersToMicroMeters(const LCreal v)
//       LCreal MicroMetersToMeters(const LCreal v
//       LCreal MetersToCentiMeters(const LCreal v)
//       LCreal CentiMetersToMeters(const LCreal v)
//       LCreal MetersToKiloMeters(const LCreal v)
//       LCreal KiloMetersToMeters(const LCreal v)
//       LCreal MetersToInches(const LCreal v)
//       LCreal InchesToMeters(const LCreal v)
//       LCreal MetersToFeet(const LCreal v)
//       LCreal FeetToMeters(const LCreal v)
//       LCreal KiloMetersToNauticalMiles(const LCreal v)
//       LCreal NauticalMilesToKiloMeters(const LCreal v)
//       LCreal KiloMetersToStatuteMiles(const LCreal v)
//       LCreal StatuteMilesToKiloMeters(const LCreal v)
//       Return the appropriate conversion factors.
//
// Output stream operator:  >>
//       ostream& operator<<(ostream& sout, const Distance& n)
//          Sends "( <the Distance derived instance class name and value> )"
//          to the output stream.
//
//
// Public methods (For classes:  Meters, CentiMeters, KiloMeters,
//                               Inches, Feet, NauticalMiles,
//                               StatuteMiles):
//
// LCreal convertStatic(const Distance& n) const
//       Static function to convert the given Distance derived
//       instance into the units of a specific Distance derived
//       class.
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Distance_H__
#define __Eaagles_Basic_Distance_H__

#include "../Number.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:  Distance
// Base class:  Object -> Number -> Distance
// Description:  Base class for distances.  Defined as a meter which is
//               equivalent to an instance of Meters with its value equal
//               to 1.0.
//------------------------------------------------------------------------------
class Distance : public Number
{
    DECLARE_SUBCLASS(Distance, Number)

public:
    Distance();
    Distance(const LCreal value);

    void set(const LCreal v)      { val = v; }
    void set(const Distance& n) { val = fromDistance(n.toDistance()); }

    virtual LCreal toDistance() const = 0;
    virtual LCreal fromDistance(const LCreal) const = 0;
    LCreal convert(const Distance& n) { return fromDistance(n.toDistance()); }


    // Conversions between distances
    static LCreal MetersToMicroMeters(const LCreal v)    { return v * M2UM; }
    static LCreal MicroMetersToMeters(const LCreal v)    { return v * UM2M; }
    static LCreal MetersToCentiMeters(const LCreal v)    { return v * M2CM; }
    static LCreal CentiMetersToMeters(const LCreal v)    { return v * CM2M; }
    static LCreal MetersToKiloMeters(const LCreal v)     { return v * M2KM; }
    static LCreal KiloMetersToMeters(const LCreal v)     { return v * KM2M; }
    static LCreal MetersToInches(const LCreal v)         { return v * M2IN; }
    static LCreal InchesToMeters(const LCreal v)         { return v * IN2M; }
    static LCreal MetersToFeet(const LCreal v)           { return v * M2FT; }
    static LCreal FeetToMeters(const LCreal v)           { return v * FT2M; }
    static LCreal KiloMetersToNauticalMiles(const LCreal v)    { return (v * KM2M) * M2NM; }
    static LCreal NauticalMilesToKiloMeters(const LCreal v)    { return (v * NM2M) * M2KM; }
    static LCreal KiloMetersToStatuteMiles(const LCreal v)     { return (v * KM2M) * M2SM; }
    static LCreal StatuteMilesToKiloMeters(const LCreal v)     { return (v * SM2M) * M2KM; }
    static LCreal NauticalMilesToStatuteMiles(const LCreal v)  { return (v * NM2M) * M2SM; }
    static LCreal StatuteMilesToNauticalMiles(const LCreal v)  { return (v * SM2M) * M2NM; }


    // ---
    // Defined Distance Conversion Constants:
    // ---
    static const LCreal FT2M;    // Feet => Meters
    static const LCreal M2FT;    // Meters => Feet
    static const LCreal IN2M;    // Inches => Meters
    static const LCreal M2IN;    // Meters => Inches

    static const LCreal NM2M;    // Nautical Miles => Meters
    static const LCreal M2NM;    // Meters => Nautical Miles
    static const LCreal NM2FT;   // Nautical Miles => Feet
    static const LCreal FT2NM;   // Feet => Nautical Miles

    static const LCreal SM2M;    // Statue Miles => Meters
    static const LCreal M2SM;    // Meters => Statue Miles
    static const LCreal SM2FT;   // Statue Miles => Feet
    static const LCreal FT2SM;   // Feet => Statue Miles

    static const LCreal KM2M;    // Kilometers => Meters
    static const LCreal M2KM;    // Meters => Kilometers
    static const LCreal CM2M;    // Centimeters => Meters
    static const LCreal M2CM;    // Meters => Centimeters
    static const LCreal UM2M;    // Micrometer (Micron) => Meters
    static const LCreal M2UM;    // Meters => Micrometer (Micron)
};


inline std::ostream& operator<<(std::ostream& sout, const Distance& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getReal() << " )";
   return sout;
}


//------------------------------------------------------------------------------
// Class: Meters
// Base class:  Object -> Number -> Distance -> Meters
// Description:  An instance of Meters with its value equal to 1.0 is one
//               base unit for distances.
//------------------------------------------------------------------------------
class Meters : public Distance
{
    DECLARE_SUBCLASS(Meters, Distance)

public:
    Meters();
    Meters(const LCreal value);
    Meters(const Distance& value);

    static LCreal convertStatic(const Distance& n)        { return n.toDistance(); }
    // Distance interface
    LCreal toDistance() const override                    { return static_cast<LCreal>(val); }
    LCreal fromDistance(const LCreal a) const override    { return a; }
};


//------------------------------------------------------------------------------
// Class:  CentiMeters
// Base class:  Object -> Number -> Distance -> CentiMeters
// Description:  Meters * 100.0
//------------------------------------------------------------------------------
class CentiMeters : public Distance
{
    DECLARE_SUBCLASS(CentiMeters, Distance)

public:
    CentiMeters();
    CentiMeters(const LCreal value);
    CentiMeters(const Distance& value);

    static LCreal convertStatic(const Distance& n)        { return n.toDistance() * M2CM; }
    // Distance interface
    LCreal toDistance() const override                    { return static_cast<LCreal>(val * CM2M); }
    LCreal fromDistance(const LCreal a) const override    { return a * M2CM; }
};



//------------------------------------------------------------------------------
// Class:  MicroMeters
// Base class:  Object -> Number -> Distance -> MicroMeters
// Description:  Meters * 1,000,000.0
//------------------------------------------------------------------------------
class MicroMeters : public Distance
{
    DECLARE_SUBCLASS(MicroMeters, Distance)

public:
    MicroMeters();
    MicroMeters(const LCreal value);
    MicroMeters(const Distance& value);

    static LCreal convertStatic(const Distance& n)       { return n.toDistance() * M2UM; }
    // Distance interface
    LCreal toDistance() const override                   { return static_cast<LCreal>(val * UM2M); }
    LCreal fromDistance(const LCreal a) const override   { return a * M2UM; }
};

//------------------------------------------------------------------------------
// Class: Microns(Same as MicroMeters)
// Base class:  Object -> Number -> Distance -> MicroMeters
// Description:  Meters * 1,000,000.0
//------------------------------------------------------------------------------
class Microns : public Distance
{
    DECLARE_SUBCLASS(Microns, Distance)

public:
    Microns();
    Microns(const LCreal value);
    Microns(const Distance& value);

    static LCreal convertStatic(const Distance& n)       { return n.toDistance() * M2UM; }
    // Distance interface
    LCreal toDistance() const override                   { return static_cast<LCreal>(val * UM2M); }
    LCreal fromDistance(const LCreal a) const override   { return a * M2UM; }
};
//------------------------------------------------------------------------------
// Class:  KiloMeters
// Base class:  Object -> Number -> Distance -> KiloMeters
// Description:  Meters / 1000.0
//------------------------------------------------------------------------------
class KiloMeters : public Distance
{
    DECLARE_SUBCLASS(KiloMeters, Distance)

public:
    KiloMeters();
    KiloMeters(const LCreal value);
    KiloMeters(const Distance& value);

    static LCreal convertStatic(const Distance& n)      { return n.toDistance() * M2KM; }
    // Distance interface
    LCreal toDistance() const override                  { return static_cast<LCreal>(val * KM2M); }
    LCreal fromDistance(const LCreal a) const override  { return a * M2KM; }
};


//------------------------------------------------------------------------------
// Class:  Inches
// Base class:  Object -> Number -> Distance -> Inches
// Description:  Meters / 0.0254
//------------------------------------------------------------------------------
class Inches : public Distance
{
    DECLARE_SUBCLASS(Inches, Distance)

public:
    Inches();
    Inches(const LCreal value);
    Inches(const Distance& value);

    static LCreal convertStatic(const Distance& n)      { return n.toDistance() * M2IN; }
    // Distance interface
    LCreal toDistance() const override                  { return static_cast<LCreal>(val * IN2M); }
    LCreal fromDistance(const LCreal a) const override  { return a * M2IN; }
};


//------------------------------------------------------------------------------
// Class:  Feet
// Base class:  Object -> Number -> Distance -> Feet
// Description:  Meters / 0.3048
//------------------------------------------------------------------------------
class Feet : public Distance
{
    DECLARE_SUBCLASS(Feet, Distance)

public:
    Feet();
    Feet(const LCreal value);
    Feet(const Distance& value);

    static LCreal convertStatic(const Distance& n)      { return n.toDistance() * M2FT; }
    // Distance interface
    LCreal toDistance() const override                  { return static_cast<LCreal>(val * FT2M); }
    LCreal fromDistance(const LCreal a) const override  { return a * M2FT; }
};


//------------------------------------------------------------------------------
// Class:  NauticalMiles
// Base class:  Object -> Number -> Distance -> NauticalMiles
// Description:  Meters * 1851.999942
//------------------------------------------------------------------------------
class NauticalMiles : public Distance
{
    DECLARE_SUBCLASS(NauticalMiles, Distance)

public:
    NauticalMiles();
    NauticalMiles(const LCreal value);
    NauticalMiles(const Distance& value);

    static LCreal convertStatic(const Distance& n)    { return n.toDistance() * M2NM; }
    // Distance interface
    LCreal toDistance() const override                 { return static_cast<LCreal>(val * NM2M); }
    LCreal fromDistance(const LCreal a) const override { return a * M2NM; }
};


//------------------------------------------------------------------------------
// Class:  StatuteMiles
// Base class:  Object -> Number -> Distance -> StatuteMiles
// Description:  Meters * 1609.34313095
//------------------------------------------------------------------------------
class StatuteMiles : public Distance
{
    DECLARE_SUBCLASS(StatuteMiles, Distance)

public:
    StatuteMiles();
    StatuteMiles(const LCreal value);
    StatuteMiles(const Distance& value);

    static LCreal convertStatic(const Distance& n)    { return n.toDistance() * M2SM; }
    // Distance interface
    LCreal toDistance() const override                 { return static_cast<LCreal>(val * SM2M); }
    LCreal fromDistance(const LCreal a) const override { return a * M2SM; }
};

} // End Basic namespace
} // End Eaagles namespace

#endif
