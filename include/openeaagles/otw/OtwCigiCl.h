//------------------------------------------------------------------------------
// Classes: OtwCigiCl, CigiCl, CigiClNetwork, OtwModelCigiCl
//------------------------------------------------------------------------------
#ifndef __Eaagles_Otw_OtwCigiCl_H__
#define __Eaagles_Otw_OtwCigiCl_H__

class CigiEntityCtrlV3;
class CigiCompCtrlV3;
class CigiIGCtrlV3;
class CigiLosVectReqV3;   // CGBCGB CIGI_LOS_RANGE_REQUEST* los;          // LOS request packet
class CigiViewCtrlV3;
class CigiViewDefV3;
class CigiArtPartCtrlV3;
class CigiHatHotReqV3;
class CigiSOFV3;
class CigiHatHotRespV3;
class CigiLosRespV3;
class CigiCollDetSegRespV3;
class CigiCollDetVolRespV3;
class CigiSensorCtrlV3;
class CigiSensorRespV3;
class CigiIGMsgV3;       // CGBCGB CIGI_IG_RESPONSE_MESSAGE
class CigiHostSession;
class CigiIncomingMsg;
class CigiOutgoingMsg;

#include "openeaagles/simulation/Otw.h"

namespace Eaagles {

   namespace Basic {
      class NetHandler;
      class Thread;
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

class CigiCl;
class OtwModelCigiCl;
class CigiClNetworkSignalProcessing;

//------------------------------------------------------------------------------
// Class: OtwCigiCl
//
// Description: Boeing's Common Image Generator Interface (CIGI)
//
// Factory name: OtwCigiCl
// Slots:
//    cigi                 (CigiCl)       CIGI interface context
//    async                (Number)       True (non-zero) to run in CIGI async mode (default: false - CIGI sync)
//    hideOwnshipModel     (Number)       True to hide the ownship's model (default: true - ownship's model is not seen)
//    ownshipModel         (Number)       Ownship's model ID
//    mslTrailModel        (Number)       Missile Trail" effect model ID
//    smokePlumeModel      (Number)       Smoke Plume" effect model ID
//    airExplosionModel    (Number)       Air Explosion" effect model ID
//    groundExplosionModel (Number)       Ground Explosion" effect model ID
//    shipWakeModel        (Number)       Ship Wake" effect model ID
//
// Note: In the async mode, the sendCigiData() function, which sends the CIGI
// packets to the CigiCl handler, is called by our frameSync() function in the
// R/T thread.  In the sync mode, the sendCigiData() function is called by the
// startOfFrame() callback (i.e., sync'd with the IG).
//
//------------------------------------------------------------------------------
class OtwCigiCl : public Simulation::Otw
{
   DECLARE_SUBCLASS(OtwCigiCl, Simulation::Otw)

public:
   static const unsigned int NUM_BUFFERS = 2;

public:
   OtwCigiCl();

   // Is a LOS request pending?
   virtual bool isLosRequestPending() const { return (losReqId != losRespId); }

   // Is an elevation request pending?
   virtual bool isElevationRequestPending() const { return elevReqFlg; }

   bool isASyncMode() const { return asyncMode; }                                   // True if running in CIGI async mode
   bool isSyncMode() const { return !asyncMode; }                                   // True if running in CIGI sync mode
   virtual bool setASyncMode(const bool f);                                         // Sets the CIGI async mode flag

   bool isOwnshipModelHidden() const { return hideOwn; }                            // True if the ownship's model is hidden
   virtual bool setHideOwnshipModel(const bool f);                                  // Sets the hide ownship flag

   unsigned short getOwnshipModelID() const { return cmtOwnship; }                  // Ownship's model ID
   virtual bool setOwnshipModelId(const unsigned short);                            // Ownship's model ID

   unsigned short getMslTrailModelID() const { return cmtMslTrail; }                // "Missile Trail" effect model ID
   virtual bool setMslTrailModelId(const unsigned short);                           // "Missile Trail" effect model ID

   unsigned short getSmokePlumeModelID() const { return cmtSmokePlume; }            // "Smoke Plume" effect model ID
   virtual bool setSmokePlumeModelId(const unsigned short);                         // "Smoke Plume" effect model ID

   unsigned short getAirExplosionModelID() const { return cmtAirExplosion; }        // "Air Explosion" effect model ID
   virtual bool setAirExplosionModelId(const unsigned short);                       // "Air Explosion" effect model ID

