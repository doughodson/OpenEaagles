//------------------------------------------------------------------------------
// Classes:  Area, SquareFeet, SquareInches, SquareYards, SquareMiles,
//           SquareCentiMeters, SquareMeters, SquareMilliMeters,
//           SquareKiloMeters, DecibelSquareMeters
//
// Base class:  Object -> Number -> Area
//              Object -> Number -> Area -> SquareMeters
//              Object -> Number -> Area -> SquareFeet
//              Object -> Number -> Area -> SquareInches
//              Object -> Number -> Area -> SquareYards
//              Object -> Number -> Area -> SquareMiles
//              Object -> Number -> Area -> SquareCentiMeters
//              Object -> Number -> Area -> SquareMilliMeters
//              Object -> Number -> Area -> SquareKiloMeters
//              Object -> Number -> Area -> DecibelSquareMeters
//------------------------------------------------------------------------------
#ifndef __oe_base_Areas_H__
#define __oe_base_Areas_H__

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/units/area_utils.hpp"

#include <cmath>
#include <iostream>

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class:  Area
// Base class:  Object -> Number -> Area
// Description:  Base class for area.  Defined as a square meter which is
//               equivalent to an instance of SquareMeters with its value equal
//               to 1.0.
//
// Public methods (Defined in Area, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Area derived instance with an double.
//
//     set(const Area& n)
//        Sets, and converts if necessary, an Area derived instance with
//        another Area derived instance.
//
//     double convert(const Area& n)
//        Converts the value of an Area derived instance into
//        the units of another Area derived instance.
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Area& n)
//        Sends "( <the Area derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes: SquareFeet, SquareYards, SquareMeters, SquareMiles, SquareInches,
//                  SquareCentiMeters, SquareMilliMeters, SquareKiloMeters, DecibelSquareMeters):
//
//     double convertStatic(const Area& n)
//        Static function to convert the given Area derived instance
//        into the units of a specific Area derived class.
//
//------------------------------------------------------------------------------
class Area : public Number
{
    DECLARE_SUBCLASS(Area, Number)

public:
    Area();
    Area(const double value);

    void set(const double v) { val = v; }
    void set(const Area& n)  { val = fromArea(n.toArea()); }

    virtual double toArea() const = 0;
    virtual double fromArea(const double a) const = 0;
    double convert(const Area& n) { return fromArea(n.toArea()); }
};


