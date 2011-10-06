//------------------------------------------------------------------------------
// Classes: Weapon
//------------------------------------------------------------------------------
#if !defined(__WEAPON_H_0F20AFBD_1DC4_420f_B9BC_58C621BB3750__)
#define __WEAPON_H_0F20AFBD_1DC4_420f_B9BC_58C621BB3750__

#include "openeaagles/simulation/Player.h"

namespace Eaagles {
   namespace Basic { class Angle; class Distance; class String; class Time; }

namespace Simulation {
   class Designator;
   class Stores;
   class Track;

//==============================================================================
// Class: Weapon
// Description: Abstract class for all weapon types
//
// Form name: Weapon
// Slots:
//    released      <Basic::Number>    ! Weapon has been released (default: false)
//    failed        <Basic::Number>    ! Weapon failed (e.g., reasonableness Test) (default: false)
//    power         <Basic::Number>    ! Weapon power flag (default: true)
//    hang          <Basic::Number>    ! Will be a hung store (default: false)
//    hung          <Basic::Number>    ! Hung store flag (default: false)
//
//    maxTOF        <Basic::Time>      ! max time of flight (seconds or Basic::Time) (default: 60.0f)
//    maxTOF        <Basic::Number>    ! max time of flight (seconds or Basic::Time)
//
//    tsg           <Basic::Time>      ! time to start guidance (seconds or Basic::Time) (default: 9999.0f)
//    tsg           <Basic::Number>    ! time to start guidance (seconds or Basic::Time)
//
//    maxBurstRng   <Basic::Distance>  ! max burst range (meters or Basic::Distance) (default: 500.0f)
//    maxBurstRng   <Basic::Number>    ! max burst range (meters or Basic::Distance)
//
//    lethalRange   <Basic::Distance>  ! lethal range (meters or Basic::Distance) (default: 50.0f)
//    lethalRange   <Basic::Number>    ! lethal range (meters or Basic::Distance)
//
//    sobt          <Basic::Time>      ! start-of-burn time (seconds or Basic::Time) (default: 9999.0f)
//    sobt          <Basic::Number>    ! start-of-burn time (seconds or Basic::Time)
//
//    eobt          <Basic::Time>      ! end-of-burn time (seconds or Basic::Time) (default: 0.0f)
//    eobt          <Basic::Number>    ! end-of-burn time (seconds or Basic::Time)
//
//    maxGimbal     <Basic::Angle>     ! max gimbal angle (default: 30.0f * D2RCC)
//    tgtPos        <Basic::List>      ! TEST target position [ n e d ] (meters) (default: 0, 0, 0)
//    weaponID      <Basic::Number>    ! Weapon type ID (optional: user defined number) (default: 0)
//    dummy         <Basic::Number>    ! Dummy store (launch, but don't flyout or detonate) (default: false)
//    jettisonable  <Basic::Number>    ! Weapon can be jettisoned (default: true)
//    testTgtName   <Basic::String>    ! TEST only: target player name (default: 0)
//
// Events:
//      DESIGNATOR_EVENT    Designator (e.g., LASER) event
//      JETTISON_EVENT      Jettison event
//
//
//
// Release Sequences:
//
//    Both release() and prerelease() return a pre-ref()'d pointer to the flyout
//    weapon, or zero if unable to (pre)release.
//
//    Standard release sequence:
//       --- Simply releasing the weapon using release(); the 'releaseHold'
//       flag is always false.
//
//        Initial weapon      Flyout weapon 
//            (mode)             (mode)                   Note
//       ------------------   ------------------   -------------------------
//       (INACTIVE)           <doesn't exit>       1) Weapon is inactive; attached to the launch vehicle
//
//       (LAUNCHED)           (PRE_RELEASE)        2) release() is called; the flyout weapon
//                                                 is created and added to player list
//
//       (LAUNCHED)           (ACTIVE)             3) At the end of the next time-critcal frame --
//                                                 the flyout has gone active
//
//       (LAUNCHED)           (DETONATED)          4) After the flyout weapon has detonated
//
//
//    Pre-release sequence:
//       --- Create the flyout weapon, using prerelease(), which will add the weapon
//       to the simulation player list, but will not go active until release() is
//       later called. (see 'releaseHold' flag).
//
//        Initial weapon      Flyout weapon 
//       (mode/releaseHold)   (mode/releaseHold)              Note
//       ------------------   ------------------   -------------------------
//       (INACTIVE/false)     <doesn't exit>       1) Weapon is inactive; attached to the launch vehicle
//
//       (INACTIVE/true)      (PRE_RELEASE/true)   2) prerelease() was called; the flyout weapon
//                                                 was created and added to player list, but 'release
//                                                 hold' will prevent it from going active and it will
//                                                 remain attached to launch vehicle.
//
//       (LAUNCHED/false)     (PRE_RELEASE/false)  3) release() is later called and the 'release
//                                                 hold' flag is cleared, which will allow it
//                                                 to go active
//
//       (LAUNCHED/false)     (ACTIVE/false)       4) At the end of the next Time/Critcal frame --
//                                                 the flyout has gone active
//
//       (LAUNCHED/false)    (DETONATED/false)     5) After the flyout weapon has detonated
//
//
// Weapon pointer functions:
//
//    These all return pre-ref()'d pointers!!
//
//       getPointer()      -- After prerelease() and/or release(), returns the flyout weapon;
//                            Before prerelease() or release(), returns the initial weapon
//
//       getInitialWeapon() -- Returns the initial weapon
//
//       getFlyoutWeapon() -- After prerelease() and/or release(), returns the flyout weapon;
//                            Before prerelease() or release(), returns zero.
//
// Notes: 
//
// 1) On reset(), the flyout weapon is set to the DELETE_REQUEST mode, which
//    will remove it from the player list.
//
// 2) The Player (base class) slot 'initPosition' is used to locate the weapon
//    on its launch vehicle, where [ x y z ] is the weapon location (meters)
//    in the launch vehicles coordinate system.
//
// 3) When a weapon is copied or cloned, the launcher and station are set to zero.
//
// GUID: {0F20AFBD-1DC4-420f-B9BC-58C621BB3750}
//==============================================================================
class Weapon : public Player  
{
    DECLARE_SUBCLASS(Weapon,Player)

public:
    // Weapon Categories (bits) These bits can be bitwise OR'd together
    enum {
       GRAVITY = 0x01,
       CLUSTER = 0x02,
       GUIDED  = 0x04,
       MISSILE = 0x08
    };

