
#ifndef __oe_rprfom_H__
#define __oe_rprfom_H__

#include <RTI.hh>
#include "openeaagles/base/Object.hpp"

namespace oe {
namespace rprfom {

//------------------------------------------------------------------------------
// Real-time Platform Reference FOM (RPR-FOM) Data Structures
// RPR-FOM Version: 2.0 Draft 17
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// RPR-ROM: Enumerated Data
//------------------------------------------------------------------------------

// CamouflageEnum32 --
enum CamouflageEnum32 {
    UNIFORM_PAINT_SCHEME = 0,
    DESERT_CAMOUFLAGE = 1,
    WINTER_CAMOUFLAGE = 2,
    FOREST_CAMOUFLAGE = 3,
    GENERIC_CAMOUFLAGE = 4,
};

// Compliance State -- Compliance state of a lifeform.
enum ComplianceStateEnum32 {
    COMPLIANCE_STATE_OTHER = 0,
    Detained = 1,
    Surrender = 2,
    UsingFists = 3,
    VerbalAbuse1 = 4,
    VerbalAbuse2 = 5,
    VerbalAbuse3 = 6,
    PassiveResistance1 = 7,
    PassiveResistance2 = 8,
    PassiveResistance3 = 9,
    NonLethalWeapon1 = 10,
    NonLethalWeapon2 = 11,
    NonLethalWeapon3 = 12,
    NonLethalWeapon4 = 13,
    NonLethalWeapon5 = 14,
    NonLethalWeapon6 = 15
};

// DamageStatusEnum32 --
enum DamageStatusEnum32 {
    NO_DAMAGE = 0,
    SLIGHT_DAMAGE = 1,
    MODERATE_DAMAGE = 2,
    DESTROYED = 3,
};

// The dead reckoning algorithm in use by the issuing object
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
    AirBurst = 25,
    Kill_with_fragment_type_1 = 26,
    Kill_with_fragment_type_2 = 27,
    Kill_with_fragment_type_3 = 28,
    Kill_with_fragment_type_1_after_fly_out_failure = 29,
    Kill_with_fragment_type_2_after_fly_out_failure = 30,
    Miss_due_to_fly_out_failure = 31,
    Miss_due_to_end_game_failure = 32,
    Miss_due_to_fly_out_and_end_game_failure = 33
};

// ForceIdentifierEnum8 --
enum ForceIdentifierEnum8 {
    OTHER = 0,
    FRIENDLY   = 1,
    OPPOSING   = 2,
    NEUTRAL    = 3,
    FRIENDLY_2 = 4,
    OPPOSING_2 = 5,
    NEUTRAL_2  = 6,
    FRIENDLY_3 = 7,
    OPPOSING_3 = 8,
    NEUTRAL_3  = 9,
    FRIENDLY_4 = 10,
    OPPOSING_4 = 11,
    NEUTRAL_4  = 12,
    FRIENDLY_5 = 13,
    OPPOSING_5 = 14,
    NEUTRAL_5  = 15,
    FRIENDLY_6 = 16,
    OPPOSING_6 = 17,
    NEUTRAL_6  = 18,
    FRIENDLY_7 = 19,
    OPPOSING_7 = 20,
    NEUTRAL_7  = 21,
    FRIENDLY_8 = 22,
    OPPOSING_8 = 23,
    NEUTRAL_8  = 24,
    FRIENDLY_9 = 25,
    OPPOSING_9 = 26,
    NEUTRAL_9  = 27,
    FRIENDLY_10 = 28,
    OPPOSING_10 = 29,
    NEUTRAL_10  = 30
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

// HatchStateEnum32 --
enum HatchStateEnum32 {
    HATCH_STATE_NOT_APPLICABLE = 0,
    PRIMARY_HATCH_IS_CLOSED = 1,
    PRIMARY_HATCH_IS_POPPED = 2,
    PRIMARY_HATCH_IS_POPPED_AND_PERSON_IS_VISIBLE_UNDER_HATCH = 3,
    PRIMARY_HATCH_IS_OPEN = 4,
    PRIMARY_HATCH_IS_OPEN_AND_PERSON_IS_VISIBLE = 5,
};

// MarkingEncodingEnum8 --
enum MarkingEncodingEnum8 {
    MARKING_OTHER = 0,
    ASCII = 1,
    ARMY_MARKING_CCTT = 2,
    DIGIT_CHEVRON = 3,
};

// Stance Code --
enum StanceCodeEnum32 {
   STANCE_CODE_NOT_APPLICABLE = 0,
   UPRIGHT_STANDING_STILL = 1,
   UPRIGHT_WALKING = 2,
   UPRIGHT_RUNNING = 3,
   KNEELING = 4,
   PRONE = 5,
   CRAWLING = 6,
   SWIMMING = 7,
   PARACHUTING = 8,
   JUMPING = 9,
   SITTING = 10,
   SQUATTING = 11,
   CROUCHING = 12,
   WADING = 13,
   SURRENDER = 14,
   DETAINED = 15
};

// TrailingEffectsCodeEnum32 --
enum TrailingEffectsCodeEnum32 {
    NO_TRAIL = 0,
    SMALL_TRAIL = 1,
    MEDIUM_TRAIL = 2,
    LARGE_TRAIL = 3,
};

// Weapon State --
enum WeaponStateEnum32 {
    NO_WEAPON = 0,
    STOWED = 1,
    DEPLOYED = 2,
    FIRING_POSITION = 3
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

//// Articulated Type --
//enum ArticulatedTypeEnum {
//    ARTICULATED_TYPE_OTHER = 41,
//    Rudder = 42,
//    LeftFlap = 43,
//    RightFlap = 44,
//    LeftAileron = 45,
//    RightAileron = 46,
//    HelicopterMainRotor = 47,
//    HelicopterTailRotor = 48,
//    OtherAircraftControlSurfaces = 49,
//    Periscope = 50,
//    GenericAntenna = 51,
//    Snorkel = 52,
//    OtherExtendableParts = 53,
//    LandingGear = 54,
//    TailHook = 55,
//    SpeedBrake = 56,
//    LeftWeaponBayDoors = 57,
//    RightWeaponBayDoors = 58,
//    TankOrAPChatch = 59,
//    Wingsweep = 60,
//    BridgeLauncher = 61,
//    BridgeSection1 = 62,
//    BridgeSection2 = 63,
//    BridgeSection3 = 64,
//    PrimaryBlade1 = 65,
//    PrimaryBlade2 = 66,
//    PrimaryBoom = 67,
//    PrimaryLauncherArm = 68,
//    OtherFixedPositionParts = 69,
//    PrimaryTurretNumber1 = 70,
//    PrimaryTurretNumber2 = 71,
//    PrimaryTurretNumber3 = 72,
//    PrimaryTurretNumber4 = 73,
//    PrimaryTurretNumber5 = 74,
//    PrimaryTurretNumber6 = 75,
//    PrimaryTurretNumber7 = 76,
//    PrimaryTurretNumber8 = 77,
//    PrimaryTurretNumber9 = 78,
//    PrimaryTurretNumber10 = 79,
//    PrimaryGunNumber1 = 80,
//    PrimaryGunNumber2 = 81,
//    PrimaryGunNumber3 = 82,
//    PrimaryGunNumber4 = 83,
//    PrimaryGunNumber5 = 84,
//    PrimaryGunNumber6 = 85,
//    PrimaryGunNumber7 = 86,
//    PrimaryGunNumber8 = 87,
//    PrimaryGunNumber9 = 88,
//    PrimaryGunNumber10 = 89,
//    PrimaryLauncher1 = 90,
//    PrimaryLauncher2 = 91,
//    PrimaryLauncher3 = 92,
//    PrimaryLauncher4 = 93,
//    PrimaryLauncher5 = 94,
//    PrimaryLauncher6 = 95,
//    PrimaryLauncher7 = 96,
//    PrimaryLauncher8 = 97,
//    PrimaryLauncher9 = 98,
//    PrimaryLauncher10 = 99,
//    PrimaryDefenseSystems1 = 100,
//    PrimaryDefenseSystems2 = 101,
//    PrimaryDefenseSystems3 = 102,
//    PrimaryDefenseSystems4 = 103,
//    PrimaryDefenseSystems5 = 104,
//    PrimaryDefenseSystems6 = 105,
//    PrimaryDefenseSystems7 = 106,
//    PrimaryDefenseSystems8 = 107,
//    PrimaryDefenseSystems9 = 108,
//    PrimaryDefenseSystems10 = 109,
//    PrimaryRadar1 = 110,
//    PrimaryRadar2 = 111,
//    PrimaryRadar3 = 112,
//    PrimaryRadar4 = 113,
//    PrimaryRadar5 = 114,
//    PrimaryRadar6 = 115,
//    PrimaryRadar7 = 116,
//    PrimaryRadar8 = 117,
//    PrimaryRadar9 = 118,
//    PrimaryRadar10 = 119,
//    SecondaryTurretNumber1 = 120,
//    SecondaryTurretNumber2 = 121,
//    SecondaryTurretNumber3 = 122,
//    SecondaryTurretNumber4 = 123,
//    SecondaryTurretNumber5 = 124,
//    SecondaryTurretNumber6 = 125,
//    SecondaryTurretNumber7 = 126,
//    SecondaryTurretNumber8 = 127,
//    SecondaryTurretNumber9 = 128,
//    SecondaryTurretNumber10 = 129,
//    SecondaryGunNumber1 = 130,
//    SecondaryGunNumber2 = 131,
//    SecondaryGunNumber3 = 132,
//    SecondaryGunNumber4 = 133,
//    SecondaryGunNumber5 = 134,
//    SecondaryGunNumber6 = 135,
//    SecondaryGunNumber7 = 136,
//    SecondaryGunNumber8 = 137,
//    SecondaryGunNumber9 = 138,
//    SecondaryGunNumber10 = 139,
//    SecondaryLauncher1 = 140,
//    SecondaryLauncher2 = 141,
//    SecondaryLauncher3 = 142,
//    SecondaryLauncher4 = 143,
//    SecondaryLauncher5 = 144,
//    SecondaryLauncher6 = 145,
//    SecondaryLauncher7 = 146,
//    SecondaryLauncher8 = 147,
//    SecondaryLauncher9 = 148,
//    SecondaryLauncher10 = 149,
//    SecondaryDefenseSystems1 = 150,
//    SecondaryDefenseSystems2 = 151,
//    SecondaryDefenseSystems3 = 152,
//    SecondaryDefenseSystems4 = 153,
//    SecondaryDefenseSystems5 = 154,
//    SecondaryDefenseSystems6 = 155,
//    SecondaryDefenseSystems7 = 156,
//    SecondaryDefenseSystems8 = 157,
//    SecondaryDefenseSystems9 = 158,
//    SecondaryDefenseSystems10 = 159,
//    SecondaryRadar1 = 160,
//    SecondaryRadar2 = 161,
//    SecondaryRadar3 = 162,
//    SecondaryRadar4 = 163,
//    SecondaryRadar5 = 164,
//    SecondaryRadar6 = 165,
//    SecondaryRadar7 = 166,
//    SecondaryRadar8 = 167,
//    SecondaryRadar9 = 168,
//    SecondaryRadar10 = 169
//};


//------------------------------------------------------------------------------
// RPR-ROM: Complex Data Structures
//------------------------------------------------------------------------------

// RTIObjectIdStruct
struct RTIObjectIdStruct {
    static const int ID_SIZE = 64;
    RTIObjectIdStruct() { for (int i = 0; i < ID_SIZE; i++) { id[i] = '\0';} }
    unsigned char id[ID_SIZE];
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

// EventIdentifierStruct
struct EventIdentifierStruct {
    EventIdentifierStruct() { eventCount = 0; issuingObjectIdentifier.id[0] = '\0'; }
    RTI::UShort eventCount;
    RTIObjectIdStruct issuingObjectIdentifier;
};

// Acceleration (meters/second/second)
struct AccelerationVectorStruct {
    AccelerationVectorStruct()  { xAcceleration = 0.0; yAcceleration = 0.0; zAcceleration = 0.0; }
    RTI::Float xAcceleration;
    RTI::Float yAcceleration;
    RTI::Float zAcceleration;
};

// Angular Velocity (radians/second)
struct AngularVelocityVectorStruct {
    AngularVelocityVectorStruct() { xAngularVelocity = 0.0; yAngularVelocity = 0.0; zAngularVelocity = 0.0; }
    RTI::Float  xAngularVelocity;
    RTI::Float  yAngularVelocity;
    RTI::Float  zAngularVelocity;
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

// Orientation (radians)
struct OrientationStruct {
    OrientationStruct() { phi = 0.0; psi = 0.0; theta = 0.0; }
    RTI::Float  psi;
    RTI::Float  theta;
    RTI::Float  phi;
};

// Relative Position Struct (meters)
struct RelativePositionStruct {
    RelativePositionStruct() { bodyXDistance = 0.0; bodyYDistance = 0.0; bodyZDistance = 0.0; }
    RTI::Float bodyXDistance;
    RTI::Float bodyYDistance;
    RTI::Float bodyZDistance;
};

// Velocity (meters/second)
struct VelocityVectorStruct {
    VelocityVectorStruct()      { xVelocity = 0.0; yVelocity = 0.0; zVelocity = 0.0; }
    RTI::Float xVelocity;
    RTI::Float yVelocity;
    RTI::Float zVelocity;
};

// World Location (meters)
struct WorldLocationStruct {
    WorldLocationStruct()       { x = 0.0; y = 0.0; z = 0.0; }
    RTI::Double x;
    RTI::Double y;
    RTI::Double z;
};

// Spatial structure for Dead Reckoning Algorithm Static (1)
struct SpatialStaticStruct {
   WorldLocationStruct  worldLocation; // Location of the entity.
   RTI::Boolean         isFrozen;      // Freeze Motion
   unsigned char        padding[3];
   OrientationStruct    orientation;   // Entity's Euler Angles
};

// Spatial structure for Dead Reckoning Algorithm FPW (2) and FPB (6)
struct SpatialFPStruct {
   WorldLocationStruct  worldLocation;    // Location of the entity.
   RTI::Boolean         isFrozen;         // Freeze Motion
   unsigned char        padding[3];
   OrientationStruct    orientation;      // Entity's Euler Angles
   VelocityVectorStruct velocityVector;   // Velocity of the entity.
};

// Spatial structure for Dead Reckoning Algorithm RPW (3) and RPB (7)
struct SpatialRPStruct {
   WorldLocationStruct  worldLocation;    // Location of the entity.
   RTI::Boolean         isFrozen;         // Freeze Motion
   unsigned char        padding[3];
   OrientationStruct    orientation;      // Entity's Euler Angles
   VelocityVectorStruct velocityVector;   // Velocity of the entity.
   AngularVelocityVectorStruct angularVelocity; // The rate at which an entity's orientation is changing over time.
};

// Spatial structure for Dead Reckoning Algorithm RVW (4) and RVB (8)
struct SpatialRVStruct {
   WorldLocationStruct  worldLocation;    // Location of the entity.
   RTI::Boolean         isFrozen;         // Freeze Motion
   unsigned char        padding[3];
   OrientationStruct    orientation;      // Entity's Euler Angles
   VelocityVectorStruct velocityVector;   // Velocity of the entity.
   AccelerationVectorStruct accelerationVector; // Acceleration of the entity.
   AngularVelocityVectorStruct angularVelocity; // The rate at which an entity's orientation is changing over time.
};

// Spatial structure for Dead Reckoning Algorithm FVW (5) and RVB (9)
struct SpatialFVStruct {
   WorldLocationStruct  worldLocation;    // Location of the entity.
   RTI::Boolean         isFrozen;         // Freeze Motion
   unsigned char        padding[3];
   OrientationStruct    orientation;      // Entity's Euler Angles
   VelocityVectorStruct velocityVector;   // Velocity of the entity.
   AccelerationVectorStruct accelerationVector; // Acceleration of the entity.
};

// Variant Record format for a single spatial attribute
struct SpatialStruct {
   unsigned char          deadReckoningAlgorithm;  // DeadReckoningAlgorithmEnum8
   unsigned char          padding[7];
   //SpatialStaticStruct  spatialStatic; // (DeadReckoningAlgorithm = Static)
   //SpatialFPStruct      spatialFPW;    // (DeadReckoningAlgorithm = DRM_FPW)
   //SpatialRPStruct      spatialRPW;    // (DeadReckoningAlgorithm = DRM_RPW)
   SpatialRVStruct        spatialRVW;    // (DeadReckoningAlgorithm = DRM_RVW)
   //SpatialFVStruct      spatialFVW;    // (DeadReckoningAlgorithm = DRM_FVW)
   //SpatialFPStruct      spatialFPB;    // (DeadReckoningAlgorithm = DRM_FPB)
   //SpatialRPStruct      spatialRPB;    // (DeadReckoningAlgorithm = DRM_RPB)
   //SpatialRVStruct      spatialRVB;    // (DeadReckoningAlgorithm = DRM_RVB)
   //SpatialFVStruct      spatialFVB;    // (DeadReckoningAlgorithm = DRM_FVB)
};

// Markings
struct MarkingStruct {
    MarkingStruct() { markingEncodingType = ASCII; for (int i = 0; i < 11; markingData[i++] = '\0'); }
    MarkingEncodingEnum8    markingEncodingType;
    unsigned char           markingData[11];
};

// Propulsion System Data
struct PropulsionSystemDataStruct {
   PropulsionSystemDataStruct() { powerSetting = 0; engineRPM = 0; }
   RTI::Float  powerSetting; // The power setting of the propulsion system, after any response lags have been incorporated
   RTI::Float  engineRPM;    // The current engine speed
};

// Vectoring Nozzle System Data
struct VectoringNozzleSystemDataStruct {
   VectoringNozzleSystemDataStruct() { horizontalDeflectionAngle = 0; verticalDeflectionAngle = 0; }
   RTI::Float  horizontalDeflectionAngle; // (degrees) The nozzle deflection angle in the horizontal body axis of the entity
   RTI::Float  verticalDeflectionAngle;   // (degrees) The nozzle deflection angle in the vertical body axis of the entity
};

//// Articulated Parameter
//struct ArticulatedParameterStruct {
//   ArticulatedParameterStruct() : parameterValue() {
//       partAttachedTo = 0; articulatedParameterChange = 0;
//       articulatedParameterType = ARTICULATED_TYPE_OTHER;
//       parameterType = PARAMETER_TYPE_DUMMY;
//   }
//   short               partAttachedTo;
//   unsigned char       articulatedParameterChange;
//   ArticulatedTypeEnum articulatedParameterType;
//   ParameterValueType  parameterValue;
//   ParameterTypeEnum   parameterType;
//};


//==============================================================================
// RPR-ROM: Object Class Structures
//==============================================================================

// ---
// Base Entity Structure (S)
//
// "A base class of aggregate and discrete scenario domain participants. The
// BaseEntity class is characterized by being located at a particular location
// in space and independently movable, if capable of movement at all. It
// specifically excludes elements normally considered to be a component of
// another element. The BaseEntity class is intended to be a container for
// common attributes for entities of this type.  Since it lacks sufficient
// class specific attributes that are required for simulation purposes it is
// not expected that any federate shall publish objects of this class.  Certain
// simulation management federates, e.g. viewers, may subscribe to this class.
// Simulation federates will normally subscribe to one of the subclasses, to
// gain the extra information required to properly simulate the entity. (See
// section 6.1 of the GRIM)")
//
// ---

class BaseEntity : public base::Object {
   DECLARE_SUBCLASS(BaseEntity, base::Object)
public:
   BaseEntity();