   unsigned short getGroundExplosionModelID() const { return cmtGroundExplosion; }  // "Ground Explosion" effect model ID
   virtual bool setGroundExplosionModelId(const unsigned short);                    // "Ground Explosion" effect model ID

   unsigned short getShipWakeModelID() const { return cmtShipWake; }                // "Ship Wake" effect model ID
   virtual bool setShipWakeModelId(const unsigned short);                           // "Ship Wake" effect model ID

   // IG callbacks
   virtual void startOfFrame(const CigiSOFV3* const p);
   virtual void hatHotResp(const CigiHatHotRespV3* const p);
   virtual void losResp(const CigiLosRespV3* const p);
   virtual void collisionSegmentResp(const CigiCollDetSegRespV3* const p);
   virtual void sensorResp(const CigiSensorRespV3* const p);
   virtual void collisionVolumeResp(const CigiCollDetVolRespV3* const p);
   virtual void igResponse(const CigiIGMsgV3* const p);

   // Send data to the Cigi handler
   virtual bool sendCigiData();

   CigiCl* getCigi()                  { return cigi; }
   const CigiCl* getCigi() const      { return cigi; }

   // Set Slot functions
   virtual bool setSlotCigi(CigiCl* const msg);
   virtual bool setSlotASyncMode(const Basic::Number* const msg);
   virtual bool setSlotHideOwnshipModel(const Basic::Number* const msg);
   virtual bool setSlotOwnshipModel(const Basic::Number* const msg);
   virtual bool setSlotMslTrailModel(const Basic::Number* const msg);
   virtual bool setSlotSmokePlumeModel(const Basic::Number* const msg);
   virtual bool setSlotAirExplosionModel(const Basic::Number* const msg);
   virtual bool setSlotGroundExplosionModel(const Basic::Number* const msg);
   virtual bool setSlotShipWakeModel(const Basic::Number* const msg);

   void updateData(const LCreal dt = 0.0) override;
   void reset() override;

protected:
   enum LIFE_FORM_STATE { DEAD = 0, STANDING = 1, WALK = 2, RUN = 3 };

   virtual bool updateOwnshipModel();          // update the ownship model; returns true if ok
   virtual int updateModels();                 // update the other models; returns number of active models

   // Access functions
   CigiIGCtrlV3* getIgControlPacket()                   { return igc; }
   CigiLosVectReqV3* getLosRangeRequestPacket()         { return los; }
   CigiViewCtrlV3* getViewControlPacket()               { return view; }
   CigiSensorCtrlV3* getSensorControlPacket()           { return sensor; }
   CigiViewDefV3* getViewDefinitionPacket()             { return fov; }
   CigiCompCtrlV3* getOwnshipComponentControlPacket(const unsigned int buffer) {
      return (buffer < NUM_BUFFERS ? ownshipCC[buffer] : 0);
   }
   CigiEntityCtrlV3* getOwnshipEntityControlPacket(const unsigned int buffer) {
      return (buffer < NUM_BUFFERS ? ownshipEC[buffer] : 0);
   }

   unsigned int getWriteBuffer() const { return iw; }          // Write buffer index
   unsigned int getLastWriteBuffer() const { return iw0; }     // Last write buffer index
   unsigned int getReadBuffer() const { return ir; }           // Read index {returns index or NUM_BUFFERS if not valid)
   void swapReadBuffer() { if (iw0 < NUM_BUFFERS) ir = iw0; }  // Swap the read buffer

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
      int* const material     // Material code - not used in Cigi V3, returns 0
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
   virtual bool setViewControlPacket(CigiViewCtrlV3* const p);
   virtual bool setViewDefinitionPacket(CigiViewDefV3* const p);
   virtual bool setSensorControlPacket(CigiSensorCtrlV3* const p);

   void sendOwnshipAndModels() override;    // Send state data for ownship and models
   void sendElevationRequests() override;   // Sends terrain height requests
   void recvElevations() override;          // Receives terrain height data
   void frameSync() override;               // Send frame sync (if any)
   Simulation::OtwModel* modelFactory() override;       // Create OtwModel objects unique to interface
   Simulation::OtwModel* hotFactory() override;         // Create OtwHot objects unique to interface

