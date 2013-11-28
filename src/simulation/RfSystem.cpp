//------------------------------------------------------------------------------
// Class: RfSystem
//------------------------------------------------------------------------------
#include "openeaagles/simulation/RfSystem.h"

#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Decibel.h"
#include "openeaagles/basic/units/Powers.h"
#include "openeaagles/basic/units/Frequencies.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(RfSystem,"RfSystem")

// Slot table
BEGIN_SLOTTABLE(RfSystem)
   "antennaName",          //  1: Name of the requested Antenna  (Basic::String)
   "frequency",            //  2: Frequency     (Hz; def: 0)   (Basic::Number or Basic::Frequency)
   "bandwidth",            //  3: Bandwidth     (Hz; def: 1)   (Basic::Number or Basic::Frequency)
   "powerPeak",            //  4: Peak Power (Watts; def: 0)
   "threshold",            //  5: RF: Receiver threshold above noise (dB, def: 0.0)
   "noiseFigure",          //  6: RF: Noise Figure (> 1)            (no units; def: 1.0)
   "systemTemperature",    //  7: RF: System Temperature            (Kelvin; def: 290.0)
   "lossXmit",             //  8: RF: Transmit loss                 (dB or no units; def: 1.0) 
   "lossRecv",             //  9: RF: Receive loss                  (dB or no units; def: 1.0)
   "lossSignalProcess",    // 10: RF: Signal Processing loss        (dB or no units; def: 1.0)
   "disableEmissions",     // 11: Disable sending emission packets flag (default: false)
   "bandwidthNoise",       // 12: Bandwidth Noise (Hz; def: 'bandwidth') (Basic::Number or Basic::Frequency)
END_SLOTTABLE(RfSystem)

