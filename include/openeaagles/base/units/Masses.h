//------------------------------------------------------------------------------
// Classes:  Mass, Grams, KiloGrams, Slugs
//
// Base class:  Object -> Number -> Mass
//              Object -> Number -> Mass -> Grams
//              Object -> Number -> Mass -> KiloGrams
//              Object -> Number -> Mass -> Slugs
//
//
// Description:  Numbers as masses -- Grams, KiloGrams, Slugs,
//               Base unit for Mass derivations are KiloGrams.
//
//
// Public methods (Defined in Mass, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Mass derived instance with an double.
//
//     set(const Mass& n)
//        Sets, and converts if necessary, an Mass derived instance with
//        another Mass derived instance.
//
//     double convert(const Mass& n)
//        Converts the value of an Mass derived instance into
//        the units of another Mass derived instance.
//
//     Conversion routines:
//        static double gramsToKiloGrams(const double v)  { return v * .001f; }
//        static double gramsToSlugs(const double v)      { return v * .00006852f }
//        static double kiloGramsToGrams(const double v)  { return v * 1000f }
//        static double kiloGramsToSlugs(const double v)  { return v * 0.06854f}
//        static double slugsToGrams(const double v)      { return v * 14590f }
//        static double slugsToKiloGrams(const double v)  { return v * 14.59f }
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Mass& n)
//        Sends "( <the Mass derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  Grams, KiloGrams, Slugs):
//
//     double convertStatic(const Mass& n)
//        Static function to convert the given Mass derived instance
//        into the units of a specific Mass derived class.
//
//------------------------------------------------------------------------------
#ifndef __oe_base_Masses_H__
#define __oe_base_Masses_H__

#include "openeaagles/base/Number.h"

namespace oe {
namespace base {

// ----------------------------------------------------------------------------
// Defined Mass Conversion Constants:
//
// These constants were obtained (and cross referenced) from the following
// websites, and are assumed accurate as of 2/5/03.
//
// http://oncampus.richmond.edu/academics/as/education/projects/webunits/measurement/mass1.html
// - University of Richmond
// http://www.ex.ac.uk/cimt/dictunit/ccmass.htm - University of Exeter
// http://iul.eng.fiu.edu/Teaching/controls/course/summer2001/Appendices.pdf
// - Florida International University
// http://faculty.millikin.edu/~jaskill.nsm.faculty.mu/slug.html - Millikin University
// http://www-cta.ornl.gov/cta/data/tedb22/Edition22_AppendixB.pdf
// - Source:  Transportation Energy Data Book: Edition 22 - 2002
// ----------------------------------------------------------------------------
// Since all units are converted to or from KiloGrams, only those constants
// will be defined.
/////////////////////////////////////////////

//------------------------------------------------------------------------------
// Class:  Mass
// Base class:  Object -> Number -> Mass
// Description:  Base class for Masses.  Defined as a KiloGram which is
//               equivalent to an instance of KiloGrams with its value equal
//               to 1.0.
//------------------------------------------------------------------------------
class Mass : public Number
{
    DECLARE_SUBCLASS(Mass, Number)

public:
    Mass();
    Mass(const double value);

    void set(const double v) { val = v; }
    void set(const Mass& n)  { val = fromMass(n.toMass()); }

    //this goes to another mass (kilograms)
    virtual double toMass() const = 0;
    //this is coming from another mass (kilograms)
    virtual double fromMass(const double a) const = 0;
    double convert(const Mass& n) const { return fromMass(n.toMass()); }

    // Conversions between Masss
    static double gramsToKiloGrams(const double v) { return v * G2KG; }
    static double gramsToSlugs(const double v)     { return (v * G2KG) * KG2SL; }
    static double kiloGramsToGrams(const double v) { return v * KG2G; }
    static double kiloGramsToSlugs(const double v) { return v * KG2SL;}
    static double slugsToGrams(const double v)     { return (v * SL2KG) * KG2G; }
    static double slugsToKiloGrams(const double v) { return v * SL2KG; }

    // Conversion constant
    static const double KG2G;    // Kilograms -> Grams
    static const double G2KG;    // Grams -> Kilograms
    static const double KG2SL;   // Kilograms -> Slugs
    static const double SL2KG;   // Slugs -> Kilograms
    static const double KG2PM;   // Kilograms -> PoundsMass
    static const double PM2KG;   // PoundsMass -> Kilograms
};


inline std::ostream& operator<<(std::ostream& sout, const Mass& n)
   { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class:  KiloGrams
// Base class:  Object -> Number -> Mass -> KiloGrams
// Description:  An instance of KiloGrams with its value equal to 1.0 is one
//               base unit for mass.
//------------------------------------------------------------------------------
class KiloGrams : public Mass
{
    DECLARE_SUBCLASS(KiloGrams, Mass)

public:
    KiloGrams();
    KiloGrams(const double value);
    KiloGrams(const Mass& value);

    static double convertStatic(const Mass &n)       { return n.toMass(); }
    double toMass() const override                   { return static_cast<double>(val); }
    double fromMass(const double a) const override   { return a; }
};


//------------------------------------------------------------------------------
// Class:  Grams
// Base class:  Object -> Number -> Mass -> Grams
// Description:  KiloGrams * 1000
//------------------------------------------------------------------------------
class Grams : public Mass
{
    DECLARE_SUBCLASS(Grams, Mass)

public:
    Grams();
    Grams(const double value);
    Grams(const Mass& value);

    static double convertStatic(const Mass &n)       { return n.toMass() * KG2G; }
    double toMass() const override                   { return static_cast<double>(val * G2KG); }
    double fromMass(const double a) const override   { return a * KG2G; }
};


//------------------------------------------------------------------------------
// Class:  Slugs
// Base class:  Object -> Number -> Mass -> Slugs
// Description:  KiloGram * 0.06852176585
//------------------------------------------------------------------------------
class Slugs : public Mass
{
    DECLARE_SUBCLASS(Slugs, Mass)

public:
    Slugs();
    Slugs(const double value);
    Slugs(const Mass& value);

    static double convertStatic(const Mass &n)      { return n.toMass() * KG2SL; }
    double toMass() const override                   { return static_cast<double>(val * SL2KG); }
    double fromMass(const double a) const override   { return a * KG2SL; }
};

} // End base namespace
} // End oe namespace

#endif