   virtual bool setAirVehicleData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::AirVehicle* const p);
   virtual bool setBuildingData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::Building* const p);
   virtual bool setEffectsData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::Effects* const p);
   virtual bool setGndVehicleData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::GroundVehicle* const p);
   virtual bool setLifeFormData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::LifeForm* const p);
   virtual bool setMissileData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::Missile* const p);
   virtual bool setShipData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::Ship* const p);
   virtual bool setSpaceVehicleData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::SpaceVehicle* const p);
   virtual bool setWeaponData(OtwModelCigiCl* const m, const unsigned short entity, const Simulation::Weapon* const p);
   virtual bool setCommonModelData(CigiEntityCtrlV3* const ec, const unsigned short entity, const Simulation::Player* const p);

private:
   Basic::safe_ptr<CigiCl> cigi;         // CIGI handler (direct, networked, ...)
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
   CigiEntityCtrlV3* ownshipEC[NUM_BUFFERS]; // Ownship entity control packet
   CigiCompCtrlV3*   ownshipCC[NUM_BUFFERS]; // ownship component control packet
   CigiIGCtrlV3*     igc;                    // IG control packet
   CigiLosVectReqV3* los;                    // LOS request packet
   CigiViewCtrlV3*   view;                   // View control packet (optional, set by derived classes)
   CigiViewDefV3*    fov;                    // FOV definition packet (optional, set by derived classes
   CigiSensorCtrlV3* sensor;                 // Sensor Control packet

   // special model IDs
   unsigned short cmtOwnship;             // Ownship's model ID
   unsigned short cmtMslTrail;            // "Missile Trail" effect model ID
   unsigned short cmtSmokePlume;          // "Smoke Plume" effect model ID
   unsigned short cmtAirExplosion;        // "Air Explosion" effect model ID
   unsigned short cmtGroundExplosion;     // "Ground Explosion" effect model ID
   unsigned short cmtShipWake;            // "Ship Wake" effect model ID

   // CIGI init support
   bool cigiInitialized;            // CIGI has been initialized
   bool cigiInitFailed;             // CIGI initialization has failed
};

//------------------------------------------------------------------------------
// Class: CigiCl
// Description: CIGI I/O handler
//
// Factory name: CigiCl
//------------------------------------------------------------------------------
class CigiCl : public Basic::Component
{
   DECLARE_SUBCLASS(CigiCl, Basic::Component)

public:
   CigiCl();

   // ID number of this CIGI session
   CigiHostSession* getSession(void) const  { return session; }

   // Init this CIGI handler
   virtual bool initialize(OtwCigiCl* const p);

   // True if we've been successfully initialized
   virtual bool isInitialized();

   // R/T frame sync -- called from OtwCigiCl::frameSync() in the R/T thread
   virtual void frameSync();

   // Background frame sync -- called from OtwCigiCl::updateData() in the background
   virtual void frameBg();

   // CIGI functions
   virtual void startMessage() =0;
   virtual void endMessage() =0;
   virtual int  getOutgoingBufferSize() =0;

   virtual void addPacketIGCtrl(CigiIGCtrlV3* const p) =0;
   virtual void addPacketViewCtrl(CigiViewCtrlV3* const p) =0;
   virtual void addPacketSensorCtrl(CigiSensorCtrlV3* const p)=0;
   virtual void addPacketViewDef(CigiViewDefV3* const p) =0;
   virtual void addPacketEntityCtrl(CigiEntityCtrlV3* const p) =0;
   virtual void addPacketComponentCtrl(CigiCompCtrlV3* const p) =0;
   virtual void addPacketArtPartCtrl(CigiArtPartCtrlV3* const p) =0;
   virtual void addPacketHatHotReq(CigiHatHotReqV3* const p) = 0;
   virtual void addPacketLosRangeReq(CigiLosVectReqV3* const p) =0;

protected:

   // Our parent OtwCigiCl
   OtwCigiCl* getOtwCigi() { return otwCigi; }

   // Return the Cigi object for this session id
   static CigiCl* getSession(const int id) { return (id >= 0 && id < MAX_SESSIONS) ? sessions[id] : 0; }

private:
   OtwCigiCl* otwCigi;   // Our parent OtwCigiCl
   // (no need to ref(), it owns us and it's not going to be deleted before us)

   CigiHostSession* session;

   // Static table of CIGI session objects ...
   static const int MAX_SESSIONS = 8;
   static CigiCl* sessions[MAX_SESSIONS];

   // CIGI init support
   static bool initCigi();                 // init the CIGI static system
   static bool cigiInitialized;            // CIGI has been initialized
   static bool cigiInitFailed;             // CIGI initialization has failed
};

