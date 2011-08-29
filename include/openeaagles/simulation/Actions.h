//------------------------------------------------------------------------------
// Actions -- Things to do.
//------------------------------------------------------------------------------
#ifndef __Action_H_F4772DC9_F695_4cdb_BBE2_6431B2D43A8B__
#define __Action_H_F4772DC9_F695_4cdb_BBE2_6431B2D43A8B__

#include "openeaagles/basic/Component.h"

namespace Eaagles {

   namespace Basic { class Distance; class LatLon; class Number; }

namespace Simulation {

class OnboardComputer;
class Sar;
class Steerpoint;

//------------------------------------------------------------------------------
// Class:	Action
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
// Guid: {F4772DC9-F695-4cdb-BBE2-6431B2D43A8B}
//------------------------------------------------------------------------------
class Action : public Basic::Component 
{
    DECLARE_SUBCLASS(Action,Basic::Component)

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

protected:
   OnboardComputer* getManager()   { return manager; } // Our manager
   bool setManager(OnboardComputer* const mgr);        // Set our manager

   virtual void setCompleted(const bool flg);      // Sets the completed flag

private:
   SPtr<OnboardComputer> manager;   // Our manager (only set while we're in progress)
   int            refId;            // Ref ID
   bool           completed;        // True if action has been completed
};


//------------------------------------------------------------------------------
// Class:	ActionImagingSar
// Description: SAR capture, ATR run and Image download
// Form name: ActionImagingSar
// Slots:
//   sarLatitude   <LatLon>   ! Target's latitude (LatLon) (default: 0)
//   sarLongitude  <LatLon>   ! Target's longitude (LatLon) (default: 0)
//   sarElevation  <Distance> ! Target's elevation (Distance) (default: 0)
//   resolution    <Distance> ! Image (pixel) resolution   (Distance) (default: 3)
//   imageSize     <Number>   ! Image size: height & width (pixels) (default: 512)
//
// Guid: {5B19BC09-BEC9-4752-8FA2-F43FC37472E2}
//------------------------------------------------------------------------------
class ActionImagingSar : public Action 
{
    DECLARE_SUBCLASS(ActionImagingSar,Action)
   
public:
   ActionImagingSar();

   double getSarLatitude() const       { return sarLatitude; }    // SAR latitude in degrees
   double getSarLongitude() const      { return sarLongitude; }   // SAR longitude in degrees
   double getSarElevation() const      { return sarElevation; }   // SAR elevation in meters
   LCreal getResolution() const        { return resolution; }     // Image resolution in meters
   unsigned int getImageSize() const   { return imgSize; }        // Image size
   LCreal getOrientation() const       { return orientation; }    // Planned image orientation (Deg) (true)

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

   // Action Interface
   virtual bool trigger(OnboardComputer* const mgr);  // Starts this action
   virtual bool cancel();
   virtual void process(const LCreal dt);

protected:
   Sar* getSarSystem()        { return sar; }
   virtual void setSarSystem(Sar* const p); 

   // Action Interface
   virtual void setCompleted(const bool flg);      // Sets the completed flag

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
// Class:	ActionWeaponRelease
// Description: ### Need description here
//
// Form name: ActionWeaponRelease
// Slots:
//   targetLatitude   <LatLon>  ! Target's latitude (LatLon) (default: 0)
//   targetLongitude  <LatLon>  ! Target's longitude (LatLon) (default: 0)
//   targetElevation  <Number>  ! Target's elevation (feet) (default: 0)
//   station          <Number>  ! Station number to use (default: 0)
//
// Guid: {A63DF7A5-E114-43a4-887C-469183FC960D}
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

   // Action Interface
   virtual bool trigger(OnboardComputer* const mgr);  // Starts this action

protected:
    bool setSlotTargetLat(const Basic::LatLon* newLat);
    bool setSlotTargetLon(const Basic::LatLon* newLon);
    bool setSlotTargetElev(const Basic::Number* newElev);
    bool setSlotStationNum(const Basic::Number* newStation);

private:
   double targetLatitude;        // Latitude (deg)
   double targetLongitude;       // Longitude (deg)
   double targetElevation;       // elevation (meters)
   unsigned int station;         // Station number
};

//------------------------------------------------------------------------------
// Class:	ActionDecoyRelease
// Description:  Releases a decoy 
//
// Form name: ActionDecoyRelease
// Slots:
//   numToLaunch  <Number>  ! number of decoys to launch (default: 1)
//   interval     <Number>  ! time, in seconds, between launches (default: 0)
//
// Guid: {7382B697_767A_4c91_837F_CAA410485820}
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

    // Action Interface
   virtual bool trigger(OnboardComputer* const mgr);  // Starts this action

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
// Class:	ActionCamouflageType
// Description: On trigger, switches our ownship's camouflage type
//
// Form name: ActionCamouflageType
// Slots:
//   camouflageType  <Number>  ! User-defined camouflage type (positive integer or zero for none)
//                             ! (default: 0)
//
// Guid: {5860BD7C-C17F-4486-8FDA-1E763881A658}
//------------------------------------------------------------------------------
class ActionCamouflageType : public Action 
{
    DECLARE_SUBCLASS(ActionCamouflageType,Action)
   
public:
   ActionCamouflageType();

   unsigned int getCamouflageType() const   { return camouflage; }         // Returns the user defined camouflage type (or zero for none)
   virtual bool setCamouflageType(const unsigned int v);                   // Sets the user defined camouflage type (or zero for none)
   virtual bool setSlotCamouflageType(const Basic::Number* const msg);   // Sets user defined camouflage type

   // Action Interface
   virtual bool trigger(OnboardComputer* const mgr);  // Starts this action

private:
   unsigned int camouflage;    // Camouflage type (0 is none)
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // __Action_H_F4772DC9_F695_4cdb_BBE2_6431B2D43A8B__
