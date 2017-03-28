//------------------------------------------------------------------------------
// Classes:  Force, PoundForces, Newtons, Poundals, KiloNewtons
//
// Base class:  Object -> Number -> Force
//              Object -> Number -> Force -> Newtons
//              Object -> Number -> Force -> KiloNewtons
//              Object -> Number -> Force -> Poundals
//              Object -> Number -> Force -> PoundForces
//------------------------------------------------------------------------------

#ifndef __oe_base_Forces_H__
#define __oe_base_Forces_H__

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/units/force_utils.hpp"
#include <iostream>

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class:  Force
// Description:  Base class for forces.  Defined as a Newton which is
//               equivalent to an instance of Newtons with its value equal
//               to 1.0.
//
// Description:  Numbers as forces -- Newtons, KiloNewtons, Poundals,
//               PoundForces
//               Base unit for Force derivations are Newtons.
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

    static double convertStatic(const Force& n)                { return n.toForce() * force::N2KN; }
    virtual double toForce() const override                    { return static_cast<double>(val * force::KN2N); }
    virtual double fromForce(const double a) const override    { return a * force::N2KN; }
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

    static double convertStatic(const Force& n)                { return n.toForce() * force::N2PF; }
    virtual double toForce() const override                    { return static_cast<double>(val * force::PF2N); }
    virtual double fromForce(const double a) const override    { return a * force::N2PF; }
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

    static double convertStatic(const Force& n)                { return n.toForce() * force::N2PD; }
    virtual double toForce() const override                    { return static_cast<double>(val * force::PD2N); }
    virtual double fromForce(const double a) const override    { return a * force::N2PD; }
};

}
}

#endif
