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
//     set(const double v)
//        Sets a Distance derived instance with an double.
//
//     set(const Distance& n)
//        Sets, and converts if necessary, a Distance derived instance with
//        another Distance derived instance.
//
//     double convert(const Distance& n)
//        Converts the value of a Distance derived instance into
//        the units of another Distance derived instance.
//
//
// Conversion routines:
//       double MetersToMicroMeters(const double v)
//       double MicroMetersToMeters(const double v
//       double MetersToCentiMeters(const double v)
//       double CentiMetersToMeters(const double v)
//       double MetersToKiloMeters(const double v)
//       double KiloMetersToMeters(const double v)
//       double MetersToInches(const double v)
//       double InchesToMeters(const double v)
//       double MetersToFeet(const double v)
//       double FeetToMeters(const double v)
//       double KiloMetersToNauticalMiles(const double v)
//       double NauticalMilesToKiloMeters(const double v)
//       double KiloMetersToStatuteMiles(const double v)
//       double StatuteMilesToKiloMeters(const double v)
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
// double convertStatic(const Distance& n) const
//       Static function to convert the given Distance derived
//       instance into the units of a specific Distance derived
//       class.
//
//------------------------------------------------------------------------------
#ifndef __oe_base_Distance_H__
#define __oe_base_Distance_H__

#include "openeaagles/base/Number.hpp"
#include <iostream>

namespace oe {
namespace base {

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
    Distance(const double value);

    void set(const double v)      { val = v; }
    void set(const Distance& n)   { val = fromDistance(n.toDistance()); }

    virtual double toDistance() const = 0;
    virtual double fromDistance(const double) const = 0;
    double convert(const Distance& n) { return fromDistance(n.toDistance()); }


    // Conversions between distances
    static double MetersToMicroMeters(const double v)    { return v * M2UM; }
    static double MicroMetersToMeters(const double v)    { return v * UM2M; }
    static double MetersToCentiMeters(const double v)    { return v * M2CM; }
    static double CentiMetersToMeters(const double v)    { return v * CM2M; }
    static double MetersToKiloMeters(const double v)     { return v * M2KM; }
    static double KiloMetersToMeters(const double v)     { return v * KM2M; }
    static double MetersToInches(const double v)         { return v * M2IN; }
    static double InchesToMeters(const double v)         { return v * IN2M; }
    static double MetersToFeet(const double v)           { return v * M2FT; }
    static double FeetToMeters(const double v)           { return v * FT2M; }
    static double KiloMetersToNauticalMiles(const double v)    { return (v * KM2M) * M2NM; }
    static double NauticalMilesToKiloMeters(const double v)    { return (v * NM2M) * M2KM; }
    static double KiloMetersToStatuteMiles(const double v)     { return (v * KM2M) * M2SM; }
    static double StatuteMilesToKiloMeters(const double v)     { return (v * SM2M) * M2KM; }
    static double NauticalMilesToStatuteMiles(const double v)  { return (v * NM2M) * M2SM; }
    static double StatuteMilesToNauticalMiles(const double v)  { return (v * SM2M) * M2NM; }


    // ---
    // Defined Distance Conversion Constants:
    // ---
    static const double FT2M;    // Feet => Meters
    static const double M2FT;    // Meters => Feet
    static const double IN2M;    // Inches => Meters
    static const double M2IN;    // Meters => Inches

    static const double NM2M;    // Nautical Miles => Meters
    static const double M2NM;    // Meters => Nautical Miles
    static const double NM2FT;   // Nautical Miles => Feet
    static const double FT2NM;   // Feet => Nautical Miles

    static const double SM2M;    // Statue Miles => Meters
    static const double M2SM;    // Meters => Statue Miles
    static const double SM2FT;   // Statue Miles => Feet
    static const double FT2SM;   // Feet => Statue Miles

    static const double KM2M;    // Kilometers => Meters
    static const double M2KM;    // Meters => Kilometers
    static const double CM2M;    // Centimeters => Meters
    static const double M2CM;    // Meters => Centimeters
    static const double UM2M;    // Micrometer (Micron) => Meters
    static const double M2UM;    // Meters => Micrometer (Micron)
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
    Meters(const double value);
    Meters(const Distance& value);

    static double convertStatic(const Distance& n)                { return n.toDistance(); }
    virtual double toDistance() const override                    { return static_cast<double>(val); }
    virtual double fromDistance(const double a) const override    { return a; }
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
    CentiMeters(const double value);
    CentiMeters(const Distance& value);

    static double convertStatic(const Distance& n)                { return n.toDistance() * M2CM; }
    virtual double toDistance() const override                    { return static_cast<double>(val * CM2M); }
    virtual double fromDistance(const double a) const override    { return a * M2CM; }
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
    MicroMeters(const double value);
    MicroMeters(const Distance& value);

    static double convertStatic(const Distance& n)               { return n.toDistance() * M2UM; }
    virtual double toDistance() const override                   { return static_cast<double>(val * UM2M); }
    virtual double fromDistance(const double a) const override   { return a * M2UM; }
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
    Microns(const double value);
    Microns(const Distance& value);

    static double convertStatic(const Distance& n)               { return n.toDistance() * M2UM; }
    virtual double toDistance() const override                   { return static_cast<double>(val * UM2M); }
    virtual double fromDistance(const double a) const override   { return a * M2UM; }
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
    KiloMeters(const double value);
    KiloMeters(const Distance& value);

    static double convertStatic(const Distance& n)              { return n.toDistance() * M2KM; }
    virtual double toDistance() const override                  { return static_cast<double>(val * KM2M); }
    virtual double fromDistance(const double a) const override  { return a * M2KM; }
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
    Inches(const double value);
    Inches(const Distance& value);

    static double convertStatic(const Distance& n)              { return n.toDistance() * M2IN; }
    virtual double toDistance() const override                  { return static_cast<double>(val * IN2M); }
    virtual double fromDistance(const double a) const override  { return a * M2IN; }
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
    Feet(const double value);
    Feet(const Distance& value);

    static double convertStatic(const Distance& n)              { return n.toDistance() * M2FT; }
    virtual double toDistance() const override                  { return static_cast<double>(val * FT2M); }
    virtual double fromDistance(const double a) const override  { return a * M2FT; }
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
    NauticalMiles(const double value);
    NauticalMiles(const Distance& value);

    static double convertStatic(const Distance& n)             { return n.toDistance() * M2NM; }
    virtual double toDistance() const override                 { return static_cast<double>(val * NM2M); }
    virtual double fromDistance(const double a) const override { return a * M2NM; }
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
    StatuteMiles(const double value);
    StatuteMiles(const Distance& value);

    static double convertStatic(const Distance& n)             { return n.toDistance() * M2SM; }
    virtual double toDistance() const override                 { return static_cast<double>(val * SM2M); }
    virtual double fromDistance(const double a) const override { return a * M2SM; }
};

}
}

#endif
