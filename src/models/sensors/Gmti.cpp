
#include "openeaagles/models/sensors/Gmti.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/systems/Antenna.hpp"
#include "openeaagles/models/systems/TrackManager.hpp"
#include "openeaagles/models/Emission.hpp"
#include "openeaagles/models/Simulation.hpp"

#include "openeaagles/base/Integer.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Powers.hpp"
#include "openeaagles/base/units/Times.hpp"
#include "openeaagles/base/units/Frequencies.hpp"

#include <cmath>

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(Gmti, "Gmti")

// Slot table
BEGIN_SLOTTABLE(Gmti)
    "poi",            // 1: Point-Of-Interest (POI): meters [ north east down ]
END_SLOTTABLE(Gmti)

//  Map slot table
BEGIN_SLOT_MAP(Gmti)
    ON_SLOT(1,setSlotPoi,base::List)
END_SLOT_MAP()

EMPTY_DELETEDATA(Gmti)

Gmti::Gmti() : poiVec(0,0,0)
{
    STANDARD_CONSTRUCTOR()
}

void Gmti::copyData(const Gmti& org, const bool)
{
    BaseClass::copyData(org);
    poiVec = org.poiVec;
}

//------------------------------------------------------------------------------
// dynamics() --  Update dynamics
//------------------------------------------------------------------------------
void Gmti::dynamics(const double dt)
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
        double x =  dpoi[models::Player::INORTH] * cosHdg + dpoi[models::Player::IEAST] * sinHdg;
        double y = -dpoi[models::Player::INORTH] * sinHdg + dpoi[models::Player::IEAST] * cosHdg;
        double z = dpoi[models::Player::IDOWN];

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
void Gmti::setPoi(const double x, const double y, const double z)
{
    poiVec.set(x, y, z);
}

void Gmti::setPoi(const osg::Vec3d& newPoi)
{
    poiVec = newPoi;
}

//------------------------------------------------------------------------------
// setSlotPoi:  Set Slot POI Vector [ north east down ]
//------------------------------------------------------------------------------
bool Gmti::setSlotPoi(base::List* const numList)
{
    bool ok = false;
    double values[3];
    int n = numList->getNumberList(values, 3);
    if (n == 3) {
        setPoi(values[0], values[1], values[2]);
        ok = true;
    }
    return ok;
}

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
