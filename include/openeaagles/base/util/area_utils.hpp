
#ifndef __oe_base_util_area_utils_H__
#define __oe_base_util_area_utils_H__

#include <cmath>

namespace oe {
namespace base {
namespace area {

// conversion constants
const double SM2SFT = 10.76391f;           // Sq. Meters      => Sq. Feet
const double SFT2SM = (1.0f / SM2SFT);     // Sq. Feet        => Sq. Meters
const double SM2SYD = 1.19599f;            // Sq. Meters      => Sq. Yards
const double SYD2SM = (1.0f / SM2SYD);     // Sq. Yards       => Sq. Meters
const double SM2SMI = 0.00000038610216f;   // Sq. Meters      => Sq. Miles
const double SMI2SM = (1.0f / SM2SMI);     // Sq. Miles       => Sq. Meters
const double SM2SIN = 1550.0030399f;       // Sq. Meters      => Sq. Inches
const double SIN2SM = (1.0f / SM2SIN);     // Sq. Inches      => Sq. Meters
const double SM2SCM = 10000.0f;            // Sq. Meters      => Sq. CentiMeters
const double SCM2SM = (1.0f / SM2SCM);     // Sq. CentiMeters => Sq. Meters
const double SM2SMM = 1000000.0f;          // Sq. Meters      => Sq. MilliMeters
const double SMM2SM = (1.0f / SM2SMM);     // Sq. MilliMeters => Sq. Meters
const double SM2SKM = 0.000001f;           // Sq. Meters      => Sq. KiloMeters
const double SKM2SM = (1.0f / SM2SKM);     // Sq. KiloMeters  => Sq. Meters

// conversions between Areas
inline double squareFeetToInches(const double v)   { return (v * SFT2SM) * SM2SIN; }
inline double squareFeetToYards(const double v)    { return (v * SFT2SM) * SM2SYD; }
inline double squareFeetToMeters(const double v)   { return v * SFT2SM; }
inline double squareFeetToMiles(const double v)    { return (v * SFT2SM) * SM2SMI; }
inline double squareInchesToFeet(const double v)   { return (v * SIN2SM) * SM2SFT; }
inline double squareInchesToYards(const double v)  { return (v * SIN2SM) * SM2SYD; }
inline double squareInchesToMeters(const double v) { return v * SIN2SM; }
inline double squareInchesToMiles(const double v)  { return (v * SIN2SM) * SM2SMI; }
inline double squareYardsToFeet(const double v)    { return (v * SYD2SM) * SM2SFT; }
inline double squareYardsToInches(const double v)  { return (v * SYD2SM) * SM2SIN; }
inline double squareYardsToMeters(const double v)  { return v * SYD2SM; }
inline double squareYardsToMiles(const double v)   { return (v * SYD2SM) * SM2SMI; }
inline double squareMilesToFeet(const double v)    { return (v * SMI2SM) * SM2SFT; }
inline double squareMilesToInches(const double v)  { return (v * SMI2SM) * SM2SIN; }
inline double squareMilesToYards(const double v)   { return (v * SMI2SM) * SM2SYD; }
inline double squareMilesToMeters(const double v)  { return v * SMI2SM; }
inline double squareMetersToFeet(const double v)   { return v * SM2SFT; }
inline double squareMetersToInches(const double v) { return v * SM2SIN; }
inline double squareMetersToYards(const double v)  { return v * SM2SYD; }
inline double squareMetersToMiles(const double v)  { return v * SM2SMI; }
inline double squareMetersToDecibelSquareMeters(const double v) { return 10.0f * std::log10(v); }
inline double decibelSquareMetersToSquareMeters(const double v) { return std::pow(static_cast<double>(10.0), static_cast<double>(v/10.0)); }




}
}
}

#endif
