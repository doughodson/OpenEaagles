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
//
// Description:  Numbers as an area -- Square: Feet, In, Yds, Miles, CentiMeters,
//               MilliMeters, KiloMeters, and Decibel Meters
//
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
//     Conversion routines:
//          Because there are 72 possible combinations of conversions, only the most appropriate
//          ones will be included here.  (ie.. inches to feet).  All other conversions can be
//          handled using the set or convert member function of each class type (ie.. CentiMeters to Meters).
//
//          double squareFeetToInches(const double v)
//          double squareFeetToYards(const double v)
//          double squareFeetToMeters(const double v)
//          double squareFeetToMiles(const double v)
//          double squareInchesToFeet(const double v)
//          double squareInchesToYards(const double v)
//          double squareInchesToMeters(const double v)
//          double squareInchesToMiles(const double v)
//          double squareYardsToFeet(const double v)
//          double squareYardsToInches(const double v)
//          double squareYardsToMeters(const double v)
//          double squareYardsToMiles(const double v)
//          double squareMilesToFeet(const double v)
//          double squareMilesToInches(const double v)
//          double squareMilesToYards(const double v)
//          double squareMilesToMeters(const double v)
//          double squareMetersToFeet(const double v)
//          double squareMetersToInches(const double v)
//          double squareMetersToYards(const double v)
//          double squareMetersToMiles(const double v)
//          double squareMetersToDecibelSquareMeters(const double v)
//          double decibelSquareMetersToSquareMeters(const double v)
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
#ifndef __oe_base_Areas_H__
#define __oe_base_Areas_H__

#include "openeaagles/base/Number.hpp"

#include <cmath>
#include <iostream>

namespace oe {
namespace base {

// ----------------------------------------------------------------------------
// Define Area Conversion Constants:
//
// These constants were obtained from the following websites, and are assumed
// accurate as of 2/5/03.
//
// http://www.engineering.ucsb.edu/~me15web/top_menu/tables/table_4_unit_conversion.htm#4a
// - California State Berkley Engineering Department
// http://web.aces.uiuc.edu/aim/IAH/math/area.html - University of Illinois
// http://marian.creighton.edu/~besser/javascr/area.html - Creighton University
// http://www.units.muohio.edu/hydrogeology/glg207l/207lexercises/areas.jpg - Ohio University
// http://quickfacts.census.gov/qfd/meta/long_101619.htm - Census Department
// http://academic.regis.edu/mlotfy/cs422/Problem%20Document.htm - Regis University
// http://unr.edu/homepage/kimura/tables.html - University of Nevada, Reno
// All Decibel conversions came from the radar detection system PowerPoint presentation
// slides 47 and 48.
// ----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class:  Area
// Base class:  Object -> Number -> Area
// Description:  Base class for area.  Defined as a square meter which is
//               equivalent to an instance of SquareMeters with its value equal
//               to 1.0.
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

    // Conversions between Areas
    static double squareFeetToInches(const double v)   { return (v * SFT2SM) * SM2SIN; }
    static double squareFeetToYards(const double v)    { return (v * SFT2SM) * SM2SYD; }
    static double squareFeetToMeters(const double v)   { return v * SFT2SM; }
    static double squareFeetToMiles(const double v)    { return (v * SFT2SM) * SM2SMI; }
    static double squareInchesToFeet(const double v)   { return (v * SIN2SM) * SM2SFT; }
    static double squareInchesToYards(const double v)  { return (v * SIN2SM) * SM2SYD; }
    static double squareInchesToMeters(const double v) { return v * SIN2SM; }
    static double squareInchesToMiles(const double v)  { return (v * SIN2SM) * SM2SMI; }
    static double squareYardsToFeet(const double v)    { return (v * SYD2SM) * SM2SFT; }
    static double squareYardsToInches(const double v)  { return (v * SYD2SM) * SM2SIN; }
    static double squareYardsToMeters(const double v)  { return v * SYD2SM; }
    static double squareYardsToMiles(const double v)   { return (v * SYD2SM) * SM2SMI; }
    static double squareMilesToFeet(const double v)    { return (v * SMI2SM) * SM2SFT; }
    static double squareMilesToInches(const double v)  { return (v * SMI2SM) * SM2SIN; }
    static double squareMilesToYards(const double v)   { return (v * SMI2SM) * SM2SYD; }
    static double squareMilesToMeters(const double v)  { return v * SMI2SM; }
    static double squareMetersToFeet(const double v)   { return v * SM2SFT; }
    static double squareMetersToInches(const double v) { return v * SM2SIN; }
    static double squareMetersToYards(const double v)  { return v * SM2SYD; }
    static double squareMetersToMiles(const double v)  { return v * SM2SMI; }
    static double squareMetersToDecibelSquareMeters(const double v) { return 10.0f * std::log10(v); }
    static double decibelSquareMetersToSquareMeters(const double v) { return std::pow(static_cast<double>(10.0), static_cast<double>(v/10.0)); }

    // Conversion constants
    static const double SM2SFT;
    static const double SFT2SM;
    static const double SM2SYD;
    static const double SYD2SM;
    static const double SM2SMI;
    static const double SMI2SM;
    static const double SM2SIN;
    static const double SIN2SM;
    static const double SM2SCM;
    static const double SCM2SM;
    static const double SM2SMM;
    static const double SMM2SM;
    static const double SM2SKM;
    static const double SKM2SM;
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

    static double convertStatic(const Area& n)              { return n.toArea() * SM2SFT; }
    virtual double toArea() const override                  { return static_cast<double>(val * SFT2SM); }
    virtual double fromArea(const double a) const override  { return a * SM2SFT; }
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

    static double convertStatic(const Area& n)              { return n.toArea() * SM2SIN; }
    virtual double toArea() const override                  { return static_cast<double>(val * SIN2SM); }
    virtual double fromArea(const double a) const override  { return a * SM2SIN; }
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

    static double convertStatic(const Area& n)              { return n.toArea() * SM2SYD; }
    virtual double toArea() const override                  { return static_cast<double>(val * SYD2SM); }
    virtual double fromArea(const double a) const override  { return a * SM2SYD; }
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

    static double convertStatic(const Area& n)              { return n.toArea() * SM2SMI; }
    virtual double toArea() const override                  { return static_cast<double>(val * SMI2SM); }
    virtual double fromArea(const double a) const override  { return a * SM2SMI; }
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

    static double convertStatic(const Area& n)              { return n.toArea() * SM2SCM; }
    virtual double toArea() const override                  { return static_cast<double>(val * SCM2SM); }
    virtual double fromArea(const double a) const override  { return a * SM2SCM; }
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

    static double convertStatic(const Area& n)              { return n.toArea() * SM2SMM; }
    virtual double toArea() const override                  { return static_cast<double>(val * SMM2SM); }
    virtual double fromArea(const double a) const override  { return a * SM2SMM; }
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

    static double convertStatic(const Area& n)              { return n.toArea() * SM2SKM; }
    virtual double toArea() const override                  { return static_cast<double>(val * SKM2SM); }
    virtual double fromArea(const double a) const override  { return a * SM2SKM; }
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
