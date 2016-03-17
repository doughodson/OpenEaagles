
#ifndef __oe_base_util_math_utils_H__
#define __oe_base_util_math_utils_H__

//------------------------------------------------------------------------------
// General purpose math functions
//------------------------------------------------------------------------------

#include <cmath>

namespace oe {
namespace base {

// nint -- nearest integer
inline int nint(const double x)
{
   // int(-0.5f) equals 0 or -1, and int(-1000.5f) equals -1000 or -1001,
   // depending on platform.  So, make a negative valued 'x' positive,
   // add 0.5f, then negate the truncated int() cast.
   if (x < 0.0) return (-(static_cast<int>((-x) + 0.5f)));
   else return static_cast<int>(x + 0.5f);
}

// nint -- nearest integer
inline int nintf(const float x)
{
    if (x < 0.0f) return (-(static_cast<int>((-x) + 0.5f)));
    else return static_cast<int>(x + 0.5f);
}

// nint -- nearest integer
inline int nintd(const double x)
{
    if (x < 0.0) return (-(static_cast<int>((-x) + 0.5)));
    else return static_cast<int>(x + 0.5);
}

// ---
// Floating point equality functions
// Returns true if two values differ by less than an (optional) epsilon.
// ---
const double _EPSILON_DOUBLE_ = 1e-12;
const float _EPSILON_FLOAT_ = 1e-7f;

inline bool equal(float d1, float d2, float eps = _EPSILON_FLOAT_)
{
   return (fabsf(d1 - d2) <= eps);
}

inline bool equal(double d1, double d2, double eps = _EPSILON_DOUBLE_)
{
   return (std::fabs(d1 - d2) <= eps);
}

// ---
// sign functions
// ---
inline double sign(const double x)
{
   return ((x >= 0.0) ? +1.0 : -1.0);
}

inline float sign(const float x)
{
   return ((x >= 0.0f) ? +1.0f : -1.0f);
}

inline int sign(const int x)
{
   return ((x >= 0) ? +1 : -1);
}

// Kronecker delta
inline double kron(const unsigned int i, const unsigned int j)
{
   return ((i == j) ? +1.0 : 0.0);
}

// Signum function
inline double sgn(const unsigned int i, const unsigned int j)
{
   return (((i + j) % 2 == 0) ? +1.0 : -1.0);
}

// ---
// Limit functions
// ---

// alim -- limits the value of x to +/-limit.
inline double alim(const double x, const double limit)
{
   if (x > limit) return limit;
   else if (x < -limit) return (-limit);
   else return x;
}

// alimf -- limits the value of x to +/-limit.
inline float alimf(const float x, const float limit)
{
   if (x > limit) return limit;
   else if (x < -limit) return (-limit);
   else return x;
}

// alimd -- limits the value of x to +/-limit.
inline double alimd(const double x, const double limit)
{
   if (x > limit) return limit;
   else if (x < -limit) return (-limit);
   else return x;
}

// lim01 -- limits the value of x between 0.0 and 1.0
inline double lim01(const double x)
{
   if (x > 1.0f) return 1.0f;
   else if (x < 0.0f) return 0.0f;
   else return x;
}

// deadband -- clamps to zero the values of x within +/-limit.
inline double deadband(const double x, const double limit)
{
   if (x >= -limit && x <= limit) return 0.0f;
   else return x;
}

// inRange -- is a number bounded by lo and hi
inline bool inRange(const double x, const double lo, const double hi)
{
    return ((lo <= x) && (x <= hi));
}

// Returns the sine and cosine of an angle (radians)
void sinCos(const double angleRad, double* const sine, double* const cosine);
void sinCos(const float angleRad, float* const sine, float* const cosine);

// Computes the sines of an array of 'n' angles (radians)
void sinArray(const double* const anglesRad, double* const sines, const unsigned int n);
void sinArray(const float*  const anglesRad, float*  const sines, const unsigned int n);

// Computes the cosines of an array of 'n' angles (radians)
void cosArray(const double* const anglesRad, double* const cosines, const unsigned int n);
void cosArray(const float*  const anglesRad, float*  const cosines, const unsigned int n);

// Computes the sines and cosines of an array of 'n' angles (radians)
void sinCosArray(const double* const anglesRad, double* const sines, double* const cosines, const unsigned int n);
void sinCosArray(const float*  const anglesRad, float*  const sines, float*  const cosines, const unsigned int n);

// Computes the arc-cosines of an array of 'n' angles (radians)
void acosArray(const double* const anglesRad, double* const acosines, const unsigned int n);
void acosArray(const float*  const anglesRad, float*  const acosines, const unsigned int n);

// Computes the arc-tangents for two, 'n' element arrays (yValues and xValues)
void atan2Array(const double* const yValues, const double* const xValues, double* const atans, const unsigned int n);
void atan2Array(const float*  const yValues, const float*  const xValues, float*  const atans, const unsigned int n);

// Computes the square roots of 'n' real values
void sqrtArray(const double* const src, double* const dst, const unsigned int n);
void sqrtArray(const float*  const src, float*  const dst, const unsigned int n);

// Raises 10 to the power of 'n' src values and returns the results in 'dst'
void pow10Array(const double* const src, double* const dst, const unsigned int n);
void pow10Array(const float*  const src, float*  const dst, const unsigned int n);

// Multiply an array of reals with a constant
void multArrayConst(const double* const src, const double c, double* const dst, const unsigned int n);
void multArrayConst(const float* const src, const float c, double* const dst, const unsigned int n);

}
}

#endif
