
#include "openeaagles/otw/OtwPC.h"

#include "dsFlightModelData.hxx"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/GroundVehicle.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/SlotTable.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/osg/Vec4"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace Otw {

IMPLEMENT_SUBCLASS(OtwPC,"OtwPC")
EMPTY_SERIALIZER(OtwPC)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(OtwPC)
    "netOutput",        // 1) Network output handler
END_SLOTTABLE(OtwPC)

// Map slot table to handles
BEGIN_SLOT_MAP(OtwPC)
    ON_SLOT(1,setSlotNetOutput,Basic::NetHandler)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
OtwPC::OtwPC() : netOutput(nullptr)
{
    STANDARD_CONSTRUCTOR()
    setMaxRange(0);
    setMaxModels(0);
    setMaxElevations(0);
    netInit = false;
    netInitFail = false;
    scnt = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void OtwPC::copyData(const OtwPC& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        netOutput = nullptr;
        setMaxRange(0);
        setMaxModels(0);
        setMaxElevations(0);
    }
    netOutput = nullptr;
    netInit = org.netInit;
    netInitFail = org.netInitFail;
    scnt = 0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void OtwPC::deleteData()
{
    netOutput = nullptr;
    netInit = false;
    netInitFail = false;
}

//------------------------------------------------------------------------------
// modelFactory() -- Create OtwModel objects unique to interface
//------------------------------------------------------------------------------
Simulation::OtwModel* OtwPC::modelFactory()
{
    return new Simulation::OtwModel();
}

//------------------------------------------------------------------------------
// hotFactory() -- Create OtwHot objects unique to interface
//------------------------------------------------------------------------------
Simulation::OtwModel* OtwPC::hotFactory()
{
    return new Simulation::OtwModel();
}

//------------------------------------------------------------------------------
// sendOwnshipAndModels() - Send state data for ownship and models
//------------------------------------------------------------------------------
void OtwPC::sendOwnshipAndModels()
{
    scnt--;
    if (scnt <= 0) {
        sendPcData();
        scnt = 5;   // send every 5th frame
    }
}

//------------------------------------------------------------------------------
// sendElevationRequests() - Sends terrain height requests
//------------------------------------------------------------------------------
void OtwPC::sendElevationRequests()
{
}

//------------------------------------------------------------------------------
// recvElevations() - Receives terrain height data
//------------------------------------------------------------------------------
void OtwPC::recvElevations()
{
}

//------------------------------------------------------------------------------
// frameSync() - Trigger the frame update
//------------------------------------------------------------------------------
void OtwPC::frameSync()
{
    // Initialization
    if (!isNetworkInitialized() && !didInitializationFail()) {
        if (initNetwork()) {
            netInit = true;
            netInitFail = false;
        }
        else {
            netInit = false;
            netInitFail = true;
        }
    }
}

//------------------------------------------------------------------------------
// reset() - Reset OTW
//------------------------------------------------------------------------------
void OtwPC::reset()
{
    BaseClass::reset();
}

//------------------------------------------------------------------------------
// initNetwork() -- Init the network
//------------------------------------------------------------------------------
bool OtwPC::initNetwork()
{
    bool ok = true;

    // Initialize network output handler
    if (netOutput != nullptr) {
        if (netOutput->initNetwork(true)) {
            if (isMessageEnabled(MSG_INFO)) {
                std::cout << "netOutput Initialize OK" << std::endl;
            }
        }
    }
    else {
        if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "OtwPC::initNetwork(): failure to find the network output handler (see slot 'netOutput')" << std::endl;
        }
        ok = false;
    }

    return ok;
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

// Net Network Output Handler
bool OtwPC::setSlotNetOutput(Basic::NetHandler* const msg)
{
    netOutput = msg;
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* OtwPC::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// sendPcData() -- Send PC data block
//------------------------------------------------------------------------------
void OtwPC::sendPcData()
{
    //const LCreal DEG2MR = (PI / 180.0f * 1000.0f);

    // Ownship type air vehicle?
    const Simulation::AirVehicle* av = dynamic_cast<const Simulation::AirVehicle*>(getOwnship());
    if (av != nullptr) {

        dsFlightModelData fmd;

        fmd.m_x_cg = 0;
        fmd.m_y_cg = 0;
        fmd.m_z_cg = 0;          // altitude

        const osg::Vec3 pos = av->getPosition();
        fmd.m_pilot_eye_x = pos[0] * Basic::Distance::M2FT;
        fmd.m_pilot_eye_y = pos[1] * Basic::Distance::M2FT;
        fmd.m_pilot_eye_z = -pos[2] * Basic::Distance::M2FT;   // altitude

        fmd.m_alphad = av->getAngleOfAttackD();
        fmd.m_betad = av->getSideSlipD();
        fmd.m_mach = av->getMach();
        fmd.m_runalt = 0.0;

        fmd.m_theta    = static_cast<float_4>(av->getPitchD());
        fmd.m_phi      = static_cast<float_4>(av->getRollD());
        fmd.m_psi      = static_cast<float_4>(av->getHeadingD());
        fmd.m_airspeed = static_cast<float_4>(av->getTotalVelocityKts());

        fmd.m_heading = static_cast<float_4>(av->getHeadingD());

        fmd.m_dlg = 0;           // landing gear position 90 is down (scaled to 0-1)
        fmd.m_dsb = static_cast<float_4>(av->getSpeedBrakePosition()/100.0f);   // speed break 60 is out (scaled to 0-1)
        fmd.m_nz  = static_cast<float_4>(av->getGload());

        fmd.m_aetrc = 0;         // Commanded throttle position
        fmd.m_afterburner = 0;   // logical, true in in A/B

        fmd.m_target_id = 0;

        fmd.m_id_self = 0;       // make use of a hole
        fmd.m_flags = 0;

        fmd.m_target_x = 0;
        fmd.m_target_y = 0;
        fmd.m_target_z = 0;

        fmd.m_target_theta = 0;
        fmd.m_target_phi = 0;
        fmd.m_target_psi = 0;

        fmd.m_target_uearth = 0;
        fmd.m_target_vearth = 0;
        fmd.m_target_wearth = 0;
        fmd.m_target_vcas = 0;

        if (netOutput != nullptr) {
           netOutput->sendData( reinterpret_cast<char*>(&fmd), sizeof(fmd) );
        }
    }
}

} // End Otw namespace
} // End Eaagles namespace

