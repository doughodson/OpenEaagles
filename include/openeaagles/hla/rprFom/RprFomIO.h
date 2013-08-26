//------------------------------------------------------------------------------
// Class: RprFomIO
//
// Real-time Platform Reference FOM (RPR-FOM) Data Structures
// RPR-FOM Version: 0.1.7
//------------------------------------------------------------------------------
#ifndef __RprFomIO_H_D18424CB_4EA3_4c1c_9BE7_DD24B6433B04__
#define __RprFomIO_H_D18424CB_4EA3_4c1c_9BE7_DD24B6433B04__

#include "eaagles/hla/HlaIO.h"

namespace Eaagles {
   namespace Simulation { class Player; }

namespace Network {
namespace Hla {
   class Ambassador;

namespace RprFom {
   class BaseEntity;
   class EmitterBeam;
   class Nib;

//==============================================================================
// Class:       RprFomIO
// Base class:	... -> Simulation::NetworkIO -> HlaIO -> RprFomIO
// GUID: {D18424CB-4EA3-4c1c-9BE7-DD24B6433B04}
//==============================================================================
class DLLEXP RprFomIO : public HlaIO  
{
    DECLARE_SUBCLASS(RprFomIO,HlaIO)

public:
    // Object Class Indexes
    enum {
        BASE_ENTITY_CLASS = 1,
        PHYSICAL_ENTITY_CLASS = 2,
        MILITARY_ENTITY_CLASS = 3,
        SOLDIER_CLASS = 4,
        MUNITION_ENTITY_CLASS = 5,
        MILITARY_PLATFORM_ENTITY_CLASS = 6,
        MILITARY_SUBMERSIBLE_PLATFORM_CLASS = 7,
        MILITARY_SEASURFACE_PLATFORM_CLASS = 8,
        MILITARY_SPACE_PLATFORM_CLASS = 9,
        MILITARY_LAND_PLATFORM_CLASS = 10,
        MILITARY_AMPHIBIOUS_PLATFORM_CLASS = 11,
        MILITARY_AIRLAND_PLATFORM_CLASS = 12,
        
        NUM_OBJECT_CLASSES = 12
    };

    // Object Attribute Indexes
    enum {
        // Base Entity (BE) arguments indexes (AI)
        ACCELERATION_VECTOR_BE_AI = 1,
        ANGULAR_VELOCITY_VECTOR_BE_AI = 2,
        DEAD_RECKONING_ALGORITHM_BE_AI = 3,
        ENTITY_TYPE_BE_AI = 4,
        FEDERATE_ID_BE_AI = 5,
        IS_FROZEN_BE_AI = 6,
        ORIENTATION_BE_AI = 7,
        POSITION_BE_AI = 8,
        VELOCITY_VECTOR_BE_AI = 9,
        
        // (some) Physical Entity (PE) arguments indexes (AI)
        DAMAGE_STATE_PE_AI = 10,
        
        // (some) Military Entity (PE) arguments indexes (AI)
        FORCE_IDENTIFIER_ME_AI = 11,
        
        NUM_OBJECT_ATTRIBUTES = 11
    };
    
    // Interaction Class Indexes
    enum {
        WEAPON_FIRE_INTERACTION = 1,
        MUNITION_DETONATION_INTERACTION = 2,
        
        NUM_INTERACTION_CLASSES = 2
    };

    // Interaction Parameter Indexes
    enum {
        // Weapon Fire (WF) parameter indexes (PI)
        EVENT_ID_WF_PI = 1,
        FIRE_CONTROL_SOLUTION_RANGE_WF_PI = 2,
        FIRE_MISSION_INDEX_WF_PI = 3,
        FIRING_LOCATION_WF_PI = 4,
        FIRING_OBJECT_ID_WF_PI = 5,
        FUSE_TYPE_WF_PI = 6,
        INITIAL_VELOCITY_VECTOR_WF_PI = 7,
        MUNITION_OBJECT_ID_WF_PI = 8,
        MUNITION_TYPE_WF_PI = 9,
        QUANTITY_FIRED_WF_PI = 10,
        RATE_OF_FIRE_WF_PI = 11,
        TARGET_OBJECT_ID_WF_PI = 12,
        WARHEAD_TYPE_WF_PI = 13,


        // Munition Detonation (MD) parameter indexes (PI)
        DETONATION_LOCATION_MD_PI = 14,
        DETONATION_RESULT_CODE_MD_PI = 15,
        EVENT_ID_MD_PI = 16,
        FINAL_VELOCITY_VECTOR_MD_PI = 17,
        FIRING_OBJECT_ID_MD_PI = 18,
        FUSE_TYPE_MD_PI = 19,
        MUNITION_OBJECT_ID_MD_PI = 20,
        MUNITION_TYPE_MD_PI = 21,
        QUANTITY_FIRED_MD_PI = 22,
        RATE_OF_FIRE_MD_PI = 23,
        RELATIVE_DETONATION_LOCATION_MD_PI = 24,
        TARGET_OBJECT_ID_MD_PI = 25,
        WARHEAD_TYPE_MD_PI = 26,
        
        NUM_INTERACTION_PARAMETER = 26
    };

public:
    RprFomIO();

    // HlaIO interface
    virtual int getNumberOfObjectClasses() const;
    virtual int getNumberOfObjectAttributes() const;
    virtual int getNumberOfOInteractionClasses() const;
    virtual int getNumberOfInteractionParameters() const;
    virtual void discoverObjectInstance(
        const RTI::ObjectHandle theObject, const RTI::ObjectClassHandle theObjectClass, const char* theObjectName
    );
    virtual void receiveInteraction(
        const RTI::InteractionClassHandle theInteraction,
        const RTI::ParameterHandleValuePairSet& theParameters
    );

    // Simulation::NetworkIO interface
    virtual Simulation::Nib* createNewOutputNib(Simulation::Player* const player);
    
protected:
    virtual bool receiveWeaponFire(const RTI::ParameterHandleValuePairSet& theParameters);
    virtual bool receiveMunitionDetonation(const RTI::ParameterHandleValuePairSet& theParameters);

    virtual bool publishAndSubscribePlatforms();
    virtual bool publishAndSubscribeEmissions();
    virtual bool publishAndSubscribeWeaponFire();
    virtual bool publishAndSubscribeMunitionDetonation();

    // HlaIO interface (callbacks)
    virtual bool publishAndSubscribe();
    
    // Simulation::NetworkIO Interface (Callbacks)
    virtual void processInputList();    // Update players/systems from the Input-list
    virtual Simulation::Nib* nibFactory(const Simulation::NetworkIO::IoType ioType);   // Create a new RprFom::Nib
};

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace

#endif // __RprFomIO_H_D18424CB_4EA3_4c1c_9BE7_DD24B6433B04__