   static const char* getClassFedName()                     { return "BaseEntity"; }

   static const char* getEntityTypeAttributeFedName()       { return "EntityType"; }
   static const char* getEntityIdentifierAttributeFedName() { return "EntityIdentifier"; }
   //static const char* getIsPartOfAttributeFedName()         { return "IsPartOf"; }
   static const char* getSpatialAttributeFedName()          { return "Spatial"; }
   static const char* getRelativeSpatialAttributeFedName()  { return "RelativeSpatial"; }

   EntityTypeStruct        entityType;       // The category of the entity.
   EntityIdentifierStruct  entityIdentifier; // The unique identifier for the entity instance.
   //IsPartOfStruct        isPartOf;         // << Not supported yet >>
   SpatialStruct           spatial;          // Spatial state stored in one variant record attribute
   SpatialRVStruct         spatialRvw;       // Spatial state for DeadReckoningAlgorithm = DRM_RVW
   SpatialStruct           relativeSpatial;  // Relative spatial state stored in one variant record attribute
   SpatialRVStruct         relativeSpatialRvw; // Relative spatial state for DeadReckoningAlgorithm = DRM_RVW
};


// ---
// Physical Entity Structure (S)
//  -- subclass of BaseEntity
//
// A base class of all discrete platform scenario domain participant
// ---
class PhysicalEntity : public BaseEntity {
    DECLARE_SUBCLASS(PhysicalEntity, BaseEntity)
public:
    PhysicalEntity();

