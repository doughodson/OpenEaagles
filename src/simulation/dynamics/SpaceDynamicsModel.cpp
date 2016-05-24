
#include "openeaagles/simulation/dynamics/SpaceDynamicsModel.h"

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(SpaceDynamicsModel, "SpaceDynamicsModel")
EMPTY_SLOTTABLE(SpaceDynamicsModel)
EMPTY_SERIALIZER(SpaceDynamicsModel)
EMPTY_COPYDATA(SpaceDynamicsModel)
EMPTY_DELETEDATA(SpaceDynamicsModel)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SpaceDynamicsModel::SpaceDynamicsModel()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// setControlStickYawInput(yaw) --  Control inputs: normalized
//   yaw:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool SpaceDynamicsModel::setControlStickYawInput(const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setTranslateXStickInput(transx) --  Control inputs: normalized
//   transx:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool SpaceDynamicsModel::setTranslateXInput(const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setTranslateYStickInput(transy) --  Control inputs: normalized
//   transy:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool SpaceDynamicsModel::setTranslateYInput(const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setTranslateZStickInput(transz) --  Control inputs: normalized
//   transz:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool SpaceDynamicsModel::setTranslateZInput(const double)
{
   return false;
}


} // End simulation namespace
}
