//------------------------------------------------------------------------------
// Classes:  Angle, Degrees, Radians, Semicircles
//------------------------------------------------------------------------------

#ifndef __oe_base_Angles_H__
#define __oe_base_Angles_H__

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/util/constants.hpp"
#include "openeaagles/base/units/angle_utils.hpp"
#include <iostream>

#include <cmath>

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class:  Angle
// Description:  Base class for angles.  Defined as a semicircle which is
//               equivalent to an instance of Semicircle with its value equal
//               to 1.0.
//
// Public methods (Defined in Angle, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Angle derived instance with a double.
//
//     set(const Angle& n)
//        Sets, and converts if necessary, an Angle derived instance with
//        another Angle derived instance.
//
//     double convert(const Angle& n)
//        Converts the value of an Angle derived instance into
//        the units of another Angle derived instance.
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Angle& n)
//        Sends "( <the Angle derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  Degrees, Radians, Semicircles):
//
//     double convertStatic(const Angle& n)
//        Static function to convert the given Angle derived
//        instance into the units of a specific Angle derived class.
//
//------------------------------------------------------------------------------
class Angle : public Number
{
    DECLARE_SUBCLASS(Angle, Number)

public:
    Angle();
    Angle(const double value);

    void set(const double v) { val = v; }
    void set(const Angle& n) { val = fromAngle(n.toAngle()); }

    virtual double toAngle() const = 0;
    virtual double fromAngle(const double a) const = 0;
    double convert(const Angle& n) { return fromAngle(n.toAngle()); }
};

inline std::ostream& operator<<(std::ostream& sout, const Angle& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }


//------------------------------------------------------------------------------
// Class: Degrees
// Description: Angle * 180.0
//------------------------------------------------------------------------------
class Degrees : public Angle
{
    DECLARE_SUBCLASS(Degrees, Angle)

public:
    Degrees();
    Degrees(const double value);
    Degrees(const Angle& value);

    static double convertStatic(const Angle& n)              { return n.toAngle() * angle::SC2D; }
    virtual double toAngle() const override                  { return static_cast<double>(val * angle::D2SC); }
    virtual double fromAngle(const double a) const override  { return a * angle::SC2D; }
};

//------------------------------------------------------------------------------
// Class: Radians
// Description: Angle * PI
//------------------------------------------------------------------------------
class Radians : public Angle
{
    DECLARE_SUBCLASS(Radians, Angle)

public:
    Radians();
    Radians(const double value);
    Radians(const Angle& value);

    static double convertStatic(const Angle& n)              { return n.toAngle() * angle::SC2R; }
    virtual double toAngle() const override                  { return static_cast<double>(val * angle::R2SC); }
    virtual double fromAngle(const double a) const override  { return a * angle::SC2R; }
};

//------------------------------------------------------------------------------
// Class: Semicircles
// Description:  An instance of Semicircles with its value equal to 1.0 is
//               one base unit for distances.
//------------------------------------------------------------------------------
class Semicircles : public Angle
{
    DECLARE_SUBCLASS(Semicircles, Angle)

public:
    Semicircles();
    Semicircles(const double value);
    Semicircles(const Angle& value);

    static double convertStatic(const Angle& n)              { return n.toAngle(); }
    virtual double toAngle() const override                  { return static_cast<double>(val); }
    virtual double fromAngle(const double a) const override  { return a; }
};

}
}

#endif

