//------------------------------------------------------------------------------
// Class: Emission
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Emission_H__
#define __oe_simulation_Emission_H__

#include "openeaagles/simulation/SensorMsg.h"
#include "openeaagles/simulation/Antenna.h"

namespace oe {
namespace simulation {
   class RfSystem;

//------------------------------------------------------------------------------
// Class: Emission
//
// Description: Generic RF Emission class -- Used to pass RF emission data
//              between antennas and players.
// Factory name: Emission
//------------------------------------------------------------------------------
class Emission : public SensorMsg
{
   DECLARE_SUBCLASS(Emission,SensorMsg)

public:
   // ECM enumerations (can be expanded by derived classes)
   enum {
      ECM_OFF,
      ECM_NOISE,
      ECM_LAST       // Hook for subclasses to expand
   };

public:
   Emission();

   // Frequency (hz)
   LCreal getFrequency() const { return freq; }

   // Wavelength (meters)
   LCreal getWavelength() const { return lambda; }

   // Sets the frequency (hz) [and wavelength (meters)]
   void setFrequency(const LCreal f) {
      if (f > 0.0) {
         freq = f;
         lambda = static_cast<float>(LIGHTSPEED)/f;
      }
   }

   // Sets the wavelength (meters) [and Frequency (hz)]
   void setWavelength(const LCreal l) {
      if (l > 0.0) {
         lambda = l;
         freq = static_cast<float>(LIGHTSPEED)/l;
      }
   }

   // Bandwidth (hz)
   LCreal getBandwidth() const { return bw; }

   // Sets the beam width (hz)
   void setBandwidth(const LCreal f) { bw = f; }

   // Pulse width (sec)
   LCreal getPulseWidth() const { return pw; }

   // Sets the pulse width (sec)
   void setPulseWidth(const LCreal f) { pw = f; }

   // Pulse Repetition Freq (hz)
   LCreal getPRF() const { return prf; }

   // Sets the PRF (hz)
   void setPRF(const LCreal f) { prf = f; }

   // Number of pulses in this emission packet
   //  (e.g., at 50HZ and a PRF of 2000hz, there are 40 pulses per emission instance)
   unsigned int getPulses() const  { return pulses; }

   // Sets the number of pulses in this emission packet
   void setPulses(const unsigned int i) { pulses = i; }

   // Effective Radiated Power (ERP) (watts)
   LCreal getPower() const { return power; }

   // Sets the Effective Radiated Power (ERP) (watts)
   void setPower(const LCreal f) { power = f; }

   // (one way) Range loss (no units)
   LCreal getRangeLoss() const { return lossRng; }

   // Antenna Polarization (see the Antenna class)
   Antenna::Polarization getPolarization() const { return polar; }

   // Sets the antenna Polarization (see the Antenna class)
   void setPolarization(const Antenna::Polarization p) { polar = p; }

   // Effective antenna gain (no units)
   LCreal getGain() const          { return gain; }

   // Sets the effective antenna gain (no units)
   void setGain(const LCreal g) { gain = g; }

   // Doppler shift (hz)
   // Source: S.A. Hovanessian, Radar Detection & Tracking Systems (Equation 1-3)
   LCreal getDopplerShift() const  {
      if (lambda > 0.0f) return (-2.0f * getRangeRate()/lambda);
      else return 0.0f;
   }

   // Atmospheric attenuation loss (no units)
   LCreal getAtmosphericAttenuationLoss() const { return lossAtmos; }

   // Sets the atmospheric attenuation loss (no units)
   void setAtmosphericAttenuationLoss(const LCreal loss) { lossAtmos = loss; }

   // Returns the transmit loss (no units)
   LCreal getTransmitLoss() const   { return lossXmit; }

   // Sets the transmit loss (no units)
   void setTransmitLoss(const LCreal v) { lossXmit = v; }

   // Target's Radar Cross Section (RCS) as a function of this emission data (m^2)
   LCreal getRCS() const  { return rcs; }

   // Sets the Radar Cross Section (RCS)
   void setRCS(const LCreal f) { rcs = f; }

   // Pointer to the R/F system that transmitted this emission
   RfSystem* getTransmitter()   { return transmitter; }

   // Pointer to the R/F system that transmitted this emission
   const RfSystem* getTransmitter() const { return transmitter; }

   // Sets the pointer to the R/F system that transmitted this emission
   void setTransmitter(RfSystem* const t);

   // ECM emission flag (this is an ECM emission)
   bool isECM() const { return (ecmFlag!=ECM_OFF); }
   virtual bool isECMType(const unsigned int ecm) const { return (ecmFlag==ecm); }

   // Sets the ECM emission flag
   virtual void setECM(const unsigned int b) { ecmFlag = b; }

   void setRange(const LCreal r) override;   // Sets the range to the target (meters) (which we use to set the range loss)
   void clear() override;                    // Clear this emission's data

private:
   LCreal          freq;           // Frequency                        (Hz)
   LCreal          lambda;         // Wavelength                       (meters)
   LCreal          pw;             // Pulse Width                      (Sec)
   LCreal          bw;             // Bandwidth                        (Hz)
   LCreal          prf;            // Pulse Repetition Frequency       (Hz)
   unsigned int    pulses;         // Number of Pulses this packet     (no units)
   LCreal          power;          // Effective Radiated Power (erp)   (Watts)
   LCreal          gain;           // Effective antenna gain           (no units)
   LCreal          lossRng;        // Loss due to range                (m^-2)
   LCreal          lossAtmos;      // Atmospheric Attenuation Loss     (no units)
   LCreal          lossXmit;       // Transmit loss (default: 1.0)     (no units)
   LCreal          rcs;            // Radar Cross Section (RCS)        (m^2)
   Antenna::Polarization polar;    // Antenna polarization             (enum)
   RfSystem*       transmitter;    // The system that transmitted the emission
   unsigned int    ecmFlag;        // ECM enumeration
};

} // End simulation namespace
} // End oe namespace

#endif
