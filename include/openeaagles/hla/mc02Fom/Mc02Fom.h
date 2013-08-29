//------------------------------------------------------------------------------
// MC02 FOM Data Structures
// 
// Millenium Challenge 2002 FOM
// Version: RPR 1.0 Draft 2 w/extensions: $Revision: 1.6
//------------------------------------------------------------------------------
#ifndef __Hla_Mc02Fom_H__
#define __Hla_Mc02Fom_H__

#include <RTI.hh>
#include "eaagles/basic/Object.h"
 
namespace Eaagles {
namespace Network {
namespace Hla {
namespace Mc02Fom {

//------------------------------------------------------------------------------
// Enumerated Data
//------------------------------------------------------------------------------

// ForceIdentifierEnum8 -- 
enum ForceIdentifierEnum8 {
    OTHER = 0,
    FRIENDLY = 1,
    OPPOSING = 2,
    NEUTRAL = 3,
};

// DeadReckoningAlgorithmEnum8 -- 
enum DeadReckoningAlgorithmEnum8 {
    DRM_OTHER = 0,
    DRM_STATIC = 1,
    DRM_FPW = 2,
    DRM_RPW = 3,
    DRM_RVW = 4,
    DRM_FVW = 5,
    DRM_FPB = 6,
    DRM_RPB = 7,
    DRM_RVB = 8,
    DRM_FVB = 9,
};

// CamouflageEnum32 -- 
enum CamouflageEnum32 {
    UNIFORM_PAINT_SCHEME = 0,
    DESERT_CAMOUFLAGE = 1,
    WINTER_CAMOUFLAGE = 2,
    FOREST_CAMOUFLAGE = 3,
    GENERIC_CAMOUFLAGE = 4,
};

// DamageStatusEnum32 -- 
enum DamageStatusEnum32 {
    NO_DAMAGE = 0,
    SLIGHT_DAMAGE = 1,
    MODERATE_DAMAGE = 2,
    DESTROYED = 3,
};

// MarkingEncodingEnum8 -- 
enum MarkingEncodingEnum8 {
    MARKING_OTHER = 0,
    ASCII = 1,
    ARMY_MARKING_CCTT = 2,
    DIGIT_CHEVRON = 3,
};

// TrailingEffectsCodeEnum32 -- 
enum TrailingEffectsCodeEnum32 {
    NO_TRAIL = 0,
    SMALL_TRAIL = 1,
    MEDIUM_TRAIL = 2,
    LARGE_TRAIL = 3,
};

// HatchStateEnum32 -- 
enum HatchStateEnum32 {
    NOT_APPLICABLE = 0,
    PRIMARY_HATCH_IS_CLOSED = 1,
    PRIMARY_HATCH_IS_POPPED = 2,
    PRIMARY_HATCH_IS_POPPED_AND_PERSON_IS_VISIBLE_UNDER_HATCH = 3,
    PRIMARY_HATCH_IS_OPEN = 4,
    PRIMARY_HATCH_IS_OPEN_AND_PERSON_IS_VISIBLE = 5,
};

enum FuseTypeEnum16 {
    FuseTypeOther = 0,
    IntelligentInfluence = 10,
    Sensor = 20,
    SelfDestruct = 30,
    UltraQuick = 40,
    Body = 50,
    DeepIntrusion = 60,
    Multifunction = 100,
    PointDetonation_PD = 200,
    BaseDetonation_BD = 300,
    Contact = 1000,
    ContactInstantImpact = 1100,
    ContactDelayed = 1200,
    Contact10msDelay = 1201,
    Contact20msDelay = 1202,
    Contact50msDelay = 1205,
    Contact60msDelay = 1206,
    Contact100msDelay = 1210,
    Contact125msDelay = 1212,
    Contact250msDelay = 1225,
    ContactElectronicObliqueContact = 1300,
    ContactGraze = 1400,
    ContactCrush = 1500,
    ContactHydrostatic = 1600,
    ContactMechanical = 1700,
    ContactChemical = 1800,
    ContactPiezoelectric = 1900,
    ContactPointInitiating = 1910,
    ContactPointInitiatingBaseDetonating = 1920,
    ContactBaseDetonating = 1930,
    ContactBallisticCapAndBase = 1940,
    ContactBase = 1950,
    ContactNose = 1960,
    ContactFittedInStandoffProbe = 1970,
    ContactNonAligned = 1980,
    Timed = 2000,
    TimedProgrammable = 2100,
    TimedBurnout = 2200,
    TimedPyrotechnic = 2300,
    TimedElectronic = 2400,
    TimedBaseDelay = 2500,
    TimedReinforcedNoseImpactDelay = 2600,
    TimedShortDelayImpact = 2700,
    Timed10msDelay = 2701,
    Timed20msDelay = 2702,
    Timed50msDelay = 2705,
    Timed60msDelay = 2706,
    Timed100msDelay = 2710,
    Timed125msDelay = 2712,
    Timed250msDelay = 2725,
    TimedNoseMountedVariableDelay = 2800,
    TimedLongDelaySide = 2900,
    TimedSelectableDelay = 2910,
    TimedImpact = 2920,
    TimedSequence = 2930,
    Proximity = 3000,
    ProximityActiveLaser = 3100,
    ProximityMagneticMagpolarity = 3200,
    ProximityActiveDopplerRadar = 3300,
    ProximityRadioFrequencyRF = 3400,
    ProximityProgrammable = 3500,
    ProximityProgrammablePrefragmented = 3600,
    ProximityInfrared = 3700,
    Command = 4000,
    CommandElectronicRemotelySet = 4100,
    Altitude = 5000,
    AltitudeRadioAltimeter = 5100,
    AltitudeAirBurst = 5200,
    Depth = 6000,
    Acoustic = 7000,
    Pressure = 8000,
    PressureDelay = 8010,
    Inert = 8100,
    Dummy = 8110,
    PracticeFuseType = 8120,
    PlugRepresenting = 8130,
    Training = 8150,
    Pyrotechnic = 9000,
    PyrotechnicDelay = 9010,
    ElectroOptical = 9100,
    ElectroMechanical = 9110,
    ElectroMechanicalNose = 9120,
    Strikerless = 9200,
    StrikerlessNoseImpact = 9210,
    StrikerlessCompressionIgnition = 9220,
    CompressionIgnition = 9300,
    CompressionIgnitionStrikerlessNoseImpact = 9310,
    Percussion = 9400,
    PercussionInstantaneous = 9410,
    Electronic = 9500,
    ElectronicInternallyMounted = 9510,
    ElectronicRangeSetting = 9520,
    ElectronicProgrammed = 9530,
    Mechanical = 9600,
    MechanicalNose = 9610,
    MechanicalTail = 9620
};


enum WarheadTypeEnum16 {
    WarheadTypeOther = 0,
    CargoVariableSubmunitions = 10,
    FuelAirExplosive = 20,
    GlassBeads = 30,
    Warhead_1um = 31,
    Warhead_5um = 32,
    Warhead_10um = 33,
    HighExplosive = 1000,
    HE_Plastic = 1100,
    HE_Incendiary = 1200,
    HE_Fragmentation = 1300,
    HE_Antitank = 1400,
    HE_Bomblets = 1500,
    HE_ShapedCharge = 1600,
    HE_ContinuousRod = 1610,
    HE_TungstenBall = 1615,
    HE_BlastFragmentation = 1620,
    HE_SteerableDartswithHE = 1625,
    HE_Darts = 1630,
    HE_Flechettes = 1635,
    HE_DirectedFragmentation = 1640,
    HE_SemiArmorPiercing = 1645,
    HE_ShapedChargeFragmentation = 1650,
    HE_SemiArmorPiercingFragmentation = 1655,
    HE_HollowCharge = 1660,
    HE_DoubleHollowCharge = 1665,
    HE_GeneralPurpose = 1670,
    HE_BlastPenetrator = 1675,
    HE_RodPenetrator = 1680,
    HE_Antipersonnel = 1685,
    Smoke = 2000,
    Illumination = 3000,
    PracticeWarheadType = 4000,
    Kinetic = 5000,
    Mines = 6000,
    Nuclear = 7000,
    NuclearIMT = 7010,
    ChemicalGeneral = 8000,
    ChemicalBlisterAgent = 8100,
    HD_Mustard = 8110,
    ThickenedHD_Mustard = 8115,
    DustyHD_Mustard = 8120,
    ChemicalBloodAgent = 8200,
    AC_HCN = 8210,
    CK_CNCI = 8215,
    CG_Phosgene = 8220,
    ChemicalNerveAgent = 8300,
    VX = 8310,
    ThickenedVX = 8315,
    DustyVX = 8320,
    GA_Tabun = 8325,
    ThickenedGA_Tabun = 8330,
    DustyGA_Tabun = 8335,
    GB_Sarin = 8340,
    ThickenedGB_Sarin = 8345,
    DustyGB_Sarin = 8350,
    GD_Soman = 8355,
    ThickenedGD_Soman = 8360,
    DustyGD_Soman = 8365,
    GF = 8370,
    ThickenedGF = 8375,
    DustyGF = 8380,
    Biological = 9000,
    BiologicalVirus = 9100,
    BiologicalBacteria = 9200,
    BiologicalRickettsia = 9300,
    BiologicalGeneticallyModifiedMicroOrganisms = 9400,
    BiologicalToxin = 9500
};

enum DetonationResultCodeEnum8 {
    DetonationResultCodeOther = 0,
    EntityImpact = 1,
    EntityProximateDetonation = 2,
    GroundImpact = 3,
    GroundProximateDetonation = 4,
    Detonation = 5,
    None = 6,
    HE_hit_Small = 7,
    HE_hit_Medium = 8,
    HE_hit_Large = 9,
    ArmorPiercingHit = 10,
    DirtBlast_Small = 11,
    DirtBlast_Medium = 12,
    DirtBlast_Large = 13,
    WaterBlast_Small = 14,
    WaterBlast_Medium = 15,
    WaterBlast_Large = 16,
    AirHit = 17,
    BuildingHit_Small = 18,
    BuildingHit_Medium = 19,
    BuildingHit_Large = 20,
    MineClearingLineCharge = 21,
    EnvironmentObjectImpact = 22,
    EnvironmentObjectProximateDetonation = 23,
    WaterImpact = 24,
    AirBurst = 25
};

enum BeamFunctionCodeEnum8 {
    BeamFunctionCodeOther = 0,
    Search = 1,
    HeightFinder = 2,
    Acquisition = 3,
    Tracking = 4,
    AcquisitionAndTracking = 5,
    CommandGuidance = 6,
    BeamFunctionCodeIllumination = 7,
    RangeOnlyRadar = 8,
    MissileBeacon = 9,
    MissileFuze = 10,
    ActiveRadarMissileSeeker = 11,
    Jammer = 12
};


//------------------------------------------------------------------------------
// Complex Data Structures
//------------------------------------------------------------------------------

// RTIObjectIdStruct
struct RTIObjectIdStruct {
    static const int ID_SIZE = 64;
    RTIObjectIdStruct() { for (int i = 0; i < ID_SIZE; i++) { id[i] = '\0';} }
    unsigned char id[ID_SIZE];
};
  
// EventIdentifierStruct
struct EventIdentifierStruct {
    EventIdentifierStruct() { eventCount = 0; issuingObjectIdentifier.id[0] = '\0'; }
    unsigned short      eventCount;
    RTIObjectIdStruct   issuingObjectIdentifier;
};
  
// RTIObjectIdArrayStruct
struct RTIObjectIdArrayStruct {
    RTIObjectIdArrayStruct() { length = 0; ID[0].id[0] = '\0'; }
    unsigned short      length;
    RTIObjectIdStruct   ID[1];      // one or more IDs
};

// EntityTypeStruct
struct EntityTypeStruct {
    EntityTypeStruct() { entityKind = 0; domain = 0; countryCode = 0; category = 0; subcategory = 0; specific = 0; extra = 0; }
    unsigned char entityKind;
    unsigned char domain;
    RTI::UShort   countryCode;
    unsigned char category;
    unsigned char subcategory;
    unsigned char specific;
    unsigned char extra;
};

// Federate Identifier
struct FederateIdentifierStruct {
    FederateIdentifierStruct() { siteID = 0; applicationID = 0; }
    RTI::UShort siteID;
    RTI::UShort applicationID;
};

// Entity Identifier
struct EntityIdentifierStruct {
    EntityIdentifierStruct() : federateIdentifier() { entityNumber = 0; }
    FederateIdentifierStruct federateIdentifier;
    RTI::UShort     entityNumber;
};

// World Location (meters)
struct WorldLocationStruct {
    WorldLocationStruct()       { x = 0.0; y = 0.0; z = 0.0; }
    RTI::Double x;
    RTI::Double y;
    RTI::Double z;
};

// Velocity (meters/second)
struct VelocityVectorStruct {
    VelocityVectorStruct()      { xVelocity = 0.0; yVelocity = 0.0; zVelocity = 0.0; }
    RTI::Float xVelocity;
    RTI::Float yVelocity;
    RTI::Float zVelocity;
};

// Acceleration (meters/second/second)
struct AccelerationVectorStruct {
    AccelerationVectorStruct()  { xAcceleration = 0.0; yAcceleration = 0.0; zAcceleration = 0.0; }
    RTI::Float xAcceleration;
    RTI::Float yAcceleration;
    RTI::Float zAcceleration;
};

// Orientation (radians)
struct OrientationStruct {
    OrientationStruct() { phi = 0.0; psi = 0.0; theta = 0.0; }
    RTI::Float  psi;
    RTI::Float  theta;
    RTI::Float  phi;
};

// Angular Velocity (radians/second)
struct AngularVelocityVectorStruct {
    AngularVelocityVectorStruct() { xAngularVelocity = 0.0; yAngularVelocity = 0.0; zAngularVelocity = 0.0; }
    RTI::Float  xAngularVelocity;
    RTI::Float  yAngularVelocity;
    RTI::Float  zAngularVelocity;
};

// Relative Position Struct (meters)
struct RelativePositionStruct {
    RelativePositionStruct() { bodyXDistance = 0.0; bodyYDistance = 0.0; bodyZDistance = 0.0; }
    float   bodyXDistance;
    float   bodyYDistance;
    float   bodyZDistance;
};


// Markings
struct MarkingStruct {
    MarkingStruct() { markingEncodingType = ASCII; for (int i = 0; i < 11; markingData[i++] = '\0'); }
    MarkingEncodingEnum8    markingEncodingType;
    unsigned char           markingData[11];
};


//==============================================================================
// MC02: Object Class Structures
//==============================================================================

// ---
// Base Entity Structure (S)
//
// "A base class of aggregate and discrete scenaro domain participants. 
// The BaseEntity class is characterised by being located at a particular 
// location in space and independently movable, if capable of movement at all. 
// It specifically excludes elements normally considered to be a component of 
// another element. The BaseEntity class is intended to be a container for
// common attributes for entities of this type.  Since it lacks sufficient
// class specific attributes that are required for simulation purposes it is
// not expected that any federate shall publish objects of this class.  Certain
// simulation management federates, e.g. viewers, may subscribe to this class.
// Simulation federates will normally subscribe to one of the subclasses, to
// gain the extra information required to properly simulate the entity.
// (See section 6.1 of the GRIM)"
// ---

class BaseEntity : public Basic::Object {
    DECLARE_SUBCLASS(BaseEntity,Basic::Object)
public:
    BaseEntity();
    