    enum Detonation { /* from DIS */
        DETONATE_OTHER                       = 0,
        DETONATE_ENTITY_IMPACT               = 1,
        DETONATE_ENTITY_PROXIMATE_DETONATION = 2,
        DETONATE_GROUND_IMPACT               = 3,
        DETONATE_GROUND_PROXIMATE_DETONATION = 4,
        DETONATE_DETONATION                  = 5,
        DETONATE_NONE                        = 6,
    };

public:
   Weapon();

   Weapon* getPointer();                           // Pre-ref()'d pointer to the initial or fly-out based on mode
   const Weapon* getPointer() const;               // Pre-ref()'d pointer to the initial or fly-out based on mode (const version)

   bool isWeaponID(const int n) const;             // True if weapon type IDs match
   int getWeaponID() const;                        // Weapon type ID number

   bool isCategory(const int testCategory) const;  // Returns true if the weapon is a member of the test category
   virtual int getCategory() const =0;             // Weapon's category (see category bits)
   virtual const char* getDescription() const =0;  // Weapon's description 
   virtual const char* getNickname() const =0;     // Weapon's nick name (short description)

   Stores* getLauncher();                          // Our launcher, if any
   const Stores* getLauncher() const;              // Our launcher, if any (const version)
   unsigned int getStation() const;                // Station index (number)

