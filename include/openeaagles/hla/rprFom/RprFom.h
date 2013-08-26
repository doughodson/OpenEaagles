//------------------------------------------------------------------------------
// Real-time Platform Reference FOM (RPR-FOM) Data Structures
// RPR-FOM Version: 0.1.7
//------------------------------------------------------------------------------
#ifndef __Hla_RprFom_H__
#define __Hla_RprFom_H__

#include <RTI.hh>
#include "eaagles/basic/Object.h"
 
namespace Eaagles {
namespace Network {
namespace Hla {
namespace RprFom {

//------------------------------------------------------------------------------
// RPR-ROM: Enumerated Data
//------------------------------------------------------------------------------

// Articulated Type -- 
enum ArticulatedTypeEnum {
    ARTICULATED_TYPE_OTHER = 41,
    Rudder = 42,
    LeftFlap = 43,
    RightFlap = 44,
    LeftAileron = 45,
    RightAileron = 46,
    HelicopterMainRotor = 47,
    HelicopterTailRotor = 48,
    OtherAircraftControlSurfaces = 49,
    Periscope = 50,
    GenericAntenna = 51,
    Snorkel = 52,
    OtherExtendableParts = 53,
    LandingGear = 54,
    TailHook = 55,
    SpeedBrake = 56,
    LeftWeaponBayDoors = 57,
    RightWeaponBayDoors = 58,
    TankOrAPChatch = 59,
    Wingsweep = 60,
    BridgeLauncher = 61,
    BridgeSection1 = 62,
    BridgeSection2 = 63,
    BridgeSection3 = 64,
    PrimaryBlade1 = 65,
    PrimaryBlade2 = 66,
    PrimaryBoom = 67,
    PrimaryLauncherArm = 68,
    OtherFixedPositionParts = 69,
    PrimaryTurretNumber1 = 70,
    PrimaryTurretNumber2 = 71,
    PrimaryTurretNumber3 = 72,
    PrimaryTurretNumber4 = 73,
    PrimaryTurretNumber5 = 74,
    PrimaryTurretNumber6 = 75,
    PrimaryTurretNumber7 = 76,
    PrimaryTurretNumber8 = 77,
    PrimaryTurretNumber9 = 78,
    PrimaryTurretNumber10 = 79,
    PrimaryGunNumber1 = 80,
    PrimaryGunNumber2 = 81,
    PrimaryGunNumber3 = 82,
    PrimaryGunNumber4 = 83,
    PrimaryGunNumber5 = 84,
    PrimaryGunNumber6 = 85,
    PrimaryGunNumber7 = 86,
    PrimaryGunNumber8 = 87,
    PrimaryGunNumber9 = 88,
    PrimaryGunNumber10 = 89,
    PrimaryLauncher1 = 90,
    PrimaryLauncher2 = 91,
    PrimaryLauncher3 = 92,
    PrimaryLauncher4 = 93,
    PrimaryLauncher5 = 94,
    PrimaryLauncher6 = 95,
    PrimaryLauncher7 = 96,
    PrimaryLauncher8 = 97,
    PrimaryLauncher9 = 98,
    PrimaryLauncher10 = 99,
    PrimaryDefenseSystems1 = 100,
    PrimaryDefenseSystems2 = 101,
    PrimaryDefenseSystems3 = 102,
    PrimaryDefenseSystems4 = 103,
    PrimaryDefenseSystems5 = 104,
    PrimaryDefenseSystems6 = 105,
    PrimaryDefenseSystems7 = 106,
    PrimaryDefenseSystems8 = 107,
    PrimaryDefenseSystems9 = 108,
    PrimaryDefenseSystems10 = 109,
    PrimaryRadar1 = 110,
    PrimaryRadar2 = 111,
    PrimaryRadar3 = 112,
    PrimaryRadar4 = 113,
    PrimaryRadar5 = 114,
    PrimaryRadar6 = 115,
    PrimaryRadar7 = 116,
    PrimaryRadar8 = 117,
    PrimaryRadar9 = 118,
    PrimaryRadar10 = 119,
    SecondaryTurretNumber1 = 120,
    SecondaryTurretNumber2 = 121,
    SecondaryTurretNumber3 = 122,
    SecondaryTurretNumber4 = 123,
    SecondaryTurretNumber5 = 124,
    SecondaryTurretNumber6 = 125,
    SecondaryTurretNumber7 = 126,
    SecondaryTurretNumber8 = 127,
    SecondaryTurretNumber9 = 128,
    SecondaryTurretNumber10 = 129,
    SecondaryGunNumber1 = 130,
    SecondaryGunNumber2 = 131,
    SecondaryGunNumber3 = 132,
    SecondaryGunNumber4 = 133,
    SecondaryGunNumber5 = 134,
    SecondaryGunNumber6 = 135,
    SecondaryGunNumber7 = 136,
    SecondaryGunNumber8 = 137,
    SecondaryGunNumber9 = 138,
    SecondaryGunNumber10 = 139,
    SecondaryLauncher1 = 140,
    SecondaryLauncher2 = 141,
    SecondaryLauncher3 = 142,
    SecondaryLauncher4 = 143,
    SecondaryLauncher5 = 144,
    SecondaryLauncher6 = 145,
    SecondaryLauncher7 = 146,
    SecondaryLauncher8 = 147,
    SecondaryLauncher9 = 148,
    SecondaryLauncher10 = 149,
    SecondaryDefenseSystems1 = 150,
    SecondaryDefenseSystems2 = 151,
    SecondaryDefenseSystems3 = 152,
    SecondaryDefenseSystems4 = 153,
    SecondaryDefenseSystems5 = 154,
    SecondaryDefenseSystems6 = 155,
    SecondaryDefenseSystems7 = 156,
    SecondaryDefenseSystems8 = 157,
    SecondaryDefenseSystems9 = 158,
    SecondaryDefenseSystems10 = 159,
    SecondaryRadar1 = 160,
    SecondaryRadar2 = 161,
    SecondaryRadar3 = 162,
    SecondaryRadar4 = 163,
    SecondaryRadar5 = 164,
    SecondaryRadar6 = 165,
    SecondaryRadar7 = 166,
    SecondaryRadar8 = 167,
    SecondaryRadar9 = 168,
    SecondaryRadar10 = 169
};

// Character Set -- 
enum CharacterSetEnum {
    CHARACTER_SET_OTHER = 183
};

// Damage State -- 
enum DamageStateEnum {
   NO_DAMAGE = 195,
   SLIGHT_DAMAGE = 196,
   MODERATE_DAMAGE = 197,
   DESTROYED = 198
};

// Detonation Result -- 
enum DetonationResultEnum {
   DETONATION_RESULT_OTHER = 203,
   ENTITY_IMPACT = 204,
   ENTITY_PROXIMATE_DETONATION = 205,
   GROUND_IMPACT = 206,
   GROUND_PROXIMATE_DETONATION = 207,
   DETONATION = 208,
   NONE = 209,
   HE_HIT_SMALL = 210,
   HE_HIT_MEDIUM = 211,
   HE_HIT_LARGE = 212,
   ARMOR_PIERCING_HIT = 213,
   DIRT_BLAST_SMALL = 214,
   DIRT_BLAST_MEDIUM = 215,
   DIRT_BLAST_LARGE = 216,
   WATER_BLAST_SMALL = 217,
   WATER_BLAST_MEDIUM = 218,
   WATER_BLAST_LARGE = 219,
   AIR_HIT = 220,
   BUILDING_HIT_SMALL = 221,
   BUILDING_HIT_MEDIUM = 222,
   BUILDING_HIT_LARGE = 223,
   ENVIRONMENT_OBJECT_PROXIMATE_DETONATION = 226,
   ENVIRONMENT_OBJECT_IMPACT = 225,
   MINE_CLEARING_LINE_CHARGE = 224,
   WATER_IMPACT = 227,
   AIR_BURST = 228
};

// DR Algorithm -- 
enum DRAlgorithmEnum {
    DR_ALGORITHM_OTHER = 229,
    STATIC = 230,
    DRM_FPW = 231,
    DRM_RPW = 232,
    DRM_RVW = 233,
    DRM_FVW = 234,
    DRM_FPB = 235,
    DRM_RPB = 236,
    DRM_RVB = 237,
    DRM_FVB = 238
};

// ForceIdEnum -- 
enum ForceIdEnum {
    FORCE_ID_OTHER = 281,
    FRIENDLY = 282,
    OPPOSING = 283,
    NEUTRAL = 284
};

enum FuseTypeEnum {
   FUSE_TYPE_OTHER = 291,
   IntelligentInfluence = 292,
   Sensor = 293,
   SelfDestruct = 294,
   UltraQuick = 295,
   Body = 296,
   DeepIntrusion = 297,
   Multifunction = 298,
   PointDetonation_PD = 299,
   BaseDetonation_BD = 300,
   Contact = 301,
   ContactInstantImpact = 302,
   ContactDelayed = 303,
   ContactElectronicObliqueContact = 304,
   ContactGraze = 305,
   ContactCrush = 306,
   ContactHydrostatic = 307,
   ContactMechanical = 308,
   ContactChemical = 309,
   ContactPiezoelectric = 310,
   ContactPointInitiating = 311,
   ContactPointInitiatingBaseDetonating = 312,
   ContactBaseDetonating = 313,
   ContactBallisticCapAndBase = 314,
   ContactBase = 315,
   ContactNose = 316,
   ContactFittedInStandoffProbe = 317,
   ContactNonAligned = 318,
   Timed = 319,
   TimedProgrammable = 320,
   TimedBurnout = 321,
   TimedPyrotechnic = 322,
   TimedElectronic = 323,
   TimedBaseDelay = 324,
   TimedReinforcedNoseImpactDelay = 325,
   TimedShortDelayImpact = 326,
   TimedNoseMountedVariableDelay = 327,
   TimedLongDelaySide = 328,
   TimedSelectableDelay = 329,
   TimedImpact = 330,
   TimedSequence = 331,
   Proximity = 332,
   ProximityActiveLaser = 333,
   ProximityMagneticMagpolarity = 334,
   ProximityActiveDopplerRadar = 335,
   ProximityRadioFrequencyRF = 336,
   ProximityProgrammable = 337,
   ProximityProgrammablePrefragmented = 338,
   ProximityInfrared = 339,
   Command = 340,
   CommandElectronicRemotelySet = 341,
   Altitude = 342,
   AltitudeRadioAltimeter = 343,
   AltitudeAirBurst = 344,
   Depth = 345,
   Acoustic = 346,
   Pressure = 347,
   PressureDelay = 348,
   Inert = 349,
   FUSE_TYPE_DUMMY = 350,
   FUSE_TYPE_PRACTICE = 351,
   PlugRepresenting = 352,
   Training = 353,
   Pyrotechnic = 354,
   PyrotechnicDelay = 355,
   ElectroOptical = 356,
   ElectroMechanical = 357,
   ElectroMechanicalNose = 358,
   Strikerless = 359,
   StrikerlessNoseImpact = 360,
   StrikerlessCompressionIgnition = 361,
   CompressionIgnition = 362,
   CompressionIgnitionStrikerlessNoseImpact = 363,
   Percussion = 364,
   PercussionInstantaneous = 365,
   Electronic = 366,
   ElectronicInternallyMounted = 367,
   ElectronicRangeSetting = 368,
   ElectronicProgrammed = 369,
   Mechanical = 370,
   MechanicalNose = 371,
   MechanicalTail = 372,
};

// Hatch State -- 
enum HatchStateEnum {
   HATCH_STATE_NOT_APPLICABLE = 373,
   PRIMARY_HATCH_IS_CLOSED = 374,
   PRIMARY_HATCH_IS_POPPED = 375, 
   PRIMARY_HATCH_IS_POPPED_AND_PERSON_IS_VISIBLE_UNDER_HATCH = 376,
   PRIMARY_HATCH_IS_OPEN = 377,
   PRIMARY_HATCH_IS_OPEN_AND_PERSON_IS_VISIBLE = 378
};

// Lifeform State -- 
enum LifeformStateEnum {
   LIFEFORM_STATE_NOT_APPLICABLE = 379,
   UPRIGHT_STANDING_STILL = 380,
   UPRIGHT_WALKING = 381,
   UPRIGHT_RUNNING = 382,
   KNEELING = 383,
   PRONE = 384,
   CRAWLING = 385,
   SWIMMING = 386,
   PARACHUTING = 387,
   JUMPING = 388,
   SITTING = 389,
   SQUATTING = 390,
   CROUCHING = 391,
   WADING = 392
};

// Light State -- 
enum LightStateEnum {
    LIGHT_STATE_OTHER = 393
};

// Parameter Type -- 
enum ParameterTypeEnum {
    PARAMETER_TYPE_DUMMY = 411
};

// Trail State -- 
enum TrailStateEnum {
    TRAIL_STATE_OTHER = 419
};

// Warhead Type -- 
enum WarheadTypeEnum {
    WARHEAD_TYPE_OTHER = 420,
    CargoVariableSubmunitions = 421,
    FuelAirExplosive = 422,
    GlassBlads = 423,
    Warhead_1um = 424,
    Warhead_5um = 425,
    Warhead_10um = 426,
    HighExplosive = 427,
    HE_Plastic = 428,
    HE_Incendiary = 429,
    HE_Fragmentation = 430,
    HE_Antitank = 431,
    HE_Bomblets = 432,
    HE_ShapedCharge = 433,
    HE_ContinuousRod = 434,
    HE_TungstenBall = 435,
    HE_BlastFragmentation = 436,
    HE_SteerableDartswithHE = 437,
    HE_Darts = 438,
    HE_Flechettes = 439,
    HE_DirectedFragmentation = 440,
    HE_SemiArmorPiercing = 441,
    HE_ShapedChargeFragmentation = 442,
    HE_SemiArmorPiercingFragmentation = 443,
    HE_HollowCharge = 444,
    HE_GeneralPurpose = 446,
    HE_DoubleHollowCharge = 445,
    HE_BlastPenetrator = 447,
    HE_RodPenetrator = 448,
    HE_Antipersonnel = 449,
    Smoke = 450,
    Illumination = 451,
    WARHEAD_TYPE_PRACTICE = 452,
    Kinetic = 453,
    Mines = 454,
    Nuclear = 455,
    NuclearIMT = 456,
    ChemicalGeneral = 457,
    ChemicalBlisterAgent = 458,
    HD_Mustard = 459,
    ThickenedHD_Mustard = 460,
    DustyHD_Mustard = 461,
    ChemicalBloodAgent = 462,
    AC_HCN = 463,
    CK_CNCI = 464,
    CG_Phosgene = 465,
    ChemicalNerveAgent = 466,
    VX = 467,
    ThickenedVX = 468,
    DustyVX = 469,
    GA_Tabun = 470,
    ThickenedGA_Tabun = 471,
    DustyGA_Tabun = 472,
    GB_Sarin = 473,
    ThickenedGB_Sarin = 474,
    DustyGB_Sarin = 475,
    GD_Soman = 476,
    ThickenedGD_Soman = 477,
    DustyGD_Soman = 478,
    GF = 479,
    ThickenedGF = 480,
    DustyGF = 481,
    Biological = 482,
    BiologicalVirus = 483,
    BiologicalBacteria = 484,
    BiologicalRickettsia = 485,
    BiologicalGeneticallyModifiedMicroOrganisms = 486,
    BiologicalToxin = 487
};

// Weapon State -- 
enum WeaponStateEnum {
    NO_WEAPON = 488,
    STOWED = 489,
    DEPLOYED = 490,
    FIRING_POSITION = 491
};

//------------------------------------------------------------------------------
// RPR-ROM: Complex Data Structures
//------------------------------------------------------------------------------

// Position (meters)
struct PositionStruct {
    PositionStruct() { x = 0.0; y = 0.0; z = 0.0; }
    double      x;   // meters
    double      y;   // meters
    double      z;   // meters
};

// Relative Position (meters)
struct RelativePositionStruct {
    RelativePositionStruct() { bodyX = 0.0; bodyY = 0.0; bodyZ = 0.0; }
    float bodyX; // meters
    float bodyY; // meters
    float bodyZ; // meters
};

// EntityTypeStruct
struct EntityTypeStruct {
    EntityTypeStruct() { subcategory = 0; entityKind = 0; specific = 0; domain = 0; extra = 0; category = 0; country = 0; }
    unsigned int entityKind;
    unsigned int domain;
    unsigned int country;
    unsigned int category;
    unsigned int subcategory;
    unsigned int specific;
    unsigned int extra;
};

// FederateIdStruct
struct FederateIdStruct {
    FederateIdStruct() { applicationID = 0; entityID = 0; siteID = 0; }
    short       siteID;
    short       applicationID;
    short       entityID;
};

// EventIDStruct
struct EventIDStruct {
    EventIDStruct() { issuingObjectID = 0; eventCount = 0; }
    unsigned long issuingObjectID;
    short eventCount;
};

// Velocity
struct VelocityStruct {
    VelocityStruct() { xVelocity = 0.0; yVelocity = 0.0; zVelocity = 0.0; }
    float       xVelocity; // (meters/second)
    float       yVelocity; // (meters/second)
    float       zVelocity; // (meters/second)
};

// Acceleration
struct AccelerationStruct {
    AccelerationStruct() { xAcceleration = 0.0; yAcceleration = 0.0; zAcceleration = 0.0; }
    float       xAcceleration;  // (meters/second/second)
    float       yAcceleration;  // (meters/second/second)
    float       zAcceleration;  // (meters/second/second)
};

// Orientation
struct OrientationStruct {
    OrientationStruct() { psi = 0.0; theta = 0.0; phi = 0.0; }
    float       psi;    // (radians)
    float       theta;  // (radians)
    float       phi;    // (radians)
};

// Angular Velocity
struct AngVelocityStruct {
    AngVelocityStruct() { xAxisRate = 0.0; yAxisRate = 0.0; zAxisRate = 0.0; }
    float       xAxisRate; // (radians/second)
    float       yAxisRate; // (radians/second)
    float       zAxisRate; // (radians/second)
};


// Markings
struct MarkingStruct {
    MarkingStruct() { characterSet = CHARACTER_SET_OTHER; for (int i = 0; i < 11; markingString[i++] = '\0'); }
    CharacterSetEnum  characterSet;
    unsigned char     markingString[11];
};

// Parameter Value Type
struct ParameterValueType {
    ParameterValueType() { dummy = 0;  }
    unsigned char dummy;
};

// Articulated Parameter
struct ArticulatedParameterStruct {
   ArticulatedParameterStruct() : parameterValue() {
       partAttachedTo = 0; articulatedParameterChange = 0;
       articulatedParameterType = ARTICULATED_TYPE_OTHER;
       parameterType = PARAMETER_TYPE_DUMMY; 
   }
   short               partAttachedTo;
   unsigned char       articulatedParameterChange; 
   ArticulatedTypeEnum articulatedParameterType; 
   ParameterValueType  parameterValue; 
   ParameterTypeEnum   parameterType; 
};


//==============================================================================
// RPR-ROM: Object Class Structures
//==============================================================================

// ---
// Base Entity Structure (S)
//
// "A base class of all scenario domain participants\054 both aggregate and
// discrete. The BaseEntity class is intended to be a container for common
// attributes for entities of all classes.  Since it lacks sufficient class
// specific attributes that are required for simulation purposes it is not
// expected that any federate shall publish objects of this class.  Certain
// simulation management federates (e.g. viewers) may subscribe to this
// class.  Simulation federates will normally subscribe to one of the subclasses
// to gain the extra information required to properly simulate the entity."
//
// ---

class BaseEntity : public Basic::Object {
    DECLARE_SUBCLASS(BaseEntity,Basic::Object)
public:
    BaseEntity();
    
