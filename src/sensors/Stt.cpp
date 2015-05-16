#include "openeaagles/sensors/Stt.h"

#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/TrackManager.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Powers.h"
#include "openeaagles/basic/units/Times.h"
#include "openeaagles/basic/units/Frequencies.h"

#include <cmath>

namespace Eaagles {
namespace Sensor {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Stt,"Stt")

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator & clone()
//------------------------------------------------------------------------------
Stt::Stt()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Stt::copyData(const Stt& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Stt::deleteData()
{
}

//------------------------------------------------------------------------------
// dynamics() --  Update dynamics
//------------------------------------------------------------------------------
void Stt::dynamics(const LCreal dt)
{
    BaseClass::dynamics(dt);

    // ---
    // Update the antenna's Reference position
    // ---
    Simulation::TrackManager* tm = getTrackManager();
    if (getAntenna() != nullptr && getOwnship() != nullptr && tm != nullptr) {

        double az = 0.0;
        double el = 0.0;

        Basic::safe_ptr<Simulation::Track> trackList[2];
        int n = tm->getTrackList(trackList,2);

        if (n > 0) {
            // ---
            // Point the antenna at the first track
            // ---

            // Relative position vector to track
            osg::Vec3d dpoi = trackList[0]->getPosition();

            // rotate to ownship heading
            double sinHdg = getOwnship()->getSinHeading();
            double cosHdg = getOwnship()->getCosHeading();
            double x =  dpoi[Simulation::Player::INORTH] * cosHdg + dpoi[Simulation::Player::IEAST] * sinHdg;
            double y = -dpoi[Simulation::Player::INORTH] * sinHdg + dpoi[Simulation::Player::IEAST] * cosHdg;
            double z = dpoi[Simulation::Player::IDOWN];

            // Compute az & el to track
            double grng = std::sqrt(x*x + y*y);
            az = std::atan2(y,x);
            el = std::atan2(-z,grng);

            // Get current antenna limits
            double leftLim, rightLim;
            double lowerLim, upperLim;
            getAntenna()->getAzimuthLimits(&leftLim, &rightLim);
            getAntenna()->getElevationLimits(&lowerLim, &upperLim);

            // Limit to antenna limits
            if (az < leftLim) az = leftLim;
            else if (az > rightLim) az = rightLim;
            if (el < lowerLim) el = lowerLim;
            else if (el > upperLim) el = upperLim;

            // Set the reference 'look' angles and conical scan mode
            getAntenna()->setRefAzimuth(az);
            getAntenna()->setRefElevation(el);
            getAntenna()->setScanMode(Simulation::Antenna::CONICAL_SCAN);
        }
        else {
            // ---
            // when we don't have any tracks, enter the default (from input file) search mode
            // ---
            getAntenna()->setRefAzimuth(0.0);
            getAntenna()->setRefElevation(0.0);
            getAntenna()->setScanMode(Simulation::Antenna::HORIZONTAL_BAR_SCAN);
        }
    }
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Stt::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    // ###DPG need to print slots
    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Sensor namespace
} // End Eaagles namespace
