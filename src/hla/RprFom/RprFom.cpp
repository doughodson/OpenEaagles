
#include "eaagles/hla/rprFom/RprFom.h"
 
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
         accelerationVector(), angularVelocityVector(),
         deadReckoningAlgorithm(DRM_RVW), entityType(),
         federateID(), isFrozen(RTI::RTI_FALSE), 
         orientation(), position(),
         velocityVector()
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
         damageState(NO_DAMAGE), engineSmokeOn(RTI::RTI_FALSE),
         flamesPresent(RTI::RTI_FALSE), hasFuelSupplyCap(RTI::RTI_FALSE),
         hasRecoveryCap(RTI::RTI_FALSE), hasRepairCap(RTI::RTI_FALSE),
         hatchState(HATCH_STATE_NOT_APPLICABLE), immobilized(RTI::RTI_FALSE),
         lifeformState(LIFEFORM_STATE_NOT_APPLICABLE), lightsState(LIGHT_STATE_OTHER),
         marking(), powerPlantOn(RTI::RTI_FALSE),
         rampDeployed(RTI::RTI_FALSE),smokePlumePresent(RTI::RTI_FALSE),
         tentDeployed(RTI::RTI_FALSE)
{
}

void PhysicalEntity::copyData(const PhysicalEntity& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(PhysicalEntity)

//==============================================================================
// MilitaryEntity Structure (S)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilitaryEntity,"MilitaryEntity")
EMPTY_SERIALIZER(MilitaryEntity)

MilitaryEntity::MilitaryEntity() :
      alternateEntityType(), camouflageType(RTI::RTI_FALSE),
      isConcealed(RTI::RTI_FALSE), firePowerDisabled(RTI::RTI_FALSE),
      forceID(FORCE_ID_OTHER)
{
}

void MilitaryEntity::copyData(const MilitaryEntity& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(MilitaryEntity)
    

//==============================================================================
// Soldier Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Soldier,"Soldier")
EMPTY_SERIALIZER(Soldier)

Soldier::Soldier() :
         primaryWeaponState(NO_WEAPON), secondaryWeaponState(NO_WEAPON)
{
}

void Soldier::copyData(const Soldier& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(Soldier)


//==============================================================================
// MunitionEntity Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MunitionEntity,"MunitionEntity")
EMPTY_SERIALIZER(MunitionEntity)

MunitionEntity::MunitionEntity() :
         launcherFlashPresent(RTI::RTI_FALSE)
{
}

void MunitionEntity::copyData(const MunitionEntity& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(MunitionEntity)


//==============================================================================
// MilitaryPlatformEntity Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilitaryPlatformEntity,"MilitaryPlatformEntity")
EMPTY_SERIALIZER(MilitaryPlatformEntity)

MilitaryPlatformEntity::MilitaryPlatformEntity() :
      afterburnerOn(RTI::RTI_FALSE), hasAmmunitionSupplyCap(RTI::RTI_FALSE),
      launcherRaised(RTI::RTI_FALSE)
{
}

void MilitaryPlatformEntity::copyData(const MilitaryPlatformEntity& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(MilitaryPlatformEntity)


//==============================================================================
// MilitaryMultiDomainPlatform Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilitaryMultiDomainPlatform,"MilitaryMultiDomainPlatform")
EMPTY_SERIALIZER(MilitaryMultiDomainPlatform)

MilitaryMultiDomainPlatform::MilitaryMultiDomainPlatform()
{
}

EMPTY_COPYDATA(MilitaryMultiDomainPlatform)
EMPTY_DELETEDATA(MilitaryMultiDomainPlatform)
  
//==============================================================================
// MilitarySubmersiblePlatform Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilitarySubmersiblePlatform,"MilitarySubmersiblePlatform")
EMPTY_SERIALIZER(MilitarySubmersiblePlatform)

MilitarySubmersiblePlatform::MilitarySubmersiblePlatform()
{
}

EMPTY_COPYDATA(MilitarySubmersiblePlatform)
EMPTY_DELETEDATA(MilitarySubmersiblePlatform)
  

//==============================================================================
// MilitarySeaSurfacePlatform Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilitarySeaSurfacePlatform,"MilitarySeaSurfacePlatform")
EMPTY_SERIALIZER(MilitarySeaSurfacePlatform)

MilitarySeaSurfacePlatform::MilitarySeaSurfacePlatform()
{
}

EMPTY_COPYDATA(MilitarySeaSurfacePlatform)
EMPTY_DELETEDATA(MilitarySeaSurfacePlatform)


//==============================================================================
// MilitarySpacePlatform Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilitarySpacePlatform,"MilitarySpacePlatform")
EMPTY_SERIALIZER(MilitarySpacePlatform)

MilitarySpacePlatform::MilitarySpacePlatform()
{
}

EMPTY_COPYDATA(MilitarySpacePlatform)
EMPTY_DELETEDATA(MilitarySpacePlatform)


//==============================================================================
// MilitaryLandPlatform Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilitaryLandPlatform,"MilitaryLandPlatform")
EMPTY_SERIALIZER(MilitaryLandPlatform)

MilitaryLandPlatform::MilitaryLandPlatform()
{
}

EMPTY_COPYDATA(MilitaryLandPlatform)
EMPTY_DELETEDATA(MilitaryLandPlatform)


//==============================================================================
// MilitaryAmphibiousPlatform Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilitaryAmphibiousPlatform,"MilitaryAmphibiousPlatform")
EMPTY_SERIALIZER(MilitaryAmphibiousPlatform)

MilitaryAmphibiousPlatform::MilitaryAmphibiousPlatform()
{
}

EMPTY_COPYDATA(MilitaryAmphibiousPlatform)
EMPTY_DELETEDATA(MilitaryAmphibiousPlatform)


//==============================================================================
// MilitaryAirLandPlatform Structure (PS)
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilitaryAirLandPlatform,"MilitaryAirLandPlatform")
EMPTY_SERIALIZER(MilitaryAirLandPlatform)

MilitaryAirLandPlatform::MilitaryAirLandPlatform()
{
}

EMPTY_COPYDATA(MilitaryAirLandPlatform)
EMPTY_DELETEDATA(MilitaryAirLandPlatform)
 
  
} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