    static const char* getClassFedName()                            { return "HLAobjectRoot.BaseEntity"; }
    
    static const char* getAccelerationVectorAttributeFedName()      { return "AccelerationVector"; }
    static const char* getAngularVelocityVectorA22ttributeFedName()   { return "AngularVelocityVector"; }
    static const char* getDrAlgorithmAttributeFedName()             { return "DRAlgorithm"; }
    static const char* getEntityTypeAttributeFedName()              { return "EntityType"; }
    static const char* getFederateIdAttributeFedName()              { return "FederateID"; }
    static const char* getIsFrozenAttributeFedName()                { return "IsFrozen"; }
    static const char* getOrientationAttributeFedName()             { return "Orientation"; }
    static const char* getPositionAttributeFedName()                { return "Position"; }
    static const char* getVelocityVectorAttributeFedName()          { return "VelocityVector"; }

    AccelerationStruct          accelerationVector;     // Acceleration of the entity.
    AngVelocityStruct           angularVelocityVector;  // Rate of change of the entity's orientation
    DRAlgorithmEnum             deadReckoningAlgorithm; // Dead reckoning algorithm
    EntityTypeStruct            entityType;             // Type of the entity.
    FederateIdStruct            federateID;             // Unique identifier for this entity
    RTI::Boolean                isFrozen;               // Whether the entity is frozen or not
    OrientationStruct           orientation;            // Entity's Euler Angles
    PositionStruct              position;               // Location of the entity.
    VelocityStruct              velocityVector;         // Velocity of the entity.
};

// ---
// Physical Entity Structure (PS)
//  -- subclass of BaseEntity
//
// A base class of all discrete platform scenario domain participants."
// ---
class PhysicalEntity : public BaseEntity {
    DECLARE_SUBCLASS(PhysicalEntity,BaseEntity)
public:
    PhysicalEntity();

