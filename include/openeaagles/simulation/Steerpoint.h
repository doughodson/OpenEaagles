//------------------------------------------------------------------------------
// Class: Steerpoint
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Steerpoint_H__
#define __oe_simulation_Steerpoint_H__

#include "openeaagles/basic/Component.h"
#include "openeaagles/basic/osg/Vec3"

namespace oe {
   namespace basic {
      class Angle;
      class Distance;
      class Identifier;
      class LatLon;
      class List;
      class String;
      class Terrain;
      class Time;
   }
namespace simulation {
class Navigation;
class Action;

//------------------------------------------------------------------------------
// Class: Steerpoint
//
// Description: Generic steerpoint; contains positional and navigational data;
//              component list contains associated steerpoints (FIX, OAP, etc.)
//
// Factory name: Steerpoint
// Slots:
//   stptType  <basic::Identifier>  ! Steerpoint type; default: DEST
//                                  ! { ROUTE, DEST, MARK, FIX, OAP, TGT }
//
//   latitude  <basic::LatLon>      ! Steerpoint latitude  (default: 0)
//             <basic::Number>      ! (or as degrees decimal)
//
//   longitude <basic::LatLon>      ! Steerpoint longitude (default: 0)
//             <basic::Number>      ! (or as degrees decimal)
//
//   xPos      <basic::Distance>    ! X (north) distance from gaming area reference point
//   yPos      <basic::Distance>    ! Y (east) distance from gaming area reference point
//                                  ! (default: 0, 0)
//   elevation <basic::Distance>    ! Terrain elevation at steerpoint (default: 0)
//             <basic::Number>      ! (or as meters)
//
//   altitude  <basic::Distance>    ! Commanded altitude to steerpoint (default: 0)
//             <basic::Number>      ! (or as meters)
//
//   airspeed  <basic::Number>      ! Commanded true airspeed (Kts) (default: 0)
//
//   pta       <basic::Distance>    ! Planned Time of Arrival (PTA)  (default: 0)
//             <basic::Number>      ! (or as seconds)
//
//   sca       <basic::Distance>    ! Safe Clearance Altitude (SCA) (default: 0)
//             <basic::Number>      ! (or as feet)
//
//   description <basic::String>    ! Description of steerpoint (default: 0)
//
//   magvar    <basic::Angle>       ! Magnetic Var at steerpoint (default: 0)
//             <basic::Number>      ! (or as degrees)
//
//   next      <basic::Number>      ! "Next" steerpoint number (default: 0)
//             <basic::Identifier>  ! or by steerpoint name
//
//   action    <Action>             ! Action to be triggered at steerpoint crossing (auto only)
//                                  !  Note: the "to" steerpoint will have sequenced to the
//                                  !  next steerpoint when action is triggered. (default: 0)
//
//------------------------------------------------------------------------------
class Steerpoint : public basic::Component
{
   DECLARE_SUBCLASS(Steerpoint,basic::Component)

public:
   enum StptType { DEST, MARK, FIX, OAP, IP, TGT, TGT_GRP };

public:
    Steerpoint();

    StptType getSteerpointType() const          { return stptType; }

    // Valid data?
    bool isPosVecValid() const                  { return !needPosVec; }
    bool isLatLonValid() const                  { return !needLL; }
    bool isNavDataValid() const                 { return navDataValid; }
    bool isWarnSCA() const                      { return scaWarn; }
    bool isCmdAltValid() const                  { return haveCmdAlt; }
    bool isCmdAirspeedValid() const             { return haveCmdAs; }

    // Steerpoint parameters
    const osg::Vec3& getPosition() const        { return posVec; }
    double getLatitude() const;
    double getLongitude() const;
    LCreal getElevation() const                 { return elevation; }
    LCreal getElevationM() const                { return elevation; }
    LCreal getElevationFt() const;
    LCreal getSCA() const                       { return sca; }     // Feet
    LCreal getPTA() const                       { return pta; }     // Seconds
    const char* getDescription() const;
    LCreal getMagVarDeg() const                 { return magvar; }
    LCreal getCmdAltitude() const               { return cmdAlt; }
    LCreal getCmdAltitudeM() const              { return cmdAlt; }
    LCreal getCmdAltitudeFt() const;
    LCreal getCmdAirspeedKts() const            { return cmdAirspeed; }

