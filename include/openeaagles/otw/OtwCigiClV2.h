//------------------------------------------------------------------------------
// Classes: OtwCigiClV2, CigiClV2, CigiClV2Network, OtwModelCigiClV2
//------------------------------------------------------------------------------
#ifndef __OTWCIGICLV2_H__
#define __OTWCIGICLV2_H__

class CigiEntityCtrlV2;
class CigiCompCtrlV2;
class CigiIGCtrlV2;
class CigiLosVectReqV2;    // CGBCGB CIGI_LOS_RANGE_REQUEST* los;          // LOS request packet
class CigiViewCtrlV2;
class CigiViewDefV2;
class CigiArtPartCtrlV2;
class CigiHotReqV2;
class CigiSOFV2;
class CigiHatRespV2;
class CigiHotRespV2;
class CigiLosRespV2;
class CigiCollDetSegRespV2;
class CigiCollDetVolRespV2;
class CigiSensorRespV2;
class CigiIGMsgV2;        // CGBCGB CIGI_IG_RESPONSE_MESSAGE
class CigiHostSession;
class CigiIncomingMsg;
class CigiOutgoingMsg;

#include "openeaagles/simulation/Otw.h"

namespace Eaagles {

   namespace Basic {
      class NetHandler;
   }

   namespace Simulation {
      class AirVehicle;
      class Building;
      class Effects;
      class GroundVehicle;
      class LifeForm;
      class Missile;
      class Player;
      class Ship;
      class SpaceVehicle;
      class Weapon;
   }

namespace Otw {

class CigiClV2;
class OtwModelCigiClV2;
class CigiClV2NetworkSignalProcessing;

//------------------------------------------------------------------------------
// Class:	OtwCigiClV2
// Base class:	Basic::Object -> Component -> Otw -> OtwCigiClV2
//
// Description: Boeing's Common Image Generator Interface (CIGI)
//
// Form name: OtwCigiClV2
// Slots:
//    cigi                 (CigiClV2)         CIGI interface context
//    async                (Number)       True (non-zero) to run in CIGI async mode (default: false - CIGI sync)
//    hideOwnshipModel     (Number)       True to hide the ownship's model (default: false - ownship's model is seen)
//    ownshipModel         (Number)       Ownship's model ID
//    mslTrailModel        (Number)       Missile Trail" effect model ID
//    smokePlumeModel      (Number)       Smoke Plume" effect model ID
//    airExplosionModel    (Number)       Air Explosion" effect model ID
//    groundExplosionModel (Number)       Ground Explosion" effect model ID
//    shipWakeModel        (Number)       Ship Wake" effect model ID
//
// Note: In the async mode, the sendCigiData() function, which sends the CIGI
// packets to the CigiClV2 handler, is called by our frameSync() funciton in the
// R/T thread.  In the sync mode, the sendCigiData() function is called by the
// startOfFrame() callback (i.e., sync'd with the IG).
//
//------------------------------------------------------------------------------
class OtwCigiClV2 : public Simulation::Otw
{
    DECLARE_SUBCLASS(OtwCigiClV2,Simulation::Otw)

public:
   static const unsigned int NUM_BUFFERS = 2;

public:
   OtwCigiClV2();

   // Is a LOS request pending?
   virtual bool isLosRequestPending() const { return (losReqId != losRespId); }

   // Is an elevation request pending?
   virtual bool isElevationRequestPending() const { return elevReqFlg; }

   bool isASyncMode() const { return asyncMode; }     // True if running in CIGI async mode
   bool isSyncMode() const { return !asyncMode; }     // True if running in CIGI sync mode
   virtual bool setASyncMode(const bool f);           // Sets the CIGI async mode flag

   bool isOwnshipModelHidden() const { return hideOwn; }                   // True if the ownship's model is hidden
   virtual bool setHideOwnshipModel(const bool f);                         // Sets the hide ownship flag

   unsigned short getOwnshipModelID() const { return cmtOwnship; }         // Ownship's model ID
   virtual bool setOwnshipModelId(const unsigned short);                   // Ownship's model ID