    static const char* getClassFedName()                            { return "ObjectRoot.BaseEntity"; }
    
    static const char* getAngularVelocityVectorAttributeFedName()   { return "AngularVelocityVector"; }
    static const char* getAccelerationVectorAttributeFedName()      { return "AccelerationVector"; }
    static const char* getDeadReckoningAlgorithmAttributeFedName()  { return "DeadReckoningAlgorithm"; }
    static const char* getEntityTypeAttributeFedName()              { return "EntityType"; }
    static const char* getEntityIdentifierAttributeFedName()        { return "EntityIdentifier"; }
    static const char* getIsFrozenAttributeFedName()                { return "IsFrozen"; }
    static const char* getOrientationAttributeFedName()             { return "Orientation"; }
    static const char* getWorldLocationAttributeFedName()           { return "WorldLocation"; }
    static const char* getVelocityVectorAttributeFedName()          { return "VelocityVector"; }
    
    AccelerationVectorStruct    accelerationVector;     // The magnitude of the change in linear velocity of an object over time.
    AngularVelocityVectorStruct angularVelocityVector;  // The rate at which an entity's orientation is changing over time.
    DeadReckoningAlgorithmEnum8 deadReckoningAlgorithm; // Dead reckoning algorithm used by the issuing object.
    EntityTypeStruct            entityType;             // The category of the entity.
    EntityIdentifierStruct      entityIdentifier;       // The unique identifier for the entity instance.
    RTI::Boolean                isFrozen;               // Whether the entity is frozen or not.
    OrientationStruct           orientation;            // The angles of rotation around the coordinate axes between
                                                        // the entity's attitude and the reference coordinate system axes
                                                        // (calculated as the Tait-Bryan Euler angles specifying the successive
                                                        // rotations needed to transform from the world coordinate system to
                                                        // the entity coordinate system).
                                                    