    static const char* getClassFedName() { return "HLAobjectRoot.BaseEntity.PhysicalEntity"; }

    static const char* getDamageStateAttributeFedName()              { return "DamageState"; }
    static const char* getEngineSmokeOnAttributeFedName()            { return "EngineSmokeOn"; }
    static const char* getFlamesPresentAttributeFedName()            { return "FlamesPresent"; }
    static const char* getHasFuelSupplyCapAttributeFedName()         { return "HasFuelSupplyCap"; }
    static const char* getHasRecoveryCapAttributeFedName()           { return "HasRecoveryCap"; }
    static const char* getHasRepairCapAttributeFedName()             { return "HasRepairCap"; }
    static const char* getHatchStateAttributeFedName()               { return "HatchState"; }
    static const char* getImmobilizedAttributeFedName()              { return "Immobilized"; }
    static const char* getLifeformStateAttributeFedName()            { return "LifeformState"; }
    static const char* getLightsStateAttributeFedName()              { return "LightsState"; }
    static const char* getMarkingAttributeFedName()                  { return "Marking"; }
    static const char* getPowerPlantOnAttributeFedName()             { return "PowerPlantOn"; }
    static const char* getRampDeployedAttributeFedName()             { return "RampDeployed"; }
    static const char* getSmokePlumePresentAttributeFedName()        { return "SmokePlumePresent"; }
    static const char* getTentDeployedAttributeFedName()             { return "TentDeployed"; }
    static const char* getTrailStateAttributeFedName()               { return "TrailState"; }