   bool isReleased() const;                        // True if  the weapon has been released
   bool isPowerOn() const;                         // Weapon power flag
   bool isBlocked() const;                         // Blocked weapon flag (can not be released if true)
   bool isJettisonable() const;                    // True if the weapon can be jettisioned
   bool isJettisoned() const;                      // True if the weapon has been jettisioned
   bool isFailed() const;                          // True if the weapon has failed
   bool isHung() const;                            // True if the weapon is hung 
   bool getWillHang() const;                       // True if the weapon will hang on release
   bool isDummy() const;                           // True if this is a dummy weapon (someone else with fly it out)

   LCreal getTOF() const;                          // Time Of Flight (seconds) since release
   LCreal getMaxTOF() const;                       // Max TOF (seconds)
   LCreal getTSG() const;                          // Time-to-Start guidance (seconds since release)
   LCreal getSOBT() const;                         // Start-Of-Burn time (seconds since release)
   LCreal getEOBT() const;                         // End-Of-Burn time (seconds since release)
   virtual bool isGuidanceEnabled() const;                  // Is weapon guidance enabled?
   virtual bool isEngineBurnEnabled() const;                // Weapon engine (rocket) on
   LCreal getMaxBurstRng() const;                  // Max burst range (meters) -- most players will be damaged within this range
   LCreal getLethalRange() const;                  // Lethal range (meters) -- most players will be killed within this range
   LCreal getMaxGimbalAngle() const;               // Max gimbal angle (radians)

   Player* getLaunchVehicle();                     // Pointer to the player that launched us
   const Player* getLaunchVehicle() const;         // Pointer to the player that launched us (const version)

   Detonation getDetonationResults() const;        // Detonation result code (see 'Detonation' enum)
   LCreal getDetonationRange() const;              // Range to target at detonation (meters)
   const osg::Vec3& getDetonationLocation() const; // Location of detonation in target player's coord (meters)

   bool isTargetPositionValid() const;             // True if we have the target position and is it valid

   const osg::Vec3& getTargetPosition() const;     // Returns the target position (meters -- NED from simulation ref point)

   Track* getTargetTrack();                                 // Our target track, if any
   const Track* getTargetTrack() const;                     // Our target track, if any (const version)

   Player* getTargetPlayer();                               // Our target player, if any
   const Player* getTargetPlayer() const;                   // Our target player, if any (const version)

   Weapon* getFlyoutWeapon();                      // Pre-ref()'d pointer to the fly-out weapon 
   const Weapon* getFlyoutWeapon() const;          // Pre-ref()'d pointer to the fly-out weapon (const version)

   Weapon* getInitialWeapon();                     // Pre-ref()'d pointer to the initial weapon 
   const Weapon* getInitialWeapon() const;         // Pre-ref()'d pointer to the initial weapon (const version)

   unsigned short getReleaseEventID() const;       // Release event ID (to help match weapon launch and detonation events)
   bool isReleaseHold() const;                     // Is weapon is holding in PRE_RELEASE mode?


   // Sets a pointer to the launcher and our station number 
   virtual bool setLauncher(Stores* const launcher, const unsigned int station);

   // Sets a pointer to the target track -- 
   // -- if 'posTrkEnb' is true, we'll follow the target track's position
   virtual bool setTargetTrack(Track* const trk, const bool posTrkEnb);

   // Sets the target position (meters -- NED from simulation ref point)
   virtual bool setTargetPosition(const osg::Vec3& newTgtPos);

   // Sets the target position valid flag
   virtual bool setTargetPositionValid(const bool b);

   // Sets a pointer to the target player -- 
   // -- if 'posTrkEnb' is true, we'll follow the target player's position
   virtual bool setTargetPlayer(Player* const tgt, const bool posTrkEnb);