    static const char* getClassFedName()                                     { return "BaseEntity.PhysicalEntity"; }

    static const char* getAcousticSignatureIndexAttributeFedName()           { return "AcousticSignatureIndex"; }
    static const char* getAlternateEntityTypeAttributeFedName()              { return "AlternateEntityType"; }
    //static const char* getArticulatedParametersArrayAttributeFedName() { return "ArticulatedParametersArray"; }
    static const char* getCamouflageTypeAttributeFedName()                   { return "CamouflageType"; }
    static const char* getDamageStateAttributeFedName()                      { return "DamageState"; }
    static const char* getEngineSmokeOnAttributeFedName()                    { return "EngineSmokeOn"; }
    static const char* getFirePowerDisabledAttributeFedName()                { return "FirePowerDisabled"; }
    static const char* getFlamesPresentAttributeFedName()                    { return "FlamesPresent"; }
    static const char* getForceIdentifierAttributeFedName()                  { return "ForceIdentifier"; }
    static const char* getHasAmmunitionSupplyCapAttributeFedName()           { return "HasAmmunitionSupplyCap"; }
    static const char* getHasFuelSupplyCapAttributeFedName()                 { return "HasFuelSupplyCap"; }
    static const char* getHasRecoveryCapAttributeFedName()                   { return "HasRecoveryCap"; }
    static const char* getHasRepairCapAttributeFedName()                     { return "HasRepairCap"; }
    static const char* getImmobilizedAttributeFedName()                      { return "Immobilized"; }
    static const char* getInfraredSignatureIndexAttributeFedName()           { return "InfraredSignatureIndex"; }
    static const char* getIsConcealedAttributeFedName()                      { return "IsConcealed"; }
    static const char* getLiveEntityMeasuredSpeedAttributeFedName()          { return "LiveEntityMeasuredSpeed"; }
    static const char* getMarkingAttributeFedName()                          { return "Marking"; }
    static const char* getPowerPlantOnAttributeFedName()                     { return "PowerPlantOn"; }
    static const char* getPropulsionSystemsDataAttributeFedName()            { return "PropulsionSystemsData"; }
    static const char* getRadarCrossSectionSignatureIndexAttributeFedName()  { return "RadarCrossSectionSignatureIndex"; }
    static const char* getSmokePlumePresentAttributeFedName()                { return "SmokePlumePresent"; }
    static const char* getTentDeployedAttributeFedName()                     { return "TentDeployed"; }
    static const char* getTrailingEffectsCodeAttributeFedName()              { return "TrailingEffectsCode"; }
    static const char* getVectoringNozzleSystemDataAttributeFedName()        { return "VectoringNozzleSystemData"; }

