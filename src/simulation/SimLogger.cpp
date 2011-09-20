
#include "openeaagles/simulation/SimLogger.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/NetIO.h"
#include "openeaagles/simulation/Nib.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/TrackManager.h"

#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Times.h"
#include <string>
#include <sstream>
#include <stdio.h>

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

// ---
// SIMLOGEVENT_B -- 
// use this macro in place of IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS() for classes derived from SimLogger::SimLogEvent
// ---
#define SIMLOGEVENT_B(ThisType,FORMNAME)                                        \
    IMPLEMENT_PARTIAL_SUBCLASS(SimLogger::ThisType,FORMNAME)                    \
    EMPTY_SLOTTABLE(SimLogger::ThisType)                                        \
    SimLogger::ThisType::ThisType(const ThisType& org)                          \
    {                                                                           \
        STANDARD_CONSTRUCTOR()                                                    \
        copyData(org,true);                                                     \
    }                                                                           \
    SimLogger::ThisType::~ThisType()                                            \
    {                                                                           \
       STANDARD_DESTRUCTOR()                                                      \
    }                                                                           \
    SimLogger::ThisType& SimLogger::ThisType::operator=(const ThisType& org)    \
    {                                                                           \
        if (this != &org) copyData(org,false);                                  \
        return *this;                                                           \
    }                                                                           \
    Basic::Object* SimLogger::ThisType::clone() const                           \
    {                                                                           \
        return new SimLogger::ThisType(*this);                                  \
    }



namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class SimLogger
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(SimLogger,"SimLogger")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(SimLogger)
   "timeline",             // 1: Source of the time line { UTC, SIM or EXEC } default: UTC)  (Basic::Identifier)
   "includeUtcTime",       // 2: record UTC time for data  ( default: true)  (Basic::Number)
   "includeSimTime",       // 3: record SIM time for data  ( default: true)  (Basic::Number)
   "includeExecTime",      // 4: record EXEC time for data ( default: true)  (Basic::Number)
END_SLOTTABLE(SimLogger)

// Map slot table to handles 
BEGIN_SLOT_MAP(SimLogger)
   ON_SLOT(1,  setSlotTimeline,   Basic::Identifier)
   ON_SLOT(2,  setSlotIncludeUtcTime,   Basic::Number)
   ON_SLOT(3,  setSlotIncludeSimTime,   Basic::Number)
   ON_SLOT(4,  setSlotIncludeExecTime,  Basic::Number)
END_SLOT_MAP()


// Constructor: 
SimLogger::SimLogger() : seQueue(MAX_QUEUE_SIZE)
{
    STANDARD_CONSTRUCTOR()
    time = 0;
    execTime = 0; 
    simTime = 0;  
    utcTime = 0;  
    timeline = UTC;
    includeUtcTime = true;
    includeSimTime = true;
    includeExecTime = true;
}

// Copy Constructor
SimLogger::SimLogger(const SimLogger& org) : seQueue(MAX_QUEUE_SIZE)
{ 
    STANDARD_CONSTRUCTOR()
    copyData(org,true);
}

// Destructor
SimLogger::~SimLogger()
{
   STANDARD_DESTRUCTOR()
}

// Copy operator
SimLogger& SimLogger::operator=(const SimLogger& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

// Clone
Basic::Object* SimLogger::clone() const
{
    return new SimLogger(*this);
}

// copyData() -- copy member data
void SimLogger::copyData(const SimLogger& org, const bool)
{
    BaseClass::copyData(org);
    time = org.time;
    execTime = org.execTime;
    simTime = org.simTime;
    utcTime = org.utcTime; 
    timeline = org.timeline;
    includeUtcTime = org.includeUtcTime;
    includeSimTime = org.includeSimTime;
    includeExecTime = org.includeExecTime;
}

// deleteData() -- delete member data
void SimLogger::deleteData()
{
}

//------------------------------------------------------------------------------
// updateTC() -- Update the simulation log time
//------------------------------------------------------------------------------
void SimLogger::updateTC(const LCreal dt)
{
    BaseClass::updateTC(dt);
    
    // Try to find our simulation
    Simulation* const sim = (Simulation*) findContainerByType(typeid(Simulation));
    
    // Update the simulation time
    if (sim != 0) {
        // Use the (UTC, SIM or EXEC) time 
        if (getTimeline() == UTC) time = sim->getSysTimeOfDay();
        else if (getTimeline() == SIM) time = sim->getSimTimeOfDay();
        else time = sim->getExecTimeSec();

        execTime = sim->getExecTimeSec();          // Executive time since start (seconds)
        simTime = sim->getSimTimeOfDay();          // Simulated time of day (seconds)
        utcTime = sim->getSysTimeOfDay();          // Computer system's time of day (seconds)

    }
    else {
        // or keep your own time
        time += dt;
        execTime += dt;
    }
}

//------------------------------------------------------------------------------
// updateData() -- During background we'll build & send the descriptions of the
//                 simulation events to the log file.
//------------------------------------------------------------------------------
void SimLogger::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);
    
    // Log the simulation events
    SimLogEvent* event = seQueue.get();
    while (event != 0) {
        const char* const p = event->getDescription();
        Basic::Logger::log(p);
        event->unref();

        event = seQueue.get();
    }
}


