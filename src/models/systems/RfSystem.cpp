
#include "openeaagles/models/systems/RfSystem.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/systems/Antenna.hpp"
#include "openeaagles/models/Emission.hpp"

#include "openeaagles/models/Simulation.hpp"

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Decibel.hpp"
#include "openeaagles/base/units/Powers.hpp"
#include "openeaagles/base/units/Frequencies.hpp"

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(RfSystem, "RfSystem")

// Slot table
BEGIN_SLOTTABLE(RfSystem)
   "antennaName",          //  1: Name of the requested Antenna  (base::String)
   "frequency",            //  2: Frequency     (Hz; def: 0)   (base::Number or base::Frequency)
   "bandwidth",            //  3: Bandwidth     (Hz; def: 1)   (base::Number or base::Frequency)
   "powerPeak",            //  4: Peak Power (Watts; def: 0)
   "threshold",            //  5: RF: Receiver threshold above noise (dB, def: 0.0)
   "noiseFigure",          //  6: RF: Noise Figure (> 1)            (no units; def: 1.0)
   "systemTemperature",    //  7: RF: System Temperature            (Kelvin; def: 290.0)
   "lossXmit",             //  8: RF: Transmit loss                 (dB or no units; def: 1.0)
   "lossRecv",             //  9: RF: Receive loss                  (dB or no units; def: 1.0)
   "lossSignalProcess",    // 10: RF: Signal Processing loss        (dB or no units; def: 1.0)
   "disableEmissions",     // 11: Disable sending emission packets flag (default: false)
   "bandwidthNoise",       // 12: Bandwidth Noise (Hz; def: 'bandwidth') (base::Number or base::Frequency)
END_SLOTTABLE(RfSystem)

