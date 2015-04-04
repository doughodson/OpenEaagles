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
//     set(const LCreal v)
//        Sets an Area derived instance with an LCreal.
//
//     set(const Area& n)
//        Sets, and converts if necessary, an Area derived instance with
//        another Area derived instance.
//
//     LCreal convert(const Area& n)
//        Converts the value of an Area derived instance into
//        the units of another Area derived instance.
//
//     Conversion routines:
//          Because there are 72 possible combinations of conversions, only the most appropriate
//          ones will be included here.  (ie.. inches to feet).  All other conversions can be
//          handled using the set or convert member function of each class type (ie.. CentiMeters to Meters).
//
//          LCreal squareFeetToInches(const LCreal v)
//          LCreal squareFeetToYards(const LCreal v)
//          LCreal squareFeetToMeters(const LCreal v)
//          LCreal squareFeetToMiles(const LCreal v)
//          LCreal squareInchesToFeet(const LCreal v)
//          LCreal squareInchesToYards(const LCreal v)
//          LCreal squareInchesToMeters(const LCreal v)
//          LCreal squareInchesToMiles(const LCreal v)
//          LCreal squareYardsToFeet(const LCreal v)
//          LCreal squareYardsToInches(const LCreal v)
//          LCreal squareYardsToMeters(const LCreal v)
//          LCreal squareYardsToMiles(const LCreal v)
//          LCreal squareMilesToFeet(const LCreal v)
//          LCreal squareMilesToInches(const LCreal v)
//          LCreal squareMilesToYards(const LCreal v)
//          LCreal squareMilesToMeters(const LCreal v)
//          LCreal squareMetersToFeet(const LCreal v)
//          LCreal squareMetersToInches(const LCreal v)
//          LCreal squareMetersToYards(const LCreal v)
//          LCreal squareMetersToMiles(const LCreal v)
//          LCreal squareMetersToDecibelSquareMeters(const LCreal v)
//          LCreal decibelSquareMetersToSquareMeters(const LCreal v)
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
//     LCreal convertStatic(const Area& n)
//        Static function to convert the given Area derived instance
//        into the units of a specific Area derived class.
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Areas_H__
#define __Eaagles_Basic_Areas_H__

#include "../Number.h"

namespace Eaagles {
namespace Basic {

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
    Area(const LCreal value);

    void set(const LCreal v) { val = v; }
    void set(const Area& n)  { val = fromArea(n.toArea()); }

    virtual LCreal toArea() const = 0;
    virtual LCreal fromArea(const LCreal a) const = 0;
    LCreal convert(const Area& n) { return fromArea(n.toArea()); }

    // Conversions between Areas
    static LCreal squareFeetToInches(const LCreal v)   { return (v * SFT2SM) * SM2SIN; }
    static LCreal squareFeetToYards(const LCreal v)    { return (v * SFT2SM) * SM2SYD; }
    static LCreal squareFeetToMeters(const LCreal v)   { return v * SFT2SM; }
    static LCreal squareFeetToMiles(const LCreal v)    { return (v * SFT2SM) * SM2SMI; }
    static LCreal squareInchesToFeet(const LCreal v)   { return (v * SIN2SM) * SM2SFT; }
    static LCreal squareInchesToYards(const LCreal v)  { return (v * SIN2SM) * SM2SYD; }
    static LCreal squareInchesToMeters(const LCreal v) { return v * SIN2SM; }
    static LCreal squareInchesToMiles(const LCreal v)  { return (v * SIN2SM) * SM2SMI; }
    static LCreal squareYardsToFeet(const LCreal v)    { return (v * SYD2SM) * SM2SFT; }
    static LCreal squareYardsToInches(const LCreal v)  { return (v * SYD2SM) * SM2SIN; }
    static LCreal squareYardsToMeters(const LCreal v)  { return v * SYD2SM; }
    static LCreal squareYardsToMiles(const LCreal v)   { return (v * SYD2SM) * SM2SMI; }
    static LCreal squareMilesToFeet(const LCreal v)    { return (v * SMI2SM) * SM2SFT; }
    static LCreal squareMilesToInches(const LCreal v)  { return (v * SMI2SM) * SM2SIN; }
    static LCreal squareMilesToYards(const LCreal v)   { return (v * SMI2SM) * SM2SYD; }
    static LCreal squareMilesToMeters(const LCreal v)  { return v * SMI2SM; }
    static LCreal squareMetersToFeet(const LCreal v)   { return v * SM2SFT; }
    static LCreal squareMetersToInches(const LCreal v) { return v * SM2SIN; }
    static LCreal squareMetersToYards(const LCreal v)  { return v * SM2SYD; }
    static LCreal squareMetersToMiles(const LCreal v)  { return v * SM2SMI; }
    static LCreal squareMetersToDecibelSquareMeters(const LCreal v) { return 10.0f * lcLog10(v); }
    static LCreal decibelSquareMetersToSquareMeters(const LCreal v) { return lcPow(static_cast<LCreal>(10.0), static_cast<LCreal>(v/10.0)); }