    WorldLocationStruct         worldLocation;          // Location of the entity.
    
    VelocityVectorStruct        velocityVector;         // The rate at which an entity's position is changing over time
};

// ---
// Physical Entity Structure (S)
//  -- subclass of BaseEntity
//
// "A base class of all discrete platform scenario domain participants."
// ---
class PhysicalEntity : public BaseEntity {
    DECLARE_SUBCLASS(PhysicalEntity,BaseEntity)
public:
    PhysicalEntity();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity"; }

    static const char* getAlternateEntityTypeAttributeFedName()     { return "AlternateEntityType"; }
    static const char* getCamouflageTypeAttributeFedName()          { return "CamouflageType"; }
    static const char* getDamageStateAttributeFedName()             { return "DamageState"; }
    static const char* getEngineSmokeOnAttributeFedName()           { return "EngineSmokeOn"; }
    static const char* getFirePowerDisabledAttributeFedName()       { return "FirePowerDisabled"; }
    static const char* getFlamesPresentAttributeFedName()           { return "FlamesPresent"; }
    static const char* getForceIdentifierAttributeFedName()         { return "ForceIdentifier"; }
    static const char* getHasAmmunitionSupplyCapAttributeFedName()  { return "HasAmmunitionSupplyCap"; }
    static const char* getHasFuelSupplyCapAttributeFedName()        { return "HasFuelSupplyCap"; }
    static const char* getHasRecoveryCapAttributeFedName()          { return "HasRecoveryCap"; }
    static const char* getHasRepairCapAttributeFedName()            { return "HasRepairCap"; }
    static const char* getImmobilizedAttributeFedName()             { return "Immobilized"; }
    static const char* getIsConcealedAttributeFedName()             { return "IsConcealed"; }
    static const char* getMarkingAttributeFedName()                 { return "Marking"; }
    static const char* getPowerPlantOnAttributeFedName()            { return "PowerPlantOn"; }
    static const char* getSmokePlumePresentAttributeFedName()       { return "SmokePlumePresent"; }
    static const char* getTentDeployedAttributeFedName()            { return "TentDeployed"; }
    static const char* getTrailingEffectsCodeAttributeFedName()     { return "TrailingEffectsCode"; }
    
