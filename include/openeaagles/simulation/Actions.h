//------------------------------------------------------------------------------
// Actions -- Things to do.
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_Action_H__
#define __oe_Simulation_Action_H__

#include "openeaagles/basic/ubf/Action.h"

namespace oe {

   namespace Basic { class Distance; class LatLon; class Number; }

namespace Simulation {

class OnboardComputer;
class Sar;
class Steerpoint;

//------------------------------------------------------------------------------
// Class: Action
// Description: Actions that occur at steerpoints.
//
// Notes:
//
//  1) Use process() to process one action frame of delta time, 'dt'.  Follow
//  standard real-time rules since process() may be called from updateTC().
//
//  2) Actions that are not ready to start (isReadyToStart()) may be skipped
//     by the manager.
//
//  3) One shot action will complete during the call to trigger().
//
//  4) Any action that will take time to complete must have a manager
//     passed via trigger().
//
//  5) Derived from an UBF action (see "openeaagles/basic/ubf/Action.h").
//  The execute() function's 'actor' must be our OnboardComputer or our
//  ownship, which can be  used to find our OnboardComputer.  The execute()
//  function will find the OnboardComputer and 'trigger()' the action.
//
//------------------------------------------------------------------------------
class Action : public Basic::Ubf::Action
{
    DECLARE_SUBCLASS(Action,Basic::Ubf::Action)

public:
   Action();

   virtual bool isReadyToStart();                     // True if action is ready to start
   virtual bool isInProgress();                       // True if action is in progress
   virtual bool isCompleted();                        // True if action has been completed

   virtual bool trigger(OnboardComputer* const mgr);  // Starts this action
   virtual bool cancel();                             // Cancels this action
   virtual void process(const LCreal dt);             // Action processing

   int getRefId() const    { return refId; }          // Message Ref ID
   virtual void setRefId(const int id);               // Sets the message ref ID

   bool execute(Basic::Component* actor) override;

protected:
   OnboardComputer* getManager()   { return manager; } // Our manager
   bool setManager(OnboardComputer* const mgr);        // Set our manager

   virtual void setCompleted(const bool flg);      // Sets the completed flag

private:
   Basic::safe_ptr<OnboardComputer> manager;   // Our manager (only set while we're in progress)
   int            refId;            // Ref ID
   bool           completed;        // True if action has been completed
};


//------------------------------------------------------------------------------
// Class: ActionImagingSar
// Description: SAR capture, ATR run and Image download
// Factory name: ActionImagingSar
// Slots:
//   sarLatitude   <LatLon>   ! Target's latitude (LatLon) (default: 0)
//   sarLongitude  <LatLon>   ! Target's longitude (LatLon) (default: 0)
//   sarElevation  <Distance> ! Target's elevation (Distance) (default: 0)
//   resolution    <Distance> ! Image (pixel) resolution   (Distance) (default: 3)
//   imageSize     <Number>   ! Image size: height & width (pixels) (default: 512)
//
//------------------------------------------------------------------------------
class ActionImagingSar : public Action
{
    DECLARE_SUBCLASS(ActionImagingSar,Action)

public:
   ActionImagingSar();

   double getSarLatitude() const       { return sarLatitude;  }   // SAR latitude in degrees
   double getSarLongitude() const      { return sarLongitude; }   // SAR longitude in degrees
   double getSarElevation() const      { return sarElevation; }   // SAR elevation in meters
   LCreal getResolution() const        { return resolution;   }   // Image resolution in meters
   unsigned int getImageSize() const   { return imgSize;      }   // Image size
   LCreal getOrientation() const       { return orientation;  }   // Planned image orientation (Deg) (true)

   virtual bool setSarLatitude(const double v);       // Sets the SAR latitude in degrees
   virtual bool setSarLongitude(const double v);      // Sets the SAR longitude in degrees
   virtual bool setSarElevation(const double v);      // Sets the SAR elevation in meters
   virtual bool setResolution(const LCreal v);        // Sets the SAR image resolution (meters)
   virtual bool setImageSize(const unsigned int v);   // Sets the image size

   // Computes the planned image orientation (degs)
   virtual LCreal computeOrientation(const Steerpoint* const wp);

   // Slot functions
   virtual bool setSlotSarLat(const Basic::LatLon* const msg);
   virtual bool setSlotSarLon(const Basic::LatLon* const msg);
   virtual bool setSlotSarElev(const Basic::Distance* const msg);
   virtual bool setSlotResolution(const Basic::Distance* const msg);
   virtual bool setSlotImageSize(const Basic::Number* const msg);