    // Nav Steering: 'direct-to' data
    LCreal getTrueBrgDeg() const                { return tbrg; }
    LCreal getMagBrgDeg() const                 { return mbrg; }
    LCreal getDistNM() const                    { return dst; }
    LCreal getTTG() const                       { return ttg; }
    LCreal getCrossTrackErrNM() const           { return xte; }

    // Nav Steering: 'leg' data
    LCreal getTrueCrsDeg() const                { return tcrs; }
    LCreal getMagCrsDeg() const                 { return mcrs; }
    LCreal getLegDistNM() const                 { return tld; }
    LCreal getLegTime() const                   { return tlt; }

    // Nav Steering: 'enroute' data
    LCreal getDistEnrouteNM() const             { return tde; }
    LCreal getETE() const                       { return ete; }
    LCreal getETA() const                       { return eta; }
    LCreal getELT() const                       { return elt; }

    // Set the ground elevation at the steerpoint from this terrain database
    // Interpolate between elevation posts if the optional 'interp' flag is true.
    // Returns true if successful.
    virtual bool setElevation(const basic::Terrain* const terrain, const bool interp = false);

    // Set parameters
    virtual void setSteerpointType(const StptType t)    { stptType = t; }
    virtual void setPosition(const LCreal x, const LCreal y, const LCreal z);
    virtual void setElevation(const LCreal x);
    virtual void setPosition(const osg::Vec3& newPos);
    virtual void setLatitude(const double v);
    virtual void setLongitude(const double v);
    virtual void setPTA(const LCreal v)                 { pta = v; }
    virtual void setSCA(const LCreal v)                 { sca = v; }
    virtual void setDescription(const basic::String* const d);
    virtual void setCmdAltitude(const LCreal v);
    virtual void setCmdAirspeedKts(const LCreal v);

    // Set nav data
    virtual void setTrueBrgDeg(const LCreal v)           { tbrg = v; }
    virtual void setMagBrgDeg(const LCreal v)            { mbrg = v; }
    virtual void setDistNM(const LCreal v)               { dst = v; }
    virtual void setTTG(const LCreal v)                  { ttg = v; }
    virtual void setCrossTrackErrNM(const LCreal v)      { xte = v; }
    virtual void setTrueCrsDeg(const LCreal v)           { tcrs = v; }
    virtual void setMagCrsDeg(const LCreal v)            { mcrs = v; }
    virtual void setLegDistNM(const LCreal v)            { tld = v; }
    virtual void setLegTime(const LCreal v)              { tlt = v; }
    virtual void setDistEnrouteNM(const LCreal v)        { tde = v; }
    virtual void setETE(const LCreal v)                  { ete = v; }
    virtual void setETA(const LCreal v)                  { eta = v; }
    virtual void setELT(const LCreal v)                  { elt = v; }

    // Sets the initial lat/lon (reset()) values
    virtual void setInitLatitude(const double lat)    { initLatitude = lat; }
    virtual void setInitLongitude(const double lon)   { initLongitude = lon; }

    // Get/set steerpoint action
    Action* getAction() { return action; }              // Action to be performed
    const Action* getAction() const { return action; }  // Action to be performed
    virtual bool setAction(Action* const act);          // Sets the action to be performed

   // Compute have data 'to' this point
   virtual bool compute(const Navigation* const nav, const Steerpoint* const from = 0);
   virtual void clearNavData();

   void reset() override;

protected:

    // Slot functions
    virtual bool setSlotSteerpointType(const basic::Identifier* const msg);
    virtual bool setSlotLatitude(const basic::LatLon* const msg);
    virtual bool setSlotLatitude(const basic::Number* const msg);
    virtual bool setSlotLongitude(const basic::LatLon* const msg);
    virtual bool setSlotLongitude(const basic::Number* const msg);
    virtual bool setSlotPosition(const basic::List* const numList);
    virtual bool setSlotXPos(const basic::Distance* const num);
    virtual bool setSlotYPos(const basic::Distance* const num);
    virtual bool setSlotElevation(const basic::Distance* const num);
    virtual bool setSlotElevation(const basic::Number* const num);
    virtual bool setSlotPTA(const basic::Time* const msg);
    virtual bool setSlotPTA(const basic::Number* const msg);
    virtual bool setSlotSCA(const basic::Distance* const msg);
    virtual bool setSlotSCA(const basic::Number* const msg);
    virtual bool setSlotDescription(const basic::String* const msg);
    virtual bool setSlotMagVar(const basic::Angle* const msg);
    virtual bool setSlotMagVar(const basic::Number* const msg);
    virtual bool setSlotCmdAltitude(const basic::Distance* const num);
    virtual bool setSlotCmdAltitude(const basic::Number* const num);
    virtual bool setSlotCmdAirspeed(const basic::Number* const num);
    virtual bool setSlotNext(const basic::Identifier* const num);
    virtual bool setSlotNext(const basic::Number* const num);

