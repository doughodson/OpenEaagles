//------------------------------------------------------------------------------
// Class: NetIO
// Description: Portions of class defined to support entity state
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

         // General purpose attribute handle set used to publish and subscribe to class attributes
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

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