   unsigned short getMslTrailModelID() const { return cmtMslTrail; }       // "Missile Trail" effect model ID
   virtual bool setMslTrailModelId(const unsigned short);                  // "Missile Trail" effect model ID

   unsigned short getSmokePlumeModelID() const { return cmtSmokePlume; }   // "Smoke Plume" effect model ID
   virtual bool setSmokePlumeModelId(const unsigned short);                // "Smoke Plume" effect model ID

   unsigned short getAirExplosionModelID() const { return cmtAirExplosion; }  // "Air Explosion" effect model ID
   virtual bool setAirExplosionModelId(const unsigned short);              // "Air Explosion" effect model ID

   unsigned short getGroundExplosionModelID() const { return cmtGroundExplosion; }  // "GroundE xplosion" effect model ID
   virtual bool setGroundExplosionModelId(const unsigned short);           // "GroundE xplosion" effect model ID

   unsigned short getShipWakeModelID() const { return cmtShipWake; }       // "Ship Wake" effect model ID
   virtual bool setShipWakeModelId(const unsigned short);                  // "Ship Wake" effect model ID

   // IG callbacks
   virtual void startOfFrame(const CigiSOFV2* const p);
   virtual void hatResp(const CigiHatRespV2* const p);
   virtual void losResp(const CigiLosRespV2* const p);
   virtual void collisionSegmentResp(const CigiCollDetSegRespV2* const p);
   virtual void sensorResp(const CigiSensorRespV2* const p);
   virtual void hotResp(const CigiHotRespV2* const p);
   virtual void collisionVolumeResp(const CigiCollDetVolRespV2* const p);
   virtual void igResponse(const CigiIGMsgV2* const p);

   // Send data to the Cigi handler
   virtual bool sendCigiData();

   CigiClV2* getCigi() { return cigi; }
   const CigiClV2* getCigi() const { return cigi; }

   // Set Slot functions
   virtual bool setSlotCigi(CigiClV2* const msg);
   virtual bool setSlotASyncMode(const Basic::Number* const msg);
   virtual bool setSlotHideOwnshipModel(const Basic::Number* const msg);
   virtual bool setSlotOwnshipModel(const Basic::Number* const msg);
   virtual bool setSlotMslTrailModel(const Basic::Number* const msg);
   virtual bool setSlotSmokePlumeModel(const Basic::Number* const msg);
   virtual bool setSlotAirExplosionModel(const Basic::Number* const msg);
   virtual bool setSlotGroundExplosionModel(const Basic::Number* const msg);
   virtual bool setSlotShipWakeModel(const Basic::Number* const msg);

   // Component interface
   virtual void updateData(const LCreal dt = 0.0);
   virtual void reset();

protected:
    enum LIFE_FORM_STATE { DEAD = 0, STANDING = 1, WALK = 2, RUN = 3 };

    virtual bool updateOwnshipModel();          // update the ownship model; returns true if ok
    virtual int updateModels();                 // update the other models; returns number of active models

    // Access functions
    CigiIGCtrlV2* getIgControlPacket()                   { return igc; }
    CigiLosVectReqV2* getLosRangeRequestPacket()      { return los; }
    CigiViewCtrlV2* getViewControlPacket()               { return view; }
    CigiViewDefV2* getViewDefinitionPacket()         { return fov; }
    CigiCompCtrlV2* getOwnshipComponentControlPacket(const unsigned int buffer) {
        return (buffer < NUM_BUFFERS ? ownshipCC[buffer] : 0);
    }
    CigiEntityCtrlV2* getOwnshipEntityControlPacket(const unsigned int buffer) {
       return (buffer < NUM_BUFFERS ? ownshipEC[buffer] : 0);
    }

    unsigned int getWriteBuffer() const { return iw; }      // Write buffer index
    unsigned int getLastWriteBuffer() const { return iw0; } // Last write buffer index
    unsigned int getReadBuffer() const { return ir; }       // Read index {returns index or NUM_BUFFERS if not valid)
    void swapReadBuffer() { if (iw0 < NUM_BUFFERS) ir = iw0; } // Swap the read buffer

