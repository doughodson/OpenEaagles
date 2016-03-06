//------------------------------------------------------------------------------
// Class: Nib
// Description: Portions of class defined to support munition detonation
//------------------------------------------------------------------------------

#include "openeaagles/hla/rprFom/NetIO.h"
#include "openeaagles/hla/rprFom/RprFom.h"
#include "openeaagles/hla/rprFom/Nib.h"
#include "openeaagles/hla/Ambassador.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"

namespace Eaagles {
namespace Network {
namespace Hla {
namespace RprFom {

//------------------------------------------------------------------------------
// munitionDetonationMsgFactory() -- (Output) Munition detonation message factory
//------------------------------------------------------------------------------
bool Nib::munitionDetonationMsgFactory(const LCreal)
{
   std::cout << "RprFom::Nib::sendMunitionDetonation() HERE!!" << std::endl;

   // Early out -- we must be registered
   if (!isRegistered()) return false;

   NetIO* netIO = static_cast<NetIO*>(getNetIO());

   // Create the parameter/value set
   RTI::ParameterHandleValuePairSet* pParams =
      RTI::ParameterSetFactory::create( NetIO::NUM_INTERACTION_PARAMETER );

   // Set our mode so that we don't do this again.
   setMode(Simulation::Player::DETONATED);

   // If our player just detonated, then it must be a weapon!
   Simulation::Weapon* mPlayer = dynamic_cast<Simulation::Weapon*>(getPlayer());
   if (mPlayer == nullptr) return false;   // Early out -- it wasn't a weapon

   // ---
   // Event ID
   // ---
   unsigned short fireEvent = getWeaponFireEvent();
   EventIdentifierStruct eventIdentifier;
   Basic::NetHandler::toNetOrder(&eventIdentifier.eventCount, fireEvent);
   lcStrncpy(
      reinterpret_cast<char*>(&eventIdentifier.issuingObjectIdentifier.id[0]),
      sizeof(eventIdentifier.issuingObjectIdentifier.id),
      getObjectName(),
      RTIObjectIdStruct::ID_SIZE );    
   pParams->add(
      netIO->getInteractionParameterHandle(NetIO::EVENT_IDENTIFIER_MD_PI),
      reinterpret_cast<char*>(&eventIdentifier),
      sizeof(EventIdentifierStruct) );

   // ---
   // Location & Velocity
   // ---
   {
      osg::Vec3d geocPos = getDrPosition();
      osg::Vec3d geocVel = getDrVelocity();
      osg::Vec3d geocAcc = getDrAcceleration();

      // World Coordinates
      WorldLocationStruct detonationLocation;
      Basic::NetHandler::toNetOrder(&detonationLocation.x, geocPos[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&detonationLocation.y, geocPos[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&detonationLocation.z, geocPos[Basic::Nav::IZ]);
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::DETONATION_LOCATION_MD_PI), 
         reinterpret_cast<char*>(&detonationLocation), 
         sizeof(WorldLocationStruct) );

      // Velocity
      VelocityVectorStruct finalVelocityVector; 
      Basic::NetHandler::toNetOrder(&finalVelocityVector.xVelocity, static_cast<float>(geocVel[Basic::Nav::IX]));
      Basic::NetHandler::toNetOrder(&finalVelocityVector.yVelocity, static_cast<float>(geocVel[Basic::Nav::IY]));
      Basic::NetHandler::toNetOrder(&finalVelocityVector.zVelocity, static_cast<float>(geocVel[Basic::Nav::IZ]));
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FINAL_VELOCITY_VECTOR_MD_PI), 
         reinterpret_cast<char*>(&finalVelocityVector), 
         sizeof(VelocityVectorStruct) );
   }

   // ---
   // Munition Object identifiers:
   // ---
   {
      RTIObjectIdStruct munitionObjectIdentifier;
      lcStrncpy(
         reinterpret_cast<char*>(&munitionObjectIdentifier.id[0]),
         sizeof(munitionObjectIdentifier.id),
         getObjectName(),
         RTIObjectIdStruct::ID_SIZE );    
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::MUNITION_OBJECT_IDENTIFIER_MD_PI),
         reinterpret_cast<char*>(&munitionObjectIdentifier),
         sizeof(RTIObjectIdStruct) );
   }

   // ---
   // Firing Object identifier:
   //
   // Get the firing player and its NIB.
   //   First check to see if it's an IPlayer from an HLA network.
   //   If it's not, then check our output list.
   // ---
   {
      Nib* fNib = nullptr;
      Simulation::Player* fPlayer = mPlayer->getLaunchVehicle();
      if (fPlayer != nullptr) {
         if (fPlayer->isNetworkedPlayer()) {
            fNib = dynamic_cast<Nib*>( fPlayer->getNib() );
         }
         else {
            fNib = dynamic_cast<Nib*>( netIO->findNib(fPlayer, Simulation::NetIO::OUTPUT_NIB) );
         }
      }

      if (fNib != nullptr) {
         RTIObjectIdStruct firingObjectIdentifier;
         lcStrncpy(
            reinterpret_cast<char*>(&firingObjectIdentifier.id[0]),
            sizeof(firingObjectIdentifier.id),
            fNib->getObjectName(),
            RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            netIO->getInteractionParameterHandle(NetIO::FIRING_OBJECT_IDENTIFIER_MD_PI),
            reinterpret_cast<char*>(&firingObjectIdentifier),
            sizeof(RTIObjectIdStruct) );
      }
   }