   bool trigger(OnboardComputer* const mgr) override;
   bool cancel() override;
   void process(const LCreal dt) override;

protected:
   Sar* getSarSystem()        { return sar; }
   virtual void setSarSystem(Sar* const p);

   void setCompleted(const bool flg) override;

private:
   double sarLatitude;        // Latitude (deg)
   double sarLongitude;       // Longitude (deg)
   double sarElevation;       // Elevation (meters)
   LCreal resolution;         // Image resolution (meters)
   LCreal orientation;        // orientation (true) (degrees)
   unsigned int imgSize;      // Image size (pixels)
   double timer;              // Time-Out timer
   Sar* sar;                  // SAR system
};


//------------------------------------------------------------------------------
// Class: ActionWeaponRelease
// Description: Launches a weapon
//
// Factory name: ActionWeaponRelease
// Slots:
//   targetLatitude   <LatLon>  ! Target's latitude (LatLon) (default: 0)
//   targetLongitude  <LatLon>  ! Target's longitude (LatLon) (default: 0)
//   targetElevation  <Number>  ! Target's elevation (feet) (default: 0)
//   station          <Number>  ! Station number to use (default: 0)
//
//------------------------------------------------------------------------------
class ActionWeaponRelease : public Action
{
    DECLARE_SUBCLASS(ActionWeaponRelease,Action)

public:
   ActionWeaponRelease();

   double getTargetLatitude() const       { return targetLatitude; }    // Target latitude in degrees
   double getTargetLongitude() const      { return targetLongitude; }   // Target longitude in degrees
   double getTargetElevation() const      { return targetElevation; }   // Target elevation in meters
   unsigned int getStation() const        { return station; }           // Weapon station number

   // Sets the target location (latitude & longitude in radians, elevation in meters)
   virtual bool setTargetLocation(const double tgtLat, const double tgtLon, const double tgtElev);

   // Set planned station number
   virtual bool setStation(const unsigned int num);

   bool trigger(OnboardComputer* const mgr) override;

protected:
   bool setSlotTargetLat(const Basic::LatLon* newLat);
   bool setSlotTargetLon(const Basic::LatLon* newLon);
   bool setSlotTargetElev(const Basic::Number* newElev);
   bool setSlotStationNum(const Basic::Number* newStation);

private:
   double targetLatitude;        // latitude (deg)
   double targetLongitude;       // longitude (deg)
   double targetElevation;       // elevation (meters)
   unsigned int station;         // station number
};

//------------------------------------------------------------------------------
// Class: ActionDecoyRelease
// Description:  Releases a decoy
//
// Factory name: ActionDecoyRelease
// Slots:
//   numToLaunch  <Number>  ! number of decoys to launch (default: 1)
//   interval     <Number>  ! time, in seconds, between launches (default: 0)
//
//------------------------------------------------------------------------------
class ActionDecoyRelease : public Action
{
    DECLARE_SUBCLASS(ActionDecoyRelease,Action)

public:
    ActionDecoyRelease();

    // set functions
    virtual bool setInterval(const LCreal x) { interval = x; return true; }
    virtual bool setNumToLaunch(const int x) { numToLaunch = x; return true; }
    virtual void process(const LCreal dt);

    bool trigger(OnboardComputer* const mgr) override;

protected:
    bool setSlotInterval(const Basic::Number* x);
    bool setSlotNumToLaunch(const Basic::Number* x);

private:
    int numToLaunch;    // how many decoys to launch this action?
    LCreal interval;    // seconds delay between launch... default is 0
    double tod;         // the sim time of day (we are going to add to it)
    double startTOD;    // our starting tod (that way we know when we've elapsed so far)
};


//------------------------------------------------------------------------------
// Class: ActionCamouflageType
// Description: On trigger, switches our ownship's camouflage type
//
// Factory name: ActionCamouflageType
// Slots:
//   camouflageType  <Number>  ! User-defined camouflage type (positive integer or zero for none)
//                             ! (default: 0)
//
//------------------------------------------------------------------------------
class ActionCamouflageType : public Action
{
    DECLARE_SUBCLASS(ActionCamouflageType,Action)

public:
   ActionCamouflageType();

   unsigned int getCamouflageType() const   { return camouflage; }         // Returns the user defined camouflage type (or zero for none)
   virtual bool setCamouflageType(const unsigned int v);                   // Sets the user defined camouflage type (or zero for none)
   virtual bool setSlotCamouflageType(const Basic::Number* const msg);   // Sets user defined camouflage type

   virtual bool trigger(OnboardComputer* const mgr) override;

private:
   unsigned int camouflage;    // Camouflage type (0 is none)
};

} // End Simulation namespace
} // End oe namespace

#endif
