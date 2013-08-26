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
bool RprFomIO::publishAndSubscribeWeaponFire()
{
   RTI::RTIambassador* p = getRTIambassador();
   bool ok = true;

   // ----------
   // Get handles to the class, attributes and parameters
   // ----------
   try {

      // ---
      // Weapon Fire Interaction class handle and parmaeter handles
      // ---
      {
         RTI::InteractionClassHandle handle =
            p->getInteractionClassHandle(RprFom::WeaponFire::getInteractionFedName());

         setInteractionClassHandle(WEAPON_FIRE_INTERACTION, handle );

         setInteractionParameterHandle(
            EVENT_ID_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getEventIDParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRE_CONTROL_SOLUTION_RANGE_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getFireControlSolutionRangeParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRE_MISSION_INDEX_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getFireMissionIndexParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRING_LOCATION_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getFiringLocationParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRING_OBJECT_ID_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getFiringObjectIDParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FUSE_TYPE_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getFuseTypeParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            INITIAL_VELOCITY_VECTOR_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getInitialVelocityVectorParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            MUNITION_OBJECT_ID_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getMunitionObjectIDParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            MUNITION_TYPE_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getMunitionTypeParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            QUANTITY_FIRED_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getQuantityFiredParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            RATE_OF_FIRE_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getRateOfFireParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            TARGET_OBJECT_ID_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getTargetObjectIDParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            WARHEAD_TYPE_WF_PI,
            p->getParameterHandle(
               RprFom::WeaponFire::getWarheadTypeParameterFedName(),
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
bool RprFomIO::receiveWeaponFire(const RTI::ParameterHandleValuePairSet& theParameters)
{
   std::cout << "RprFom::Nib::receiveWeaponFire() HERE!!" << std::endl;
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


//==============================================================================
// Class: RprFom::Nib
//==============================================================================

//------------------------------------------------------------------------------
// weaponFireMsgFactory() -- (Output support) Weapon fire message factory
//------------------------------------------------------------------------------
bool RprFom::Nib::weaponFireMsgFactory(const LCreal)
{
   std::cout << "RprFom::Nib::sendWeaponFire() HERE!!" << std::endl;

   // Early out -- we must be registered
   if (!isRegistered()) return false;

   // Get our Simulation::NetworkIO
   RprFomIO* rprIO = (RprFomIO*)(getNetworkIO());

   // Create the parameter/value set
   RTI::ParameterHandleValuePairSet* pParams =
      RTI::ParameterSetFactory::create( RprFomIO::NUM_INTERACTION_PARAMETER );

   // Set our mode so that we don't do this again.
   setMode(Simulation::Player::ACTIVE);

   // If our player just launched, then it must be a weapon!
   Simulation::Weapon* mPlayer = dynamic_cast<Simulation::Weapon*>(getPlayer());
   if (mPlayer == 0) return false;  // Early out -- it wasn't a weapon! 

   // ---
   // Event ID
   // ---
   unsigned short fireEvent = mPlayer->getReleaseEventID();
   RprFom::EventIDStruct eventID;
   Basic::NetHandler::toNetOrder(&eventID.eventCount, fireEvent);
   strncpy(
      (char*)&eventID.issuingObjectID.id[0], 
      getObjectName(), 
      RprFom::RTIObjectIdStruct::ID_SIZE );    
   pParams->add(
      rprIO->getInteractionParameterHandle(RprFomIO::EVENT_ID_WF_PI),
      (char*) &eventID,
      sizeof(RprFom::EventIDStruct) );
   setWeaponFireEvent( fireEvent );  // remember for the detonation interaction

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
            &simCoord[Basic::Nav::ILAT], &simCoord[Basic::Nav::ILON], &alt);
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
      RprFom::PositionStruct firingLocation;
      Basic::NetHandler::toNetOrder(&firingLocation.x, geocPos[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&firingLocation.y, geocPos[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&firingLocation.z, geocPos[Basic::Nav::IZ]);
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::FIRING_LOCATION_WF_PI), 
         (char*) &firingLocation, 
         sizeof(RprFom::PositionStruct) );

      // Velocity
      RprFom::VelocityStruct initialVelocityVector; 
      Basic::NetHandler::toNetOrder(&initialVelocityVector.xVelocity, geocVel[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&initialVelocityVector.yVelocity, geocVel[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&initialVelocityVector.zVelocity, geocVel[Basic::Nav::IZ]);
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::INITIAL_VELOCITY_VECTOR_WF_PI), 
         (char*) &initialVelocityVector, 
         sizeof(RprFom::VelocityStruct) );
   }

   // ---
   // Munition Object identfiers:
   // ---
   {
      RprFom::RTIObjectIdStruct munitionObjectID;
      strncpy(
         (char*)&munitionObjectID.id[0],
         getObjectName(),
         RprFom::RTIObjectIdStruct::ID_SIZE );    
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::MUNITION_OBJECT_ID_WF_PI),
         (char*) &munitionObjectID,
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
         RprFom::RTIObjectIdStruct firingObjectID;
         strncpy(
            (char*)&firingObjectID.id[0],
            fNib->getObjectName(),
            RprFom::RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            rprIO->getInteractionParameterHandle(RprFomIO::FIRING_OBJECT_ID_WF_PI),
            (char*) &firingObjectID,
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
         if (tNib == 0) tNib = dynamic_cast<RprFom::Nib*>( rprIO->findNib(tPlayer, Simulation::NetworkIO::OUTPUT_NIB) );
      }

      if (tNib != 0) {
         RprFom::RTIObjectIdStruct targetObjectID;
         strncpy(
            (char*)&targetObjectID.id[0],
            tNib->getObjectName(),
            RprFom::RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            rprIO->getInteractionParameterHandle(RprFomIO::TARGET_OBJECT_ID_WF_PI),
            (char*) &targetObjectID,
            sizeof(RprFom::RTIObjectIdStruct) );
      }
   }

   // ---
   // Munition Type
   // ---
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
         rprIO->getInteractionParameterHandle(RprFomIO::MUNITION_TYPE_WF_PI),
         (char*) &munitionType,
         sizeof(RprFom::EntityTypeStruct) );
   }

   // ---
   // Fire Control Solution Range (meters)
   // ---
   {
      float fireControlSolutionRange;
      Basic::NetHandler::toNetOrder(&fireControlSolutionRange, 0 );
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::FIRE_CONTROL_SOLUTION_RANGE_WF_PI),
         (char*) &fireControlSolutionRange,
         sizeof(float) );
   }

   // ---
   // Fire Mission Index
   // ---
   {
      unsigned long fireMissionIndex = 0;
      unsigned long netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, fireMissionIndex );
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::FIRE_MISSION_INDEX_WF_PI),
         (char*) &netBuffer,
         sizeof(unsigned long) );
   }

   // ---
   // Fuse Type (16 bit enum)
   // ---
   {
      RprFom::FuseTypeEnum fuseType = RprFom::FuseTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, (unsigned short)(fuseType) );
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::FUSE_TYPE_WF_PI),
         (char*) &netBuffer,
         sizeof(FuseTypeEnum) );
   }

   // ---
   // Quantity fired
   // ---
   {
      unsigned short quantityFired = 1;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, quantityFired );
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::QUANTITY_FIRED_WF_PI),
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
         rprIO->getInteractionParameterHandle(RprFomIO::RATE_OF_FIRE_WF_PI),
         (char*) &netBuffer,
         sizeof(unsigned short) );
   }

   // ---
   // Warhead type
   // ---
   {
      RprFom::WarheadTypeEnum warheadType = RprFom::WarheadTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, (unsigned short)(warheadType) );
      pParams->add(
         rprIO->getInteractionParameterHandle(RprFomIO::WARHEAD_TYPE_WF_PI),
         (char*) &netBuffer,
         sizeof(WarheadTypeEnum) );
   }

   // ---
   // Send the interaction
   // ---
   bool ok = rprIO->sendInteraction(
         rprIO->getInteractionClassHandle(RprFomIO::WEAPON_FIRE_INTERACTION), 
         pParams );

   // don't need this anymore
   delete pParams;

   return ok;
}

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
