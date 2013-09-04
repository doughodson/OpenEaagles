//------------------------------------------------------------------------------
// Class: NetIO
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
      // Munition detonation Interaction class handle and parmaeter handles
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
    Simulation::Weapon::Detonation detonationResult = Simulation::Weapon::DETONATE_NONE;

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
                    detonationResult = Simulation::Weapon::DETONATE_OTHER;
                    break;
                case EntityImpact :
                    detonationResult = Simulation::Weapon::DETONATE_ENTITY_IMPACT;
                    break;
                case EntityProximateDetonation :
                    detonationResult = Simulation::Weapon::DETONATE_ENTITY_PROXIMATE_DETONATION;
                    break;
                case GroundImpact :
                    detonationResult = Simulation::Weapon::DETONATE_GROUND_IMPACT;
                    break;
                case GroundProximateDetonation :
                    detonationResult = Simulation::Weapon::DETONATE_GROUND_PROXIMATE_DETONATION;
                    break;
                case Detonation :
                    detonationResult = Simulation::Weapon::DETONATE_DETONATION;
                    break;
                case None :
                    detonationResult = Simulation::Weapon::DETONATE_NONE;
                    break;
                default :
                    detonationResult = Simulation::Weapon::DETONATE_OTHER;
                    break;
            };
        }
        break;
        
        case FIRING_OBJECT_IDENTIFIER_MD_PI : {
            // Get the object's name
            RTI::ULong n = RTIObjectIdStruct::ID_SIZE;
            if (n > length) n = length;
            lcStrncpy((char*)&firingObjectIdentifier.id[0], sizeof(firingObjectIdentifier.id), netBuffer, n);
            firingObjectIdentifier.id[n-1] = '\0';   
        }
        break;
        
        case MUNITION_OBJECT_IDENTIFIER_MD_PI : {
            // Get the object's name
            RTI::ULong n = RTIObjectIdStruct::ID_SIZE;
            if (n > length) n = length;
            lcStrncpy((char*)&munitionObjectIdentifier.id[0], sizeof(munitionObjectIdentifier.id), netBuffer, n);
            munitionObjectIdentifier.id[n-1] = '\0';   
        }
        break;
        
        case TARGET_OBJECT_IDENTIFIER_MD_PI : {
            // Get the object's name
            RTI::ULong n = RTIObjectIdStruct::ID_SIZE;
            if (n > length) n = length;
            lcStrncpy((char*)&targetObjectIdentifier.id[0], sizeof(targetObjectIdentifier.id), netBuffer, n);
            targetObjectIdentifier.id[n-1] = '\0';   
        }
        break;
                        
        }
    }
    std::cout << "RprFom::Nib::receiveMunitionDetonation() fired(" << firingObjectIdentifier.id << ")"  << std::endl;
  
    // ---
    // 1) Find the target (local) player
    // ---
    Simulation::Player* tPlayer = 0;
    if ( strlen((const char*)targetObjectIdentifier.id) > 0 ) {
        Simulation::Nib* tNib = findNibByObjectName( (char*)targetObjectIdentifier.id, OUTPUT_NIB);
        if (tNib != 0) tPlayer = tNib->getPlayer();
    }
    
    // ---
    // Note: we're only interested (at this time) with our local players being hit
    // by other networked IPlayers.
    // ---
    if (tPlayer != 0) {
        
        // ---
        // 2) Find the firing player and munitions (networked) IPlayers
        // ---
        Simulation::Nib* fNib = 0;
        Simulation::Nib* mNib = 0;
        if ( strlen((const char*)firingObjectIdentifier.id) > 0 ) {
            fNib = findNibByObjectName( (char*)firingObjectIdentifier.id, INPUT_NIB);
        }
        if ( strlen((const char*)munitionObjectIdentifier.id) > 0 ) {
            mNib = findNibByObjectName( (char*)munitionObjectIdentifier.id, INPUT_NIB);
        }

        // ---
        // 3) Tell the target player that it was killed by the firing player
        // ---
        if (detonationResult == Simulation::Weapon::DETONATE_ENTITY_IMPACT) {
            if (fNib != 0) {
                tPlayer->event(KILL_EVENT,fNib->getPlayer());
            }
            else {
                tPlayer->event(KILL_EVENT,0);   // killed by we don't know by whom
            }
        }
        
        // ---
        // 4) Update the mode of the munition IPlayer
        // ---
        if (mNib != 0) {
            Simulation::Weapon* mPlayer = dynamic_cast<Simulation::Weapon*>(mNib->getPlayer());
            if (mPlayer != 0) {
                mPlayer->setMode(Simulation::Player::DETONATED);
                mPlayer->setDetonationResults(detonationResult);
            }
            mNib->setMode(Simulation::Player::DETONATED);
        }
    }

    return true;
}


