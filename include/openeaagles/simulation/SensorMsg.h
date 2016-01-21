//------------------------------------------------------------------------------
// Class: SensorMsg
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_SensorMsg_H__
#define __oe_Simulation_SensorMsg_H__

#include "openeaagles/basic/Object.h"

namespace oe {
namespace Simulation {
   class Player;
   class Gimbal;

//------------------------------------------------------------------------------
// Class: SensorMsg
// Description: Generic sensor message class; used to pass requests data
//              between sensors (i.e., antennas, IR seekers) and players.
//
// Factory name: SensorMsg
//------------------------------------------------------------------------------
class SensorMsg : public basic::Object
{
   DECLARE_SUBCLASS(SensorMsg,basic::Object)

public:
   SensorMsg();

   // ---
   // Normalized ownship to target LOS vector (ownship's NED)
   // ---
   const osg::Vec3d& getLosVec() const          { return losO2T; }
   void setLosVec(const osg::Vec3d& v)          { losO2T = v; }

   // ---
   // Normalized target to ownship LOS vector (target's NED)
   // ---
   const osg::Vec3d& getTgtLosVec() const       { return losT2O; }
   void setTgtLosVec(const osg::Vec3d& v)       { losT2O = v; }

   // ---
   // Angles to target in gimbal coordinates
   // ---

   // Azimuth (radians)
   LCreal getGimbalAzimuth() const              { return gaz; }
   void setGimbalAzimuth(const LCreal a)        { gaz = a; }

   // Elevation (radians)
   LCreal getGimbalElevation() const            { return gel; }
   void setGimbalElevation(const LCreal a)      { gel = a; }

   // ---
   // Ranges
   // ---

   // Range from gimbal to target along LOS vector (meters)
   LCreal getRange() const                      { return rng; }

   // Max range of this sensor/message (NM)
   LCreal getMaxRangeNM() const                 { return maxRng; }

   // Sets the range to the target (meters)
   virtual void setRange(const LCreal r);

   // Sets the max range (NM)
   void setMaxRangeNM(const LCreal r)           { maxRng = r; }

   // ---
   // Range rates (projected along the LOS vector)
   // ---

   // Range rate (player to target) along the LOS vector (m/s)
   LCreal getRangeRate() const                  { return rngRate; }

   // Range rate (player to target) along the LOS vector (ft/s)
   LCreal getRangeRateFPS() const;

   // Range rate (player to target) along the LOS vector (knots)
   LCreal getRangeRateKts() const;

   // Sets the target range rate (positive for increasing range) (m/s)
   void setRangeRate(const LCreal v)            { rngRate = v; }

   // ---
   // Target's Angle Of Incidence (AOI); computed by the target
   // player upon receiving this sensor message
   // ---

   // Target's azimuth AOI (radians; target body coordinates)
   LCreal getAzimuthAoi() const                 { return iaz; }

   // Target's elevation AOI (radians; target body coordinates)
   LCreal getElevationAoi() const               { return iel; }

   // Target's normalized AOI vector
   const osg::Vec3d& getAoiVector() const       { return aoi; }

   // Sets the target's azimuth AOI (radians; target body coordinates)
   void setAzimuthAoi(const LCreal a)           { iaz = a; }

   // Sets the target's elevation AOI (radians; target body coordinates)
   void setElevationAoi(const LCreal a)         { iel = a; }

   // Sets the target's normalized AOI vector
   void setAoiVector(const osg::Vec3d& v)       { aoi = v; }
   void setAoiVector(const osg::Vec4d& v)       { aoi.set(v.x(),v.y(),v.z()); }

   // ---
   // Modes and flags
   // ---

   // Process by local players only flag
   bool isLocalPlayersOnly() const              { return localOnly; }

   // Return message requested flag
   // (tells the target player to return this message with its signature)
   bool isReturnRequested() const               { return returnReq; }

   // Sets the 'process only local players' flag
   void setLocalPlayersOnly(const bool b)       { localOnly = b;   }

   // Sets the return request flag
   void setReturnRequest(const bool b)          { returnReq = b; }

   // ---
   // Pointers
   // ---

   // Pointer to the gimbal (seeker, antenna) that sent this message
   Gimbal* getGimbal()                          { return gimbal; }
   const Gimbal* getGimbal() const              { return gimbal; }

   // Pointer to the player that sent this message
   Player* getOwnship()                         { return ownship; }
   const Player* getOwnship() const             { return ownship; }

   // Pointer to the target of this message
   Player* getTarget()                          { return target; }
   const Player* getTarget() const              { return target; }

   // Optional: data message attached to sensor message
   basic::Object* getDataMessage()              { return dataMsg; }
   const basic::Object* getDataMessage() const  { return dataMsg; }

   // Sets the gimbal that generated this message
   void setGimbal(Gimbal* const t);

   // Sets the player that sent this message
   void setOwnship(Player* const p);

   // Sets the target player
   void setTarget(Player* const p);

   // Sets the optional data message attached to sensor message
   void setDataMessage(basic::Object* const msg);


   // Clear data
   virtual void clear();

private:
   void initData();

   LCreal          maxRng;         // Maximum range                                (NM)
   LCreal          rng;            // Range                                        (meters)
   LCreal          rngRate;        // Range Rate                                   (m/s)
   LCreal          gaz;            // Gimbal azimuth                               (rad)
   LCreal          gel;            // Gimbal elevation                             (rad)
   LCreal          iaz;            // Target's Angle Of Incidence (AOI) azimuth    (rad)
   LCreal          iel;            // Target's AOI elevation                       (rad)
   osg::Vec3d      losO2T;         // Normalized ownship to target LOS vector (ownship's NED)
   osg::Vec3d      losT2O;         // Normalized target to ownship LOS vector (target's NED)
   osg::Vec3d      aoi;            // Normalized target Angle Of Incidence (AOI) vector
   Gimbal*         gimbal;         // The gimbal that transmitted the message
   basic::safe_ptr<Player>    ownship;     // The originating (ownship) player
   basic::safe_ptr<Player>    target;      // The Target player
   basic::safe_ptr<basic::Object> dataMsg; // Embedded data message (e.g., datalink, etc)
   bool            returnReq;      // Return Request
   bool            localOnly;      // Local players only flag
};

} // End Simulation namespace
} // End oe namespace

#endif
