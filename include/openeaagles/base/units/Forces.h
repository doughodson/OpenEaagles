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
//     set(const LCreal v)
//        Sets a Force derived instance with an LCreal.
//
//     set(const Force& n)
//        Sets, and converts if necessary, a Force derived instance with
//        another Force derived instance.
//
//     LCreal convert(const Force& n)
//        Converts the value of a Force derived instance into
//        the units of another Force derived instance.
//
//
//     Conversion routines:
//        static LCreal newtonsToKiloNewtons(const LCreal v)     { return v * 0.001; }
//        static LCreal newtonsToPoundForces(const LCreal v)     { return v * 0.2248; }
//        static LCreal newtonsToPoundals(const LCreal v)        { return v * 7.233; }
//        static LCreal kiloNewtonsToNewtons(const LCreal v)     { return v * 1000; }
//        static LCreal kiloNetonsToPoundForces(const LCreal v)  { return v * 224.8; }
//        static LCreal kiloNewtonsToPoundals(const LCreal v)    { return v * 7233; }
//        static LCreal poundForcesToNewtons(const LCreal v)     { return v * 4.448; }
//        static LCreal poundForcesToKiloNewtons(const LCreal v) { return v * 0.004448; }
//        static LCreal poundForcesToPoundals(const LCreal v)    { return v * 32.17; }
//        static LCreal poundalsToNewtons(const LCreal v)        { return v * 0.1383; }
//        static LCreal poundalsToKiloNewtons(const LCreal v)    { return v * 0.0001383; }
//        static LCreal poundalsToPoundForces(const LCreal v)    { return v * 0.03108; }
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Force& n)
//        Sends "( <the Force derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  Newtons, KiloNewtons, PoundForce, Poundals):
//
//     LCreal convertStatic(const Force& n)
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
    Force(const LCreal value);

    void set(const LCreal v) { val = v; }
    void set(const Force& n) { val = fromForce(n.toForce()); }

    virtual LCreal toForce() const = 0;
    virtual LCreal fromForce(const LCreal a) const = 0;
    LCreal convert(const Force& n){ return fromForce(n.toForce()); }

    // Conversions between Forces
    static LCreal newtonsToKiloNewtons(const LCreal v)     { return v * N2KN; }
    static LCreal newtonsToPoundForces(const LCreal v)     { return v * N2PF; }
    static LCreal newtonsToPoundals(const LCreal v)        { return v * N2PD; }
    static LCreal kiloNewtonsToNewtons(const LCreal v)     { return v * KN2N; }
    static LCreal kiloNewtonsToPoundForces(const LCreal v) { return (v * KN2N) * N2PF; }
    static LCreal kiloNewtonsToPoundals(const LCreal v)    { return (v * KN2N) * N2PD; }
    static LCreal poundForcesToNewtons(const LCreal v)     { return v * PF2N; }
    static LCreal poundForcesToKiloNewtons(const LCreal v) { return (v * PF2N) * N2KN; }
    static LCreal poundForcesToPoundals(const LCreal v)    { return (v * PF2N) * N2PD; }
    static LCreal poundalsToNewtons(const LCreal v)        { return v * PD2N; }
    static LCreal poundalsToKiloNewtons(const LCreal v)    { return (v * PD2N) * N2KN; }
    static LCreal poundalsToPoundForces(const LCreal v)    { return (v * PD2N) * N2PF; }

    // Conversion constants
    static const LCreal N2KN;
    static const LCreal KN2N;
    static const LCreal N2PF;
    static const LCreal PF2N;
    static const LCreal N2PD;
    static const LCreal PD2N;
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
    Newtons(const LCreal value);
    Newtons(const Force& value);

    static LCreal convertStatic(const Force& n)        { return n.toForce(); }
    LCreal toForce() const override                    { return static_cast<LCreal>(val); }
    LCreal fromForce(const LCreal a) const override    { return a; }
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
    KiloNewtons(const LCreal value);
    KiloNewtons(const Force& value);

    static LCreal convertStatic(const Force& n)        { return n.toForce() * N2KN; }
    LCreal toForce() const override                    { return static_cast<LCreal>(val * KN2N); }
    LCreal fromForce(const LCreal a) const override    { return a * N2KN; }
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
    PoundForces(const LCreal value);
    PoundForces(const Force& value);

    static LCreal convertStatic(const Force& n)        { return n.toForce() * N2PF; }
    LCreal toForce() const override                    { return static_cast<LCreal>(val * PF2N); }
    LCreal fromForce(const LCreal a) const override    { return a * N2PF; }
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
    Poundals(const LCreal value);
    Poundals(const Force& value);

    static LCreal convertStatic(const Force& n)        { return n.toForce() * N2PD; }
    LCreal toForce() const override                    { return static_cast<LCreal>(val * PD2N); }
    LCreal fromForce(const LCreal a) const override    { return a * N2PD; }
};

} // End base namespace
} // End oe namespace

#endif