   RTI::Short   acousticSignatureIndex;                       // Index used to obtain the acoustics (sound through air) signature state of the entity
   EntityTypeStruct alternateEntityType;                      // The category of entity to be used when viewed by entities on the "opposite" side.
   //ArticulatedParameterStruct ArticulatedParametersArray; // << not supported yet >>
   CamouflageEnum32     camouflageType;                       // The type of camouflage in use (if any).
   DamageStatusEnum32   damageState;                          // The state of damage of the entity.
   RTI::Boolean         engineSmokeOn;                        // Whether the entity's engine is generating smoke or not.
   RTI::Boolean         firePowerDisabled;                    // Whether the entity's main weapon system has been disabled or not.
   RTI::Boolean         flamesPresent;                        // Whether the entity is on fire (with visible flames) or not.
   ForceIdentifierEnum8 forceIdentifier;                      // The identification of the force that the entity belongs to.
   RTI::Boolean         hasAmmunitionSupplyCap;               // Whether the entity has the capability to supply other entities with ammunition.
   RTI::Boolean         hasFuelSupplyCap;                     // Whether the entity has the capability to supply other entities with fuel or not.
   RTI::Boolean         hasRecoveryCap;                       // Whether the entity has the capability to recover other entities or not.
   RTI::Boolean         hasRepairCap;                         // Whether the entity has the capability to repair other entities or not.
   RTI::Boolean         immobilized;                          // Whether the entity is immobilized or not.
   RTI::Short           infraredSignatureIndex;               // Index used to obtain the infra-red signature state of the entity
   RTI::Boolean         isConcealed;                          // Whether the entity is concealed or not.
   RTI::UShort          liveEntityMeasuredSpeed;              // The entity's own measurement of speed (e.g. air speed for aircraft)
   MarkingStruct        marking;                              // A unique marking or combination of characters used to distinguish the entity from other entities.
   RTI::Boolean         powerPlantOn;                         // Whether the entity's power plant is on or not.
   PropulsionSystemDataStruct propulsionSystemsData;          // The basic operating data of the propulsion systems aboard the entity
   RTI::Short radarCrossSectionSignatureIndex;                // Index used to obtain the radar cross section signature state of the entity
   RTI::Boolean         smokePlumePresent;                    // Whether the entity is generating smoke or not (intentional or unintentional).
   RTI::Boolean         tentDeployed;                         // Whether the entity has deployed tent or not.
   TrailingEffectsCodeEnum32   trailingEffectsCode;           // The type and size of any trail that the entity is making.
   VectoringNozzleSystemDataStruct vectoringNozzleSystemData; // The type and size of any trail that the entity is making.
};


// ---
// Lifeform Structure (S)
//  -- subclass of PhysicalEntity
//
// "A living military platform (human or not)"
// ---
class Lifeform : public PhysicalEntity {
    DECLARE_SUBCLASS(Lifeform, PhysicalEntity)
public:
    Lifeform();

