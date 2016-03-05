
#include "openeaagles/models/sensors/Gmti.h"

#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/TrackManager.h"
#include "openeaagles/base/Integer.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Powers.h"
#include "openeaagles/base/units/Times.h"
#include "openeaagles/base/units/Frequencies.h"

#include <cmath>

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(Gmti,"Gmti")

// Slot table
BEGIN_SLOTTABLE(Gmti)
    "poi",            // 1: Point-Of-Interest (POI): meters [ north east down ]
END_SLOTTABLE(Gmti)

//  Map slot table
BEGIN_SLOT_MAP(Gmti)
    ON_SLOT(1,setSlotPoi,base::List)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator & clone()
//------------------------------------------------------------------------------
Gmti::Gmti() : poiVec(0,0,0)
{
    STANDARD_CONSTRUCTOR()
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Gmti::copyData(const Gmti& org, const bool)
{
    BaseClass::copyData(org);
    poiVec = org.poiVec;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Gmti::deleteData()
{
}


//------------------------------------------------------------------------------
// dynamics() --  Update dynamics
//------------------------------------------------------------------------------
void Gmti::dynamics(const LCreal dt)
{
    BaseClass::dynamics(dt);

    // ---
    // Update the antenna's Reference position
    // ---
    if (getAntenna() != nullptr && getOwnship() != nullptr) {
        // Compute relative vector to POI
        osg::Vec3d dpoi = getPoi() - getOwnship()->getPosition();

        // rotate to ownship heading
        double sinHdg = getOwnship()->getSinHeading();
        double cosHdg = getOwnship()->getCosHeading();
        double x =  dpoi[simulation::Player::INORTH] * cosHdg + dpoi[simulation::Player::IEAST] * sinHdg;
        double y = -dpoi[simulation::Player::INORTH] * sinHdg + dpoi[simulation::Player::IEAST] * cosHdg;
        double z = dpoi[simulation::Player::IDOWN];

        // Compute az & el to POI
        double grng = std::sqrt(x*x + y*y);
        double az = std::atan2(y,x);
        double el = std::atan2(-z,grng);

        // Get current antenna limits and search volume
        double leftLim, rightLim;
        double lowerLim, upperLim;
        double width, height;
        getAntenna()->getAzimuthLimits(&leftLim, &rightLim);
        getAntenna()->getElevationLimits(&lowerLim, &upperLim);
        getAntenna()->getScanVolume(&width, &height);

        // Limit to within search scan limits of antenna
        if (az < base::Angle::aepcdRad(leftLim + width/2.0))
            az = base::Angle::aepcdRad(leftLim + width/2.0);
        else if (az > base::Angle::aepcdRad(rightLim - width/2.0))
            az = base::Angle::aepcdRad(rightLim - width/2.0);

        if (el < base::Angle::aepcdRad(lowerLim + height/2.0))
            el = base::Angle::aepcdRad(lowerLim + height/2.0);
        else if (el > base::Angle::aepcdRad(upperLim - height/2.0))
            el = base::Angle::aepcdRad(upperLim - height/2.0);

        // Set the reference 'look' angles
        getAntenna()->setRefAzimuth(az);
        getAntenna()->setRefElevation(el);
    }

    BaseClass::dynamics(dt);
}

//------------------------------------------------------------------------------
// setPoi() -- Gmti Point Of Interest
//------------------------------------------------------------------------------
void Gmti::setPoi(const LCreal x, const LCreal y, const LCreal z)
{
    poiVec.set(x, y, z);
}

void Gmti::setPoi(const osg::Vec3& newPoi)
{
    poiVec = newPoi;
}

//------------------------------------------------------------------------------
// setSlotPoi:  Set Slot POI Vector [ north east down ]
//------------------------------------------------------------------------------
bool Gmti::setSlotPoi(base::List* const numList)
{
    bool ok = false;
    LCreal values[3];
    int n = numList->getNumberList(values, 3);
    if (n == 3) {
        setPoi(values[0], values[1], values[2]);
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* Gmti::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Gmti::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    // Position Vector [ north east down ]
    indent(sout,i+j);
    sout << "poi: [ ";
    sout << poiVec.x();
    sout << " ";
    sout << poiVec.y();
    sout << " ";
    sout << poiVec.z();
    sout << " ]" << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