    EntityTypeStruct            alternateEntityType;    // The category of entity to be used when viewed by entities on the "opposite" side.
    CamouflageEnum32            camouflageType;         // The type of camouflage in use (if any).
    DamageStatusEnum32          damageState;            // The state of damage of the entity.
    RTI::Boolean                engineSmokeOn;          // Whether the entity's engine is generating smoke or not.
    RTI::Boolean                firePowerDisabled;      // Whether the entity's main weapon system has been disabled or not.
    RTI::Boolean                flamesPresent;          // Whether the entity is on fire (with visible flames) or not.
    ForceIdentifierEnum8        forceIdentifier;        // The identification of the force that the entity belongs to.
    RTI::Boolean                hasAmmunitionSupplyCap; // Whether the entity has the capability to supply other entities with ammunition.
    RTI::Boolean                hasFuelSupplyCap;       // Whether the entity has the capability to supply other entities with fuel or not.
    RTI::Boolean                hasRecoveryCap;         // Whether the entity has the capability to recover other entities or not.
    RTI::Boolean                hasRepairCap;           // Whether the entity has the capability to repair other entities or not.
    RTI::Boolean                immobilized;            // Whether the entity is immobilized or not.
    RTI::Boolean                isConcealed;            // Whether the entity is concealed or not.
    MarkingStruct               marking;                // A unique marking or combination of characters used to distinguish the entity from other entities.
    RTI::Boolean                powerPlantOn;           // Whether the entity's power plant is on or not.
    RTI::Boolean                smokePlumePresent;      // Whether the entity is generating smoke or not (intentional or unintentional).
    RTI::Boolean                tentDeployed;           // Whether the entity has deployed tent or not.
    TrailingEffectsCodeEnum32   trailingEffectsCode;    // The type and size of any trail that the entity is making.
};

// ---
// Munition Structure (PS)
//  -- subclass of PhysicalEntity
//
// "A complete device charged with explosives, propellants, pyrotechnics, initiating composition,
//  or nuclear, biological or chemical material for use in military operations, including demolitions."
// ---
class Munition : public PhysicalEntity {
    DECLARE_SUBCLASS(Munition,PhysicalEntity)
public:
    Munition();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Munition"; }