    void processComponents(   // Process our subcomponent list (which should be other steerpoints)
         basic::PairStream* const list,        // Source list of components
         const std::type_info& filter,           // Type filter
         basic::Pair* const add = 0,           // Optional pair to add
         basic::Component* const remove = 0    // Optional subcomponent to remove
       ) override;

private:
    // Steerpoint parameters
    double      latitude;       // latitude
    double      longitude;      // Longitude
    LCreal      elevation;      // Elevation                (m)
    osg::Vec3   posVec;         // Position vector          (m)           [ x, y, z ] NED
    StptType    stptType;       // Steerpoint type
    LCreal      pta;            // Planned Time of Arrival  (sec)
    LCreal      sca;            // Safe Clearance Alt       (ft)
    LCreal      magvar;         // Mag Var                  (degs)
    basic::safe_ptr<const basic::String> description; // Description
    bool        needPosVec;     // Request calucaltion of 'posVec' from Lat/Lon
    bool        needLL;         // Request calucaltion of Lat/Lon from 'posVec'
    LCreal      cmdAlt;         // Commanded Altitude       (m)
    bool        haveCmdAlt;     // Have commanded altitude
    LCreal      cmdAirspeed;    // Commanded Airspeed       (kts)
    bool        haveCmdAs;      // Have commanded airspeed
    basic::safe_ptr<basic::Pair> next;  // "next" Steerpoint pair [ name steerpoint ]

    // Steerpoint action
    basic::safe_ptr<Action> action;     // Action to be performed at this steerpoint

    // Initial (reset) data
    double      initLatitude;   // latitude
    double      initLongitude;  // Longitude
    osg::Vec3   initPosVec;     // Init Position vector (m)           [ x, y, z ] NED
    LCreal      initMagVar;     // Mag Var entered for this point  (deg)
    bool        haveInitLat;    // Have initial latitude
    bool        haveInitLon;    // Have initial longitude
    bool        haveInitPos;    // Have initial pos vector
    bool        haveInitMagVar; // Have initial mag var
    LCreal      initElev;       // Elevation                  (m)
    bool        haveInitElev;   // Have initial elevation
    LCreal      initCmdAlt;     // Initial commanded altitude (m)
    bool        haveInitCmdAlt; // Have initial cmd altitude
    LCreal      initCmdAirspeed; // Initial commanded airspeed (kts)
    bool        haveInitCmdAs;   // Have commanded airspeed
    basic::safe_ptr<const basic::String> initNextStptName; // Name of the inital "next" steerpoint
    int         initNextStptIdx;    // Index of the initial "next" steerpoint

    // Computed data
    LCreal      tbrg;           // True bearing direct-to point    (deg)
    LCreal      mbrg;           // Mag bearing direct-to point     (deg)
    LCreal      dst;            // Distance direct-to point (nm)
    LCreal      ttg;            // Time-To-Go (direct)      (sec)
    LCreal      xte;            // Cross-Track Error        (nm)

    LCreal      tcrs;           // TRUE Course to point     (degs)
    LCreal      mcrs;           // Mag Course to point      (degs)
    LCreal      tlt;            // Total Time this Leg      (sec)
    LCreal      tld;            // Total Leg Distance       (nm)

    LCreal      tde;            // Total Distance Enroute   (nm)
    LCreal      ete;            // Est Time Enroute         (sec)
    LCreal      eta;            // Est Time of Arrival (UTC)(sec)
    LCreal      elt;            // Early/Late time          (sec)
    bool        scaWarn;        // Safe clearance Alt warning flag
    bool        navDataValid;   // Nav data is valid
};

} // End simulation namespace
} // End oe namespace

#endif
