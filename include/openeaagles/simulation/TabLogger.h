//------------------------------------------------------------------------------
// Classes:
//      TabLogger                     -- Simulation Event logger
//      TabLogger::TabLogEvent        -- Abstract simulation log event
//      TabLogger::LogPlayerData      -- Log Player Data event (header, new, update, removed)
//      TabLogger::LogWeaponActivity  -- Weapon release, detonation (or KILL, but it is not in use now.)
//      TabLogger::LogGunActivity     -- Gun was fired
//      TabLogger::LogActiveTrack     -- Log active (radar) track event (header, new, update, removed)
//      TabLogger::LogPassiveTrack    -- Log passive (rwr)  track event (header, new, update, removed)
//
// Description: Very similar to SimLogger, except ASCII output is more verbose and
//              formatted using tab characters
//------------------------------------------------------------------------------
#ifndef __oe_simulation_TabLogger_H__
#define __oe_simulation_TabLogger_H__

#include "openeaagles/simulation/SimLogger.h"

namespace oe {

namespace base { class Identifier; }

namespace simulation {

class Weapon;
class Track;
class TrackManager;
class Player;
class Emission;
class Simulation;

//------------------------------------------------------------------------------
// Class:   TabLogger
// Base class:  base::Object -> base::Component -> base::Logger -> SimLogger -> TabLogger
// Description: Simulation Event & Data Logger
// Factory name: TabLogger
//------------------------------------------------------------------------------
class TabLogger : public SimLogger
{
    DECLARE_SUBCLASS(TabLogger, SimLogger)

public:

    TabLogger();

    void updateTC(const double dt = 0.0) override;
    void updateData(const double dt = 0.0) override;

    //==============================================================================
    // ######### Simulation Log Event Classes #########
    //==============================================================================

    //------------------------------------------------------------------------------
    // Class:   TabLogger::TabLogEvent
    // Factory name: TabLogEvent
    // Description: Abstract class for all simulation log events
    //------------------------------------------------------------------------------
    class TabLogEvent : public SimLogger::SimLogEvent {
        DECLARE_SUBCLASS(TabLogEvent,SimLogger::SimLogEvent)

    public:
        TabLogEvent();
    protected:
        std::ostream& makeTimeHdr(std::ostream& sout);
        std::ostream& makeTimeMsg(std::ostream& sout);

        std::ostream& makePlayerIdSpacer(std::ostream& sout);
        std::ostream& makePlayerIdHdr(std::ostream& sout);
        std::ostream& makePlayerIdMsg(std::ostream& sout, const Player* const player);

        std::ostream& makePlayerDataSpacer(std::ostream& sout);
        std::ostream& makePlayerDataHdr(std::ostream& sout);
        std::ostream& makePlayerDataMsg(std::ostream& sout, osg::Vec3 pos0, osg::Vec3 vel0, osg::Vec3 angles0);

        std::ostream& makePlayerLatLonSpacer(std::ostream& sout);
        std::ostream& makePlayerLatLonHdr(std::ostream& sout);
        std::ostream& makePlayerLatLonMsg(std::ostream& sout, double theLat, double theLong);

        std::ostream& makePlayerDamageSpacer(std::ostream& sout);
        std::ostream& makePlayerDamageHdr(std::ostream& sout);
        std::ostream& makePlayerDamageMsg(std::ostream& sout, const Player* const player, bool checkKillOverride);

        std::ostream& makeTrackDataHdr(std::ostream& sout);
        std::ostream& makeTrackDataMsg(std::ostream& sout, const Track* const trk);

        std::ostream& makeEmissionDataSpacer(std::ostream& sout);
        std::ostream& makeEmissionDataHdr(std::ostream& sout);
        std::ostream& makeEmissionDataMsg(std::ostream& sout, const Emission* const em);
    };


    //------------------------------------------------------------------------------
    // Class:   TabLogger::LogPlayerData
    // Base class:  base::Object -> base::Logger::LogEvent -> SimLogEvent -> TabLogEvent -> LogPlayerData
    // Description: Log player's data event
    // Factory name: LogPlayerData
    //------------------------------------------------------------------------------
    class LogPlayerData : public TabLogEvent {
        DECLARE_SUBCLASS(LogPlayerData,TabLogEvent)
    public:
        LogPlayerData(int theType, const Player* const thePlayer);
        LogPlayerData(int theType, const Player* const thePlayer, const Player* const theSource);
        const char* getDescription() override;
        void captureData() override;
    private:
        void initData();
        int theType;
        base::safe_ptr<const Player> thePlayer;
        base::safe_ptr<const Player> theSource;   // source of damage, usually a weapon, always a player
        osg::Vec3 pos;
        osg::Vec3 vel;
        osg::Vec3 angles;
        double    latitude;
        double    longitude;
        double    alpha;
        double    beta;
        double    ias;
        double    mach;
        double    pLoading;
    };

