
#include "openeaagles/networks/rprfom/NetIO.hpp"
#include "openeaagles/networks/rprfom/RprFom.hpp"
#include "openeaagles/networks/rprfom/Nib.hpp"
#include "openeaagles/networks/hla/Ambassador.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/players/AbstractWeapon.hpp"

#include "openeaagles/base/NetHandler.hpp"

namespace oe {
namespace rprfom {

//------------------------------------------------------------------------------
// Class: NetIO
// Description: Portions of class defined to support weapon fire
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// publishAndSubscribe()
//------------------------------------------------------------------------------
bool NetIO::publishAndSubscribeWeaponFire()
{
   RTI::RTIambassador* p = getRTIambassador();
   bool ok = true;

   // ----------
   // Get handles to the class, attributes and parameters
   // ----------
   try {

      // ---
      // Weapon Fire Interaction class handle and parameter handles
      // ---
      {
         RTI::InteractionClassHandle handle =
            p->getInteractionClassHandle(WeaponFire::getInteractionFedName());

         setInteractionClassHandle(WEAPON_FIRE_INTERACTION, handle );

         setInteractionParameterHandle(
            EVENT_IDENTIFIER_WF_PI,
            p->getParameterHandle(
               WeaponFire::getEventIdentifierParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRE_CONTROL_SOLUTION_RANGE_WF_PI,
            p->getParameterHandle(
               WeaponFire::getFireControlSolutionRangeParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRE_MISSION_INDEX_WF_PI,
            p->getParameterHandle(
               WeaponFire::getFireMissionIndexParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRING_LOCATION_WF_PI,
            p->getParameterHandle(
               WeaponFire::getFiringLocationParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRING_OBJECT_IDENTIFIER_WF_PI,
            p->getParameterHandle(
               WeaponFire::getFiringObjectIdentifierParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FUSE_TYPE_WF_PI,
            p->getParameterHandle(
               WeaponFire::getFuseTypeParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            INITIAL_VELOCITY_VECTOR_WF_PI,
            p->getParameterHandle(
               WeaponFire::getInitialVelocityVectorParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            MUNITION_OBJECT_IDENTIFIER_WF_PI,
            p->getParameterHandle(
               WeaponFire::getMunitionObjectIdentifierParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            MUNITION_TYPE_WF_PI,
            p->getParameterHandle(
               WeaponFire::getMunitionTypeParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            QUANTITY_FIRED_WF_PI,
            p->getParameterHandle(
               WeaponFire::getQuantityFiredParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            RATE_OF_FIRE_WF_PI,
            p->getParameterHandle(
               WeaponFire::getRateOfFireParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            TARGET_OBJECT_IDENTIFIER_WF_PI,
            p->getParameterHandle(
               WeaponFire::getTargetObjectIdentifierParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            WARHEAD_TYPE_WF_PI,
            p->getParameterHandle(
               WeaponFire::getWarheadTypeParameterFedName(),
               handle)
         );
      }


   }
   catch( RTI::Exception& e) {
      std::cerr << &e << std::endl;
      ok = false;
   }


   // ----------
   // Publish & Subscribe to class attributes
   // ----------
   if (ok) {
      try {

         // ---
         // Publish these interactions
         // ---
         if (isOutputEnabled()) {
            p->publishInteractionClass(getInteractionClassHandle(WEAPON_FIRE_INTERACTION));
            setInteractionClassPublished(WEAPON_FIRE_INTERACTION, true);
         }

         // ---
         // Subscribe to these interactions
         // ---
         if (isInputEnabled()) {
            // (Growth: see receiveWeaponFire())
             p->subscribeInteractionClass(getInteractionClassHandle(WEAPON_FIRE_INTERACTION));
             setInteractionClassSubscribed(WEAPON_FIRE_INTERACTION, true);
         }

      }
      catch (RTI::Exception& e) {
         std::cerr << &e << std::endl;
         ok = false;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// receiveWeaponFire() -- (Input support) handle the Weapon Fire interaction
//------------------------------------------------------------------------------
bool NetIO::receiveWeaponFire(const RTI::ParameterHandleValuePairSet& theParameters)
{
    std::cout << "rprfom::Nib::receiveWeaponFire() HERE!!" << std::endl;
    RTI::ULong length;
    char netBuffer[1000];
    for (RTI::ULong i = 0 ; i < theParameters.size(); i++ ) {
        
        // get the parameter's handed and data (network byte order)
        //RTI::ParameterHandle theHandle = theParameters.getHandle(i);
        theParameters.getValue(i, netBuffer, length);

        // Process the parameter
        //switch ( findParameterIndex(theHandle) ) {
        //        
        //}
    }

    // Note:: This is for growth: we're not subscribing to WeaponFire interactions at
    // this time (see publishAndSubscribe() above)

    return true;
}

}
}