    RTI::Boolean                launcherFlashPresent;   // Whether the flash of the munition being launched is present or not.
};

// ---
// Platform Structure (S)
//  -- subclass of PhysicalEntity
//
// "A physical object under the control of armed forces upon which sensor, communication,
//  or weapon systems may be mounted."
// ---
class Platform : public PhysicalEntity {
    DECLARE_SUBCLASS(Platform,PhysicalEntity)
public:
    Platform();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform"; }
    
    RTI::Boolean                afterburnerOn;          // Whether the entity's afterburner is on or not.
    RTI::Boolean                antiCollisionLightsOn;  // Whether the entity's anti-collision lights are on or not.
    RTI::Boolean                blackOutBrakeLightsOn;  // Whether the entity's black out brake lights are on or not.
    RTI::Boolean                blackOutLightsOn;       // Whether the entity's black out lights are on or not.
    RTI::Boolean                brakeLightsOn;          // Whether the entity's brake lights are on or not.
    RTI::Boolean                formationLightsOn;      // Whether the entity's formation lights are on or not.
    HatchStateEnum32            hatchState;             // The state of the entity's (main) hatch.
    RTI::Boolean                headLightsOn;           // Whether the entity's headlights are on or not.
    RTI::Boolean                interiorLightsOn;       // Whether the entity's internal lights are on or not.
    RTI::Boolean                landingLightsOn;        // Whether the entity's landing lights are on or not.
    RTI::Boolean                launcherRaised;         // Whether the entity's weapon launcher is in the raised position.
    RTI::Boolean                navigationLightsOn;     // Whether the entity's navigation lights are on or not.
    RTI::Boolean                rampDeployed;           // Whether the entity has deployed a ramp or not.
    RTI::Boolean                runningLightsOn;        // Whether the entity's running lights are on or not.
    RTI::Boolean                spotLightsOn;           // Whether the entity's spotlights are on or not.
    RTI::Boolean                tailLightsOn;           // Whether the entity's tail lights are on or not.
};
  
// ---
// Missile Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that represents a munition that can be targeted"
// ---
class Missile : public Platform {
    DECLARE_SUBCLASS(Missile,Platform)
public:
    Missile();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.Missile"; }
};
  
// ---
// Aircraft Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that operates mainly in the air, such as aircraft,
//  balloons, etc.  This includes the entities when they are on the ground."
// ---
class Aircraft : public Platform {
    DECLARE_SUBCLASS(Aircraft,Platform)
public:
    Aircraft();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.Aircraft"; }
};
  
// ---
// Amphibious Vehicle Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that can operate both on the land and the sea."
// ---
class AmphibiousVehicle : public Platform {
    DECLARE_SUBCLASS(AmphibiousVehicle,Platform)
public:
    AmphibiousVehicle();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.AmphibiousVehicle"; }
};
  
// ---
// Ground Vehicle Structure (S)
//  -- subclass of Platform
//
// "A platform entity that operates wholy on the surface of the earth."
// ---
class GroundVehicle : public Platform {
    DECLARE_SUBCLASS(GroundVehicle,Platform)
public:
    GroundVehicle();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.GroundVehicle"; }
};
  
// ---
// MultiDomain Platform Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that operates in more than one domain (excluding those
//  combinations explicitly defined as subclasses of the superclass of this class)."
// ---
class MultiDomainPlatform : public Platform {
    DECLARE_SUBCLASS(MultiDomainPlatform,Platform)
public:
    MultiDomainPlatform();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.MultiDomainPlatform"; }
};
  
// ---
// Spacecraft Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that operates mainly in space."
// ---
class Spacecraft : public Platform {
    DECLARE_SUBCLASS(Spacecraft,Platform)
public:
    Spacecraft();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.Spacecraft"; }
};
  
// ---
// Submersible Vessel Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that operates either on the surface of the sea, or beneath it."
// ---
class SubmersibleVessel : public Platform {
    DECLARE_SUBCLASS(SubmersibleVessel,Platform)
public:
    SubmersibleVessel();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.SubmersibleVessel"; }
};
  
// ---
// Surface Vessel Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that operates wholy on the surface of the sea."
// ---
class SurfaceVessel : public Platform {
    DECLARE_SUBCLASS(SurfaceVessel,Platform)
public:
    SurfaceVessel();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.SurfaceVessel"; }
};
  
// ---
// Default Structure (PS)
//  -- subclass of GroundVehicle
//
// "A ground platform that doesn't fit in the other categories"
// ---
class Default : public GroundVehicle {
    DECLARE_SUBCLASS(Default,GroundVehicle)
public:
    Default();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.GroundVehicle.Default"; }
};
  
// ---
// Command_Center Structure (PS)
//  -- subclass of GroundVehicle
//
// "Command Center"
// ---
class CommandCenter : public GroundVehicle {
    DECLARE_SUBCLASS(CommandCenter,GroundVehicle)
public:
    CommandCenter();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.GroundVehicle.Command_Center"; }
};
  
// ---
// SAM_Commander Structure (PS)
//  -- subclass of GroundVehicle
//
// "SAM Commander"
// ---
class SAM_Commander : public GroundVehicle {
    DECLARE_SUBCLASS(SAM_Commander,GroundVehicle)
public:
    SAM_Commander();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.GroundVehicle.SAM_Commander"; }
};
  
// ---
// Generic Site Structure (PS)
//  -- subclass of GroundVehicle
//
// "A generic ground site"
// ---
class GenericSite : public GroundVehicle {
    DECLARE_SUBCLASS(GenericSite,GroundVehicle)
public:
    GenericSite();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.GroundVehicle.GenericSite"; }
};
  
// ---
// Radar_Site Structure (PS)
//  -- subclass of GroundVehicle
//
// "Radar site"
// ---
class RadarSite : public GroundVehicle {
    DECLARE_SUBCLASS(RadarSite,GroundVehicle)
public:
    RadarSite();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.GroundVehicle.Radar_Site"; }
};

// ---
// SAM_TELAR Structure (PS)
//  -- subclass of GroundVehicle
//
// "SAM TELAR"
// ---
class SAM_TELAR : public GroundVehicle {
    DECLARE_SUBCLASS(SAM_TELAR,GroundVehicle)
public:
    SAM_TELAR();
    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.Platform.GroundVehicle.SAM_TELAR"; }
};


// ---
// Embedded System Structure (N)
//
// "A base class used to associate sensor and emitting systems with their parent entity object."
// ---

// ---
// IFF Structure (PS)
//
// "Object which indicates whether or not the sending entity is friendly or foe."
// ---

// ---
// Designator Structure (PS)
//
// "A system used to designate or mark a location or object, such as a laser
//  designator which supports a laser-guided weapon engagement."
// ---

// ---
// Emitter System Structure (PS)
//
// "A device that radiates an electromagnetic signal, such as a radar or jammer."
// ---

// ---
// Radio Transmitter Structure (PS)
//
// "A device that sends out information encoded in electromagnetic waves in
//  the radio frequency range."
// ---

// ---
// Emitter Beam Structure (S)
//
// "A sector of concentrated energy from a device that radiates an electromagnetic signal. 
// See also IEEE 1278.1-1995 Section 5.4.7.1."
// ---
class EmitterBeam : public Basic::Object {
    DECLARE_SUBCLASS(EmitterBeam,Basic::Object)
public:
    EmitterBeam();
    