    //------------------------------------------------------------------------------
    // Class: TabLogger::LogGunActivity
    // Base class:  base::Object -> base::Logger::LogEvent -> SimLogEvent -> TabLogEvent -> LogGunActivity
    // Description: Log a gun fire event; Player just fired a burst of 'n' rounds
    // Factory name: LogGunActivity
    //------------------------------------------------------------------------------
    class LogGunActivity : public TabLogEvent {
        DECLARE_SUBCLASS(LogGunActivity,TabLogEvent)
    public:
        LogGunActivity(int theType, const Player* const player, const int n);
        const char* getDescription() override;
        void captureData() override;
    private:
        void initData();
        int theType;
        base::safe_ptr<const Player> thePlayer;
        int rounds;
    };


    //------------------------------------------------------------------------------
    // Class: TabLogger::LogWeaponActivity
    // Base class:  base::Object -> base::Logger::LogEvent -> SimLogEvent -> TabLogEvent -> LogWeaponActivity
    // Description: Log a detonation event
    //    (i.e., Target, 'tgt', killed by weapon, 'wpn', released from the 'player'.
    // Factory name: LogWeaponActivity
    //------------------------------------------------------------------------------
    class LogWeaponActivity : public TabLogEvent {
        DECLARE_SUBCLASS(LogWeaponActivity,TabLogEvent)
    public:
        LogWeaponActivity(const int theType, const Player* const player, const Player* const wpn,
                          const Player* const tgt, const unsigned int detType, const double distance = -1.0f);
        const char* getDescription() override;
        void captureData() override;
    private:
        void initData();
        int theType;
        base::safe_ptr<const Player> thePlayer;
        base::safe_ptr<const Player> theWeapon;
        base::safe_ptr<const Player> theTarget;
        base::safe_ptr<const Track> theTrack;
        unsigned short eventID;
        unsigned int detType;
        double missDist;
    };


    //------------------------------------------------------------------------------
    // Class: TabLogger::LogActiveTrack
    // Base class:  base::Object -> base::Logger::LogEvent -> SimLogEvent -> TabLogEvent -> LogActiveTrack
    // Description: Log new, removed, or updated "active" track [not passive]
    // Factory name: LogActiveTrack
    //------------------------------------------------------------------------------
    class LogActiveTrack : public TabLogEvent {
        DECLARE_SUBCLASS(LogActiveTrack,TabLogEvent)
    public:
        LogActiveTrack(int theType, const TrackManager* const mgr, const Track* const trk);
        const char* getDescription() override;
        void captureData() override;
    private:
        void initData();
        int theType;
        base::safe_ptr<const TrackManager> theManager;
        base::safe_ptr<const Track> theTrack;
        base::safe_ptr<const Player> thePlayer;
        base::safe_ptr<const Emission> theEmission;
        osg::Vec3 pos;
        osg::Vec3 vel;
        osg::Vec3 angles;
        osg::Vec3 tgtPos;
        osg::Vec3 tgtVel;
        osg::Vec3 tgtAngles;
        double sn;              // Signal/Noise
    };


    //------------------------------------------------------------------------------
    // Class: TabLogger::LogPassiveTrack
    // Base class:  base::Object -> base::Logger::LogEvent -> SimLogEvent -> TabLogEvent -> LogPassiveTrack
    // Description: Log passive track event (header, new, updated, removed) [RWR]
    // Factory name: LogPassiveTrack
    //------------------------------------------------------------------------------
    class LogPassiveTrack : public TabLogEvent {
        DECLARE_SUBCLASS(LogPassiveTrack,TabLogEvent)
    public:
        LogPassiveTrack(int theType, const TrackManager* const mgr, const Track* const trk);
        const char* getDescription() override;
        void captureData() override;
    private:
        void initData();
        int theType;
        base::safe_ptr<const TrackManager> theManager;
        base::safe_ptr<const Track> theTrack;
        base::safe_ptr<const Player> thePlayer;
        base::safe_ptr<const Emission> theEmission;
        osg::Vec3 pos;
        osg::Vec3 vel;
        osg::Vec3 angles;
        osg::Vec3 tgtPos;
        osg::Vec3 tgtVel;
        osg::Vec3 tgtAngles;
        double sn;              // Signal/Noise
    };
};

} // End simulation namespace
} // End oe namespace

#endif
