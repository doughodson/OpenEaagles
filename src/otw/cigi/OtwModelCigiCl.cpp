
#include "openeaagles/otw/cigi/OtwModelCigiCl.hpp"
#include "openeaagles/otw/cigi/OtwCigiCl.hpp"

#include "openeaagles/models/player/GroundVehicle.hpp"

#include "cigicl/CigiEntityCtrlV3.h"
#include "cigicl/CigiCompCtrlV3.h"
#include "cigicl/CigiArtPartCtrlV3.h"

namespace oe {
namespace otw {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(OtwModelCigiCl, "OtwModelCigiCl")
EMPTY_SERIALIZER(OtwModelCigiCl)

OtwModelCigiCl::OtwModelCigiCl()
{
   STANDARD_CONSTRUCTOR()
}

void OtwModelCigiCl::copyData(const OtwModelCigiCl& org, const bool)
{
   BaseClass::copyData(org);

   parentActive = false;
   trailActive = false;
   explosionActive = false;
   smokeActive = false;
   damageActive = false;
   animationActive = false;
   attachedEcActive = false;
   launcherApcActive = false;
   attachedCcActive = false;
   isGroundPlayer = false;
   effectsTimer = 0.0;
   id = 0;
}

void OtwModelCigiCl::deleteData()
{
   for (unsigned int i = 0; i < OtwCigiCl::NUM_BUFFERS; i++) {
      if (parentEC[i]    != nullptr) { delete parentEC[i]; parentEC[i] = nullptr;       }
      if (trailEC[i]     != nullptr) { delete trailEC[i]; trailEC[i] = nullptr;         }
      if (explosionEC[i] != nullptr) { delete explosionEC[i]; explosionEC[i] = nullptr; }
      if (smokeEC[i]     != nullptr) { delete smokeEC[i]; smokeEC[i] = nullptr;         }
      if (damageCC[i]    != nullptr) { delete damageCC[i]; damageCC[i] = nullptr;       }
      if (animationCC[i] != nullptr) { delete animationCC[i]; animationCC[i] = nullptr; }
      if (attachedEC[i]  != nullptr) { delete attachedEC[i]; attachedEC[i] = nullptr;   }
      if (launcherAPC[i] != nullptr) { delete launcherAPC[i]; launcherAPC[i] = nullptr; }
      if (attachedCC[i]  != nullptr) { delete attachedCC[i]; attachedCC[i] = nullptr;   }
   }
}

void OtwModelCigiCl::initialize(models::Player* const p)
{
   isGroundPlayer = p->isClassType(typeid(models::GroundVehicle));
   BaseClass::initialize(p);
}

void OtwModelCigiCl::clear()
{
   parentActive = false;
   trailActive = false;
   explosionActive = false;
   smokeActive = false;
   animationActive = false;
   damageActive = false;
   attachedEcActive = false;
   launcherApcActive = false;
   attachedCcActive = false;
   isGroundPlayer = false;

   BaseClass::clear();
}

}
}
