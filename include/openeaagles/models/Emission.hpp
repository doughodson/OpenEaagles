
#ifndef __oe_models_Emission_H__
#define __oe_models_Emission_H__

#include "openeaagles/models/SensorMsg.hpp"
#include "openeaagles/models/systems/Antenna.hpp"

namespace oe {
namespace models {
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
   DECLARE_SUBCLASS(Emission, SensorMsg)

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
   double getFrequency() const { return freq; }

   // Wavelength (meters)
   double getWavelength() const { return lambda; }

   // Sets the frequency (hz) [and wavelength (meters)]
   void setFrequency(const double f) {
      if (f > 0.0) {
         freq = f;
         lambda = static_cast<float>(base::LIGHTSPEED)/f;
      }
   }

   // Sets the wavelength (meters) [and Frequency (hz)]
   void setWavelength(const double l) {
      if (l > 0.0) {
         lambda = l;
         freq = static_cast<float>(base::LIGHTSPEED)/l;
      }
   }

   // Bandwidth (hz)
   double getBandwidth() const { return bw; }

   // Sets the beam width (hz)
   void setBandwidth(const double f) { bw = f; }

   // Pulse width (sec)
   double getPulseWidth() const { return pw; }

   // Sets the pulse width (sec)
   void setPulseWidth(const double f) { pw = f; }

   // Pulse Repetition Freq (hz)
   double getPRF() const { return prf; }

   // Sets the PRF (hz)
   void setPRF(const double f) { prf = f; }

   // Number of pulses in this emission packet
   //  (e.g., at 50HZ and a PRF of 2000hz, there are 40 pulses per emission instance)
   unsigned int getPulses() const  { return pulses; }

   // Sets the number of pulses in this emission packet
   void setPulses(const unsigned int i) { pulses = i; }

   // Effective Radiated Power (ERP) (watts)
   double getPower() const { return power; }

   // Sets the Effective Radiated Power (ERP) (watts)
   void setPower(const double f) { power = f; }

   // (one way) Range loss (no units)
   double getRangeLoss() const { return lossRng; }

   // Antenna Polarization (see the Antenna class)
   Antenna::Polarization getPolarization() const { return polar; }

   // Sets the antenna Polarization (see the Antenna class)
   void setPolarization(const Antenna::Polarization p) { polar = p; }

   // Effective antenna gain (no units)
   double getGain() const          { return gain; }

   // Sets the effective antenna gain (no units)
   void setGain(const double g) { gain = g; }

   // Doppler shift (hz)
   // Source: S.A. Hovanessian, Radar Detection & Tracking Systems (Equation 1-3)
   double getDopplerShift() const  {
      if (lambda > 0.0f) return (-2.0f * getRangeRate()/lambda);
      else return 0.0f;
   }

   // Atmospheric attenuation loss (no units)
   double getAtmosphericAttenuationLoss() const { return lossAtmos; }

   // Sets the atmospheric attenuation loss (no units)
   void setAtmosphericAttenuationLoss(const double loss) { lossAtmos = loss; }

   // Returns the transmit loss (no units)
   double getTransmitLoss() const   { return lossXmit; }

   // Sets the transmit loss (no units)
   void setTransmitLoss(const double v) { lossXmit = v; }

   // Target's Radar Cross Section (RCS) as a function of this emission data (m^2)
   double getRCS() const  { return rcs; }

   // Sets the Radar Cross Section (RCS)
   void setRCS(const double f) { rcs = f; }

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

   virtual void setRange(const double r) override;   // Sets the range to the target (meters) (which we use to set the range loss)
   virtual void clear() override;                    // Clear this emission's data

private:
   double freq {};           // Frequency                        (Hz)
   double lambda {};         // Wavelength                       (meters)
   double pw {};             // Pulse Width                      (Sec)
   double bw {};             // Bandwidth                        (Hz)
   double prf {};            // Pulse Repetition Frequency       (Hz)
   unsigned int pulses {1};  // Number of Pulses this packet     (no units)
   double power {};          // Effective Radiated Power (erp)   (Watts)
   double gain {};           // Effective antenna gain           (no units)
   double lossRng {1.0};     // Loss due to range                (m^-2)
   double lossAtmos {1.0};   // Atmospheric Attenuation Loss     (no units)
   double lossXmit {1.0};    // Transmit loss (default: 1.0)     (no units)
   double rcs {};            // Radar Cross Section (RCS)        (m^2)
   Antenna::Polarization polar {Antenna::NONE};  // Antenna polarization   (enum)
   RfSystem* transmitter {};                     // The system that transmitted the emission
   unsigned int ecmFlag {ECM_OFF};               // ECM enumeration
};

}
}

#endif
