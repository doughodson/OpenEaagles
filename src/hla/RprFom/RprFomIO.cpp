#include "eaagles/hla/rprFom/RprFomIO.h"
#include "eaagles/hla/rprFom/RprFom.h"
#include "eaagles/hla/rprFom/RprFomNib.h"
#include "eaagles/hla/Ambassador.h"

#include "eaagles/simulation/AirVehicle.h"
#include "eaagles/simulation/Missile.h"
#include "eaagles/simulation/Player.h"
#include "eaagles/simulation/Simulation.h"
#include "eaagles/simulation/Signatures.h"
#include "eaagles/simulation/Weapon.h"
#include "eaagles/basic/Pair.h"
#include "eaagles/basic/PairStream.h"
#include "eaagles/basic/String.h"
#include "eaagles/basic/Nav.h"
#include "eaagles/basic/NetHandler.h"
#include "eaagles/basic/Number.h"

//==============================================================================
// Class: HlaIO
//==============================================================================
namespace Eaagles {
namespace Network {
namespace Hla {
namespace RprFom {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(RprFomIO,"RprFomIO")
EMPTY_SERIALIZER(RprFomIO)


//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
RprFomIO::RprFomIO()
{
    SET_SLOTTABLE
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void RprFomIO::copyData(const RprFomIO& org, const bool cc)
{
    BaseClass::copyData(org);
    
    if (cc) {
    }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void RprFomIO::deleteData()
{
    BaseClass::deleteData();
}

//------------------------------------------------------------------------------
// getNumberOfObjectClasses() -- Number of object classses we're using
//------------------------------------------------------------------------------
int RprFomIO::getNumberOfObjectClasses() const
{
    return NUM_OBJECT_CLASSES;
}

//------------------------------------------------------------------------------
// getNumberOfObjectAttributes() -- Number of object attributes we're using
//------------------------------------------------------------------------------
int RprFomIO::getNumberOfObjectAttributes() const
{
    return NUM_OBJECT_ATTRIBUTES;
}

//------------------------------------------------------------------------------
// getNumberOfOInteractionClasses() -- Number of interaction classses we're using
//------------------------------------------------------------------------------
int RprFomIO::getNumberOfOInteractionClasses() const
{
    return NUM_INTERACTION_CLASSES;
}

//------------------------------------------------------------------------------
// getNumberOfInteractionParameters() -- Number of interaction parameters we're using
//------------------------------------------------------------------------------
int RprFomIO::getNumberOfInteractionParameters() const
{
    return NUM_INTERACTION_PARAMETER;
}

//------------------------------------------------------------------------------
// Create a new output NIB
//------------------------------------------------------------------------------
Simulation::Nib* RprFomIO::createNewOutputNib(Simulation::Player* const player)
{
    // ---
    // Check if we are enabled to register this class of objects and 
    // create the proper RPR-FOM class structure
    // ---
    BaseEntity* baseEntity = 0;
    if (player->isClassType(typeid(Simulation::AirVehicle))) {
        if (isObjectClassRegistrationEnabled( MILITARY_AIRLAND_PLATFORM_CLASS )) {
            baseEntity = new MilitaryAirLandPlatform();
        }
    }
    else if (player->isClassType(typeid(Simulation::Missile))) {
        if (isObjectClassRegistrationEnabled( MUNITION_ENTITY_CLASS )) {
            baseEntity = new MunitionEntity();
        }
    }
    
    // ---
    // If enabled, create and set the output NIB
    // ---
    RprFom::Nib* nib = 0;
    if (baseEntity != 0) {
        nib = (RprFom::Nib*) nibFactory(Simulation::NetworkIO::OUTPUT_NIB);
        if (nib != 0) {
           nib->setBaseEntity(baseEntity);
           nib->setNetworkIO(this);
           nib->setPlayer(player);
           nib->setOutputPlayerType(player);
        }
        baseEntity->unref();  // the NIB has it
    }
    return nib;
}


//------------------------------------------------------------------------------
// publishAndSubscribe()
//------------------------------------------------------------------------------
bool RprFomIO::publishAndSubscribe()
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
   if (ok) ok = publishAndSubscribeEmissions();
   if (ok) ok = publishAndSubscribeWeaponFire();
   if (ok) ok = publishAndSubscribeMunitionDetonation();

   return ok;
}

//------------------------------------------------------------------------------
// processInputList() -- Update players/systems from the Input-list
//------------------------------------------------------------------------------
void RprFomIO::processInputList()
{
   for (unsigned int idx = 0; idx < getInputListSize(); idx++) {
      RprFom::Nib* nib = (RprFom::Nib*)( getInputNib(idx) );
      if (nib != 0) nib->updateTheIPlayer();
   }
}

//------------------------------------------------------------------------------
// nibFactory() -- Create a new NIB
//------------------------------------------------------------------------------
Simulation::Nib* RprFomIO::nibFactory(const Simulation::NetworkIO::IoType ioType)
{
    return new RprFom::Nib(ioType);
}

//------------------------------------------------------------------------------
// discoverObjectInstance() -- handle the discover of an object
//------------------------------------------------------------------------------
void RprFomIO::discoverObjectInstance(
        const RTI::ObjectHandle theObject,
        const RTI::ObjectClassHandle theObjectClass,
        const char* theObjectName)
{
   int idx = findObjectClassIndex(theObjectClass);
   switch (idx) {
      case MILITARY_ENTITY_CLASS :
      case MUNITION_ENTITY_CLASS :
      case MILITARY_AIRLAND_PLATFORM_CLASS :
         {
            RprFom::Nib* nib = dynamic_cast<RprFom::Nib*>( createNewInputNib() );
            if (nib != 0) {
               nib->setObjectHandle(theObject);
               nib->setObjectName(theObjectName);
               nib->setClassIndex(idx);
               nib->setTimeExec( (LCreal) getCurrentTime() );
               BaseEntity* baseEntity = new MilitaryEntity();
               nib->setBaseEntity(baseEntity);
               baseEntity->unref();  // (NIB has it now)
               addNib2InputList(nib);
               addNibToObjectTables(nib, INPUT_NIB);
               nib->unref();
            }
         }
         break;
   };
}

//------------------------------------------------------------------------------
// receiveInteraction() -- handle the discover of an object
//------------------------------------------------------------------------------
void RprFomIO::receiveInteraction(
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
