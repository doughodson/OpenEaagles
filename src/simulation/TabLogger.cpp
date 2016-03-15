//------------------------------------------------------------------------------
// Classes:
//      TabLogger                     -- Simulation Event logger
//      TabLogger::TabLogEvent        -- Abstract simulation log event
//      TabLogger::LogPlayerData      -- Log Player Data event (header, new, update, removed)
//      TabLogger::LogWeaponActivity  -- Weapon release, detonation (or KILL, but it is not in use now.)
//      TabLogger::LogGunActivity     -- Gun was fired
//      TabLogger::LogActiveTrack     -- Log active (radar) track event (header, new, update, removed)
//      TabLogger::LogPassiveTrack    -- Log passive (rwr)  track event (header, new, update, removed)
//------------------------------------------------------------------------------
#include "openeaagles/simulation/TabLogger.h"

#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/NetIO.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Nib.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/TrackManager.h"
#include "openeaagles/simulation/Weapon.h"

#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Times.h"
#include "openeaagles/base/util/str_utils.h"

#include <string>
#include <sstream>

// ---
// TABLOGEVENT_B --
// use this macro in place of IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS() for classes derived from TabLogger::TabLogEvent
// ---
#define TABLOGEVENT_B(ThisType,FORMNAME)                                        \
    IMPLEMENT_PARTIAL_SUBCLASS(TabLogger::ThisType,FORMNAME)                    \
    EMPTY_SLOTTABLE(TabLogger::ThisType)                                        \
    TabLogger::ThisType::ThisType(const ThisType& org)                          \
    {                                                                           \
        STANDARD_CONSTRUCTOR()                                                  \
        copyData(org,true);                                                     \
    }                                                                           \
    TabLogger::ThisType::~ThisType()                                            \
    {                                                                           \
        STANDARD_DESTRUCTOR()                                                   \
    }                                                                           \
    TabLogger::ThisType& TabLogger::ThisType::operator=(const ThisType& org)    \
    {                                                                           \
        if (this != &org) copyData(org,false);                                  \
        return *this;                                                           \
    }                                                                           \
    TabLogger::ThisType* TabLogger::ThisType::clone() const                     \
    {                                                                           \
        return new TabLogger::ThisType(*this);                                  \
    }


