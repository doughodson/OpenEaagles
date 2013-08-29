#include "openeaagles/hla/rprFom/NetIO.h"
#include "openeaagles/hla/rprFom/RprFom.h"
#include "openeaagles/hla/rprFom/Nib.h"
#include "openeaagles/hla/Ambassador.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"

//==============================================================================
// Class: NetIO
//==============================================================================
namespace Eaagles {
namespace Network {
namespace Hla {
namespace RprFom {

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
      // Weapon Fire Interaction class handle and parmaeter handles
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
// Class: Nib
//==============================================================================

//------------------------------------------------------------------------------
// weaponFireMsgFactory() -- (Output support) Weapon fire message factory
//------------------------------------------------------------------------------
bool Nib::weaponFireMsgFactory(const LCreal)
{
   std::cout << "RprFom::Nib::sendWeaponFire() HERE!!" << std::endl;

   // Early out -- we must be registered
   if (!isRegistered()) return false;

   // Get our Simulation::NetIO
   NetIO* netIO = (NetIO*)(getNetIO());

   // Create the parameter/value set
   RTI::ParameterHandleValuePairSet* pParams =
      RTI::ParameterSetFactory::create( NetIO::NUM_INTERACTION_PARAMETER );

   // Set our mode so that we don't do this again.
   setMode(Simulation::Player::ACTIVE);

   // If our player just launched, then it must be a weapon!
   Simulation::Weapon* mPlayer = dynamic_cast<Simulation::Weapon*>(getPlayer());
   if (mPlayer == 0) return false;  // Early out -- it wasn't a weapon! 

   // ---
   // Event ID
   // ---
   unsigned short fireEvent = mPlayer->getReleaseEventID();
   EventIdentifierStruct eventIdentifier;
   Basic::NetHandler::toNetOrder(&eventIdentifier.eventCount, fireEvent);
   lcStrncpy(
      (char*)&eventIdentifier.issuingObjectIdentifier.id[0], 
      sizeof(eventIdentifier.issuingObjectIdentifier.id),
      getObjectName(), 
      RTIObjectIdStruct::ID_SIZE );    
   pParams->add(
      netIO->getInteractionParameterHandle(NetIO::EVENT_IDENTIFIER_WF_PI),
      (char*) &eventIdentifier,
      sizeof(EventIdentifierStruct) );
   setWeaponFireEvent( fireEvent );  // remember for the detonation interaction

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
      WorldLocationStruct firingLocation;
      Basic::NetHandler::toNetOrder(&firingLocation.x, geocPos[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&firingLocation.y, geocPos[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&firingLocation.z, geocPos[Basic::Nav::IZ]);
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FIRING_LOCATION_WF_PI), 
         (char*) &firingLocation, 
         sizeof(WorldLocationStruct) );

      // Velocity
      VelocityVectorStruct initialVelocityVector; 
      Basic::NetHandler::toNetOrder(&initialVelocityVector.xVelocity, geocVel[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&initialVelocityVector.yVelocity, geocVel[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&initialVelocityVector.zVelocity, geocVel[Basic::Nav::IZ]);
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::INITIAL_VELOCITY_VECTOR_WF_PI), 
         (char*) &initialVelocityVector, 
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
         netIO->getInteractionParameterHandle(NetIO::MUNITION_OBJECT_IDENTIFIER_WF_PI),
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
            netIO->getInteractionParameterHandle(NetIO::FIRING_OBJECT_IDENTIFIER_WF_PI),
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
         if (tNib == 0) tNib = dynamic_cast<Nib*>( netIO->findNib(tPlayer, Simulation::NetIO::OUTPUT_NIB) );
      }

      if (tNib != 0) {
         RTIObjectIdStruct targetObjectIdentifier;
         lcStrncpy(
            (char*)&targetObjectIdentifier.id[0],
            sizeof(targetObjectIdentifier.id),
            tNib->getObjectName(),
            RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            netIO->getInteractionParameterHandle(NetIO::TARGET_OBJECT_IDENTIFIER_WF_PI),
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
            netIO->getInteractionParameterHandle(NetIO::MUNITION_TYPE_WF_PI),
            (char*) &munitionType,
            sizeof(EntityTypeStruct)
         );
   }

   // ---
   // Fire Control Solution Range (meters)
   // ---
   {
      float fireControlSolutionRange;
      Basic::NetHandler::toNetOrder(&fireControlSolutionRange, 0 );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FIRE_CONTROL_SOLUTION_RANGE_WF_PI),
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
         netIO->getInteractionParameterHandle(NetIO::FIRE_MISSION_INDEX_WF_PI),
         (char*) &netBuffer,
         sizeof(unsigned long) );
   }

   // ---
   // Fuse Type (16 bit enum)
   // ---
   {
      FuseTypeEnum16 fuseType = FuseTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, (unsigned short)(fuseType) );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FUSE_TYPE_WF_PI),
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
         netIO->getInteractionParameterHandle(NetIO::QUANTITY_FIRED_WF_PI),
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
         netIO->getInteractionParameterHandle(NetIO::RATE_OF_FIRE_WF_PI),
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
         netIO->getInteractionParameterHandle(NetIO::WARHEAD_TYPE_WF_PI),
         (char*) &netBuffer,
         sizeof(unsigned short) );
   }

   // ---
   // Send the interaction
   // ---
   bool ok = netIO->sendInteraction(
         netIO->getInteractionClassHandle(NetIO::WEAPON_FIRE_INTERACTION), 
         pParams );

   // don't need this anymore
   delete pParams;

   return ok;
}

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
