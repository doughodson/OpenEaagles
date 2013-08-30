#include "openeaagles/hla/mc02Fom/Mc02FomIO.h"
#include "openeaagles/hla/mc02Fom/Mc02Fom.h"
#include "openeaagles/hla/mc02Fom/Mc02FomNib.h"
#include "openeaagles/hla/Ambassador.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"

//==============================================================================
// Class: HlaIO
//==============================================================================
namespace Eaagles {
namespace Network {
namespace Hla {
namespace Mc02Fom {

//------------------------------------------------------------------------------
// publishAndSubscribe()
//------------------------------------------------------------------------------
bool Mc02FomIO::publishAndSubscribeWeaponFire()
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
            p->getInteractionClassHandle(Mc02Fom::WeaponFire::getInteractionFedName());

         setInteractionClassHandle(WEAPON_FIRE_INTERACTION, handle );

         setInteractionParameterHandle(
            EVENT_IDENTIFIER_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getEventIdentifierParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRE_CONTROL_SOLUTION_RANGE_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getFireControlSolutionRangeParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRE_MISSION_INDEX_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getFireMissionIndexParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRING_LOCATION_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getFiringLocationParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FIRING_OBJECT_IDENTIFIER_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getFiringObjectIdentifierParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            FUSE_TYPE_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getFuseTypeParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            INITIAL_VELOCITY_VECTOR_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getInitialVelocityVectorParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            MUNITION_OBJECT_IDENTIFIER_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getMunitionObjectIdentifierParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            MUNITION_TYPE_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getMunitionTypeParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            QUANTITY_FIRED_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getQuantityFiredParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            RATE_OF_FIRE_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getRateOfFireParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            TARGET_OBJECT_IDENTIFIER_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getTargetObjectIdentifierParameterFedName(),
               handle)
         );

