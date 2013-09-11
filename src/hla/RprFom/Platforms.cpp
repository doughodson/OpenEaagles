//------------------------------------------------------------------------------
// Class: NetIO
//------------------------------------------------------------------------------

#include "openeaagles/hla/rprFom/NetIO.h"
#include "openeaagles/hla/rprFom/RprFom.h"
#include "openeaagles/hla/rprFom/Nib.h"
#include "openeaagles/hla/Ambassador.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"

namespace Eaagles {
namespace Network {
namespace Hla {
namespace RprFom {

//------------------------------------------------------------------------------
// publishAndSubscribe()
//------------------------------------------------------------------------------
bool NetIO::publishAndSubscribePlatforms()
{
   bool ok = true;
   RTI::RTIambassador* p = getRTIambassador();

   // ----------
   // Get handles to the class, attributes and parameters
   // ----------
   try {

      // ---
      // Base entity class and attribute handles
      // ---
      {
         RTI::ObjectClassHandle basicEntityClassHandle = 
            p->getObjectClassHandle(BaseEntity::getClassFedName());

         setObjectClassHandle(BASE_ENTITY_CLASS, basicEntityClassHandle );

         setObjectAttributeHandle(
            ENTITY_TYPE_AI,
            p->getAttributeHandle(
               BaseEntity::getEntityTypeAttributeFedName(),
               basicEntityClassHandle)
         );

         setObjectAttributeHandle(
            ENTITY_IDENTIFIER_AI,
            p->getAttributeHandle(
               BaseEntity::getEntityIdentifierAttributeFedName(),
               basicEntityClassHandle)
         );

         setObjectAttributeHandle(
            SPATIAL_AI,
            p->getAttributeHandle(
               BaseEntity::getSpatialAttributeFedName(), 
               basicEntityClassHandle)
         );
      }

      // ---
      // Physical entity class and attribute handles
      // ---
      {
         RTI::ObjectClassHandle physicalEntityClassHandle =
            p->getObjectClassHandle(PhysicalEntity::getClassFedName());

         setObjectClassHandle(PHYSICAL_ENTITY_CLASS, physicalEntityClassHandle );

         setObjectAttributeHandle(
            FORCE_IDENTIFIER_AI,
            p->getAttributeHandle(
               PhysicalEntity::getForceIdentifierAttributeFedName(), 
               physicalEntityClassHandle) 
         );
      }

      // ---
      // Platform entity class and attribute handles
      // ---
      {
         RTI::ObjectClassHandle platformClassHandle =
            p->getObjectClassHandle(Platform::getClassFedName());

         setObjectClassHandle(PLATFORM_CLASS, platformClassHandle );
         // No attributes at this time ...
      }

      // ---
      // Aircraft class handle
      // ---
      setObjectClassHandle(
         AIRCRAFT_CLASS,
         p->getObjectClassHandle(Aircraft::getClassFedName())
      );

      // ---
      // Ground vehicle class handle
      // ---
      setObjectClassHandle(
         GROUND_VEHICLE_CLASS,
         p->getObjectClassHandle(GroundVehicle::getClassFedName())
      );

      // ---
      // Human class handle
      // ---
      setObjectClassHandle(
         HUMAN_CLASS,
         p->getObjectClassHandle(Human::getClassFedName())
      );

      // ---
      // Munition class handle
      // ---
      setObjectClassHandle(
         MUNITION_CLASS,
         p->getObjectClassHandle(Munition::getClassFedName())
      );

      // ---
      // Surface vessel class handle
      // ---
      setObjectClassHandle(
         SURFACE_VESSEL_CLASS,
         p->getObjectClassHandle(SurfaceVessel::getClassFedName())
      );
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

         // General purpuse attribue handle set used to publish and subscribe to class attribues
         RTI::AttributeHandleSet* attrs =
            RTI::AttributeHandleSetFactory::create( NUM_OBJECT_ATTRIBUTES );

         // ---
         // Publish these entity classes and attributes
         // ---
         attrs->empty();
         attrs->add(getObjectAttributeHandle(ENTITY_TYPE_AI));
         attrs->add(getObjectAttributeHandle(ENTITY_IDENTIFIER_AI));
         attrs->add(getObjectAttributeHandle(SPATIAL_AI));
         attrs->add(getObjectAttributeHandle(RELATIVE_SPATIAL_AI));

         if (isOutputEnabled()) {
            p->publishObjectClass(getObjectClassHandle(AIRCRAFT_CLASS), *attrs);
            setObjectClassPublished(AIRCRAFT_CLASS, true);

            p->publishObjectClass(getObjectClassHandle(GROUND_VEHICLE_CLASS), *attrs);
            setObjectClassPublished(GROUND_VEHICLE_CLASS, true);

            p->publishObjectClass(getObjectClassHandle(HUMAN_CLASS), *attrs);
            setObjectClassPublished(HUMAN_CLASS, true);

            p->publishObjectClass(getObjectClassHandle(MUNITION_CLASS), *attrs);
            setObjectClassPublished(MUNITION_CLASS, true);

            p->publishObjectClass(getObjectClassHandle(SURFACE_VESSEL_CLASS), *attrs);
            setObjectClassPublished(SURFACE_VESSEL_CLASS, true);
         }

         // ---
         // Subscribe to these entity classes and attributes, and request an update.
         // ---
         //  Note: For now we subscribe to the same attribute list that we publish!
         if (isInputEnabled()) {
            p->subscribeObjectClassAttributes(getObjectClassHandle(AIRCRAFT_CLASS), *attrs);
            p->requestClassAttributeValueUpdate(getObjectClassHandle(AIRCRAFT_CLASS), *attrs);
            setObjectClassSubscribed(AIRCRAFT_CLASS, true);

            p->subscribeObjectClassAttributes(getObjectClassHandle(GROUND_VEHICLE_CLASS), *attrs);
            p->requestClassAttributeValueUpdate(getObjectClassHandle(GROUND_VEHICLE_CLASS), *attrs);
            setObjectClassSubscribed(GROUND_VEHICLE_CLASS, true);

            p->subscribeObjectClassAttributes(getObjectClassHandle(HUMAN_CLASS), *attrs);
            p->requestClassAttributeValueUpdate(getObjectClassHandle(HUMAN_CLASS), *attrs);
            setObjectClassSubscribed(HUMAN_CLASS, true);

            p->subscribeObjectClassAttributes(getObjectClassHandle(MUNITION_CLASS), *attrs);
            p->requestClassAttributeValueUpdate(getObjectClassHandle(MUNITION_CLASS), *attrs);
            setObjectClassSubscribed(MUNITION_CLASS, true);

            p->subscribeObjectClassAttributes(getObjectClassHandle(SURFACE_VESSEL_CLASS), *attrs);
            p->requestClassAttributeValueUpdate(getObjectClassHandle(SURFACE_VESSEL_CLASS), *attrs);
            setObjectClassSubscribed(SURFACE_VESSEL_CLASS, true);
         }

         delete attrs;
      }
      catch (RTI::Exception& e) {
         std::cerr << &e << std::endl;
         ok = false;
      }
   }