    DamageStateEnum             damageState;            // The state of damage of the entity.
    RTI::Boolean                engineSmokeOn;          // Whether the entity's engine is generating smoke or not.
    RTI::Boolean                flamesPresent;          // Whether the entity is on fire (with visible flames) or not.
    RTI::Boolean                hasFuelSupplyCap;       // Whether the entity has the capability to supply other entities with fuel or not.
    RTI::Boolean                hasRecoveryCap;         // Whether the entity has the capability to recover other entities or not.
    RTI::Boolean                hasRepairCap;           // Whether the entity has the capability to repair other entities or not.
    HatchStateEnum              hatchState;             // The state of the entity's (main) hatch.
    RTI::Boolean                immobilized;            // Whether the entity is immobilized or not.
    LifeformStateEnum           lifeformState;          // 
    LightStateEnum              lightsState;            // 
    MarkingStruct               marking;                // A unique marking or combination of characters used to distinguish the entity from
    RTI::Boolean                powerPlantOn;           // Whether the entity's power plant is on or not.
    RTI::Boolean                rampDeployed;           // Whether the entity has deployed a ramp or not.
    RTI::Boolean                smokePlumePresent;      // Whether the entity is generating smoke or not (intentional or unintentional).
    RTI::Boolean                tentDeployed;           // Whether the entity has deployed tent or not.
    TrailStateEnum              trailState;             // 
};

// ---
// Military Entity Structure (S)
//  -- subclass of PhysicalEntity
// ---
class MilitaryEntity : public PhysicalEntity {
    DECLARE_SUBCLASS(MilitaryEntity,PhysicalEntity)
public:
    MilitaryEntity();

    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity"; }

