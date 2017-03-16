
#ifndef __oe_interop_common_Nib_H__
#define __oe_interop_common_Nib_H__

#include "openeaagles/simulation/AbstractNib.hpp"

#include "openeaagles/interop/common/NetIO.hpp"
#include "openeaagles/models/players/Player.hpp"
#include <array>

namespace oe {
namespace models { class Missile; }
namespace interop {

//------------------------------------------------------------------------------
// Class: Nib
//
// Description: Network Interface Block (Nib)
//
//    This class is used to manage the relationship between players and their
//    corresponding entities on the interoperability networks (e.g., DIS, HLA).
//
//    Each Nib object manages either an incoming network entity to player
//    relationship or an outgoing player to network entity relationship.
//    The I/O type, which is either NetIO::INPUT_NIB or NetIO::OUTPUT_NIB, is
//    set by the constructor and is accessed using the getIoType() function.
//
//    The parent NetIO class object maintains two lists of Nib objects: one to
//    manage the incoming entities and a second to manage the outgoing players.
//    To create Nib objects, NetIO uses a NIB factory function, nibFactory(),
//    which is implemented by the NetIO network specific derived class.
//
//    Each Nib maintains pointers to their parent NetIO object and to the Player
//    object that they're managing.  The remainder of the data in this Nib class
//    is common data used by most interoperability network entities.  Additional
//    entity data is added by the network specific classes derived from Nib.
//
// Factory name: Nib
//
//------------------------------------------------------------------------------
class Nib : public simulation::AbstractNib
{
   DECLARE_SUBCLASS(Nib, simulation::AbstractNib)

public:
   // Standard (mil-std-1278.1) Dead_Reckoning Model codes [ 0 .. 9 ]
   enum DeadReckoning {
      OTHER_DRM,        // User defined (i.e., virtual function dynamicsOther())
      STATIC_DRM,       // No dead reckoning
      FPW_DRM,          // World, No rotation, 1st order linear
      RPW_DRM,          // World, 1st order rotation, 1st order linear
      RVW_DRM,          // World, 1st order rotation, 2nd order linear
      FVW_DRM,          // World, No rotation, 2nd order linear
      FPB_DRM,          // Body, No rotation, 1st order linear
      RPB_DRM,          // Body, 1st order rotation, 1st order linear
      RVB_DRM,          // Body, 1st order rotation, 2nd order linear
      FVB_DRM,          // Body, No rotation, 2nd order linear
   };

public:
   Nib(const NetIO::IoType ioType);

   NetIO::IoType getIoType() const                    { return ioType; }

   NetIO* getNetIO() override                         { return pNetIO; }  // Controlling Network I/O
   const NetIO* getNetIO() const                      { return pNetIO; }  // Controlling Network I/O (const version)
   virtual bool setNetIO(NetIO* const p);                                 // Sets our Network I/O controller

   // The player, its name and ID
   models::Player* getPlayer()                        { return pPlayer; }
   unsigned short getPlayerID() const override        { return playerID; }
   const char* getPlayerName() const                  { return pname; }

   virtual bool setPlayer(models::Player* const p);
   virtual void setPlayerID(const unsigned short v);
   virtual void setPlayerName(const char* s);

   virtual const base::String* getFederateName() const override;  // Federate name as String
   virtual bool setFederateName(const base::String* const msg);   // Sets our federate name

   // Mode
   bool isMode(const models::Player::Mode m) const            { return mode == m; }
   bool isNotMode(const models::Player::Mode m) const         { return mode != m; }
   models::Player::Mode getMode() const                       { return mode; }
   virtual void setMode(const models::Player::Mode m);

   // Side
   bool isSide(const models::Player::Side s) const            { return (s == side); }
   bool isNotSide(const models::Player::Side s) const         { return (s != side); }
   models::Player::Side getSide() const                       { return side; }
   virtual void setSide(const models::Player::Side s);

   // Dead Reckoning (DR) algorithm (see enum DeadReckoning)
   bool isDeadReckoning(const unsigned char dr) const         { return (drNum == dr); }
   unsigned char getDeadReckoning() const                     { return drNum; }
   bool setDeadReckoning(const unsigned char dr)              { drNum = dr; return true; }

