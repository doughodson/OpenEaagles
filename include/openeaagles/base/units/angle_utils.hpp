
#ifndef __oe_base_angle_utils_H__
#define __oe_base_angle_utils_H__

#include "openeaagles/base/util/constants.hpp"

#include <cmath>

namespace oe {
namespace base {
namespace angle {

// conversion constants:
const double D2SC  = 0.0055555555555556;  // Degrees => Semicircles
const double SC2D  = 180.0;               // Semicircles => Degrees
const double R2SC  = 0.3183098861837906;  // Radians => Semicircles
const double SC2R  = PI;                  // Semicircles => Radians
const double R2DCC = (180.0 / PI);        // Radians => Degrees
const double D2RCC = (PI / 180.0);        // Degrees => Radians

// conversions between angles
inline double degreesToRadians(const double v)     { return (v * D2SC) * SC2R; }
inline double degreesToSemicircles(const double v) { return v * D2SC; }
inline double radiansToDegrees(const double v)     { return (R2SC * v) * SC2D; }
inline double radiansToSemicircles(const double v) { return v * R2SC; }
inline double semicirclesToRadians(const double v) { return v * SC2R; }
inline double semicirclesToDegrees(const double v) { return v * SC2D; }

//
// Angle End-Point Check (keeps angles within the range: -180 <= x <= 180
//

// aepcfDeg -- angle end point check (float-degrees)
inline float aepcfDeg(float x)
{
   float y(0.0);
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
inline double aepcdDeg(double x)
{
   double y(0.0);
   if (x < -180.0 || x > 180.0) {
      y = std::fmod(x, 360.0);
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
inline float aepcfRad(float x)
{
   float y(0.0);
   if (x < -static_cast<float>(PI) || x > static_cast<float>(PI)) {
      y = fmodf(x, (2.0f * static_cast<float>(PI)));
      if (y >  static_cast<float>(PI)) y -= (2.0f * static_cast<float>(PI));
      if (y < -static_cast<float>(PI)) y += (2.0f * static_cast<float>(PI));
      return(y);
   }
   else if (x == -static_cast<float>(PI))
      return(static_cast<float>(PI));
   else
      return(x);
}

// aepcdRad -- angle end point check (double-radians)
inline double aepcdRad(double x)
{
   double y(0.0);
   if (x < -PI || x > PI) {
      y = std::fmod(x, (2.0 * PI));
      if (y >  PI) y -= (2.0 * PI);
      if (y < -PI) y += (2.0 * PI);
      return(y);
   }
   else if (x == -PI)
      return(PI);
   else
      return(x);
}

}
}
}

#endif