    bool isIgResetRequested() const { return resetRequest; }
    void clearIgResetRequest() { resetRequest = false; }

    unsigned short getNexLosId() { return ++losReqId; }
    bool isNewLosequested() const { return newLosReq; }
    void losRequestSend();          // LOS request has been sent to the IG

    void elevationRequestSend();    // Elevation request has been sent to the IG

    // Get Line of sight data from previous request
    virtual bool getLineOfSightData(
            double* const lat,      // Point latitude         (deg)
            double* const lon,      // Point longitude        (deg)
            double* const alt,      // Point altitude         (m)
            LCreal* const rng,      // Range to point         (m)
            int* const material     // Material code
         );

    // Issue a new light of sight request
    virtual bool lineOfSightRequest(
            const double lat,          // Source latitude         (deg)
            const double lon,          // Source longitude        (deg)
            const double alt,          // Source altitude         (m)
            const LCreal hdg,          // Source heading          (deg)
            const LCreal pitch,        // Source pitch            (deg)
            const LCreal minRange,     // Request minimum range   (m)
            const LCreal maxRange      // Request maximum range   (m)
         );

    // Set functions
    virtual bool setViewControlPacket(CigiViewCtrlV2* const p);
    virtual bool setViewDefinitionPacket(CigiViewDefV2* const p);

    // Otw Interface
    virtual void sendOwnshipAndModels();    // Send state data for ownship and models
    virtual void sendElevationRequests();   // Sends terrain height requests
    virtual void recvElevations();          // Receives terrain height data
    virtual void frameSync();               // Send frame sync (if any)
    virtual Simulation::OtwModel* modelFactory();       // Create OtwModel objects unique to interface
    virtual Simulation::OtwModel* hotFactory();         // Create OtwHot objects unique to interface

    virtual bool setAirVehicleData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::AirVehicle* const p);
    virtual bool setBuildingData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::Building* const p);
    virtual bool setEffectsData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::Effects* const p);
    virtual bool setGndVehicleData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::GroundVehicle* const p);
    virtual bool setLifeFormData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::LifeForm* const p);
    virtual bool setMissileData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::Missile* const p);
    virtual bool setShipData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::Ship* const p);
    virtual bool setSpaceVehicleData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::SpaceVehicle* const p);
    virtual bool setWeaponData(OtwModelCigiClV2* const m, const unsigned short entity, const Simulation::Weapon* const p);
    virtual bool setCommonModelData(CigiEntityCtrlV2* const ec, const unsigned short entity, const Simulation::Player* const p);

private:
    Basic::safe_ptr<CigiClV2> cigi;       // CIGI handler (direct, networked, ...)
    bool   asyncMode;                     // Running in ASYNC mode if true
    bool   hideOwn;                       // Hide ownship model flag

    bool   resetRequest;                  // IG reset request
    unsigned short entityIdCount;         // Entity ID count
    unsigned short elevReqIdCount;        // Elevation request ID count

    // Terrain elevation request data
    bool    elevReqFlg;                   // Elevation request flag
    LCreal  elevReqTimer;                 // Elevation request timer

    // Line of sight (LOS) data
    double  losRespLat;                   // LOS Response latitude intersection point (deg)
    double  losRespLon;                   // LOS Response longitude intersection point (deg)
    double  losRespAlt;                   // LOS Response altitude intersection point (m)
    LCreal  losRespRange;                 // LOS response range (m)
    int     losRespMaterialCode;          // LOS response material code response
    unsigned short losRespId;             // LOS Response ID
    bool    losRespDataValid;             // LOS response data is valid flag
    unsigned short losReqId;              // LOS Request ID
    bool    newLosReq;                    // New LOS request flag
    LCreal  losReqTimer;                  // LOS request timer

    // CIGI entity data buffers
    unsigned int iw;                      // Write buffer index
    unsigned int iw0;                     // Last write buffer index
    unsigned int ir;                      // Read index

    // Packets
    CigiEntityCtrlV2* ownshipEC[NUM_BUFFERS];   // Ownship entity control packet
    CigiCompCtrlV2* ownshipCC[NUM_BUFFERS];     // ownship component control packet
    CigiIGCtrlV2* igc;                          // IG control packet
    CigiLosVectReqV2* los;                      // LOS request packet
    CigiViewCtrlV2* view;                       // View control packet (optional, set by derived classes)
    CigiViewDefV2* fov;                         // FOV defination packet (optional, set by derived classes

    // special model IDs
    unsigned short cmtOwnship;             // Ownship's model ID
    unsigned short cmtMslTrail;            // "Missile Trail" effect model ID
    unsigned short cmtSmokePlume;          // "Smoke Plume" effect model ID
    unsigned short cmtAirExplosion;        // "Air Explosion" effect model ID
    unsigned short cmtGroundExplosion;     // "GroundE xplosion" effect model ID
    unsigned short cmtShipWake;            // "Ship Wake" effect model ID

    // CIGI init support
    bool cigiInitialized;            // CIGI has been initialized
    bool cigiInitFailed;             // CIGI initialization has failed
};