   // DR's position vector @ T0 (meters) (ECEF)
   const base::Vec3d& getDrPosition() const                   { return drP0; }

   // DR's velocity vector @ T0 (m/sec)  (ECEF or Body based on the DR algorithm)
   const base::Vec3d& getDrVelocity() const override          { return drV0; }

   // DR's acceleration vector @ T0 ((m/sec)/sec) (ECEF or Body based on the DR algorithm)
   const base::Vec3d& getDrAcceleration() const override      { return drA0; }

   // DR's Euler angles @ T0 (rad) [ phi theta psi ] (Body/ECEF)
   const base::Vec3d& getDrEulerAngles() const                { return drRPY0; }

   // DR's angular rates @ T0 (rad/sec)  [ phi theta psi ] (Body/ECEF)
   const base::Vec3d& getDrAngularVelocities() const override { return drAV0; }

   // update incoming entity dead reckoning
   bool updateDeadReckoning(
      const double dt,                    // delta time (sec)
      base::Vec3d* const pNewPos,         // New DR position
      base::Vec3d* const pNewAngles       // New DR orientation
   ) override;

   // (re)initialize the dead reckoning function
   bool resetDeadReckoning(
         const unsigned char dr,       // Dead-Reckoning algorithm number (see enum DeadReckoning)
         const base::Vec3d& p,         // Position vector @ T=0 (meters) (ECEF)
         const base::Vec3d& v,         // Velocity vector @ T=0 (m/sec)  (ECEF or Body based on 'dr')
         const base::Vec3d& a,         // Acceleration vector @ T=0 ((m/sec)/sec) (ECEF or Body based on 'dr')
         const base::Vec3d& rpy,       // Euler angles @ T=0 (rad) [ phi theta psi ] (Body/ECEF)
         const base::Vec3d& av,        // Angular rates @ T=0 (rad/sec)  [ phi theta psi ] (Body/ECEF)
         const double time = 0         // Initial time (seconds) (default: zero)
      );

   // Checked flags
   bool isChecked() const                             { return checked; }
   void setCheckedFlag(const bool flg)                { checked = flg; }

   // Standard player (entity) type codes
   const Ntm* getTypeMapper() const                   { return ntm; }
   bool isEntityTypeChecked() const                   { return entityTypeChecked; }
   bool isEntityTypeUnchecked() const                 { return !entityTypeChecked; }
   bool isEntityTypeValid() const                     { return (ntm != nullptr); }
   bool isEntityTypeInvalid() const                   { return (ntm == nullptr); }
   virtual bool setTypeMapper(const Ntm* const p);
   virtual void setEntityTypeChecked(const bool f);

   // Appearance
   double getDamage() const                           { return damage; }
   double getSmoke() const                            { return smoking; }
   double getFlames() const                           { return flames; }
   unsigned int getCamouflageType() const             { return camouflage; }
   virtual bool setDamage(const double v);
   virtual bool setSmoke(const double v);
   virtual bool setFlames(const double v);
   virtual bool setCamouflageType(const unsigned int v);

   // Articulated Part: wing sweep angle change count
   unsigned int getAPartWingSweepCnt() const          { return apartWingSweepCnt; }

   // Articulated Part: wing sweep (radians)
   double getAPartWingSweep() const                   { return apartWingSweep; }

   // Articulated Part: gear position change count
   unsigned int getAPartGearPosCnt() const            { return apartGearPosCnt; }

   // Articulated Part: landing gear position (% [0% up; 100% down] )
   double getAPartPartGearPos() const                 { return apartLandingGear; }

   // Articulated Part: bay door position change count
   unsigned int getAPartBayDoorCnt() const            { return apartBayDoorCnt; }

   // Articulated Part: bay door position (% [0% closed; 100% open] )
   double getAPartBayDoorPos() const                  { return apartBayDoor; }

