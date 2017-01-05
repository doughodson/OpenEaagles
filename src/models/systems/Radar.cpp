
#include "openeaagles/models/systems/Radar.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/systems/Antenna.hpp"
#include "openeaagles/models/systems/TrackManager.hpp"
#include "openeaagles/models/Emission.hpp"
#include "openeaagles/models/Simulation.hpp"

#include "openeaagles/base/Integer.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Distances.hpp"

#include <cmath>

namespace oe {
namespace models {

IMPLEMENT_PARTIAL_SUBCLASS(Radar, "Radar")

// Slot table
BEGIN_SLOTTABLE(Radar)
   "igain",    //  1: RF: Integrator gain (dB or no units; def: 1.0)
END_SLOTTABLE(Radar)

//  Map slot table
BEGIN_SLOT_MAP(Radar)
    ON_SLOT(1,  setSlotIGain, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator & clone()
//------------------------------------------------------------------------------
Radar::Radar() :
      myLock(0), rptQueue(MAX_EMISSIONS), rptSnQueue(MAX_EMISSIONS)
{
   STANDARD_CONSTRUCTOR()

   initData();

   setTransmitterEnableFlag(true);
   setReceiverEnabledFlag(true);
   setTypeId("RADAR");
}

Radar::Radar(const Radar& org) :
      myLock(0), rptQueue(MAX_EMISSIONS), rptSnQueue(MAX_EMISSIONS)
{
    STANDARD_CONSTRUCTOR()
    copyData(org,true);
}

Radar::~Radar()
{
   STANDARD_DESTRUCTOR()
}

Radar& Radar::operator=(const Radar& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

Radar* Radar::clone() const
{
    return new Radar(*this);
}

void Radar::initData()
{
   for (unsigned int i = 0; i < MAX_REPORTS; i++) {
      reports[i] = nullptr;
      rptMaxSn[i] = 0;
   }
   numReports = 0;

   endOfScanFlg = false;

   for (unsigned int i = 0; i < NUM_SWEEPS; i++) clearSweep(i);
   csweep = 0;

   currentJamSignal = 0.0;
   numberOfJammedEmissions = 0;

   rfIGain = 1.0;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Radar::copyData(const Radar& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // ---
   // ### Clear out the reports and queues
   // ### and don't copy any data
   // ---
   clearTracksAndQueues();
   endOfScanFlg = false;

   for (unsigned int i = 0; i < NUM_SWEEPS; i++) clearSweep(i);
   csweep = 0;

   currentJamSignal = org.currentJamSignal;
   numberOfJammedEmissions = org.numberOfJammedEmissions;

   rfIGain = org.rfIGain;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Radar::deleteData()
{
   clearTracksAndQueues();
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool Radar::shutdownNotification()
{
   clearTracksAndQueues();

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// clearTracksAndQueues() -- clear out tracks and queues
//------------------------------------------------------------------------------
void Radar::clearTracksAndQueues()
{
   // Clear reports
   base::lock(myLock);
   for (unsigned int i = 0; i < numReports && i < MAX_REPORTS; i++) {
      if (reports[i] != nullptr) {
         reports[i]->unref();
         reports[i] = nullptr;
      }
   }
   numReports = 0;
   base::unlock(myLock);

   // ---
   // Clear out the queues
   // ---
   base::lock(myLock);
   for (Emission* em = rptQueue.get(); em != nullptr; em = rptQueue.get()) { em->unref(); }
   while (rptSnQueue.isNotEmpty()) { rptSnQueue.get(); }
   base::unlock(myLock);
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void Radar::updateData(const double dt)
{
   ageSweeps();

   BaseClass::updateData(dt);
}


//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Radar::reset()
{
   BaseClass::reset();
   clearTracksAndQueues();
}

//------------------------------------------------------------------------------
// Set function
//------------------------------------------------------------------------------

// Sets integration gain
bool Radar::setIGain(const double g)
{
   rfIGain = g;
   return true;
}

//------------------------------------------------------------------------------
// transmit() -- send radar emissions
//------------------------------------------------------------------------------
void Radar::transmit(const double dt)
{
   BaseClass::transmit(dt);

   // Transmitting, scanning and have an antenna?
   if ( !areEmissionsDisabled() && isTransmitting() ) {
      // Send the emission to the other player
      Emission* em = new Emission();
      em->setFrequency(getFrequency());
      em->setBandwidth(getBandwidth());
      const double prf1 = getPRF();
      em->setPRF(prf1);
      int pulses = static_cast<int>(prf1 * dt + 0.5);
      if (pulses == 0) pulses = 1; // at least one
      em->setPulses(pulses);
      const double p = getPeakPower();
      em->setPower(p);
      em->setMaxRangeNM(getRange());
      em->setPulseWidth(getPulseWidth());
      em->setTransmitLoss(getRfTransmitLoss());
      em->setReturnRequest( isReceiverEnabled() );
      em->setTransmitter(this);
      getAntenna()->rfTransmit(em);
      em->unref();
   }

}

//------------------------------------------------------------------------------
// receive() -- process received emissions
//------------------------------------------------------------------------------
void Radar::receive(const double dt)
{
   BaseClass::receive(dt);

   // Can't do anything without an antenna
   if (getAntenna() == nullptr) return;

   // Clear the next sweep
   csweep = computeSweepIndex( static_cast<double>(base::Angle::R2DCC * getAntenna()->getAzimuth()) );
   clearSweep(csweep);

   // Compute noise level
   // CGB moved here from RfSystem
   // Basically, we're simulation Hannen's S/I equation from page 356 of his notes.
   // Where I is N + J. J is noise from jamming.
   // Receiver Loss affects the total I, so we have to wait until this point to account for it.
   const double interference = (getRfRecvNoise() + jamSignal) * getRfReceiveLoss();
   const double noise = getRfRecvNoise() * getRfReceiveLoss();
   currentJamSignal = jamSignal * getRfReceiveLoss();
   int countNumJammedEm = 0;

   // ---
   // Process Returned Emissions
   // ---

   Emission* em = nullptr;
   double signal = 0;

   // Get an emission from the queue
   base::lock(packetLock);
   if (np > 0) {
      np--; // Decrement 'np', now the array index
      em = packets[np];
      signal = signals[np];
   }
   base::unlock(packetLock);

   while (em != nullptr) {

      // exclude noise jammers (accounted for already in RfSystem::rfReceivedEmission)
      if (em->getTransmitter() == this || (em->isECM() && !em->isECMType(Emission::ECM_NOISE)) ) {

         // compute the return trip loss ...

         // Compute signal received
         double rcs = em->getRCS();

         // Signal Equation (Equation 2-7)
         double rl = em->getRangeLoss();
         signal *= (rcs * rl);

         // Integration gain
         signal *= rfIGain;

         // Range attenuation: we don't want the strong signal from short range targets
         double maxRng = getRange() * base::Distance::NM2M;
         //double maxRng4 = (maxRng*maxRng*maxRng*maxRng);
         //double rng = (em->getRange());

         const double s1 = 1.0;
         //if (rng > 0) {
         //    double rng4 = (rng*rng*rng*rng);
         //    s1 = (rng4/maxRng4);
         //    if (s1 > 1.0f) s1 = 1.0f;
         //}
         signal *= s1;

         if (signal > 0.0) {

            // Signal/Noise  (Equation 2-9)
            const double signalToInterferenceRatio = signal / interference;
            const double signalToInterferenceRatioDbl = 10.0f * std::log10(signalToInterferenceRatio);
            const double signalToNoiseRatio = signal / noise;
            const double signalToNoiseRatioDbl = 10.0f * std::log10(signalToNoiseRatio);

            //std::cout << "Radar::receive(" << em->getTarget() << "): ";
            //std::cout << " pwr=" << em->getPower();
            //std::cout << " gain=" << em->getGain();
            //std::cout << " rl=" << rl;
            //std::cout << " rcs=" << rcs;
            //std::cout << " signal=" << signal;
            //std::cout << " recvN=" << getRfRecvNoise();
            //std::cout << " signalToInterferenceRatio=" << signalToInterferenceRatio;
            //std::cout << " signalToInterferenceRatioDbl=" << signalToInterferenceRatioDbl;
            //std::cout << " thrs=" << getRfThreshold();
            //std::cout << std::endl;

            // Is S/N above receiver threshold and within 125% of max range?
            // CGB, if "signal <= 0.0", then "signalToInterferenceRatioDbl" is probably invalid
            // we should probably do something smart with "signalToInterferenceRatioDbl" above as well.
            base::lock(myLock);
            if (signalToInterferenceRatioDbl >= getRfThreshold() && em->getRange() <= (maxRng*1.25) && rptQueue.isNotFull()) {

               // send the report to the track manager
               em->ref();
               rptQueue.put(em);
               rptSnQueue.put(signalToInterferenceRatioDbl);

               //std::cout << " (" << em->getRange() << ", " << signalToInterferenceRatioDbl << ", " << signalToInterferenceRatio << ", " << signalToInterferenceRatioDbl << ")";

               // Save signal for real-beam display
               int iaz = csweep;
               int irng = computeRangeIndex( em->getRange() );
               sweeps[iaz][irng] += (signalToInterferenceRatioDbl/100.0f);
               vclos[iaz][irng] = em->getRangeRate();

            } else if (signalToInterferenceRatioDbl < getRfThreshold() && signalToNoiseRatioDbl >= getRfThreshold()) {
               countNumJammedEm++;
            }
            base::unlock(myLock);
         }
      }

      em->unref();   // this unref() undoes the ref() done by RfSystem::rfReceivedEmission
      em = nullptr;

      //if (np >= 0 && np < MAX_EMISSIONS) {
      //    packets[np] = 0;
      //    signals[np] = 0;
      //}

      // Get another emission from the queue
      base::lock(packetLock);
      if (np > 0) {
         np--;
         em = packets[np];
         signal = signals[np];
      }
      base::unlock(packetLock);
   }
   //std::cout << std::endl;

   numberOfJammedEmissions = countNumJammedEm;

   // Set interference signal back to zero
   jamSignal = 0;
}

//------------------------------------------------------------------------------
// process() -- process the TWS reports
//------------------------------------------------------------------------------
void Radar::process(const double dt)
{
   BaseClass::process(dt);

   // Find the track manager
   TrackManager* tm = getTrackManager();
   if (tm == nullptr) {
      // No track manager! Then just flush the input queue.
      base::lock(myLock);
      for (Emission* em = rptQueue.get(); em != nullptr; em = rptQueue.get()) {
         em->unref();
         rptSnQueue.get();
      }
      base::unlock(myLock);
   }

   // ---
   // When end of scan, send all unsent reports to the track manager
   // ---
   if (endOfScanFlg) {

      endOfScanFlg = false;

      base::lock(myLock);
      for (unsigned int i = 0; i < numReports && i < MAX_REPORTS; i++) {
         if (tm != nullptr) {
            tm->newReport(reports[i], rptMaxSn[i]);
         }
         reports[i]->unref();
         reports[i] = nullptr;
         rptMaxSn[i] = 0;
      }
      numReports = 0;
      base::unlock(myLock);
   }


   // ---
   // Process our returned emissions into reports for the track manager
   //   1) Match each emission with existing reports
   //   2) On emission/report matches, if the S/N value of the new emission
   //      is greater than the report, use the new emission
   //   3) Create new reports for unmatched emissions
   // ---
   base::lock(myLock);
   while (rptQueue.isNotEmpty()) {

      // Get the emission
      Emission* em = rptQueue.get();
      double snDbl = rptSnQueue.get();

      if (em != nullptr) {
         // ---
         // 1) Match the emission with existing reports
         // ---
         int matched = -1;
         for (unsigned int i = 0; i < numReports && matched < 0; i++) {
            // Compare targets
            if ( em->getTarget() == reports[i]->getTarget() ) {
               // We have a match!!!
               matched = i;
            }
         }

         // ---
         // 2) On emission/report match
         // ---
         if (matched >= 0) {
            if (snDbl > rptMaxSn[matched]) {
               // When the S/N value of the new emission is greater than the report,
               // we use the new emission
               reports[matched]->unref();
               em->ref();
               reports[matched] = em;
               rptMaxSn[matched] = snDbl;
            }
         }


         // ---
         // 3) Create a new report entry for the unmatched emission
         // ---

         if (matched < 0 && numReports < MAX_REPORTS) {
            em->ref();
            reports[numReports] = em;
            rptMaxSn[numReports] = snDbl;
            numReports++;
         }
         // finished
         em->unref();
      }
   }
   base::unlock(myLock);
}


//------------------------------------------------------------------------------
// getReports() -- returns a list of prereferenced pointers to emission reports
//------------------------------------------------------------------------------
unsigned int Radar::getReports(const Emission** list, const unsigned int max) const
{
   unsigned int num = 0;
   if (list != nullptr && max > 0 && numReports > 0) {
      base::lock(myLock);
      num = numReports;
      if (num > max) num = max;
      for (unsigned int i = 0; i < num; i++) {
         reports[i]->ref();
         list[i] = reports[i];
      }
      base::unlock(myLock);
   }
   return num;
}


//------------------------------------------------------------------------------
// killedNotification() -- We were just killed by player 'p'
//------------------------------------------------------------------------------
bool Radar::killedNotification(Player* const p)
{
   clearTracksAndQueues();

   // ---
   // Make sure our base class knows we're dead.
   // ---
   BaseClass::killedNotification(p);
   return true;
}

//------------------------------------------------------------------------------
// onEndScanEvent() -- process the end of a scan
//------------------------------------------------------------------------------
bool Radar::onEndScanEvent(const base::Integer* const bar)
{
   endOfScanFlg = true;
   BaseClass::onEndScanEvent(bar);
   return true;
}

//------------------------------------------------------------------------------
// clearSweep -- Computer power at angle off center of beam
//------------------------------------------------------------------------------
void Radar::clearSweep(const unsigned int n)
{
   if (n < NUM_SWEEPS) {
      for (unsigned int i = 0; i < PTRS_PER_SWEEP; i++) {
         sweeps[n][i] = 0.0;
         vclos[n][i] = 0.0;
      }
   }
}

//------------------------------------------------------------------------------
// ageSweeps -- age the power in the sweeps
//------------------------------------------------------------------------------
void Radar::ageSweeps()
{
   const double aging = 0.002;
   for (unsigned int i = 0; i < NUM_SWEEPS; i++) {
      for (unsigned int j = 0; j < PTRS_PER_SWEEP; j++) {
         double p = sweeps[i][j];
         if (p > 0) {
            p -= aging;
            if (p < 0) p = 0;
            sweeps[i][j] = p;
         }
      }
   }
}

//------------------------------------------------------------------------------
// computeSweepIndex -- compute the sweep index
//------------------------------------------------------------------------------
unsigned int Radar::computeSweepIndex(const double az)
{
   double s = static_cast<double>(NUM_SWEEPS-1)/60.0;      // sweeps per display scaling

   double az1 = az + 30.0;                                 // Offset from left side (sweep 0)
   int n = static_cast<int>(az1*s + 0.5);                  // Compute index
   if (n >= NUM_SWEEPS) n = NUM_SWEEPS - 1;
   if (n < 0) n = 0;
   return static_cast<unsigned int>(n);
}

//------------------------------------------------------------------------------
// computeRangeIndex -- compute the range index
//------------------------------------------------------------------------------
unsigned int Radar::computeRangeIndex(const double rng)
{
   // range must be positive, if not, return return an index of 0
   if (rng < 0) return 0;

   //double maxRng = 40000.0;
   double maxRng = getRange() * base::Distance::NM2M;
   double rng1 = (rng/ maxRng );
   unsigned int n = static_cast<unsigned int>(rng1 * static_cast<double>(PTRS_PER_SWEEP) + 0.5);
   if (n >= PTRS_PER_SWEEP) n = PTRS_PER_SWEEP - 1;
   return n;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// igain: Integrator gain (dB or no units; def: 1.0)
bool Radar::setSlotIGain(base::Number* const v)
{
   bool ok = false;
   if (v != nullptr) {
      double g = v->getReal();
      if (g >= 1.0) {
         ok = setIGain(g);
      }
      else {
         std::cerr << "RfSystem::setSlotIGain: gain must be greater than or equal to one (i.e., 0db)" << std::endl;
      }
   }
   return ok;
}

std::ostream& Radar::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   if (rfIGain > 1.0) {
      indent(sout,i+j);
      sout << "igain: " << rfIGain << std::endl;
   }

   // DPG #### Need to print slots!!!
   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

}
}
