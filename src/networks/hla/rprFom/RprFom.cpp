
#include "openeaagles/hla/rprFom/RprFom.h"
 
namespace Eaagles {
namespace Network {
namespace Hla {
namespace RprFom {

//==============================================================================
// BaseEntity Structure (S)
//==============================================================================

IMPLEMENT_SUBCLASS(BaseEntity,"BaseEntity")
EMPTY_SLOTTABLE(BaseEntity)
EMPTY_SERIALIZER(BaseEntity)

BaseEntity::BaseEntity() :
   entityType(), entityIdentifier(), spatial(), relativeSpatial()
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

PhysicalEntity::PhysicalEntity() :
         BaseEntity(), acousticSignatureIndex(0),
         alternateEntityType(), camouflageType(UNIFORM_PAINT_SCHEME),
         damageState(NO_DAMAGE), engineSmokeOn(RTI::RTI_FALSE),
         firePowerDisabled(RTI::RTI_FALSE), flamesPresent(RTI::RTI_FALSE),
         forceIdentifier(OTHER), hasAmmunitionSupplyCap(RTI::RTI_FALSE),
         hasFuelSupplyCap(RTI::RTI_FALSE), hasRecoveryCap(RTI::RTI_FALSE),
         hasRepairCap(RTI::RTI_FALSE), immobilized(RTI::RTI_FALSE),
         infraredSignatureIndex(0), isConcealed(RTI::RTI_FALSE),
         liveEntityMeasuredSpeed(0), marking(),
         powerPlantOn(RTI::RTI_FALSE), propulsionSystemsData(),
         radarCrossSectionSignatureIndex(0), smokePlumePresent(RTI::RTI_FALSE),
         tentDeployed(RTI::RTI_FALSE), trailingEffectsCode(NO_TRAIL),
         vectoringNozzleSystemData()
{
}

void PhysicalEntity::copyData(const PhysicalEntity& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(PhysicalEntity)
    


//==============================================================================
// Lifeform Structure (S)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Lifeform,"Lifeform")
EMPTY_SERIALIZER(Lifeform)

Lifeform::Lifeform()
{
}

void Lifeform::copyData(const Lifeform& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(Lifeform)

//==============================================================================
// Human Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Human,"Human")
EMPTY_SERIALIZER(Human)

Human::Human()
{
}

void Human::copyData(const Human& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(Human)


//==============================================================================
// NonHuman Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(NonHuman,"NonHuman")
EMPTY_SERIALIZER(NonHuman)

NonHuman::NonHuman()
{
}

void NonHuman::copyData(const NonHuman& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(NonHuman)


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
                hatchState(HATCH_STATE_NOT_APPLICABLE), headLightsOn(RTI::RTI_FALSE), interiorLightsOn(RTI::RTI_FALSE),
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

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