    static const char* getAlternateEntityTypeAttributeFedName()   { return "AlternateEntityType"; }
    static const char* getCamouflageTypeAttributeFedName()        { return "CamouflageType"; }
    static const char* getIsConcealedAttributeFedName()           { return "IsConcealed"; }
    static const char* getFirePowerDisabledAttributeFedName()     { return "FirePowerDisabled"; }
    static const char* getForceIDAttributeFedName()               { return "ForceID"; }
    
    EntityTypeStruct alternateEntityType;    // The category of entity to be used when viewed by entities on the "opposite" side.
    RTI::Boolean     camouflageType;         // The type of camouflage in use (if any).
    RTI::Boolean     isConcealed;            // Whether the entity is concealed or not.
    RTI::Boolean     firePowerDisabled;      // Whether the entity's main weapon system has been disabled or not.
    ForceIdEnum      forceID;                // The identification of the force that the entity belongs to.
};

// ---
// Soldier Structure (PS)
//  -- subclass of MilitaryEntity
//
// "Soldier"
// ---
class Soldier : public MilitaryEntity {
    DECLARE_SUBCLASS(Soldier,MilitaryEntity)
public:
    Soldier();

    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity.Soldier"; }

    static const char* getPrimaryWeaponStateAttributeFedName()    { return "PrimaryWeaponState"; }
    static const char* getSecondaryWeaponStateAttributeFedName()  { return "SecondaryWeaponState"; }
    
