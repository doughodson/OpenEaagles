
#include "openeaagles/networks/rprfom/NetIO.h"
#include "openeaagles/networks/rprfom/RprFom.h"
#include "openeaagles/networks/rprfom/Nib.h"
#include "openeaagles/networks/hla/Ambassador.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/base/Nav.h"
#include "openeaagles/base/NetHandler.h"

namespace oe {
namespace rprfom {

//------------------------------------------------------------------------------
// Class: Nib
// Description: Portions of class defined to support entity state
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// reflectAttributeValues() -- (Input support)
//  Called by our FederateAmbassador to update the attribute values for
//  this object instance.  (Also handles the network to host byte swapping)
//------------------------------------------------------------------------------
void Nib::reflectAttributeValues(const RTI::AttributeHandleValuePairSet& theAttrs)
{
   NetIO* netIO = static_cast<NetIO*>(getNetIO());
   if (netIO != nullptr && baseEntity != nullptr) {

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
               EntityIdentifierStruct* net = reinterpret_cast<EntityIdentifierStruct*>(&netBuffer);

               base::NetHandler::fromNetOrder(&baseEntity->entityIdentifier.federateIdentifier.applicationID, net->federateIdentifier.applicationID );
               base::NetHandler::fromNetOrder(&baseEntity->entityIdentifier.federateIdentifier.siteID, net->federateIdentifier.siteID );
               base::NetHandler::fromNetOrder(&baseEntity->entityIdentifier.entityNumber, net->entityNumber );

               setAttributeUpdateRequiredFlag(NetIO::ENTITY_IDENTIFIER_AI, true);
            }
            break;

            // Update Entity Type
            case NetIO::ENTITY_TYPE_AI : {
               EntityTypeStruct* net = reinterpret_cast<EntityTypeStruct*>(&netBuffer);

               // All bytes except for country
               baseEntity->entityType = *net;
               base::NetHandler::fromNetOrder(&baseEntity->entityType.countryCode, net->countryCode );

               setAttributeUpdateRequiredFlag(NetIO::ENTITY_TYPE_AI, true);
            }
            break;

            // Update Spatial
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
               SpatialStruct* netSpatial = reinterpret_cast<SpatialStruct*>(&netBuffer[0]);
               WorldLocationStruct* netWorldLocation = nullptr;
               OrientationStruct* netOrientation = nullptr;
               VelocityVectorStruct* netVelocityVector = nullptr;
               AccelerationVectorStruct* netAccelerationVector = nullptr;
               AngularVelocityVectorStruct* netAngularVelocity = nullptr;

               // Dead reckoning 
               spatial->deadReckoningAlgorithm = netSpatial->deadReckoningAlgorithm;

               // find network components based on dead reckoning algorithm
               // (and set the isFrozen flag)
               switch (spatial->deadReckoningAlgorithm) {

                  case DRM_STATIC : {
                     SpatialStaticStruct* netSpatialStatic = reinterpret_cast<SpatialStaticStruct*>(&netBuffer[sizeof(SpatialStruct)]);
                     spatialRvw->isFrozen = netSpatialStatic->isFrozen;
                     netWorldLocation = &netSpatialStatic->worldLocation;
                     netOrientation = &netSpatialStatic->orientation;
                  }
                  break;

                  case DRM_FPW : {
                     SpatialFPStruct* netSpatialFpw = reinterpret_cast<SpatialFPStruct*>(&netBuffer[sizeof(SpatialStruct)]);
                     spatialRvw->isFrozen = netSpatialFpw->isFrozen;
                     netWorldLocation = &netSpatialFpw->worldLocation;
                     netOrientation = &netSpatialFpw->orientation;
                     netVelocityVector = &netSpatialFpw->velocityVector;
                  }
                  break;

                  case DRM_RPW : {
                     SpatialRPStruct* netSpatialRpw = reinterpret_cast<SpatialRPStruct*>(&netBuffer[sizeof(SpatialStruct)]);
                     spatialRvw->isFrozen = netSpatialRpw->isFrozen;
                     netWorldLocation = &netSpatialRpw->worldLocation;
                     netOrientation = &netSpatialRpw->orientation;
                     netVelocityVector = &netSpatialRpw->velocityVector;
                     netAngularVelocity = &netSpatialRpw->angularVelocity;
                  }
                  break;

                  case DRM_RVW : {
                     SpatialRVStruct* netSpatialRvw = reinterpret_cast<SpatialRVStruct*>(&netBuffer[sizeof(SpatialStruct)]);
                     spatialRvw->isFrozen = netSpatialRvw->isFrozen;
                     netWorldLocation = &netSpatialRvw->worldLocation;
                     netOrientation = &netSpatialRvw->orientation;
                     netVelocityVector = &netSpatialRvw->velocityVector;
                     netAccelerationVector = &netSpatialRvw->accelerationVector;
                     netAngularVelocity = &netSpatialRvw->angularVelocity;
                  }
                  break;

                  case DRM_FVW : {
                     SpatialFVStruct* netSpatialFvw = reinterpret_cast<SpatialFVStruct*>(&netBuffer[sizeof(SpatialStruct)]);
                     spatialRvw->isFrozen = netSpatialFvw->isFrozen;
                     netWorldLocation = &netSpatialFvw->worldLocation;
                     netOrientation = &netSpatialFvw->orientation;
                     netVelocityVector = &netSpatialFvw->velocityVector;
                     netAccelerationVector = &netSpatialFvw->accelerationVector;
                  }
                  break;
               }  // end dead rec switch

               if (netWorldLocation != nullptr) {
                  base::NetHandler::fromNetOrder(&worldLocation->x, netWorldLocation->x);
                  base::NetHandler::fromNetOrder(&worldLocation->y, netWorldLocation->y);
                  base::NetHandler::fromNetOrder(&worldLocation->z, netWorldLocation->z);
               }
               else {
                  worldLocation->x = 0;
                  worldLocation->y = 0;
                  worldLocation->z = 0;
               }

               if (netOrientation != nullptr) {
                  base::NetHandler::fromNetOrder(&orientation->phi, netOrientation->phi);
                  base::NetHandler::fromNetOrder(&orientation->theta, netOrientation->theta);
                  base::NetHandler::fromNetOrder(&orientation->psi, netOrientation->psi);
               }
               else {
                  orientation->phi = 0;
                  orientation->theta = 0;
                  orientation->psi = 0;
               }

               if (netVelocityVector != nullptr) {
                  base::NetHandler::fromNetOrder(&velocityVector->xVelocity, netVelocityVector->xVelocity);
                  base::NetHandler::fromNetOrder(&velocityVector->yVelocity, netVelocityVector->yVelocity);
                  base::NetHandler::fromNetOrder(&velocityVector->zVelocity, netVelocityVector->zVelocity);
               }
               else {
                  velocityVector->xVelocity = 0;
                  velocityVector->yVelocity = 0;
                  velocityVector->zVelocity = 0;
               }

               if (netAccelerationVector != nullptr) {
                  base::NetHandler::fromNetOrder(&accelerationVector->xAcceleration, netAccelerationVector->xAcceleration);
                  base::NetHandler::fromNetOrder(&accelerationVector->yAcceleration, netAccelerationVector->yAcceleration);
                  base::NetHandler::fromNetOrder(&accelerationVector->zAcceleration, netAccelerationVector->zAcceleration);
               }
               else {
                  accelerationVector->xAcceleration = 0;
                  accelerationVector->yAcceleration = 0;
                  accelerationVector->zAcceleration = 0;
               }

               if (netAngularVelocity != nullptr) {
                  base::NetHandler::fromNetOrder(&angularVelocity->xAngularVelocity, netAngularVelocity->xAngularVelocity);
                  base::NetHandler::fromNetOrder(&angularVelocity->yAngularVelocity, netAngularVelocity->yAngularVelocity);
                  base::NetHandler::fromNetOrder(&angularVelocity->zAngularVelocity, netAngularVelocity->zAngularVelocity);
               }
               else {
                  angularVelocity->xAngularVelocity = 0;
                  angularVelocity->yAngularVelocity = 0;
                  angularVelocity->zAngularVelocity = 0;
               }

               setAttributeUpdateRequiredFlag(NetIO::SPATIAL_AI, true);
            }
            break;

