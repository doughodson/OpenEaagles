//------------------------------------------------------------------------------
// Classes:  Angle, Degrees, Radians, Semicircles
// Base class:  Object -> Number -> Angle
//              Object -> Number -> Angle -> Degrees
//              Object -> Number -> Angle -> Radians
//              Object -> Number -> Angle -> Semicircles
//
// Description:  Numbers as angles -- Degrees, Radians, Semicircles.
//               Base unit for Angle derivations is a semicircle.
//
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
//     Conversion routines:
//        double degreesToRadians(const double v)
//        double degreesToSemicircles(const double v)
//        double radiansToDegrees(const double v)
//        double radiansToSemicircles(const double v)
//        double semicirclesToRadians(const double v)
//        double semicirclesToDegrees(const double v)
//        Return the appropriate conversion factors.
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
#ifndef __Eaagles_Basic_Angles_H__
#define __Eaagles_Basic_Angles_H__

#include "../Number.h"

namespace Eaagles {
namespace Basic {

// -----------------------------------------------------------------------------
// Defined constants:
// All constants were collected via the internet from 
// http://physics.nist.gov/cuu/Units/outside.html
// National Institute of Technology Standards website.
// 
// Since all Angles are based in Semicircles, all conversions will either 
// convert to or from semicircles.
// -----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class:  Angle
// Base class:  Object -> Number -> Angle
// Description:  Base class for angles.  Defined as a semicircle which is
//               equivalent to an instance of Semicircle with its value equal
//               to 1.0.
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
        

    // Conversions between angles
    static double degreesToRadians(const double v)     { return (v * D2SC) * SC2R; }
    static double degreesToSemicircles(const double v) { return v * D2SC; }
    static double radiansToDegrees(const double v)     { return (R2SC * v) * SC2D; }
    static double radiansToSemicircles(const double v) { return v * R2SC; }
    static double semicirclesToRadians(const double v) { return v * SC2R; }
    static double semicirclesToDegrees(const double v) { return v * SC2D; }

    // Conversion constants:
    static const double D2SC;   // Degrees to Semi-Circle Conversion Constant 
    static const double SC2D;   // Semi-Circle to Degrees Conversion Constant 
    static const double R2SC;   // Radians to Semi-Circle Conversion Constant 
    static const double SC2R;   // Semi-Circle to Radians Conversion Constant 
    static const double R2DCC;  // Radians to Degrees Conversion Constant 
    static const double D2RCC;  // Degrees to Radians Conversion Constant

    // Angle End-Point Check (keeps angles within the range: -180 <= x <= 180
    static float  aepcfDeg(float x);     // (float-degrees)
    static double aepcdDeg(double x);    // (double-degrees)
    static float  aepcfRad(float x);     // (float-radians)
    static double aepcdRad(double x);    // (double-radians)
};


inline std::ostream& operator<<(std::ostream& sout, const Angle& n)
    { sout << "( " << n.getFormName() << " " << n.getReal() << " )"; return sout; }


// aepcfDeg -- angle end point check (float-degrees)
inline float Angle::aepcfDeg(float x)
{
   float y;
   if (x < -180.0f || x > 180.0f) {
      y = fmodf(x, 360.0f);
      if (y >  180.0f) y -= 360.0f;
      if (y < -180.0f) y += 360.0f;
      return(y);
   }
   else if (x == -180.0f)
      return(180.0f);
   else
      return(x);
}

// aepcdDeg -- angle end point check (float-degrees)
inline double Angle::aepcdDeg(double x)
{
   double y;
   if (x < -180.0 || x > 180.0) {
      y = fmod(x, 360.0);
      if (y >  180.0) y -= 360.0;
      if (y < -180.0) y += 360.0;
      return(y);
   }
   else if (x == -180.0)
      return(180.0);
   else
      return(x);
}


// aepcfRad -- angle end point check (float-radians)
inline float Angle::aepcfRad(float x)
{
   float y;
   if (x < -float(PI) || x > float(PI)) {
      y = fmodf(x, (2.0f * float(PI)));
      if (y >  float(PI)) y -= (2.0f * float(PI));
      if (y < -float(PI)) y += (2.0f * float(PI));
      return(y);
   }
   else if (x == -float(PI))
      return(float(PI));
   else
      return(x);
}

// aepcdRad -- angle end point check (double-radians)
inline double Angle::aepcdRad(double x)
{
   double y;
   if (x < -PI || x > PI) {
      y = fmod(x, (2.0 * PI));
      if (y >  PI) y -= (2.0 * PI);
      if (y < -PI) y += (2.0 * PI);
      return(y);
   }
   else if (x == -PI)
      return(PI);
   else
      return(x);
}

//------------------------------------------------------------------------------
// Class:  Degrees
// Base class:  Object -> Number -> Angle -> Degrees
// Description:  Angle * 180.0
//------------------------------------------------------------------------------
class Degrees : public Angle  
{
    DECLARE_SUBCLASS(Degrees, Angle)

public:
    Degrees();
    Degrees(const double value);
    Degrees(const Angle& value);

    static double convertStatic(const Angle& n)     { return n.toAngle() * SC2D; }
    virtual double toAngle() const                  { return (double)(val * D2SC); }
    virtual double fromAngle(const double a) const  { return a * SC2D; }
};


//------------------------------------------------------------------------------
// Class:  Radians
// Base class:  Object -> Number -> Angle -> Radians
// Description:  Angle * PI
//------------------------------------------------------------------------------
class Radians : public Angle  
{
    DECLARE_SUBCLASS(Radians, Angle)

public:
    Radians();
    Radians(const double value);
    Radians(const Angle& value);

    static double convertStatic(const Angle& n)     { return n.toAngle() * SC2R; }
    virtual double toAngle() const                  { return (double)(val * R2SC); }
    virtual double fromAngle(const double a) const  { return a * SC2R; }
};


//------------------------------------------------------------------------------
// Class:  Semicircles
// Base class:  Object -> Number -> Angle -> Semicircles
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

    static double convertStatic(const Angle& n)     { return n.toAngle(); }
    virtual double toAngle() const                  { return (double)(val); }
    virtual double fromAngle(const double a) const  { return a; }
};

} // End Basic namespace
} // End Eaagles namespace

#endif

