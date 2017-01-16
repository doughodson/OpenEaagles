//------------------------------------------------------------------------------
// Classes:  Volume, CubicMeters, CubicFeet, CubicInches, Liters
//
// Base class:  Object -> Number -> Volume
//              Object -> Number -> Volume -> CubicMeters
//              Object -> Number -> Volume -> CubicFeet
//              Object -> Number -> Volume -> CubicInches
//              Object -> Number -> Volume -> Liters
//------------------------------------------------------------------------------

#ifndef __oe_base_Volumes_H__
#define __oe_base_Volumes_H__

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/units/volume_utils.hpp"
#include <iostream>

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: Volume
// Description:  Base class for volume.  Defined as a cubic meter which is
// equivalent to an instance of CubicMeters with its value equal to 1.0.
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
};

inline std::ostream& operator<<(std::ostream& sout, const Volume& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }


//------------------------------------------------------------------------------
// Class: CubicMeters
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
// Description: Cubic Meters * 35.31467
//------------------------------------------------------------------------------
class CubicFeet : public Volume
{
    DECLARE_SUBCLASS(CubicFeet, Volume)

public:
    CubicFeet();
    CubicFeet(const double value);
    CubicFeet(const Volume& value);

    static double convertStatic(const Volume &n)             { return n.toVolume() * volume::CM2CFT; }
    virtual double toVolume() const override                 { return static_cast<double>(val * volume::CFT2CM); }
    virtual double fromVolume(const double a) const override { return a * volume::CM2CFT; }
};


//------------------------------------------------------------------------------
// Class: CubicInches
// Description: Cubic Meters * 61023.74
//------------------------------------------------------------------------------
class CubicInches : public Volume
{
    DECLARE_SUBCLASS(CubicInches, Volume)

public:
    CubicInches();
    CubicInches(const double value);
    CubicInches(const Volume& value);

    static double convertStatic(const Volume &n)             { return n.toVolume() * volume::CM2CIN; }
    virtual double toVolume() const override                 { return static_cast<double>(val * volume::CIN2CM); }
    virtual double fromVolume(const double a) const override { return a * volume::CM2CIN; }
};


//------------------------------------------------------------------------------
// Class: Liters
// Description: Cubic Meters * 1000
//------------------------------------------------------------------------------
class Liters : public Volume
{
    DECLARE_SUBCLASS(Liters, Volume)

public:
    Liters();
    Liters(const double value);
    Liters(const Volume& value);

    static double convertStatic(const Volume &n)             { return n.toVolume() * volume::CM2L; }
    virtual double toVolume() const override                 { return static_cast<double>(val * volume::L2CM); }
    virtual double fromVolume(const double a) const override { return a * volume::CM2L; }
};

}
}

#endif