//  Map slot table 
BEGIN_SLOT_MAP(RfSystem)
    ON_SLOT(1,  setSlotAntennaName,  Basic::String)
    ON_SLOT(2,  setSlotFrequency,    Basic::Number)
    ON_SLOT(3,  setSlotBandwidth,    Basic::Number)
    ON_SLOT(4,  setSlotPeakPower,    Basic::Number)
    ON_SLOT(5,  setSlotRfThreshold,  Basic::Decibel)
    ON_SLOT(6,  setSlotRfNoiseFigure,Basic::Number)
    ON_SLOT(7,  setSlotRfSysTemp,    Basic::Number)
    ON_SLOT(8,  setSlotRfTransmitLoss, Basic::Number)
    ON_SLOT(9,  setSlotRfReceiveLoss,  Basic::Number)
    ON_SLOT(10, setSlotRfSignalProcessLoss, Basic::Number)
    ON_SLOT(11, setSlotDisableEmissions, Basic::Number)
    ON_SLOT(12, setSlotBandwidthNoise, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
RfSystem::RfSystem() : packetLock(0), antenna(0), antennaName(0)
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

   frequency = 0;
   bandwidth = 1.0;
   bandwidthNoise = 1.0;
   powerPeak = 0.0;

   rfNoiseFigure = 1.0;
   rfSysTemp = 290.0;
   rfThreshold = 0.0;
   rfLossXmit = 1.0;
   rfLossRecv = 1.0;
   rfLossSignalProcess = 1.0;

   jamSignal = 0.0;
   np = 0;
   for (unsigned int i = 0; i < MAX_EMISSIONS; i++) {
      signals[i] = 0;
      packets[i] = 0;
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
   setAntenna(0);
   Basic::String* p = (Basic::String*) org.getAntennaName();
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
   setAntenna(0);
   setSlotAntennaName(0);
   for (unsigned int i = 0; i < np && i < MAX_EMISSIONS; i++) {
      if (packets[i] != 0) {
         packets[i]->unref();
         packets[i] = 0;
      }
   }
   np = 0;
}

void RfSystem::reset()
{
   BaseClass::reset();

   // ---
   // Do we need to find the antenna?
   // ---
   if (getAntenna() == 0 && getAntennaName() != 0 && getOwnship() != 0) {
      // We have a name of the antenna, but not the antenna itself
      const char* name = *getAntennaName();
      
      // Get the named antenna from the player's list of gimbals, antennas and optics
      Antenna* p = dynamic_cast<Antenna*>( getOwnship()->getGimbalByName(name) );
      if (p != 0) {
         setAntenna( p );
         getAntenna()->setSystem(this);
      }
      
      if (getAntenna() == 0) {
         // The assigned antenna was not found!
         std::cerr << "RfSystem::reset() ERROR -- antenna: " << name << ", was not found!" << std::endl;
         setSlotAntennaName(0);
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
void RfSystem::updateData(const LCreal dt)
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
void RfSystem::process(const LCreal)
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
   if (getAntenna() != 0) {

      // Pass our players of interest to the antenna for processing
      Basic::PairStream* poi = 0;
      Simulation* sim = getSimulation();
      if ( sim != 0 && !areEmissionsDisabled() ) {
         poi = sim->getPlayers();
      }

      getAntenna()->processPlayersOfInterest(poi);

      if (poi != 0) { poi->unref(); poi = 0; }
   }
}

//------------------------------------------------------------------------------
// rfReceivedEmission() -- process returned RF Emission
//------------------------------------------------------------------------------
void RfSystem::rfReceivedEmission(Emission* const em, Antenna* const, LCreal raGain)
{
   // Queue up emissions for receive() to process
   if (em != 0 && isReceiverEnabled()) {

      // Test to make sure the received emission is in-band before proceeding
      if (affectsRfSystem(em)) {

         // Pulses this radar frame (from emission)
         //LCreal pulses = LCreal( em->getPulses() );
         //if (pulses <= 0) pulses = 1.0f;

         // Compute signal losses
         //    Basically, we're simulating Hannen's S/I equation from page 356 of his notes.
         //    Where I is N + J. J is noise from jamming.
         //    Receiver Loss affects the total I, so we have to wait until J is added to N in Radar.
         LCreal losses = getRfSignalProcessLoss() * em->getAtmosphericAttenuationLoss() * em->getTransmitLoss();
         if (losses < 1.0f) losses = 1.0f;

         // Range loss
         LCreal rl = em->getRangeLoss();

         // Signal Equation (one way signal)
         // Signal Equation (Part of equation 2-7)
         // Signal (equation 3-3)
         LCreal signal = em->getPower() * rl * raGain / losses;

         // Noise Jammer -- add this signal to the total interference signal (noise)
         if ( em->isECMType(Emission::ECM_NOISE) ) {
            // CGB part of the noise jamming equation says we're only affected by the ratio of the
            // transmitter and receiver bandwidths.
            // It's possible that we'll want to account for this in the signal calculation above.
            // But, for now, it is sufficient right here.
            jamSignal += (signal * getBandwidth() / em->getBandwidth());
         }

         // Save packet and signal for receive()
         lcLock(packetLock);
         if (np < MAX_EMISSIONS) {
            em->ref();
            packets[np] = em;
            signals[np] = signal;
            np++;
         }
         lcUnlock(packetLock);

      }
   }
}


//------------------------------------------------------------------------------
// transmitPower() -- Compute transmitter power (Part of equation 2-1)
//------------------------------------------------------------------------------
LCreal RfSystem::transmitPower(const LCreal peakPwr) const
{
    LCreal pwr = peakPwr;
    if (rfLossXmit >= 1.0f) pwr = peakPwr/rfLossXmit;
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
   return ( isTransmitterEnabled() && getAntenna() != 0 && getOwnship() != 0 );
}

// Returns true if the input frequency is within the frequency band of the R/F system
bool RfSystem::isFrequencyInBand(LCreal hz) const
{
    if ((frequency - bandwidth/(LCreal)2.0) <= hz && (frequency + bandwidth/(LCreal)2.0) >= hz) {
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
LCreal RfSystem::getFrequency() const
{
   return frequency;
}


// Returns the R/F system's bandwidth (hertz)
LCreal RfSystem::getBandwidth() const
{
   return bandwidth;
}

// Returns the R/F system's bandwidth noise (hertz)
LCreal RfSystem::getBandwidthNoise() const
{
   return (bwNoiseSet ? bandwidthNoise : bandwidth);
}

// Returns the R/F system's transmitter's peak power (watts)
LCreal RfSystem::getPeakPower() const
{
   return powerPeak;
}

// Returns system temperature (Kelvin)
LCreal RfSystem::getRfSysTemp() const
{
   return rfSysTemp;
}

// Returns the receiver noise (watts)
LCreal RfSystem::getRfRecvNoise() const
{
   return rfRecvNoise;
}

// Returns the receiver threshold (over S/N) (dB)
LCreal RfSystem::getRfThreshold() const
{
   return rfThreshold;
}

// Returns the transmit loss (no units)
LCreal RfSystem::getRfTransmitLoss() const
{
   return rfLossXmit;
}

// Returns the receive loss   (no units)
LCreal RfSystem::getRfReceiveLoss() const
{
   return rfLossRecv;
}

// Returns the signal Processing loss (no units)
LCreal RfSystem::getRfSignalProcessLoss() const
{
   return rfLossSignalProcess;
}

// Returns the receiver noise figure (no units)
LCreal RfSystem::getRfNoiseFigure() const
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
Basic::String* RfSystem::getAntennaName()
{
   return antennaName;
}

const Basic::String* RfSystem::getAntennaName() const
{
   return antennaName;
}

// Returns true if the received emission is in-band
bool RfSystem::affectsRfSystem(Emission* const em) const
{
   double emFreq = em->getFrequency();
   double emBandwidth = em->getBandwidth();
   double sysFreq = getFrequency();
   double sysBandwidth = getBandwidth();
   double emFreqStart = emFreq - 0.5f * emBandwidth;
   double emFreqEnd = emFreq + 0.5f * emBandwidth;
   double sysFreqStart = sysFreq - 0.5f * sysBandwidth;
   double sysFreqEnd = sysFreq + 0.5f * sysBandwidth;
   return (emFreqEnd >= sysFreqStart && emFreqStart <= sysFreqEnd);
}

//------------------------------------------------------------------------------
// Set Functions (return 'true' if the argument was set, else 'false')
//------------------------------------------------------------------------------

// setPeakPower() -- set the peak power of the R/F transmitter
bool RfSystem::setPeakPower(const LCreal watts)
{
   powerPeak = watts;
   return true;
}

// setFrequency() -- set frequency
bool RfSystem::setFrequency(const LCreal hz)
{
   frequency = hz;
   return true;
}

// setBandwidth() -- set the bandwidth; must be >= 1
bool RfSystem::setBandwidth(const LCreal hz)
{
   bool ok = false;
   // Bandwidth must be greater than or equal one!
   if (hz >= 1.0f) {
      bandwidth = hz;
      computeReceiverNoise();
      ok = true;
   }
   return ok;
}

// setBandwidthNoise() -- set the bandwidth noise; must be >= 1
bool RfSystem::setBandwidthNoise(const LCreal hz)
{
   bool ok = false;
   // Bandwidth noise must be greater than or equal one!
   if (hz >= 1.0f) {
      bandwidthNoise = hz;
      bwNoiseSet = true;
      computeReceiverNoise();
      ok = true;
   }
   return ok;
}

bool RfSystem::setRfThreshold(const LCreal v)
{
   rfThreshold = v;
   return true;
}

bool RfSystem::setRfTransmitLoss(const LCreal v)
{
   bool ok = false;
   // Transmitter loss must be greater than or equal one!
   if (v >= 1.0f) {
      rfLossXmit = v;
      ok = true;
   }
   return ok;
}

bool RfSystem::setRfReceiveLoss(const LCreal v)
{
   bool ok = false;
   // Receiver loss must be greater than or equal one!
   if (v >= 1.0f) {
      rfLossRecv = v;
      ok = true;
   }
   return ok;
}

bool RfSystem::setRfSignalProcessLoss(const LCreal v)
{
   bool ok = false;
   // Signal processing loss must be greater than or equal one!
   if (v >= 1.0f) {
      rfLossSignalProcess = v;
      ok = true;
   }
   return ok;
}

bool RfSystem::setRfNoiseFigure(const LCreal v)
{
   bool ok = false;
   // Noise figure must be greater than or equal one!
   if (v >= 1.0f) {
      rfNoiseFigure = v;
      computeReceiverNoise();
      ok = true;
   }
   return ok;
}

bool RfSystem::setRfSysTemp(const LCreal v)
{
   bool ok = false;
   // Temperature must be greater than zero!
   if (v >= 1.0f) {
      rfSysTemp = v;
      computeReceiverNoise();
      ok = true;
   }
   return ok;
}

// Sets the receiver noise (Watts)
bool RfSystem::setReceiverNoise(const LCreal v)
{
   bool ok = false;
   // noise must be greater than or equal zero!
   if (v >= 0.0f) {
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
   if (antenna != 0) {
      antenna->unref();
   }
   antenna = p;
   if (antenna != 0) {
      antenna->ref();
   }
   return true;
}

// Compute receiver thermal noise (equation 2-8)
bool RfSystem::computeReceiverNoise()
{
   return setReceiverNoise(rfNoiseFigure * float(BOLTZMANN) * rfSysTemp * getBandwidthNoise());
}

//------------------------------------------------------------------------------
// Slot Functions  (return 'true' if the slot was set, else 'false')
//------------------------------------------------------------------------------

// antennaName: Antenna name  (Basic::String)
bool RfSystem::setSlotAntennaName(Basic::String* const p)
{
   if (antennaName != 0) {
      antennaName->unref();
   }
   antennaName = p;
   if (antennaName != 0) {
      antennaName->ref();
   }
   return true;
}

// setSlotFrequency() -- Set the Frequency (Basic::Number or Basic::Frequency)
bool RfSystem::setSlotFrequency(Basic::Number* const v)
{
    bool ok = false;
    LCreal x = -1.0f;

    const Basic::Frequency* p = dynamic_cast<const Basic::Frequency*>(v);
    if (p != 0) {
        // Has frequency and we need hertz
        x = Basic::Hertz::convertStatic(*p);
    }
    else if (v != 0) {
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
bool RfSystem::setSlotBandwidth(Basic::Number* const num)
{
    bool ok = false;
    if (num != 0) {

        LCreal bw = -1.0f;

        const Basic::Frequency* p = dynamic_cast<const Basic::Frequency*>(num);
        if (p != 0) {
            // Has frequency and we need hertz
            bw = Basic::Hertz::convertStatic(*p);
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
bool RfSystem::setSlotBandwidthNoise(Basic::Number* const num)
{
    bool ok = false;
    if (num != 0) {

        LCreal bw = -1.0f;

        const Basic::Frequency* p = dynamic_cast<const Basic::Frequency*>(num);
        if (p != 0) {
            // Has frequency and we need hertz
            bw = Basic::Hertz::convertStatic(*p);
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

// setSlotPeakPower() -- set the peak power (Basic::Number or Basic::Watts)
bool RfSystem::setSlotPeakPower(Basic::Number* const v)
{
    bool ok = false;
    LCreal x = -1.0f;

    Basic::Power* p = dynamic_cast<Basic::Power*>(v);
    if (p != 0) {
        // Has power units and we need watts
        Basic::Watts watts;
        x = watts.convert(*p);
    }
    else if (v != 0) {
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
bool RfSystem::setSlotRfThreshold(Basic::Decibel* const v)
{
    bool ok = false;
    if (v != 0) {
        ok = setRfThreshold( v->getValueDB() );
    }
    return ok;
}

// setSlotRfNoiseFigure() -- set the noise figure (no units)
bool RfSystem::setSlotRfNoiseFigure(Basic::Number* const v)
{
    bool ok = false;
    if (v != 0) {
        LCreal fig = v->getReal();
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
bool RfSystem::setSlotRfSysTemp(Basic::Number* const v)
{
    bool ok = false;
    if (v != 0) {
        LCreal tmp = v->getReal();
        if (tmp > 0.0) {
            ok = setRfSysTemp( tmp );;
        }
        else {
            std::cerr << "RfSystem::setRfSysTemp: Temperature must be greater than zero!" << std::endl;
        }
    }
    return ok;
}

// setSlotRfTransmitLoss() -- set the trasmit loss
bool RfSystem::setSlotRfTransmitLoss(Basic::Number* const v)
{
    bool ok = false;
    if (v != 0) {
        LCreal loss = v->getReal();
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
bool RfSystem::setSlotRfReceiveLoss(Basic::Number* const v)
{
    bool ok = false;
    if (v != 0) {
        LCreal loss = v->getReal();
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
bool RfSystem::setSlotRfSignalProcessLoss(Basic::Number* const v)
{
    bool ok = false;
    if (v != 0) {
        LCreal loss = v->getReal();
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
bool RfSystem::setSlotDisableEmissions(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setDisableEmissionsFlag( msg->getBoolean() );
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* RfSystem::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& RfSystem::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    //"antennaName",          //  Name of the requested Antenna  (Basic::String)
    if (antennaName != 0) {
        indent(sout,i+j);
        sout << "antenna: " << *antennaName << std::endl;
    }

    //"frequency",            //  Frequency     (Hz; def: 0)        (Basic::Number or LcFrequecy)
    indent(sout,i+j);
    sout << "frequency: ( Hertz " << frequency << std::endl;

    //"bandwidth",            //  Bandwidth     (Hz; def: 0)        (Basic::Number or LcFrequecy)
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

    //"disableEmissions",     //  Disable sending emisison packets flag (default: false)
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

} // End Simulation namespace
} // End Eaagles namespace
