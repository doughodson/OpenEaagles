//------------------------------------------------------------------------------
// Classes:  Force, PoundForces, Newtons, Poundals, KiloNewtons
//
// Base class:  Object -> Number -> Force
//              Object -> Number -> Force -> Newtons
//              Object -> Number -> Force -> KiloNewtons
//              Object -> Number -> Force -> Poundals
//              Object -> Number -> Force -> PoundForces
//
// Description:  Numbers as forces -- Newtons, KiloNewtons, Poundals,
//               PoundForces
//               Base unit for Force derivations are Newtons.
//
//
// Public methods (Defined in Force, and inherited by all derived classes):
//
//     set(const double v)
//        Sets a Force derived instance with an double.
//
//     set(const Force& n)
//        Sets, and converts if necessary, a Force derived instance with
//        another Force derived instance.
//
//     double convert(const Force& n)
//        Converts the value of a Force derived instance into
//        the units of another Force derived instance.
//
//
//     Conversion routines:
//        static double newtonsToKiloNewtons(const double v)     { return v * 0.001; }
//        static double newtonsToPoundForces(const double v)     { return v * 0.2248; }
//        static double newtonsToPoundals(const double v)        { return v * 7.233; }
//        static double kiloNewtonsToNewtons(const double v)     { return v * 1000; }
//        static double kiloNetonsToPoundForces(const double v)  { return v * 224.8; }
//        static double kiloNewtonsToPoundals(const double v)    { return v * 7233; }
//        static double poundForcesToNewtons(const double v)     { return v * 4.448; }
//        static double poundForcesToKiloNewtons(const double v) { return v * 0.004448; }
//        static double poundForcesToPoundals(const double v)    { return v * 32.17; }
//        static double poundalsToNewtons(const double v)        { return v * 0.1383; }
//        static double poundalsToKiloNewtons(const double v)    { return v * 0.0001383; }
//        static double poundalsToPoundForces(const double v)    { return v * 0.03108; }
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Force& n)
//        Sends "( <the Force derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  Newtons, KiloNewtons, PoundForce, Poundals):
//
//     double convertStatic(const Force& n)
//        static function to convert the given Force derived
//        instance into the units of a specific Force derived class.
//
//------------------------------------------------------------------------------
#ifndef __oe_base_Forces_H__
#define __oe_base_Forces_H__

#include "openeaagles/base/Number.h"

namespace oe {
namespace base {

// ----------------------------------------------------------------------------
// Defined Force Conversion Constants:
//
// These constants were obtained (and cross referenced) from the following
// websites, and are assumed accurate as of 2/5/03.
//
// http://labserver.catlin.edu/faculty/merrilla/ics/SpaceTravel.html
// - Catlin Gabel School
// http://www.pitt.edu/~rsup/forceconv.html  - University of Pittsburg
// http://www.unc.edu/~rowlett/units/dictK.html - U. of N.C.(Chapel Hill)
// Source for above (unc) website: Russ Rowlett dictionary - July 28, 2000
// http://www.uoregon.edu/~opp/climbing/forcesinleadfalls.pdf - U. of Oregon
// ----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class:  Force
// Base class:  Object -> Number -> Force
// Description:  Base class for forces.  Defined as a Newton which is
//               equivalent to an instance of Newtons with its value equal
//               to 1.0.
//------------------------------------------------------------------------------
class Force : public Number
{
    DECLARE_SUBCLASS(Force, Number)

public:
    Force();
    Force(const double value);

    void set(const double v) { val = v; }
    void set(const Force& n) { val = fromForce(n.toForce()); }

    virtual double toForce() const = 0;
    virtual double fromForce(const double a) const = 0;
    double convert(const Force& n){ return fromForce(n.toForce()); }

    // Conversions between Forces
    static double newtonsToKiloNewtons(const double v)     { return v * N2KN; }
    static double newtonsToPoundForces(const double v)     { return v * N2PF; }
    static double newtonsToPoundals(const double v)        { return v * N2PD; }
    static double kiloNewtonsToNewtons(const double v)     { return v * KN2N; }
    static double kiloNewtonsToPoundForces(const double v) { return (v * KN2N) * N2PF; }
    static double kiloNewtonsToPoundals(const double v)    { return (v * KN2N) * N2PD; }
    static double poundForcesToNewtons(const double v)     { return v * PF2N; }
    static double poundForcesToKiloNewtons(const double v) { return (v * PF2N) * N2KN; }
    static double poundForcesToPoundals(const double v)    { return (v * PF2N) * N2PD; }
    static double poundalsToNewtons(const double v)        { return v * PD2N; }
    static double poundalsToKiloNewtons(const double v)    { return (v * PD2N) * N2KN; }
    static double poundalsToPoundForces(const double v)    { return (v * PD2N) * N2PF; }

    // Conversion constants
    static const double N2KN;
    static const double KN2N;
    static const double N2PF;
    static const double PF2N;
    static const double N2PD;
    static const double PD2N;
};


inline std::ostream& operator<<(std::ostream& sout, const Force& n)
   { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class:  Newtons
// Base class:  Object -> Number -> Force -> Newtons
// Description:  An instance of Newtons with its value equal to 1.0 is one
//               base unit for forces.
//------------------------------------------------------------------------------
class Newtons : public Force
{
    DECLARE_SUBCLASS(Newtons, Force)

public:
    Newtons();
    Newtons(const double value);
    Newtons(const Force& value);

    static double convertStatic(const Force& n)                { return n.toForce(); }
    virtual double toForce() const override                    { return static_cast<double>(val); }
    virtual double fromForce(const double a) const override    { return a; }
};


//------------------------------------------------------------------------------
// Class:  KiloNewtons
// Base class:  Object -> Number -> Force -> KiloNewtons
// Description:  Newtons * 1000.0
//------------------------------------------------------------------------------
class KiloNewtons : public Force
{
    DECLARE_SUBCLASS(KiloNewtons, Force)

public:
    KiloNewtons();
    KiloNewtons(const double value);
    KiloNewtons(const Force& value);

    static double convertStatic(const Force& n)                { return n.toForce() * N2KN; }
    virtual double toForce() const override                    { return static_cast<double>(val * KN2N); }
    virtual double fromForce(const double a) const override    { return a * N2KN; }
};


//------------------------------------------------------------------------------
// Class:  PoundForces
// Base class:  Object -> Number -> Force -> PoundForces
// Description:  Newtons * 0.224809
//------------------------------------------------------------------------------
class PoundForces : public Force
{
    DECLARE_SUBCLASS(PoundForces, Force)

public:
    PoundForces();
    PoundForces(const double value);
    PoundForces(const Force& value);

    static double convertStatic(const Force& n)                { return n.toForce() * N2PF; }
    virtual double toForce() const override                    { return static_cast<double>(val * PF2N); }
    virtual double fromForce(const double a) const override    { return a * N2PF; }
};


//------------------------------------------------------------------------------
// Class:  Poundals
// Base class:  Object -> Number -> Force -> Poundals
// Description:  Newtons * 7.23301
//------------------------------------------------------------------------------
class Poundals : public Force
{
    DECLARE_SUBCLASS(Poundals, Force)

public:
    Poundals();
    Poundals(const double value);
    Poundals(const Force& value);

    static double convertStatic(const Force& n)                { return n.toForce() * N2PD; }
    virtual double toForce() const override                    { return static_cast<double>(val * PD2N); }
    virtual double fromForce(const double a) const override    { return a * N2PD; }
};

}
}

#endif