    static const char* getClassFedName()        { return "ObjectRoot.EmitterBeam"; }
    
    static const char* getBeamAzimuthCenterAttributeFedName()       { return "BeamAzimuthCenter"; }
    static const char* getBeamAzimuthSweepAttributeFedName()        { return "BeamAzimuthSweep"; }
    static const char* getBeamElevationCenterAttributeFedName()     { return "BeamElevationCenter"; }
    static const char* getBeamElevationSweepAttributeFedName()      { return "BeamElevationSweep"; }
    static const char* getBeamFunctionCodeAttributeFedName()        { return "BeamFunctionCode"; }
    static const char* getBeamIdentifierAttributeFedName()          { return "BeamIdentifier"; }
    static const char* getBeamParameterIndexAttributeFedName()      { return "BeamParameterIndex"; }
    static const char* getEffectiveRadiatedPowerAttributeFedName()  { return "EffectiveRadiatedPower"; }
    static const char* getEmissionFrequencyAttributeFedName()       { return "EmissionFrequency"; }
    static const char* getEmitterSystemIdentifierAttributeFedName() { return "EmitterSystemIdentifier"; }
    static const char* getEventIdentifierAttributeFedName()         { return "EventIdentifier"; }
    static const char* getFrequencyRangeAttributeFedName()          { return "FrequencyRange"; }
    static const char* getPulseRepetitionFrequencyAttributeFedName() { return "PulseRepetitionFrequency"; }
    static const char* getPulseWidthAttributeFedName()              { return "PulseWidth"; }
    static const char* getSweepSynchAttributeFedName()              { return "SweepSynch"; }
    