         setInteractionParameterHandle(
            WARHEAD_TYPE_WF_PI,
            p->getParameterHandle(
               Mc02Fom::WeaponFire::getWarheadTypeParameterFedName(),
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
bool Mc02FomIO::receiveWeaponFire(const RTI::ParameterHandleValuePairSet& theParameters)
{
   std::cout << "Mc02Fom::Nib::receiveWeaponFire() HERE!!" << std::endl;
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
// Class: Mc02Fom::Nib
//==============================================================================

//------------------------------------------------------------------------------
// weaponFireMsgFactory() -- (Output support) Weapon fire message factory
//------------------------------------------------------------------------------
bool Mc02Fom::Nib::weaponFireMsgFactory(const LCreal)
{
   std::cout << "Mc02Fom::Nib::sendWeaponFire() HERE!!" << std::endl;

   // Early out -- we must be registered
   if (!isRegistered()) return false;

   // Get our Simulation::NetworkIO
   Mc02FomIO* mc02IO = (Mc02FomIO*)(getNetworkIO());

   // Create the parameter/value set
   RTI::ParameterHandleValuePairSet* pParams =
      RTI::ParameterSetFactory::create( Mc02FomIO::NUM_INTERACTION_PARAMETER );

   // Set our mode so that we don't do this again.
   setMode(Simulation::Player::ACTIVE);

   // If our player just launched, then it must be a weapon!
   Simulation::Weapon* mPlayer = dynamic_cast<Simulation::Weapon*>(getPlayer());
   if (mPlayer == 0) return false;  // Early out -- it wasn't a weapon! 

   // ---
   // Event ID
   // ---
   unsigned short fireEvent = mPlayer->getReleaseEventID();
   Mc02Fom::EventIdentifierStruct eventIdentifier;
   Basic::NetHandler::toNetOrder(&eventIdentifier.eventCount, fireEvent);
   strncpy(
      (char*)&eventIdentifier.issuingObjectIdentifier.id[0], 
      getObjectName(), 
      Mc02Fom::RTIObjectIdStruct::ID_SIZE );    
   pParams->add(
      mc02IO->getInteractionParameterHandle(Mc02FomIO::EVENT_IDENTIFIER_WF_PI),
      (char*) &eventIdentifier,
      sizeof(Mc02Fom::EventIdentifierStruct) );
   setWeaponFireEvent( fireEvent );  // remember for the detonation interaction

   // ---
   // Location & Velociy
   // ---
   {
      double refLat = 0.0;
      double refLon = 0.0;
      if (mc02IO->getSimulation() != 0) {
         refLat = mc02IO->getSimulation()->getRefLatitude();
         refLon = mc02IO->getSimulation()->getRefLongitude();
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
      Mc02Fom::WorldLocationStruct firingLocation;
      Basic::NetHandler::toNetOrder(&firingLocation.x, geocPos[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&firingLocation.y, geocPos[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&firingLocation.z, geocPos[Basic::Nav::IZ]);
      pParams->add(
         mc02IO->getInteractionParameterHandle(Mc02FomIO::FIRING_LOCATION_WF_PI), 
         (char*) &firingLocation, 
         sizeof(Mc02Fom::WorldLocationStruct) );

      // Velocity
      Mc02Fom::VelocityVectorStruct initialVelocityVector; 
      Basic::NetHandler::toNetOrder(&initialVelocityVector.xVelocity, geocVel[Basic::Nav::IX]);
      Basic::NetHandler::toNetOrder(&initialVelocityVector.yVelocity, geocVel[Basic::Nav::IY]);
      Basic::NetHandler::toNetOrder(&initialVelocityVector.zVelocity, geocVel[Basic::Nav::IZ]);
      pParams->add(
         mc02IO->getInteractionParameterHandle(Mc02FomIO::INITIAL_VELOCITY_VECTOR_WF_PI), 
         (char*) &initialVelocityVector, 
         sizeof(Mc02Fom::VelocityVectorStruct) );
   }

   // ---
   // Munition Object identfiers:
   // ---
   {
      Mc02Fom::RTIObjectIdStruct munitionObjectIdentifier;
      strncpy(
         (char*)&munitionObjectIdentifier.id[0],
         getObjectName(),
         Mc02Fom::RTIObjectIdStruct::ID_SIZE );    
      pParams->add(
         mc02IO->getInteractionParameterHandle(Mc02FomIO::MUNITION_OBJECT_IDENTIFIER_WF_PI),
         (char*) &munitionObjectIdentifier,
         sizeof(Mc02Fom::RTIObjectIdStruct) );
   }

   // ---
   // Firing Object identfier:
   //
   // Get the firing player and its NIB.
   //   First check to see if it's an IPlayer from an HLA network.
   //   If it's not, then check our output list.
   // ---
   {
      Mc02Fom::Nib* fNib = 0;
      Simulation::Player* fPlayer = mPlayer->getLauncher();
      if (fPlayer != 0) {
         if (fPlayer->isNetworkedPlayer()) {
            fNib = dynamic_cast<Mc02Fom::Nib*>( fPlayer->getNib() );
         }
         else {
            fNib = dynamic_cast<Mc02Fom::Nib*>( mc02IO->findNib(fPlayer, Simulation::NetworkIO::OUTPUT_NIB) );
         }
      }

      if (fNib != 0) {
         Mc02Fom::RTIObjectIdStruct firingObjectIdentifier;
         strncpy(
            (char*)&firingObjectIdentifier.id[0],
            fNib->getObjectName(),
            Mc02Fom::RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            mc02IO->getInteractionParameterHandle(Mc02FomIO::FIRING_OBJECT_IDENTIFIER_WF_PI),
            (char*) &firingObjectIdentifier,
            sizeof(Mc02Fom::RTIObjectIdStruct) );
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
      Mc02Fom::Nib* tNib = 0;
      Simulation::Player* tPlayer = mPlayer->getTargetPlayer();
      if (tPlayer != 0) {
         tNib = dynamic_cast<Mc02Fom::Nib*>( tPlayer->getNib() );
         if (tNib == 0) tNib = dynamic_cast<Mc02Fom::Nib*>( mc02IO->findNib(tPlayer, Simulation::NetworkIO::OUTPUT_NIB) );
      }

      if (tNib != 0) {
         Mc02Fom::RTIObjectIdStruct targetObjectIdentifier;
         strncpy(
            (char*)&targetObjectIdentifier.id[0],
            tNib->getObjectName(),
            Mc02Fom::RTIObjectIdStruct::ID_SIZE );    
         pParams->add(
            mc02IO->getInteractionParameterHandle(Mc02FomIO::TARGET_OBJECT_IDENTIFIER_WF_PI),
            (char*) &targetObjectIdentifier,
            sizeof(Mc02Fom::RTIObjectIdStruct) );
      }
   }

   // ---
   // Munition Type
   // ---
   {
      Mc02Fom::EntityTypeStruct munitionType;
      munitionType.entityKind = getKind();
      munitionType.domain = getDomain();
      Basic::NetHandler::toNetOrder(&munitionType.countryCode, getCountry() );
      munitionType.category  = getCategory();
      munitionType.subcategory = getSubcategory();
      munitionType.specific   = getSpecific();
      munitionType.extra  = getExtra();
      pParams->add(
         mc02IO->getInteractionParameterHandle(Mc02FomIO::MUNITION_TYPE_WF_PI),
         (char*) &munitionType,
         sizeof(Mc02Fom::EntityTypeStruct) );
   }

   // ---
   // Fire Control Solution Range (meters)
   // ---
   {
      float fireControlSolutionRange;
      Basic::NetHandler::toNetOrder(&fireControlSolutionRange, 0 );
      pParams->add(
         mc02IO->getInteractionParameterHandle(Mc02FomIO::FIRE_CONTROL_SOLUTION_RANGE_WF_PI),
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
         mc02IO->getInteractionParameterHandle(Mc02FomIO::FIRE_MISSION_INDEX_WF_PI),
         (char*) &netBuffer,
         sizeof(unsigned long) );
   }

   // ---
   // Fuse Type (16 bit enum)
   // ---
   {
      Mc02Fom::FuseTypeEnum16 fuseType = Mc02Fom::FuseTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, (unsigned short)(fuseType) );
      pParams->add(
         mc02IO->getInteractionParameterHandle(Mc02FomIO::FUSE_TYPE_WF_PI),
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
         mc02IO->getInteractionParameterHandle(Mc02FomIO::QUANTITY_FIRED_WF_PI),
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
         mc02IO->getInteractionParameterHandle(Mc02FomIO::RATE_OF_FIRE_WF_PI),
         (char*) &netBuffer,
         sizeof(unsigned short) );
   }

   // ---
   // Warhead type
   // ---
   {
      Mc02Fom::WarheadTypeEnum16 warheadType = Mc02Fom::WarheadTypeOther;
      unsigned short netBuffer;
      Basic::NetHandler::toNetOrder(&netBuffer, (unsigned short)(warheadType) );
      pParams->add(
         mc02IO->getInteractionParameterHandle(Mc02FomIO::WARHEAD_TYPE_WF_PI),
         (char*) &netBuffer,
         sizeof(unsigned short) );
   }

   // ---
   // Send the interaction
   // ---
   bool ok = mc02IO->sendInteraction(
         mc02IO->getInteractionClassHandle(Mc02FomIO::WEAPON_FIRE_INTERACTION), 
         pParams );

   // don't need this anymore
   delete pParams;

   return ok;
}

} // End Mc02Fom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