   // Articulated Part: launcher elevation angle change count
   unsigned int getAPartLauncherElevationCnt() const  { return apartLnchrElevCnt; }

   // Articulated Part: Launcher elevation angle (radians)
   double getAPartLauncherElevation() const               { return apartLnchrElev; }

   // Articulated Part: Number of attached missiles
   unsigned int getAPartNumberAttachedNumMissiles() const { return apartNumMissiles; }

   // Articulated Part: Returns a pointer to the n'th attached missiles
   //                   [n: 1 .. number-of-missiles]
   const models::Missile* getAPartAttachedMissile(const unsigned int n) const {
      return (n >= 1 && n <= apartNumMissiles ? apartMsl[n-1] : 0);
   }

   // Articulated Part: Returns the attached missile change counts for the n'th attached missile
   //                   [n: 1 .. number-of-missiles]
   unsigned int getAPartAttacheMissileChangeCnt(const unsigned int n) const {
      return (n >= 1 && n <= apartNumMissiles ? apartMslCnt[n-1] : 0);
   }

   bool wasDetonationMessageSent() const          { return detMsgSent; }
   virtual bool setDetonationMessageSent(const bool flg);

   // NIB timeout
   bool isTimeoutEnabled() const                  { return timeoutEnbFlg; }
   void setTimeoutEnabled(const bool flg)         { timeoutEnbFlg = flg; }

   double getTimeExec() const                     { return execTime; }
   virtual void setTimeExec(const double t);

   double getTimeUtc() const                      { return utcTime; }
   virtual void setTimeUtc(const double t);

   virtual bool setOutputPlayerType(const models::Player* const p);
   virtual bool isPlayerStateUpdateRequired(const double curExecTime);
   virtual void playerState2Nib();
   virtual void nib2PlayerState();

   virtual bool entityStateManager(const double curExecTime);
   virtual bool weaponFireMsgFactory(const double curExecTime);
   virtual bool munitionDetonationMsgFactory(const double curExecTime);
   virtual bool networkOutputManagers(const double curExecTime);

protected:
   // Max number of attached missiles
   static const unsigned int MAX_AMSL = 4;

   // Main Dead Reckoning Function
   bool mainDeadReckoning(
         const double dT,                 // DR time (seconds)
         base::Vec3d* const pNewP0,       // DR Position vector @ time = 'dT' (meters) (ECEF)
         base::Vec3d* const pNewRPY       // DR Euler angles @ time = 'dT' (rad) [ phi theta psi ] (Body/ECEF)
      ) const;

   // Update our DR time and return the new time
   double updateDrTime(const double dt)           { return (drTime += dt); }

   virtual bool shutdownNotification() override;

private:
   // compute the rotational matrix R0
   static bool drComputeMatrixR0(
         const base::Vec3d& RPY,      // [radians]
         base::Matrixd* const pR0     // Rotational matrix R0
      );

   // compute the wwT matrix
   static bool drComputeMatrixWwT(
         const base::Vec3d& av,       // angular velocities [rps]
         base::Matrixd* const pwwT    // matrix
      );

   // compute the omega matrix
   static bool drComputeMatrixOmega(
         const base::Vec3d& av,       // [rps]
         base::Matrixd* const pOmega
      );

   static bool drComputeMatrixDR(
         const double dT,             // DR time [sec]
         const base::Vec3d& av,       // angular velocities [rps]
         const base::Matrixd& wwT,    // wwT matrix
         const base::Matrixd& omega,  // omega matrix
         base::Matrixd* const pDR     // DR matrix
      );

   static bool drComputeMatrixR1(
         const double dT,             // DR time [sec]
         const base::Vec3d& av,       // angular velocities [rps]
         const base::Matrixd& wwT,    // wwT matrix
         const base::Matrixd& omega,  // omega matrix
         base::Matrixd* const pR1     // Matrix R1
      );

   static bool drComputeMatrixR2(
         const double dT,             // DR time [sec]
         const base::Vec3d& av,       // angular velocities [rps]
         const base::Matrixd& wwT,    // wwT matrix
         const base::Matrixd& omega,  // omega matrix
         base::Matrixd* const pR2     // Matrix R2
      );

private:
   void initData();