    // The angle, in azimuth, of the centre of the emitter beam\'s scan volume relative to the emitter system.
    // (radians) (UpdateCondition "> EE AZ THRSH")
    float beamAzimuthCenter;
                                
    // The angle, relative to the emitter system, indicating the half angle that the emitter beam sweeps through, in azimuth.
    // (radians) (UpdateCondition "> EE AZ THRSH")
    float beamAzimuthSweep;
                                
    // The angle, in elevation, of the centre of the emitter beam\'s scan volume relative to the emitter system. 
    // (radians) (UpdateCondition "> EE EL THRSH")
    float beamElevationCenter;
                                
    // The angle, relative to the emitter system, indicating the half angle that the emitter beam sweeps through, in elevation.
    // (radians) (UpdateCondition "> EE EL THRSH")
    float beamElevationSweep;
                                
    // The function of the emitter beam. (UpdateCondition "On change")
    BeamFunctionCodeEnum8   beamFunctionCode;
    
    // The identification of the emitter beam (must be unique on the emitter system).
    unsigned char  beamIdentifier;
    
    // The index, into the federation specific emissions database, of the current operating mode of the emitter beam.
    unsigned short beamParameterIndex;
    
    // The effective radiated power of the emitter beam.
    // (dBm) (UpdateCondition "On change")
    float effectiveRadiatedPower;
                                        
    // The centre frequency of the emitter beam.
    // (Hz) (UpdateCondition "> EE FREQ THRSH")
    float emissionFrequency; 
                                
    // The identification of the emitter sysytem that is generating this emitter beam.
    RTIObjectIdStruct emitterSystemIdentifier;
    
    // The EventIdentifier is used by the generating federate to associate related events. The event number shall start
    // at one at the beginning of the exercise, and be incremented by one for each event.
    EventIdentifierStruct eventIdentifier;
                                                
    // The bandwidth of the frequencies covered by the emitter beam.
    // (Hz) (UpdateCondition "> EE FRNG THRSH")
    float frequencyRange;
                        
    // The Pulse Repetition Frequency of the emitter beam.
    // (Hz) (UpdateCondition "> EE PRF THRSH")
    float pulseRepetitionFrequency;
                                        
    // The pulse width of the emitter beam.
    // (microsec) (UpdateCondition "> EE PW THRSH")
    float pulseWidth;
                        
    // The percentage of the way through the scan of the emitter beam.
    // (%) ((UpdateCondition "HRT BEAT TIMER secs")
    float sweepSynch;
};

// ---
// Jammer Beam Structure (PS)
//  -- subclass of EmitterBeam
//
// "An emitter beam that is designed to jam or otherwise interfere or confuse another emitter system"
// ---
class JammerBeam : public EmitterBeam {
    DECLARE_SUBCLASS(JammerBeam,EmitterBeam)
public:
    JammerBeam();
    static const char* getClassFedName() { return "ObjectRoot.EmitterBeam.JammerBeam"; }

    static const char* getJammingModeSequenceAttributeFedName()     { return "JammingModeSequence"; }
    static const char* getJammedObjectIdentifiersAttributeFedName() { return "JammedObjectIdentifiers"; }
    static const char* getHighDensityJamAttributeFedName()          { return "HighDensityJam"; }
    
    // The jamming mode technique or series of techniques being applied.
    unsigned long           jammingModeSequence;
    
    // Identification of the objects (emitter beams) being  jammed.
    RTIObjectIdArrayStruct  jammedObjectIdentifiers;
    
    // When TRUE the receiving simulation should assume that all emitter beams,
    // that are in the scan pattern of the jammer beam, are being jammed
    RTI::Boolean            highDensityJam;
};

// ---
// Radar Beam Structure (PS)
//  -- subclass of EmitterBeam
//
// "A sector of concentrated energy from a device that radiates an electromagnetic signal."
// ---
class RadarBeam : public EmitterBeam {
    DECLARE_SUBCLASS(RadarBeam,EmitterBeam)
public:
    RadarBeam();
    static const char* getClassFedName() { return "ObjectRoot.EmitterBeam.RadarBeam"; }

    static const char* getHighDensityTrackAttributeFedName()       { return "HighDensityTrack"; }
    static const char* getTrackObjectIdentifiersAttributeFedName() { return "TrackObjectIdentifiers"; }
    
    // When TRUE the receiving simulation should assume that all tqargets,
    // that are in the scan pattern of the radar beam, are being tracked
    RTI::Boolean            HighDensityTrack;
    
    // Identification of the objects (emitter beams) being  tracked.
    RTIObjectIdArrayStruct            TrackObjectIdentifiers;
};


//==============================================================================
// MC02: Interaction Structures
//==============================================================================

// ---
// WeaponFire (IR)
//
// "Communicates information associated with the firing or launch of a munition."
// ---

struct WeaponFire {
    static const char* getInteractionFedName() { return "WeaponFire"; }

