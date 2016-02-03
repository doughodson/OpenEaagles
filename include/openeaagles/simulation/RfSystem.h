//------------------------------------------------------------------------------
// Class: RfSystem
//------------------------------------------------------------------------------
#ifndef __oe_simulation_RfSystem_H__
#define __oe_simulation_RfSystem_H__

#include "openeaagles/simulation/System.h"

namespace oe {
   namespace basic { class Decibel; }

namespace simulation {
   class Antenna;
   class Emission;

//------------------------------------------------------------------------------
// Class: RfSystem
//
// Description: Generic class for all R/F type systems.  Provides a common
//              interface, and some default behavior, for R/F systems.
//
// Notes
//    1) By convention, R/F systems are responsible for filtering their own players
//    of interest, so there is the processPlayersOfInterest() function, which is
//    called by our updateData(), that can be used by our derived classes to filter
//    players, as needed.  The Gimbal class, which is used to create antennas and
//    seekers, has its own member function processPlayersOfInterest() that filters
//    by gimbal field of view and player type.  It's the responsibility of the
//    various R/F systems to use or not use this gimbal function -- our default
//    member function processPlayersOfInterest() does use the gimbal function.
//
// Factory name: RfSystem
// Slots:
//    antennaName        <basic::String>        ! Name of the requested Antenna
//
//    frequency          <basic::Frequency>     ! Radio's Main Frequency (default: 0 hz)
//                       <basic::Number>        ! Radio's Main Frequency (hz)
//
//    bandwidth          <basic::Frequency>     ! Radio's Bandwidth (default: 1.0 hz)
//                       <basic::Number>        ! Radio's Bandwidth (Hz)
//
//    bandwidthNoise     <basic::Frequency>     ! Radio's Bandwidth Noise (default: 'bandwidth', 1.0)
//                       <basic::Number>        ! Radio's Bandwidth Noise (Hz)
//
//    powerPeak          <basic::Watts>         ! Peak Power (default: 0.0)
//                       <basic::Number>        ! Peak Power in watts
//
//    threshold          <basic::Decibel>       ! Receiver threshold above noise (dB, default: 0.0)
//
//    noiseFigure        <basic::Number>        ! Noise Figure (> 1)            (no units; def: 1.0)
//
//    systemTemperature  <basic::Number>        ! System Temperature            (Kelvin; def: 290.0)
//
//    lossXmit           <basic::Number>        ! Transmit loss                 (no units; def: 1.0)
//                       <basic::Decibel>       ! Transmit loss                 (dB)
//
//    lossRecv           <basic::Number>        ! Receive loss                  (no units; def: 1.0)
//                       <basic::Decibel>       ! Receive loss                  (dB)
//
//    lossSignalProcess  <basic::Number>        ! Signal Processing loss        (no units; def: 1.0)
//                       <basic::Decibel>       ! Signal Processing loss        (dB)
//
//    disableEmissions   <Number>               ! Disable sending emission packets flag (default: false)
//
//------------------------------------------------------------------------------
class RfSystem : public System
{
   DECLARE_SUBCLASS(RfSystem,System)

public:
   RfSystem();

   virtual bool isReceiverEnabled() const;               // Returns true if the R/F system's receiver is enabled
   virtual bool isTransmitterEnabled() const;            // Returns true if the R/F system's transmitter is enabled
   virtual bool isTransmitting() const;                  // Returns true if the R/F system's is transmitting
   virtual bool isFrequencyInBand(LCreal hz) const;      // Returns true if the input frequency is within the frequency
                                                         // band of the R/F system

   virtual bool areEmissionsDisabled() const;            // Returns true if sending emission packets has been disabled

   virtual LCreal getFrequency() const;                  // Returns the R/F system's frequency (hertz)
   virtual LCreal getBandwidth() const;                  // Returns the R/F system's bandwidth (hertz)
   virtual LCreal getBandwidthNoise() const;             // Returns the R/F system's bandwidth noise (hertz)
   virtual LCreal getPeakPower() const;                  // Returns the R/F system's transmitter's peak power (watts)
   virtual LCreal getRfSysTemp() const;                  // Returns system temperature (Kelvin)
   virtual LCreal getRfRecvNoise() const;                // Returns the receiver noise (watts)
   virtual LCreal getRfThreshold() const;                // Returns the receiver threshold (over S/N) (dB)
   virtual LCreal getRfTransmitLoss() const;             // Returns the transmit loss (no units)
   virtual LCreal getRfReceiveLoss() const;              // Returns the receive loss   (no units)
   virtual LCreal getRfSignalProcessLoss() const;        // Returns the signal Processing loss (no units)
   virtual LCreal getRfNoiseFigure() const;              // Returns the receiver noise figure (no units)

   virtual Antenna* getAntenna();
   virtual const Antenna* getAntenna() const;            // Pointer to the antenna model, or zero (0) if none
   virtual bool setAntenna(Antenna* const p);            // Sets the R/F system's antenna

   virtual basic::String* getAntennaName();
   virtual const basic::String* getAntennaName() const;  // Name of the antenna model, or zero (0) if none

   // Tests if the received emission can affect the RfSystem and be processed by it.
   virtual bool affectsRfSystem(Emission* const em) const;