//  Map slot table
BEGIN_SLOT_MAP(RfSystem)
    ON_SLOT(1,  setSlotAntennaName,  base::String)
    ON_SLOT(2,  setSlotFrequency,    base::Number)
    ON_SLOT(3,  setSlotBandwidth,    base::Number)
    ON_SLOT(4,  setSlotPeakPower,    base::Number)
    ON_SLOT(5,  setSlotRfThreshold,  base::Decibel)
    ON_SLOT(6,  setSlotRfNoiseFigure,base::Number)
    ON_SLOT(7,  setSlotRfSysTemp,    base::Number)
    ON_SLOT(8,  setSlotRfTransmitLoss, base::Number)
    ON_SLOT(9,  setSlotRfReceiveLoss,  base::Number)
    ON_SLOT(10, setSlotRfSignalProcessLoss, base::Number)
    ON_SLOT(11, setSlotDisableEmissions, base::Number)
    ON_SLOT(12, setSlotBandwidthNoise, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
RfSystem::RfSystem() : packetLock(0), antenna(nullptr), antennaName(nullptr)
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void RfSystem::initData()
{
   xmitEnable = false;
   recvEnable = false;
   disableEmissions = false;
   bwNoiseSet =  false;

   frequency = 0.0;

   rfNoiseFigure = 1.0;
   rfSysTemp = 290.0;
   rfThreshold = 0.0;
   rfLossXmit = 1.0;
   rfLossRecv = 1.0;
   rfLossSignalProcess = 1.0;

   jamSignal = 0.0;
   np = 0;
   for (unsigned int i = 0; i < MAX_EMISSIONS; i++) {
      signals[i] = 0.0;
      packets[i] = nullptr;
   }
   packetLock = 0;

   computeReceiverNoise();
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void RfSystem::copyData(const RfSystem& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // No antenna yet
   setAntenna(nullptr);
   const auto p = const_cast<base::String*>(static_cast<const base::String*>(org.getAntennaName()));
   setSlotAntennaName( p );

   xmitEnable = org.xmitEnable;
   recvEnable = org.recvEnable;
   disableEmissions = org.disableEmissions;
   bwNoiseSet =  org.bwNoiseSet;

   frequency = org.frequency;
   bandwidth = org.bandwidth;
   bandwidthNoise = org.bandwidthNoise;
   powerPeak = org.powerPeak;

   rfNoiseFigure = org.rfNoiseFigure;
   rfSysTemp = org.rfSysTemp;
   rfThreshold = org.rfThreshold;
   rfLossXmit = org.rfLossXmit;
   rfLossRecv = org.rfLossRecv;
   rfLossSignalProcess = org.rfLossSignalProcess;
   rfRecvNoise = org.rfRecvNoise;

   computeReceiverNoise();
}


//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void RfSystem::deleteData()
{
   setAntenna(nullptr);
   setSlotAntennaName(nullptr);
   for (unsigned int i = 0; i < np && i < MAX_EMISSIONS; i++) {
      if (packets[i] != nullptr) {
         packets[i]->unref();
         packets[i] = nullptr;
      }
   }
   np = 0;
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool RfSystem::shutdownNotification()
{
   setAntenna(nullptr);

   for (unsigned int i = 0; i < np && i < MAX_EMISSIONS; i++) {
      if (packets[i] != nullptr) {
         packets[i]->unref();
         packets[i] = nullptr;
      }
   }
   np = 0;

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset()
//------------------------------------------------------------------------------
void RfSystem::reset()
{
   BaseClass::reset();

   // ---
   // Do we need to find the antenna?
   // ---
   if (getAntenna() == nullptr && getAntennaName() != nullptr && getOwnship() != nullptr) {
      // We have a name of the antenna, but not the antenna itself
      const char* name = *getAntennaName();

      // Get the named antenna from the player's list of gimbals, antennas and optics
      const auto p = dynamic_cast<Antenna*>( getOwnship()->getGimbalByName(name) );
      if (p != nullptr) {
         setAntenna( p );
         getAntenna()->setSystem(this);
      }

      if (getAntenna() == nullptr) {
         // The assigned antenna was not found!
         std::cerr << "RfSystem::reset() ERROR -- antenna: " << name << ", was not found!" << std::endl;
         setSlotAntennaName(nullptr);
      }
   }

   // ---
   // Initialize players of interest
   // ---
   processPlayersOfInterest();

}
//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void RfSystem::updateData(const double dt)
{
   // ---
   // Process our players of interest
   // ---
   processPlayersOfInterest();

   // ---
   // Our base class methods
   // ---
   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// process() -- for test
//------------------------------------------------------------------------------
void RfSystem::process(const double)
{
}

//------------------------------------------------------------------------------
// Process players of interest -- This will work with the function in Gimbal to create
// a filtered list of players that we plan to send emission packets to.
//------------------------------------------------------------------------------
void RfSystem::processPlayersOfInterest()
{
   // ---
   // Do we have an antenna?
   // ---
   if (getAntenna() != nullptr) {

      // Pass our players of interest to the antenna for processing
      base::PairStream* poi = nullptr;
      Simulation* sim = getSimulation();
      if ( sim != nullptr && !areEmissionsDisabled() ) {
         poi = sim->getPlayers();
      }

      getAntenna()->processPlayersOfInterest(poi);

      if (poi != nullptr)     { poi->unref(); poi = nullptr; }
   }
}

//------------------------------------------------------------------------------
// rfReceivedEmission() -- process returned RF Emission
//------------------------------------------------------------------------------
void RfSystem::rfReceivedEmission(Emission* const em, Antenna* const, double raGain)
{
   // Queue up emissions for receive() to process
   if (em != nullptr && isReceiverEnabled()) {

      // Test to make sure the received emission is in-band before proceeding
      if (affectsRfSystem(em)) {

         // Pulses this radar frame (from emission)
         //double pulses = static_cast<double>( em->getPulses() );
         //if (pulses <= 0) pulses = 1.0f;

         // Compute signal losses
         //    Basically, we're simulating Hannen's S/I equation from page 356 of his notes.
         //    Where I is N + J. J is noise from jamming.
         //    Receiver Loss affects the total I, so we have to wait until J is added to N in Radar.
         double losses = getRfSignalProcessLoss() * em->getAtmosphericAttenuationLoss() * em->getTransmitLoss();
         if (losses < 1.0) losses = 1.0;

         // Range loss
         const double rl = em->getRangeLoss();

         // Signal Equation (one way signal)
         // Signal Equation (Part of equation 2-7)
         // Signal (equation 3-3)
         const double signal = em->getPower() * rl * raGain / losses;

         // Noise Jammer -- add this signal to the total interference signal (noise)
         if ( em->isECMType(Emission::ECM_NOISE) ) {
            // CGB part of the noise jamming equation says we're only affected by the ratio of the
            // transmitter and receiver bandwidths.
            // It's possible that we'll want to account for this in the signal calculation above.
            // But, for now, it is sufficient right here.
            jamSignal += (signal * getBandwidth() / em->getBandwidth());
         }

         // Save packet and signal for receive()
         base::lock(packetLock);
         if (np < MAX_EMISSIONS) {
            em->ref();
            packets[np] = em;
            signals[np] = signal;
            np++;
         }
         base::unlock(packetLock);

      }
   }
}


//------------------------------------------------------------------------------
// transmitPower() -- Compute transmitter power (Part of equation 2-1)
//------------------------------------------------------------------------------
double RfSystem::transmitPower(const double peakPwr) const
{
    double pwr = peakPwr;
    if (rfLossXmit >= 1.0) pwr = peakPwr / rfLossXmit;
    return pwr;
}


//------------------------------------------------------------------------------
// Get Functions
//------------------------------------------------------------------------------

// Returns true if the R/F system's receiver is enabled
bool RfSystem::isReceiverEnabled() const
{
   return recvEnable && (getPowerSwitch() > PWR_STBY);
}

// Returns true if the R/F system's transmitter is enabled
bool RfSystem::isTransmitterEnabled() const
{
   return xmitEnable && (getPowerSwitch() > PWR_STBY);
}

// Returns true if the R/F system is transmitting
bool RfSystem::isTransmitting() const
{
   // Default: if we're enabled and have an antenna, we're transmitting.
   return ( isTransmitterEnabled() && getAntenna() != nullptr && getOwnship() != nullptr );
}

// Returns true if the input frequency is within the frequency band of the R/F system
bool RfSystem::isFrequencyInBand(double hz) const
{
    if ((frequency - bandwidth / static_cast<double>(2.0)) <= hz
         && (frequency + bandwidth / static_cast<double>(2.0)) >= hz) {
        return true;
    }
    return false;
}

// Returns true if sending emission packets has been disabled
bool RfSystem::areEmissionsDisabled() const
{
   return disableEmissions;
}


// Returns the R/F system's frequency (hertz)
double RfSystem::getFrequency() const
{
   return frequency;
}

// Returns the R/F system's bandwidth (hertz)
double RfSystem::getBandwidth() const
{
   return bandwidth;
}

// Returns the R/F system's bandwidth noise (hertz)
double RfSystem::getBandwidthNoise() const
{
   return (bwNoiseSet ? bandwidthNoise : bandwidth);
}

// Returns the R/F system's transmitter's peak power (watts)
double RfSystem::getPeakPower() const
{
   return powerPeak;
}

// Returns system temperature (Kelvin)
double RfSystem::getRfSysTemp() const
{
   return rfSysTemp;
}

// Returns the receiver noise (watts)
double RfSystem::getRfRecvNoise() const
{
   return rfRecvNoise;
}

// Returns the receiver threshold (over S/N) (dB)
double RfSystem::getRfThreshold() const
{
   return rfThreshold;
}

// Returns the transmit loss (no units)
double RfSystem::getRfTransmitLoss() const
{
   return rfLossXmit;
}

// Returns the receive loss   (no units)
double RfSystem::getRfReceiveLoss() const
{
   return rfLossRecv;
}

// Returns the signal Processing loss (no units)
double RfSystem::getRfSignalProcessLoss() const
{
   return rfLossSignalProcess;
}

// Returns the receiver noise figure (no units)
double RfSystem::getRfNoiseFigure() const
{
   return rfNoiseFigure;
}

// Pointer to the antenna model, or zero (0) if none
Antenna* RfSystem::getAntenna()
{
   return antenna;
}

const Antenna* RfSystem::getAntenna() const
{
   return antenna;
}

// Name of the antenna model, or zero (0) if none
base::String* RfSystem::getAntennaName()
{
   return antennaName;
}

const base::String* RfSystem::getAntennaName() const
{
   return antennaName;
}

// Returns true if the received emission is in-band
bool RfSystem::affectsRfSystem(Emission* const em) const
{
   const double emFreq = em->getFrequency();
   const double emBandwidth = em->getBandwidth();
   const double sysFreq = getFrequency();
   const double sysBandwidth = getBandwidth();
   const double emFreqStart = emFreq - 0.5 * emBandwidth;
   const double emFreqEnd = emFreq + 0.5f * emBandwidth;
   const double sysFreqStart = sysFreq - 0.5 * sysBandwidth;
   const double sysFreqEnd = sysFreq + 0.5 * sysBandwidth;
   return (emFreqEnd >= sysFreqStart && emFreqStart <= sysFreqEnd);
}

//------------------------------------------------------------------------------
// Set Functions (return 'true' if the argument was set, else 'false')
//------------------------------------------------------------------------------

// setPeakPower() -- set the peak power of the R/F transmitter
bool RfSystem::setPeakPower(const double watts)
{
   powerPeak = watts;
   return true;
}

// setFrequency() -- set frequency
bool RfSystem::setFrequency(const double hz)
{
   frequency = hz;
   return true;
}

// setBandwidth() -- set the bandwidth; must be >= 1
bool RfSystem::setBandwidth(const double hz)
{
   bool ok = false;
   // Bandwidth must be greater than or equal one!
   if (hz >= 1.0) {
      bandwidth = hz;
      computeReceiverNoise();
      ok = true;
   }
   return ok;
}

// setBandwidthNoise() -- set the bandwidth noise; must be >= 1
bool RfSystem::setBandwidthNoise(const double hz)
{
   bool ok = false;
   // Bandwidth noise must be greater than or equal one!
   if (hz >= 1.0) {
      bandwidthNoise = hz;
      bwNoiseSet = true;
      computeReceiverNoise();
      ok = true;
   }
   return ok;
}

bool RfSystem::setRfThreshold(const double v)
{
   rfThreshold = v;
   return true;
}

bool RfSystem::setRfTransmitLoss(const double v)
{
   bool ok = false;
   // Transmitter loss must be greater than or equal one!
   if (v >= 1.0) {
      rfLossXmit = v;
      ok = true;
   }
   return ok;
}

bool RfSystem::setRfReceiveLoss(const double v)
{
   bool ok = false;
   // Receiver loss must be greater than or equal one!
   if (v >= 1.0) {
      rfLossRecv = v;
      ok = true;
   }
   return ok;
}

bool RfSystem::setRfSignalProcessLoss(const double v)
{
   bool ok = false;
   // Signal processing loss must be greater than or equal one!
   if (v >= 1.0) {
      rfLossSignalProcess = v;
      ok = true;
   }
   return ok;
}

bool RfSystem::setRfNoiseFigure(const double v)
{
   bool ok = false;
   // Noise figure must be greater than or equal one!
   if (v >= 1.0) {
      rfNoiseFigure = v;
      computeReceiverNoise();
      ok = true;
   }
   return ok;
}

bool RfSystem::setRfSysTemp(const double v)
{
   bool ok = false;
   // Temperature must be greater than zero!
   if (v >= 1.0) {
      rfSysTemp = v;
      computeReceiverNoise();
      ok = true;
   }
   return ok;
}

// Sets the receiver noise (Watts)
bool RfSystem::setReceiverNoise(const double v)
{
   bool ok = false;
   // noise must be greater than or equal zero!
   if (v >= 0.0) {
      rfRecvNoise = v;
      ok = true;
   }
   return ok;
}


// setReceiverEnabledFlag() -- enable/disable the receiver
bool RfSystem::setReceiverEnabledFlag(const bool b)
{
   recvEnable = b;
   return true;
}

// setTransmitterEnableFlag() -- enable/disable the transmitter
bool RfSystem::setTransmitterEnableFlag(const bool b)
{
   xmitEnable = b;
   return true;
}

// Disables/enables sending the R/F emissions packets
bool RfSystem::setDisableEmissionsFlag(const bool b)
{
   disableEmissions = b;
   return true;
}

// setAntenna() -- set the antenna
bool RfSystem::setAntenna(Antenna* const p)
{
   if (antenna != nullptr) {
      antenna->unref();
   }
   antenna = p;
   if (antenna != nullptr) {
      antenna->ref();
   }
   return true;
}

// Compute receiver thermal noise (equation 2-8)
bool RfSystem::computeReceiverNoise()
{
   return setReceiverNoise(rfNoiseFigure * static_cast<float>(base::BOLTZMANN) * rfSysTemp * getBandwidthNoise());
}

//------------------------------------------------------------------------------
// Slot Functions  (return 'true' if the slot was set, else 'false')
//------------------------------------------------------------------------------

// antennaName: Antenna name  (base::String)
bool RfSystem::setSlotAntennaName(base::String* const p)
{
   if (antennaName != nullptr) {
      antennaName->unref();
   }
   antennaName = p;
   if (antennaName != nullptr) {
      antennaName->ref();
   }
   return true;
}

// setSlotFrequency() -- Set the Frequency (base::Number or base::Frequency)
bool RfSystem::setSlotFrequency(base::Number* const v)
{
    bool ok = false;
    double x = -1.0;

    const auto p = dynamic_cast<const base::Frequency*>(v);
    if (p != nullptr) {
        // Has frequency and we need hertz
        x = base::Hertz::convertStatic(*p);
    }
    else if (v != nullptr) {
        // Just a Number
        x = v->getReal();
    }

    // Test and set the freq
    if (x >= 0.0) {
        ok = setFrequency(x);
    }
    else {
        std::cerr << "RfSystem::setFrequency: Frequency must be greater than or equal zero!" << std::endl;
    }

    return ok;
}

// bandwidth: Bandwidth     (Hz)
bool RfSystem::setSlotBandwidth(base::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {

        double bw = -1.0f;

        const auto p = dynamic_cast<const base::Frequency*>(num);
        if (p != nullptr) {
            // Has frequency and we need hertz
            bw = base::Hertz::convertStatic(*p);
        }
        else {
            // Just a Number
            bw = num->getReal();
        }

        if (bw >= 1.0) {
            ok = setBandwidth(bw);
        }
        else {
            std::cerr << "RfSystem::setSlotBandwidth: Bandwidth must be greater than or equal to one!" << std::endl;
        }
    }
    return ok;
}

// bandwidthNoise: Bandwidth Noise  (Hz)
bool RfSystem::setSlotBandwidthNoise(base::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {

        double bw = -1.0f;

        const auto p = dynamic_cast<const base::Frequency*>(num);
        if (p != nullptr) {
            // Has frequency and we need hertz
            bw = base::Hertz::convertStatic(*p);
        }
        else {
            // Just a Number
            bw = num->getReal();
        }

        if (bw >= 1.0) {
            ok = setBandwidthNoise(bw);
        }
        else {
            std::cerr << "RfSystem::setSlotBandwidthNoise: Bandwidth Noise must be greater than or equal to one!" << std::endl;
        }
    }
    return ok;
}

// setSlotPeakPower() -- set the peak power (base::Number or base::Watts)
bool RfSystem::setSlotPeakPower(base::Number* const v)
{
    bool ok = false;
    double x = -1.0;

    const auto p = dynamic_cast<base::Power*>(v);
    if (p != nullptr) {
        // Has power units and we need watts
        base::Watts watts;
        x = watts.convert(*p);
    }
    else if (v != nullptr) {
        // Just a Number
        x = v->getReal();
    }

    // Test and set the power
    if (x >= 0.0) {
        ok = setPeakPower(x);
    }
    else {
        std::cerr << "RfSystem::setPeakPower: Power must be greater than or equal zero!" << std::endl;
    }

    return ok;
}

// setSlotRfThreshold() -- set the receiver threshold (db over S/N)
bool RfSystem::setSlotRfThreshold(base::Decibel* const v)
{
    bool ok = false;
    if (v != nullptr) {
        ok = setRfThreshold( v->getValueDB() );
    }
    return ok;
}

// setSlotRfNoiseFigure() -- set the noise figure (no units)
bool RfSystem::setSlotRfNoiseFigure(base::Number* const v)
{
    bool ok = false;
    if (v != nullptr) {
        const double fig = v->getReal();
        if (fig >= 1.0) {
            ok = setRfNoiseFigure( fig );
        }
        else {
            std::cerr << "RfSystem::setRfNoiseFigure: Figure must be greater than one!" << std::endl;
        }
    }
    return ok;
}

// setSlotRfSysTemp() -- set the system temperature (kelvin)
bool RfSystem::setSlotRfSysTemp(base::Number* const v)
{
    bool ok = false;
    if (v != nullptr) {
        const double tmp = v->getReal();
        if (tmp > 0.0) {
            ok = setRfSysTemp( tmp );;
        }
        else {
            std::cerr << "RfSystem::setRfSysTemp: Temperature must be greater than zero!" << std::endl;
        }
    }
    return ok;
}

// setSlotRfTransmitLoss() -- set the transmit loss
bool RfSystem::setSlotRfTransmitLoss(base::Number* const v)
{
    bool ok = false;
    if (v != nullptr) {
        const double loss = v->getReal();
        if (loss >= 1.0) {
            ok = setRfTransmitLoss(loss);
        }
        else {
            std::cerr << "RfSystem::setTransmitLoss: Loss must be greater than or equal to one!" << std::endl;
        }
    }
    return ok;
}

// setSlotRfReceiveLoss() -- set the receive loss
bool RfSystem::setSlotRfReceiveLoss(base::Number* const v)
{
    bool ok = false;
    if (v != nullptr) {
        const double loss = v->getReal();
        if (loss >= 1.0) {
            ok = setRfReceiveLoss(loss);
        }
        else {
            std::cerr << "RfSystem::setReceiveLoss: Loss must be greater than or equal to one!" << std::endl;
        }
    }
    return ok;
}

// setSlotRfSignalProcessLoss() -- set signal processing loss
bool RfSystem::setSlotRfSignalProcessLoss(base::Number* const v)
{
    bool ok = false;
    if (v != nullptr) {
        const double loss = v->getReal();
        if (loss >= 1.0) {
            ok = setRfSignalProcessLoss(loss);
        }
        else {
            std::cerr << "RfSystem::setRfSignalProcessLoss: Loss must be greater than or equal to one!" << std::endl;
        }
    }
    return ok;
}

// setSlotDisableEmissions() -- sets the disable sending emissions flag
bool RfSystem::setSlotDisableEmissions(base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setDisableEmissionsFlag( msg->getBoolean() );
   }
   return ok;
}

std::ostream& RfSystem::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    //"antennaName",          //  Name of the requested Antenna  (base::String)
    if (antennaName != nullptr) {
        indent(sout,i+j);
        sout << "antenna: " << *antennaName << std::endl;
    }

    //"frequency",            //  Frequency     (Hz; def: 0)        (base::Number or LcFrequecy)
    indent(sout,i+j);
    sout << "frequency: ( Hertz " << frequency << std::endl;

    //"bandwidth",            //  Bandwidth     (Hz; def: 0)        (base::Number or LcFrequecy)
    indent(sout,i+j);
    sout << "bandwidth: ( Hertz " << bandwidth << std::endl;

    //"powerPeak",            //  Peak Power (Watts; def: 0)
    indent(sout,i+j);
    sout << "powerPeak: " << powerPeak << std::endl;

    //"threshold",            //  RF: Receiver threshold above noise (dB, def: 0.0)
    indent(sout,i+j);
    sout << "threshold: " << rfThreshold << std::endl;

    //"noiseFigure",          //  RF: Noise Figure (> 1)            (no units; def: 1.0)
    indent(sout,i+j);
    sout << "noiseFigure: " << rfNoiseFigure << std::endl;

    //"systemTemperature",    //  RF: System Temperature            (Kelvin; def: 290.0)
    indent(sout,i+j);
    sout << "systemTemperature: " << rfSysTemp  << std::endl;

    //"lossXmit",             //  RF: Transmit loss                 (dB or no units; def: 1.0)
    indent(sout,i+j);
    sout << "lossXmit: " << rfLossXmit  << std::endl;

    //"lossRecv",             //  RF: Receive loss                  (dB or no units; def: 1.0)
    indent(sout,i+j);
    sout << "lossRecv: " << rfLossRecv << std::endl;

    //"lossSignalProcess",    //  RF: Signal Processing loss        (dB or no units; def: 1.0)
    indent(sout,i+j);
    sout << "lossSignalProcess: " << rfLossSignalProcess << std::endl;

    //"disableEmissions",     //  Disable sending emission packets flag (default: false)
    indent(sout,i+j);
    if (disableEmissions) {
        sout << "disableEmissions: true  " << std::endl;
    }
    else {
        sout << "disableEmissions: false " << std::endl;
    }
    // DPG #### Need to print slots!!!
    indent(sout,i+j);
    sout << "//--- inherits System ------ "  << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