   virtual bool setPower(const bool f);                     // Sets the weapon power flag
   virtual bool setBlocked(const bool b);                   // Sets the weapon blocked flag
   virtual bool setJettisonable(const bool f);              // Sets the jettision enable flag
   virtual bool setWillHang(const bool f);                  // Sets the 'will' hang flag
   virtual bool setDummy(const bool f);                     // Sets the dummy weapon flag
   virtual bool setLaunchVehicle(Player* const lch);        // Sets the pointer to the player that launched us
   virtual bool setDetonationResults(const Detonation dr);  // Sets the detonation result code (see 'Detonation' enum)
   virtual bool setDetonationLocation(const osg::Vec3&);    // Sets the detonation location in target player's coord (meters)
   virtual bool setMaxBurstRng(const LCreal v);             // Sets the max burst range (meters)
   virtual bool setLethalRange(const LCreal v);             // Sets the lethal range (meters)
   virtual bool setMaxGimbalAngle(const LCreal v);          // Sets the max gimbal angle( radians)
   virtual bool setWeaponID(const int n);                   // Sets the weapon's type ID number 

   // Check local players for the effects of the detonation
   virtual void checkDetonationEffect();

   // Computes and sets 'loc' to our location relative to the target player, 'tgt'
   virtual bool computeTargetLocation(osg::Vec3* const loc, const Player* const tgt);

   // prerelease() -- prerelease this weapon.
   //    Returns a point to the flyout weapon player, which is still
   //    in release hold.
   virtual Weapon* prerelease();

   // release() -- release this weapon
   //    Returns a point to the flyout weapon player
   virtual Weapon* release();

   // Event handlers
   virtual bool onDesignatorEvent(const Designator* const msg);
   virtual bool onJettisonEvent();

   // Slot functions
   virtual bool setSlotReleased(const Basic::Number* const p);
   virtual bool setSlotFailed(const Basic::Number* const p);
   virtual bool setSlotPower(const Basic::Number* const p);
   virtual bool setSlotWillHang(const Basic::Number* const p);
   virtual bool setSlotHung(const Basic::Number* const p);
   virtual bool setSlotMaxTOF(const Basic::Time* const p);
   virtual bool setSlotMaxTOF(const Basic::Number* const p);
   virtual bool setSlotTSG(const Basic::Time* const p);
   virtual bool setSlotTSG(const Basic::Number* const p);
   virtual bool setSlotMaxBurstRng(const Basic::Distance* const p);
   virtual bool setSlotMaxBurstRng(const Basic::Number* const p);
   virtual bool setSlotLethalRange(const Basic::Distance* const p);
   virtual bool setSlotLethalRange(const Basic::Number* const p);
   virtual bool setSlotSOBT(const Basic::Time* const p);
   virtual bool setSlotSOBT(const Basic::Number* const p);
   virtual bool setSlotEOBT(const Basic::Time* const p);
   virtual bool setSlotEOBT(const Basic::Number* const p);
   virtual bool setSlotMaxGimbal(const Basic::Angle* const p);
   virtual bool setSlotTgtPos(const Basic::List* const p);
   virtual bool setSlotWeaponID(const Basic::Number* const p);
   virtual bool setSlotDummy(const Basic::Number* const p);
   virtual bool setSlotJettisonable(const Basic::Number* const p);
   virtual bool setSlotTestTgtName(const Basic::String* const p);

   // Player interface
   virtual unsigned int getMajorType() const;
   virtual bool collisionNotification(Player* const p);
   virtual bool crashNotification();

   // Basic::Component interface
   virtual void updateTC(const LCreal dt = 0.0);
   virtual bool event(const int event, Basic::Object* const obj = 0);
   virtual void reset();
    
protected:
   virtual void weaponGuidance(const LCreal dt);
   virtual void weaponDynamics(const LCreal dt);
   virtual void positionTracking();

   virtual void updateTOF(const LCreal dt);

   virtual void setTOF(const LCreal newTOF);
   virtual bool setMaxTOF(const LCreal v);      // Sets the max TOF (seconds)
   virtual bool setTSG(const LCreal v);         // Sets the Time-to-Start guidance (seconds since release)
   virtual bool setSOBT(const LCreal v);        // Sets the "start of burn" time  (seconds since release)
   virtual bool setEOBT(const LCreal v);        // Sets the "End-Of-Burn" time (seconds since release)