   virtual bool setFrequency(const LCreal hz);           // Sets the frequency (hertz)
   virtual bool setBandwidth(const LCreal hz);           // Sets the bandwidth (hertz) (must be >= 1)
   virtual bool setBandwidthNoise(const LCreal hz);      // Sets the bandwidth noise (hertz) (must be >= 1)
   virtual bool setPeakPower(const LCreal watts);        // Sets the transmitter's peak power (watts)
   virtual bool setRfSysTemp(const LCreal v);            // Sets the system temperature (Kelvin)
   virtual bool setRfThreshold(const LCreal v);          // Sets the receiver threshold (over S/N) (dB)
   virtual bool setRfNoiseFigure(const LCreal v);        // Sets the receiver noise figure (>= 1)  (no units)
   virtual bool setRfTransmitLoss(const LCreal v);       // Sets the transmit loss (default: 1.0) (no units)
   virtual bool setRfReceiveLoss(const LCreal v);        // Sets the receive loss (default: 1.0) (no units)
   virtual bool setRfSignalProcessLoss(const LCreal v);  // Sets the signal Processing loss (default: 1.0) (no units)
   virtual bool setReceiverNoise(const LCreal v);        // Sets the receiver noise (Watts)

   virtual bool setReceiverEnabledFlag(const bool b);    // Enables/disables the R/F system's receiver
   virtual bool setTransmitterEnableFlag(const bool b);  // Enables/disables the R/F system's transmitter
   virtual bool setDisableEmissionsFlag(const bool b);   // Disables/enables sending the R/F emissions packets

   // Returns the transmitter power after losses (watts)
   virtual LCreal transmitPower(const LCreal peakPwr) const;

   // Accepts an emission from an antenna
   virtual void rfReceivedEmission(Emission* const em, Antenna* const ra, const LCreal raGain);

   // Slot functions
   virtual bool setSlotAntennaName(basic::String* const msg);
   virtual bool setSlotPeakPower(basic::Number* const msg);
   virtual bool setSlotFrequency(basic::Number* const msg);
   virtual bool setSlotBandwidth(basic::Number* const msg);
   virtual bool setSlotBandwidthNoise(basic::Number* const msg);
   virtual bool setSlotRfThreshold(basic::Decibel* const msg);
   virtual bool setSlotRfNoiseFigure(basic::Number* const msg);
   virtual bool setSlotRfSysTemp(basic::Number* const msg);
   virtual bool setSlotRfTransmitLoss(basic::Number* const msg);
   virtual bool setSlotRfReceiveLoss(basic::Number* const msg);
   virtual bool setSlotRfSignalProcessLoss(basic::Number* const msg);
   virtual bool setSlotDisableEmissions(basic::Number* const msg);

   void updateData(const LCreal dt = 0.0) override;
   void reset() override;

protected:
   // Max size of emission queues (per frame)
   static const unsigned int MAX_EMISSIONS = OE_CONFIG_RF_MAX_EMISSIONS;

   // Compute receiver thermal noise
   virtual bool computeReceiverNoise();

   // The following are filled by rfReceivedEmission() and consumed (emptied) by receive()
   LCreal jamSignal;                  // Interference signal (from Jammer)
   unsigned int np;                   // Number of emission packets being passed from rfReceivedEmission() to receive()
   LCreal signals[MAX_EMISSIONS];     // signals values being passed from rfReceivedEmission() to receive()
   Emission* packets[MAX_EMISSIONS];  // emission packets being passed from rfReceivedEmission() to receive()
   mutable long packetLock;           // Semaphore to protect 'signals' and 'xxpackets

   // Process players of interest -- Called by our updateData() -- the background thread --
   // This function will create a filtered list of players that R/F systems will interact with.
   virtual void processPlayersOfInterest();

   // Callbacks by phase -- to be used by the derived classes, as needed
   void process(const LCreal dt) override;     // Phase 3

   // basic::Component protected interface
   bool shutdownNotification() override;

private:
   void initData();

   Antenna*  antenna;            // Our antenna
   basic::String* antennaName;        // Name of our antenna

   bool     xmitEnable;          // Transmitter enabled
   bool     recvEnable;          // Receiver enabled
   bool     disableEmissions;    // Disable sending emission packets flag (default: false)
   bool     bwNoiseSet;          // Bandwidth noise has been set

   LCreal   powerPeak;           // Peak Power (default: 0) )    (Watts)
   LCreal   frequency;           // Frequency            (Hz)
   LCreal   bandwidth;           // Receiver bandwidth    (Hz)
   LCreal   bandwidthNoise;      // [B] Receiver bandwidth noise (Hz)
   LCreal   rfNoiseFigure;       // [F] Receiver noise figure (> 1)          (no units)
   LCreal   rfSysTemp;           // [T] System Temperature                   (Kelvin)
   LCreal   rfRecvNoise;         // [N] Noise (N = F * k * T * B)            (Watts)
   LCreal   rfThreshold;         // Receiver threshold (over S/N)            (dB)
   LCreal   rfLossXmit;          // Transmit loss (default: 1.0)             (no units)
   LCreal   rfLossRecv;          // Receive loss (default: 1.0)              (no units)
   LCreal   rfLossSignalProcess; // Signal Processing loss (default: 1.0)    (no units)
};

} // End simulation namespace
} // End oe namespace

#endif
