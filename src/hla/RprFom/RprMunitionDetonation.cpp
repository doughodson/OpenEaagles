#include "eaagles/hla/rprFom/RprFomIO.h"
#include "eaagles/hla/rprFom/RprFom.h"
#include "eaagles/hla/rprFom/RprFomNib.h"
#include "eaagles/hla/Ambassador.h"

#include "eaagles/simulation/Player.h"
#include "eaagles/simulation/Simulation.h"
#include "eaagles/simulation/Weapon.h"
#include "eaagles/basic/Nav.h"
#include "eaagles/basic/NetHandler.h"

//==============================================================================
// Class: HlaIO
//==============================================================================
namespace Eaagles {
namespace Network {
namespace Hla {
namespace RprFom {

//------------------------------------------------------------------------------
// publishAndSubscribe()
//------------------------------------------------------------------------------
bool RprFomIO::publishAndSubscribeMunitionDetonation()
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
            p->getInteractionClassHandle(RprFom::MunitionDetonation::getInteractionFedName());

         setInteractionClassHandle(MUNITION_DETONATION_INTERACTION, handle );

         setInteractionParameterHandle(
            DETONATION_LOCATION_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getDetonationLocationParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            DETONATION_RESULT_CODE_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getDetonationResultCodeParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            EVENT_IDENTIFIER_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getEventIdentifierParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            FIRING_OBJECT_IDENTIFIER_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getFiringObjectIdentifierParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            FINAL_VELOCITY_VECTOR_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getFinalVelocityVectorParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            FUSE_TYPE_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getFuseTypeParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            MUNITION_OBJECT_IDENTIFIER_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getMunitionObjectIdentifierParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            MUNITION_TYPE_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getMunitionTypeParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            QUANTITY_FIRED_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getQuantityFiredParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            RATE_OF_FIRE_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getRateOfFireParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            RELATIVE_DETONATION_LOCATION_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getRelativeDetonationLocationParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            TARGET_OBJECT_IDENTIFIER_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getTargetObjectIdentifierParameterFedName(),
               handle) );