   return ok;
}

//==============================================================================
// Class: Nib
//==============================================================================

//------------------------------------------------------------------------------
// reflectAttributeValues() -- (Input support)
//  Called by our FederateAmbassador to update the attribute values for
//  this object instance.  (Also handles the network to host byte swapping)
//------------------------------------------------------------------------------
void Nib::reflectAttributeValues(const RTI::AttributeHandleValuePairSet& theAttrs)
{
   NetIO* netIO = (NetIO*)(getNetIO());
   if (netIO != 0 && baseEntity != 0) {

      // PhysicalEntity pointer
      PhysicalEntity* physicalEntity = dynamic_cast<PhysicalEntity*>(baseEntity);

      RTI::ULong length;
      char netBuffer[1000];
      for (RTI::ULong j = 0; j < theAttrs.size(); j++ ) {

         // get the attribute's handle and data (network byte order)
         RTI::AttributeHandle theAttr = theAttrs.getHandle(j);
         theAttrs.getValue(j, netBuffer, length);

         // Process the attribute
         switch ( netIO->findAttributeIndex(theAttr) ) {

            // Update Federate ID
            case NetIO::ENTITY_IDENTIFIER_AI : {
               EntityIdentifierStruct* net = (EntityIdentifierStruct*) &netBuffer;

               Basic::NetHandler::fromNetOrder(&baseEntity->entityIdentifier.federateIdentifier.applicationID, net->federateIdentifier.applicationID );                
               Basic::NetHandler::fromNetOrder(&baseEntity->entityIdentifier.federateIdentifier.siteID, net->federateIdentifier.siteID );
               Basic::NetHandler::fromNetOrder(&baseEntity->entityIdentifier.entityNumber, net->entityNumber );

               setAttributeUpdateRequiredFlag(NetIO::ENTITY_IDENTIFIER_AI,true);
            }
            break;

            // Update Entity Type
            case NetIO::ENTITY_TYPE_AI : {
               EntityTypeStruct* net = (EntityTypeStruct*) &netBuffer;

               // All bytes except for country
               baseEntity->entityType = *net;
               Basic::NetHandler::fromNetOrder(&baseEntity->entityType.countryCode, net->countryCode );

               setAttributeUpdateRequiredFlag(NetIO::ENTITY_TYPE_AI,true);
            }
            break;

            // Update Spacial
            case NetIO::SPATIAL_AI : {

               // NIB's base entity structure pointers
               SpatialStruct* spatial = &(baseEntity->spatial);
               SpatialRVStruct* spatialRvw = &(baseEntity->spatialRvw);
               WorldLocationStruct* worldLocation = &spatialRvw->worldLocation;
               OrientationStruct* orientation = &spatialRvw->orientation;
               VelocityVectorStruct* velocityVector = &spatialRvw->velocityVector;
               AccelerationVectorStruct* accelerationVector = &spatialRvw->accelerationVector;
               AngularVelocityVectorStruct* angularVelocity = &spatialRvw->angularVelocity;

               // Net buffer component pointers 
               SpatialStruct* netSpatial = (SpatialStruct*) &netBuffer[0];
               WorldLocationStruct* netWorldLocation = 0;
               OrientationStruct* netOrientation = 0;
               VelocityVectorStruct* netVelocityVector = 0;
               AccelerationVectorStruct* netAccelerationVector = 0;
               AngularVelocityVectorStruct* netAngularVelocity = 0;

               // Dead reckoning 
               spatial->deadReckoningAlgorithm = netSpatial->deadReckoningAlgorithm;

               // find network components based on dead reckoning alg
               // (and set the isFrozen flag)
               switch (spatial->deadReckoningAlgorithm) {

                  case DRM_STATIC : {
                     SpatialStaticStruct* netSpatialStatic =
                     (SpatialStaticStruct*) &netBuffer[sizeof(SpatialStruct)];
                     spatialRvw->isFrozen = netSpatialStatic->isFrozen;
                     netWorldLocation = &netSpatialStatic->worldLocation;
                     netOrientation = &netSpatialStatic->orientation;
                  }
                  break;

                  case DRM_FPW : {
                     SpatialFPStruct* netSpatialFpw =
                           (SpatialFPStruct*) &netBuffer[sizeof(SpatialStruct)];
                     spatialRvw->isFrozen = netSpatialFpw->isFrozen;
                     netWorldLocation = &netSpatialFpw->worldLocation;
                     netOrientation = &netSpatialFpw->orientation;
                     netVelocityVector = &netSpatialFpw->velocityVector;
                  }
                  break;

                  case DRM_RPW : {
                     SpatialRPStruct* netSpatialRpw =
                           (SpatialRPStruct*) &netBuffer[sizeof(SpatialStruct)];
                     spatialRvw->isFrozen = netSpatialRpw->isFrozen;
                     netWorldLocation = &netSpatialRpw->worldLocation;
                     netOrientation = &netSpatialRpw->orientation;
                     netVelocityVector = &netSpatialRpw->velocityVector;
                     netAngularVelocity = &netSpatialRpw->angularVelocity;
                  }
                  break;

                  case DRM_RVW : {
                     SpatialRVStruct* netSpatialRvw =
                           (SpatialRVStruct*) &netBuffer[sizeof(SpatialStruct)];
                     spatialRvw->isFrozen = netSpatialRvw->isFrozen;
                     netWorldLocation = &netSpatialRvw->worldLocation;
                     netOrientation = &netSpatialRvw->orientation;
                     netVelocityVector = &netSpatialRvw->velocityVector;
                     netAccelerationVector = &netSpatialRvw->accelerationVector;
                     netAngularVelocity = &netSpatialRvw->angularVelocity;
                  }
                  break;

                  case DRM_FVW : {
                     SpatialFVStruct* netSpatialFvw =
                           (SpatialFVStruct*) &netBuffer[sizeof(SpatialStruct)];
                     spatialRvw->isFrozen = netSpatialFvw->isFrozen;
                     netWorldLocation = &netSpatialFvw->worldLocation;
                     netOrientation = &netSpatialFvw->orientation;
                     netVelocityVector = &netSpatialFvw->velocityVector;
                     netAccelerationVector = &netSpatialFvw->accelerationVector;
                  }
                  break;
               }  // end dead rec switch

               if (netWorldLocation != 0) {
                  Basic::NetHandler::fromNetOrder(&worldLocation->x, netWorldLocation->x);
                  Basic::NetHandler::fromNetOrder(&worldLocation->y, netWorldLocation->y);
                  Basic::NetHandler::fromNetOrder(&worldLocation->z, netWorldLocation->z);
               }
               else {
                  worldLocation->x = 0;
                  worldLocation->y = 0;
                  worldLocation->z = 0;
               }

               if (netOrientation != 0) {
                  Basic::NetHandler::fromNetOrder(&orientation->phi, netOrientation->phi);
                  Basic::NetHandler::fromNetOrder(&orientation->theta, netOrientation->theta);
                  Basic::NetHandler::fromNetOrder(&orientation->psi, netOrientation->psi);
               }
               else {
                  orientation->phi = 0;
                  orientation->theta = 0;
                  orientation->psi = 0;
               }

               if (netVelocityVector != 0) {
                  Basic::NetHandler::fromNetOrder(&velocityVector->xVelocity, netVelocityVector->xVelocity);
                  Basic::NetHandler::fromNetOrder(&velocityVector->yVelocity, netVelocityVector->yVelocity);
                  Basic::NetHandler::fromNetOrder(&velocityVector->zVelocity, netVelocityVector->zVelocity);
               }
               else {
                  velocityVector->xVelocity = 0;
                  velocityVector->yVelocity = 0;
                  velocityVector->zVelocity = 0;
               }

               if (netAccelerationVector != 0) {
                  Basic::NetHandler::fromNetOrder(&accelerationVector->xAcceleration, netAccelerationVector->xAcceleration);
                  Basic::NetHandler::fromNetOrder(&accelerationVector->yAcceleration, netAccelerationVector->yAcceleration);
                  Basic::NetHandler::fromNetOrder(&accelerationVector->zAcceleration, netAccelerationVector->zAcceleration);
               }
               else {
                  accelerationVector->xAcceleration = 0;
                  accelerationVector->yAcceleration = 0;
                  accelerationVector->zAcceleration = 0;
               }

               if (netAngularVelocity != 0) {
                  Basic::NetHandler::fromNetOrder(&angularVelocity->xAngularVelocity, netAngularVelocity->xAngularVelocity);
                  Basic::NetHandler::fromNetOrder(&angularVelocity->yAngularVelocity, netAngularVelocity->yAngularVelocity);
                  Basic::NetHandler::fromNetOrder(&angularVelocity->zAngularVelocity, netAngularVelocity->zAngularVelocity);
               }
               else {
                  angularVelocity->xAngularVelocity = 0;
                  angularVelocity->yAngularVelocity = 0;
                  angularVelocity->zAngularVelocity = 0;
               }

               setAttributeUpdateRequiredFlag(NetIO::SPATIAL_AI,true);
            }
            break;

            case NetIO::FORCE_IDENTIFIER_AI : {
               unsigned char* net = (unsigned char*) &netBuffer;


               physicalEntity->forceIdentifier = ForceIdentifierEnum8( *net );
               //std::cout << "Recv force: " << physicalEntity->forceIdentifier << std::endl;;

               setAttributeUpdateRequiredFlag(NetIO::FORCE_IDENTIFIER_AI,true);
            }
            break;

         } // end -- process attribute switch
      } // end -- for each attribute pair

      // Update the basic NIB state data with this new data
      entityState2Nib();
   }
}

//------------------------------------------------------------------------------
// entityState2Nib() -- (Input support)
//  Transfers data from the incoming entity attributes to the basic
//  NIB class variables.
//------------------------------------------------------------------------------
void Nib::entityState2Nib()
{    
   if (isAttributeUpdateRequired(NetIO::ENTITY_IDENTIFIER_AI)) {
      setPlayerID(baseEntity->entityIdentifier.entityNumber);
      setSiteID(baseEntity->entityIdentifier.federateIdentifier.siteID);
      setApplicationID(baseEntity->entityIdentifier.federateIdentifier.applicationID);
      setAttributeUpdateRequiredFlag(NetIO::ENTITY_IDENTIFIER_AI,false);
      haveEntityIdFlg = true;
   }

   if (isAttributeUpdateRequired(NetIO::ENTITY_TYPE_AI)) {
      setEntityType(
            baseEntity->entityType.entityKind,
            baseEntity->entityType.domain,
            baseEntity->entityType.countryCode,
            baseEntity->entityType.category,
            baseEntity->entityType.subcategory,
            baseEntity->entityType.specific,
            baseEntity->entityType.extra
         );
      setAttributeUpdateRequiredFlag(NetIO::ENTITY_TYPE_AI,false);
      haveEntityTypeFlg = true;
   }

   if (isAttributeUpdateRequired(NetIO::SPATIAL_AI)) {

      // NIB's base entity structures
      //SpatialStruct* spatial = &(baseEntity->spatial);
      SpatialRVStruct* spatialRvw = &(baseEntity->spatialRvw);

      // Set the freeze flag
      freeze(spatialRvw->isFrozen != 0);

      // Get the geocentric position, velocity and acceleration
      osg::Vec3d geocPos;
      geocPos[Basic::Nav::IX] = spatialRvw->worldLocation.x;
      geocPos[Basic::Nav::IY] = spatialRvw->worldLocation.y;
      geocPos[Basic::Nav::IZ] = spatialRvw->worldLocation.z;

      osg::Vec3d geocVel;
      geocVel[Basic::Nav::IX] = spatialRvw->velocityVector.xVelocity;
      geocVel[Basic::Nav::IY] = spatialRvw->velocityVector.yVelocity;
      geocVel[Basic::Nav::IZ] = spatialRvw->velocityVector.zVelocity;

      osg::Vec3d geocAcc;
      geocAcc[Basic::Nav::IX] = spatialRvw->accelerationVector.xAcceleration;
      geocAcc[Basic::Nav::IY] = spatialRvw->accelerationVector.yAcceleration;
      geocAcc[Basic::Nav::IZ] = spatialRvw->accelerationVector.zAcceleration;

      // Get orientation orientation and rates
      osg::Vec3d geocAngles;
      geocAngles[Basic::Nav::IPHI] = spatialRvw->orientation.phi;
      geocAngles[Basic::Nav::ITHETA] = spatialRvw->orientation.theta;
      geocAngles[Basic::Nav::IPSI] = spatialRvw->orientation.psi; 

      osg::Vec3d arates(0.0, 0.0, 0.0);

      // (re)initialize the dead reckoning function
      double diffTime(0.0);
      resetDeadReckoning(
         RVW_DRM,
         geocPos,
         geocVel,
         geocAcc,
         geocAngles,
         arates,
         diffTime);

      haveWorldLocationFlg = true;
      haveOrientationFlg = true;

      setAttributeUpdateRequiredFlag(NetIO::SPATIAL_AI,false);
   }
    
   // PhysicalEntity pointer
   PhysicalEntity* physicalEntity = dynamic_cast<PhysicalEntity*>(baseEntity);
   if (physicalEntity != 0) {

      if (isAttributeUpdateRequired(NetIO::FORCE_IDENTIFIER_AI)) {
         // Side: When mapping Force ID to Player Side ...
         if (physicalEntity->forceIdentifier == FRIENDLY) {
            // Friendly's are blue, ...
            setSide(Simulation::Player::BLUE);
         }
         else if (physicalEntity->forceIdentifier == OPPOSING) {
            // opposing side is red, ...
            setSide(Simulation::Player::RED);
         }
         else if (physicalEntity->forceIdentifier == NEUTRAL) {
            // Neutrals are white, ...
            setSide(Simulation::Player::WHITE);
         }
         else  {
            // and everyone else is gray.
            setSide(Simulation::Player::GRAY);
         }
         setAttributeUpdateRequiredFlag(NetIO::FORCE_IDENTIFIER_AI,false);
      }

   }

   setMode(Simulation::Player::ACTIVE);    
   setTimeExec( (LCreal) getNetIO()->getCurrentTime() );
}

//------------------------------------------------------------------------------
// entityStateManager() --  (Output support)
//    -- Update the entity object for this NIB(Player)
//------------------------------------------------------------------------------
bool Nib::entityStateManager(const LCreal curExecTime)
{
   bool ok = true;
   if (getPlayer()->isMode(Simulation::Player::ACTIVE) && isPlayerStateUpdateRequired(curExecTime)) {

      // Need to update this entity object ...

      NetIO* netIO = (NetIO*)(getNetIO());
      RTI::RTIambassador* rtiAmb = netIO->getRTIambassador();

      // ---
      // First, make sure this entity has been registered
      // ---
      if (!isRegistered()) {
         try {
            RTI::ObjectClassHandle theClassHandle = netIO->getObjectClassHandle( getClassIndex() );
            makeObjectName();
            setObjectHandle( rtiAmb->registerObjectInstance( theClassHandle, getObjectName() ) );
            netIO->addNibToObjectTables(this, Simulation::NetIO::OUTPUT_NIB);
            std::cout << "RprFom::Nib::updateEntity(): Register entity: " << getObjectName() << " handle = " << getObjectHandle() << std::endl;
         }
         catch (RTI::Exception& e) {
            std::cerr << &e << std::endl;
            ok = false;
         }
      }

      // ---
      // Next, update the entity's attribute values ...
      // ---
      if ( ok && isRegistered()) {
         try {
            // Create the attribute-value pair set
            RTI::AttributeHandleValuePairSet* attrs = 0;
            attrs = RTI::AttributeSetFactory::create( NetIO::NUM_OBJECT_ATTRIBUTES );

            // Load the set with updated attribute values
            updateBasicEntity(attrs,curExecTime);
            updatePhysicalEntity(attrs,curExecTime);
            updatePlatform(attrs,curExecTime);

            // Send attributes to the RTI
            //std::cout << "RprFom::Nib::updateEntity(): Update entity: " << getObjectName() << " handle = " << getObjectHandle() << std::endl;
            ok = netIO->updateAttributeValues(getObjectHandle(), attrs);

            delete attrs;
         }

         catch (RTI::Exception& e) {
            std::cerr << &e << std::endl;
            ok = false;
         }
      }
   } // end -- if active player needs an update

   return ok;
}

//------------------------------------------------------------------------------
// updateBasicEntity() -- (Output support)
//    -- sets the BasicEntity attribute values that need to be updated
//
//    -- We send all published BasicEntity attributes everytime.
//
//       The function isPlayerStateUpdateRequired() checking if the player's state
//       needs to be re-setn.  Therefore, we're not check the individual update
//       required flags with isAttributeUpdateRequired(), but we do clear them
//       with setAttributeUpdateRequiredFlag().
//
//   (Also handles the host to network byte swapping)
//------------------------------------------------------------------------------
void Nib::updateBasicEntity(
         RTI::AttributeHandleValuePairSet* attrs, 
         const LCreal)
{
   if (baseEntity != 0) {
      // Our handler 
      NetIO* netIO = (NetIO*)(getNetIO());

      // Our simulation
      Simulation::Simulation* ourSim = netIO->getSimulation();

      // Get the player data
      playerState2Nib();

      // Entity ID
      if (isAttributeUpdateEnabled(NetIO::ENTITY_IDENTIFIER_AI)) {
         EntityIdentifierStruct* entityId = &baseEntity->entityIdentifier;
         entityId->federateIdentifier.siteID = getSiteID();
         entityId->federateIdentifier.applicationID = getApplicationID();
         entityId->entityNumber = getPlayerID();
         //std::cout << "Send Federate ID: (" << entityId->entityNumber << "," << entityId->federateIdentifier.applicationID << "," << entityId->federateIdentifier.siteID << ")" << std::endl;

         EntityIdentifierStruct netBuffer;
         Basic::NetHandler::toNetOrder(&netBuffer.federateIdentifier.applicationID, entityId->federateIdentifier.applicationID );                
         Basic::NetHandler::toNetOrder(&netBuffer.federateIdentifier.siteID, entityId->federateIdentifier.siteID );
         Basic::NetHandler::toNetOrder(&netBuffer.entityNumber, entityId->entityNumber );
         attrs->add(netIO->getObjectAttributeHandle(
               NetIO::ENTITY_IDENTIFIER_AI),
               (char*)&netBuffer,
               sizeof(EntityIdentifierStruct)
            );

         setAttributeUpdateRequiredFlag(NetIO::ENTITY_IDENTIFIER_AI,false);
      }

      // Entity Type
      if (isAttributeUpdateEnabled(NetIO::ENTITY_TYPE_AI)) {

         EntityTypeStruct* entityType = &baseEntity->entityType;
         entityType->entityKind   = getEntityKind();
         entityType->domain       = getEntityDomain();
         entityType->countryCode  = getEntityCountry();
         entityType->category     = getEntityCategory();
         entityType->subcategory  = getEntitySubcategory();
         entityType->specific     = getEntitySpecific();
         entityType->extra        = getEntityExtra();

         // Network byte order: all bytes except country code which is unsigned short.
         EntityTypeStruct netBuffer = *entityType;
         Basic::NetHandler::toNetOrder(&netBuffer.countryCode, entityType->countryCode );
         attrs->add(netIO->getObjectAttributeHandle(
               NetIO::ENTITY_TYPE_AI),
               (char*)&netBuffer,
               sizeof(EntityTypeStruct)
            );

         setAttributeUpdateRequiredFlag(NetIO::ENTITY_TYPE_AI,false);
      }

      // Spatial Structure
      if (isAttributeUpdateEnabled(NetIO::SPATIAL_AI)) {

         osg::Vec3d pos = getDrPosition();
         osg::Vec3d vel = getDrVelocity();
         osg::Vec3d accel = getDrAcceleration();
         osg::Vec3d angles = getDrEulerAngles();
         osg::Vec3d arates = getDrAngularVelocities();

         // NIB's base entity structures
         SpatialStruct* spatial = &(baseEntity->spatial);
         SpatialRVStruct* spatialRvw = &(baseEntity->spatialRvw);

         // Net order buffer (used to send the attribute to the RTI)
         const unsigned int SPATIAL_NET_BUFFER_SIZE = sizeof(SpatialStruct)+sizeof(SpatialRVStruct);
         unsigned char netBuffer[SPATIAL_NET_BUFFER_SIZE];

         SpatialStruct* netSpatial =
               (SpatialStruct*) &netBuffer[0];
         SpatialRVStruct* netSpatialRvw =
               (SpatialRVStruct*) &netBuffer[sizeof(SpatialStruct)];

         // Ref Position 
         double refLat = 0.0;
         double refLon = 0.0;
         if (ourSim != 0) {
            refLat = ourSim->getRefLatitude();
            refLon = ourSim->getRefLongitude();
         }

         // Convert position vector to Lat/Lon/Alt
         double alt;
         double simCoord[3] = { 0, 0, 0 };
         Basic::Nav::convertPosVec2LL(
               refLat, refLon, 
               pos,
               &simCoord[Basic::Nav::ILAT], &simCoord[Basic::Nav::ILON], &alt
            );
         simCoord[Basic::Nav::IALT] = alt;

         //std::cout << "RprFom::Nib::entityState2Nib(): simCoord(" << simCoord[Basic::Nav::ILAT] << "," << simCoord[Basic::Nav::ILON] << "," << simCoord[Basic::Nav::IALT] << ")"  << std::endl;

         // Convert to geocentric coordinates
         double geocPos[3] = { 0, 0, 0 };
         LCreal geocVel[3] = { 0, 0, 0 };
         LCreal geocAcc[3] = { 0, 0, 0 };
         Basic::Nav::getWorldPosAccVel(simCoord, vel.ptr(), accel.ptr(), geocPos, geocVel, geocAcc);

         // Dead reckoning algorithm
         {
            spatial->deadReckoningAlgorithm = DRM_RVW;

            netSpatial->deadReckoningAlgorithm = spatial->deadReckoningAlgorithm;
         }

         // Is Frozen?
         {
            bool simFrz = false;
            if (ourSim != 0) simFrz = ourSim->isFrozen();
            if (isFrozen() || simFrz) spatialRvw->isFrozen = RTI::RTI_TRUE;  // Is this object or the simulation frozen?
            else spatialRvw->isFrozen = RTI::RTI_FALSE;

            netSpatialRvw->isFrozen = spatialRvw->isFrozen;
         }

         // World Location
         {
            WorldLocationStruct* worldLocation = &spatialRvw->worldLocation;
            WorldLocationStruct* netWorldLocation = &netSpatialRvw->worldLocation;

            worldLocation->x = geocPos[Basic::Nav::IX];
            worldLocation->y = geocPos[Basic::Nav::IY];
            worldLocation->z = geocPos[Basic::Nav::IZ];

            Basic::NetHandler::toNetOrder(&netWorldLocation->x, worldLocation->x);
            Basic::NetHandler::toNetOrder(&netWorldLocation->y, worldLocation->y);
            Basic::NetHandler::toNetOrder(&netWorldLocation->z, worldLocation->z);
         }

         // Velocity vector
         {
            VelocityVectorStruct* velocityVector = &spatialRvw->velocityVector;
            VelocityVectorStruct* netVelocityVector = &netSpatialRvw->velocityVector;

            velocityVector->xVelocity = geocVel[Basic::Nav::IX];
            velocityVector->yVelocity = geocVel[Basic::Nav::IY];
            velocityVector->zVelocity = geocVel[Basic::Nav::IZ];

            Basic::NetHandler::toNetOrder(&netVelocityVector->xVelocity, velocityVector->xVelocity);
            Basic::NetHandler::toNetOrder(&netVelocityVector->yVelocity, velocityVector->yVelocity);
            Basic::NetHandler::toNetOrder(&netVelocityVector->zVelocity, velocityVector->zVelocity);
         }

         // Acceleration vector
         {
            AccelerationVectorStruct* accelerationVector = &spatialRvw->accelerationVector;
            AccelerationVectorStruct* netAccelerationVector = &netSpatialRvw->accelerationVector;

            accelerationVector->xAcceleration = geocAcc[Basic::Nav::IX];
            accelerationVector->yAcceleration = geocAcc[Basic::Nav::IY];
            accelerationVector->zAcceleration = geocAcc[Basic::Nav::IZ];

            Basic::NetHandler::toNetOrder(&netAccelerationVector->xAcceleration, accelerationVector->xAcceleration);
            Basic::NetHandler::toNetOrder(&netAccelerationVector->yAcceleration, accelerationVector->yAcceleration);
            Basic::NetHandler::toNetOrder(&netAccelerationVector->zAcceleration, accelerationVector->zAcceleration);
         }

         // Orientation
         {
            OrientationStruct* orientation = &spatialRvw->orientation;
            OrientationStruct* netOrientation = &netSpatialRvw->orientation;

            // Convert euler angles to geocentric angles
            LCreal geocAngles[3] = { 0, 0, 0 };
            Basic::Nav::getGeocAngle(simCoord, angles.ptr(), geocAngles);

            orientation->phi = geocAngles[Basic::Nav::IPHI];
            orientation->theta = geocAngles[Basic::Nav::ITHETA];
            orientation->psi = geocAngles[Basic::Nav::IPSI];

            Basic::NetHandler::toNetOrder(&netOrientation->phi, orientation->phi);
            Basic::NetHandler::toNetOrder(&netOrientation->theta, orientation->theta);
            Basic::NetHandler::toNetOrder(&netOrientation->psi, orientation->psi);
         }

         // Angular velocity vector (all zeros for now)
         {
            AngularVelocityVectorStruct* angularVelocityVector = &spatialRvw->angularVelocity;
            AngularVelocityVectorStruct* netAngularVelocityVector = &netSpatialRvw->angularVelocity;

            angularVelocityVector->xAngularVelocity = 0;
            angularVelocityVector->yAngularVelocity = 0;
            angularVelocityVector->zAngularVelocity = 0;

            Basic::NetHandler::toNetOrder(&netAngularVelocityVector->xAngularVelocity, angularVelocityVector->xAngularVelocity);
            Basic::NetHandler::toNetOrder(&netAngularVelocityVector->yAngularVelocity, angularVelocityVector->yAngularVelocity);
            Basic::NetHandler::toNetOrder(&netAngularVelocityVector->zAngularVelocity, angularVelocityVector->zAngularVelocity);
         }

         attrs->add(
               netIO->getObjectAttributeHandle(NetIO::SPATIAL_AI),
               (char*)&netBuffer,
               SPATIAL_NET_BUFFER_SIZE
            );

         setAttributeUpdateRequiredFlag(NetIO::SPATIAL_AI,false);
      }
   }
}

//------------------------------------------------------------------------------
// updatePhysicalEntity() -- (Output support)
//    -- sets the PhysicalEntity attribute values that need to be updated
//
//    -- We send all published PhysicalEntity attributes everytime.
//       The function isPlayerStateUpdateRequired() check if the player's state
//       needs to be re-sent.  Therefore, we're not checking the individual update
//       required flags with isAttributeUpdateRequired(), but we do clear them
//       with setAttributeUpdateRequiredFlag().
//
//   (Also handles the host to network byte swapping)
//------------------------------------------------------------------------------
void Nib::updatePhysicalEntity(
         RTI::AttributeHandleValuePairSet* attrs, 
         const LCreal)
{
   // PhysicalEntity??
   PhysicalEntity* physicalEntity =  dynamic_cast<PhysicalEntity*>(baseEntity);
   if (physicalEntity != 0) {
      // Our handler 
      NetIO* netIO = (NetIO*)(getNetIO());

      // Force Identifier
      if (isAttributeUpdateEnabled(NetIO::FORCE_IDENTIFIER_AI)) {

         // Force ID: When mapping Player side to force IDs ...
         if (getSide() == Simulation::Player::BLUE) {
            // blue's are friendly, ...
            physicalEntity->forceIdentifier = FRIENDLY;     
         }
         else if (getSide() == Simulation::Player::RED) {
            // red's are not, ...
            physicalEntity->forceIdentifier = OPPOSING;
         }
         else if (getSide() == Simulation::Player::WHITE) {
            // white is neutral, ...
            physicalEntity->forceIdentifier = NEUTRAL;
         }
         else {
            // and everyone else is type OTHER.
            physicalEntity->forceIdentifier = OTHER;
         }

         //std::cout << "Send force: " << physicalEntity->forceIdentifier << std::endl;;

         unsigned char netBuffer = (unsigned char)(physicalEntity->forceIdentifier);  // 8 bits enum type (ForceIdentifierEnum8)
         attrs->add(netIO->getObjectAttributeHandle(
               NetIO::FORCE_IDENTIFIER_AI),
               (char*)&netBuffer,
               sizeof(netBuffer)
            );

         setAttributeUpdateRequiredFlag(NetIO::FORCE_IDENTIFIER_AI,false);
      }
   }
}

//------------------------------------------------------------------------------
// updatePlatform() -- (Output support)
//    -- sets the Platform attribute values that need to be updated
//
//    -- We send all published Platform attributes everytime.
//       The function isPlayerStateUpdateRequired() check if the player's state
//       needs to be re-sent.  Therefore, we're not checking the individual update
//       required flags with isAttributeUpdateRequired(), but we do clear them
//       with setAttributeUpdateRequiredFlag().
//
//   (Also handles the host to network byte swapping)
//------------------------------------------------------------------------------
void Nib::updatePlatform(
         RTI::AttributeHandleValuePairSet*,
         const LCreal)
{
   // No Platform attributes are published at this time
}


} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
