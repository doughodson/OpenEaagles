//------------------------------------------------------------------------------
// Class: IrAtmosphere
//------------------------------------------------------------------------------

#include "openeaagles/simulation/IrAtmosphere.h"
#include "openeaagles/simulation/IrQueryMsg.h"
#include "openeaagles/simulation/IrSensor.h"
#include "openeaagles/simulation/Gimbal.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/base/List.h"
#include "openeaagles/base/functors/Tables.h"
#include "openeaagles/base/Number.h"

#include "openeaagles/base/Nav.h"
#include "openeaagles/base/units/Distances.h"

#include <cmath>

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(IrAtmosphere,"IrAtmosphere")
EMPTY_SERIALIZER(IrAtmosphere)
EMPTY_COPYDATA(IrAtmosphere)

// slot table
BEGIN_SLOTTABLE(IrAtmosphere)
   "waveBands",                // The size of the wavelength bins used in the tables
   "transmissivityTable1",     // transmissivities expressed as coefficients of (absorption+scattering), per km
   "skyRadiance",              // Simple background radiance for targets in sky
   "earthRadiance",            // Simple background radiance for targets on ground
END_SLOTTABLE(IrAtmosphere)

// slot map
BEGIN_SLOT_MAP(IrAtmosphere)
   ON_SLOT(1,setSlotWaveBands, base::Table1)
   ON_SLOT(2,setSlotTransmissivityTable1, base::Table1)
   ON_SLOT(3,setSlotSkyRadiance, base::Number)
   ON_SLOT(4,setSlotEarthRadiance, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
IrAtmosphere::IrAtmosphere()
{
    STANDARD_CONSTRUCTOR()
    waveBandTable = nullptr;
    numWaveBands = 0;
    transmissivityTable1 = nullptr;
    skyRadiance = 0.0;
    earthRadiance = 0.0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void IrAtmosphere::deleteData()
{
    if (waveBandTable != nullptr)        { waveBandTable->unref();        waveBandTable = nullptr;        }
    if (transmissivityTable1 != nullptr) { transmissivityTable1->unref(); transmissivityTable1 = nullptr; }
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool IrAtmosphere::setSlotWaveBands(const base::Table1* const tbl)
{
    bool ok = false;
    if (tbl != nullptr) {
        if (waveBandTable != nullptr) waveBandTable->unref();
        tbl->ref();
        waveBandTable = tbl;
        numWaveBands = tbl->getNumXPoints();
        ok = true;
    }
    return ok;
}

bool IrAtmosphere::setSlotTransmissivityTable1(const base::Table1* const tbl)
{
    bool ok = false;
    if (tbl != nullptr) {
        if (transmissivityTable1 != nullptr) transmissivityTable1->unref();
        tbl->ref();
        transmissivityTable1 = tbl;
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotSkyRadiance() -- sky radiance
//------------------------------------------------------------------------------
bool IrAtmosphere::setSlotSkyRadiance(oe::base::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        skyRadiance = num->getReal();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotEarthRadiance() -- set background radiance for ground
//------------------------------------------------------------------------------
bool IrAtmosphere::setSlotEarthRadiance(oe::base::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        earthRadiance = num->getReal();
        ok = true;
    }
    return ok;
}

// Transmissivity table should have same wavebands defined as for waveBandTable.
// Values in the table are coefficients of absorption
double IrAtmosphere::getTransmissivity(const unsigned int i, const double range) const
{
    double trans = 1.0;
    if (transmissivityTable1 != nullptr && i < transmissivityTable1->tableSize()) {
        const double* transmissivities = transmissivityTable1->getDataTable();
        trans = transmissivities[i];
        trans = std::exp(trans * -0.001 * range);
    }
    return trans;
}

double IrAtmosphere::getTransmissivity(const double wavebandCenter, const double range) const
{
    double trans = 1.0;
    if (transmissivityTable1 != nullptr){
        trans = transmissivityTable1->lfi(wavebandCenter);
        trans = std::exp(trans * -0.001 * range);
    }
    return trans;
}

bool IrAtmosphere::calculateAtmosphereContribution(IrQueryMsg* const msg, double* totalSignal, double* totalBackground)
{
    const double* centerWavelengths = getWaveBandCenters();
    const double* widths = getWaveBandWidths();
    double totalWavelengthRange = ((centerWavelengths[getNumWaveBands() - 1] + (widths[getNumWaveBands() - 1] / 2.0f))-(centerWavelengths[0] - (widths[0] / 2.0f)));
    const double* sigArray = msg->getSignatureByWaveband();
    double range2D = msg->getRange();
    *totalSignal = 0.0;
    *totalBackground = 0.0;
    double backgroundRadiance(0.0);

    // determine relation of FOV to horizon, to decide how much earth and how much sky in background
    {
        double currentViewAngle(0.0);
        double viewAngleToHorizon(0.0);

        // viewAngleToTarget is angle to target, not angle my sensor is actually pointing.
        // we want the fov i'm actually pointing at, not a FOV centered on each target

        // determine elevation angle to target, negative angles are down
        //{
        // //Player* ownship = msg->getOwnship();
        // // Determine the angle above the horizon to be used for background radiation lookup
        // double range2D = msg->getRange();
        // double tanPhi = (double)( (msg->getTarget()->getAltitudeM() - msg->getOwnship()->getAltitudeM())/ range2D );
        // double tanPhiPrime = tanPhi - ( range2D / 12756776.0f ); // Twice earth radius
        // // appears that negative angles are down in this calculation
        // currentViewAngle = lcAtan(tanPhiPrime);
        // // table limits are 0 to pi; this correction assumes that 0 in the table is straight down, PI is straight up
        // //viewAngleToTarget += PI/2.0;
        //}

        // determine elevation angle of gimbal/ownship, negative angles are down
        {
            const osg::Matrixd mm = msg->getGimbal()->getRotMat() * msg->getOwnship()->getRotMat();
            // compute Geodetic orientation angles
            osg::Vec3d angles;
            base::Nav::computeEulerAngles(mm, &angles);
            currentViewAngle = angles[Player::IPITCH];
        }

        // FAB determine angle to horizon, positive angles are down
        {
            double hDist = 1000000.0 * base::Distance::NM2M;  // Distance to horizon (m) (default: really far away)
            double hTanAng = 0;

            // earth radius in meters
            const double er = base::Nav::ERAD60 * base::Distance::NM2M;

            // distance from the center of the earth
            const double distEC = msg->getOwnship()->getAltitudeM() + er;
            const double distEC2 = distEC * distEC;  // squared

            // earth radius squared
            const double er2 = er * er;

            // distance to horizon squared
            const double dh2 = distEC2 - er2;

            // the distance and the tangent of the angle to the horizon
            hDist = std::sqrt(dh2);
            hTanAng = ( hDist / er ); // positive angles are below level (down)
            viewAngleToHorizon = std::atan(hTanAng);
        }

        // determine ratio of earth and sky in the FOV
        const double angleToHorizon = currentViewAngle + viewAngleToHorizon;
        const double fovtheta = msg->getSendingSensor()->getIFOVTheta();

        if (angleToHorizon - fovtheta >= 0) {
            // no ground, all sky?
            backgroundRadiance = getSkyRadiance();
        }
        else if (fovtheta - angleToHorizon >= 2*fovtheta) {
            // all ground, no sky?
            backgroundRadiance = getEarthRadiance();
        }
        else  {
            // looking at ground & sky
            double ratio = 0.5 + 0.5*angleToHorizon/fovtheta;   // convert range of -1 to 1 noninclusive to range of 0 to 1 noninclusive
            backgroundRadiance = ratio*getSkyRadiance() + (1.0-ratio)*getEarthRadiance();
        }
    }

    for (unsigned int i=0; i<getNumWaveBands(); i++) {
        const double lowerBandBound = centerWavelengths[i] - (widths[i] / 2.0);
        const double upperBandBound = lowerBandBound + widths[i];

        // determine ratio of this band's coverage to entire atmosphere waveband
        const double fractionOfBandToTotal = (upperBandBound - lowerBandBound) / totalWavelengthRange;

        // Find the limits of the sensor
        const double lowerSensorBound = msg->getLowerWavelength();
        const double upperSensorBound = msg->getUpperWavelength();

        // Determine how much of this wave band overlaps the sensor limits
        double lowerOverlap = getLowerEndOfWavelengthOverlap(lowerBandBound, lowerSensorBound);
        double upperOverlap = getUpperEndOfWavelengthOverlap(upperBandBound, upperSensorBound);

        if (upperOverlap < lowerOverlap) upperOverlap = lowerOverlap;

        const double overlapRatio = (upperOverlap - lowerOverlap) / (upperBandBound - lowerBandBound);

        // this depends on whether target is a sky or earth based target
        const double backgroundRadianceInBand = backgroundRadiance * fractionOfBandToTotal * overlapRatio;

        double radiantIntensityInBin(0.0);
        if (sigArray == nullptr) {
            // signature is a simple number
            // distribute simple signature evenly across atmosphere bins
            // need to apply overlapRatio to simple signature - already applied for complex signature in IrSignature...
            radiantIntensityInBin = msg->getSignatureAtRange() * fractionOfBandToTotal * overlapRatio;
        }
        else {
            // if the IrQueryMsg has a sigArray, then the signature is a multi-band signature
            // Find the limits of the bin
            //lowerBandBound = sigArray [i*3];
            //upperBandBound = sigArray [i*3 + 1] ;
            // assuming that signature bands match atmosphere bands
            radiantIntensityInBin = sigArray[i*3 + 2];
        }
        //double test = getTransmissivity(i, range2D);
        *totalSignal += radiantIntensityInBin * getTransmissivity(i, range2D);     //* getTransmissivity();

        // Add the background radiance from the this waveband within the sensor limits
        // to the total background radiance received by the sensor, watts/sr-m^2
        *totalBackground += backgroundRadianceInBand * getTransmissivity(i, range2D);   //* getTransmissivity();
    }

    return true;
}

//------------------------------------------------------------------------------
// getWaveBandCenters() -- Return center frequency of all wave bands
//------------------------------------------------------------------------------
const double* IrAtmosphere::getWaveBandCenters() const
{
    return ((waveBandTable!=nullptr) ? waveBandTable->getXData() : nullptr);
}

//------------------------------------------------------------------------------
// getWaveBandWidths() -- Return widths for all wave band frequencies
//------------------------------------------------------------------------------
const double* IrAtmosphere::getWaveBandWidths() const
{
    return ((waveBandTable != nullptr) ? waveBandTable->getDataTable() : nullptr);
}

base::Object* IrAtmosphere::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End simulation namespace
}

