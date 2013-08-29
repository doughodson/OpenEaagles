#include "eaagles/hla/mc02Fom/Mc02FomIO.h"
#include "eaagles/hla/mc02Fom/Mc02Fom.h"
#include "eaagles/hla/mc02Fom/Mc02FomNib.h"
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
namespace Mc02Fom {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Mc02FomIO,"Mc02FomIO")
EMPTY_SERIALIZER(Mc02FomIO)


//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
Mc02FomIO::Mc02FomIO()
{
    SET_SLOTTABLE
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Mc02FomIO::copyData(const Mc02FomIO& org, const bool cc)
{
    BaseClass::copyData(org);
    
    if (cc) {
    }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Mc02FomIO::deleteData()
{
    BaseClass::deleteData();
}

//------------------------------------------------------------------------------
// getNumberOfObjectClasses() -- Number of object classses we're using
//------------------------------------------------------------------------------
int Mc02FomIO::getNumberOfObjectClasses() const
{
    return NUM_OBJECT_CLASSES;
}

//------------------------------------------------------------------------------
// getNumberOfObjectAttributes() -- Number of object attributes we're using
//------------------------------------------------------------------------------
int Mc02FomIO::getNumberOfObjectAttributes() const
{
    return NUM_OBJECT_ATTRIBUTES;
}

//------------------------------------------------------------------------------
// getNumberOfOInteractionClasses() -- Number of interaction classses we're using
//------------------------------------------------------------------------------
int Mc02FomIO::getNumberOfOInteractionClasses() const
{
    return NUM_INTERACTION_CLASSES;
}

//------------------------------------------------------------------------------
// getNumberOfInteractionParameters() -- Number of interaction parameters we're using
//------------------------------------------------------------------------------
int Mc02FomIO::getNumberOfInteractionParameters() const
{
    return NUM_INTERACTION_PARAMETER;
}

//------------------------------------------------------------------------------
// Create a new output NIB
//------------------------------------------------------------------------------
Simulation::Nib* Mc02FomIO::createNewOutputNib(Simulation::Player* const player)
{
    // ---
    // Check if we are enabled to register this class of objects and 
    // create the proper MC02 class structure
    // ---
    BaseEntity* baseEntity = 0;
    if (player->isClassType(typeid(Simulation::AirVehicle))) {
        if (isObjectClassRegistrationEnabled( AIRCRAFT_CLASS )) {
            baseEntity = new Mc02Fom::Aircraft();
        }
    }
    else if (player->isClassType(typeid(Simulation::Missile))) {
        if (isObjectClassRegistrationEnabled( MISSILE_CLASS )) {
            baseEntity = new Mc02Fom::Missile();
        }
    }
    
    // ---
    // If enabled, create and set the output NIB
    // ---
    Mc02Fom::Nib* nib = 0;
    if (baseEntity != 0) {
        nib = (Mc02Fom::Nib*) nibFactory(Simulation::NetworkIO::OUTPUT_NIB);
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
bool Mc02FomIO::publishAndSubscribe()
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
void Mc02FomIO::processInputList()
{
   for (unsigned int idx = 0; idx < getInputListSize(); idx++) {
      Mc02Fom::Nib* nib = (Mc02Fom::Nib*)( getInputNib(idx) );
      if (nib != 0) nib->updateTheIPlayer();
   }
}

//------------------------------------------------------------------------------
// nibFactory() -- Create a new NIB
//------------------------------------------------------------------------------
Simulation::Nib* Mc02FomIO::nibFactory(const Simulation::NetworkIO::IoType ioType)
{
    return new Mc02Fom::Nib(ioType);
}

//------------------------------------------------------------------------------
// discoverObjectInstance() -- handle the discover of an object
//------------------------------------------------------------------------------
void Mc02FomIO::discoverObjectInstance(
        const RTI::ObjectHandle theObject,
        const RTI::ObjectClassHandle theObjectClass,
        const char* theObjectName)
{
   int idx = findObjectClassIndex(theObjectClass);
   switch (idx) {
      case PLATFORM_CLASS :
      case AIRCRAFT_CLASS :
      case MISSILE_CLASS :
         {
            Mc02Fom::Nib* nib = dynamic_cast<Mc02Fom::Nib*>( createNewInputNib() );
            if (nib != 0) {
               nib->setObjectHandle(theObject);
               nib->setObjectName(theObjectName);
               nib->setClassIndex(idx);
               nib->setTimeExec( (LCreal) getCurrentTime() );
               BaseEntity* baseEntity = new Mc02Fom::Platform();
               nib->setBaseEntity(baseEntity);
               baseEntity->unref();  // (NIB has it now)
               addNib2InputList(nib);
               addNibToObjectTables(nib, INPUT_NIB);
               nib->unref();
            }
         }
         break;
      case EMITTER_BEAM_CLASS :
      case JAMMER_BEAM_CLASS :
      case RADAR_BEAM_CLASS :
         //{
         //   EmitterBeamNib* nib = new EmitterBeamNib();
         //   nib->setObjectHandle(theObject);
         //   nib->setObjectName(theObjectName);
         //   nib->setClassIndex(idx);
         //   nib->setTimeExec( getCurrentTime() );
         //   nib->setJammerBeamFlag( idx == JAMMER_BEAM_CLASS );
         //   EmitterBeam* emitterBeam = new Mc02::EmitterBeam();
         //   nib->setEmitterBeam(emitterBeam);
         //   emitterBeam->unref();  // (NIB has it now)
         //   addNibToObjectTables(nib, INPUT_NIB);
         //   nib->unref();
         //}
         break;
   };
}

//------------------------------------------------------------------------------
// receiveInteraction() -- handle the discover of an object
//------------------------------------------------------------------------------
void Mc02FomIO::receiveInteraction(
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

} // End Mc02Fom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