//==============================================================================
// Class: Nib
//==============================================================================

//------------------------------------------------------------------------------
// munitionDetonationMsgFactory() -- (Output) Munition detonation message factory
//------------------------------------------------------------------------------
bool Nib::munitionDetonationMsgFactory(const LCreal)
{
   std::cout << "RprFom::Nib::sendMunitionDetonation() HERE!!" << std::endl;

   // Early out -- we must be registered
   if (!isRegistered()) return false;

   NetIO* netIO = (NetIO*)(getNetIO());

   // Create the parameter/value set
   RTI::ParameterHandleValuePairSet* pParams =
      RTI::ParameterSetFactory::create( NetIO::NUM_INTERACTION_PARAMETER );

   // Set our mode so that we don't do this again.
   setMode(Simulation::Player::DETONATED);

   // If our player just detonated, then it must be a weapon!
   Simulation::Weapon* mPlayer = dynamic_cast<Simulation::Weapon*>(getPlayer());
   if (mPlayer == 0) return false;   // Early out -- it wasn't a weapon

   // ---
   // Event ID
   // ---
   unsigned short fireEvent = getWeaponFireEvent();
   EventIdentifierStruct eventIdentifier;
   Basic::NetHandler::toNetOrder(&eventIdentifier.eventCount, fireEvent);
   lcStrncpy(
      (char*)&eventIdentifier.issuingObjectIdentifier.id[0],
      sizeof(eventIdentifier.issuingObjectIdentifier.id),
      getObjectName(),
      RTIObjectIdStruct::ID_SIZE );    
   pParams->add(
      netIO->getInteractionParameterHandle(NetIO::EVENT_IDENTIFIER_MD_PI),
      (char*) &eventIdentifier,
      sizeof(EventIdentifierStruct) );

   // ---
   // Location & Velociy
   // ---
   {
      double refLat = 0.0;
      double refLon = 0.0;
      if (netIO->getSimulation() != 0) {
         refLat = netIO->getSimulation()->getRefLatitude();
         refLon = netIO->getSimulation()->getRefLongitude();
      }

      // Convert position vector to Lat/Lon/Alt
      double alt;
      double simCoord[3] = { 0, 0, 0 };
      Basic::Nav::convertPosVec2LL(
            refLat, refLon, 
            getPosition(),
            &simCoord[Basic::Nav::ILAT], &simCoord[Basic::Nav::ILON], &alt );
      simCoord[Basic::Nav::IALT] = alt;

      // Get the geodetic velocity and acceleration
      osg::Vec3 vel = getVelocity();
      LCreal geodVel[3] = { 0, 0, 0 };

      // Convert to geocentric coordinates
      double geocPos[3] = { 0, 0, 0 };
      LCreal geocVel[3] = { 0, 0, 0 };
      LCreal geocAcc[3] = { 0, 0, 0 };
      Basic::Nav::getWorldPosAccVel(simCoord, vel.ptr(), geodVel, geocPos, geocVel, geocAcc);

      // World Coordinates
      WorldLocationStruct detonationLocation;
      Basic::NetHandler::toNetOrder(&detonationLocation.x, geocPos[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&detonationLocation.y, geocPos[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&detonationLocation.z, geocPos[Basic::Nav::IZ]);
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::DETONATION_LOCATION_MD_PI), 
         (char*) &detonationLocation, 
         sizeof(WorldLocationStruct) );

      // Velocity
      VelocityVectorStruct finalVelocityVector; 
      Basic::NetHandler::toNetOrder(&finalVelocityVector.xVelocity, geocVel[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&finalVelocityVector.yVelocity, geocVel[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&finalVelocityVector.zVelocity, geocVel[Basic::Nav::IZ]);
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FINAL_VELOCITY_VECTOR_MD_PI), 
         (char*) &finalVelocityVector, 
         sizeof(VelocityVectorStruct) );
   }


   // ---
   // Munition Object identfiers:
   // ---
   {
      RTIObjectIdStruct munitionObjectIdentifier;
      lcStrncpy(
         (char*)&munitionObjectIdentifier.id[0],
         sizeof(munitionObjectIdentifier.id),
         getObjectName(),
         RTIObjectIdStruct::ID_SIZE );    
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::MUNITION_OBJECT_IDENTIFIER_MD_PI),
         (char*) &munitionObjectIdentifier,
         sizeof(RTIObjectIdStruct) );
   }

   // ---
   // Firing Object identfier:
   //
   // Get the firing player and its NIB.
   //   First check to see if it's an IPlayer from an HLA network.
   //   If it's not, then check our output list.
   // ---
   {
      Nib* fNib = 0;
      Simulation::Player* fPlayer = mPlayer->getLaunchVehicle();
      if (fPlayer != 0) {
         if (fPlayer->isNetworkedPlayer()) {
            fNib = dynamic_cast<Nib*>( fPlayer->getNib() );
         }
         else {
            fNib = dynamic_cast<Nib*>( netIO->findNib(fPlayer, Simulation::NetIO::OUTPUT_NIB) );
         }
      }

      if (fNib != 0) {
         RTIObjectIdStruct firingObjectIdentifier;
         lcStrncpy(
            (char*)&firingObjectIdentifier.id[0],
            sizeof(firingObjectIdentifier.id),
            fNib->getObjectName(),
            RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            netIO->getInteractionParameterHandle(NetIO::FIRING_OBJECT_IDENTIFIER_MD_PI),
            (char*) &firingObjectIdentifier,
            sizeof(RTIObjectIdStruct) );
      }
   }

   // ---
   // Target Object identfier:
   //
   // Get the target player and its NIB.
   //   First check to see if it's an IPlayer from an HLA network.
   //   If it's not, then check our output list.
   // ---
   {
      Nib* tNib = 0;
      Simulation::Player* tPlayer = mPlayer->getTargetPlayer();
      if (tPlayer != 0) {
         tNib = dynamic_cast<Nib*>( tPlayer->getNib() );
         if (tNib == 0)
            tNib = dynamic_cast<Nib*>( netIO->findNib(tPlayer, Simulation::NetIO::OUTPUT_NIB) );
      }

      if (tNib != 0) {
         RTIObjectIdStruct targetObjectIdentifier;
         lcStrncpy(
            (char*)&targetObjectIdentifier.id[0],
            sizeof(targetObjectIdentifier.id),
            tNib->getObjectName(),
            RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            netIO->getInteractionParameterHandle(NetIO::TARGET_OBJECT_IDENTIFIER_MD_PI),
            (char*) &targetObjectIdentifier,
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
         (char*) &munitionType,
         sizeof(EntityTypeStruct) );
   }

   // ---
   // Fuse Type (16 bit enum)
   // ---
   {
      FuseTypeEnum16 fuseType = FuseTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, (unsigned short)(fuseType) );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FUSE_TYPE_MD_PI),
         (char*) &netBuffer,
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
         (char*) &netBuffer,
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
         (char*) &netBuffer,
         sizeof(unsigned short) );
   }

   // ---
   // Warhead type
   // ---
   {
      WarheadTypeEnum16 warheadType = WarheadTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, (unsigned short)(warheadType) );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::WARHEAD_TYPE_MD_PI),
         (char*) &netBuffer,
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
         (char*) &netBuffer,
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

      unsigned char netBuffer = (unsigned char)(detonationResultCode);
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::DETONATION_RESULT_CODE_MD_PI),
         (char*) &netBuffer,
         sizeof(unsigned char) );
   }

   // ---
   // Send the interaction
   // ---
   bool ok = netIO->sendInteraction(
      netIO->getInteractionClassHandle(NetIO::MUNITION_DETONATION_INTERACTION),
      pParams );

   // don't need this anymore
   delete pParams;

   return ok;
}

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