    static const char* getEventIdentifierParameterFedName()             { return "EventIdentifier"; }
    static const char* getFireControlSolutionRangeParameterFedName()    { return "FireControlSolutionRange"; }
    static const char* getFireMissionIndexParameterFedName()            { return "FireMissionIndex"; }
    static const char* getFiringLocationParameterFedName()              { return "FiringLocation"; }
    static const char* getFiringObjectIdentifierParameterFedName()      { return "FiringObjectIdentifier"; }
    static const char* getFuseTypeParameterFedName()                    { return "FuseType"; }
    static const char* getInitialVelocityVectorParameterFedName()       { return "InitialVelocityVector"; }
    static const char* getMunitionObjectIdentifierParameterFedName()    { return "MunitionObjectIdentifier"; }
    static const char* getMunitionTypeParameterFedName()                { return "MunitionType"; }
    static const char* getQuantityFiredParameterFedName()               { return "QuantityFired"; }
    static const char* getRateOfFireParameterFedName()                  { return "RateOfFire"; }
    static const char* getTargetObjectIdentifierParameterFedName()      { return "TargetObjectIdentifier"; }
    static const char* getWarheadTypeParameterFedName()                 { return "WarheadType"; }

    EventIdentifierStruct eventIdentifier;          // An ID, generated by the issuing federate, used to associated related fire and detonation events.
    float               fireControlSolutionRange;   // (meters) The range used in the fire control solution. Zero if the range is unknown or inapplicable.
    unsigned long       fireMissionIndex;           // A unique index to identify the fire mission (used to associated weapon fire interactions in a single fire mission).
    WorldLocationStruct firingLocation;             // The location, the world coordinate system, of the weapon fire.
    RTIObjectIdStruct   firingObjectIdentifier;     // The ID of the object firing the munition.
    FuseTypeEnum16      fuseType;                   // The type of fuse on the munition.
    VelocityVectorStruct initialVelocityVector;     // The velocity vector of the munition when fired.
    RTIObjectIdStruct   munitionObjectIdentifier;   // he ID of the associated munition object (if any).
    EntityTypeStruct    munitionType;               // The type of munition being fired.
    unsigned short      quantityFired;              // The number of rounds fired in the fire event.
    unsigned short      rateOfFire;                 // The rate of fire at which the munitions in the burst described in the fire event.
    RTIObjectIdStruct   targetObjectIdentifier;     // The ID of the object being fired at (if any).
    WarheadTypeEnum16   warheadType;                // The type of warhead fitted to the munition being fired.
};

// ---
// MunitionDetonation (IR)
//
// "Communicates information associated with the impact or detonation of a munition"
// ---

struct MunitionDetonation {
    static const char* getInteractionFedName() { return "MunitionDetonation"; }

    static const char* getDetonationLocationParameterFedName()      { return "DetonationLocation"; }
    static const char* getDetonationResultCodeParameterFedName()    { return "DetonationResultCode"; }
    static const char* getEventIdentifierParameterFedName()         { return "EventIdentifier"; }
    static const char* getFiringObjectIdentifierParameterFedName()  { return "FiringObjectIdentifier"; }
    static const char* getFinalVelocityVectorParameterFedName()     { return "FinalVelocityVector"; }
    static const char* getFuseTypeParameterFedName()                { return "FuseType"; }
    static const char* getMunitionObjectIdentifierParameterFedName()    { return "MunitionObjectIdentifier"; }
    static const char* getMunitionTypeParameterFedName()            { return "MunitionType"; }
    static const char* getQuantityFiredParameterFedName()           { return "QuantityFired"; }
    static const char* getRateOfFireParameterFedName()              { return "RateOfFire"; }
    static const char* getRelativeDetonationLocationParameterFedName()  { return "RelativeDetonationLocation"; }
    static const char* getTargetObjectIdentifierParameterFedName()  { return "TargetObjectIdentifier"; }
    static const char* getWarheadTypeParameterFedName()             { return "WarheadType"; }

    WorldLocationStruct detonationLocation;         // The location, in the world coordinate system, of the detonation.
    DetonationResultCodeEnum8 detonationResultCode; // The type of detonation (including no detonation).
    EventIdentifierStruct eventIdentifier;          // An ID, generated by the issuing federate, used to associated related fire and detonation events.
    RTIObjectIdStruct   firingObjectIdentifier;     // The ID of the object firing the munition.
    VelocityVectorStruct finalVelocityVector;       // The velocity vector of the munition at the moment of the detonation.
    FuseTypeEnum16      fuseType;                   // The type of fuse on the munition.
    RTIObjectIdStruct   munitionObjectIdentifier;   // The ID of the associated munition object (if any).
    EntityTypeStruct    munitionType;               // The type of munition that is detonating.
    unsigned short      quantityFired;              // The quantity of rounds fired in a burst.
    unsigned short      rateOfFire;                 // The rate of fire, in rounds per minute, of the munitions in the burst.
    RelativePositionStruct relativeDetonationLocation; // The location of the detonation, relative to the target object (if any).
    RTIObjectIdStruct   targetObjectIdentifier;     // The ID of the object that the munition has detonated on.
    WarheadTypeEnum16   warheadType;                // The type of warhead on the munition.

};

} // End Mc02Fom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace

#endif 