namespace oe {
namespace simulation {

//==============================================================================
// Class: TabLogger
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(TabLogger,"TabLogger")

EMPTY_SLOTTABLE(TabLogger)
EMPTY_DELETEDATA(TabLogger)

// Constructor:
TabLogger::TabLogger()
{
    STANDARD_CONSTRUCTOR()
}

// Copy Constructor
TabLogger::TabLogger(const TabLogger& org)
{
    STANDARD_CONSTRUCTOR()
    copyData(org,true);
}

// Destructor
TabLogger::~TabLogger()
{
    STANDARD_DESTRUCTOR()
}

// Copy operator
TabLogger& TabLogger::operator=(const TabLogger& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

// Clone
TabLogger* TabLogger::clone() const
{
    return new TabLogger(*this);
}

// copyData() -- copy member data
void TabLogger::copyData(const TabLogger& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// updateTC() -- Update the simulation log time
//------------------------------------------------------------------------------
void TabLogger::updateTC(const double dt)
{
    BaseClass::updateTC(dt);
}

//------------------------------------------------------------------------------
// updateData() -- During background we'll build & send the descriptions of the
//                 simulation events to the log file.
//------------------------------------------------------------------------------
void TabLogger::updateData(const double dt)
{
    BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& TabLogger::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}


//==============================================================================
// Class: TabLogEvent
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(TabLogger::TabLogEvent,"TabLogEvent")
EMPTY_DELETEDATA(TabLogger::TabLogEvent)
EMPTY_SLOTTABLE(TabLogger::TabLogEvent)
EMPTY_SERIALIZER(TabLogger::TabLogEvent)

//------------------------------------------------------------------------------
// Constructor(s) & Destructors
//------------------------------------------------------------------------------
TabLogger::TabLogEvent::TabLogEvent()
{
    STANDARD_CONSTRUCTOR()
}

TabLogger::TabLogEvent::TabLogEvent(const TabLogEvent& org)
{
    STANDARD_CONSTRUCTOR()
    copyData(org,true);
}

// Destructor
TabLogger::TabLogEvent::~TabLogEvent()
{
   STANDARD_DESTRUCTOR()
}

//------------------------------------------------------------------------------
// Copy & Clone
//------------------------------------------------------------------------------
TabLogger::TabLogEvent& TabLogger::TabLogEvent::operator=(const TabLogEvent& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

TabLogger::TabLogEvent* TabLogger::TabLogEvent::clone() const
{
    return nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TabLogger::TabLogEvent::copyData(const TabLogEvent& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// makeTimeHdr() -- creates header line for the time string
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makeTimeHdr(std::ostream& sout)
{
    BaseClass::makeTabTimeHdr(sout);
    return sout;
}

//------------------------------------------------------------------------------
// makeTimeMsg() -- make the time string
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makeTimeMsg(std::ostream& sout)
{
    BaseClass::makeTabTimeMsg(sout);
    return sout;
}


//------------------------------------------------------------------------------
// makePlayerIdHdr() -- creates header line for the player ID message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerIdHdr(std::ostream& sout)
{
    sout << "player ID"                 << "\t";    // player ID                           (1 field)
    sout << "federate name"             << "\t";    // federate name of networked player   (1 field)
    sout << "network ID"                << "\t";    // network of networked player         (1 field)

    return sout;
}

//------------------------------------------------------------------------------
// makePlayerIdSpacer() -- creates empty space for the player ID message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerIdSpacer(std::ostream& sout)
{
    sout << "\t" ;   // player ID                           (1 field)
    sout << "\t";    // federate name of networked player   (1 field)
    sout << "\t";    // network of networked player         (1 field)

    return sout;
}


//------------------------------------------------------------------------------
// makePlayerIdMsg() -- creates the player ID message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerIdMsg(std::ostream& sout, const Player* const player)
{
    if (player != nullptr) {

        sout << player->getID()    << "\t" ;           // player ID                           (1 field)

        if (player->isNetworkedPlayer()) {             // federate name of networked player   (1 field)
                                                       // and network of networked player     (1 field)
            const Nib* const pNib = player->getNib();
            sout << static_cast<const char*>(*pNib->getFederateName()) << "\t" ;

            const NetIO* const pNet = pNib->getNetIO();

            if (pNet != nullptr)
                sout << pNet->getNetworkID() << "\t";
            else
                sout << "\t";

        }
        else
            sout <<"\t\t" ;
    }
    else
        makePlayerIdSpacer(sout);

    return sout;
}


//------------------------------------------------------------------------------
// makePlayerDataHdr() -- creates the player ID message heading
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerDataHdr( std::ostream& sout )
{
    sout << "X position" << "\t"          // (3 fields)
         << "Y position" << "\t"
         << "Z position" << "\t" ;
    sout << "vel[0]" << "\t"              // (3 fields)
         << "vel[1]" << "\t"
         << "vel[2]" << "\t" ;
    sout << "orientation[0]" << "\t"      // (3 fields)
         << "orientation[1]" << "\t"
         << "orientation[2]" << "\t" ;
    return sout;
}


//------------------------------------------------------------------------------
// makePlayerDataSpacer() -- creates empty space for the player ID message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerDataSpacer( std::ostream& sout )
{
    sout << "\t"       // X position      // (3 fields)
         << "\t"       // Y position
         << "\t" ;     // Z position
    sout << "\t"       // vel[0]          // (3 fields)
         << "\t"       // vel[1]
         << "\t" ;     // vel[2]
    sout << "\t"       // orientation[0]  // (3 fields)
         << "\t"       // orientation[1]
         << "\t" ;     // orientation[2]

    return sout;
}


//------------------------------------------------------------------------------
// makePlayerDataMsg() -- creates the player ID message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerDataMsg(  std::ostream& sout,
                               osg::Vec3 pos0, osg::Vec3 vel0, osg::Vec3 angles0)
{
    sout << pos0[0]   << "\t"                                 // (3 fields)
         << pos0[1]   << "\t"
         << pos0[2]   << "\t" ;
    sout << vel0[0]   << "\t"                                 // (3 fields)
         << vel0[1]   << "\t"
         << vel0[2]   << "\t" ;
    sout << (angles0[0] * base::Angle::R2DCC) << "\t"      // (3 fields)
         << (angles0[1] * base::Angle::R2DCC) << "\t"
         << (angles0[2] * base::Angle::R2DCC) << "\t" ;
    return sout;
}



//------------------------------------------------------------------------------
// makePlayerLatLonSpacer() -- creates empty space for the player ID message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerLatLonSpacer(  std::ostream& sout)
{
    sout << "\t" ;               // latitude                  // (1 field)
    sout << "\t" ;               // longitude                 // (1 field)

    return sout;
}


//------------------------------------------------------------------------------
// makePlayerLatLonHdr() -- creates the player ID message heading
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerLatLonHdr(  std::ostream& sout)
{
    sout << "lat."  << "\t" ;    // latitude                  // (1 field)
    sout << "lon." << "\t" ;     // longitude                 // (1 field)

    return sout;
}

//------------------------------------------------------------------------------
// makePlayerLatLonMsg() -- creates the player ID message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerLatLonMsg(  std::ostream& sout,
                                                   double theLat, double theLon)
{
   sout << theLat << "\t" ;                   // (1 field)
   sout << theLon << "\t" ;                   // (1 field)

    return sout;
}



//------------------------------------------------------------------------------
// makePlayerDamageHdr() -- creates header line for the player damage message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerDamageHdr(std::ostream& sout)
{
    sout << "damage state"              << "\t" ;  // player damage value                   (1 field)
    sout << "override"                  << "\t";   // kill or crash override (true/false)   (1 field)

    return sout;
}

//------------------------------------------------------------------------------
// makePlayerDamageSpacer() -- creates empty space for the player damage message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerDamageSpacer(std::ostream& sout)
{
    sout << "\t" ;   // player damage value                   (1 field)
    sout << "\t";    // kill or crash override (true/false)   (1 field)

    return sout;
}


//------------------------------------------------------------------------------
// makePlayerDamageMsg() -- creates the player damage message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makePlayerDamageMsg(std::ostream& sout, const Player* const player, bool checkKillOverride)
{
    if (player != nullptr) {

        sout << player->getDamage()    << "\t" ;            // player damage value                   (1 field)

        if (  (checkKillOverride && player->isKillOverride() ) ||   // kill or crash override
              (!checkKillOverride && player->isCrashOverride() ))   //          (true/false)         (1 field)
            sout << "true" << "\t" ;
        else
            sout << "false" << "\t" ;
    }
    else
        makePlayerDamageSpacer(sout);

    return sout;
}

//------------------------------------------------------------------------------
// makeTrackDataHdr() --  creates header line for the track data message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makeTrackDataHdr(std::ostream& sout)
{
    sout << "Type"                     << "\t";   // type                   (1 field)
    sout << "Track ID"                 << "\t";   // track ID               (1 field)
    sout << "Range"                    << "\t";   // range                  (1 field)
    sout << "Ground Range"             << "\t";   // ground range           (1 field)
    sout << "Azimuth, true (deg)"      << "\t";   // true azimuth (deg)     (1 field)
    sout << "Azimuth, relative (deg)"  << "\t";   // relative azimuth (deg) (1 field)
    sout << "Elevation Angle (deg)"    << "\t";   // elevation              (1 field)

    sout << "position[0]"     << "\t"             // position               (3 fields)
         << "position[1]"     << "\t"
         << "position[2]"     << "\t" ;

    sout << "velocity[0]"     << "\t"             // velocity               (3 fields)
         << "velocity[1]"     << "\t"
         << "velocity[2]"     << "\t" ;

    sout << "signal"          << "\t" ;           // signal                 (1 field)
    sout << "shootlist index" << "\t" ;           // shootlist index        (1 field)

    return sout;
}


//------------------------------------------------------------------------------
// makeTrackDataMsg() -- creates the track data message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makeTrackDataMsg(std::ostream& sout, const Track* const trk)
{
    if (trk != nullptr)  {

        sout << trk->getType()            << "\t" ;    // type                   (1 field)
        sout << trk->getTrackID()         << "\t" ;    // track ID               (1 field)
        sout << trk->getRange()           << "\t" ;    // range                  (1 field)
        sout << trk->getGroundRange()     << "\t" ;    // ground range           (1 field)
        sout << trk->getTrueAzimuthD()    << "\t" ;    // true azimuth (deg)     (1 field)
        sout << trk->getRelAzimuthD()     << "\t" ;    // relative azimuth (deg) (1 field)
        sout << trk->getElevationD()      << "\t" ;    // elevation              (1 field)

        osg::Vec3 tpos = trk->getPosition();
        sout << tpos[0]    << "\t"                     // position               (3 fields)
             << tpos[1]    << "\t"
             << tpos[2]    << "\t" ;

        osg::Vec3 tvel = trk->getVelocity();
        sout << tvel[0]    << "\t"                     // velocity               (3 fields)
             << tvel[1]    << "\t"
             << tvel[2]    << "\t" ;

        const RfTrack* const rfTrk = dynamic_cast<const RfTrack*>(trk);
        if (rfTrk != nullptr)
            sout << rfTrk->getAvgSignal() << "\t";     // signal                 (1 field)
        else
            sout << "\t";

        sout <<  trk->getShootListIndex() << "\t";     // shootlist index        (1 field)

    }
    else
        sout << "\t\t\t\t\t\t \t\t\t \t\t\t \t \t";    // (14 spacer fields)

    return sout;
}


//------------------------------------------------------------------------------
// makeEmissionDataHdr() -- creates header line for the emission data message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makeEmissionDataHdr(std::ostream& sout)
{
    sout << "AOI Azimuth"      << "\t";    // azimuth AOI       (1 field)
    sout << "AOI Elevation"    << "\t";    // elevation AOI     (1 field)
    sout << "Frequency"        << "\t";    // frequency         (1 field)
    sout << "Lamda"            << "\t";    // wavelength        (1 field)
    sout << "Pulse width"      << "\t";    // pulsewidth        (1 field)
    sout << "PRF"              << "\t";    // prf               (1 field)

    return sout;
}


//------------------------------------------------------------------------------
// makeEmissionDataSpacer() -- creates empty space for the emission data message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makeEmissionDataSpacer(std::ostream& sout)
{
    sout << "\t";    // azimuth AOI              (1 field)
    sout << "\t";    // elevation AOI            (1 field)
    sout << "\t";    // frequency                (1 field)
    sout << "\t";    // wavelength               (1 field)
    sout << "\t";    // pulsewidth               (1 field)
    sout << "\t";    // prf                      (1 field)

    return sout;
}

//------------------------------------------------------------------------------
// makeEmissionDataMsg() -- creates the emission data message
//------------------------------------------------------------------------------
std::ostream& TabLogger::TabLogEvent::makeEmissionDataMsg(std::ostream& sout, const Emission* const em)
{
    if (em != nullptr) {

        sout << (base::Angle::R2DCC * em->getAzimuthAoi())     << "\t";  // azimuth AOI    (1 field)
        sout << (base::Angle::R2DCC * em->getElevationAoi())   << "\t";  // elevation AOI  (1 field)
        sout << (em->getFrequency())                            << "\t";  // frequency      (1 field)
        sout << (em->getWavelength())                           << "\t";  // wavelength     (1 field)
        sout << (em->getPulseWidth())                           << "\t";  // pulsewidth     (1 field)
        sout << (em->getPRF())                                  << "\t";  // prf            (1 field)
    }
    else
        sout << "\t\t\t\t\t\t" ;   // (six spacer fields)

    return sout;
}


//==============================================================================
// Class: TabLogger::LogPlayerData
//==============================================================================
TABLOGEVENT_B(LogPlayerData,"TabLogger::LogPlayerData")
EMPTY_SERIALIZER(TabLogger::LogPlayerData)

// Constructor
TabLogger::LogPlayerData::LogPlayerData(int t, const Player* const p)
{
    STANDARD_CONSTRUCTOR()

    initData();
    thePlayer = p;
    theType = t;
}

TabLogger::LogPlayerData::LogPlayerData(int t, const Player* const p, const Player* const w)
{
    STANDARD_CONSTRUCTOR()

    initData();

    theSource = w;  // source of damage, usually a weapon, always a player
    thePlayer = p;
    theType = t;
    mach = -1.0;
}

void TabLogger::LogPlayerData::initData()
{
    theSource = nullptr;
    thePlayer = nullptr;
    theType = 0;
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
    latitude = 0.0;
    longitude = 0.0;
    alpha = 0.0;
    beta = 0.0;
    ias = 0.0;
    mach = 0.0;
    pLoading = 0.0;
}

// Copy data function
void TabLogger::LogPlayerData::copyData(const LogPlayerData& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();

    theSource = org.theSource;
    thePlayer = org.thePlayer;
    theType = org.theType;
    pos = org.pos;
    vel = org.vel;
    angles = org.angles;
    latitude = org.latitude;
    longitude = org.longitude;
    alpha = org.alpha;
    beta = org.beta;
    ias = org.ias;
    mach = org.mach;
    pLoading = org.pLoading;
}

// Delete data function
void TabLogger::LogPlayerData::deleteData()
{
    theSource = nullptr;
    thePlayer = nullptr;
    theType = 0;
}

// Get the description
const char* TabLogger::LogPlayerData::getDescription()
{
    if (msg == nullptr) {

        std::stringstream sout;

        if (theType != 0)
            makeTimeMsg(sout);
        else
            makeTimeHdr(sout);

        sout << "PLAYER\t";

        switch(theType)
        {
        case 0:    { sout << "HEADER\t";      break; }  // same header shared for all of following
        case 1:    { sout << "NEW\t";         break; }
        case 2:    { sout << "DATA\t";        break; }
        case 3:    { sout << "REMOVED\t";     break; }
        case 4:    { sout << "DAMAGE\t";      break; }
        case 5:    { sout << "COLLISION\t";   break; }
        case 6:    { sout << "CRASH\t";       break; }
        case 7:    { sout << "KILL\t";        break; }
        default:   { sout << "UNKNOWN\t";     break; }  // someone called for a player.... unknown reason
        }

        if (theType == 0) { // header line

            makePlayerIdHdr(sout);
            makePlayerDataHdr(sout);
            makePlayerLatLonHdr(sout);

            sout << "alpha"      << "\t" ;  // (1 field)
            sout << "beta"       << "\t" ;  // (1 field)
            sout << "ias"        << "\t" ;  // (1 field)

            sout <<  "mach"      << "\t" ;  // (1 field)
            sout <<  "pLoading"  << "\t" ;  // (1 field)

            makePlayerDamageHdr(sout);
            sout << "damage source: " ;
            makePlayerIdHdr(sout);
        }
        else if (thePlayer == nullptr) {
            // no player data, so leave the line blank and skedaddle
            //sout << "\n";
        }
        else {
            // Print the Player data
            makePlayerIdMsg(sout,thePlayer);
            makePlayerDataMsg(sout,pos,vel,angles);
            makePlayerLatLonMsg(sout, latitude, longitude);

            if ((ias >= 0.0) && (theType == 2)) {
                sout << alpha << "\t" ;   // (1 field)
                sout << beta  << "\t" ;   // (1 field)
                sout << ias   << "\t" ;   // (1 field)
            }
            else {
                sout << " \t\t\t";
            }

            if (mach > 0.0) {
                sout <<  mach      << "\t" ;  // (1 field)
                sout <<  pLoading  << "\t" ;  // (1 field)
            }
            else {
                sout << "\t\t" ;
            }

            // print damage data
            if (theType == 4) {                               // detonation
                // third field in makePlayerDamageMsg differentiates between checking
                //    killOverride (true) and crashOverride (false).
                makePlayerDamageMsg(sout, thePlayer, true);
                makePlayerIdMsg(sout, theSource);
            }
            else if ((theType == 5) || (theType == 7)) {      // collision or kill
                makePlayerDamageMsg(sout, thePlayer, false);
                makePlayerIdMsg(sout, theSource);
            }
            else if (theType == 6) {                          // crash (ground)
                makePlayerDamageMsg(sout, thePlayer, false);
                makePlayerIdSpacer(sout);
            }
            else {
                makePlayerDamageSpacer(sout);
                makePlayerIdSpacer(sout);
            }
        }

        // Complete the description
        const int len = static_cast<int>(sout.str().size());
        msg = new char[len+1];
        base::lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void TabLogger::LogPlayerData::captureData()
{
    if (thePlayer != nullptr) {
        alpha = -1.0;
        beta = -1.0;
        ias = -1.0;
        {
            pos = thePlayer->getPosition();
            vel = thePlayer->getVelocity();
            latitude = thePlayer->getLatitude();
            longitude = thePlayer->getLongitude();
            angles = thePlayer->getEulerAngles();
            const Player* const p = thePlayer;
            const AirVehicle* const av = dynamic_cast<const AirVehicle*>(p);
            if ((av != nullptr) && (theType == 2) ) {
                alpha = av->getAngleOfAttackD();
                beta = av->getSideSlipD();
                ias = av->getCalibratedAirspeed();
                mach = thePlayer->getMach();
                pLoading = av->getGload();
            }
        }
    }
}


//==============================================================================
// Class: TabLogger::LogGunActivity
//==============================================================================
TABLOGEVENT_B(LogGunActivity,"TabLogger::LogGunActivity")
EMPTY_SERIALIZER(TabLogger::LogGunActivity)

// Constructor
TabLogger::LogGunActivity::LogGunActivity(int t, const Player* const launcher, const int n)
{
    STANDARD_CONSTRUCTOR()
    theType = t;
    thePlayer = launcher;
    rounds = n;
}

void TabLogger::LogGunActivity::initData()
{
    theType = 0;
    thePlayer = nullptr;
    rounds = 0;
}

// Copy data function
void TabLogger::LogGunActivity::copyData(const LogGunActivity& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();
    theType = org.theType;
    thePlayer = org.thePlayer;
    rounds = org.rounds;
}

// Delete data function
void TabLogger::LogGunActivity::deleteData()
{
    theType = 0;
    thePlayer = nullptr;
}

// Get the description
const char* TabLogger::LogGunActivity::getDescription()
{
    if (msg == nullptr) {

        std::stringstream sout;

        // Time & Event message
        if (theType != 0)
            makeTimeMsg(sout);
        else
            makeTimeHdr(sout);

        sout << "GUN\t" ;

        switch(theType)
        {
        case 0:    { sout << "HEADER\t";       break; }  // same header shared for all of following
        case 1:    { sout << "FIRED\t";        break; }
        default:   { sout << "UNKNOWN\t";      break; }  // unknown weapon activity....
        }

        if (theType == 0) {
            sout << "launcher: ";
            makePlayerIdHdr(sout);

            sout << "rounds" << "\t";            // (1 field)
        }
        else {
            makePlayerIdMsg(sout, thePlayer);
            sout << rounds << "\t";              // (1 field)
        }

        // Complete the description
        const int len = static_cast<int>(sout.str().size());
        msg = new char[len+1];
        base::lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void TabLogger::LogGunActivity::captureData()
{
}


//==============================================================================
// Class: TabLogger::LogWeaponActivity
//==============================================================================
TABLOGEVENT_B(LogWeaponActivity,"TabLogger::LogWeaponActivity")
EMPTY_SERIALIZER(TabLogger::LogWeaponActivity)

// Constructor
TabLogger::LogWeaponActivity::LogWeaponActivity(const int callType, const Player* const lancher, const Player* const wpn,
                                                const Player* const tgt, const unsigned int t, const double d)
{
    STANDARD_CONSTRUCTOR()
    theType   = callType;
    thePlayer = lancher;
    theWeapon = wpn;
    theTarget = tgt;
    detType   = t;
    missDist  = d;
    theTrack = nullptr;
    eventID = 0;
}

void TabLogger::LogWeaponActivity::initData()
{
    theType   = 0;
    thePlayer = nullptr;
    theWeapon = nullptr;
    theTarget = nullptr;
    detType   = 0;
    missDist  = 0;
    theTrack = nullptr;
    eventID = 0;
}

// Copy data function
void TabLogger::LogWeaponActivity::copyData(const LogWeaponActivity& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();
    theType   = org.theType;
    thePlayer = org.thePlayer;
    theWeapon = org.theWeapon;
    theTarget = org.theTarget;
    detType   = org.detType;
    missDist  = org.missDist;
    theTrack = org.theTrack;
    eventID = org.eventID;
}

// Delete data function
void TabLogger::LogWeaponActivity::deleteData()
{
    theType   = 0;
    thePlayer = nullptr;
    theWeapon = nullptr;
    theTarget = nullptr;
    theTrack = nullptr;
}

// Get the description
const char* TabLogger::LogWeaponActivity::getDescription()
{
    if (msg == nullptr) {

        std::stringstream sout;

        // Time & Event message
        if (theType != 0)
            makeTimeMsg(sout);
        else
            makeTimeHdr(sout);

        sout << "WEAPON\t" ;

        switch(theType)
        {
        case 0:    { sout << "HEADER\t";       break; }  // same header shared for all of following
        case 1:    { sout << "RELEASE\t";      break; }
        case 2:    { sout << "DETONATE\t";     break; }
        case 3:    { sout << "KILL\t";         break; }  // "KILL" not currently in use.
        case 4:    { sout << "HUNG\t";         break; }
        default:   { sout << "UNKNOWN\t";      break; }  // unknown weapon activity....
        }

        if (theType == 0) { // header line

            sout << "launcher: ";
            makePlayerIdHdr(sout);

            sout << "weapon: ";
            makePlayerIdHdr(sout);

            sout << "target: ";
            makePlayerIdHdr(sout);

            sout << "launch event ID\t" ;    // launch event ID           // (1 field)

            sout << "detonation type\t" ;    // detonation type           // (1 field)
            sout << "miss distance\t" ;      // miss distance             // (1 field)

            sout << "track: ";               // track stored on weapon
            makeTrackDataHdr(sout);
        }
        else {
            // Print the (launcher) Player ID
            makePlayerIdMsg(sout, thePlayer);

            // Print the weapon ID
            makePlayerIdMsg(sout, theWeapon);

            // Print the target ID
            makePlayerIdMsg(sout, theTarget);

            sout << eventID      << "\t" ;   // launch event ID           // (1 field)

            if ((theType == 1) || (theType == 4)) {  // (type 1, launch) or (type 4, hung store)
                sout << "\t";                // detonation type           // (1 field)
                sout << "\t";                // miss distance             // (1 field)
            }
            else {                                    // (type 2, detonation) or (type 3, kill)
                sout << detType  << "\t";    // detonation type           // (1 field)
                sout << missDist << "\t";    // miss distance             // (1 field)
            }

            makeTrackDataMsg(sout, theTrack);
        }

        // Complete the description
        const int len = static_cast<int>(sout.str().size());
        msg = new char[len+1];
        base::lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void TabLogger::LogWeaponActivity::captureData()
{
    const Player* const w = theWeapon;
    const Weapon* const wpn = dynamic_cast<const Weapon*>(w);

    if (wpn != nullptr) {
        theTrack = wpn->getTargetTrack();
        eventID = wpn->getReleaseEventID();
    }
}


//==============================================================================
// Class: TabLogger::LogActiveTrack
//==============================================================================
TABLOGEVENT_B(LogActiveTrack,"TabLogger::LogActiveTrack")
EMPTY_SERIALIZER(TabLogger::LogActiveTrack)

// Constructor
TabLogger::LogActiveTrack::LogActiveTrack(int t, const TrackManager* const mgr, const Track* const trk)
{
    STANDARD_CONSTRUCTOR()

    initData();

    theType = t;
    theManager = mgr;
    theTrack = trk;
    if (theManager != nullptr) {
        thePlayer = dynamic_cast<const Player*>( theManager->findContainerByType(typeid(Player)) );
    }
    if (theTrack != nullptr) {
        const RfTrack* const rfTrk = dynamic_cast<const RfTrack*>(trk);
        if (rfTrk != nullptr) {
            theEmission = rfTrk->getLastEmission();
        }
    }
}

void TabLogger::LogActiveTrack::initData()
{
    thePlayer = nullptr;
    theEmission = nullptr;
    theType = 0;
    theManager = nullptr;
    theTrack = nullptr;
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
    tgtPos.set(0,0,0);
    tgtVel.set(0,0,0);
    tgtAngles.set(0,0,0);
    sn = 0.0;
}

// Copy data function
void TabLogger::LogActiveTrack::copyData(const LogActiveTrack& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();

    thePlayer = org.thePlayer;
    theEmission = org.theEmission;
    theType = org.theType;
    theManager = org.theManager;
    theTrack = org.theTrack;
    pos = org.pos;
    vel = org.vel;
    angles = org.angles;
    tgtPos = org.tgtPos;
    tgtVel = org.tgtVel;
    tgtAngles = org.tgtAngles;
    sn = org.sn;
}

// Delete data function
void TabLogger::LogActiveTrack::deleteData()
{
    theType = 0;
    theManager = nullptr;
    theTrack = nullptr;
    thePlayer = nullptr;
    theEmission = nullptr;
}

// Get the description
const char* TabLogger::LogActiveTrack::getDescription()
{
    if (msg == nullptr) {
        std::stringstream sout;

        // Time & Event message
        if (theType != 0)
            makeTimeMsg(sout);
        else
            makeTimeHdr(sout);

        sout << "TRACK_ACTIVE\t" ;

        switch(theType)
        {
        case 0:    { sout << "HEADER\t";       break; }  // same header shared for all of following
        case 1:    { sout << "ADDED\t";        break; }
        case 2:    { sout << "UPDATE\t";       break; }
        case 3:    { sout << "REMOVED\t";      break; }
        default:   { sout << "UNKNOWN\t";      break; }  // someone called for a track.... unknown reason
        }

        if (theType == 0) { // header line
            sout << "Player: " ;
            makePlayerIdHdr(sout);
            makePlayerDataHdr(sout);

            sout << "Target: " ;
            makePlayerIdHdr(sout);
            makePlayerDataHdr(sout);

            sout << "Track: " ;
            makeTrackDataHdr(sout);
        }
        else {
            // Player information
            makePlayerIdMsg(sout, thePlayer);

            if (thePlayer != nullptr)
                makePlayerDataMsg(sout,pos,vel,angles);
            else
                makePlayerDataSpacer(sout);

            // Target Information
            if (theEmission != nullptr) {
                makePlayerIdMsg(sout, theEmission->getTarget());

                if (theEmission->getTarget() != nullptr)
                    makePlayerDataMsg(sout,tgtPos,tgtVel,tgtAngles);
                else
                    makePlayerDataSpacer(sout);
            }
            else {
                makePlayerIdSpacer(sout);
                makePlayerDataSpacer(sout);
            }

            // General track information
            makeTrackDataMsg(sout, theTrack);
        }

        // Complete the description
        const int len = static_cast<int>(sout.str().size());
        msg = new char[len+1];
        base::lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void TabLogger::LogActiveTrack::captureData()
{
    if (thePlayer != nullptr) {
        pos = thePlayer->getPosition();
        vel = thePlayer->getVelocity();
        angles = thePlayer->getEulerAngles();
    }
    if (theEmission != nullptr) {
        if (theEmission->getTarget() != nullptr) {
            tgtPos = theEmission->getTarget()->getPosition();
            tgtVel = theEmission->getTarget()->getVelocity();
            tgtAngles = theEmission->getTarget()->getEulerAngles();
        }
    }
}


//==============================================================================
// Class: TabLogger::LogPassiveTrack
//==============================================================================
TABLOGEVENT_B(LogPassiveTrack,"TabLogger::LogPassiveTrack")
EMPTY_SERIALIZER(TabLogger::LogPassiveTrack)

// Constructor
TabLogger::LogPassiveTrack::LogPassiveTrack(int t, const TrackManager* const mgr, const Track* const trk)
{
    STANDARD_CONSTRUCTOR()

    initData();

    theType = t;
    theManager = mgr;
    theTrack = trk;
    if (theManager != nullptr) {
        thePlayer = dynamic_cast<const Player*>( theManager->findContainerByType(typeid(Player)) );
    }
    if (theTrack != nullptr) {
        const RfTrack* const rfTrk = dynamic_cast<const RfTrack*>(trk);
        if (rfTrk != nullptr) {
            theEmission = rfTrk->getLastEmission();
        }
    }
}

void TabLogger::LogPassiveTrack::initData()
{
    thePlayer = nullptr;
    theEmission = nullptr;
    theType = 0;
    theManager = nullptr;
    theTrack = nullptr;
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
    tgtPos.set(0,0,0);
    tgtVel.set(0,0,0);
    tgtAngles.set(0,0,0);
    sn = 0.0;
}

// Copy data function
void TabLogger::LogPassiveTrack::copyData(const LogPassiveTrack& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();

    thePlayer = org.thePlayer;
    theEmission = org.theEmission;
    theType = org.theType;
    theManager = org.theManager;
    theTrack = org.theTrack;
    pos = org.pos;
    vel = org.vel;
    angles = org.angles;
    tgtPos = org.tgtPos;
    tgtVel = org.tgtVel;
    tgtAngles = org.tgtAngles;
    sn = org.sn;
}

// Delete data function
void TabLogger::LogPassiveTrack::deleteData()
{
    theType = 0;
    theManager = nullptr;
    theTrack = nullptr;
    thePlayer = nullptr;
    theEmission = nullptr;
}

// Get the description
const char* TabLogger::LogPassiveTrack::getDescription()
{
    if (msg == nullptr) {
        std::stringstream sout;

        // Time & Event message
        if (theType != 0)
            makeTimeMsg(sout);
        else
            makeTimeHdr(sout);

        sout << "TRACK_PASSIVE\t" ;

        switch(theType)
        {
        case 0:    { sout << "HEADER\t";       break; }  // same header shared for all of following
        case 1:    { sout << "ADDED\t";        break; }
        case 2:    { sout << "UPDATE\t";       break; }
        case 3:    { sout << "REMOVED\t";      break; }
        default:   { sout << "UNKNOWN\t";      break; }  // someone called for a track.... unknown reason
        }

        if (theType == 0) { // header line

            sout << "Player: " ;
            makePlayerIdHdr(sout);
            makePlayerDataHdr(sout);

            sout << "Target: " ;
            makePlayerIdHdr(sout);
            makePlayerDataHdr(sout);

            sout << "Emission: " ;
            makeEmissionDataHdr(sout);

            sout << "Track: " ;  // (1 field)
            makeTrackDataHdr(sout);
        }
        else {
            // Player information
            makePlayerIdMsg(sout, thePlayer);

            if (thePlayer != nullptr)
                makePlayerDataMsg(sout,pos,vel,angles);
            else
                makePlayerDataSpacer(sout);

            // Target Information
            if (theEmission != nullptr) {
                makePlayerIdMsg(sout, theEmission->getOwnship());

                if (theEmission->getOwnship() != nullptr)
                    makePlayerDataMsg(sout,tgtPos,tgtVel,tgtAngles);
                else
                    makePlayerDataSpacer(sout);

                makeEmissionDataMsg(sout, theEmission);
            }
            else {
                makePlayerIdSpacer(sout);
                makePlayerDataSpacer(sout);
                makeEmissionDataSpacer(sout);
            }

            // General track information
            makeTrackDataMsg(sout, theTrack);
        }

        // Complete the description
        const int len = static_cast<int>(sout.str().size());
        msg = new char[len+1];
        base::lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void TabLogger::LogPassiveTrack::captureData()
{
    if (thePlayer != nullptr) {
        pos = thePlayer->getPosition();
        vel = thePlayer->getVelocity();
        angles = thePlayer->getEulerAngles();
    }
    if (theEmission != nullptr) {
        // The emission's ownship is our target!
        if (theEmission->getOwnship() != nullptr) {
            tgtPos = theEmission->getOwnship()->getPosition();
            tgtVel = theEmission->getOwnship()->getVelocity();
            tgtAngles = theEmission->getOwnship()->getEulerAngles();
        }
    }
}

} // End simulation namespace
} // End oe namespace
