
#include "openeaagles/networks/rprfom/NetIO.hpp"
#include "openeaagles/networks/rprfom/RprFom.hpp"
#include "openeaagles/networks/rprfom/Nib.hpp"
#include "openeaagles/networks/hla/Ambassador.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/players/Weapon.hpp"

#include "openeaagles/simulation/Simulation.hpp"

#include "openeaagles/base/Nav.hpp"
#include "openeaagles/base/NetHandler.hpp"

#include "openeaagles/base/util/str_utils.hpp"

#include <iostream>
#include <cstring>

namespace oe {
namespace rprfom {

//------------------------------------------------------------------------------
// Class: NetIO
// Description: Portions of class defined to support munition detonation
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// publishAndSubscribe()
//------------------------------------------------------------------------------
bool NetIO::publishAndSubscribeMunitionDetonation()
{
   RTI::RTIambassador* p = getRTIambassador();
   bool ok = true;

   // ----------
   // Get handles to the class, attributes and parameters
   // ----------
   try {

      // ---
      // Munition detonation Interaction class handle and parameter handles
      // ---
      {
         RTI::InteractionClassHandle handle =
            p->getInteractionClassHandle(MunitionDetonation::getInteractionFedName());

         setInteractionClassHandle(MUNITION_DETONATION_INTERACTION, handle );

         setInteractionParameterHandle(
            DETONATION_LOCATION_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getDetonationLocationParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            DETONATION_RESULT_CODE_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getDetonationResultCodeParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            EVENT_IDENTIFIER_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getEventIdentifierParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            FIRING_OBJECT_IDENTIFIER_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getFiringObjectIdentifierParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            FINAL_VELOCITY_VECTOR_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getFinalVelocityVectorParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            FUSE_TYPE_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getFuseTypeParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            MUNITION_OBJECT_IDENTIFIER_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getMunitionObjectIdentifierParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            MUNITION_TYPE_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getMunitionTypeParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            QUANTITY_FIRED_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getQuantityFiredParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            RATE_OF_FIRE_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getRateOfFireParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            RELATIVE_DETONATION_LOCATION_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getRelativeDetonationLocationParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            TARGET_OBJECT_IDENTIFIER_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getTargetObjectIdentifierParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            WARHEAD_TYPE_MD_PI,
            p->getParameterHandle(
               MunitionDetonation::getWarheadTypeParameterFedName(),
               handle) );

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
            p->publishInteractionClass(getInteractionClassHandle(MUNITION_DETONATION_INTERACTION));
            setInteractionClassPublished(MUNITION_DETONATION_INTERACTION, true);
         }

         // ---
         // Subscribe to these interactions
         // ---
         if (isInputEnabled()) {
            p->subscribeInteractionClass(getInteractionClassHandle(MUNITION_DETONATION_INTERACTION));
            setInteractionClassSubscribed(MUNITION_DETONATION_INTERACTION, true);
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
// receiveMunitionDetonation() -- (Input) handles the Munition Detonation interaction
//------------------------------------------------------------------------------
bool NetIO::receiveMunitionDetonation(const RTI::ParameterHandleValuePairSet& theParameters)
{
    // Things we need
    RTIObjectIdStruct firingObjectIdentifier;
    RTIObjectIdStruct munitionObjectIdentifier;
    RTIObjectIdStruct targetObjectIdentifier;
    models::Weapon::Detonation detonationResult = models::Weapon::DETONATE_NONE;

    // ---
    // Extract the required data from the interaction's parameters
    // ---
    RTI::ULong length;
    char netBuffer[1000];
    for (RTI::ULong i = 0 ; i < theParameters.size(); i++ ) {
        
        // get the parameter's handed and data (network byte order)
        RTI::ParameterHandle theHandle = theParameters.getHandle(i);
        theParameters.getValue(i, netBuffer, length);

        // Process the parameter
        switch ( findParameterIndex(theHandle) ) {

        case DETONATION_RESULT_CODE_MD_PI : {

            switch ( DetonationResultCodeEnum8( netBuffer[0] ) ) {
                case DetonationResultCodeOther :
                    detonationResult = models::Weapon::DETONATE_OTHER;
                    break;
                case EntityImpact :
                    detonationResult = models::Weapon::DETONATE_ENTITY_IMPACT;
                    break;
                case EntityProximateDetonation :
                    detonationResult = models::Weapon::DETONATE_ENTITY_PROXIMATE_DETONATION;
                    break;
                case GroundImpact :
                    detonationResult = models::Weapon::DETONATE_GROUND_IMPACT;
                    break;
                case GroundProximateDetonation :
                    detonationResult = models::Weapon::DETONATE_GROUND_PROXIMATE_DETONATION;
                    break;
                case Detonation :
                    detonationResult = models::Weapon::DETONATE_DETONATION;
                    break;
                case None :
                    detonationResult = models::Weapon::DETONATE_NONE;
                    break;
                default :
                    detonationResult = models::Weapon::DETONATE_OTHER;
                    break;
            };
        }
        break;
        
        case FIRING_OBJECT_IDENTIFIER_MD_PI : {
            // Get the object's name
            RTI::ULong n = RTIObjectIdStruct::ID_SIZE;
            if (n > length) n = length;
            base::utStrncpy(reinterpret_cast<char*>(&firingObjectIdentifier.id[0]), sizeof(firingObjectIdentifier.id), netBuffer, n);
            firingObjectIdentifier.id[n-1] = '\0';   
        }
        break;
        
        case MUNITION_OBJECT_IDENTIFIER_MD_PI : {
            // Get the object's name
            RTI::ULong n = RTIObjectIdStruct::ID_SIZE;
            if (n > length) n = length;
            base::utStrncpy(reinterpret_cast<char*>(&munitionObjectIdentifier.id[0]), sizeof(munitionObjectIdentifier.id), netBuffer, n);
            munitionObjectIdentifier.id[n-1] = '\0';   
        }
        break;
        
        case TARGET_OBJECT_IDENTIFIER_MD_PI : {
            // Get the object's name
            RTI::ULong n = RTIObjectIdStruct::ID_SIZE;
            if (n > length) n = length;
            base::utStrncpy(reinterpret_cast<char*>(&targetObjectIdentifier.id[0]), sizeof(targetObjectIdentifier.id), netBuffer, n);
            targetObjectIdentifier.id[n-1] = '\0';   
        }
        break;
                        
        }
    }
    std::cout << "RprFom::Nib::receiveMunitionDetonation() fired(" << firingObjectIdentifier.id << ")"  << std::endl;
  
    // ---
    // 1) Find the target (local) player
    // ---
    models::Player* tPlayer = nullptr;
    if ( std::strlen(reinterpret_cast<const char*>(targetObjectIdentifier.id)) > 0 ) {
        interop::Nib* tNib = findNibByObjectName( reinterpret_cast<char*>(targetObjectIdentifier.id), OUTPUT_NIB);
        if (tNib != nullptr) tPlayer = tNib->getPlayer();
    }
    
    // ---
    // Note: we're only interested (at this time) with our local players being hit
    // by other networked IPlayers.
    // ---
    if (tPlayer != nullptr) {
        
        // ---
        // 2) Find the firing player and munitions (networked) IPlayers
        // ---
        interop::Nib* fNib = nullptr;
        interop::Nib* mNib = nullptr;
        if ( std::strlen(reinterpret_cast<const char*>(firingObjectIdentifier.id)) > 0 ) {
            fNib = findNibByObjectName( reinterpret_cast<char*>(firingObjectIdentifier.id), INPUT_NIB);
        }
        if ( std::strlen(reinterpret_cast<const char*>(munitionObjectIdentifier.id)) > 0 ) {
            mNib = findNibByObjectName( reinterpret_cast<char*>(munitionObjectIdentifier.id), INPUT_NIB);
        }

        // ---
        // 3) Tell the target player that it was killed by the firing player
        // ---
        if (detonationResult == models::Weapon::DETONATE_ENTITY_IMPACT) {
            if (fNib != nullptr) {
                tPlayer->event(KILL_EVENT,fNib->getPlayer());
            }
            else {
                tPlayer->event(KILL_EVENT,0);   // killed by we don't know by whom
            }
        }
        
        // ---
        // 4) Update the mode of the munition IPlayer
        // ---
        if (mNib != nullptr) {
            models::Weapon* mPlayer = dynamic_cast<models::Weapon*>(mNib->getPlayer());
            if (mPlayer != nullptr) {
                mPlayer->setMode(models::Player::DETONATED);
                mPlayer->setDetonationResults(detonationResult);
            }
            mNib->setMode(models::Player::DETONATED);
        }
    }

    return true;
}

}
}