   // ---
   // Target Object identifier:
   //
   // Get the target player and its NIB.
   //   First check to see if it's an IPlayer from an HLA network.
   //   If it's not, then check our output list.
   // ---
   {
      Nib* tNib = nullptr;
      Simulation::Player* tPlayer = mPlayer->getTargetPlayer();
      if (tPlayer != nullptr) {
         tNib = dynamic_cast<Nib*>( tPlayer->getNib() );
         if (tNib == nullptr)
            tNib = dynamic_cast<Nib*>( netIO->findNib(tPlayer, Simulation::NetIO::OUTPUT_NIB) );
      }

      if (tNib != nullptr) {
         RTIObjectIdStruct targetObjectIdentifier;
         lcStrncpy(
            reinterpret_cast<char*>(&targetObjectIdentifier.id[0]),
            sizeof(targetObjectIdentifier.id),
            tNib->getObjectName(),
            RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            netIO->getInteractionParameterHandle(NetIO::TARGET_OBJECT_IDENTIFIER_MD_PI),
            reinterpret_cast<char*>(&targetObjectIdentifier),
            sizeof(RTIObjectIdStruct) );
      }
   }

   // ---
   // Munition Type
   // ---
   {
      EntityTypeStruct munitionType;
      munitionType.entityKind = getEntityKind();
      munitionType.domain = getEntityDomain();
      Basic::NetHandler::toNetOrder(&munitionType.countryCode, getEntityCountry() );
      munitionType.category  = getEntityCategory();
      munitionType.subcategory = getEntitySubcategory();
      munitionType.specific   = getEntitySpecific();
      munitionType.extra  = getEntityExtra();
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::MUNITION_TYPE_MD_PI),
         reinterpret_cast<char*>(&munitionType),
         sizeof(EntityTypeStruct) );
   }

   // ---
   // Fuse Type (16 bit enum)
   // ---
   {
      FuseTypeEnum16 fuseType = FuseTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, static_cast<unsigned short>(fuseType) );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FUSE_TYPE_MD_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(unsigned short) );
   }

   // ---
   // Quantity fired
   // ---
   {
      unsigned short quantityFired = 1;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, quantityFired );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::QUANTITY_FIRED_MD_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(unsigned short) );
   }

   // ---
   // Rate Of Fire
   // ---
   {
      unsigned short rateOfFire = 0;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, rateOfFire );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::RATE_OF_FIRE_MD_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(unsigned short) );
   }

   // ---
   // Warhead type
   // ---
   {
      WarheadTypeEnum16 warheadType = WarheadTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, static_cast<unsigned short>(warheadType) );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::WARHEAD_TYPE_MD_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(unsigned short) );
   }

   // ---
   // Relative detonation location
   // ---
   {
      RelativePositionStruct relativeDetonationLocation;
      relativeDetonationLocation.bodyXDistance = 0;
      relativeDetonationLocation.bodyYDistance = 0;
      relativeDetonationLocation.bodyZDistance = 0;

      RelativePositionStruct netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer.bodyXDistance, relativeDetonationLocation.bodyXDistance );
      Basic::NetHandler::toNetOrder(&netBuffer.bodyYDistance, relativeDetonationLocation.bodyYDistance );
      Basic::NetHandler::toNetOrder(&netBuffer.bodyZDistance, relativeDetonationLocation.bodyZDistance );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::RELATIVE_DETONATION_LOCATION_MD_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(RelativePositionStruct) );
   }

   // ---
   // Detonation result code
   // ---
   {
      DetonationResultCodeEnum8 detonationResultCode;
      switch ( mPlayer->getDetonationResults() ) {
         case Simulation::Weapon::DETONATE_OTHER :
            detonationResultCode = DetonationResultCodeOther;
            break;
         case Simulation::Weapon::DETONATE_ENTITY_IMPACT :
            detonationResultCode = EntityImpact;
            break;
         case Simulation::Weapon::DETONATE_ENTITY_PROXIMATE_DETONATION :
            detonationResultCode = EntityProximateDetonation;
            break;
         case Simulation::Weapon::DETONATE_GROUND_IMPACT :
            detonationResultCode = GroundImpact;
            break;
         case Simulation::Weapon::DETONATE_GROUND_PROXIMATE_DETONATION :
            detonationResultCode = GroundProximateDetonation;
            break;
         case Simulation::Weapon::DETONATE_DETONATION :
            detonationResultCode = Detonation;
            break;
         case Simulation::Weapon::DETONATE_NONE :
            detonationResultCode = None;
            break;
         default :
            detonationResultCode = DetonationResultCodeOther;
            break;
      };

      unsigned char netBuffer = static_cast<unsigned char>(detonationResultCode);
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::DETONATION_RESULT_CODE_MD_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(unsigned char) );
   }

   // ---
   // Send the interaction
   // ---
   bool ok = netIO->sendInteraction(
      netIO->getInteractionClassHandle(NetIO::MUNITION_DETONATION_INTERACTION), pParams );

   // don't need this anymore
   delete pParams;

   return ok;
}

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