   NetIO::IoType ioType;               // Input/Output direction of this NIB

   base::safe_ptr<const base::String> federateName; // Federate name
   base::safe_ptr<models::Player> pPlayer;          // Our player
   base::safe_ptr<NetIO> pNetIO;                    // Our Network
   bool checked {};                                 // NIB was checked
   unsigned short playerID {};                      // Player ID

   // Type mapper and type checked flags
   base::safe_ptr<const Ntm> ntm;      // Type mapper
   bool entityTypeChecked {};          // Entity type has been checked

   // Player data
   static const unsigned int PNAME_BUF_SIZE = 64;
   char        pname[PNAME_BUF_SIZE] {};  // Name
   models::Player::Side side;             // Side
   models::Player::Mode mode;             // NIB Mode
   bool        timeoutEnbFlg {true};      // NIB can timeout flag
   double      damage {};                 // Damage state from no damage(0.0) to destroyed (1.0)
   double      smoking {};                // Smoke state from no smoke (0.0) to maximum (1.0)
   double      flames {};                 // Flames state from no flames (0.0) to maximum (1.0)
   unsigned int camouflage {};            // Camouflage type (0 is none)
   bool        detMsgSent {};             // True if detonation message was sent

   // Times
   double execTime {};                 // Exec time of last update (seconds)
   double utcTime {};                  // UTC time of last update (seconds)

   // Dead Reckoning (DR) data
   unsigned char drNum {STATIC_DRM};   // Dead-Reckoning algorithm (see enum DeadReckoning)
   base::Vec3d  drP0;                  // Position vector @ t0 (meters) (ECEF)
   base::Vec3d  drV0;                  // Velocity vector @ t0 (m/sec)  (ECEF or Body based on the DR algorithm)
   base::Vec3d  drA0;                  // Acceleration vector @ t0 ((m/sec)/sec) (ECEF or Body based on the DR algorithm)
   base::Vec3d  drRPY0;                // Euler angles @ t0 (rad) (Body/ECEF)
   base::Vec3d  drAV0;                 // Angular rates @ t0 (rad/sec) (Body/ECEF)
   base::Matrixd drR0;                 // R0 matrix
   base::Matrixd drWwT;                // wwT matrix
   base::Matrixd drOmega;              // omega matrix

   // Current DR values (incoming only)
   double drTime {};                   // DR time (sec)
   base::Vec3d drPos;                  // Current DR position vector (meters) (ECEF)
   base::Vec3d drAngles;               // Current DR angles (rad) [ roll pitch yaw ] (Body/ECEF)

   // DR smoothing data
   base::Vec3d smoothVel;              // Smoothing Velocity (meters/second) (ECEF)
   double smoothTime {};               // Smoothing Time

   // Articulated parts (Air Vehicles)
   unsigned int apartWingSweepCnt {};     // Articulated Part: wing sweep angle change count
   unsigned int apartGearPosCnt {};       // Articulated Part: gear position change count
   unsigned int apartBayDoorCnt {};       // Articulated Part: bay door position change count
   unsigned int apartLnchrElevCnt {};     // Articulated Part: launcher elevation angle change count
   double apartWingSweep {};              // Articulated Part: wing sweep angle (radians)
   double apartLandingGear {};            // Articulated Part: landing gear position (% [0% up; 100% down] )
   double apartBayDoor {};                // Articulated Part: bay door position (% [0% closed; 100% open] )
   double apartLnchrElev {};              // Articulated Part: Launcher elevation angle (radians)

   std::array<const models::Missile*, MAX_AMSL> apartMsl {};  // Articulated Part: Attached missiles
   std::array<unsigned int, MAX_AMSL> apartMslCnt {};         // Articulated Part: Attached missile change counts
   std::array<bool, MAX_AMSL> apartMslAttached {};            // Articulated Part: Missile attached (not launched) flag
   unsigned int apartNumMissiles {};                          // Articulated Part: Number of attached missiles
};

}
}

#endif
