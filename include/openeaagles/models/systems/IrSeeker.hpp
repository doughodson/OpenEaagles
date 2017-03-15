
#ifndef __oe_models_IrSeeker_H__
#define __oe_models_IrSeeker_H__

#include "openeaagles/models/systems/ScanGimbal.hpp"

#include "openeaagles/models/Tdb.hpp"

#include "openeaagles/base/safe_queue.hpp"
#include "openeaagles/base/safe_stack.hpp"

//#define USE_TDBIR

namespace oe {
namespace base { class PairStream; }
namespace models {
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
//------------------------------------------------------------------------------
class IrSeeker : public ScanGimbal
{
   DECLARE_SUBCLASS(IrSeeker, ScanGimbal)

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

   virtual bool event(const int event, base::Object* const obj = nullptr) override;
   virtual void reset() override;

protected:
   void clearQueues();

   virtual void process(const double dt) override;

   virtual bool shutdownNotification() override;

   base::safe_stack<IrQueryMsg*> freeQueryStack {MAX_QUERIES};   // stack of free queries of target IR signatures
   mutable long freeQueryLock {};                                // Semaphore to protect 'freeQueryStack'

   base::safe_queue<IrQueryMsg*> inUseQueryQueue {MAX_QUERIES};  // Queue of in use queries of target IR signatures
   mutable long inUseQueryLock {};                               // Semaphore to protect 'inUseQueryQueue'

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
   DECLARE_SUBCLASS(TdbIr, Tdb)

public:
   TdbIr() = delete;
   TdbIr(const unsigned int maxTargets, const Gimbal* const gimbal);

   //------------------------------------------------------------------------------
   // Process players-of-interest --- Scan the provided player list and compute range,
   // range rate, normalized Line-Of-Sight (LOS) vectors for each target player.
   // (Background task)
   //------------------------------------------------------------------------------
   virtual unsigned int processPlayers(base::PairStream* const players);

   static bool horizonCheck(const base::Vec3d& position1, const base::Vec3d& position2);
};
#endif

}
}

#endif