    static const char* getClassFedName()                          { return "BaseEntity.PhysicalEntity.Lifeform"; }

    static const char* getFlashLightsOnAttributeFedName()         { return "FlashLightsOn"; }
    static const char* getStanceCodeAttributeFedName()            { return "StanceCode"; }
    static const char* getPrimaryWeaponStateAttributeFedName()    { return "PrimaryWeaponState"; }
    static const char* getSecondaryWeaponStateAttributeFedName()  { return "SecondaryWeaponState"; }
    static const char* getComplianceStateAttributeFedName()       { return "ComplianceState"; }

   RTI::Boolean         flashLightsOn;       // Whether the lifeform's flash lights are on or not.
   StanceCodeEnum32     stanceCode;          // The stance of the lifeform.
   WeaponStateEnum32    primaryWeaponState;  // The state of the soldier\'s primary weapon system.
   WeaponStateEnum32    secondaryWeaponState; // The state of the soldier\'s secondary weapon system.
   ComplianceStateEnum32 complianceState;    // The compliance of the lifeform.

};

// ---
// Human Structure (PS)
//  -- subclass of Lifeform
//
// "A human lifeform"
// ---
class Human : public Lifeform {
    DECLARE_SUBCLASS(Human, Lifeform)
public:
    Human();
    static const char* getClassFedName()      { return "BaseEntity.PhysicalEntity.Lifeform.Human"; }
};


// ---
// NonHuman Structure (PS)
//  -- subclass of Lifeform
//
// "An animal or other non-human lifeform"
// ---
class NonHuman : public Lifeform {
    DECLARE_SUBCLASS(NonHuman, Lifeform)
public:
    NonHuman();
    static const char* getClassFedName()      { return "BaseEntity.PhysicalEntity.Lifeform.NonHuman"; }
};


// ---
// Munition Structure (PS)
//  -- subclass of PhysicalEntity
//
// "A complete device charged with explosives, propellants, pyrotechnics, initiating composition,
//  or nuclear, biological or chemical material for use in military operations, including demolitions."
// ---
class Munition : public PhysicalEntity {
   DECLARE_SUBCLASS(Munition, PhysicalEntity)
public:
   Munition();
   static const char* getClassFedName() { return "BaseEntity.PhysicalEntity.Munition"; }