         setInteractionParameterHandle(
            WARHEAD_TYPE_MD_PI,
            p->getParameterHandle(
               RprFom::MunitionDetonation::getWarheadTypeParameterFedName(),
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
bool RprFomIO::receiveMunitionDetonation(const RTI::ParameterHandleValuePairSet& theParameters)
{
    // Things we need
    RprFom::RTIObjectIdStruct firingObjectIdentifier;
    RprFom::RTIObjectIdStruct munitionObjectIdentifier;
    RprFom::RTIObjectIdStruct targetObjectIdentifier;
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

            switch ( RprFom::DetonationResultCodeEnum8( netBuffer[0] ) ) {
                case RprFom::DetonationResultCodeOther :
                    detonationResult = Simulation::Weapon::DETONATE_OTHER;
                    break;
                case RprFom::EntityImpact :
                    detonationResult = Simulation::Weapon::DETONATE_ENTITY_IMPACT;
                    break;
                case RprFom::EntityProximateDetonation :
                    detonationResult = Simulation::Weapon::DETONATE_ENTITY_PROXIMATE_DETONATION;
                    break;
                case RprFom::GroundImpact :
                    detonationResult = Simulation::Weapon::DETONATE_GROUND_IMPACT;
                    break;
                case RprFom::GroundProximateDetonation :
                    detonationResult = Simulation::Weapon::DETONATE_GROUND_PROXIMATE_DETONATION;
                    break;
                case RprFom::Detonation :
                    detonationResult = Simulation::Weapon::DETONATE_DETONATION;
                    break;
                case RprFom::None :
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
            RTI::ULong n = RprFom::RTIObjectIdStruct::ID_SIZE;
            if (n > length) n = length;
            strncpy((char*)&firingObjectIdentifier.id[0], netBuffer, n);
            firingObjectIdentifier.id[n-1] = '\0';   
        }
        break;
        
        case MUNITION_OBJECT_IDENTIFIER_MD_PI : {
            // Get the object's name
            RTI::ULong n = RprFom::RTIObjectIdStruct::ID_SIZE;
            if (n > length) n = length;
            strncpy((char*)&munitionObjectIdentifier.id[0], netBuffer, n);
            munitionObjectIdentifier.id[n-1] = '\0';   
        }
        break;
        
        case TARGET_OBJECT_IDENTIFIER_MD_PI : {
            // Get the object's name
            RTI::ULong n = RprFom::RTIObjectIdStruct::ID_SIZE;
            if (n > length) n = length;
            strncpy((char*)&targetObjectIdentifier.id[0], netBuffer, n);
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
// Class: RprFom::Nib
//==============================================================================

//------------------------------------------------------------------------------
// munitionDetonationMsgFactory() -- (Output) Munition detonation message factory
//------------------------------------------------------------------------------
bool RprFom::Nib::munitionDetonationMsgFactory(const LCreal)
{
   std::cout << "RprFom::Nib::sendMunitionDetonation() HERE!!" << std::endl;

   // Early out -- we must be registered
   if (!isRegistered()) return false;

   RprFomIO* rprIO = (RprFomIO*)(getNetworkIO());

   // Create the parameter/value set
   RTI::ParameterHandleValuePairSet* pParams =
      RTI::ParameterSetFactory::create( RprFomIO::NUM_INTERACTION_PARAMETER );

   // Set our mode so that we don't do this again.
   setMode(Simulation::Player::DETONATED);

   // If our player just detonated, then it must be a weapon!
   Simulation::Weapon* mPlayer = dynamic_cast<Simulation::Weapon*>(getPlayer());
   if (mPlayer == 0) return false;   // Early out -- it wasn't a weapon

   // ---
   // Event ID
   // ---
   unsigned short fireEvent = getWeaponFireEvent();
   RprFom::EventIdentifierStruct eventIdentifier;
   Basic::NetHandler::toNetOrder(&eventIdentifier.eventCount, fireEvent);
   strncpy(
      (char*)&eventIdentifier.issuingObjectIdentifier.id[0],
      getObjectName(),
      RprFom::RTIObjectIdStruct::ID_SIZE );    
   pParams->add(
      rprIO->getInteractionParameterHandle(RprFomIO::EVENT_IDENTIFIER_MD_PI),
      (char*) &eventIdentifier,
      sizeof(RprFom::EventIdentifierStruct) );

   // ---
   // Location & Velociy
   // ---
   {
      double refLat = 0.0;
      double refLon = 0.0;
      if (rprIO->getSimulation() != 0) {
         refLat = rprIO->getSimulation()->getRefLatitude();
         refLon = rprIO->getSimulation()->getRefLongitude();
      }

      // Convert position vector to Lat/Lon/Alt
      LCreal alt;
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
      RprFom::WorldLocationStruct detonationLocation;
      Basic::NetHandler::toNetOrder(&detonationLocation.x, geocPos[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&detonationLocation.y, geocPos[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&detonationLocation.z, geocPos[Basic::Nav::IZ]);
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::DETONATION_LOCATION_MD_PI), 
         (char*) &detonationLocation, 
         sizeof(RprFom::WorldLocationStruct) );

      // Velocity
      RprFom::VelocityVectorStruct finalVelocityVector; 
      Basic::NetHandler::toNetOrder(&finalVelocityVector.xVelocity, geocVel[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&finalVelocityVector.yVelocity, geocVel[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&finalVelocityVector.zVelocity, geocVel[Basic::Nav::IZ]);
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::FINAL_VELOCITY_VECTOR_MD_PI), 
         (char*) &finalVelocityVector, 
         sizeof(RprFom::VelocityVectorStruct) );
   }


   // ---
   // Munition Object identfiers:
   // ---
   {
      RprFom::RTIObjectIdStruct munitionObjectIdentifier;
      strncpy(
         (char*)&munitionObjectIdentifier.id[0],
         getObjectName(),
         RprFom::RTIObjectIdStruct::ID_SIZE );    
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::MUNITION_OBJECT_IDENTIFIER_MD_PI),
         (char*) &munitionObjectIdentifier,
         sizeof(RprFom::RTIObjectIdStruct) );
   }

   // ---
   // Firing Object identfier:
   //
   // Get the firing player and its NIB.
   //   First check to see if it's an IPlayer from an HLA network.
   //   If it's not, then check our output list.
   // ---
   {
      RprFom::Nib* fNib = 0;
      Simulation::Player* fPlayer = mPlayer->getLauncher();
      if (fPlayer != 0) {
         if (fPlayer->isNetworkedPlayer()) {
            fNib = dynamic_cast<RprFom::Nib*>( fPlayer->getNib() );
         }
         else {
            fNib = dynamic_cast<RprFom::Nib*>( rprIO->findNib(fPlayer, Simulation::NetworkIO::OUTPUT_NIB) );
         }
      }

      if (fNib != 0) {
         RprFom::RTIObjectIdStruct firingObjectIdentifier;
         strncpy(
            (char*)&firingObjectIdentifier.id[0],
            fNib->getObjectName(),
            RprFom::RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            rprIO->getInteractionParameterHandle(RprFomIO::FIRING_OBJECT_IDENTIFIER_MD_PI),
            (char*) &firingObjectIdentifier,
            sizeof(RprFom::RTIObjectIdStruct) );
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
      RprFom::Nib* tNib = 0;
      Simulation::Player* tPlayer = mPlayer->getTargetPlayer();
      if (tPlayer != 0) {
         tNib = dynamic_cast<RprFom::Nib*>( tPlayer->getNib() );
         if (tNib == 0)
            tNib = dynamic_cast<RprFom::Nib*>( rprIO->findNib(tPlayer, Simulation::NetworkIO::OUTPUT_NIB) );
      }

      if (tNib != 0) {
         RprFom::RTIObjectIdStruct targetObjectIdentifier;
         strncpy(
            (char*)&targetObjectIdentifier.id[0],
            tNib->getObjectName(),
            RprFom::RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            rprIO->getInteractionParameterHandle(RprFomIO::TARGET_OBJECT_IDENTIFIER_MD_PI),
            (char*) &targetObjectIdentifier,
            sizeof(RprFom::RTIObjectIdStruct) );
      }
   }

    ---
    Munition Type
    ---
   {
      RprFom::EntityTypeStruct munitionType;
      munitionType.entityKind = getKind();
      munitionType.domain = getDomain();
      Basic::NetHandler::toNetOrder(&munitionType.countryCode, getCountry() );
      munitionType.category  = getCategory();
      munitionType.subcategory = getSubcategory();
      munitionType.specific   = getSpecific();
      munitionType.extra  = getExtra();
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::MUNITION_TYPE_MD_PI),
         (char*) &munitionType,
         sizeof(RprFom::EntityTypeStruct) );
   }

   // ---
   // Fuse Type (16 bit enum)
   // ---
   {
      RprFom::FuseTypeEnum16 fuseType = RprFom::FuseTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, (unsigned short)(fuseType) );
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::FUSE_TYPE_MD_PI),
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
         rprIO->getInteractionParameterHandle(RprFomIO::QUANTITY_FIRED_MD_PI),
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
         rprIO->getInteractionParameterHandle(RprFomIO::RATE_OF_FIRE_MD_PI),
         (char*) &netBuffer,
         sizeof(unsigned short) );
   }