    // Conversion constants
    static const LCreal SM2SFT;
    static const LCreal SFT2SM;
    static const LCreal SM2SYD;
    static const LCreal SYD2SM;
    static const LCreal SM2SMI;
    static const LCreal SMI2SM;
    static const LCreal SM2SIN;
    static const LCreal SIN2SM;
    static const LCreal SM2SCM;
    static const LCreal SCM2SM;
    static const LCreal SM2SMM;
    static const LCreal SMM2SM;
    static const LCreal SM2SKM;
    static const LCreal SKM2SM;
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
    SquareMeters(const LCreal value);
    SquareMeters(const Area& value);

    static LCreal convertStatic(const Area& n)      { return n.toArea(); }
    // Area interface
    LCreal toArea() const override                  { return static_cast<LCreal>(val); }
    LCreal fromArea(const LCreal a) const override  { return a; }
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
    SquareFeet(const LCreal value);
    SquareFeet(const Area& value);

    static LCreal convertStatic(const Area& n)      { return n.toArea() * SM2SFT; }
    // Area interface
    LCreal toArea() const override                  { return static_cast<LCreal>(val * SFT2SM); }
    LCreal fromArea(const LCreal a) const override  { return a * SM2SFT; }
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
    SquareInches(const LCreal value);
    SquareInches(const Area& value);

    static LCreal convertStatic(const Area& n)      { return n.toArea() * SM2SIN; }
    // Area interface
    LCreal toArea() const override                  { return static_cast<LCreal>(val * SIN2SM); }
    LCreal fromArea(const LCreal a) const override  { return a * SM2SIN; }
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
    SquareYards(const LCreal value);
    SquareYards(const Area& value);

    static LCreal convertStatic(const Area& n)      { return n.toArea() * SM2SYD; }
    // Area interface
    LCreal toArea() const override                  { return static_cast<LCreal>(val * SYD2SM); }
    LCreal fromArea(const LCreal a) const override  { return a * SM2SYD; }
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
    SquareMiles(const LCreal value);
    SquareMiles(const Area& value);

    static LCreal convertStatic(const Area& n)      { return n.toArea() * SM2SMI; }
    // Area interface
    LCreal toArea() const override                  { return static_cast<LCreal>(val * SMI2SM); }
    LCreal fromArea(const LCreal a) const override  { return a * SM2SMI; }
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
    SquareCentiMeters(const LCreal value);
    SquareCentiMeters(const Area& value);

    static LCreal convertStatic(const Area& n)      { return n.toArea() * SM2SCM; }
    // Area interface
    LCreal toArea() const override                  { return static_cast<LCreal>(val * SCM2SM); }
    LCreal fromArea(const LCreal a) const override  { return a * SM2SCM; }
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
    SquareMilliMeters(const LCreal value);
    SquareMilliMeters(const Area& value);

    static LCreal convertStatic(const Area& n)      { return n.toArea() * SM2SMM; }
    // Area interface
    LCreal toArea() const override                  { return static_cast<LCreal>(val * SMM2SM); }
    LCreal fromArea(const LCreal a) const override  { return a * SM2SMM; }
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
    SquareKiloMeters(const LCreal value);
    SquareKiloMeters(const Area& value);

    static LCreal convertStatic(const Area& n)      { return n.toArea() * SM2SKM; }
    // Area interface
    LCreal toArea() const override                  { return static_cast<LCreal>(val * SKM2SM); }
    LCreal fromArea(const LCreal a) const override  { return a * SM2SKM; }
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
    DecibelSquareMeters(const LCreal value);
    DecibelSquareMeters(const Area& value);

    static LCreal convertStatic(const Area& n)     { return 10.0f * lcLog10( n.toArea() ); }
    // Area interface
    LCreal toArea() const override                 { return lcPow(static_cast<LCreal>(10.0), static_cast<LCreal>(val/10.0)); }
    LCreal fromArea(const LCreal a) const override { return 10.0f * lcLog10(a); }

};

} // End Basic namespace
} // End Eaagles namespace

#endif
