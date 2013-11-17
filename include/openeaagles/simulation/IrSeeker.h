//------------------------------------------------------------------------------
// Classes: IrSeeker, TdbIr
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_IrSeeker_H__
#define __Eaagles_Simulation_IrSeeker_H__

#include "openeaagles/simulation/ScanGimbal.h"
#include "openeaagles/simulation/Tdb.h"

//#define USE_TDBIR

namespace Eaagles {
   namespace Basic { class PairStream; }

namespace Simulation {

class Player;
class IrSystem;
class IrSensor; 
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrSeeker
//
// Description: Simple IR seeker model
//
// Form name: IrSeeker
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
   virtual unsigned int processPlayersOfInterest(Basic::PairStream* const poi);
#endif

   // Component Interface
   virtual bool event(const int event, Basic::Object* const obj = 0);
   virtual void reset();

protected:
   void clearQueues();

   // System class Interface -- phase() callbacks
   virtual void process(const LCreal dt);     // Phase 3

   QStack<IrQueryMsg*> freeQueryStack;  // stack of free queries of target IR signatures
   mutable long        freeQueryLock;   // Semaphore to protect 'freeQueryStack'

   QQueue<IrQueryMsg*> inUseQueryQueue; // Queue of in use queries of target IR signatures 
   mutable long        inUseQueryLock;  // Semaphore to protect 'inUseQueryQueue'

private:
   static const int MAX_QUERIES = 10000;   // Max size of queues and arrays
};

#ifdef USE_TDBIR
//------------------------------------------------------------------------------
// Class: TdbIr
// Base class: Basic::Object -> Tdb -> TdbIr
//
// Description: Target(s) Data Block for an IrSeeker (see Tdb in Gimbal.h)
//
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
   virtual unsigned int processPlayers(Basic::PairStream* const players);

   static bool horizonCheck(const osg::Vec3& position1, const osg::Vec3& position2); 

protected:
   TdbIr();
};
#endif

} // End Simulation namespace
} // End Eaagles namespace

#endif