            case NetIO::FORCE_IDENTIFIER_AI : {
               unsigned char* net = reinterpret_cast<unsigned char*>(&netBuffer);
               physicalEntity->forceIdentifier = ForceIdentifierEnum8( *net );
               //std::cout << "Recv force: " << physicalEntity->forceIdentifier << std::endl;;
               setAttributeUpdateRequiredFlag(NetIO::FORCE_IDENTIFIER_AI, true);
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
      setAttributeUpdateRequiredFlag(NetIO::ENTITY_IDENTIFIER_AI, false);
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
      setAttributeUpdateRequiredFlag(NetIO::ENTITY_TYPE_AI, false);
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
      geocPos[base::Nav::IX] = spatialRvw->worldLocation.x;
      geocPos[base::Nav::IY] = spatialRvw->worldLocation.y;
      geocPos[base::Nav::IZ] = spatialRvw->worldLocation.z;

      osg::Vec3d geocVel;
      geocVel[base::Nav::IX] = spatialRvw->velocityVector.xVelocity;
      geocVel[base::Nav::IY] = spatialRvw->velocityVector.yVelocity;
      geocVel[base::Nav::IZ] = spatialRvw->velocityVector.zVelocity;

      osg::Vec3d geocAcc;
      geocAcc[base::Nav::IX] = spatialRvw->accelerationVector.xAcceleration;
      geocAcc[base::Nav::IY] = spatialRvw->accelerationVector.yAcceleration;
      geocAcc[base::Nav::IZ] = spatialRvw->accelerationVector.zAcceleration;

      // Get orientation orientation and rates
      osg::Vec3d geocAngles;
      geocAngles[base::Nav::IPHI] = spatialRvw->orientation.phi;
      geocAngles[base::Nav::ITHETA] = spatialRvw->orientation.theta;
      geocAngles[base::Nav::IPSI] = spatialRvw->orientation.psi; 

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

      setAttributeUpdateRequiredFlag(NetIO::SPATIAL_AI, false);
   }
    