    WeaponStateEnum  primaryWeaponState;   // N/A
    WeaponStateEnum  secondaryWeaponState; // N/A
};
  
// ---
// MunitionEntity Structure (PS)
//  -- subclass of MilitaryEntity
//
// "A complete device charged with explosives, propellants, pyrotechnics, initiating composition,
//  or nuclear, biological or chemical material for use in military operations, including demolitions."
// ---
class MunitionEntity : public MilitaryEntity {
    DECLARE_SUBCLASS(MunitionEntity,MilitaryEntity)
public:
    MunitionEntity();

    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity.MunitionEntity"; }

    static const char* getLauncherFlashPresentAttributeFedName()      { return "LauncherFlashPresent"; }

    RTI::Boolean launcherFlashPresent;   // Whether the flash of the munition being launched is present or not.
};


// ---
// MilitaryPlatformEntity Structure (PS)
//  -- subclass of MilitaryEntity
//
// "A physical object under the control of armed forces upon which sensor, communication,
//  or weapon systems may be mounted."
// ---
class MilitaryPlatformEntity : public MilitaryEntity {
    DECLARE_SUBCLASS(MilitaryPlatformEntity,MilitaryEntity)
public:
    MilitaryPlatformEntity();

    static const char* getClassFedName() { return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity.MilitaryPlatformEntity"; }

    static const char* getAfterburnerOnAttributeFedName()   { return "AfterburnerOn"; }
    static const char* getHasAmmunitionSupplyCapAttributeFedName()  { return "HasAmmunitionSupplyCap"; }
    static const char* getLauncherRaisedAttributeFedName()  { return "LauncherRaised"; }
    
    RTI::Boolean                afterburnerOn;          // Whether the entity's afterburner is on or not.
    RTI::Boolean                hasAmmunitionSupplyCap; // Whether the entity has the capability to supply other entities with ammunition.
    RTI::Boolean                launcherRaised;         // Whether the entity's weapon launcher is in the raised position.
};
  
// ---
// MilitaryMultiDomainPlatform Structure (PS)
//  -- subclass of MilitaryPlatformEntity
//
// "A military platform entity that operates in more than one domain (excluding those
//  combinations explicitly defined as subclasses of the superclass of this class)."
// ---
class MilitaryMultiDomainPlatform : public MilitaryPlatformEntity {
    DECLARE_SUBCLASS(MilitaryMultiDomainPlatform,MilitaryPlatformEntity)
public:
    MilitaryMultiDomainPlatform();