   // ---
   // Warhead type
   // ---
   {
      RprFom::WarheadTypeEnum16 warheadType = RprFom::WarheadTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, (unsigned short)(warheadType) );
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::WARHEAD_TYPE_MD_PI),
         (char*) &netBuffer,
         sizeof(unsigned short) );
   }

   // ---
   // Relative detonation location
   // ---
   {
      RprFom::RelativePositionStruct relativeDetonationLocation;
      relativeDetonationLocation.bodyXDistance = 0;
      relativeDetonationLocation.bodyYDistance = 0;
      relativeDetonationLocation.bodyZDistance = 0;

      RprFom::RelativePositionStruct netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer.bodyXDistance, relativeDetonationLocation.bodyXDistance );
      Basic::NetHandler::toNetOrder(&netBuffer.bodyYDistance, relativeDetonationLocation.bodyYDistance );
      Basic::NetHandler::toNetOrder(&netBuffer.bodyZDistance, relativeDetonationLocation.bodyZDistance );
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::RELATIVE_DETONATION_LOCATION_MD_PI),
         (char*) &netBuffer,
         sizeof(RprFom::RelativePositionStruct) );
   }

   // ---
   // Detonation result code
   // ---
   {
      RprFom::DetonationResultCodeEnum8 detonationResultCode;
      switch ( mPlayer->getDetonationResults() ) {
         case Simulation::Weapon::DETONATE_OTHER :
            detonationResultCode = RprFom::DetonationResultCodeOther;
            break;
         case Simulation::Weapon::DETONATE_ENTITY_IMPACT :
            detonationResultCode = RprFom::EntityImpact;
            break;
         case Simulation::Weapon::DETONATE_ENTITY_PROXIMATE_DETONATION :
            detonationResultCode = RprFom::EntityProximateDetonation;
            break;
         case Simulation::Weapon::DETONATE_GROUND_IMPACT :
            detonationResultCode = RprFom::GroundImpact;
            break;
         case Simulation::Weapon::DETONATE_GROUND_PROXIMATE_DETONATION :
            detonationResultCode = RprFom::GroundProximateDetonation;
            break;
         case Simulation::Weapon::DETONATE_DETONATION :
            detonationResultCode = RprFom::Detonation;
            break;
         case Simulation::Weapon::DETONATE_NONE :
            detonationResultCode = RprFom::None;
            break;
         default :
            detonationResultCode = RprFom::DetonationResultCodeOther;
            break;
      };

      unsigned char netBuffer = (unsigned char)(detonationResultCode);
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::DETONATION_RESULT_CODE_MD_PI),
         (char*) &netBuffer,
         sizeof(unsigned char) );
   }

   // ---
   // Send the interaction
   // ---
   bool ok = rprIO->sendInteraction(
      rprIO->getInteractionClassHandle(RprFomIO::MUNITION_DETONATION_INTERACTION),
      pParams );

   // don't need this anymore
   delete pParams;

   return ok;
}

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