//------------------------------------------------------------------------------
// Class:	CigiClV2
// Base class:	Basic::Object -> CigiClV2
//
// Description: CIGI handler
//
// Form name: CigiClV2
//------------------------------------------------------------------------------
class CigiClV2 : public Basic::Object
{
    DECLARE_SUBCLASS(CigiClV2,Basic::Object)

public:
   CigiClV2();

   // ID number of this CIGI session
   CigiHostSession* getSession(void) const  { return session; }

   // Init this CIGI handler
   virtual bool initialize(OtwCigiClV2* const p);

   // True if we've been successfully initialized
   virtual bool isInitialized();

   // R/T frame sync -- called from OtwCigiClV2::frameSync() in the R/T thread
   virtual void frameSync();

   // Background frame sync -- called from OtwCigiClV2::updateData() in the background
   virtual void frameBg();

   // CIGI functions
   virtual void startMessage() =0;
   virtual void endMessage() =0;
   virtual int  getOutgoingBufferSize() =0;

   virtual void addPacketIGCtrl(CigiIGCtrlV2* const p) =0;
   virtual void addPacketViewCtrl(CigiViewCtrlV2* const p) =0;
   virtual void addPacketViewDef(CigiViewDefV2* const p) =0;
   virtual void addPacketEntityCtrl(CigiEntityCtrlV2* const p) =0;
   virtual void addPacketComponentCtrl(CigiCompCtrlV2* const p) =0;
   virtual void addPacketArtPartCtrl(CigiArtPartCtrlV2* const p) =0;
   virtual void addPacketHotReq(CigiHotReqV2* const p) = 0;
   virtual void addPacketLosRangeReq(CigiLosVectReqV2* const p) =0;

protected:

   // Our parent OtwCigiClV2
   OtwCigiClV2* getOtwCigi() { return otwCigi; }

   // Return the Cigi object for this session id
   static CigiClV2* getSession(const int id) { return (id >= 0 && id < MAX_SESSIONS) ? sessions[id] : 0; }

private:
    OtwCigiClV2* otwCigi;   // Our parent OtwCigiClV2
                        // (no need to ref(), it owns us and it's not going to be deleted before us)

    CigiHostSession*	session;

    // Static table of CIGI session objects ...
    static const int MAX_SESSIONS = 8;
    static CigiClV2* sessions[MAX_SESSIONS];

    // CIGI init support
    static bool initCigi();                 // init the CIGI static system
    static bool cigiInitialized;            // CIGI has been initialized
    static bool cigiInitFailed;             // CIGI initialization has failed
};

//------------------------------------------------------------------------------
// Class: CigiClV2Network
// Base class: Basic::Object -> CigiClV2 -> CigiClV2Network
//
// Description: Networked CIGI interface to the IG system
//
// Form name: CigiClV2Network
// Slots:
//    netInput       (NetHandler)   Network input handler
//    netOutput      (NetHandler)   Network output handler
//------------------------------------------------------------------------------
class CigiClV2Network : public CigiClV2
{
    DECLARE_SUBCLASS(CigiClV2Network,CigiClV2)

public:
   CigiClV2Network();