    static const char* getClassFedName() {
       return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity.MilitaryPlatformEntity.MilitaryMultiDomainPlatform"; 
    }
};
  
// ---
// MilitarySubmersiblePlatform Structure (PS)
//  -- subclass of MilitaryPlatformEntity
//
// "A military platform entity that operates either on the surface of the sea, or beneath it."
// ---
class MilitarySubmersiblePlatform : public MilitaryPlatformEntity {
    DECLARE_SUBCLASS(MilitarySubmersiblePlatform,MilitaryPlatformEntity)
public:
    MilitarySubmersiblePlatform();

    static const char* getClassFedName() {
       return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity.MilitaryPlatformEntity.MilitarySubmersiblePlatform"; 
    }
};
  
// ---
// MilitarySeaSurfacePlatform Structure (PS)
//  -- subclass of MilitaryPlatformEntity
//
// "A military platform entity that operates wholey on the surface of the sea."
// ---
class MilitarySeaSurfacePlatform : public MilitaryPlatformEntity {
    DECLARE_SUBCLASS(MilitarySeaSurfacePlatform,MilitaryPlatformEntity)
public:
    MilitarySeaSurfacePlatform();

    static const char* getClassFedName() {
       return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity.MilitaryPlatformEntity.MilitarySubmersiblePlatform"; 
    }
};
  
// ---
// MilitarySpacePlatform Structure (PS)
//  -- subclass of MilitaryPlatformEntity
//
// "A military platform entity that operates mainly in space."
// ---
class MilitarySpacePlatform : public MilitaryPlatformEntity {
    DECLARE_SUBCLASS(MilitarySpacePlatform,MilitaryPlatformEntity)
public:
    MilitarySpacePlatform();

    static const char* getClassFedName() {
       return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity.MilitaryPlatformEntity.MilitarySpacePlatform"; 
    }
};
  
// ---
// MilitaryLandPlatform Structure (PS)
//  -- subclass of MilitaryPlatformEntity
//
// "A military platform entity that operates wholey on the surface of the earth."
// ---
class MilitaryLandPlatform : public MilitaryPlatformEntity {
    DECLARE_SUBCLASS(MilitaryLandPlatform,MilitaryPlatformEntity)
public:
    MilitaryLandPlatform();

    static const char* getClassFedName() {
       return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity.MilitaryPlatformEntity.MilitaryLandPlatform"; 
    }
};
  
// ---
// MilitaryAmphibiousPlatform Structure (PS)
//  -- subclass of MilitaryPlatformEntity
//
// "A military platform entity that can operate both on the land and the sea."
// ---
class MilitaryAmphibiousPlatform : public MilitaryPlatformEntity {
    DECLARE_SUBCLASS(MilitaryAmphibiousPlatform,MilitaryPlatformEntity)
public:
    MilitaryAmphibiousPlatform();

    static const char* getClassFedName() {
       return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity.MilitaryPlatformEntity.MilitaryAmphibiousPlatform"; 
    }
};
  
// ---
// MilitaryAirLandPlatform Structure (PS)
//  -- subclass of MilitaryPlatformEntity
//
// "A military platform entity that operates mainly in the air, such as aircraft,
//  balloons, etc.  This includes the entities when they are on the ground."
// ---
class MilitaryAirLandPlatform : public MilitaryPlatformEntity {
    DECLARE_SUBCLASS(MilitaryAirLandPlatform,MilitaryPlatformEntity)
public:
    MilitaryAirLandPlatform();

    static const char* getClassFedName() {
       return "ObjectRoot.BaseEntity.PhysicalEntity.MilitaryEntity.MilitaryPlatformEntity.MilitaryAmphibiousPlatform"; 
    }
};


//==============================================================================
// RPR-ROM: Interaction Structures
//==============================================================================

// ---
// WeaponFire (IR)
//
// "Communicates information associated with the firing or launch of a munition."
// ---

struct WeaponFire {

    static const char* getInteractionFedName() { return "WeaponFire"; }

