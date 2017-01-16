
#ifndef __oe_base_distance_utils_H__
#define __oe_base_distance_utils_H__

namespace oe {
namespace base {
namespace distance {

// conversion constants
const double FT2M =  0.30480f;        // Feet => Meters
const double M2FT = (1 / FT2M);       // Meters => Feet
const double IN2M =  0.02540f;        // Inches => Meters
const double M2IN =  (1 / IN2M);      // Meters => Inches

const double NM2M = 1852.0f;          // Nautical Miles => Meters
const double M2NM = (1 / NM2M);       // Meters => Nautical Miles
const double NM2FT = (NM2M * M2FT);   // Nautical Miles => Feet
const double FT2NM = 1/NM2FT;         // Feet => Nautical Miles

const double SM2M = 1609.344f;        // Statue Miles => Meters
const double M2SM = (1 / SM2M);       // Meters => Statue Miles
const double SM2FT = 5280.0f;         // Statue Miles => Feet
const double FT2SM = (1 / SM2FT);     // Feet => Statue Miles

const double KM2M = 1000.0f;          // Kilometers => Meters
const double M2KM = (1 / KM2M);       // Meters => Kilometers
const double CM2M = 0.01f;            // Centimeters => Meters
const double M2CM = (1 / CM2M);       // Meters => Centimeters
const double UM2M = 0.000001f;        // Micrometer (Micron) => Meters
const double M2UM = (1 / UM2M);       // Meters => Micrometer (Micron)

// conversions between distances
inline double MetersToMicroMeters(const double v)    { return v * M2UM; }
inline double MicroMetersToMeters(const double v)    { return v * UM2M; }
inline double MetersToCentiMeters(const double v)    { return v * M2CM; }
inline double CentiMetersToMeters(const double v)    { return v * CM2M; }
inline double MetersToKiloMeters(const double v)     { return v * M2KM; }
inline double KiloMetersToMeters(const double v)     { return v * KM2M; }
inline double MetersToInches(const double v)         { return v * M2IN; }
inline double InchesToMeters(const double v)         { return v * IN2M; }
inline double MetersToFeet(const double v)           { return v * M2FT; }
inline double FeetToMeters(const double v)           { return v * FT2M; }
inline double KiloMetersToNauticalMiles(const double v)    { return (v * KM2M) * M2NM; }
inline double NauticalMilesToKiloMeters(const double v)    { return (v * NM2M) * M2KM; }
inline double KiloMetersToStatuteMiles(const double v)     { return (v * KM2M) * M2SM; }
inline double StatuteMilesToKiloMeters(const double v)     { return (v * SM2M) * M2KM; }
inline double NauticalMilesToStatuteMiles(const double v)  { return (v * NM2M) * M2SM; }
inline double StatuteMilesToNauticalMiles(const double v)  { return (v * SM2M) * M2NM; }

}
}
}

#endif
