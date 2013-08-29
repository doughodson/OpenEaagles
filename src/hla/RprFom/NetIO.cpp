#include "openeaagles/hla/rprFom/NetIO.h"
#include "openeaagles/hla/rprFom/Nib.h"
#include "openeaagles/hla/rprFom/RprFom.h"
#include "openeaagles/hla/Ambassador.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/GroundVehicle.h"
#include "openeaagles/simulation/LifeForms.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Ships.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Signatures.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Number.h"

//==============================================================================
// Class: NetIO
//==============================================================================
namespace Eaagles {
namespace Network {
namespace Hla {
namespace RprFom {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(NetIO,"RprFomNetIO")
EMPTY_SERIALIZER(NetIO)


//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
NetIO::NetIO()
{
    SET_SLOTTABLE
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void NetIO::copyData(const NetIO& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void NetIO::deleteData()
{
}

//------------------------------------------------------------------------------
// getNumberOfObjectClasses() -- Number of object classses we're using
//------------------------------------------------------------------------------
unsigned int NetIO::getNumberOfObjectClasses() const
{
    return NUM_OBJECT_CLASSES;
}

//------------------------------------------------------------------------------
// getNumberOfObjectAttributes() -- Number of object attributes we're using
//------------------------------------------------------------------------------
unsigned int NetIO::getNumberOfObjectAttributes() const
{
    return NUM_OBJECT_ATTRIBUTES;
}

//------------------------------------------------------------------------------
// getNumberOfOInteractionClasses() -- Number of interaction classses we're using
//------------------------------------------------------------------------------
unsigned int NetIO::getNumberOfOInteractionClasses() const
{
    return NUM_INTERACTION_CLASSES;
}

//------------------------------------------------------------------------------
// getNumberOfInteractionParameters() -- Number of interaction parameters we're using
//------------------------------------------------------------------------------
unsigned int NetIO::getNumberOfInteractionParameters() const
{
    return NUM_INTERACTION_PARAMETER;
}

//------------------------------------------------------------------------------
// Create a new output NIB
//------------------------------------------------------------------------------
Simulation::Nib* NetIO::createNewOutputNib(Simulation::Player* const player)
{
    // ---
    // Check if we are enabled to register this class of objects and 
    // create the proper FOM class structure
    // ---
    unsigned int idx = 0;
    BaseEntity* baseEntity = 0;
    if (player->isClassType(typeid(Simulation::AirVehicle))) {
        if (isObjectClassRegistrationEnabled( AIRCRAFT_CLASS )) {
            baseEntity = new Aircraft();
            idx = AIRCRAFT_CLASS;
        }
    }
    else if (player->isClassType(typeid(Simulation::Missile))) {
        if (isObjectClassRegistrationEnabled( MUNITION_CLASS )) {
            baseEntity = new Munition();
            idx = MUNITION_CLASS;
        }
    }
    else if (player->isClassType(typeid(Simulation::LifeForm))) {
        if (isObjectClassRegistrationEnabled( HUMAN_CLASS )) {
            baseEntity = new Human();
            idx = HUMAN_CLASS;
        }
    }
    else if (player->isClassType(typeid(Simulation::GroundVehicle))) {
        if (isObjectClassRegistrationEnabled( GROUND_VEHICLE_CLASS )) {
            baseEntity = new GroundVehicle();
            idx = GROUND_VEHICLE_CLASS;
        }
    }
    else if (player->isClassType(typeid(Simulation::Ship))) {
        if (isObjectClassRegistrationEnabled( SURFACE_VESSEL_CLASS )) {
            baseEntity = new SurfaceVessel();
            idx = SURFACE_VESSEL_CLASS;
        }
    }

    // ---
    // If enabled, create and set the output NIB
    // ---
    Nib* nib = 0;
    if (baseEntity != 0) {
        nib = (Nib*) nibFactory(Simulation::NetIO::OUTPUT_NIB);
        if (nib != 0) {
           nib->setBaseEntity(baseEntity);
           nib->setNetIO(this);
           nib->setPlayer(player);
           nib->setClassIndex(idx);
           nib->setOutputPlayerType(player);
        }
        baseEntity->unref();  // the NIB should have it
    }
    return nib;
}


//------------------------------------------------------------------------------
// publishAndSubscribe()
//------------------------------------------------------------------------------
bool NetIO::publishAndSubscribe()
{
   bool ok = true;

   // ----------
   // We want the callbacks for attribute level advisories
   // ----------
   try {
      std::cout << "*** Enable Attribute Advisories" << std::endl;
      getRTIambassador()->enableAttributeRelevanceAdvisorySwitch();
   }
   catch (RTI::Exception& e) {
      std::cerr << &e << std::endl;
      ok = false;
   }

   // ----------
   // Get handles to the class, attributes and parameters
   // and publish & Subscribe to class attributes
   // ----------

   if (ok) ok = publishAndSubscribePlatforms();
   if (ok) ok = publishAndSubscribeWeaponFire();
   if (ok) ok = publishAndSubscribeMunitionDetonation();

   return ok;
}

//------------------------------------------------------------------------------
// processInputList() -- Update players/systems from the Input-list
//------------------------------------------------------------------------------
void NetIO::processInputList()
{
   for (unsigned int idx = 0; idx < getInputListSize(); idx++) {
      Nib* nib = (Nib*)( getInputNib(idx) );
      if (nib != 0) nib->updateTheIPlayer();
   }
}

//------------------------------------------------------------------------------
// nibFactory() -- Create a new NIB
//------------------------------------------------------------------------------
Simulation::Nib* NetIO::nibFactory(const Simulation::NetIO::IoType ioType)
{
    return new Nib(ioType);
}

//------------------------------------------------------------------------------
// discoverObjectInstance() -- handle the discover of an object
//------------------------------------------------------------------------------
void NetIO::discoverObjectInstance(
        const RTI::ObjectHandle theObject,
        const RTI::ObjectClassHandle theObjectClass,
        const char* theObjectName)
{
   BaseEntity* baseEntity = 0;

   unsigned int idx = findObjectClassIndex(theObjectClass);
   switch (idx) {
      case AIRCRAFT_CLASS :
         baseEntity = new Aircraft();
         break;
      case MUNITION_CLASS :
         baseEntity = new Munition();
         break;
      case HUMAN_CLASS :
         baseEntity = new Human();
         break;
      case GROUND_VEHICLE_CLASS :
         baseEntity = new GroundVehicle();
         break;
      case SURFACE_VESSEL_CLASS :
         baseEntity = new SurfaceVessel();
         break;
   };

   if (baseEntity != 0) {
      Nib* nib = dynamic_cast<Nib*>( createNewInputNib() );
      if (nib != 0) {
         nib->setObjectHandle(theObject);
         nib->setObjectName(theObjectName);
         nib->setClassIndex(idx);
         nib->setTimeExec( (LCreal) getCurrentTime() );
         nib->setBaseEntity(baseEntity);
         addNib2InputList(nib);
         addNibToObjectTables(nib, INPUT_NIB);
         nib->unref();
      }
      baseEntity->unref();  // (NIB has it now)
   }
}

//------------------------------------------------------------------------------
// receiveInteraction() -- handle the discover of an object
//------------------------------------------------------------------------------
void NetIO::receiveInteraction(
    const RTI::InteractionClassHandle theInteraction,
    const RTI::ParameterHandleValuePairSet& theParameters)
{
    // Select the proper method to handle this interaction
    switch( findInteractionClassIndex(theInteraction) ) {
    
        case WEAPON_FIRE_INTERACTION :
            receiveWeaponFire(theParameters);
            break;
            
        case MUNITION_DETONATION_INTERACTION :
            receiveMunitionDetonation(theParameters);
            break;
            
    };
}

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