//------------------------------------------------------------------------------
// Class: CigiClNetwork
// Base class: Basic::Object -> CigiCl -> CigiClNetwork
//
// Description: Networked CIGI interface to the IG system
//
// Factory name: CigiClNetwork
// Slots:
//    netInput       (NetHandler)   Network input handler
//    netOutput      (NetHandler)   Network output handler
//------------------------------------------------------------------------------
class CigiClNetwork : public CigiCl
{
   DECLARE_SUBCLASS(CigiClNetwork, CigiCl)

public:
   CigiClNetwork();

   // get a pre-ref'd pointer to the network input handler
   virtual Basic::NetHandler* getInputHandler();

   // get a pre-ref'd pointer to the network output handler
   virtual Basic::NetHandler* getOutputHandler();

   // Set Slot functions
   virtual bool setSlotNetInput(Basic::NetHandler* const msg);
   virtual bool setSlotNetOutput(Basic::NetHandler* const msg);

   // CIGI's (sync-mode) main network loop
   virtual void mainLoop();

   bool initialize(OtwCigiCl* const p) override;
   bool isInitialized() override;
   void startMessage() override;
   void endMessage() override;
   int  getOutgoingBufferSize() override;

   void addPacketIGCtrl(CigiIGCtrlV3* const p) override;
   void addPacketViewCtrl(CigiViewCtrlV3* const p) override;
   void addPacketSensorCtrl(CigiSensorCtrlV3* const p) override;
   void addPacketViewDef(CigiViewDefV3* const p) override;
   void addPacketEntityCtrl(CigiEntityCtrlV3* const p) override;
   void addPacketComponentCtrl(CigiCompCtrlV3* const p) override;
   void addPacketArtPartCtrl(CigiArtPartCtrlV3* const p) override;
   void addPacketHatHotReq(CigiHatHotReqV3* const p) override;
   void addPacketLosRangeReq(CigiLosVectReqV3* const p) override;

protected:
   bool createCigiProcess();        // Create the CIGI network thread
   bool initCigiNetwork();          // Initialize the network

private:
   Basic::safe_ptr<Basic::NetHandler>  netInput;    // Input network handler
   Basic::safe_ptr<Basic::NetHandler>  netOutput;   // Output network handler
   Basic::safe_ptr<Basic::Thread>      thread;      // The thread
   bool networkInitialized;               // CIGI has been initialized
   bool networkInitFailed;                // CIGI initialization has failed

   CigiIncomingMsg* msgIn;
   CigiOutgoingMsg* msgOut;
   CigiClNetworkSignalProcessing* sigProcessor;
};

//------------------------------------------------------------------------------
// Class: OtwModelCigiCl
// Base class: Basic::Object -> OtwModel -> OtwModelCigiCl
// Description: CIGI OTW model
//------------------------------------------------------------------------------
class OtwModelCigiCl : public Simulation::OtwModel
{
   DECLARE_SUBCLASS(OtwModelCigiCl,Simulation::OtwModel)

public:
   OtwModelCigiCl();

   void clear() override;

   virtual void initialize(Simulation::Player* const p);

   unsigned short getID() const          { return id; }
   void setID(const unsigned short i)    { id = i; }

   // private: or at least should have been!
   CigiEntityCtrlV3* parentEC[OtwCigiCl::NUM_BUFFERS];      // (entity_id)   Our main entity
   CigiEntityCtrlV3* trailEC[OtwCigiCl::NUM_BUFFERS];       // (entity_id+1) Trails (missile, smoke, wake, etc.)
   CigiEntityCtrlV3* explosionEC[OtwCigiCl::NUM_BUFFERS];   // (entity_id+2) Expolsions (air or ground)
   CigiEntityCtrlV3* smokeEC[OtwCigiCl::NUM_BUFFERS];       // (entity_id+3) Smoke from damage
   CigiCompCtrlV3* damageCC[OtwCigiCl::NUM_BUFFERS];        // Damage Component Control (CC)
   CigiCompCtrlV3* animationCC[OtwCigiCl::NUM_BUFFERS];     // animation component control
   CigiEntityCtrlV3* attachedEC[OtwCigiCl::NUM_BUFFERS];    // (entity_id+4) Attached missile
   CigiArtPartCtrlV3* launcherAPC[OtwCigiCl::NUM_BUFFERS];  // Entity's launcher Articulated Part Control (APC)
   CigiCompCtrlV3* attachedCC[OtwCigiCl::NUM_BUFFERS];      // attached missile component control

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
