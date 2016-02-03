//------------------------------------------------------------------------------
// Class: SpaceDynamicsModel
//------------------------------------------------------------------------------
#ifndef __oe_simulation_SpaceDynamicsModel_H__
#define __oe_simulation_SpaceDynamicsModel_H__

#include "openeaagles/simulation/DynamicsModel.h"

namespace oe {
namespace simulation {

//==============================================================================
// Class SpaceDynamicsModel
// Description:  External player dynamics model
//
// Note --
//    1) updateTC() and updateData() are called by Component::updateTC() and
//    Component::updateData() as part of the player's component list.
//
//    2) computer model dynamics in DynamicsModel::dynamics(), which is called
//    from Player::dynamics() to be synchronized.
//
// Factory name: SpaceDynamicsModel
//==============================================================================
class SpaceDynamicsModel : public DynamicsModel
{
   DECLARE_SUBCLASS(SpaceDynamicsModel, DynamicsModel)

public:
   SpaceDynamicsModel();

   // Rotation Stick Input Positions (Roll and Pitch in BaseClass)
   virtual bool setControlStickYawInput(const LCreal yaw);

   // Translate Stick Input Positions
   virtual bool setTranslateXInput(const LCreal transx);
   virtual bool setTranslateYInput(const LCreal transy);
   virtual bool setTranslateZInput(const LCreal transz);
};

} // End simulation namespace
} // End oe namespace

#endif
