//------------------------------------------------------------------------------
// Classes:  Volume, CubicMeters, CubicFeet, CubicInches, Liters
//
// Base class:  Object -> Number -> Volume
//              Object -> Number -> Volume -> CubicMeters
//              Object -> Number -> Volume -> CubicFeet
//              Object -> Number -> Volume -> CubicInches
//              Object -> Number -> Volume -> Liters
//
// Description:  Numbers as a volume -- Cubic: Meters, Feet, Inches, and Liters.
//
//
// Public methods (Defined in Volume, and inherited by all derived classes):
//
//     set(const double v)
//        Sets a Volume derived instance with an double.
//
//     set(const Volume& n)
//        Sets, and converts if necessary, a Volume derived
//        instance with another Volume derived instance.
//
//     double convert(const Volume& n)
//        Converts the value of a Volume derived instance into
//        the units of another Volume derived instance.
//
//    Conversion routines:
//      double cubicMetersToFeet(const double v)   { return v * CM2CFT; }
//      double cubicFeetToMeters(const double v)   { return v * CFT2CM; }
//      double litersToCubicMeters(const double v) { return v * L2CM; }
//      double cubicMetersToLiters(const double v) { return v * CM2L; }
//      double cubicFeetToInches(const double v)   { return (v * CFT2CM) * CM2CIN; }
//      double cubicInchesToFeet(const double v)   { return (v * CIN2CM) * CM2CFT; }
//
//   Output stream operator:<<
//        ostream& operator<<(ostream& sout, const Volume& n)
//        Sends "( <the Volume derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes: CubicMeters, CubicFeet, CubicInches, Liters):
//    double convertStatic(const Volume &n)
//    Static function to convert the given Volume derived instance
//    into the units of a specific Volume derived class.
//
//------------------------------------------------------------------------------

#ifndef __oe_base_Volumes_H__
#define __oe_base_Volumes_H__

#include "openeaagles/base/Number.h"

namespace oe {
namespace base {

// ----------------------------------------------------------------------------
// Define Volumes Conversion Constants:
//
// These constants were obtained from the following websites, and are assumed
// accurate as of 9/03/03.
//
// http://ts.nist.gov/ts/htdocs/230/235/h4402/appenc.pdf - National Institute of Standards and Technology
// http://www.calculator.org/properties/volume.prop - Flow Simulation Calculator
// http://www.gcsaa.org/resource/supertools/volume_form.asp - Golf Course Superintendents Association of America
// http://www-cta.ornl.gov/cta/data/tedb22/Edition22_AppendixB.pdf - Transportation Energy Data Book: Edition 22 - 2002
// ----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class: Volume
// Base class:  Object -> Number -> Volume
// Description:  Base class for volume.  Defined as a cubic meter which is
// equivalent to an instance of CubicMeters with its value equal to 1.0.
//------------------------------------------------------------------------------
class Volume : public Number
{
    DECLARE_SUBCLASS(Volume, Number)

public:
    Volume();
    Volume(const double value);

    void set(const double v)  { val = v; }
    void set(const Volume& n) { val = fromVolume(n.toVolume()); }

    virtual double toVolume() const = 0;
    virtual double fromVolume(const double a) const = 0;
    double convert(const Volume& n) const  { return fromVolume(n.toVolume()); }

    // Conversion routines
    static double cubicMetersToFeet(const double v)   { return v * CM2CFT; }
    static double cubicFeetToMeters(const double v)   { return v * CFT2CM; }

    static double litersToCubicMeters(const double v) { return v * L2CM; }
    static double cubicMetersToLiters(const double v) { return v * CM2L; }

    static double cubicFeetToInches(const double v)   { return (v * CFT2CM) * CM2CIN; }
    static double cubicInchesToFeet(const double v)   { return (v * CIN2CM) * CM2CFT; }

    // Conversion constants
    static const double CM2CFT;  // CubicMeters -> CubicFeet
    static const double CFT2CM;  // CubicFeet -> CubicMeters
    static const double CM2CIN;  // CubicMeters -> CubicInches
    static const double CIN2CM;  // CubicInches -> CubicMeters
    static const double CM2L;    // CubicMeters -> Liters
    static const double L2CM;    // Liters -> CubicMeters
};


inline std::ostream& operator<<(std::ostream& sout, const Volume& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }


//------------------------------------------------------------------------------
// Class: CubicMeters
// Base class:  Object -> Number -> Volume -> CubicMeters
// Description: An instance of CubicMeters with its value equal to 1.0 is one
// base unit for volume.
//------------------------------------------------------------------------------
class CubicMeters : public Volume
{
    DECLARE_SUBCLASS(CubicMeters, Volume)

public:
    CubicMeters();
    CubicMeters(const double value);
    CubicMeters(const Volume& value);

    static double convertStatic(const Volume &n)              { return n.toVolume(); }
    virtual double toVolume() const override                  { return static_cast<double>(val); }
    virtual double fromVolume(const double a) const override  { return a; }
};

//------------------------------------------------------------------------------
// Class: CubicFeet
// Base class:  Object -> Number -> Volume -> CubicFeet
// Description: Cubic Meters * 35.31467
//------------------------------------------------------------------------------
class CubicFeet : public Volume
{
    DECLARE_SUBCLASS(CubicFeet, Volume)

public:
    CubicFeet();
    CubicFeet(const double value);
    CubicFeet(const Volume& value);

    static double convertStatic(const Volume &n)             { return n.toVolume() * CM2CFT; }
    virtual double toVolume() const override                 { return static_cast<double>(val * CFT2CM); }
    virtual double fromVolume(const double a) const override { return a * CM2CFT; }
};


//------------------------------------------------------------------------------
// Class: CubicInches
// Base class:  Object -> Number -> Volume -> CubicInches
// Description: Cubic Meters * 61023.74
//------------------------------------------------------------------------------
class CubicInches : public Volume
{
    DECLARE_SUBCLASS(CubicInches, Volume)

public:
    CubicInches();
    CubicInches(const double value);
    CubicInches(const Volume& value);

    static double convertStatic(const Volume &n)             { return n.toVolume() * CM2CIN; }
    virtual double toVolume() const override                 { return static_cast<double>(val * CIN2CM); }
    virtual double fromVolume(const double a) const override { return a * CM2CIN; }
};


//------------------------------------------------------------------------------
// Class: Liters
// Base class:  Object -> Number -> Volume -> Liters
// Description: Cubic Meters * 1000
//------------------------------------------------------------------------------
class Liters : public Volume
{
    DECLARE_SUBCLASS(Liters, Volume)

public:
    Liters();
    Liters(const double value);
    Liters(const Volume& value);

    static double convertStatic(const Volume &n)             { return n.toVolume() * CM2L; }
    virtual double toVolume() const override                 { return static_cast<double>(val * L2CM); }
    virtual double fromVolume(const double a) const override { return a * CM2L; }
};

}
}

#endif