inline std::ostream& operator<<(std::ostream& sout, const Area& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class:  SquareMeters
// Base class:  Object -> Number -> Area -> SquareMeters
// Description: An instance of SquareMeters with its value equal to 1.0 is one
//              base unit for area.
//------------------------------------------------------------------------------
class SquareMeters : public Area
{
    DECLARE_SUBCLASS(SquareMeters, Area)

public:
    SquareMeters();
    SquareMeters(const double value);
    SquareMeters(const Area& value);

    static double convertStatic(const Area& n)              { return n.toArea(); }
    virtual double toArea() const override                  { return static_cast<double>(val); }
    virtual double fromArea(const double a) const override  { return a; }
};

//------------------------------------------------------------------------------
// Class:  SquareFeet
// Base class:  Object -> Number -> Area -> SquareFeet
// Description: Square Meters * 10.76391
//------------------------------------------------------------------------------
class SquareFeet : public Area
{
    DECLARE_SUBCLASS(SquareFeet, Area)

public:
    SquareFeet();
    SquareFeet(const double value);
    SquareFeet(const Area& value);

    static double convertStatic(const Area& n)              { return n.toArea() * area::SM2SFT; }
    virtual double toArea() const override                  { return static_cast<double>(val * area::SFT2SM); }
    virtual double fromArea(const double a) const override  { return a * area::SM2SFT; }
};

//------------------------------------------------------------------------------
// Class:  SquareInches
// Base class:  Object -> Number -> Area -> SquareInches
// Description: Square Meters * 1550.0030399
//------------------------------------------------------------------------------
class SquareInches : public Area
{
    DECLARE_SUBCLASS(SquareInches, Area)

public:
    SquareInches();
    SquareInches(const double value);
    SquareInches(const Area& value);

    static double convertStatic(const Area& n)              { return n.toArea() * area::SM2SIN; }
    virtual double toArea() const override                  { return static_cast<double>(val * area::SIN2SM); }
    virtual double fromArea(const double a) const override  { return a * area::SM2SIN; }
};

//------------------------------------------------------------------------------
// Class:  SquareYards
// Base class:  Object -> Number -> Area -> SquareYards
// Description: Square Meters * 1.19599
//------------------------------------------------------------------------------
class SquareYards : public Area
{
    DECLARE_SUBCLASS(SquareYards, Area)

public:
    SquareYards();
    SquareYards(const double value);
    SquareYards(const Area& value);

    static double convertStatic(const Area& n)              { return n.toArea() * area::SM2SYD; }
    virtual double toArea() const override                  { return static_cast<double>(val * area::SYD2SM); }
    virtual double fromArea(const double a) const override  { return a * area::SM2SYD; }
};

//------------------------------------------------------------------------------
// Class:  SquareMiles
// Base class:  Object -> Number -> Area -> SquareMiles
// Description: Square Meters * 0.00000038610216
//------------------------------------------------------------------------------
class SquareMiles : public Area
{
    DECLARE_SUBCLASS(SquareMiles, Area)

public:
    SquareMiles();
    SquareMiles(const double value);
    SquareMiles(const Area& value);

    static double convertStatic(const Area& n)              { return n.toArea() * area::SM2SMI; }
    virtual double toArea() const override                  { return static_cast<double>(val * area::SMI2SM); }
    virtual double fromArea(const double a) const override  { return a * area::SM2SMI; }
};

//------------------------------------------------------------------------------
// Class:  SquareCentiMeters
// Base class:  Object -> Number -> Area -> SquareCentiMeters
// Description: Square Meters * 10000.0
//------------------------------------------------------------------------------
class SquareCentiMeters : public Area
{
    DECLARE_SUBCLASS(SquareCentiMeters, Area)

public:
    SquareCentiMeters();
    SquareCentiMeters(const double value);
    SquareCentiMeters(const Area& value);

    static double convertStatic(const Area& n)              { return n.toArea() * area::SM2SCM; }
    virtual double toArea() const override                  { return static_cast<double>(val * area::SCM2SM); }
    virtual double fromArea(const double a) const override  { return a * area::SM2SCM; }
};

//------------------------------------------------------------------------------
// Class:  SquareMilliMeters
// Base class:  Object -> Number -> Area -> SquareMilliMeters
// Description: Square Meters * 1000000.0
//------------------------------------------------------------------------------
class SquareMilliMeters : public Area
{
    DECLARE_SUBCLASS(SquareMilliMeters, Area)

public:
    SquareMilliMeters();
    SquareMilliMeters(const double value);
    SquareMilliMeters(const Area& value);

    static double convertStatic(const Area& n)              { return n.toArea() * area::SM2SMM; }
    virtual double toArea() const override                  { return static_cast<double>(val * area::SMM2SM); }
    virtual double fromArea(const double a) const override  { return a * area::SM2SMM; }
};

//------------------------------------------------------------------------------
// Class:  SquareKiloMeters
// Base class:  Object -> Number -> Area -> SquareKiloMeters
// Description: Square Meters * 0.000001
//------------------------------------------------------------------------------
class SquareKiloMeters : public Area
{
    DECLARE_SUBCLASS(SquareKiloMeters, Area)

public:
    SquareKiloMeters();
    SquareKiloMeters(const double value);
    SquareKiloMeters(const Area& value);

    static double convertStatic(const Area& n)              { return n.toArea() * area::SM2SKM; }
    virtual double toArea() const override                  { return static_cast<double>(val * area::SKM2SM); }
    virtual double fromArea(const double a) const override  { return a * area::SM2SKM; }
};

//------------------------------------------------------------------------------
// Class:  DecibelSquareMeters
// Base class:  Object -> Number -> Area -> DecibelSquareMeters
// Description: 10 * Log(Square Meters)
//------------------------------------------------------------------------------
class DecibelSquareMeters : public Area
{
    DECLARE_SUBCLASS(DecibelSquareMeters, Area)

public:
    DecibelSquareMeters();
    DecibelSquareMeters(const double value);
    DecibelSquareMeters(const Area& value);

    static double convertStatic(const Area& n)             { return 10.0f * std::log10( n.toArea() ); }
    virtual double toArea() const override                 { return std::pow(static_cast<double>(10.0), static_cast<double>(val/10.0)); }
    virtual double fromArea(const double a) const override { return 10.0f * std::log10(a); }
};

}
}

#endif
