//------------------------------------------------------------------------------
// Common support data and functions
//
// Note: The "Lc" prefix stands for "Link class" in honor of Ed Link, who
//       invented the "Link Blue Box", which was the first flight simulator.
//------------------------------------------------------------------------------
#ifndef _BASIC_SUPPORT_H_
#define _BASIC_SUPPORT_H_

// ---
// Defined the following to use double,
// default is float
// ---
#define LCREAL_DOUBLE
namespace Eaagles {
   #ifdef LCREAL_DOUBLE
      typedef double      LCreal;
   #else
      typedef float       LCreal;
   #endif
}

// ---
// Common header files
// ---
#include "openeaagles/basic/osg/Math"
#include "openeaagles/basic/osg/Vec2"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/osg/Vec4"
#include "openeaagles/basic/osg/Matrix"
#include <typeinfo>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>


// ---
// Include OS/Processor unique stuff
// ---
#if defined(WIN32)
  #include "openeaagles/basic/windows/support0.h"
#else
  #include "openeaagles/basic/linux/support0.h"
#endif


namespace Eaagles {

// ---
// Simple semaphore spinlock and unlock functions: 
//    lcLock(long& s)   -- locks the semaphore w/spinlock wait
//    lcUnlock(long& s) -- frees the semaphore
// where 's' is the semaphore that must be initialized to zero.
// ---
#if defined(WIN32)
  #if defined(__MINGW32__)
    // MinGW lock
    #include "openeaagles/basic/windows/mingwLock.h"
  #else
    // Visual Studio lock
    #include "openeaagles/basic/windows/vsLock.h"
  #endif
#else
  // Linux GCC lock
  #include "openeaagles/basic/linux/lock.h"
#endif


// ---
// Functions for native resolutions
// ---
#ifdef LCREAL_DOUBLE
    // ---
    // When double sse standard C functions
    // ---
    #define lcSqrt      ::sqrt
    #define lcSin       ::sin
    #define lcCos       ::cos
    #define lcTan       ::tan
    #define lcAtan      ::atan
    #define lcAtan2     ::atan2
    #define lcAcos      ::acos
    #define lcAbs       ::fabs
    #define lcFmod      ::fmod
    #define lcPow       ::pow
    #define lcExp       ::exp
    #define lcLog       ::log
    #define lcLog10     ::log10
    #define lcAepcDeg   Basic::Angle::aepcdDeg   /* needs basic/units/Angles.h */
    #define lcAepcRad   Basic::Angle::aepcdRad   /* needs basic/units/Angles.h */
#else
    // Standard C functions
    #define lcSqrt      ::sqrtf
    #define lcSin       ::sinf
    #define lcCos       ::cosf  
    #define lcTan       ::tanf
    #define lcAtan      ::atanf
    #define lcAtan2     ::atan2f
    #define lcAcos      ::acosf
    #define lcAbs       ::fabsf
    #define lcFmod      ::fmodf
    #define lcPow       ::powf
    #define lcExp       ::expf
    #define lcLog       ::logf
    #define lcLog10     ::log10f
    #define lcAepcDeg   Basic::Angle::aepcfDeg   /* needs basic/units/Angles.h */
    #define lcAepcRad   Basic::Angle::aepcfRad   /* needs basic/units/Angles.h */
#endif

// ---
// Make sure PI is defined  (see <math.h>)
// ---
const double PI = 3.1415926535897932384626433832795;
const double TWO_PI = PI + PI;

// ---
// Standard constants ----
//  Source: National Institude of Science & Technology (NIST) web site
//    Speed of light:       299,792,458 m/s
//    Boltzmann constant:   1.3806503E-23 (Joules/Kelvin)
//                          0.0000024E-23 (uncertainty)
// ---
const double LIGHTSPEED = 299792458.0;
const double BOLTZMANN = 1.3806503e-23f;

// One "G" to feet per second per second
//  [This is an estimated average.
//  [One actual "G" is a function of latitude, longitude, and other exotic parameters.]
const LCreal ETHG = 32.16f;

// Convert Barometric Pressure (Inches Mercury to/from Millibars)
// This is an estimate.
const LCreal INCH2MBAR  = (1013.2f / 29.92f);     // LDB -  Inches -> Mbars
const LCreal MBAR2INCH  = (1.0f / INCH2MBAR);     // LDB -  Mbars -> Inches

// Earth gravitational parameter.
// Units: meters cubed per seconds squared.
// 3986004.418E8 meters^3/sec^2.
const double MuM3perS2 = 3986004.418E8;

// ---
// default initial values for various readouts
// ---
const LCreal UNDEFINED_VALUE         = FLT_MIN;
const LCreal INIT_VALUE              = 0.0f;
const int    INIT_INT_VALUE          = 0;
const int    INIT_SELECT_VALUE       = 1;
const LCreal INIT_TIME_VALUE         = 0.0f;
const LCreal INIT_DIRECTION_VALUE    = 0.0f;
const LCreal INIT_LATITUDE_VALUE     = 0.0f;
const LCreal INIT_LONGITUDE_VALUE    = 0.0f;

// ---
// Max/min functions
// ---

// maximim value of v1 or v2
inline LCreal lcMax(const LCreal v1, const LCreal v2)
{
    return (v1 >= v2) ? v1 : v2;
}

// minimum value of v1 or v2
inline LCreal lcMin(const LCreal v1, const LCreal v2)
{
    return (v1 <= v2) ? v1 : v2;
}

// nint -- nearest integer
inline int nint(const LCreal x)
{
    // int(-0.5f) equals 0 or -1, and int(-1000.5f) equals -1000 or -1001,
    // depending on platform.  So, make a negative valued 'x' positive,
    // add 0.5f, then negate the truncated int() cast.
    if (x < 0.0f) return (-(int((-x) + 0.5f)));
    else return int(x + 0.5f);
}

// nint -- nearest integer
inline int nintf(const float x)
{
    if (x < 0.0f) return (-(int((-x) + 0.5f)));
    else return int(x + 0.5f);
}

// nint -- nearest integer
inline int nintd(const double x)
{
    if (x < 0.0) return (-(int((-x) + 0.5)));
    else return int(x + 0.5);
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
	return (fabs(d1 - d2) <= eps);
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
inline LCreal alim(const LCreal x, const LCreal limit)
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
inline LCreal lim01(const LCreal x)
{
    if (x > 1.0f) return 1.0f;
    else if (x < 0.0f) return 0.0f;
    else return x;
}

// deadband -- clamps to zero the values of x within +/-limit.
inline LCreal deadband(const LCreal x, const LCreal limit)
{
    if (x >= -limit && x <= limit) return 0.0f;
    else return x;
}


// inRange -- is a number bounded by lo and hi 
inline bool inRange(const LCreal x, const LCreal lo, const LCreal hi)
{
    return ((lo <= x) && (x <= hi));
}

//------------------------------------------------------------------------------
// General support functions
//------------------------------------------------------------------------------

// Sleep for 'msec' milliseconds
void lcSleep(const unsigned int msec);

// Computer time (seconds)
double getComputerTime();

// UTC time of day (seconds since midnight) and optional date (day/month/year)
double getTimeUtc(unsigned int* const day=0, unsigned int* const month=0, unsigned int* const year=0);

// Get UTC time since midnight (00:00:00), January 1, 1970 
double getTime();

// Get UTC time since midnight (00:00:00), January 1, 1970 
void getTime(
      unsigned long* const sec,  // (OUT) whole seconds 
      unsigned long* const uSec  // (OUT) microseconds seconds
   );

// Convert seconds since midnight (00:00:00), January 1, 1970 to year:month:day:hour:minute:second 
bool convertSec2Ymdhms(
      const unsigned long seconds,  // (IN) whole seconds since midnight (00:00:00), January 1, 1970
      unsigned int* const year,     // (OUT) year YYYY
      unsigned int* const month,    // (OUT) month of the year [ 1 .. 12 ]
      unsigned int* const day,      // (OUT) day of the month   [ 1 .. 31 ]
      unsigned int* const hour,     // (OUT) hours since midnight  [ 0 .. 23 ]
      unsigned int* const min,      // (OUT) minutes after the hour [ 0 .. 59 ]
      unsigned int* const sec       // (OUT) seconds after the minute [ 0 .. 59 ]
   );

// Convert year:month:day:hour:minute:second to seconds since midnight (00:00:00), January 1, 1970
bool convertYmdhms2Sec(
      const unsigned int year,      // (IN) year YYYY
      const unsigned int month,     // (IN) month of the year [ 1 .. 12 ]
      const unsigned int day,       // (IN) day of the month   [ 1 .. 31 ]
      const unsigned int hour,      // (IN) hours since midnight  [ 0 .. 23 ]
      const unsigned int min,       // (IN) minutes after the hour [ 0 .. 59 ]
      const unsigned int sec,       // (IN) seconds after the minute [ 0 .. 59 ]
      unsigned long* const seconds  // (OUT) whole seconds since midnight (00:00:00), January 1, 1970
   );

// Convert seconds since midnight (00:00:00), January 1, 1970 to NTP time stamp
bool convertSec2Ntp(
      const unsigned long seconds,  // (IN) whole seconds since midnight (00:00:00), January 1, 1970
      const unsigned long uSec,     // (IN) microseconds seconds
      unsigned long* const ntpSec,  // (OUT) NTP seconds
      unsigned long* const ntpSecF  // (OUT) NTP fraction of seconds
   );

// Convert NTP time stamp to seconds since midnight (00:00:00), January 1, 1970
bool convertNtp2Sec(
      const unsigned long ntpSec,   // (IN) NTP seconds
      const unsigned long ntpSecF,  // (IN) NTP fraction of seconds
      unsigned long* const seconds, // (OUT) whole seconds since midnight (00:00:00), January 1, 1970
      unsigned long* const uSec     // (OUT) microseconds seconds
   );

// doesFileExist -- returns true if file exists
bool doesFileExist(const char* const fullname);

// Pre-multiply an array of 4D vectors with a 4x4 matrix

void lcPreMultVec4Array(
      const osg::Vec4 orig[],
      const osg::Matrix& matrix,
      osg::Vec4 result[],
      const int n
   );

void preMultVec4Array(
      const osg::Vec4d orig[],
      const osg::Matrixd& matrix,
      osg::Vec4d result[],
      const unsigned int n
   );

void preMultVec4Array(
      const osg::Vec4f orig[],
      const osg::Matrixf& matrix,
      osg::Vec4f result[],
      const unsigned int n
   );

// Pre-multiply an array of 3D vectors with a 4x4 matrix

void lcPreMultVec3Array(
      const osg::Vec3 orig[],
      const osg::Matrix& matrix,
      osg::Vec3 result[],
      const int n
   );

void preMultVec3Array(
      const osg::Vec3d orig[],
      const osg::Matrixd& matrix,
      osg::Vec3d result[],
      const unsigned int n
   );

void preMultVec3Array(
      const osg::Vec3f orig[],
      const osg::Matrixf& matrix,
      osg::Vec3f result[],
      const unsigned int n
   );

// Post-multiply an array of 4D vectors with a 4x4 matrix
void lcPostMultVec4Array(
      const osg::Vec4 orig[],
      const osg::Matrix& matrix,
      osg::Vec4 result[],
      const int n
   );

void postMultVec4Array(
      const osg::Vec4d orig[],
      const osg::Matrixd& matrix,
      osg::Vec4d result[],
      const unsigned int n
   );

void postMultVec4Array(
      const osg::Vec4f orig[],
      const osg::Matrixf& matrix,
      osg::Vec4f result[],
      const unsigned int n
   );

// Post-multiply an array of 3D vectors with a 4x4 matrix
void lcPostMultVec3Array(
      const osg::Vec3 orig[],
      const osg::Matrix& matrix,
      osg::Vec3 result[],
      const int n
   );

void postMultVec3Array(
      const osg::Vec3d orig[],
      const osg::Matrixd& matrix,
      osg::Vec3d result[],
      const unsigned int n
   );

void postMultVec3Array(
      const osg::Vec3f orig[],
      const osg::Matrixf& matrix,
      osg::Vec3f result[],
      const unsigned int n
   );

// Returns the sine and cosine of an angle (radians)
void lcSinCos(const LCreal angleRad, LCreal* const sine, LCreal* const cosine);
void sinCos(const double angleRad, double* const sine, double* const cosine);
void sinCos(const float angleRad, float* const sine, float* const cosine);

// Computes the sines of an array of 'n' angles (radians)
void lcSinArray(const LCreal* const anglesRad, LCreal* const sines, const int n);
void sinArray(const double* const anglesRad, double* const sines, const unsigned int n);
void sinArray(const float*  const anglesRad, float*  const sines, const unsigned int n);

// Computes the cosines of an array of 'n' angles (radians)
void lcCosArray(const LCreal* const anglesRad, LCreal* const cosines, const int n);
void cosArray(const double* const anglesRad, double* const cosines, const unsigned int n);
void cosArray(const float*  const anglesRad, float*  const cosines, const unsigned int n);

// Computes the sines and cosines of an array of 'n' angles (radians)
void lcSinCosArray(const LCreal* const anglesRad, LCreal* const sines, LCreal* const cosines, const int n);
void sinCosArray(const double* const anglesRad, double* const sines, double* const cosines, const unsigned int n);
void sinCosArray(const float*  const anglesRad, float*  const sines, float*  const cosines, const unsigned int n);

// Computes the arc-cosines of an array of 'n' angles (radians)
void lcAcosArray(const LCreal* const anglesRad, LCreal* const acosines, const int n);
void acosArray(const double* const anglesRad, double* const acosines, const unsigned int n);
void acosArray(const float*  const anglesRad, float*  const acosines, const unsigned int n);

// Computes the arc-tangants for two, 'n' element arrays (yValues and xValues)
void lcAtan2Array(const LCreal* const yValues, const LCreal* const xValues, LCreal* const atans, const int n);
void atan2Array(const double* const yValues, const double* const xValues, double* const atans, const unsigned int n);
void atan2Array(const float*  const yValues, const float*  const xValues, float*  const atans, const unsigned int n);

// Computes the square roots of 'n' real values  
void lcSqrtArray(const LCreal* const src, LCreal* const dst, const int n);
void sqrtArray(const double* const src, double* const dst, const unsigned int n);
void sqrtArray(const float*  const src, float*  const dst, const unsigned int n);

// Raises 10 to the power of 'n' src values and returns the results in 'dst'
void lcPow10Array(const LCreal* const src, LCreal* const dst, const int n);
void pow10Array(const double* const src, double* const dst, const unsigned int n);
void pow10Array(const float*  const src, float*  const dst, const unsigned int n);

// Mutiply an array of reals with a constant
void lcMultArrayConst(const LCreal* const src, const LCreal c, LCreal* const dst, const int n);
void multArrayConst(const double* const src, const double c, double* const dst, const unsigned int n);
void multArrayConst(const float* const src, const float c, LCreal* const dst, const unsigned int n);



// Converts the integer number, 'num', to an ascii string.
void lcInteger2Str(const int num, char* const str, int width);

// String copy function: Copies a string from 'strSource' to 'strDest'.  The destination
// string is null terminated.  If the source string is too large for the destination, then
// the source string is truncated at (sizeOfDest-1) characters and null terminated.
bool lcStrcpy(char* const strDest, const size_t sizeOfDest, const char* const strSource);

// String N copy function: Copies up to 'count' characters from 'strSource' to 'strDest'.
// The destination string is null terminated.  If the 'count' characters from the source
// string is too large for the destination, then the source string is truncated at (sizeOfDest-1)
// characters and null terminated.
bool lcStrncpy(char* const strDest, const size_t sizeOfDest, const char* const strSource, const size_t count);

// String cat function: Appends a string from 'strSource' to the end of 'strDest'.  The new
// destination string is null terminated.  If the source string is too large for the
// destination, then the source string is truncated at (sizeOfDest-1) characters and null terminated.
bool lcStrcat(char* const strDest, const size_t sizeOfDest, const char* const strSource);

// Compare two strings ignoring case --
// Returns an integer less than, equal to or greater than zero if 's1' is less
// than, equal to or greater than 's2', respectively.
// (using lower case characters)
int lcStrcasecmp(const char* const s1, const char* const s2);

// Compare the first 'n' characters of two strings ignoring case --
// Returns an integer less than, equal to or greater than zero if 's1' is less
// than, equal to or greater than 's2', respectively.
// (using lower case characters)
int lcStrncasecmp(const char* const s1, const char* const s2, const size_t n);

// returns number of digits in the whole number part (i.e. left of decimal)
// of a floating point number 
unsigned int getDigits(const double x);

} // End Eaagles namespace


#endif // _BASIC_SUPPORT_H_