   // PhysicalEntity pointer
   PhysicalEntity* physicalEntity = dynamic_cast<PhysicalEntity*>(baseEntity);
   if (physicalEntity != nullptr) {

      if (isAttributeUpdateRequired(NetIO::FORCE_IDENTIFIER_AI)) {
         // Side: When mapping Force ID to Player Side ...
         if (physicalEntity->forceIdentifier == FRIENDLY) {
            // Friendly's are blue, ...
            setSide(simulation::Player::BLUE);
         }
         else if (physicalEntity->forceIdentifier == OPPOSING) {
            // opposing side is red, ...
            setSide(simulation::Player::RED);
         }
         else if (physicalEntity->forceIdentifier == NEUTRAL) {
            // Neutrals are white, ...
            setSide(simulation::Player::WHITE);
         }
         else  {
            // and everyone else is gray.
            setSide(simulation::Player::GRAY);
         }
         setAttributeUpdateRequiredFlag(NetIO::FORCE_IDENTIFIER_AI, false);
      }

   }

   setMode(simulation::Player::ACTIVE);    
   setTimeExec( (double) getNetIO()->getCurrentTime() );
}

//------------------------------------------------------------------------------
// entityStateManager() --  (Output support)
//    -- Update the entity object for this NIB(Player)
//------------------------------------------------------------------------------
bool Nib::entityStateManager(const double curExecTime)
{
   bool ok = true;
   if (getPlayer()->isMode(simulation::Player::ACTIVE) && isPlayerStateUpdateRequired(curExecTime)) {

      // Need to update this entity object ...

      NetIO* netIO = static_cast<NetIO*>(getNetIO());
      RTI::RTIambassador* rtiAmb = netIO->getRTIambassador();

      // ---
      // First, make sure this entity has been registered
      // ---
      if (!isRegistered()) {
         try {
            RTI::ObjectClassHandle theClassHandle = netIO->getObjectClassHandle( getClassIndex() );
            makeObjectName();
            setObjectHandle( rtiAmb->registerObjectInstance( theClassHandle, getObjectName() ) );
            netIO->addNibToObjectTables(this, simulation::NetIO::OUTPUT_NIB);
            std::cout << "rprfom::Nib::updateEntity(): Register entity: " << getObjectName() << " handle = " << getObjectHandle() << std::endl;
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
            RTI::AttributeHandleValuePairSet* attrs = nullptr;
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
//    -- We send all published BasicEntity attributes every time.
//
//       The function isPlayerStateUpdateRequired() checking if the player's state
//       needs to be resent.  Therefore, we're not check the individual update
//       required flags with isAttributeUpdateRequired(), but we do clear them
//       with setAttributeUpdateRequiredFlag().
//
//   (Also handles the host to network byte swapping)
//------------------------------------------------------------------------------
void Nib::updateBasicEntity(
         RTI::AttributeHandleValuePairSet* attrs, 
         const double)
{
   if (baseEntity != nullptr) {
      // Our handler 
      NetIO* netIO = static_cast<NetIO*>(getNetIO());

      // Our simulation
      simulation::Simulation* ourSim = netIO->getSimulation();

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
         base::NetHandler::toNetOrder(&netBuffer.federateIdentifier.applicationID, entityId->federateIdentifier.applicationID );
         base::NetHandler::toNetOrder(&netBuffer.federateIdentifier.siteID, entityId->federateIdentifier.siteID );
         base::NetHandler::toNetOrder(&netBuffer.entityNumber, entityId->entityNumber );
         attrs->add(netIO->getObjectAttributeHandle(
               NetIO::ENTITY_IDENTIFIER_AI),
               reinterpret_cast<char*>(&netBuffer),
               sizeof(EntityIdentifierStruct)
            );

         setAttributeUpdateRequiredFlag(NetIO::ENTITY_IDENTIFIER_AI, false);
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
         base::NetHandler::toNetOrder(&netBuffer.countryCode, entityType->countryCode );
         attrs->add(netIO->getObjectAttributeHandle(
               NetIO::ENTITY_TYPE_AI),
               reinterpret_cast<char*>(&netBuffer),
               sizeof(EntityTypeStruct)
            );

         setAttributeUpdateRequiredFlag(NetIO::ENTITY_TYPE_AI, false);
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
         const unsigned int SPATIAL_NET_BUFFER_SIZE = sizeof(SpatialStruct) + sizeof(SpatialRVStruct);
         unsigned char netBuffer[SPATIAL_NET_BUFFER_SIZE];

         SpatialStruct* netSpatial = reinterpret_cast<SpatialStruct*>(&netBuffer[0]);
         SpatialRVStruct* netSpatialRvw = reinterpret_cast<SpatialRVStruct*>(&netBuffer[sizeof(SpatialStruct)]);

         // Ref Position 
         double refLat = 0.0;
         double refLon = 0.0;
         if (ourSim != nullptr) {
            refLat = ourSim->getRefLatitude();
            refLon = ourSim->getRefLongitude();
         }

         // Convert position vector to Lat/Lon/Alt
         double alt = 0.0;
         double simCoord[3] = { 0.0, 0.0, 0.0 };
         base::Nav::convertPosVec2LL(
               refLat, refLon, 
               pos,
               &simCoord[base::Nav::ILAT], &simCoord[base::Nav::ILON], &alt
            );
         simCoord[base::Nav::IALT] = alt;

         //std::cout << "RprFom::Nib::entityState2Nib(): simCoord(" << simCoord[Basic::Nav::ILAT] << "," << simCoord[Basic::Nav::ILON] << "," << simCoord[Basic::Nav::IALT] << ")"  << std::endl;

         // Convert to geocentric coordinates
         double geocPos[3] = { 0.0, 0.0, 0.0 };
         double geocVel[3] = { 0.0, 0.0, 0.0 };
         double geocAcc[3] = { 0.0, 0.0, 0.0 };
         base::Nav::getWorldPosAccVel(simCoord, vel.ptr(), accel.ptr(), geocPos, geocVel, geocAcc);

         // Dead reckoning algorithm
         {
            spatial->deadReckoningAlgorithm = DRM_RVW;

            netSpatial->deadReckoningAlgorithm = spatial->deadReckoningAlgorithm;
         }

         // Is Frozen?
         {
            bool simFrz = false;
            if (ourSim != nullptr) simFrz = ourSim->isFrozen();
            if (isFrozen() || simFrz) spatialRvw->isFrozen = RTI::RTI_TRUE;  // Is this object or the simulation frozen?
            else spatialRvw->isFrozen = RTI::RTI_FALSE;

            netSpatialRvw->isFrozen = spatialRvw->isFrozen;
         }

         // World Location
         {
            WorldLocationStruct* worldLocation = &spatialRvw->worldLocation;
            WorldLocationStruct* netWorldLocation = &netSpatialRvw->worldLocation;

            worldLocation->x = geocPos[base::Nav::IX];
            worldLocation->y = geocPos[base::Nav::IY];
            worldLocation->z = geocPos[base::Nav::IZ];

            base::NetHandler::toNetOrder(&netWorldLocation->x, worldLocation->x);
            base::NetHandler::toNetOrder(&netWorldLocation->y, worldLocation->y);
            base::NetHandler::toNetOrder(&netWorldLocation->z, worldLocation->z);
         }

         // Velocity vector
         {
            VelocityVectorStruct* velocityVector = &spatialRvw->velocityVector;
            VelocityVectorStruct* netVelocityVector = &netSpatialRvw->velocityVector;

            velocityVector->xVelocity = static_cast<RTI::Float>(geocVel[base::Nav::IX]);
            velocityVector->yVelocity = static_cast<RTI::Float>(geocVel[base::Nav::IY]);
            velocityVector->zVelocity = static_cast<RTI::Float>(geocVel[base::Nav::IZ]);

            base::NetHandler::toNetOrder(&netVelocityVector->xVelocity, velocityVector->xVelocity);
            base::NetHandler::toNetOrder(&netVelocityVector->yVelocity, velocityVector->yVelocity);
            base::NetHandler::toNetOrder(&netVelocityVector->zVelocity, velocityVector->zVelocity);
         }

         // Acceleration vector
         {
            AccelerationVectorStruct* accelerationVector = &spatialRvw->accelerationVector;
            AccelerationVectorStruct* netAccelerationVector = &netSpatialRvw->accelerationVector;

            accelerationVector->xAcceleration = static_cast<RTI::Float>(geocAcc[base::Nav::IX]);
            accelerationVector->yAcceleration = static_cast<RTI::Float>(geocAcc[base::Nav::IY]);
            accelerationVector->zAcceleration = static_cast<RTI::Float>(geocAcc[base::Nav::IZ]);

            base::NetHandler::toNetOrder(&netAccelerationVector->xAcceleration, accelerationVector->xAcceleration);
            base::NetHandler::toNetOrder(&netAccelerationVector->yAcceleration, accelerationVector->yAcceleration);
            base::NetHandler::toNetOrder(&netAccelerationVector->zAcceleration, accelerationVector->zAcceleration);
         }

         // Orientation
         {
            OrientationStruct* orientation = &spatialRvw->orientation;
            OrientationStruct* netOrientation = &netSpatialRvw->orientation;

            // Convert Euler angles to geocentric angles
            double geocAngles[3] = { 0.0, 0.0, 0.0 };
            base::Nav::getGeocAngle(simCoord, angles.ptr(), geocAngles);

            orientation->phi   = static_cast<RTI::Float>(geocAngles[base::Nav::IPHI]);
            orientation->theta = static_cast<RTI::Float>(geocAngles[base::Nav::ITHETA]);
            orientation->psi   = static_cast<RTI::Float>(geocAngles[base::Nav::IPSI]);

            base::NetHandler::toNetOrder(&netOrientation->phi, orientation->phi);
            base::NetHandler::toNetOrder(&netOrientation->theta, orientation->theta);
            base::NetHandler::toNetOrder(&netOrientation->psi, orientation->psi);
         }

         // Angular velocity vector (all zeros for now)
         {
            AngularVelocityVectorStruct* angularVelocityVector = &spatialRvw->angularVelocity;
            AngularVelocityVectorStruct* netAngularVelocityVector = &netSpatialRvw->angularVelocity;

            angularVelocityVector->xAngularVelocity = 0;
            angularVelocityVector->yAngularVelocity = 0;
            angularVelocityVector->zAngularVelocity = 0;

            base::NetHandler::toNetOrder(&netAngularVelocityVector->xAngularVelocity, angularVelocityVector->xAngularVelocity);
            base::NetHandler::toNetOrder(&netAngularVelocityVector->yAngularVelocity, angularVelocityVector->yAngularVelocity);
            base::NetHandler::toNetOrder(&netAngularVelocityVector->zAngularVelocity, angularVelocityVector->zAngularVelocity);
         }

         attrs->add(
               netIO->getObjectAttributeHandle(NetIO::SPATIAL_AI),
               reinterpret_cast<char*>(&netBuffer),
               SPATIAL_NET_BUFFER_SIZE
            );

         setAttributeUpdateRequiredFlag(NetIO::SPATIAL_AI, false);
      }
   }
}

//------------------------------------------------------------------------------
// updatePhysicalEntity() -- (Output support)
//    -- sets the PhysicalEntity attribute values that need to be updated
//
//    -- We send all published PhysicalEntity attributes every time.
//       The function isPlayerStateUpdateRequired() check if the player's state
//       needs to be re-sent.  Therefore, we're not checking the individual update
//       required flags with isAttributeUpdateRequired(), but we do clear them
//       with setAttributeUpdateRequiredFlag().
//
//   (Also handles the host to network byte swapping)
//------------------------------------------------------------------------------
void Nib::updatePhysicalEntity(
         RTI::AttributeHandleValuePairSet* attrs, 
         const double)
{
   // PhysicalEntity??
   PhysicalEntity* physicalEntity =  dynamic_cast<PhysicalEntity*>(baseEntity);
   if (physicalEntity != nullptr) {
      // Our handler 
      NetIO* netIO = static_cast<NetIO*>(getNetIO());

      // Force Identifier
      if (isAttributeUpdateEnabled(NetIO::FORCE_IDENTIFIER_AI)) {

         // Force ID: When mapping Player side to force IDs ...
         if (getSide() == simulation::Player::BLUE) {
            // blue's are friendly, ...
            physicalEntity->forceIdentifier = FRIENDLY;     
         }
         else if (getSide() == simulation::Player::RED) {
            // red's are not, ...
            physicalEntity->forceIdentifier = OPPOSING;
         }
         else if (getSide() == simulation::Player::WHITE) {
            // white is neutral, ...
            physicalEntity->forceIdentifier = NEUTRAL;
         }
         else {
            // and everyone else is type OTHER.
            physicalEntity->forceIdentifier = OTHER;
         }

         //std::cout << "Send force: " << physicalEntity->forceIdentifier << std::endl;;

         unsigned char netBuffer = static_cast<unsigned char>(physicalEntity->forceIdentifier);  // 8 bits enum type (ForceIdentifierEnum8)
         attrs->add(netIO->getObjectAttributeHandle(
               NetIO::FORCE_IDENTIFIER_AI),
               reinterpret_cast<char*>(&netBuffer),
               sizeof(netBuffer)
            );

         setAttributeUpdateRequiredFlag(NetIO::FORCE_IDENTIFIER_AI, false);
      }
   }
}

//------------------------------------------------------------------------------
// updatePlatform() -- (Output support)
//    -- sets the Platform attribute values that need to be updated
//
//    -- We send all published Platform attributes every time.
//       The function isPlayerStateUpdateRequired() check if the player's state
//       needs to be re-sent.  Therefore, we're not checking the individual update
//       required flags with isAttributeUpdateRequired(), but we do clear them
//       with setAttributeUpdateRequiredFlag().
//
//   (Also handles the host to network byte swapping)
//------------------------------------------------------------------------------
void Nib::updatePlatform(
         RTI::AttributeHandleValuePairSet*,
         const double)
{
   // No Platform attributes are published at this time
}


}
}
