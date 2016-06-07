//------------------------------------------------------------------------------
// Classes: PlaneBehaviorBase
//          PlaneBehaviorBase -> PlaneFire
//          PlaneBehaviorBase -> PlaneFlyStraight
//          PlaneBehaviorBase -> PlaneFollowEnemy
//          PlaneBehaviorBase -> PlaneTurn
//          PlaneBehaviorBase -> PlaneSlowTurn
//          PlaneBehaviorBase -> PlaneClimb
//          PlaneBehaviorBase -> PlaneDive
//          PlaneBehaviorBase -> PlaneTrim
//          PlaneBehaviorBase -> PlaneRoll
//          PlaneBehaviorBase -> PlaneBarrelRoll
//          PlaneBehaviorBase -> PlaneLoop
//------------------------------------------------------------------------------
#ifndef __oe_behaviors_PlaneBehaviors_H__
#define __oe_behaviors_PlaneBehaviors_H__

#include "openeaagles/base/ubf/Behavior.h"

namespace oe {

namespace base { class Distance; class State; }

namespace behaviors {

//------------------------------------------------------------------------------
// test code for a base class for PlaneBehaviors, implements some common slots
//------------------------------------------------------------------------------
class PlaneBehaviorBase : public base::ubf::Behavior
{
  DECLARE_SUBCLASS(PlaneBehaviorBase, base::ubf::Behavior)
public:
   PlaneBehaviorBase();

protected:
   bool setSlotCriticalAltitude(const base::Distance* const msg);
   bool setSlotVoteOnCriticalAltitude(const base::Number* const num);
   bool setSlotVoteOnIncomingMissile(const base::Number* const num);

   unsigned int voteOnIncomingMissile;
   unsigned int voteOnCriticalAltitude;
   double criticalAltitude;
};


//------------------------------------------------------------------------------
// Class: PlaneFire
//
// Description: Behavior class that shoots missiles at enemy planes
//------------------------------------------------------------------------------
class PlaneFire : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneFire, PlaneBehaviorBase)
public:
   PlaneFire();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
protected:
   bool setSlotMaxDistance(const base::Distance* const msg);
private:
   double maxDistance;
};

class PlaneFlyStraight : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneFlyStraight, PlaneBehaviorBase)
public:
   PlaneFlyStraight();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
private:
   double holdingAltitude;
};

class PlaneFollowEnemy : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneFollowEnemy, PlaneBehaviorBase)
public:
   PlaneFollowEnemy();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
};

class PlaneTurn : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneTurn, PlaneBehaviorBase)
public:
   PlaneTurn();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
private:
   //double prevAlt;
};

class PlaneSlowTurn : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneSlowTurn, PlaneBehaviorBase)
public:
   PlaneSlowTurn();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
private:
   //double prevAlt;
};

class PlaneClimb : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneClimb, PlaneBehaviorBase)
public:
   PlaneClimb();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
private:
};

class PlaneDive : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneDive, PlaneBehaviorBase)
public:
   PlaneDive();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
private:
};

class PlaneTrim : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneTrim, PlaneBehaviorBase)
public:
   PlaneTrim();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
private:
   double holdingAltitude;
   int count;
};

class PlaneRoll : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneRoll, PlaneBehaviorBase)
public:
   PlaneRoll();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
private:
};

class PlaneBarrelRoll : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneBarrelRoll, PlaneBehaviorBase)
public:
   PlaneBarrelRoll();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
private:
};


class PlaneLoop : public PlaneBehaviorBase
{
   DECLARE_SUBCLASS(PlaneLoop, PlaneBehaviorBase)
public:
   PlaneLoop();
   virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;
private:
   bool keepGoing;
};

}
}

#endif

