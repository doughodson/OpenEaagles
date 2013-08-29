
#include "openeaagles/hla/mc02Fom/Mc02Fom.h"
 
namespace Eaagles {
namespace Network {
namespace Hla {
namespace Mc02Fom {

//==============================================================================
// BaseEntity Structure (S)
//==============================================================================

IMPLEMENT_SUBCLASS(BaseEntity,"BaseEntity")
EMPTY_SLOTTABLE(BaseEntity)
EMPTY_SERIALIZER(BaseEntity)

BaseEntity::BaseEntity() : accelerationVector(), angularVelocityVector(), deadReckoningAlgorithm(DRM_RVW),
                entityType(), entityIdentifier(), isFrozen(RTI::RTI_FALSE), 
                orientation(), worldLocation(), velocityVector()
{
}

void BaseEntity::copyData(const BaseEntity& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(BaseEntity)

//==============================================================================
// PhysicalEntity Structure (S)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(PhysicalEntity,"PhysicalEntity")
EMPTY_SERIALIZER(PhysicalEntity)

PhysicalEntity::PhysicalEntity() :  BaseEntity(),
                    alternateEntityType(), camouflageType(UNIFORM_PAINT_SCHEME), damageState(NO_DAMAGE),
                    engineSmokeOn(RTI::RTI_FALSE), firePowerDisabled(RTI::RTI_FALSE), flamesPresent(RTI::RTI_FALSE), 
                    forceIdentifier(OTHER), hasAmmunitionSupplyCap(RTI::RTI_FALSE), hasFuelSupplyCap(RTI::RTI_FALSE),
                    hasRecoveryCap(RTI::RTI_FALSE), hasRepairCap(RTI::RTI_FALSE), immobilized(RTI::RTI_FALSE),
                    isConcealed(RTI::RTI_FALSE), marking(), powerPlantOn(RTI::RTI_FALSE),
                    smokePlumePresent(RTI::RTI_FALSE), tentDeployed(RTI::RTI_FALSE), trailingEffectsCode(NO_TRAIL)
{
}

void PhysicalEntity::copyData(const PhysicalEntity& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(PhysicalEntity)
    

//==============================================================================
// Munition Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Munition,"Munition")
EMPTY_SERIALIZER(Munition)

Munition::Munition() :  PhysicalEntity(), 
                launcherFlashPresent(RTI::RTI_FALSE)
{
}

void Munition::copyData(const Munition& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(Munition)

//==============================================================================
// Platform Structure (S)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Platform,"Platform")
EMPTY_SERIALIZER(Platform)

Platform::Platform() :  PhysicalEntity(), 
                afterburnerOn(RTI::RTI_FALSE), antiCollisionLightsOn(RTI::RTI_FALSE), blackOutBrakeLightsOn(RTI::RTI_FALSE),
                blackOutLightsOn(RTI::RTI_FALSE), brakeLightsOn(RTI::RTI_FALSE), formationLightsOn(RTI::RTI_FALSE), 
                hatchState(NOT_APPLICABLE), headLightsOn(RTI::RTI_FALSE), interiorLightsOn(RTI::RTI_FALSE),
                landingLightsOn(RTI::RTI_FALSE), launcherRaised(RTI::RTI_FALSE), navigationLightsOn(RTI::RTI_FALSE),
                rampDeployed(RTI::RTI_FALSE), runningLightsOn(RTI::RTI_FALSE), spotLightsOn(RTI::RTI_FALSE),
                tailLightsOn(RTI::RTI_FALSE)
{
}

void Platform::copyData(const Platform& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(Platform)

//==============================================================================
// Missile Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Missile,"Missile")
EMPTY_SERIALIZER(Missile)

Missile::Missile() :  Platform() {}

EMPTY_COPYDATA(Missile)
EMPTY_DELETEDATA(Missile)

//==============================================================================
// Aircraft Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Aircraft,"Aircraft")
EMPTY_SERIALIZER(Aircraft)

Aircraft::Aircraft() :  Platform() {}

EMPTY_COPYDATA(Aircraft)
EMPTY_DELETEDATA(Aircraft)
  
//==============================================================================
// AmphibiousVehicle Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(AmphibiousVehicle,"AmphibiousVehicle")
EMPTY_SERIALIZER(AmphibiousVehicle)

AmphibiousVehicle::AmphibiousVehicle() :  Platform() {}

EMPTY_COPYDATA(AmphibiousVehicle)
EMPTY_DELETEDATA(AmphibiousVehicle)
  
//==============================================================================
// GroundVehicle Structure (S)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(GroundVehicle,"GroundVehicle")
EMPTY_SERIALIZER(GroundVehicle)

GroundVehicle::GroundVehicle() :  Platform() {}

EMPTY_COPYDATA(GroundVehicle)
EMPTY_DELETEDATA(GroundVehicle)
  
//==============================================================================
// MultiDomainPlatform Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MultiDomainPlatform,"MultiDomainPlatform")
EMPTY_SERIALIZER(MultiDomainPlatform)

MultiDomainPlatform::MultiDomainPlatform() :  Platform() {}

EMPTY_COPYDATA(MultiDomainPlatform)
EMPTY_DELETEDATA(MultiDomainPlatform)
  
//==============================================================================
// Spacecraft Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Spacecraft,"Spacecraft")
EMPTY_SERIALIZER(Spacecraft)

Spacecraft::Spacecraft() :  Platform() {}

EMPTY_COPYDATA(Spacecraft)
EMPTY_DELETEDATA(Spacecraft)
  
//==============================================================================
// SubmersibleVessel Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SubmersibleVessel,"SubmersibleVessel")
EMPTY_SERIALIZER(SubmersibleVessel)

SubmersibleVessel::SubmersibleVessel() :  Platform() {}

EMPTY_COPYDATA(SubmersibleVessel)
EMPTY_DELETEDATA(SubmersibleVessel)

//==============================================================================
// SurfaceVessel Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SurfaceVessel,"SurfaceVessel")
EMPTY_SERIALIZER(SurfaceVessel)

SurfaceVessel::SurfaceVessel() :  Platform() {}

EMPTY_COPYDATA(SurfaceVessel)
EMPTY_DELETEDATA(SurfaceVessel)
  
//==============================================================================
// Default Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Default,"Default")
EMPTY_SERIALIZER(Default)

Default::Default() :  GroundVehicle() {}

EMPTY_COPYDATA(Default)
EMPTY_DELETEDATA(Default)
  
//==============================================================================
// Command_Center Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(CommandCenter,"CommandCenter")
EMPTY_SERIALIZER(CommandCenter)

CommandCenter::CommandCenter() :  GroundVehicle() {}

EMPTY_COPYDATA(CommandCenter)
EMPTY_DELETEDATA(CommandCenter)
  
//==============================================================================
// SAM_Commander Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SAM_Commander,"SAM_Commander")
EMPTY_SERIALIZER(SAM_Commander)

SAM_Commander::SAM_Commander() :  GroundVehicle() {}

EMPTY_COPYDATA(SAM_Commander)
EMPTY_DELETEDATA(SAM_Commander)
  
//==============================================================================
// GenericSite Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(GenericSite,"GenericSite")
EMPTY_SERIALIZER(GenericSite)

GenericSite::GenericSite() :  GroundVehicle() {}

EMPTY_COPYDATA(GenericSite)
EMPTY_DELETEDATA(GenericSite)
  
//==============================================================================
// Radar_Site Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(RadarSite,"RadarSite")
EMPTY_SERIALIZER(RadarSite)

RadarSite::RadarSite() :  GroundVehicle() {}

EMPTY_COPYDATA(RadarSite)
EMPTY_DELETEDATA(RadarSite)
  
//==============================================================================
// SAM_TELAR Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SAM_TELAR,"SAM_TELAR")
EMPTY_SERIALIZER(SAM_TELAR)

SAM_TELAR::SAM_TELAR() :  GroundVehicle() {}

EMPTY_COPYDATA(SAM_TELAR)
EMPTY_DELETEDATA(SAM_TELAR)


//==============================================================================
// Emitter Beam Structure (S)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(EmitterBeam,"EmitterBeam")
EMPTY_SERIALIZER(EmitterBeam)

EmitterBeam::EmitterBeam() :
        beamAzimuthCenter(0), beamAzimuthSweep(0), beamElevationCenter(0),
        beamElevationSweep(0), beamFunctionCode(BeamFunctionCodeOther), beamIdentifier(0),
        beamParameterIndex(0), effectiveRadiatedPower(0), emissionFrequency(0),
        emitterSystemIdentifier(), eventIdentifier(), frequencyRange(0),
        pulseRepetitionFrequency(0), pulseWidth(0), sweepSynch(0)
{
}
                                

void EmitterBeam::copyData(const EmitterBeam& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(EmitterBeam)

//==============================================================================
// Jammer Beam Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(JammerBeam,"JammerBeam")
EMPTY_SERIALIZER(JammerBeam)

JammerBeam::JammerBeam() :
jammingModeSequence(0), jammedObjectIdentifiers(), highDensityJam(RTI::RTI_FALSE)
{
}

void JammerBeam::copyData(const JammerBeam& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(JammerBeam)

//==============================================================================
// JRadar Beam Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(RadarBeam,"RadarBeam")
EMPTY_SERIALIZER(RadarBeam)

RadarBeam::RadarBeam() :
    TrackObjectIdentifiers(), HighDensityTrack(RTI::RTI_FALSE)
{
}

void RadarBeam::copyData(const RadarBeam& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(RadarBeam)

  
} // End Mc02Fom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
