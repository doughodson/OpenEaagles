

#include "openeaagles/ubf/behaviors/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/ubf/behaviors/PlaneState.h"
#include "openeaagles/ubf/behaviors/PlaneBehaviors.h"
#include "openeaagles/ubf/behaviors/PriorityArbiter.h"

#include <string>

namespace oe {
namespace behaviors {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    if ( name == PlaneState::getFactoryName() ) {
        obj = new PlaneState();
    }

    else if ( name == PriorityArbiter::getFactoryName() ) {
        obj = new PriorityArbiter();
    }

    else if ( name == PlaneFire::getFactoryName() ) {
        obj = new PlaneFire();
    }
    else if ( name == PlaneFlyStraight::getFactoryName() ) {
        obj = new PlaneFlyStraight();
    }
    else if ( name == PlaneFollowEnemy::getFactoryName() ) {
        obj = new PlaneFollowEnemy();
    }

    else if ( name == PlaneTurn::getFactoryName() ) {
        obj = new PlaneTurn();
    }
    else if ( name == PlaneSlowTurn::getFactoryName() ) {
        obj = new PlaneSlowTurn();
    }
    else if ( name == PlaneClimb::getFactoryName() ) {
        obj = new PlaneClimb();
    }
    else if ( name == PlaneDive::getFactoryName() ) {
        obj = new PlaneDive();
    }

    else if ( name == PlaneTrim::getFactoryName() ) {
        obj = new PlaneTrim();
    }
    else if ( name == PlaneRoll::getFactoryName() ) {
        obj = new PlaneRoll();
    }
    else if ( name == PlaneBarrelRoll::getFactoryName() ) {
        obj = new PlaneBarrelRoll();
    }
    else if ( name == PlaneLoop::getFactoryName() ) {
        obj = new PlaneLoop();
    }

    return obj;
}

}
}