   virtual bool setReleased(const bool f);      // Sets the weapon released flag
   virtual bool setHung(const bool f);          // Sets the hung weapon flag
   virtual bool setFailed(const bool f);        // Sets the weapon failed flag

   // Init the flyout weapon's data as the player switches from PRE_RELEASE mode to ACTIVE mode
   virtual void atReleaseInit();

   // Sets a weapon type player to release hold mode
   virtual bool setReleaseHold(const bool f);

   // Sets the weapon jettisoned flag
   virtual bool setJettisoned(const bool f);

   // Sets a pointer to the fly-out weapon
   virtual bool setFlyoutWeapon(Weapon* const p);

   // Sets a pointer to the initial weapon
   virtual bool setInitialWeapon(Weapon* const p);

   // At detonation: compute the location of the detonation relative to the target player
   bool setLocationOfDetonation();

   // Player (protected) interface
   virtual void dynamics(const LCreal  dt = 0.0);

   // Basic::Component protected functions
   virtual bool shutdownNotification();

private:
    void initData();

    static const LCreal DEFAULT_MAX_TGT_RNG;        // meters
    static const LCreal DEFAULT_MAX_TGT_LOS_ERR;    // radians

    SPtr<Weapon>  flyoutWpn;     // Initial weapon: points to the cloned flyout weapon
                                 // Cloned flyout: weapon: points to self

    SPtr<Weapon>  initialWpn;    // Initial weapon: points to self
                                 // Cloned flyout: points to the initial weapon

    osg::Vec3     tgtPos;        // Target Position -- platform coord (NED)
    bool       tgtPosValid;      // If true, target position is valid
    SPtr<Player>  tgtPlayer;     // Target Player
    SPtr<Track>   tgtTrack;      // Target Track
    SPtr<Player>  launchVehicle; // Launching/Releasing Player
    bool       posTrkEnb;        // If true, update tgtPos from the target/track
    LCreal     maxTgtRng;        // Max target range for default tgt selection      (meters)
    LCreal     maxTgtLosErr;     // Max target LOS error for default tgt selection  (radians)
    LCreal     detonationRange;  // Range to target at time of detonation           (meters)
    osg::Vec3  tgtDetLoc;        // Detonation location in target player's coord    (meters)

    SPtr<Stores> launcher;       // Launcher
    int         station;         // Station number (on launcher)

    int         weaponID;        // Weapon type ID (user defined)
    unsigned short eventID;      // Release event ID
    bool        power;           // Weapon power flag
    bool        failed;          // Weapon failed flag
    bool        released;        // Released flag
    bool        releaseHold;     // Weapon is holding in PRE_RELEASE mode
    bool        willHang;        // Weapon will hang (or jam) on release.
    bool        hung;            // Hung (or jammed) weapon flag
    bool        blocked;         // Weapon blocked flag
    bool        canJettison;     // Weapon can be jettisioned.
    bool        jettisoned;      // Weapon has been jettisioned.
    bool        dummyFlg;        // Dummy (launch, but don't flyout or detonate)
    Detonation  results;         // Results of weapon detonation
    SPtr<const Basic::String> tstTgtNam; // Test only: target player name
    
    // ---
    // Default guidance & dynamics parameters
    // ---
    LCreal      tof;            // Current time of flight   (sec)   (tod)
    LCreal      maxTOF;         // max time of flight       (sec)
    LCreal      tsg;            // time to start quidance   (sec)
    LCreal      maxBurstRng;    // max burst range -- most entities are damaged within this range. (meters)
    LCreal      lethalRange;    // lethal range -- most entities are killed within this range.     (meters)
    LCreal      sobt;           // start-of-burn time       (sec)
    LCreal      eobt;           // end-of-burn time         (sec)
    LCreal      maxGimbal;      // max gimbal angle         (radians)
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // !defined(__WEAPON_H_0F20AFBD_1DC4_420f_B9BC_58C621BB3750__)
