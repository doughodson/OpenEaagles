//------------------------------------------------------------------------------
// Class: Hla::RprFom::NetIO
//
// Form Name: RprFomNetIO
//
// Real-time Platform Reference FOM (RPR-FOM) Data Structures
// RPR-FOM Version: 2.0 Draft 17
//------------------------------------------------------------------------------
#ifndef __Eaagles_Network_Hla_RprFom_NetIO_H__
#define __Eaagles_Network_Hla_RprFom_NetIO_H__

#include "openeaagles/hla/NetIO.h"

namespace Eaagles {
   namespace Simulation { class Player; }

namespace Network {
namespace Hla {
   class Ambassador;

namespace RprFom {
   class BaseEntity;
   class EmitterBeam;
   class Nib;
   class Ntm;


//==============================================================================
// Class: Hla::RprFom::NetIO
// Form Name: RprFomNetIO
//==============================================================================
class NetIO : public Hla::NetIO  
{
    DECLARE_SUBCLASS(NetIO,Hla::NetIO)

public:
    // Object Class Indexes
    // (see RpfFom.h for class declarations)
    enum {
        BASE_ENTITY_CLASS = 1,
        PHYSICAL_ENTITY_CLASS = 2,
        LIFEFORM_CLASS = 3,
        HUMAN_CLASS = 4,
        NON_HUMAN_CLASS = 5,
        MUNITION_CLASS = 6,
        PLATFORM_CLASS = 7,
        AIRCRAFT_CLASS = 8,
        AMPHIBIOUS_VEHICLE_CLASS = 9,
        GROUND_VEHICLE_CLASS = 10,
        MULTI_DOMAIN_PLATFORM_CLASS = 11,
        SPACECRAFT_CLASS = 12,
        SUBMERSIBLE_VESSEL_CLASS = 13,
        SURFACE_VESSEL_CLASS = 14,
        
        NUM_OBJECT_CLASSES = 14
    };

    // Object Attribute Indexes
    // (see RpfFom.h for class declarations)
    enum {
        // Base Entity arguments indexes (AI)
        ENTITY_TYPE_AI = 1,
        ENTITY_IDENTIFIER_AI = 2,
        SPATIAL_AI = 3,
        RELATIVE_SPATIAL_AI = 4,
        
        // Physical Entity arguments indexes (AI)
        ACOUSTIC_SIGNATURE_INDEX_AI = 5,
        ALTERNATEENTITY_TYPE_AI = 6,
        CAMOUFLAGE_TYPE_AI = 7,
        DAMAGE_STATE_AI = 8,
        ENGINE_SMOKE_ON_AI = 9,
        FIRE_POWER_DISABLED_AI = 10,
        FLAMES_PRESENT_AI = 11,
        FORCE_IDENTIFIER_AI = 12,
        HAS_AMMUNITION_SUPPLY_CAP_AI = 13,
        HAS_FUEL_SUPPLY_CAP_AI = 14,
        HAS_RECOVERY_CAP_AI = 15,
        HAS_REPAIR_CAP_AI = 16,
        IMMOBILIZED_AI = 17,
        INFRARED_SIGNATURE_INDEX_AI = 18,
        IS_CONCEALED_AI = 19,
        LIVE_ENTITY_MEASURED_SPEED_AI = 20,
        MARKING_AI = 21,
        POWER_PLANT_ON_AI = 22,
        PROPULSION_SYSTEMS_DATA_AI = 23,
        RADAR_CROSS_SECTION_SIGNATURE_INDEX_AI = 24,
        SMOKE_PLUME_PRESENT_AI = 25,
        TENT_DEPLOYED_AI = 26,
        TRAILING_EFFECTS_CODE_AI = 27,
        VECTORING_NOZZLE_SYSTEM_DATA_AI = 28,
        
        // Lifeform Entity arguments indexes (AI)
        FLASH_LIGHTS_ON_AI = 29,
        STANCE_CODE_AI = 30,
        PRIMARY_WEAPON_STATE_AI = 31,
        SECONDARY_WEAPON_STATE_AI = 32,
        COMPLIANCE_STATE_AI = 33,
        