    static const char* getLauncherFlashPresentAttributeFedName()    { return "LauncherFlashPresent"; }

   RTI::Boolean launcherFlashPresent;  // Whether the flash of the munition being launched is present or not.
};


// ---
// Platform Structure (S)
//  -- subclass of PhysicalEntity
//
// "A physical object under the control of armed forces upon which sensor, communication,
//  or weapon systems may be mounted."
// ---
class Platform : public PhysicalEntity {
   DECLARE_SUBCLASS(Platform, PhysicalEntity)
public:
   Platform();
   static const char* getClassFedName()                           { return "BaseEntity.PhysicalEntity.Platform"; }

   static const char* getAfterburnerOnAttributeFedName()          { return "AfterburnerOn"; }
   static const char* getAntiCollisionLightsOnAttributeFedName()  { return "AntiCollisionLightsOn"; }
   static const char* getBlackOutBrakeLightsOnAttributeFedName()  { return "BlackOutBrakeLightsOn"; }
   static const char* getBlackOutLightsOnAttributeFedName()       { return "BlackOutLightsOn"; }
   static const char* getBrakeLightsOnAttributeFedName()          { return "BrakeLightsOn"; }
   static const char* getFormationLightsOnAttributeFedName()      { return "FormationLightsOn"; }
   static const char* getHatchStateAttributeFedName()             { return "HatchState"; }
   static const char* getHeadLightsOnAttributeFedName()           { return "HeadLightsOn"; }
   static const char* getInteriorLightsOnAttributeFedName()       { return "InteriorLightsOn"; }
   static const char* getLandingLightsOnAttributeFedName()        { return "LandingLightsOn"; }
   static const char* getLauncherRaisedAttributeFedName()         { return "LauncherRaised"; }
   static const char* getNavigationLightsOnAttributeFedName()     { return "NavigationLightsOn"; }
   static const char* getRampDeployedAttributeFedName()           { return "RampDeployed"; }
   static const char* getRunningLightsOnAttributeFedName()        { return "RunningLightsOn"; }
   static const char* getSpotLightsOnAttributeFedName()           { return "SpotLightsOn"; }
   static const char* getTailLightsOnAttributeFedName()           { return "TailLightsOn"; }

