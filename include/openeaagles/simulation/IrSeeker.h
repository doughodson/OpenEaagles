//------------------------------------------------------------------------------
// Classes: IrSeeker, TdbIr
//------------------------------------------------------------------------------
#ifndef __oe_simulation_IrSeeker_H__
#define __oe_simulation_IrSeeker_H__

#include "openeaagles/simulation/ScanGimbal.h"
#include "openeaagles/simulation/Tdb.h"
#include "openeaagles/base/safe_queue.h"
#include "openeaagles/base/safe_stack.h"

//#define USE_TDBIR

namespace oe {
   namespace base { class PairStream; }

namespace simulation {

class Player;
class IrSystem;
class IrSensor;
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrSeeker
//
// Description: Simple IR seeker model
//
// Factory name: IrSeeker
//
//------------------------------------------------------------------------------
class IrSeeker : public ScanGimbal
{
    DECLARE_SUBCLASS(IrSeeker,ScanGimbal)

public:

   IrSeeker();

   virtual void irRequestSignature(IrQueryMsg* const irQuery);
   virtual bool irQueryReturnEvent(IrQueryMsg* const irQuery);

   // System limits
   int getMaxQueries() const                 { return MAX_QUERIES; }

#ifdef USE_TDBIR
   // FAB - was missing, but needed, since IrSeeker uses TdbIr; copied in from v2009_0204
   // Gimbal Interface
   virtual unsigned int processPlayersOfInterest(base::PairStream* const poi);
#endif

   bool event(const int event, base::Object* const obj = nullptr) override;
   void reset() override;

protected:
   void clearQueues();

   void process(const double dt) override;

   bool shutdownNotification() override;

   base::safe_stack<IrQueryMsg*> freeQueryStack;  // stack of free queries of target IR signatures
   mutable long freeQueryLock;                     // Semaphore to protect 'freeQueryStack'

   base::safe_queue<IrQueryMsg*> inUseQueryQueue; // Queue of in use queries of target IR signatures
   mutable long inUseQueryLock;                    // Semaphore to protect 'inUseQueryQueue'

private:
   static const int MAX_QUERIES = 10000;   // Max size of queues and arrays
};

#ifdef USE_TDBIR
//------------------------------------------------------------------------------
// Class: TdbIr
//
// Description: Target(s) Data Block for an IrSeeker (see Tdb in Gimbal.h)
//------------------------------------------------------------------------------
class TdbIr : public Tdb
{
   DECLARE_SUBCLASS(TdbIr,Tdb)

public:
   TdbIr(const unsigned int maxTargets, const Gimbal* const gimbal);

   //------------------------------------------------------------------------------
   // Process players-of-interest --- Scan the provided player list and compute range,
   // range rate, normalized Line-Of-Sight (LOS) vectors for each target player.
   // (Background task)
   //------------------------------------------------------------------------------
   virtual unsigned int processPlayers(base::PairStream* const players);

   static bool horizonCheck(const osg::Vec3& position1, const osg::Vec3& position2);

protected:
   TdbIr();
};
#endif

}
}

#endif