        // Munition Entity (MUN) arguments indexes (AI)
        LAUNCHER_FLASH_PRESENT_AI = 34,
        
        // Platform Entity (PLAT) arguments indexes (AI)
        AFTERBURNER_ON_AI = 35,
        ANTI_COLLISION_LIGHTS_ON_AI = 36,
        BLACK_OUT_BRAKE_LIGHTS_ON_AI = 37,
        BLACK_OUT_LIGHTS_ON_AI = 38,
        BRAKE_LIGHTS_ON_AI = 39,
        FORMATION_LIGHTS_ON_AI = 40,
        HATCH_STATE_AI = 41,
        HEAD_LIGHTS_ON_AI = 42,
        INTERIOR_LIGHTS_ON_AI = 43,
        LANDING_LIGHTS_ON_AI = 44,
        LAUNCHER_RAISED_AI = 45,
        NAVIGATION_LIGHTS_ON_AI = 46,
        RAMP_DEPLOYED_AI = 47,
        RUNNING_LIGHTS_ON_AI = 48,
        SPOT_LIGHTS_ON_AI = 49,
        TAIL_LIGHTS_ON_AI = 50,
        
        NUM_OBJECT_ATTRIBUTES = 50
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

   // Standard (IST-CF-03-01, May 5, 2003) entity type "kind" codes [ 0 .. 9 ]
   enum EntityTypeKindEnum {
      KIND_OTHER, KIND_PLATFORM, KIND_MUNITION, KIND_LIFEFORM,
      KIND_ENVIRONMENTAL, KIND_CULTURAL_FEATURE, KIND_SUPPLY, KIND_RADIO,
      KIND_EXPENDABLE, KIND_SENSOR_EMITTER, NUM_ENTITY_KINDS
   };

   // Standard (IST-CF-03-01, May 5, 2003) "platform domain" codes [ 0 .. 5 ]
   enum PlatformDomainEnum {
      PLATFORM_DOMAIN_OTHER, PLATFORM_DOMAIN_LAND, PLATFORM_DOMAIN_AIR, PLATFORM_DOMAIN_SURFACE,
      PLATFORM_DOMAIN_SUBSURFACE, PLATFORM_DOMAIN_SPACE, NUM_ENTITY_DOMAINS
   };

public:
    NetIO();

   // Finds the Ntm by entity type codes
   virtual const Ntm* findNtmByTypeCodes(
         const unsigned char  kind,
         const unsigned char  domain,
         const unsigned short countryCode,
         const unsigned char  category,
         const unsigned char  subcategory = 0,
         const unsigned char  specific = 0,
         const unsigned char  extra = 0
      ) const;

    // NetIO interface
    virtual unsigned int getNumberOfObjectClasses() const;
    virtual unsigned int getNumberOfObjectAttributes() const;
    virtual unsigned int getNumberOfOInteractionClasses() const;
    virtual unsigned int getNumberOfInteractionParameters() const;
    virtual void discoverObjectInstance(
        const RTI::ObjectHandle theObject, const RTI::ObjectClassHandle theObjectClass, const char* theObjectName
    );
    virtual void receiveInteraction(
        const RTI::InteractionClassHandle theInteraction,
        const RTI::ParameterHandleValuePairSet& theParameters
    );

    // Simulation::NetIO interface
    virtual Simulation::Nib* createNewOutputNib(Simulation::Player* const player);
    virtual Simulation::NetIO::NtmInputNode* rootNtmInputNodeFactory() const;
    
protected:
    virtual bool receiveWeaponFire(const RTI::ParameterHandleValuePairSet& theParameters);
    virtual bool receiveMunitionDetonation(const RTI::ParameterHandleValuePairSet& theParameters);

    virtual bool publishAndSubscribePlatforms();
    virtual bool publishAndSubscribeWeaponFire();
    virtual bool publishAndSubscribeMunitionDetonation();

    // NetIO interface (callbacks)
    virtual bool publishAndSubscribe();
    
    // Simulation::NetIO Interface (Callbacks)
    virtual void processInputList();    // Update players/systems from the Input-list
    virtual Simulation::Nib* nibFactory(const Simulation::NetIO::IoType ioType);   // Create a new RprFom::Nib
};

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace

#endif
