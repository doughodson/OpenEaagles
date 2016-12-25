
#ifndef __oe_models_SpaceDynamicsModel_H__
#define __oe_models_SpaceDynamicsModel_H__

#include "openeaagles/models/dynamics/DynamicsModel.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
class SpaceDynamicsModel : public DynamicsModel
{
   DECLARE_SUBCLASS(SpaceDynamicsModel, DynamicsModel)

public:
   SpaceDynamicsModel();

   // Rotation Stick Input Positions (Roll and Pitch in BaseClass)
   virtual bool setControlStickYawInput(const double yaw);

   // Translate Stick Input Positions
   virtual bool setTranslateXInput(const double transx);
   virtual bool setTranslateYInput(const double transy);
   virtual bool setTranslateZInput(const double transz);
};

}
}

#endif
