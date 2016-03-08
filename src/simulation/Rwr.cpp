#include "openeaagles/simulation/Rwr.h"

#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/TrackManager.h"

#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/simulation/Player.h"

#include <cmath>

namespace oe {
namespace simulation {

IMPLEMENT_PARTIAL_SUBCLASS(Rwr,"Rwr")
EMPTY_SLOTTABLE(Rwr)

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator & clone()
//------------------------------------------------------------------------------
Rwr::Rwr() : rptQueue(MAX_EMISSIONS)
{
   STANDARD_CONSTRUCTOR()

   initData();
   setReceiverEnabledFlag(true);
}

Rwr::Rwr(const Rwr& org) : rptQueue(MAX_EMISSIONS)
{
    STANDARD_CONSTRUCTOR()
    copyData(org,true);
}

Rwr::~Rwr()
{
   STANDARD_DESTRUCTOR()
}

Rwr& Rwr::operator=(const Rwr& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

Rwr* Rwr::clone() const
{
    return new Rwr(*this);
}

void Rwr::initData()
{
   clearRays(0);
   clearRays(1);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Rwr::copyData(const Rwr& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Rwr::deleteData()
{
   // Clear out the queues
   for (Emission* em = rptQueue.get(); em != nullptr; em = rptQueue.get()) { em->unref(); }
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool Rwr::shutdownNotification()
{
   // Clear out the queues
   for (Emission* em = rptQueue.get(); em != nullptr; em = rptQueue.get()) { em->unref(); }
   return BaseClass::shutdownNotification();
}


//------------------------------------------------------------------------------
// receive() -- process received emissions
//------------------------------------------------------------------------------
void Rwr::receive(const double dt)
{
   BaseClass::receive(dt);

   // clear the back buffer
   clearRays(0);

   // Receiver losses
#if 0
   const double noise = getRfRecvNoise();
#else
   const double noise = getRfRecvNoise() * getRfReceiveLoss();
#endif

   // Process received emissions
   TrackManager* tm = getTrackManager();
   Emission* em = nullptr;
   double signal = 0;

   // Get an emission from the queue
   base::lcLock(packetLock);
   if (np > 0) {
      np--; // Decrement 'np', now the array index
      em = packets[np];
      signal = signals[np];
   }
   base::lcUnlock(packetLock);

   while (em != nullptr) {

      //std::cout << "Rwr::receive(" << em->getOwnship() << "): ";
      //std::cout << " pwr=" << em->getPower();
      //std::cout << " gain=" << em->getGain();
      //std::cout << " rl=" << rl;
      //std::cout << " pulses=" << pulses;
      //std::cout << " losses=" << losses;
      //std::cout << " signal=" << signal;
      //std::cout << " recvN=" << getRfRecvNoise();
      //std::cout << " sn=" << sn;
      //std::cout << " snDbl=" << snDbl;
      //std::cout << " thrs=" << getRfThreshold();
      //std::cout << std::endl;

      // CGB, if "signal <= 0.0", then "snDbl" is probably invalid
      if (signal > 0.0 && dt != 0.0) {

         // Signal over noise (equation 3-5)
         const double sn = signal / noise;
         const double snDbl = 10.0 * std::log10(sn);

         // Is S/N above receiver threshold  ## dpg -- for now, don't include ECM emissions
         if (snDbl > getRfThreshold() && !em->isECM() && rptQueue.isNotFull()) {
            // Send report to the track manager
            if (tm != nullptr) {
               tm->newReport(em, snDbl);
            }

            // Get Angle Of Arrival
            const double aoa= em->getAzimuthAoi();

            // Store received power for real-beam display
            const double sigDbl = 10.0f * std::log10(signal);
            const double signal10 = (sigDbl + 50.0f)/50.f;
            const int idx = getRayIndex( static_cast<double>(base::Angle::R2DCC * aoa) );
            rays[0][idx] = base::lim01(rays[0][idx] + signal10);
            //if (idx == 0 && getOwnship()->getID() == 1011) {
            //   std::cout << "sig = " << signal10 << std::endl;
            //}

            // Send to the track list processor
            em->ref();  // ref() for track list processing
            rptQueue.put(em);
         }
      }

      // finished
      em->unref();   // this unref() undoes the ref() done by RfSystem::rfReceivedEmission
      em = nullptr;


      // Get another emission from the queue
      base::lcLock(packetLock);
      if (np > 0) {
         np--;
         em = packets[np];
         signal = signals[np];
      }
      base::lcUnlock(packetLock);

   }

   // Transfer the rays
   xferRays();
}

//------------------------------------------------------------------------------
// process() -- process the reports
//------------------------------------------------------------------------------
void Rwr::process(const double dt)
{
   BaseClass::process(dt);

   // ---
   // Process Emissions into tracks
   // ---
   for (Emission* em = rptQueue.get(); em != nullptr; em = rptQueue.get()) {
      // finished
      em->unref();   // this undoes the ref() added in Rwr::receive()
   }
}

//------------------------------------------------------------------------------
// killedNotification() -- We were just killed by player 'p'
//------------------------------------------------------------------------------
bool Rwr::killedNotification(Player* const p)
{
    // ---
    // Clear out the queues
    // ---
    for (Emission* em = rptQueue.get(); em != nullptr; em = rptQueue.get()) { em->unref(); }

    // ---
    // Make sure our base class knows we're dead.
    // ---
    BaseClass::killedNotification(p);
    return true;
}

//------------------------------------------------------------------------------
// getRayIndex() --
//------------------------------------------------------------------------------
int Rwr::getRayIndex(const double az) const
{
    double az1 = base::Angle::aepcdDeg(az);
    if (az1 < 0.0) az1 += 360.0;
    int idx = static_cast<int>( (az1/ getDegreesPerRay()) + 0.5 );
    if (idx >= NUM_RAYS || idx < 0) idx = 0;
    return idx;
}

//------------------------------------------------------------------------------
// getRayAzimuth() --
//------------------------------------------------------------------------------
double Rwr::getRayAzimuth(const int idx) const
{
    const double az = getDegreesPerRay() * static_cast<double>(idx);
    return base::Angle::aepcdDeg(az);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Rwr::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End simulation namespace
} // End oe namespace