//------------------------------------------------------------------------------
// log() -- Log a simulation event -- this may be from the T/C thread so just
//          set the time and queue it up for processing by updateData().
//------------------------------------------------------------------------------
void SimLogger::log(LogEvent* const event)
{
    SimLogEvent* const simEvent = dynamic_cast<SimLogEvent*>(event);
    if (simEvent != 0) {
        simEvent->ref();
        simEvent->setTime(time);
        simEvent->setExecTime(execTime);
        simEvent->setUtcTime(utcTime);
        simEvent->setSimTime(simTime);
        simEvent->setPrintExecTime(includeExecTime);
        simEvent->setPrintUtcTime(includeUtcTime);
        simEvent->setPrintSimTime(includeSimTime);
        simEvent->captureData();

        seQueue.put(simEvent);
    }
    else {
        Basic::Logger::log(event);
    }
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the logger's time line
bool SimLogger::setTimeline(const TSource ts)
{
    timeline = ts;
    return true;
}

bool SimLogger::setIncludeUtcTime(const bool b)
{
    includeUtcTime = b;
    return true;
}

bool SimLogger::setIncludeSimTime(const bool b)
{
    includeSimTime = b;
    return true;
}

bool SimLogger::setIncludeExecTime(const bool b)
{
    includeExecTime = b;
    return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// Sets the source of the time ( UTM, SIM or EXEC )
bool SimLogger::setSlotTimeline(const Basic::Identifier* const p)
{
    bool ok = false;
    if (p != 0) {
        if (*p == "EXEC" || *p == "exec") {
            setTimeline( EXEC );
            ok = true;
        }
        else if (*p == "UTC" || *p == "utc") {
            setTimeline( UTC );
            ok = true;
        }
        else if (*p == "SIM" || *p == "sim") {
            setTimeline( SIM );
            ok = true;
        }
    }
    return ok;
}

// whether or not to include UTC time in tabular time message
bool SimLogger::setSlotIncludeUtcTime(const Basic::Number* const num)
{
    return setIncludeUtcTime(num->getBoolean());
}

// whether or not to include SIM time in tabular time message
bool SimLogger::setSlotIncludeSimTime(const Basic::Number* const num)
{
    return setIncludeSimTime(num->getBoolean());
}

// whether or not to include EXEC time in tabular time message
bool SimLogger::setSlotIncludeExecTime(const Basic::Number* const num)
{
    return setIncludeExecTime(num->getBoolean());
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* SimLogger::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& SimLogger::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "timeline: ";
    if (getTimeline() == EXEC) sout << "EXEC";
    else if (getTimeline() == SIM) sout << "SIM";
    else sout << "UTC";
    sout << std::endl;

    sout << "includeExecTime: ";
    if (includeExecTime == true) 
        sout << "true";
    else 
        sout << "false";
    sout << std::endl;

    sout << "includeUtcTime: ";
    if (includeUtcTime == true) 
        sout << "true";
    else 
        sout << "false";
    sout << std::endl;

    sout << "includeSimTime: ";
    if (includeSimTime == true) 
        sout << "true";
    else 
        sout << "false";

    sout << std::endl;


    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}
//==============================================================================
// Class SimLogEvent
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(SimLogger::SimLogEvent,"SimLogEvent")
EMPTY_SLOTTABLE(SimLogger::SimLogEvent)
EMPTY_SERIALIZER(SimLogger::SimLogEvent)

//------------------------------------------------------------------------------
// Constructor(s) & Destructors
//------------------------------------------------------------------------------
SimLogger::SimLogEvent::SimLogEvent()
{
    STANDARD_CONSTRUCTOR()

    time = 0;
    simTime = 0;
    execTime = 0;
    utcTime = 0;
    printUtcTime = false;
    printSimTime = false;
    printExecTime = false;
    msg = 0;
}

SimLogger::SimLogEvent::SimLogEvent(const SimLogEvent& org)
{ 
    STANDARD_CONSTRUCTOR()
    copyData(org,true);
}

// Destructor
SimLogger::SimLogEvent::~SimLogEvent()
{
   STANDARD_DESTRUCTOR()
}

//------------------------------------------------------------------------------
// Copy & Clone
//------------------------------------------------------------------------------
SimLogger::SimLogEvent& SimLogger::SimLogEvent::operator=(const SimLogEvent& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

Basic::Object* SimLogger::SimLogEvent::clone() const
{
    return 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data 
//------------------------------------------------------------------------------
void SimLogger::SimLogEvent::copyData(const SimLogEvent& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) msg = 0;

    time = org.time;
    simTime = org.simTime;
    execTime = org.execTime;
    utcTime = org.utcTime;
    printUtcTime = org.printUtcTime;
    printSimTime = org.printSimTime;
    printExecTime = org.printExecTime;

    if (org.msg != 0) {
        size_t len = strlen(org.msg);
        msg = new char[len+1];
        lcStrcpy(msg,(len+1),org.msg);
    }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SimLogger::SimLogEvent::deleteData()
{
    if (msg != 0) delete[] msg;
    msg = 0;
}

//------------------------------------------------------------------------------
// makeTimeMsg() -- make the time string
//------------------------------------------------------------------------------
std::ostream& SimLogger::SimLogEvent::makeTimeMsg(std::ostream& sout)
{
    char cbuf[16];
    int hh = 0;     // Hours
    int mm = 0;     // Min
    LCreal ss = 0;  // Sec
    Basic::Time::getHHMMSS(LCreal(time), &hh, &mm, &ss);
    sprintf(cbuf, "%02d:%02d:%06.3f", hh, mm, ss);
    sout << cbuf;
    return sout;
}


//------------------------------------------------------------------------------
// makeExecTimeMsg() -- make the time string for EXEC time
//------------------------------------------------------------------------------
std::ostream& SimLogger::SimLogEvent::makeExecTimeMsg(std::ostream& sout)  
{
    char cbuf[16];
    int hh = 0;     // Hours
    int mm = 0;     // Min
    LCreal ss = 0;  // Sec

    // exec time
    Basic::Time::getHHMMSS(LCreal(execTime), &hh, &mm, &ss);
    sprintf(cbuf, "%02d:%02d:%06.3f", hh, mm, ss);
    sout << cbuf;

    return sout;
}


//------------------------------------------------------------------------------
// makeUtcTimeMsg() -- make the time string for UTC time 
//------------------------------------------------------------------------------
std::ostream& SimLogger::SimLogEvent::makeUtcTimeMsg(std::ostream& sout)  
{
    char cbuf[16];
    int hh = 0;     // Hours
    int mm = 0;     // Min
    LCreal ss = 0;  // Sec

    // sim time
    Basic::Time::getHHMMSS(LCreal(utcTime), &hh, &mm, &ss);
    sprintf(cbuf, "%02d:%02d:%06.3f", hh, mm, ss);
    sout << cbuf;

    return sout;
}


//------------------------------------------------------------------------------
// makeSimTimeMsg() -- make the time string for Simulation time
//------------------------------------------------------------------------------
std::ostream& SimLogger::SimLogEvent::makeSimTimeMsg(std::ostream& sout)  
{
    char cbuf[16];
    int hh = 0;     // Hours
    int mm = 0;     // Min
    LCreal ss = 0;  // Sec

    // utc time
    Basic::Time::getHHMMSS(LCreal(simTime), &hh, &mm, &ss);
    sprintf(cbuf, "%02d:%02d:%06.3f", hh, mm, ss);
    sout << cbuf;

    return sout;
}


//------------------------------------------------------------------------------
// makeTabTimeHdr() -- make heading for the time string
//------------------------------------------------------------------------------
std::ostream& SimLogger::SimLogEvent::makeTabTimeHdr(std::ostream& sout)
{
    if(printExecTime)
        sout << "EXEC time" << "\t";

    if(printUtcTime)
        sout << "UTC time" << "\t";

    if(printSimTime)
        sout << "SIM time" << "\t";

    if(!printExecTime && !printUtcTime && !printSimTime)
        sout << "time" << "\t";

    return sout;
}


//------------------------------------------------------------------------------
// makeTimeMsg() -- make the time string
//------------------------------------------------------------------------------
std::ostream& SimLogger::SimLogEvent::makeTabTimeMsg(std::ostream& sout)
{
    if(printExecTime)
    {
        makeExecTimeMsg(sout);
        sout << "\t";
    }

    if(printUtcTime)
    {
        makeUtcTimeMsg(sout);
        sout << "\t";
    }

    if(printSimTime)
    {
        makeSimTimeMsg(sout);
        sout << "\t";
    }

    if(!printExecTime && !printUtcTime && !printSimTime)
    {
        makeTimeMsg(sout);
        sout << "\t";
    }

    return sout;
}

//------------------------------------------------------------------------------
// makePlayerIdMsg() -- creates the player ID message
//------------------------------------------------------------------------------
std::ostream& SimLogger::SimLogEvent::makePlayerIdMsg(std::ostream& sout, const Player* const player)
{
    if (player != 0) {
        sout << "(" << player->getID();
        if (player->isNetworkedPlayer()) {
            const Nib* const pNib = player->getNib();
            sout << "," << (const char*) *pNib->getFederateName();
        }
        sout << ")";
    }
    return sout;
}

//------------------------------------------------------------------------------
// makePlayerIdMsg() -- creates the player ID message
//------------------------------------------------------------------------------
std::ostream& SimLogger::SimLogEvent::makePlayerDataMsg(
            std::ostream& sout,
            osg::Vec3 pos0, osg::Vec3 vel0, osg::Vec3 angles0)
{
    sout << ", position=("    << pos0[0]    << "," << pos0[1]    << "," << pos0[2]    << ")";
    sout << ", velocity=("    << vel0[0]    << "," << vel0[1]    << "," << vel0[2]    << ")";
    sout << ", orientation=(" << (angles0[0] * Basic::Angle::R2DCC) << "," << (angles0[1] * Basic::Angle::R2DCC) << "," << (angles0[2] * Basic::Angle::R2DCC) << ")";
    return sout;
}

//------------------------------------------------------------------------------
// makeTrackDataMsg() -- creates the track data message
//------------------------------------------------------------------------------
std::ostream& SimLogger::SimLogEvent::makeTrackDataMsg(std::ostream& sout, const Track* const trk)
{
    if (trk != 0) {
        sout << "\t Type              = " << trk->getType()            << "\n";
        sout << "\t Range Slang       = " << trk->getRange()           << "\n";
        sout << "\t Range Ground      = " << trk->getGroundRange()     << "\n";
        sout << "\t Azimuth True      = " << trk->getTrueAzimuthD()    << "\n";
        sout << "\t Azimuth Relative  = " << trk->getRelAzimuthD()     << "\n";
        sout << "\t Elevation Angle   = " << trk->getElevationD()      << "\n";
        osg::Vec3 tpos = trk->getPosition();
        sout << "\t Position          =(" << tpos[0]    << "," << tpos[1]    << "," << tpos[2]    << ")"      << "\n";;
        osg::Vec3 tvel = trk->getVelocity();
        sout << "\t Velocity          =(" << tvel[0]    << "," << tvel[1]    << "," << tvel[2]    << ")"      << "\n";

        const RfTrack* const rfTrk = dynamic_cast<const RfTrack*>(trk);
        if (rfTrk != 0) {
           sout << "\t Signal            = " << rfTrk->getAvgSignal()       << "\n";
        }
    }
    return sout;
}

//------------------------------------------------------------------------------
// makeEmissionDataMsg() -- creates the track data message
//------------------------------------------------------------------------------
std::ostream& SimLogger::SimLogEvent::makeEmissionDataMsg(std::ostream& sout, const Emission* const em)
{
    if (em != 0) {
        sout << "\t AOI Azimuth   = " << (Basic::Angle::R2DCC * em->getAzimuthAoi())   << "\n";
        sout << "\t AOI Elevation = " << (Basic::Angle::R2DCC * em->getElevationAoi()) << "\n";
        sout << "\t Frequency     = " << (em->getFrequency())                     << "\n";
        sout << "\t Lambda        = " << (em->getWavelength())                    << "\n";
        sout << "\t Pulse width   = " << (em->getPulseWidth())                    << "\n";
        sout << "\t PRF           = " << (em->getPRF())                           << "\n";
    }
    return sout;
}


//==============================================================================
// Class SimLogger::NewPlayer
//==============================================================================
SIMLOGEVENT_B(NewPlayer,"SimLogger::NewPlayer")
EMPTY_SERIALIZER(SimLogger::NewPlayer)

// Constructor
SimLogger::NewPlayer::NewPlayer(Player* const p)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = p;
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
}

// Copy data function
void SimLogger::NewPlayer::copyData(const NewPlayer& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::NewPlayer::deleteData()
{
    thePlayer = 0;
}

// Get the description
const char* SimLogger::NewPlayer::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " ADDED_PLAYER:\n";
        
        // Print the Player data    
        if (thePlayer != 0) {
            sout << "\tPlayer";
            makePlayerIdMsg(sout,thePlayer);
            makePlayerDataMsg(sout,pos,vel,angles);
            sout << "\n";
        }
        
        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::NewPlayer::captureData()
{
    if (thePlayer != 0) {
        {
            pos = thePlayer->getPosition();
            vel = thePlayer->getVelocity();
            angles = thePlayer->getEulerAngles();
        }
    }
}

//==============================================================================
// Class SimLogger::LogPlayerData
//==============================================================================
SIMLOGEVENT_B(LogPlayerData,"SimLogger::LogPlayerData")
EMPTY_SERIALIZER(SimLogger::LogPlayerData)

// Constructor
SimLogger::LogPlayerData::LogPlayerData(Player* const p)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = p;
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
    alpha = 0;
    beta = 0;
    ias = 0;
}

// Copy data function
void SimLogger::LogPlayerData::copyData(const LogPlayerData& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::LogPlayerData::deleteData()
{
    thePlayer = 0;
}

// Get the description
const char* SimLogger::LogPlayerData::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " PLAYER_DATA:\n";
        
        // Print the Player data    
        if (thePlayer != 0) {
            sout << "\tPlayer";
            makePlayerIdMsg(sout,thePlayer);
            makePlayerDataMsg(sout,pos,vel,angles);
            if (ias >= 0.0f) {
                sout << ", alpha=" << alpha;
                sout << ", beta=" << beta;
                sout << ", ias=" << ias;
            }
            sout << "\n";
        }
        
        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::LogPlayerData::captureData()
{
    if (thePlayer != 0) {
        alpha = -1.0;
        beta = -1.0;
        ias = -1.0;
        {
            pos = thePlayer->getPosition();
            vel = thePlayer->getVelocity();
            angles = thePlayer->getEulerAngles();
            const Player* const p = thePlayer;
            const AirVehicle* const av = dynamic_cast<const AirVehicle*>(p);
            if (av != 0) {
                alpha = av->getAngleOfAttackD();
                beta = av->getSideSlipD();
                ias = av->getCalibratedAirspeed();
            }
        }
    }
}

//==============================================================================
// Class SimLogger::RemovePlayer
//==============================================================================
SIMLOGEVENT_B(RemovePlayer,"SimLogger::RemovePlayer")
EMPTY_SERIALIZER(SimLogger::RemovePlayer)

// Constructor
SimLogger::RemovePlayer::RemovePlayer(Player* const p)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = p;
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
}

// Copy data function
void SimLogger::RemovePlayer::copyData(const RemovePlayer& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::RemovePlayer::deleteData()
{
    thePlayer = 0;
}

// Get the description
const char* SimLogger::RemovePlayer::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " REMOVED_PLAYER:\n";
        
        // Print the Player Data      
        if (thePlayer != 0) {
            sout << "\tPlayer";
            makePlayerIdMsg(sout, thePlayer);
            makePlayerDataMsg(sout,pos,vel,angles);
            sout << "\n";
        }

        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::RemovePlayer::captureData()
{
    if (thePlayer != 0) {
        {
            pos = thePlayer->getPosition();
            vel = thePlayer->getVelocity();
            angles = thePlayer->getEulerAngles();
        }
    }
}

//==============================================================================
// Class SimLogger::WeaponRelease
//==============================================================================
SIMLOGEVENT_B(WeaponRelease,"SimLogger::WeaponRelease")
EMPTY_SERIALIZER(SimLogger::WeaponRelease)

// Constructor
SimLogger::WeaponRelease::WeaponRelease(Player* const lancher, Player* const wpn, Player* const tgt)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = lancher;
    theWeapon = wpn;
    theTarget = tgt;
}

// Copy data function
void SimLogger::WeaponRelease::copyData(const WeaponRelease& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::WeaponRelease::deleteData()
{
    thePlayer = 0;
    theWeapon = 0;
    theTarget = 0;
}

// Get the description
const char* SimLogger::WeaponRelease::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " WEAPON_RELEASE:";
        
        // Print the Player ID        
        if (thePlayer != 0) {
            sout << " launcher";
            makePlayerIdMsg(sout, thePlayer);
        }
        
        // Print the WPN ID        
        if (theWeapon != 0) {
            sout << " wpn";
            makePlayerIdMsg(sout, theWeapon);
        }
        
        // Print the TGT ID        
        if (theTarget != 0) {
            sout << " tgt";
            makePlayerIdMsg(sout, theTarget);
        }

        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::WeaponRelease::captureData()
{
}

//==============================================================================
// Class SimLogger::GunFired
//==============================================================================
SIMLOGEVENT_B(GunFired,"SimLogger::GunFired")
EMPTY_SERIALIZER(SimLogger::GunFired)

// Constructor
SimLogger::GunFired::GunFired(Player* const lancher, const int n)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = lancher;
    rounds = n;
}

// Copy data function
void SimLogger::GunFired::copyData(const GunFired& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::GunFired::deleteData()
{
    thePlayer = 0;
}

// Get the description
const char* SimLogger::GunFired::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " GUN FIRED:";
        
        // Print the Player ID        
        if (thePlayer != 0) {
            sout << " launcher";
            makePlayerIdMsg(sout, thePlayer);
        }

        sout << ", rounds=" << rounds;

        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::GunFired::captureData()
{
}

//==============================================================================
// Class SimLogger::KillEvent
//==============================================================================
SIMLOGEVENT_B(KillEvent,"SimLogger::KillEvent")
EMPTY_SERIALIZER(SimLogger::KillEvent)

// Constructor
SimLogger::KillEvent::KillEvent(Player* const lancher, Player* const wpn, Player* const tgt)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = lancher;
    theWeapon = wpn;
    theTarget = tgt;
}

// Copy data function
void SimLogger::KillEvent::copyData(const KillEvent& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::KillEvent::deleteData()
{
    thePlayer = 0;
    theWeapon = 0;
    theTarget = 0;
}

// Get the description
const char* SimLogger::KillEvent::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " KILL_EVENT:";
        
        // Print the Player ID        
        if (thePlayer != 0) {
            sout << " launcher";
            makePlayerIdMsg(sout, thePlayer);
        }
        
        // Print the WPN ID        
        if (theWeapon != 0) {
            sout << " wpn";
            makePlayerIdMsg(sout, theWeapon);
        }
        
        // Print the TGT ID        
        if (theTarget != 0) {
            sout << " tgt";
            makePlayerIdMsg(sout, theTarget);
        }
        
        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::KillEvent::captureData()
{
}

//==============================================================================
// Class SimLogger::DetonationEvent
//==============================================================================
SIMLOGEVENT_B(DetonationEvent,"SimLogger::DetonationEvent")
EMPTY_SERIALIZER(SimLogger::DetonationEvent)

// Constructor
SimLogger::DetonationEvent::DetonationEvent(Player* const lancher, Player* const wpn, Player* const tgt, const unsigned int t, const LCreal d)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = lancher;
    theWeapon = wpn;
    theTarget = tgt;
    detType   = t;
    missDist  = d;
}

// Copy data function
void SimLogger::DetonationEvent::copyData(const DetonationEvent& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::DetonationEvent::deleteData()
{
    thePlayer = 0;
    theWeapon = 0;
    theTarget = 0;
}

// Get the description
const char* SimLogger::DetonationEvent::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " WPN_DET_EVENT:";
        
        // Print the Player ID        
        if (thePlayer != 0) {
            sout << " launcher";
            makePlayerIdMsg(sout, thePlayer);
        }
        
        // Print the WPN ID        
        if (theWeapon != 0) {
            sout << " wpn";
            makePlayerIdMsg(sout, theWeapon);
        }
        
        // Print the TGT ID        
        if (theTarget != 0) {
            sout << " tgt";
            makePlayerIdMsg(sout, theTarget);
        }
        
        sout << " type: " << detType;
        sout << " missDist: " << missDist;

        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::DetonationEvent::captureData()
{
}

//==============================================================================
// Class SimLogger::NewTrack
//==============================================================================
SIMLOGEVENT_B(NewTrack,"SimLogger::NewTrack")
EMPTY_SERIALIZER(SimLogger::NewTrack)

// Constructor
SimLogger::NewTrack::NewTrack(TrackManager* const mgr, Track* const trk)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = 0;
    theEmission = 0;
    theManager = mgr;
    theTrack = trk;
    if (theManager != 0) {
        thePlayer = dynamic_cast<const Player*>( theManager->findContainerByType(typeid(Player)) );
    }
    if (theTrack != 0) {
        const RfTrack* const rfTrk = dynamic_cast<const RfTrack*>(trk);
        if (rfTrk != 0) {
            theEmission = rfTrk->getLastEmission();
        }
    }
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
    tgtPos.set(0,0,0);
    tgtVel.set(0,0,0);
    tgtAngles.set(0,0,0);
    sn = 0;
}

// Copy data function
void SimLogger::NewTrack::copyData(const NewTrack& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::NewTrack::deleteData()
{
    theManager = 0;
    theTrack = 0;
    thePlayer = 0;
    theEmission = 0;
}

// Get the description
const char* SimLogger::NewTrack::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " ADDED_TRACK:\n";
        
        // Player information
        if (thePlayer != 0) {
            sout << "\tPlayer";
            makePlayerIdMsg(sout, thePlayer);
            makePlayerDataMsg(sout,pos,vel,angles);
            sout << "\n";
        }
        
        // Target Information 
        if (theEmission != 0) {
            if (theEmission->getTarget() != 0) {
                sout << "\tTarget";
                makePlayerIdMsg(sout, theEmission->getTarget());
                makePlayerDataMsg(sout,tgtPos,tgtVel,tgtAngles);
                sout << "\n";
            }
        }
        
        // General track information
        if (theTrack != 0) {
            makeTrackDataMsg(sout, theTrack);
        }
                
        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::NewTrack::captureData()
{
    if (thePlayer != 0) {
        {
            pos = thePlayer->getPosition();
            vel = thePlayer->getVelocity();
            angles = thePlayer->getEulerAngles();
        }
    }
    if (theEmission != 0) {
        if (theEmission->getTarget() != 0) {
            {
                tgtPos = theEmission->getTarget()->getPosition();
                tgtVel = theEmission->getTarget()->getVelocity();
                tgtAngles = theEmission->getTarget()->getEulerAngles();
            }
        }
    }
}

//==============================================================================
// Class SimLogger::UpdateTrack
//==============================================================================
SIMLOGEVENT_B(UpdateTrack,"SimLogger::UpdateTrack")
EMPTY_SERIALIZER(SimLogger::UpdateTrack)

// Constructor
SimLogger::UpdateTrack::UpdateTrack(TrackManager* const mgr, Track* const trk)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = 0;
    theEmission = 0;
    theManager = mgr;
    theTrack = trk;
    if (theManager != 0) {
        thePlayer = dynamic_cast<const Player*>( theManager->findContainerByType(typeid(Player)) );
    }
    if (theTrack != 0) {
        const RfTrack* const rfTrk = dynamic_cast<const RfTrack*>(trk);
        if (rfTrk != 0) {
            theEmission = rfTrk->getLastEmission();
        }
    }
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
    tgtPos.set(0,0,0);
    tgtVel.set(0,0,0);
    tgtAngles.set(0,0,0);
    sn = 0;
}

// Copy data function
void SimLogger::UpdateTrack::copyData(const UpdateTrack& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::UpdateTrack::deleteData()
{
    theManager = 0;
    theTrack = 0;
    thePlayer = 0;
    theEmission = 0;
}

// Get the description
const char* SimLogger::UpdateTrack::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " UPDATE_TRACK:\n";
        
        // Player information
        if (thePlayer != 0) {
            sout << "\tPlayer";
            makePlayerIdMsg(sout, thePlayer);
            makePlayerDataMsg(sout,pos,vel,angles);
            sout << "\n";
        }
        
        // Target Information 
        if (theEmission != 0) {
            if (theEmission->getTarget() != 0) {
                sout << "\tTarget";
                makePlayerIdMsg(sout, theEmission->getTarget());
                makePlayerDataMsg(sout,tgtPos,tgtVel,tgtAngles);
                sout << "\n";
            }
        }
        
        // General track information
        if (theTrack != 0) {
            makeTrackDataMsg(sout, theTrack);
        }
                
        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::UpdateTrack::captureData()
{
    if (thePlayer != 0) {
        {
            pos = thePlayer->getPosition();
            vel = thePlayer->getVelocity();
            angles = thePlayer->getEulerAngles();
        }
    }
    if (theEmission != 0) {
        if (theEmission->getTarget() != 0) {
            {
                tgtPos = theEmission->getTarget()->getPosition();
                tgtVel = theEmission->getTarget()->getVelocity();
                tgtAngles = theEmission->getTarget()->getEulerAngles();
            }
        }
    }
}
//==============================================================================
// Class SimLogger::RemovedTrack
//==============================================================================
SIMLOGEVENT_B(RemovedTrack,"SimLogger::RemovedTrack")
EMPTY_SERIALIZER(SimLogger::RemovedTrack)

// Constructor
SimLogger::RemovedTrack::RemovedTrack(TrackManager* const mgr, Track* const trk)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = 0;
    theEmission = 0;
    theManager = mgr;
    theTrack = trk;
    if (theManager != 0) {
        thePlayer = dynamic_cast<const Player*>( theManager->findContainerByType(typeid(Player)) );
    }
    if (theTrack != 0) {
        const RfTrack* const rfTrk = dynamic_cast<const RfTrack*>(trk);
        if (rfTrk != 0) {
            theEmission = rfTrk->getLastEmission();
        }
    }
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
    tgtPos.set(0,0,0);
    tgtVel.set(0,0,0);
    tgtAngles.set(0,0,0);
    sn = 0;
}

// Copy data function
void SimLogger::RemovedTrack::copyData(const RemovedTrack& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::RemovedTrack::deleteData()
{
    theManager = 0;
    theTrack = 0;
    thePlayer = 0;
    theEmission = 0;
}

// Get the description
const char* SimLogger::RemovedTrack::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " REMOVE_TRACK:\n";
        
        // Player information
        if (thePlayer != 0) {
            sout << "\tPlayer";
            makePlayerIdMsg(sout, thePlayer);
            makePlayerDataMsg(sout,pos,vel,angles);
            sout << "\n";
        }
        
        // Target Information 
        if (theEmission != 0) {
            if (theEmission->getTarget() != 0) {
                sout << "\tTarget";
                makePlayerIdMsg(sout, theEmission->getTarget());
                makePlayerDataMsg(sout,tgtPos,tgtVel,tgtAngles);
                sout << "\n";
            }
        }
        
        // General track information
        if (theTrack != 0) {
            makeTrackDataMsg(sout, theTrack);
        }
                
        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::RemovedTrack::captureData()
{
    if (thePlayer != 0) {
        {
            pos = thePlayer->getPosition();
            vel = thePlayer->getVelocity();
            angles = thePlayer->getEulerAngles();
        }
    }
    if (theEmission != 0) {
        if (theEmission->getTarget() != 0) {
            {
                tgtPos = theEmission->getTarget()->getPosition();
                tgtVel = theEmission->getTarget()->getVelocity();
                tgtAngles = theEmission->getTarget()->getEulerAngles();
            }
        }
    }
}

//==============================================================================
// Class SimLogger::NewRwrTrack
//==============================================================================
SIMLOGEVENT_B(NewRwrTrack,"SimLogger::NewRwrTrack")
EMPTY_SERIALIZER(SimLogger::NewRwrTrack)

// Constructor
SimLogger::NewRwrTrack::NewRwrTrack(TrackManager* const mgr, Track* const trk)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = 0;
    theEmission = 0;
    theManager = mgr;
    theTrack = trk;
    if (theManager != 0) {
        thePlayer = dynamic_cast<const Player*>( theManager->findContainerByType(typeid(Player)) );
    }
    if (theTrack != 0) {
        const RfTrack* const rfTrk = dynamic_cast<const RfTrack*>(trk);
        if (rfTrk != 0) {
            theEmission = rfTrk->getLastEmission();
        }
    }
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
    tgtPos.set(0,0,0);
    tgtVel.set(0,0,0);
    tgtAngles.set(0,0,0);
    sn = 0;
}

// Copy data function
void SimLogger::NewRwrTrack::copyData(const NewRwrTrack& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::NewRwrTrack::deleteData()
{
    theManager = 0;
    theTrack = 0;
    thePlayer = 0;
    theEmission = 0;
}

// Get the description
const char* SimLogger::NewRwrTrack::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " ADDED_RWR_TRACK:\n";
        
        // Player information
        if (thePlayer != 0) {
            sout << "\tPlayer";
            makePlayerIdMsg(sout, thePlayer);
            makePlayerDataMsg(sout,pos,vel,angles);
            sout << "\n";
        }
        
        // Target Information 
        if (theEmission != 0) {
            if (theEmission->getOwnship() != 0) {
                sout << "\tTarget";
                makePlayerIdMsg(sout, theEmission->getOwnship());
                makePlayerDataMsg(sout,tgtPos,tgtVel,tgtAngles);
                sout << "\n";
            }
            // General emission information
            makeEmissionDataMsg(sout, theEmission);
        }
        
        // General track information
        if (theTrack != 0) {
            makeTrackDataMsg(sout, theTrack);
        }
                
        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::NewRwrTrack::captureData()
{
    if (thePlayer != 0) {
        {
            pos = thePlayer->getPosition();
            vel = thePlayer->getVelocity();
            angles = thePlayer->getEulerAngles();
        }
    }
    if (theEmission != 0) {
        // The emission's ownship is our target!
        if (theEmission->getOwnship() != 0) {
            {
                tgtPos = theEmission->getOwnship()->getPosition();
                tgtVel = theEmission->getOwnship()->getVelocity();
                tgtAngles = theEmission->getOwnship()->getEulerAngles();
            }
        }
    }
}

//==============================================================================
// Class SimLogger::UpdateRwrTrack
//==============================================================================
SIMLOGEVENT_B(UpdateRwrTrack,"SimLogger::UpdateRwrTrack")
EMPTY_SERIALIZER(SimLogger::UpdateRwrTrack)

// Constructor
SimLogger::UpdateRwrTrack::UpdateRwrTrack(TrackManager* const mgr, Track* const trk)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = 0;
    theEmission = 0;
    theManager = mgr;
    theTrack = trk;
    if (theManager != 0) {
        thePlayer = dynamic_cast<const Player*>( theManager->findContainerByType(typeid(Player)) );
    }
    if (theTrack != 0) {
        const RfTrack* const rfTrk = dynamic_cast<const RfTrack*>(trk);
        if (rfTrk != 0) {
            theEmission = rfTrk->getLastEmission();
        }
    }
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
    tgtPos.set(0,0,0);
    tgtVel.set(0,0,0);
    tgtAngles.set(0,0,0);
    sn = 0;
}

// Copy data function
void SimLogger::UpdateRwrTrack::copyData(const UpdateRwrTrack& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::UpdateRwrTrack::deleteData()
{
    theManager = 0;
    theTrack = 0;
    thePlayer = 0;
    theEmission = 0;
}

// Get the description
const char* SimLogger::UpdateRwrTrack::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " UPDATE_RWR_TRACK:\n";
        
        // Player information
        if (thePlayer != 0) {
            sout << "\tPlayer";
            makePlayerIdMsg(sout, thePlayer);
            makePlayerDataMsg(sout,pos,vel,angles);
            sout << "\n";
        }
        
        // Target Information 
        if (theEmission != 0) {
            if (theEmission->getOwnship() != 0) {
                sout << "\tTarget";
                makePlayerIdMsg(sout, theEmission->getOwnship());
                makePlayerDataMsg(sout,tgtPos,tgtVel,tgtAngles);
                sout << "\n";
            }
            // General emission information
            makeEmissionDataMsg(sout, theEmission);
        }
        
        // General track information
        if (theTrack != 0) {
            makeTrackDataMsg(sout, theTrack);
        }
                
        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::UpdateRwrTrack::captureData()
{
    if (thePlayer != 0) {
        {
            pos = thePlayer->getPosition();
            vel = thePlayer->getVelocity();
            angles = thePlayer->getEulerAngles();
        }
    }
    if (theEmission != 0) {
        // The emission's ownship is our target!
        if (theEmission->getOwnship() != 0) {
            {
                tgtPos = theEmission->getOwnship()->getPosition();
                tgtVel = theEmission->getOwnship()->getVelocity();
                tgtAngles = theEmission->getOwnship()->getEulerAngles();
            }
        }
    }
}

//==============================================================================
// Class SimLogger::RemovedRwrTrack
//==============================================================================
SIMLOGEVENT_B(RemovedRwrTrack,"SimLogger::RemovedRwrTrack")
EMPTY_SERIALIZER(SimLogger::RemovedRwrTrack)

// Constructor
SimLogger::RemovedRwrTrack::RemovedRwrTrack(TrackManager* const mgr, Track* const trk)
{
    STANDARD_CONSTRUCTOR()
    thePlayer = 0;
    theEmission = 0;
    theManager = mgr;
    theTrack = trk;
    if (theManager != 0) {
        thePlayer = dynamic_cast<const Player*>( theManager->findContainerByType(typeid(Player)) );
    }
    if (theTrack != 0) {
        const RfTrack* const rfTrk = dynamic_cast<const RfTrack*>(trk);
        if (rfTrk != 0) {
            theEmission = rfTrk->getLastEmission();
        }
    }
    pos.set(0,0,0);
    vel.set(0,0,0);
    angles.set(0,0,0);
    tgtPos.set(0,0,0);
    tgtVel.set(0,0,0);
    tgtAngles.set(0,0,0);
    sn = 0;
}

// Copy data function
void SimLogger::RemovedRwrTrack::copyData(const RemovedRwrTrack& org, const bool)
{
    BaseClass::copyData(org);
}

// Delete data function
void SimLogger::RemovedRwrTrack::deleteData()
{
    theManager = 0;
    theTrack = 0;
    thePlayer = 0;
    theEmission = 0;
}

// Get the description
const char* SimLogger::RemovedRwrTrack::getDescription()
{
    if (msg == 0) {
	
        std::stringstream sout;
        
        // Time & Event message
        makeTimeMsg(sout);
        sout << " REMOVE_RWR_TRACK:\n";
        
        // Player information
        if (thePlayer != 0) {
            sout << "\tPlayer";
            makePlayerIdMsg(sout, thePlayer);
            makePlayerDataMsg(sout,pos,vel,angles);
            sout << "\n";
        }
        
        // Target Information 
        if (theEmission != 0) {
            if (theEmission->getOwnship() != 0) {
                sout << "\tTarget";
                makePlayerIdMsg(sout, theEmission->getOwnship());
                makePlayerDataMsg(sout,tgtPos,tgtVel,tgtAngles);
                sout << "\n";
            }
            // General emission information
            makeEmissionDataMsg(sout, theEmission);
        }
        
        // General track information
        if (theTrack != 0) {
            makeTrackDataMsg(sout, theTrack);
        }
                
        // Complete the description
        int len = (int)sout.str().size();
        msg = new char[len+1];
        lcStrncpy(msg, (len+1), sout.str().c_str(), len);
    }
    return msg;
}

// Capture the data
void SimLogger::RemovedRwrTrack::captureData()
{
    if (thePlayer != 0) {
        {
            pos = thePlayer->getPosition();
            vel = thePlayer->getVelocity();
            angles = thePlayer->getEulerAngles();
        }
    }
    if (theEmission != 0) {
        // The emission's ownship is our target!
        if (theEmission->getOwnship() != 0) {
            {
                tgtPos = theEmission->getOwnship()->getPosition();
                tgtVel = theEmission->getOwnship()->getVelocity();
                tgtAngles = theEmission->getOwnship()->getEulerAngles();
            }
        }
    }
}

} // End Simulation namespace
} // End Eaagles namespace
