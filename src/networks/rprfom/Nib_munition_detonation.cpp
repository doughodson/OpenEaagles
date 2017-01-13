
#include "openeaagles/networks/rprfom/NetIO.hpp"
#include "openeaagles/networks/rprfom/RprFom.hpp"
#include "openeaagles/networks/rprfom/Nib.hpp"
#include "openeaagles/networks/hla/Ambassador.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/players/AbstractWeapon.hpp"

#include "openeaagles/models/Simulation.hpp"

#include "openeaagles/base/Nav.hpp"
#include "openeaagles/base/NetHandler.hpp"

namespace oe {
namespace rprfom {

//------------------------------------------------------------------------------
// Class: Nib
// Description: Portions of class defined to support munition detonation
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// munitionDetonationMsgFactory() -- (Output) Munition detonation message factory
//------------------------------------------------------------------------------
bool Nib::munitionDetonationMsgFactory(const double)
{
   std::cout << "rprfom::Nib::sendMunitionDetonation() HERE!!" << std::endl;

   // Early out -- we must be registered
   if (!isRegistered()) return false;

   NetIO* netIO = static_cast<NetIO*>(getNetIO());

   // Create the parameter/value set
   RTI::ParameterHandleValuePairSet* pParams =
      RTI::ParameterSetFactory::create( NetIO::NUM_INTERACTION_PARAMETER );

   // Set our mode so that we don't do this again.
   setMode(models::Player::DETONATED);

   // If our player just detonated, then it must be a weapon!
   auto mPlayer = dynamic_cast<models::AbstractWeapon*>(getPlayer());
   if (mPlayer == nullptr) return false;   // Early out -- it wasn't a weapon

   // ---
   // Event ID
   // ---
   unsigned short fireEvent = getWeaponFireEvent();
   EventIdentifierStruct eventIdentifier;
   base::NetHandler::toNetOrder(&eventIdentifier.eventCount, fireEvent);
   base::utStrncpy(
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
      base::NetHandler::toNetOrder(&detonationLocation.x, geocPos[base::Nav::IX]);
      base::NetHandler::toNetOrder(&detonationLocation.y, geocPos[base::Nav::IY]);
      base::NetHandler::toNetOrder(&detonationLocation.z, geocPos[base::Nav::IZ]);
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::DETONATION_LOCATION_MD_PI), 
         reinterpret_cast<char*>(&detonationLocation), 
         sizeof(WorldLocationStruct) );

      // Velocity
      VelocityVectorStruct finalVelocityVector; 
      base::NetHandler::toNetOrder(&finalVelocityVector.xVelocity, static_cast<float>(geocVel[base::Nav::IX]));
      base::NetHandler::toNetOrder(&finalVelocityVector.yVelocity, static_cast<float>(geocVel[base::Nav::IY]));
      base::NetHandler::toNetOrder(&finalVelocityVector.zVelocity, static_cast<float>(geocVel[base::Nav::IZ]));
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
      base::utStrncpy(
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
      models::Player* fPlayer = mPlayer->getLaunchVehicle();
      if (fPlayer != nullptr) {
         if (fPlayer->isNetworkedPlayer()) {
            fNib = dynamic_cast<Nib*>( fPlayer->getNib() );
         }
         else {
            fNib = dynamic_cast<Nib*>( netIO->findNib(fPlayer, interop::NetIO::OUTPUT_NIB) );
         }
      }

      if (fNib != nullptr) {
         RTIObjectIdStruct firingObjectIdentifier;
         base::utStrncpy(
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
      models::Player* tPlayer = mPlayer->getTargetPlayer();
      if (tPlayer != nullptr) {
         tNib = dynamic_cast<Nib*>( tPlayer->getNib() );
         if (tNib == nullptr)
            tNib = dynamic_cast<Nib*>( netIO->findNib(tPlayer, interop::NetIO::OUTPUT_NIB) );
      }

      if (tNib != nullptr) {
         RTIObjectIdStruct targetObjectIdentifier;
         base::utStrncpy(
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
      base::NetHandler::toNetOrder(&munitionType.countryCode, getEntityCountry() );
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
      base::NetHandler::toNetOrder(&netBuffer, static_cast<unsigned short>(fuseType) );
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
      base::NetHandler::toNetOrder(&netBuffer, quantityFired );
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
      base::NetHandler::toNetOrder(&netBuffer, rateOfFire );
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
      base::NetHandler::toNetOrder(&netBuffer, static_cast<unsigned short>(warheadType) );
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
      base::NetHandler::toNetOrder(&netBuffer.bodyXDistance, relativeDetonationLocation.bodyXDistance );
      base::NetHandler::toNetOrder(&netBuffer.bodyYDistance, relativeDetonationLocation.bodyYDistance );
      base::NetHandler::toNetOrder(&netBuffer.bodyZDistance, relativeDetonationLocation.bodyZDistance );
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
         case models::AbstractWeapon::DETONATE_OTHER :
            detonationResultCode = DetonationResultCodeOther;
            break;
         case models::AbstractWeapon::DETONATE_ENTITY_IMPACT :
            detonationResultCode = EntityImpact;
            break;
         case models::AbstractWeapon::DETONATE_ENTITY_PROXIMATE_DETONATION :
            detonationResultCode = EntityProximateDetonation;
            break;
         case models::AbstractWeapon::DETONATE_GROUND_IMPACT :
            detonationResultCode = GroundImpact;
            break;
         case models::AbstractWeapon::DETONATE_GROUND_PROXIMATE_DETONATION :
            detonationResultCode = GroundProximateDetonation;
            break;
         case models::AbstractWeapon::DETONATE_DETONATION :
            detonationResultCode = Detonation;
            break;
         case models::AbstractWeapon::DETONATE_NONE :
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

}
}
