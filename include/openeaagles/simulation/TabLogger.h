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
// Description: Very simular to SimLogger, except ASCII output is more verbose and
//              formatted using tab characters
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_TabLogger_H__
#define __Eaagles_Simulation_TabLogger_H__

#include "openeaagles/simulation/SimLogger.h"

namespace Eaagles {

namespace Basic { class Identifier; }

namespace Simulation {

class Weapon;
class Track;
class TrackManager;
class Player;
class Emission;
class Simulation;

//------------------------------------------------------------------------------
// Class:   TabLogger
// Base class:  Basic::Object -> Basic::Component -> Basic::Logger -> SimLogger -> TabLogger
// Description: Simulation Event & Data Logger
// Form name: TabLogger
//------------------------------------------------------------------------------
class TabLogger : public SimLogger
{
    DECLARE_SUBCLASS(TabLogger, SimLogger)

public:

    TabLogger();

    // Basic::Component interface
    virtual void updateTC(const LCreal dt = 0.0f);
    virtual void updateData(const LCreal dt = 0.0);

    //==============================================================================
    // ######### Simulation Log Event Classes #########
    //==============================================================================

    //------------------------------------------------------------------------------
    // Class:   TabLogger::TabLogEvent
    // Form name: TabLogEvent
    // Description: Abstract class for all simulation log events
    //------------------------------------------------------------------------------
    class TabLogEvent : public SimLogger::SimLogEvent {
        DECLARE_SUBCLASS(TabLogEvent,SimLogger::SimLogEvent)

    public:
        TabLogEvent();
        virtual void captureData() =0;
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
    // Base class:  Basic::Object -> Basic::Logger::LogEvent -> SimLogEvent -> TabLogEvent -> LogPlayerData
    // Description: Log player's data event
    // Form name: LogPlayerData
    //------------------------------------------------------------------------------
    class LogPlayerData : public TabLogEvent {
        DECLARE_SUBCLASS(LogPlayerData,TabLogEvent)
    public:
        LogPlayerData(int theType, const Player* const thePlayer);
        LogPlayerData(int theType, const Player* const thePlayer, const Player* const theSource);
        virtual const char* getDescription();
        virtual void captureData();
    private:
        int theType;
        SPtr<const Player> thePlayer;
        SPtr<const Player> theSource;   // source of damage, usually a weapon, always a player
        osg::Vec3 pos;
        osg::Vec3 vel;
        osg::Vec3 angles;
        double    latitude;
        double    longitude;
        LCreal    alpha;
        LCreal    beta;
        LCreal    ias;
        LCreal    mach;
        LCreal    pLoading;
    };
    
    //------------------------------------------------------------------------------
    // Class: TabLogger::LogGunActivity
    // Base class:  Basic::Object -> Basic::Logger::LogEvent -> SimLogEvent -> TabLogEvent -> LogGunActivity
    // Description: Log a gun fire event; Player just fired a burst of 'n' rounds
    // Form name: LogGunActivity
    //------------------------------------------------------------------------------
    class LogGunActivity : public TabLogEvent {
        DECLARE_SUBCLASS(LogGunActivity,TabLogEvent)
    public:
        LogGunActivity(int theType, const Player* const player, const int n);
        virtual const char* getDescription();
        virtual void captureData();
    private:
        int theType;
        SPtr<const Player> thePlayer;
        int rounds;
    };


    //------------------------------------------------------------------------------
    // Class: TabLogger::LogWeaponActivity
    // Base class:  Basic::Object -> Basic::Logger::LogEvent -> SimLogEvent -> TabLogEvent -> LogWeaponActivity
    // Description: Log a detonation event
    //    (i.e., Target, 'tgt', killed by weapon, 'wpn', released from the 'player'.
    // Form name: LogWeaponActivity
    //------------------------------------------------------------------------------
    class LogWeaponActivity : public TabLogEvent {
        DECLARE_SUBCLASS(LogWeaponActivity,TabLogEvent)
    public:
        LogWeaponActivity(const int theType, const Player* const player, const Player* const wpn,
                          const Player* const tgt, const unsigned int detType, const LCreal distance = -1.0f);
        virtual const char* getDescription();
        virtual void captureData();
    private:
        int theType;
        SPtr<const Player> thePlayer;
        SPtr<const Player> theWeapon;
        SPtr<const Player> theTarget;
        SPtr<const Track> theTrack;
        unsigned short eventID;
        unsigned int detType;
        LCreal missDist;
    };


    //------------------------------------------------------------------------------
    // Class: TabLogger::LogActiveTrack
    // Base class:  Basic::Object -> Basic::Logger::LogEvent -> SimLogEvent -> TabLogEvent -> LogActiveTrack
    // Description: Log new, removed, or updated "active" track [not passive]
    // Form name: LogActiveTrack
    //------------------------------------------------------------------------------
    class LogActiveTrack : public TabLogEvent {
        DECLARE_SUBCLASS(LogActiveTrack,TabLogEvent)
    public:
        LogActiveTrack(int theType, const TrackManager* const mgr, const Track* const trk);
        virtual const char* getDescription();
        virtual void captureData();
    private:
        int theType;
        SPtr<const TrackManager> theManager;
        SPtr<const Track> theTrack;
        SPtr<const Player> thePlayer;
        SPtr<const Emission> theEmission;
        osg::Vec3 pos;
        osg::Vec3 vel;
        osg::Vec3 angles;
        osg::Vec3 tgtPos;
        osg::Vec3 tgtVel;
        osg::Vec3 tgtAngles;
        LCreal sn;              // Signal/Noise
    };


    //------------------------------------------------------------------------------
    // Class: TabLogger::LogPassiveTrack
    // Base class:  Basic::Object -> Basic::Logger::LogEvent -> SimLogEvent -> TabLogEvent -> LogPassiveTrack
    // Description: Log passive track event (header, new, updated, removed) [RWR]
    // Form name: LogPassiveTrack
    //------------------------------------------------------------------------------
    class LogPassiveTrack : public TabLogEvent {
        DECLARE_SUBCLASS(LogPassiveTrack,TabLogEvent)
    public:
        LogPassiveTrack(int theType, const TrackManager* const mgr, const Track* const trk);
        virtual const char* getDescription();
        virtual void captureData();
    private:
        int theType;
        SPtr<const TrackManager> theManager;
        SPtr<const Track> theTrack;
        SPtr<const Player> thePlayer;
        SPtr<const Emission> theEmission;
        osg::Vec3 pos;
        osg::Vec3 vel;
        osg::Vec3 angles;
        osg::Vec3 tgtPos;
        osg::Vec3 tgtVel;
        osg::Vec3 tgtAngles;
        LCreal sn;              // Signal/Noise
    };
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