    static const char* getEventIDParameterFedName()                     { return "EventID"; }
    static const char* getFireControlSolutionRangeParameterFedName()    { return "FireControlSolutionRange"; }
    static const char* getFireMissionIndexParameterFedName()            { return "FireMissionIndex"; }
    static const char* getFiringLocationParameterFedName()              { return "FiringLocation"; }
    static const char* getFiringObjectIDParameterFedName()              { return "FiringObjectID"; }
    static const char* getFuseTypeParameterFedName()                    { return "FuseType"; }
    static const char* getInitialVelocityVectorParameterFedName()       { return "InitialVelocityVector"; }
    static const char* getMunitionObjectIDParameterFedName()            { return "MunitionObjectID"; }
    static const char* getMunitionTypeParameterFedName()                { return "MunitionType"; }
    static const char* getQuantityFiredParameterFedName()               { return "QuantityFired"; }
    static const char* getRateOfFireParameterFedName()                  { return "RateOfFire"; }
    static const char* getTargetObjectIDParameterFedName()              { return "TargetObjectID"; }
    static const char* getWarheadTypeParameterFedName()                 { return "WarheadType"; }


    EventIDStruct eventId;          // An ID, generated by the issuing federate, used to
                                    //  associated related fire and detonation events.
    float fireControlSolutionRange; // (meters) The range used in the fire control solution.
                                    // Zero if the range is unknown or inapplicable.
    unsigned long fireMissionIndex; // A unique index to identify the fire mission
                                    //   (used to associated weapon fire interactions in a
                                    //    single fire mission).
    PositionStruct firingLocation;  // The location, the world coordinate system, of the weapon fire.
    unsigned long firingObjectId;   // The ID of the object firing the munition.
    FuseTypeEnum fuseType;          // The type of fuse on the munition.
    VelocityStruct initialVelocityVector; // The velocity vector of the munition when fired.
    unsigned long munitionObjectId; // The ID of the associated munition object (if any).
    EntityTypeStruct munitionType;  // The type of munition being fired.
    unsigned short quantityFired;   // The number of rounds fired in the fire event.
    unsigned short rateOfFire;      // The rate of fire at which the munitions in the burst
                                    //  described in the fire event.
    unsigned long targetObjectId;   // The ID of the object being fired at (if any).
    WarheadTypeEnum warheadType;    // The type of warhead fitted to the munition being fired.

};

// ---
// MunitionDetonation (IR)
//
// "Communicates information associated with the the impact or detonation of a munition"
// ---

struct MunitionDetonation {

    static const char* getInteractionFedName() { return "MunitionDetonation"; }

    static const char* getDetonationLocationParameterFedName()    { return "DetonationLocation"; }
    static const char* getDetonationResultParameterFedName()      { return "DetonationResult"; }
    static const char* getEventIdParameterFedName()               { return "EventID"; }
    static const char* getFinalVelocityVectorParameterFedName()   { return "FinalVelocityVector"; }
    static const char* getFiringObjectIdParameterFedName()        { return "FiringObjectID"; }
    static const char* getFuseTypeParameterFedName()              { return "FuseType"; }
    static const char* getMunitionObjectIdParameterFedName()      { return "MunitionObjectID"; }
    static const char* getMunitionTypeParameterFedName()          { return "MunitionType"; }
    static const char* getQuantityFiredParameterFedName()         { return "QuantityFired"; }
    static const char* getRateOfFireParameterFedName()            { return "RateOfFire"; }
    static const char* getRelativeDetonationLocationParameterFedName()  { return "RelativeDetonationLocation"; }
    static const char* getTargetObjectIdParameterFedName()        { return "TargetObjectID"; }
    static const char* getWarheadTypeParameterFedName()           { return "WarheadType"; }


    PositionStruct detonationLocation;       // The location, in the world coordinate system, of the detonation.
    DetonationResultEnum detonationResult;   // The type of detonation (including no detonation).
    EventIDStruct eventID;                   // An ID, generated by the issuing federate, used to associated
                                             // related fire and detonation events.
    VelocityStruct finalVelocityVector;      // The velocity vector of the munition at the moment of the detonation.
    unsigned long firingObjectID;            // The ID of the object firing the munition.
    FuseTypeEnum  fuseType;                  // The type of fuse on the munition.
    unsigned long munitionObjectID;          // The ID of the associated munition object (if any).
    EntityTypeStruct munitionType;           // The type of munition that is detonating.
    unsigned short quantityFired;            // The quantity of rounds fired in a burst.
    unsigned short rateOfFire;               // The rate of fire, in rounds per minute, of the munitions
                                             // in the burst.
    RelativePositionStruct relativeDetonationLocation; // The location of the detonation,
                                             // relative to the target object (if any).
    unsigned long targetObjectID;            // The ID of the object that the munition has detonated on.
    WarheadTypeEnum warheadType;             // The type of warhead on the munition.


};

} // End __Hla_RprFom_H__ namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace

#endif 