   // Set Slot functions
   virtual bool setSlotNetInput(Basic::NetHandler* const msg);
   virtual bool setSlotNetOutput(Basic::NetHandler* const msg);

   // CIGI's (sync-mode) main network loop
   virtual void mainLoop();

   // CigiClV2 class interface
   virtual bool initialize(OtwCigiClV2* const p);
   virtual bool isInitialized();
   virtual void startMessage();
   virtual void endMessage();
   virtual int  getOutgoingBufferSize();

   virtual void addPacketIGCtrl(CigiIGCtrlV2* const p);
   virtual void addPacketViewCtrl(CigiViewCtrlV2* const p);
   virtual void addPacketViewDef(CigiViewDefV2* const p);
   virtual void addPacketEntityCtrl(CigiEntityCtrlV2* const p);
   virtual void addPacketComponentCtrl(CigiCompCtrlV2* const p);
   virtual void addPacketArtPartCtrl(CigiArtPartCtrlV2* const p);
   virtual void addPacketHotReq(CigiHotReqV2* const p);
   virtual void addPacketLosRangeReq(CigiLosVectReqV2* const p);

protected:
   bool createCigiProcess();        // Create the CIGI network thread
   bool initCigiNetwork();          // Initialize the network

private:
   Basic::safe_ptr<Basic::NetHandler> netInput;   // Input network handler
   Basic::safe_ptr<Basic::NetHandler> netOutput;  // Output network handler
   bool haveThread;                        // Have a network thread
   bool networkInitialized;                // CIGI has been initialized
   bool networkInitFailed;                 // CIGI initialization has failed

   CigiIncomingMsg* msgIn;
   CigiOutgoingMsg* msgOut;
   CigiClV2NetworkSignalProcessing* sigProcessor;
};

//------------------------------------------------------------------------------
// Class:	OtwModelCigiClV2
// Base class:	Basic::Object -> OtwModel -> OtwModelCigiClV2
// Description: CIGI OTW model
//------------------------------------------------------------------------------
class OtwModelCigiClV2 : public Simulation::OtwModel
{
    DECLARE_SUBCLASS(OtwModelCigiClV2,Simulation::OtwModel)

public:
    OtwModelCigiClV2();

    // OtwModel interface
    virtual void clear();
    virtual void initialize(Simulation::Player* const p);

    unsigned short getID() const          { return id; }
    void setID(const unsigned short i)    { id = i; }

// private: or at least should have been!
    CigiEntityCtrlV2* parentEC[OtwCigiClV2::NUM_BUFFERS];    // (entity_id)   Our main entity
    CigiEntityCtrlV2* trailEC[OtwCigiClV2::NUM_BUFFERS];     // (entity_id+1) Trails (missile, smoke, wake, etc.)
    CigiEntityCtrlV2* explosionEC[OtwCigiClV2::NUM_BUFFERS]; // (entity_id+2) Expolsions (air or ground)
    CigiEntityCtrlV2* smokeEC[OtwCigiClV2::NUM_BUFFERS];     // (entity_id+3) Smoke from damage
    CigiCompCtrlV2* damageCC[OtwCigiClV2::NUM_BUFFERS];    // Damage Component Control (CC)
    CigiCompCtrlV2* animationCC[OtwCigiClV2::NUM_BUFFERS]; // animation component control
    CigiEntityCtrlV2* attachedEC[OtwCigiClV2::NUM_BUFFERS];  // (entity_id+4) Attached missile
    CigiArtPartCtrlV2* launcherAPC[OtwCigiClV2::NUM_BUFFERS];  // Entity's launcher Articulated Part Control (APC)
    CigiCompCtrlV2* attachedCC[OtwCigiClV2::NUM_BUFFERS];  // attached missile component control

    bool parentActive;
    bool trailActive;
    bool explosionActive;
    bool smokeActive;
    bool damageActive;
    bool animationActive;
    bool attachedEcActive;
    bool launcherApcActive;
    bool attachedCcActive;

    bool isGroundPlayer;
    LCreal effectsTimer;

private:
    // Entity ID
   unsigned short id;
};

} // End Otw namespace
} // End Eaagles namespace

#endif
