
#ifndef __oe_base_energy_utils_H__
#define __oe_base_energy_utils_H__

namespace oe {
namespace base {
namespace energy {

// conversion constants
const double J2KWH = 0.0000002777777778f;  // Joules => KiloWattHours
const double KWH2J = (1 / J2KWH);          // KiloWattHours => Joules
const double J2BTU = 0.0009478f;           // Joules => BTUs
const double BTU2J = (1 / J2BTU);          // BTUs => Joules
const double J2FP = 0.7376f;               // Joules => FootPounds
const double FP2J = (1 / J2FP);            // FootPounds => Joules
const double J2C = 0.2388888889f;          // Joules => Calories
const double C2J = (1 / J2C);              // Calories => Joules

// conversions between Energy
inline double btusToFootPounds(const double v)          { return (v * BTU2J) * J2FP;  }
inline double btusToJoules(const double v)              { return v * BTU2J;           }
inline double btusToKiloWattHours(const double v)       { return (v * BTU2J) * J2KWH; }
inline double btusToCalories(const double v)            { return (v * BTU2J) * J2C;   }
inline double footPoundsToBTUs(const double v)          { return (v * FP2J) * J2BTU;  }
inline double footPoundsToJoules(const double v)        { return v * FP2J;            }
inline double footPoundsToKiloWattHours(const double v) { return (v * FP2J) * J2KWH;  }
inline double footPoundsToCalories(const double v)      { return (v * FP2J) * J2C;    }
inline double joulesToBTUs(const double v)              { return v * J2BTU;           }
inline double joulesToFootPounds(const double v)        { return v * J2FP;            }
inline double joulesToKiloWattHours(const double v)     { return v * J2KWH;           }
inline double joulesToCalories(const double v)          { return v * J2C;             }
inline double kiloWattHoursToBTUs(const double v)       { return (v * KWH2J) * J2BTU; }
inline double kiloWattHoursToFootPounds(const double v) { return (v * KWH2J) * J2FP;  }
inline double kiloWattHoursToJoules(const double v)     { return v * KWH2J;           }
inline double kiloWattHoursToCalories(const double v)   { return (v * KWH2J) * J2C;   }
inline double caloriesToBTUs(const double v)            { return (v * C2J) * J2BTU;   }
inline double caloriesToFootPounds(const double v)      { return (v * C2J) * J2FP;    }
inline double caloriesToJoules(const double v)          { return v * C2J;             }
inline double caloriesToKiloWattHours(const double v)   { return (v * C2J) * J2KWH;   }

}
}
}

#endif
