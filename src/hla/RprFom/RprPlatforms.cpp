#include "eaagles/hla/rprFom/RprFomIO.h"
#include "eaagles/hla/rprFom/RprFom.h"
#include "eaagles/hla/rprFom/RprFomNib.h"
#include "eaagles/hla/Ambassador.h"

#include "eaagles/simulation/Player.h"
#include "eaagles/simulation/Simulation.h"
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
bool RprFomIO::publishAndSubscribePlatforms()
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
            p->getObjectClassHandle(RprFom::BaseEntity::getClassFedName());

         setObjectClassHandle(BASE_ENTITY_CLASS, basicEntityClassHandle );

         setObjectAttributeHandle(
            WORLD_LOCATION_BE_AI,
            p->getAttributeHandle(
               RprFom::BaseEntity::getWorldLocationAttributeFedName(), 
               basicEntityClassHandle)
         );

         setObjectAttributeHandle(
            ENTITY_TYPE_BE_AI,
            p->getAttributeHandle(
               RprFom::BaseEntity::getEntityTypeAttributeFedName(),
               basicEntityClassHandle)
         );

         setObjectAttributeHandle(
            ENTITY_IDENTIFIER_BE_AI,
            p->getAttributeHandle(
               RprFom::BaseEntity::getEntityIdentifierAttributeFedName(),
               basicEntityClassHandle)
         );

         setObjectAttributeHandle(
            IS_FROZEN_BE_AI,
            p->getAttributeHandle(
               RprFom::BaseEntity::getIsFrozenAttributeFedName(), 
               basicEntityClassHandle)
         );

         setObjectAttributeHandle(
            DEAD_RECKONING_ALGORITHM_BE_AI,
            p->getAttributeHandle(
               RprFom::BaseEntity::getDeadReckoningAlgorithmAttributeFedName(), 
               basicEntityClassHandle)
         );

         setObjectAttributeHandle(
            ORIENTATION_BE_AI,
            p->getAttributeHandle(
               RprFom::BaseEntity::getOrientationAttributeFedName(), 
               basicEntityClassHandle)
         );

         setObjectAttributeHandle(
            VELOCITY_VECTOR_BE_AI,
            p->getAttributeHandle(
               RprFom::BaseEntity::getVelocityVectorAttributeFedName(), 
               basicEntityClassHandle)
         );

         setObjectAttributeHandle(
            ANGULAR_VELOCITY_VECTOR_BE_AI,
            p->getAttributeHandle(
               RprFom::BaseEntity::getAngularVelocityVectorAttributeFedName(), 
               basicEntityClassHandle)
         );

         setObjectAttributeHandle(
            ACCELERATION_VECTOR_BE_AI,
            p->getAttributeHandle(
               RprFom::BaseEntity::getAccelerationVectorAttributeFedName(), 
               basicEntityClassHandle)
         );
      }

      // ---
      // Physical entity class and attribute handles
      // ---
      {
         RTI::ObjectClassHandle physicalEntityClassHandle =
            p->getObjectClassHandle(RprFom::PhysicalEntity::getClassFedName());

         setObjectClassHandle(PHYSICAL_ENTITY_CLASS, physicalEntityClassHandle );

         setObjectAttributeHandle(
            FORCE_IDENTIFIER_PE_AI,
            p->getAttributeHandle(
               RprFom::PhysicalEntity::getForceIdentifierAttributeFedName(), 
               physicalEntityClassHandle) 
         );
      }

      // ---
      // Platform entity class and attribute handles
      // ---
      {
         RTI::ObjectClassHandle platformClassHandle =
            p->getObjectClassHandle(RprFom::Platform::getClassFedName());

         setObjectClassHandle(PLATFORM_CLASS, platformClassHandle );
         // No attributes at this time ...
      }

      // ---
      // Aircraft class handle
      // ---
      setObjectClassHandle(
         AIRCRAFT_CLASS,
         p->getObjectClassHandle(RprFom::Aircraft::getClassFedName())
      );

      // ---
      // Missile class handle
      // ---
      setObjectClassHandle(
         MISSILE_CLASS,
         p->getObjectClassHandle(RprFom::Missile::getClassFedName())
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
         attrs->add(getObjectAttributeHandle(WORLD_LOCATION_BE_AI));
         attrs->add(getObjectAttributeHandle(ENTITY_TYPE_BE_AI));
         attrs->add(getObjectAttributeHandle(ENTITY_IDENTIFIER_BE_AI));
         attrs->add(getObjectAttributeHandle(IS_FROZEN_BE_AI));
         attrs->add(getObjectAttributeHandle(DEAD_RECKONING_ALGORITHM_BE_AI));
         attrs->add(getObjectAttributeHandle(ORIENTATION_BE_AI));
         attrs->add(getObjectAttributeHandle(VELOCITY_VECTOR_BE_AI));
         attrs->add(getObjectAttributeHandle(ANGULAR_VELOCITY_VECTOR_BE_AI));
         attrs->add(getObjectAttributeHandle(ACCELERATION_VECTOR_BE_AI));
         attrs->add(getObjectAttributeHandle(FORCE_IDENTIFIER_PE_AI));

         if (isOutputEnabled()) {
            p->publishObjectClass(getObjectClassHandle(AIRCRAFT_CLASS), *attrs);
            setObjectClassPublished(AIRCRAFT_CLASS,true);

            p->publishObjectClass(getObjectClassHandle(MISSILE_CLASS), *attrs);
            setObjectClassPublished(MISSILE_CLASS,true);
         }

         // ---
         // Subscribe to these entity classes and attributes, and request an update.
         // ---
         //  Note: For now we subscribe to the same attribute list that we publish!
         if (isInputEnabled()) {
            p->subscribeObjectClassAttributes(getObjectClassHandle(AIRCRAFT_CLASS), *attrs);
            p->requestClassAttributeValueUpdate(getObjectClassHandle(AIRCRAFT_CLASS), *attrs);
            setObjectClassSubscribed(AIRCRAFT_CLASS, true);

            p->subscribeObjectClassAttributes(getObjectClassHandle(MISSILE_CLASS), *attrs);
            p->requestClassAttributeValueUpdate(getObjectClassHandle(MISSILE_CLASS), *attrs);
            setObjectClassSubscribed(MISSILE_CLASS, true);
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
// Class: RprFom::Nib
//==============================================================================

//------------------------------------------------------------------------------
// reflectAttributeValues() -- (Input support)
//  Called by our FederateAmbassador to update the attribute values for
//  this object instance.  (Also handles the network to host byte swapping)
//------------------------------------------------------------------------------
void RprFom::Nib::reflectAttributeValues(const RTI::AttributeHandleValuePairSet& theAttrs)
{
    RprFomIO* rprIO = (RprFomIO*)(getNetworkIO());
    if (rprIO != 0 && baseEntity != 0) {
    
        // PhysicalEntity pointer
        RprFom::PhysicalEntity* physicalEntity = dynamic_cast<RprFom::PhysicalEntity*>(baseEntity);

        RTI::ULong length;
        char netBuffer[1000];
        for (RTI::ULong j = 0 ; j < theAttrs.size(); j++ ) {
            
            // get the attribute's handle and data (network byte order)
            RTI::AttributeHandle theAttr = theAttrs.getHandle(j);
            theAttrs.getValue(j, netBuffer, length);

            // Process the attribute
            switch ( rprIO->findAttributeIndex(theAttr) ) {
            
                // Update Federate ID
                case RprFomIO::ENTITY_IDENTIFIER_BE_AI : {
                    RprFom::EntityIdentifierStruct* net = (RprFom::EntityIdentifierStruct*) &netBuffer;

                    Basic::NetHandler::fromNetOrder(&baseEntity->entityIdentifier.federateIdentifier.applicationID, net->federateIdentifier.applicationID );                
                    Basic::NetHandler::fromNetOrder(&baseEntity->entityIdentifier.federateIdentifier.siteID, net->federateIdentifier.siteID );
                    Basic::NetHandler::fromNetOrder(&baseEntity->entityIdentifier.entityNumber, net->entityNumber );
                
                    setAttributeUpdateRequiredFlag(RprFomIO::ENTITY_IDENTIFIER_BE_AI,true);
                    //std::cout << "Recv Federate ID: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
                }
                break;

                // Update Entity Type
                case RprFomIO::ENTITY_TYPE_BE_AI : {
                    RprFom::EntityTypeStruct* net = (RprFom::EntityTypeStruct*) &netBuffer;
                    
                    // All bytes except for country
                    baseEntity->entityType = *net;
                    Basic::NetHandler::fromNetOrder(&baseEntity->entityType.countryCode, net->countryCode );
                    
                    setAttributeUpdateRequiredFlag(RprFomIO::ENTITY_TYPE_BE_AI,true);

                     //std::cout << "Recv EntityType: type: (";
                     //std::cout << int( baseEntity->entityType.category ) << ", ";
                     //std::cout << int( baseEntity->entityType.countryCode ) << ", ";
                     //std::cout << int( baseEntity->entityType.domain ) << ", ";
                     //std::cout << int( baseEntity->entityType.entityKind ) << ", ";
                     //std::cout << int( baseEntity->entityType.extra ) << ", ";
                     //std::cout << int( baseEntity->entityType.specific ) << ", ";
                     //std::cout << int( baseEntity->entityType.subcategory ) << ")" << std::endl;
                }
                break;

                // Update IsFrozen
                case RprFomIO::IS_FROZEN_BE_AI : {
                    RTI::Boolean* net = (RTI::Boolean*) &netBuffer;
                    baseEntity->isFrozen = *net;
                    
                    setAttributeUpdateRequiredFlag(RprFomIO::IS_FROZEN_BE_AI,true);
                    //std::cout << "Recv IsFrozen: " << baseEntity->isFrozen << std::endl;
                }
                break;

                // Update entity position
                case RprFomIO::WORLD_LOCATION_BE_AI : {
                    RprFom::WorldLocationStruct* net = (RprFom::WorldLocationStruct*) &netBuffer;
                    
                    Basic::NetHandler::fromNetOrder(&baseEntity->worldLocation.x, net->x);
                    Basic::NetHandler::fromNetOrder(&baseEntity->worldLocation.y, net->y);
                    Basic::NetHandler::fromNetOrder(&baseEntity->worldLocation.z, net->z);
                    
                    setAttributeUpdateRequiredFlag(RprFomIO::WORLD_LOCATION_BE_AI,true);
                    //std::cout << "Recv Position: (" << baseEntity->worldLocation.x << "," << baseEntity->worldLocation.y << "," << baseEntity->worldLocation.z << ")" << std::endl;
                }
                break;

                // Update entity velocity
                case RprFomIO::VELOCITY_VECTOR_BE_AI : {
                    RprFom::VelocityVectorStruct* net = (RprFom::VelocityVectorStruct*) &netBuffer;
                    
                    Basic::NetHandler::fromNetOrder(&baseEntity->velocityVector.xVelocity, net->xVelocity);
                    Basic::NetHandler::fromNetOrder(&baseEntity->velocityVector.yVelocity, net->yVelocity);
                    Basic::NetHandler::fromNetOrder(&baseEntity->velocityVector.zVelocity, net->zVelocity);
                    
                    setAttributeUpdateRequiredFlag(RprFomIO::VELOCITY_VECTOR_BE_AI,true);
                    //std::cout << "Recv Velocity: (" << baseEntity->velocityVector.xVelocity << "," << baseEntity->velocityVector.yVelocity << "," << baseEntity->velocityVector.zVelocity << ")" << std::endl;
                }
                break;

                // Update entity acceleration
                case RprFomIO::ACCELERATION_VECTOR_BE_AI : {
                    RprFom::AccelerationVectorStruct* net = (RprFom::AccelerationVectorStruct*) &netBuffer;

                    Basic::NetHandler::fromNetOrder(&baseEntity->accelerationVector.xAcceleration, net->xAcceleration);
                    Basic::NetHandler::fromNetOrder(&baseEntity->accelerationVector.yAcceleration, net->yAcceleration);
                    Basic::NetHandler::fromNetOrder(&baseEntity->accelerationVector.zAcceleration, net->zAcceleration);

                    setAttributeUpdateRequiredFlag(RprFomIO::ACCELERATION_VECTOR_BE_AI,true);
                    //std::cout << "Recv Acceleration: (" << baseEntity->accelerationVector.xAcceleration << "," << baseEntity->accelerationVector.yAcceleration << "," << baseEntity->accelerationVector.zAcceleration << ")" << std::endl;
                }
                break;
                
                // Update entity orientation
                case RprFomIO::ORIENTATION_BE_AI : {
                    RprFom::OrientationStruct* net = (RprFom::OrientationStruct*) &netBuffer;

                    Basic::NetHandler::fromNetOrder(&baseEntity->orientation.phi, net->phi);
                    Basic::NetHandler::fromNetOrder(&baseEntity->orientation.theta, net->theta);
                    Basic::NetHandler::fromNetOrder(&baseEntity->orientation.psi, net->psi);

                    setAttributeUpdateRequiredFlag(RprFomIO::ORIENTATION_BE_AI,true);
                    //std::cout << "Recv Orientation: (" << baseEntity->orientation.phi << "," << baseEntity->orientation.theta << "," << baseEntity->orientation.psi << ")" << std::endl;
                }
                break;

                // Update entity angular velocity
                case RprFomIO::ANGULAR_VELOCITY_VECTOR_BE_AI : {
                    RprFom::AngularVelocityVectorStruct* net = (RprFom::AngularVelocityVectorStruct*) &netBuffer;

                    Basic::NetHandler::fromNetOrder(&baseEntity->angularVelocityVector.xAngularVelocity, net->xAngularVelocity);
                    Basic::NetHandler::fromNetOrder(&baseEntity->angularVelocityVector.yAngularVelocity, net->yAngularVelocity);
                    Basic::NetHandler::fromNetOrder(&baseEntity->angularVelocityVector.zAngularVelocity, net->zAngularVelocity);

                    setAttributeUpdateRequiredFlag(RprFomIO::ANGULAR_VELOCITY_VECTOR_BE_AI,true);
                    //std::cout << "Recv AngularVelocity: (" << baseEntity->angularVelocityVector.xAngularVelocity << "," << baseEntity->angularVelocityVector.yAngularVelocity << "," << baseEntity->angularVelocityVector.zAngularVelocity << ")" << std::endl;
                }
                break;

                case RprFomIO::DEAD_RECKONING_ALGORITHM_BE_AI : {
                    unsigned char* net = (unsigned char*) &netBuffer;
                    
                    baseEntity->deadReckoningAlgorithm = RprFom::DeadReckoningAlgorithmEnum8( *net );

                    //std::cout << "Recv dra: " << baseEntity->deadReckoningAlgorithm << std::endl;;
                    
                    setAttributeUpdateRequiredFlag(RprFomIO::DEAD_RECKONING_ALGORITHM_BE_AI,true);
                }
                break;

                case RprFomIO::FORCE_IDENTIFIER_PE_AI : {
                    unsigned char* net = (unsigned char*) &netBuffer;

                    
                    physicalEntity->forceIdentifier = RprFom::ForceIdentifierEnum8( *net );
                    //std::cout << "Recv force: " << physicalEntity->forceIdentifier << std::endl;;
                    
                    setAttributeUpdateRequiredFlag(RprFomIO::FORCE_IDENTIFIER_PE_AI,true);
                }
                break;
                
            }
        }
            
        // Update the basic NIB state data with this new data
        entityState2Nib();
    }
}


//------------------------------------------------------------------------------
// entityState2Nib() -- (Input support)
//  Transfers data from the incoming entity attributes to the basic
//  NIB class variables.
//------------------------------------------------------------------------------
void RprFom::Nib::entityState2Nib()
{    

    if (isAttributeUpdateRequired(RprFomIO::ENTITY_IDENTIFIER_BE_AI)) {
        setPlayerID(baseEntity->entityIdentifier.entityNumber);
        setSiteID(baseEntity->entityIdentifier.federateIdentifier.siteID);
        setApplicationID(baseEntity->entityIdentifier.federateIdentifier.applicationID);
        setAttributeUpdateRequiredFlag(RprFomIO::ENTITY_IDENTIFIER_BE_AI,false);
        haveEntityIdFlg = true;
    }
    
    if (isAttributeUpdateRequired(RprFomIO::ENTITY_TYPE_BE_AI)) {
        setKind( baseEntity->entityType.entityKind );
        setDomain( baseEntity->entityType.domain );
        setCountry( baseEntity->entityType.countryCode );
        setCategory( baseEntity->entityType.category );
        setSubcategory( baseEntity->entityType.subcategory );
        setSpecific( baseEntity->entityType.specific );
        setExtra( baseEntity->entityType.extra );
        setAttributeUpdateRequiredFlag(RprFomIO::ENTITY_TYPE_BE_AI,false);
        haveEntityTypeFlg = true;
    }
    
    // Need location, velocity vector and acceleration vector
    if ( isAttributeUpdateRequired(RprFomIO::WORLD_LOCATION_BE_AI) ) {

        // Get the reference lat/lon
        double refLat = 0.0;
        double refLon = 0.0;
        if (getNetworkIO()->getSimulation() != 0) {
            refLat = getNetworkIO()->getSimulation()->getRefLatitude();
            refLon = getNetworkIO()->getSimulation()->getRefLongitude();
        }

        // Get the geocentric position, velocity and acceleration
        double geocPos[3];
        geocPos[Basic::Nav::IX] = baseEntity->worldLocation.x;
        geocPos[Basic::Nav::IY] = baseEntity->worldLocation.y;
        geocPos[Basic::Nav::IZ] = baseEntity->worldLocation.z;
        
        LCreal geocVel[3];
        geocVel[Basic::Nav::IX] = baseEntity->velocityVector.xVelocity;
        geocVel[Basic::Nav::IY] = baseEntity->velocityVector.yVelocity;
        geocVel[Basic::Nav::IZ] = baseEntity->velocityVector.zVelocity;
      
        LCreal geocAcc[3];
        geocAcc[Basic::Nav::IX] = baseEntity->accelerationVector.xAcceleration;
        geocAcc[Basic::Nav::IY] = baseEntity->accelerationVector.yAcceleration;
        geocAcc[Basic::Nav::IZ] = baseEntity->accelerationVector.zAcceleration;
      
        // Convert geocentric coordinates to geodetic
        osg::Vec3 vel( 0, 0, 0 );
        osg::Vec3 accel( 0, 0, 0 );
        Basic::Nav::getSimPosAccVel(geocPos, geocVel, geocAcc, geodPos, vel.ptr(), accel.ptr() );
        //std::cout << "RprFom::Nib::entityState2Nib(): geodPos(" << geodPos[Basic::Nav::ILAT] << "," << geodPos[Basic::Nav::ILON] << "," << geodPos[Basic::Nav::IALT] << ")"  << std::endl;

        // Convert geodetic to position vector
        osg::Vec3 pos;
        Basic::Nav::convertLL2PosVec(refLat, refLon, geodPos[Basic::Nav::ILAT], geodPos[Basic::Nav::ILON], geodPos[Basic::Nav::IALT], &pos);

        // Store the position
        setPosition(pos);
        setAttributeUpdateRequiredFlag(RprFomIO::WORLD_LOCATION_BE_AI,false);
        //std::cout << "RprFom::Nib::entityState2Nib(): setPosition(" << pos[0] << "," << pos[1] << "," << pos[2] << ")"  << std::endl;
        haveWorldLocationFlg = true;
        
        // Store the velocity 
        if ( isAttributeUpdateRequired(RprFomIO::VELOCITY_VECTOR_BE_AI) ) {
            setVelocity(vel);
            setAttributeUpdateRequiredFlag(RprFomIO::VELOCITY_VECTOR_BE_AI,false);
        //std::cout << "RprFom::Nib::entityState2Nib(): setVelocity(" << vel[0] << "," << vel[1] << "," << vel[2] << ")"  << std::endl;
        }
        
        // Store the acceleration
        if ( isAttributeUpdateRequired(RprFomIO::ACCELERATION_VECTOR_BE_AI) ) {
            setAcceleration(accel);
            setAttributeUpdateRequiredFlag(RprFomIO::ACCELERATION_VECTOR_BE_AI,false);
        //std::cout << "RprFom::Nib::entityState2Nib(): setAcceleration(" << accel[0] << "," << accel[1] << "," << accel[2] << ")"  << std::endl;
        }
    }

    if (isAttributeUpdateRequired(RprFomIO::ORIENTATION_BE_AI) && haveWorldLocationFlg) {
        
        // Get geocentric angles
        LCreal geocAngles[3];
        geocAngles[Basic::Nav::IPHI] = baseEntity->orientation.phi;
        geocAngles[Basic::Nav::ITHETA] = baseEntity->orientation.theta;
        geocAngles[Basic::Nav::IPSI] = baseEntity->orientation.psi; 
        
        // Convert geocentric angles to geodetic
        osg::Vec3 angles( 0, 0, 0);
        Basic::Nav::getGeodAngle(geodPos, geocAngles, angles.ptr());
        setEulerAngles(angles);
        //std::cout << "RprFom::Nib::entityState2Nib(): geodPos2(" << geodPos[Basic::Nav::ILAT] << "," << geodPos[Basic::Nav::ILON] << "," << geodPos[Basic::Nav::IALT] << ")"  << std::endl;
        //std::cout << "RprFom::Nib::entityState2Nib(): setEulerAngles(" << (angles[0]*LcAngle::R2DCC) << "," << (angles[1]*LcAngle::R2DCC) << "," << (angles[2]*LcAngle::R2DCC) << ")"  << std::endl;
        
        setAttributeUpdateRequiredFlag(RprFomIO::ORIENTATION_BE_AI,false);
        haveOrientationFlg = true;
        
        if (isAttributeUpdateRequired(RprFomIO::ANGULAR_VELOCITY_VECTOR_BE_AI)) {
            osg::Vec3 arates(0.0, 0.0, 0.0);
            setAngularVelocities(arates);
            setAttributeUpdateRequiredFlag(RprFomIO::ANGULAR_VELOCITY_VECTOR_BE_AI,false);
        }
    
    }

    if (isAttributeUpdateRequired(RprFomIO::IS_FROZEN_BE_AI)) {
        freeze(baseEntity->isFrozen != 0);
        setAttributeUpdateRequiredFlag(RprFomIO::IS_FROZEN_BE_AI,false);
    }
    
    // PhysicalEntity pointer
    RprFom::PhysicalEntity* physicalEntity = dynamic_cast<RprFom::PhysicalEntity*>(baseEntity);
    if (physicalEntity != 0) {
    
        if (isAttributeUpdateRequired(RprFomIO::FORCE_IDENTIFIER_PE_AI)) {
            // Side: When mapping Force ID to Player Side ...
            if (physicalEntity->forceIdentifier == RprFom::FRIENDLY) {
                // Friendly's are blue, ...
                setSide(Simulation::Player::BLUE);
            }
            else if (physicalEntity->forceIdentifier == RprFom::OPPOSING) {
                // opposing side is red, ...
                setSide(Simulation::Player::RED);
            }
            else if (physicalEntity->forceIdentifier == RprFom::NEUTRAL) {
                // Neutrals are white, ...
                setSide(Simulation::Player::WHITE);
            }
            else  {
                // and everyone else is gray.
                setSide(Simulation::Player::GRAY);
            }
            setAttributeUpdateRequiredFlag(RprFomIO::FORCE_IDENTIFIER_PE_AI,false);
        }
        
    }
    
    setMode(Simulation::Player::ACTIVE);    
    setTimeExec( getNetworkIO()->getCurrentTime() );
}

//------------------------------------------------------------------------------
// entityStateManager() --  (Output support)
//    -- Update the entity object for this NIB(Player)
//------------------------------------------------------------------------------
bool RprFom::Nib::entityStateManager(const LCreal curExecTime)
{
   bool ok = true;
   if (getPlayer()->isMode(Simulation::Player::ACTIVE) && isPlayerStateUpdateRequired(curExecTime)) {
    
      // Need to update this entity object ...

      RprFomIO* rprIO = (RprFomIO*)(getNetworkIO());
      RTI::RTIambassador* rtiAmb = rprIO->getRTIambassador();

      // ---
      // First, make sure this entity has been registered
      // ---
      if (!isRegistered()) {
         try {
            RTI::ObjectClassHandle theClassHandle = rprIO->getObjectClassHandle( getClassIndex() );
            makeObjectName();
            setObjectHandle( rtiAmb->registerObjectInstance( theClassHandle, getObjectName() ) );
            rprIO->addNibToObjectTables(this, Simulation::NetworkIO::OUTPUT_NIB);
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
            RTI::AttributeHandleValuePairSet* attrs = NULL;
            attrs = RTI::AttributeSetFactory::create( RprFomIO::NUM_OBJECT_ATTRIBUTES );

            // Load the set with updated attribute values
            updateBasicEntity(attrs,curExecTime);
            updatePhysicalEntity(attrs,curExecTime);
            updatePlatform(attrs,curExecTime);

            // Send attributes to the RTI
            //std::cout << "RprFom::Nib::updateEntity(): Update entity: " << getObjectName() << " handle = " << getObjectHandle() << std::endl;
            ok = rprIO->updateAttributeValues(getObjectHandle(), attrs);

            delete attrs;
         }

         catch (RTI::Exception& e) {
            std::cerr << &e << std::endl;
            ok = false;
         }
      }
   }

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
void RprFom::Nib::updateBasicEntity(
         RTI::AttributeHandleValuePairSet* attrs, 
         const LCreal curExecTime)
{
   if (baseEntity != 0) {
      // Our handler 
      RprFomIO* rprIO = (RprFomIO*)(getNetworkIO());

      // Our simulation
      Simulation::Simulation* ourSim = rprIO->getSimulation();

      // Get the player data
      playerState2Nib(curExecTime);

      osg::Vec3 pos = getPosition();
      osg::Vec3 vel = getVelocity();
      osg::Vec3 accel = getAcceleration();
      osg::Vec3 angles = getEulerAngles();
      osg::Vec3 arates = getAngularVelocities();

      // Entity ID
      if (isAttributeUpdateEnabled(RprFomIO::ENTITY_IDENTIFIER_BE_AI)) {
         RprFom::EntityIdentifierStruct* entityId = &baseEntity->entityIdentifier;
         entityId->federateIdentifier.siteID = getSiteID();
         entityId->federateIdentifier.applicationID = getApplicationID();
         entityId->entityNumber = getPlayerID();
         //std::cout << "Send Federate ID: (" << entityId->entityNumber << "," << entityId->federateIdentifier.applicationID << "," << entityId->federateIdentifier.siteID << ")" << std::endl;

         RprFom::EntityIdentifierStruct netBuffer;
         Basic::NetHandler::toNetOrder(&netBuffer.federateIdentifier.applicationID, entityId->federateIdentifier.applicationID );                
         Basic::NetHandler::toNetOrder(&netBuffer.federateIdentifier.siteID, entityId->federateIdentifier.siteID );
         Basic::NetHandler::toNetOrder(&netBuffer.entityNumber, entityId->entityNumber );
         attrs->add(rprIO->getObjectAttributeHandle(
            RprFomIO::ENTITY_IDENTIFIER_BE_AI),
            (char*)&netBuffer,
            sizeof(RprFom::EntityIdentifierStruct));

         setAttributeUpdateRequiredFlag(RprFomIO::ENTITY_IDENTIFIER_BE_AI,false);
      }

      // Entity Type
      if (isAttributeUpdateEnabled(RprFomIO::ENTITY_TYPE_BE_AI)) {

         RprFom::EntityTypeStruct* entityType = &baseEntity->entityType;
         entityType->entityKind   = getKind();
         entityType->domain       = getDomain();
         entityType->countryCode  = getCountry();
         entityType->category     = getCategory();
         entityType->subcategory  = getSubcategory();
         entityType->specific     = getSpecific();
         entityType->extra        = getExtra();

         //std::cout << "Send EntityType: type: (";
         //std::cout << int( entityType->category ) << ", ";
         //std::cout << int( entityType->countryCode ) << ", ";
         //std::cout << int( entityType->domain ) << ", ";
         //std::cout << int( entityType->entityKind ) << ", ";
         //std::cout << int( entityType->extra ) << ", ";
         //std::cout << int( entityType->specific ) << ", ";
         //std::cout << int( entityType->subcategory ) << ")" << std::endl;

         // Network byte order: all bytes except country code which is unsigned short.
         RprFom::EntityTypeStruct netBuffer = *entityType;
         Basic::NetHandler::toNetOrder(&netBuffer.countryCode, entityType->countryCode );
         attrs->add(rprIO->getObjectAttributeHandle(
            RprFomIO::ENTITY_TYPE_BE_AI),
            (char*)&netBuffer,
            sizeof(RprFom::EntityTypeStruct));

         setAttributeUpdateRequiredFlag(RprFomIO::ENTITY_TYPE_BE_AI,false);
      }

      // Is Frozen?
      if (isAttributeUpdateEnabled(RprFomIO::IS_FROZEN_BE_AI)) {
         bool simFrz = false;
         if (ourSim != 0) simFrz = ourSim->isFrozen();
         if (isFrozen() || simFrz) baseEntity->isFrozen = RTI::RTI_TRUE;  // Is this object or the simulation frozen?
         else baseEntity->isFrozen = RTI::RTI_FALSE;

         //std::cout << "Send IsFrozen: " << baseEntity->isFrozen << std::endl;

         attrs->add(rprIO->getObjectAttributeHandle(
            RprFomIO::IS_FROZEN_BE_AI),
            (char*)&baseEntity->isFrozen,
            sizeof(RTI::Boolean));

         setAttributeUpdateRequiredFlag(RprFomIO::IS_FROZEN_BE_AI,false);
      }

      // Dead reckoning algorithm
      if (isAttributeUpdateEnabled(RprFomIO::DEAD_RECKONING_ALGORITHM_BE_AI)) {
         baseEntity->deadReckoningAlgorithm = RprFom::DRM_RVW;

         //std::cout << "Send dra: " << baseEntity->deadReckoningAlgorithm << std::endl;;

         unsigned char netBuffer = (unsigned char)(baseEntity->deadReckoningAlgorithm);  // 8 bits enum type (DeadReckoningAlgorithmEnum8)
         attrs->add(rprIO->getObjectAttributeHandle(
            RprFomIO::DEAD_RECKONING_ALGORITHM_BE_AI),
            (char*)&netBuffer,
            sizeof(netBuffer));

         setAttributeUpdateRequiredFlag(RprFomIO::DEAD_RECKONING_ALGORITHM_BE_AI,false);
      }


      // Ref Position 
      double refLat = 0.0;
      double refLon = 0.0;
      if (ourSim != 0) {
         refLat = ourSim->getRefLatitude();
         refLon = ourSim->getRefLongitude();
      }

      // Convert position vector to Lat/Lon/Alt
      LCreal alt;
      double simCoord[3] = { 0, 0, 0 };
      Basic::Nav::convertPosVec2LL(
         refLat, refLon, 
         pos,
         &simCoord[Basic::Nav::ILAT], &simCoord[Basic::Nav::ILON], &alt);
      simCoord[Basic::Nav::IALT] = alt;

      //std::cout << "RprFom::Nib::entityState2Nib(): simCoord(" << simCoord[Basic::Nav::ILAT] << "," << simCoord[Basic::Nav::ILON] << "," << simCoord[Basic::Nav::IALT] << ")"  << std::endl;

      // Convert to geocentric coordinates
      double geocPos[3] = { 0, 0, 0 };
      LCreal geocVel[3] = { 0, 0, 0 };
      LCreal geocAcc[3] = { 0, 0, 0 };
      Basic::Nav::getWorldPosAccVel(simCoord, vel.ptr(), accel.ptr(), geocPos, geocVel, geocAcc);

      // Position
      if (isAttributeUpdateEnabled(RprFomIO::WORLD_LOCATION_BE_AI)) {
         RprFom::WorldLocationStruct* worldLocation = &baseEntity->worldLocation;
         worldLocation->x = geocPos[Basic::Nav::IX];
         worldLocation->y = geocPos[Basic::Nav::IY];
         worldLocation->z = geocPos[Basic::Nav::IZ];

        //std::cout << "updateBasicEntity(): getPosition(" << pos[0] << "," << pos[1] << "," << pos[2] << ")"  << std::endl;
        //std::cout << "Send Position: (" << baseEntity->worldLocation.x << "," << baseEntity->worldLocation.y << "," << baseEntity->worldLocation.z << ")" << std::endl;

         RprFom::WorldLocationStruct netBuffer;
         Basic::NetHandler::toNetOrder(&netBuffer.x, worldLocation->x);
         Basic::NetHandler::toNetOrder(&netBuffer.y, worldLocation->y);
         Basic::NetHandler::toNetOrder(&netBuffer.z, worldLocation->z);
         attrs->add(rprIO->getObjectAttributeHandle(
            RprFomIO::WORLD_LOCATION_BE_AI),
            (char*)&netBuffer,
            sizeof(RprFom::WorldLocationStruct));

         setAttributeUpdateRequiredFlag(RprFomIO::WORLD_LOCATION_BE_AI,false);
      }

      // Velocity vector
         if (isAttributeUpdateEnabled(RprFomIO::VELOCITY_VECTOR_BE_AI)) {
         RprFom::VelocityVectorStruct* velocityVector = &baseEntity->velocityVector;
         velocityVector->xVelocity = geocVel[Basic::Nav::IX];
         velocityVector->yVelocity = geocVel[Basic::Nav::IY];
         velocityVector->zVelocity = geocVel[Basic::Nav::IZ];

        //std::cout << "updateBasicEntity(): getVelocity(" << vel[0] << "," << vel[1] << "," << vel[2] << ")"  << std::endl;
        // std::cout << "Send Velocity: (" << baseEntity->velocityVector.xVelocity << "," << baseEntity->velocityVector.yVelocity << "," << baseEntity->velocityVector.zVelocity << ")" << std::endl;

         RprFom::VelocityVectorStruct netBuffer;
         Basic::NetHandler::toNetOrder(&netBuffer.xVelocity, velocityVector->xVelocity);
         Basic::NetHandler::toNetOrder(&netBuffer.yVelocity, velocityVector->yVelocity);
         Basic::NetHandler::toNetOrder(&netBuffer.zVelocity, velocityVector->zVelocity);
         attrs->add(rprIO->getObjectAttributeHandle(
            RprFomIO::VELOCITY_VECTOR_BE_AI),
            (char*)&netBuffer,
            sizeof(RprFom::VelocityVectorStruct));

         setAttributeUpdateRequiredFlag(RprFomIO::VELOCITY_VECTOR_BE_AI,false);
      }

      // Acceleration vector
      if (isAttributeUpdateEnabled(RprFomIO::ACCELERATION_VECTOR_BE_AI)) {
         RprFom::AccelerationVectorStruct* accelerationVector = &baseEntity->accelerationVector;
         accelerationVector->xAcceleration = geocAcc[Basic::Nav::IX];
         accelerationVector->yAcceleration = geocAcc[Basic::Nav::IY];
         accelerationVector->zAcceleration = geocAcc[Basic::Nav::IZ];

        //std::cout << "updateBasicEntity(): getAccel(" << accel[0] << "," << accel[1] << "," << accel[2] << ")"  << std::endl;
        //std::cout << "Send Acceleration: (" << baseEntity->accelerationVector.xAcceleration << "," << baseEntity->accelerationVector.yAcceleration << "," << baseEntity->accelerationVector.zAcceleration << ")" << std::endl;

         RprFom::AccelerationVectorStruct netBuffer;
         Basic::NetHandler::toNetOrder(&netBuffer.xAcceleration, accelerationVector->xAcceleration);
         Basic::NetHandler::toNetOrder(&netBuffer.yAcceleration, accelerationVector->yAcceleration);
         Basic::NetHandler::toNetOrder(&netBuffer.zAcceleration, accelerationVector->zAcceleration);
         attrs->add(rprIO->getObjectAttributeHandle(
            RprFomIO::ACCELERATION_VECTOR_BE_AI),
            (char*)&netBuffer,
            sizeof(RprFom::AccelerationVectorStruct));

         setAttributeUpdateRequiredFlag(RprFomIO::ACCELERATION_VECTOR_BE_AI,false);
      }

      // Orientation
      if (isAttributeUpdateEnabled(RprFomIO::ORIENTATION_BE_AI)) {
      //std::cout << "RprFom::Nib::entityState2Nib(): simCoord2(" << simCoord[Basic::Nav::ILAT] << "," << simCoord[Basic::Nav::ILON] << "," << simCoord[Basic::Nav::IALT] << ")"  << std::endl;

         // Convert euler angles to geocentric angles
         LCreal geocAngles[3] = { 0, 0, 0 };
         Basic::Nav::getGeocAngle(simCoord, angles.ptr(), geocAngles);

         RprFom::OrientationStruct* orientation = &baseEntity->orientation;
         orientation->phi = geocAngles[Basic::Nav::IPHI];
         orientation->theta = geocAngles[Basic::Nav::ITHETA];
         orientation->psi = geocAngles[Basic::Nav::IPSI];

        //std::cout << "updateBasicEntity(): getAngles(" << angles[0] << "," << angles[1] << "," << angles[2] << ")" << std::endl;
        //std::cout << "Send Orientation: (" << orientation->phi << "," << orientation->theta << "," << orientation->psi << ")" << std::endl;

         RprFom::OrientationStruct netBuffer;
         Basic::NetHandler::toNetOrder(&netBuffer.phi, orientation->phi);
         Basic::NetHandler::toNetOrder(&netBuffer.theta, orientation->theta);
         Basic::NetHandler::toNetOrder(&netBuffer.psi, orientation->psi);
         attrs->add(rprIO->getObjectAttributeHandle(
            RprFomIO::ORIENTATION_BE_AI),
            (char*)&netBuffer,
            sizeof(RprFom::OrientationStruct));

         setAttributeUpdateRequiredFlag(RprFomIO::ORIENTATION_BE_AI,false);
      }

      // Angular velocity vector (all zeros for now)
      if (isAttributeUpdateEnabled(RprFomIO::ANGULAR_VELOCITY_VECTOR_BE_AI)) {
         RprFom::AngularVelocityVectorStruct* angularVelocityVector = &baseEntity->angularVelocityVector;
         angularVelocityVector->xAngularVelocity = 0;
         angularVelocityVector->yAngularVelocity = 0;
         angularVelocityVector->zAngularVelocity = 0;

         //std::cout << "Send AngularVelocity: (" << baseEntity->angularVelocityVector.xAngularVelocity << "," << baseEntity->angularVelocityVector.yAngularVelocity << "," << baseEntity->angularVelocityVector.zAngularVelocity << ")" << std::endl;

         RprFom::AngularVelocityVectorStruct netBuffer;
         Basic::NetHandler::toNetOrder(&netBuffer.xAngularVelocity, angularVelocityVector->xAngularVelocity);
         Basic::NetHandler::toNetOrder(&netBuffer.yAngularVelocity, angularVelocityVector->yAngularVelocity);
         Basic::NetHandler::toNetOrder(&netBuffer.zAngularVelocity, angularVelocityVector->zAngularVelocity);
         attrs->add(rprIO->getObjectAttributeHandle(
            RprFomIO::ANGULAR_VELOCITY_VECTOR_BE_AI),
            (char*)&angularVelocityVector,
            sizeof(RprFom::AngularVelocityVectorStruct));

         setAttributeUpdateRequiredFlag(RprFomIO::ANGULAR_VELOCITY_VECTOR_BE_AI,false);
      }

      //     
      ////###  strncpy(pdu.entityMarking.marking,nib->getPlayerName(),10);

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
void RprFom::Nib::updatePhysicalEntity(
         RTI::AttributeHandleValuePairSet* attrs, 
         const LCreal)
{

   // PhysicalEntity??
   RprFom::PhysicalEntity* physicalEntity =  dynamic_cast<RprFom::PhysicalEntity*>(baseEntity);

   if (physicalEntity != 0) {
      // Our handler 
      RprFomIO* rprIO = (RprFomIO*)(getNetworkIO());

      // Force Identifier
      if (isAttributeUpdateEnabled(RprFomIO::FORCE_IDENTIFIER_PE_AI)) {

         // Force ID: When mapping Player side to force IDs ...
         if (getSide() == Simulation::Player::BLUE) {
            // blue's are friendly, ...
            physicalEntity->forceIdentifier = RprFom::FRIENDLY;     
         }
         else if (getSide() == Simulation::Player::RED) {
            // red's are not, ...
            physicalEntity->forceIdentifier = RprFom::OPPOSING;
         }
         else if (getSide() == Simulation::Player::WHITE) {
            // white is neutral, ...
            physicalEntity->forceIdentifier = RprFom::NEUTRAL;
         }
         else {
            // and everyone else is type OTHER.
            physicalEntity->forceIdentifier = RprFom::OTHER;
         }

         //std::cout << "Send force: " << physicalEntity->forceIdentifier << std::endl;;

         unsigned char netBuffer = (unsigned char)(physicalEntity->forceIdentifier);  // 8 bits enum type (ForceIdentifierEnum8)
         attrs->add(rprIO->getObjectAttributeHandle(
            RprFomIO::FORCE_IDENTIFIER_PE_AI),
            (char*)&netBuffer,
            sizeof(netBuffer));

         setAttributeUpdateRequiredFlag(RprFomIO::FORCE_IDENTIFIER_PE_AI,false);
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
void RprFom::Nib::updatePlatform(
         RTI::AttributeHandleValuePairSet* attrs,
         const LCreal curExecTime)
{
   // No Platform attributes are published at this time
}


} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