   RTI::Boolean      afterburnerOn;          // Whether the entity's afterburner is on or not.
   RTI::Boolean      antiCollisionLightsOn;  // Whether the entity's anti-collision lights are on or not.
   RTI::Boolean      blackOutBrakeLightsOn;  // Whether the entity's black out brake lights are on or not.
   RTI::Boolean      blackOutLightsOn;       // Whether the entity's black out lights are on or not.
   RTI::Boolean      brakeLightsOn;          // Whether the entity's brake lights are on or not.
   RTI::Boolean      formationLightsOn;      // Whether the entity's formation lights are on or not.
   HatchStateEnum32  hatchState;             // The state of the entity's (main) hatch.
   RTI::Boolean      headLightsOn;           // Whether the entity's headlights are on or not.
   RTI::Boolean      interiorLightsOn;       // Whether the entity's internal lights are on or not.
   RTI::Boolean      landingLightsOn;        // Whether the entity's landing lights are on or not.
   RTI::Boolean      launcherRaised;         // Whether the entity's weapon launcher is in the raised position.
   RTI::Boolean      navigationLightsOn;     // Whether the entity's navigation lights are on or not.
   RTI::Boolean      rampDeployed;           // Whether the entity has deployed a ramp or not.
   RTI::Boolean      runningLightsOn;        // Whether the entity's running lights are on or not.
   RTI::Boolean      spotLightsOn;           // Whether the entity's spotlights are on or not.
   RTI::Boolean      tailLightsOn;           // Whether the entity's tail lights are on or not.
};


// ---
// Aircraft Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that operates mainly in the air, such as aircraft,
//  balloons, etc.  This includes the entities when they are on the ground."
// ---
class Aircraft : public Platform {
    DECLARE_SUBCLASS(Aircraft, Platform)
public:
    Aircraft();
   static const char* getClassFedName()       { return "BaseEntity.PhysicalEntity.Platform.Aircraft"; }
};


// ---
// Amphibious Vehicle Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that can operate both on the land and the sea."
// ---
class AmphibiousVehicle : public Platform {
    DECLARE_SUBCLASS(AmphibiousVehicle, Platform)
public:
    AmphibiousVehicle();
   static const char* getClassFedName()      { return "BaseEntity.PhysicalEntity.Platform.AmphibiousVehicle"; }
};


// ---
// Ground Vehicle Structure (S)
//  -- subclass of Platform
//
// "A platform entity that operates wholly on the surface of the earth."
// ---
class GroundVehicle : public Platform {
    DECLARE_SUBCLASS(GroundVehicle, Platform)
public:
    GroundVehicle();
   static const char* getClassFedName() { return "BaseEntity.PhysicalEntity.Platform.GroundVehicle"; }
};


// ---
// MultiDomain Platform Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that operates in more than one domain (excluding those
//  combinations explicitly defined as subclasses of the superclass of this class)."
// ---
class MultiDomainPlatform : public Platform {
    DECLARE_SUBCLASS(MultiDomainPlatform, Platform)
public:
    MultiDomainPlatform();
   static const char* getClassFedName() { return "BaseEntity.PhysicalEntity.Platform.MultiDomainPlatform"; }
};


// ---
// Spacecraft Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that operates mainly in space."
// ---
class Spacecraft : public Platform {
    DECLARE_SUBCLASS(Spacecraft, Platform)
public:
    Spacecraft();
   static const char* getClassFedName() { return "BaseEntity.PhysicalEntity.Platform.Spacecraft"; }
};


// ---
// Submersible Vessel Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that operates either on the surface of the sea, or beneath it."
// ---
class SubmersibleVessel : public Platform {
    DECLARE_SUBCLASS(SubmersibleVessel, Platform)
public:
    SubmersibleVessel();
   static const char* getClassFedName() { return "BaseEntity.PhysicalEntity.Platform.SubmersibleVessel"; }
};


// ---
// Surface Vessel Structure (PS)
//  -- subclass of Platform
//
// "A platform entity that operates wholly on the surface of the sea."
// ---
class SurfaceVessel : public Platform {
    DECLARE_SUBCLASS(SurfaceVessel, Platform)
public:
    SurfaceVessel();
   static const char* getClassFedName() { return "BaseEntity.PhysicalEntity.Platform.SurfaceVessel"; }
};


//==============================================================================
// RPR-ROM: Interaction Structures
//==============================================================================

// ---
// Weapon Fire (IR)
//
// "Communicates information associated with the firing or launch of a munition."
// ---
struct WeaponFire {
    static const char* getInteractionFedName()                          { return "WeaponFire"; }

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

