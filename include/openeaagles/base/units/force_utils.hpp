
#ifndef __oe_base_force_utils_H__
#define __oe_base_force_utils_H__

namespace oe {
namespace base {
namespace force {

// conversion constants
const double N2KN = 0.001f;       // Newtons => KiloNewtons
const double KN2N = (1 / N2KN);   // KiloNewtons => Newtons
const double N2PF = 0.224809f;    // Newtons => PoundForces
const double PF2N = (1 / N2PF);   // PoundForces => Newtons
const double N2PD = 7.23301f;     // Newtons => Poundals
const double PD2N = (1 / N2PD);   // Poundals => Newtons

// conversions between Forces
inline double newtonsToKiloNewtons(const double v)     { return v * N2KN; }
inline double newtonsToPoundForces(const double v)     { return v * N2PF; }
inline double newtonsToPoundals(const double v)        { return v * N2PD; }
inline double kiloNewtonsToNewtons(const double v)     { return v * KN2N; }
inline double kiloNewtonsToPoundForces(const double v) { return (v * KN2N) * N2PF; }
inline double kiloNewtonsToPoundals(const double v)    { return (v * KN2N) * N2PD; }
inline double poundForcesToNewtons(const double v)     { return v * PF2N; }
inline double poundForcesToKiloNewtons(const double v) { return (v * PF2N) * N2KN; }
inline double poundForcesToPoundals(const double v)    { return (v * PF2N) * N2PD; }
inline double poundalsToNewtons(const double v)        { return v * PD2N; }
inline double poundalsToKiloNewtons(const double v)    { return (v * PD2N) * N2KN; }
inline double poundalsToPoundForces(const double v)    { return (v * PD2N) * N2PF; }

}
}
}

#endif
