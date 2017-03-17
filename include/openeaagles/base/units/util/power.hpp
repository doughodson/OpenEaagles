
#ifndef __oe_base_util_power_H__
#define __oe_base_util_power_H__

namespace oe {
namespace base {
namespace power {

// conversion constants
const double W2KW = 0.001f;       // Watts => KiloWatts
const double KW2W = (1 / W2KW);   // KiloWatts => Watts
const double W2HP = 1341.0f;      // Watts => Horsepower
const double HP2W = (1 / W2HP);   // Horsepower => Watts
const double W2MW = 1000.0f;      // Watts => MilliWatts
const double MW2W = (1 / W2MW);   // MilliWatts => Watts

// conversions between Powers
inline double kiloWattsToWatts(const double v)       { return v * KW2W; }
inline double kiloWattsToHorsepower(const double v)  { return (v * KW2W) * W2HP; }
inline double kiloWattsToMilliWatts( const double v) { return (v * KW2W) * W2MW; }
inline double wattsToKiloWatts(const double v)       { return v * W2KW; }
inline double wattsToHorsepower(const double v)      { return v * W2HP; }
inline double wattsToMilliWatts(const double v)      { return v * W2MW; }
inline double horsepowerToWatts(const double v)      { return v * HP2W; }
inline double horsepowerToKiloWatts(const double v)  { return (v * HP2W) * W2KW; }
inline double horsepowerToMilliWatts(const double v) { return (v * HP2W) * W2MW; }
inline double milliWattsToWatts(const double v)      { return v * MW2W; }
inline double milliWattsToKiloWatts(const double v)  { return (v * MW2W) * W2KW; }
inline double milliWattsToHorsepower(const double v) { return (v * MW2W) * W2HP; }

}
}
}

#endif