   EventIdentifierStruct eventIdentifier; // An ID, generated by the issuing federate, used to associated
                                          // related fire and detonation events.
   RTI::Float fireControlSolutionRange;   // (meters) The range used in the fire control solution. Zero
                                          // if the range is unknown or inapplicable.
   RTI::ULong fireMissionIndex;           // A unique index to identify the fire mission (used to associated
                                          // weapon fire interactions in a single fire mission).
   WorldLocationStruct firingLocation;    // The location, the world coordinate system, of the weapon fire.
   RTIObjectIdStruct firingObjectIdentifier;  // The ID of the object firing the munition.
   FuseTypeEnum16    fuseType;            // The type of fuse on the munition.
   VelocityVectorStruct initialVelocityVector; // The velocity vector of the munition when fired.
   RTIObjectIdStruct munitionObjectIdentifier;   // he ID of the associated munition object (if any).
   EntityTypeStruct  munitionType;        // The type of munition being fired.
   RTI::UShort       quantityFired;       // The number of rounds fired in the fire event.
   RTI::UShort       rateOfFire;          // The rate of fire at which the munitions in the burst described in the fire event.
   RTIObjectIdStruct targetObjectIdentifier; // The ID of the object being fired at (if any).
   WarheadTypeEnum16 warheadType;         // The type of warhead fitted to the munition being fired.
};


// ---
// Munition Detonation (IR)
//
// "Communicates information associated with the impact or detonation of a munition"
// ---
struct MunitionDetonation {
   static const char* getInteractionFedName()                         { return "MunitionDetonation"; }

   //static const char* getArticulatedPartDataParameterFedName()      { return "ArticulatedPartData"; }
   static const char* getDetonationLocationParameterFedName()         { return "DetonationLocation"; }
   static const char* getDetonationResultCodeParameterFedName()       { return "DetonationResultCode"; }
   static const char* getEventIdentifierParameterFedName()            { return "EventIdentifier"; }
   static const char* getFiringObjectIdentifierParameterFedName()     { return "FiringObjectIdentifier"; }
   static const char* getFinalVelocityVectorParameterFedName()        { return "FinalVelocityVector"; }
   static const char* getFuseTypeParameterFedName()                   { return "FuseType"; }
   static const char* getMunitionObjectIdentifierParameterFedName()   { return "MunitionObjectIdentifier"; }
   static const char* getMunitionTypeParameterFedName()               { return "MunitionType"; }
   static const char* getQuantityFiredParameterFedName()              { return "QuantityFired"; }
   static const char* getRateOfFireParameterFedName()                 { return "RateOfFire"; }
   static const char* getRelativeDetonationLocationParameterFedName() { return "RelativeDetonationLocation"; }
   static const char* getTargetObjectIdentifierParameterFedName()     { return "TargetObjectIdentifier"; }
   static const char* getWarheadTypeParameterFedName()                { return "WarheadType"; }

   //ArticulatedParameterStruct articulatedPartData; // The set of articulated parts affected by the detonation (including
   WorldLocationStruct detonationLocation;      // The location, in the world coordinate system, of the detonation.
   DetonationResultCodeEnum8 detonationResultCode; // The type of detonation (including no detonation).
   EventIdentifierStruct eventIdentifier;       // An ID, generated by the issuing federate, used to associated related fire and detonation events.
   RTIObjectIdStruct firingObjectIdentifier;    // The ID of the object firing the munition.
   VelocityVectorStruct finalVelocityVector;    // The velocity vector of the munition at the moment of the detonation.
   FuseTypeEnum16    fuseType;                  // The type of fuse on the munition.
   RTIObjectIdStruct munitionObjectIdentifier;  // The ID of the associated munition object (if any).
   EntityTypeStruct  munitionType;              // The type of munition that is detonating.
   RTI::UShort       quantityFired;             // The quantity of rounds fired in a burst.
   RTI::UShort       rateOfFire;                // The rate of fire, in rounds per minute, of the munitions in the burst.
   RelativePositionStruct relativeDetonationLocation; // The location of the detonation, relative to the target object (if any).
   RTIObjectIdStruct targetObjectIdentifier;    // The ID of the object that the munition has detonated on.
   WarheadTypeEnum16 warheadType;               // The type of warhead on the munition.
};

}
}

#endif
