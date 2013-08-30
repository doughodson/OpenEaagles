//------------------------------------------------------------------------------
// Millenium Challenge 2002 FOM
// Version: RPR 1.0 Draft 2 w/extensions: $Revision: 1.6
//
// Classes: Mc02FomIO
//------------------------------------------------------------------------------
#ifndef __Mc02FomIO_H_AD1D1AE8_768A_4d72_AF4C_90F4A4DFB6AF__
#define __Mc02FomIO_H_AD1D1AE8_768A_4d72_AF4C_90F4A4DFB6AF__

#include "openeaagles/hla/HlaIO.h"

namespace Eaagles {
   namespace Simulation { class Player; }

namespace Network {
namespace Hla {
   class Ambassador;

namespace Mc02Fom {
   class BaseEntity;
   class EmitterBeam;
   class Nib;

//==============================================================================
// Class:       Mc02FomIO
// Base class:	... -> Simulation::NetworkIO -> HlaIO -> Mc02FomIO
//==============================================================================
class Mc02FomIO : public HlaIO  
{
    DECLARE_SUBCLASS(Mc02FomIO,HlaIO)

public:
    // Object Class Indexes
    enum {
        BASE_ENTITY_CLASS = 1,
        PHYSICAL_ENTITY_CLASS = 2,
        PLATFORM_CLASS = 3,
        AIRCRAFT_CLASS = 4,
        MISSILE_CLASS = 5,
        EMITTER_BEAM_CLASS = 6,
        JAMMER_BEAM_CLASS = 7,
        RADAR_BEAM_CLASS = 8,
        
        NUM_OBJECT_CLASSES = 8
    };

    // Object Attribute Indexes
    enum {
        // Base Entity (BE) arguments indexes (AI)
        WORLD_LOCATION_BE_AI = 1,
        ENTITY_TYPE_BE_AI = 2,
        ENTITY_IDENTIFIER_BE_AI = 3,
        IS_FROZEN_BE_AI = 4,
        DEAD_RECKONING_ALGORITHM_BE_AI = 5,
        ORIENTATION_BE_AI = 6,
        VELOCITY_VECTOR_BE_AI = 7,
        ANGULAR_VELOCITY_VECTOR_BE_AI = 8,
        ACCELERATION_VECTOR_BE_AI = 9,
        
        // Physical Entity (PE) arguments indexes (AI)
        FORCE_IDENTIFIER_PE_AI = 10,
        
        // Emitter Beam (EB) arguments indexes (AI)
        BEAM_AZIMUTH_CENTER_EB_AI = 11,
        BEAM_AZIMUTH_SWEEP_EB_AI = 12,
        BEAM_ELEVATION_CENTER_EB_AI = 13,
        BEAM_ELEVATION_SWEEP_EB_AI = 14,
        BEAM_FUNCTION_CODE_EB_AI = 15,
        BEAM_IDENTIFIER_EB_AI = 16,
        BEAM_PARAMETER_INDEX_EB_AI = 17,
        EFFECTIVE_RADIATED_POWER_EB_AI = 18,
        EMISSION_FREQUENCY_EB_AI = 19,
        EMITTER_SYSTEM_IDENTIFIER_EB_AI = 20,
        EVENT_IDENTIFIER_EB_AI = 21,
        FREQUENCY_RANGE_EB_AI = 22,
        PULSE_REPETITION_FREQUENCY_EB_AI = 23,
        PULSE_WIDTH_EB_AI = 24,
        SWEEP_SYNCH_EB_AI = 25,
        
        NUM_OBJECT_ATTRIBUTES = 25
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
        EVENT_IDENTIFIER_WF_PI = 1,
        FIRE_CONTROL_SOLUTION_RANGE_WF_PI = 2,
        FIRE_MISSION_INDEX_WF_PI = 3,
        FIRING_LOCATION_WF_PI = 4,
        FIRING_OBJECT_IDENTIFIER_WF_PI = 5,
        FUSE_TYPE_WF_PI = 6,
        INITIAL_VELOCITY_VECTOR_WF_PI = 7,
        MUNITION_OBJECT_IDENTIFIER_WF_PI = 8,
        MUNITION_TYPE_WF_PI = 9,
        QUANTITY_FIRED_WF_PI = 10,
        RATE_OF_FIRE_WF_PI = 11,
        TARGET_OBJECT_IDENTIFIER_WF_PI = 12,
        WARHEAD_TYPE_WF_PI = 13,

        // Munition Detonation (MD) parameter indexes (PI)
        DETONATION_LOCATION_MD_PI = 14,
        DETONATION_RESULT_CODE_MD_PI = 15,
        EVENT_IDENTIFIER_MD_PI = 16,
        FIRING_OBJECT_IDENTIFIER_MD_PI = 17,
        FINAL_VELOCITY_VECTOR_MD_PI = 18,
        FUSE_TYPE_MD_PI = 19,
        MUNITION_OBJECT_IDENTIFIER_MD_PI = 20,
        MUNITION_TYPE_MD_PI = 21,
        QUANTITY_FIRED_MD_PI = 22,
        RATE_OF_FIRE_MD_PI = 23,
        RELATIVE_DETONATION_LOCATION_MD_PI = 24,
        TARGET_OBJECT_IDENTIFIER_MD_PI = 25,
        WARHEAD_TYPE_MD_PI = 26,
        
        NUM_INTERACTION_PARAMETER = 26
    };

public:
    Mc02FomIO();

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
    virtual Simulation::Nib* nibFactory(const Simulation::NetworkIO::IoType ioType);   // Create a new Mc02Fom::Nib
};

} // End Mc02Fom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace

#endif // __Mc02FomIO_H_AD1D1AE8_768A_4d72_AF4C_90F4A4DFB6AF__
