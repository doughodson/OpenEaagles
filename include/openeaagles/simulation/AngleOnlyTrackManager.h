//----------------------------------------------------
// Classes: AngleOnlyTrackManager, AirAngleOnlyTrkMgr
//----------------------------------------------------
#ifndef __Eaagles_Simulation_AngleOnlyTrackManager_H__
#define __Eaagles_Simulation_AngleOnlyTrackManager_H__

#include "openeaagles/simulation/TrackManager.h"

namespace Eaagles {
namespace Simulation {
   class IrQueryMsg;
   class Track;

//==============================================================================
// Class: AngleOnlyTrackManager
// Description: Abstract class for Angle Only Track Managers
//
// Factory name: AngleOnlyTrackManager
// Slots:
//    azimuthBin    <Number>   ! Azimuth Bin (default: PI)
//    elevationBin  <Number>   ! Elevation Bin (default: PI)
//
//==============================================================================
class AngleOnlyTrackManager : public TrackManager  
{
   DECLARE_SUBCLASS(AngleOnlyTrackManager,TrackManager)

public:
   AngleOnlyTrackManager();

   virtual void newReport(IrQueryMsg* q, LCreal snDbl);

   virtual void clearTracksAndQueues();                // Clear all tracks and queues
   virtual bool addTrack(Track* const t);              // Add a track

protected:

   virtual void processTrackList(const LCreal dt) =0;                  // Derived class unique

   virtual IrQueryMsg* getQuery(LCreal* const sn);                     // Get the next 'new' report from the queue

   virtual bool setSlotAzimuthBin(const Basic::Number* const num);              // Sets azimuth bin
   virtual bool setSlotElevationBin(const Basic::Number* const num);            // Sets elevation bin
   virtual bool setSlotAlpha(const Basic::Number* const num);        // Sets alpha
   virtual bool setSlotBeta(const Basic::Number* const num);         // Sets beta

   // Basic::Component protected interface
   virtual bool shutdownNotification();

   // Prediction parameters
   LCreal              azimuthBin;         // Azimuth Bin
   LCreal              elevationBin;       // Elevation Bin
   LCreal              oneMinusAlpha;      // 1 - Alpha parameter
   LCreal              oneMinusBeta;       // 1 - Beta parameter

private:
   QQueue<IrQueryMsg*>   queryQueue;       // Emission input queue (used with the
                                           //   TrackManager::queueLock semaphore)

};

//==============================================================================
// Class: AirAngleOnlyTrkMgr
// Base class: Basic::Object -> Basic::Component -> System -> TrackManager -> AirAngleOnlyTrkMgr
//
// Description: Track Manager for A/A modes (e.g., TWS, ACM, SST)
// Factory name: AirAngleOnlyTrkMgr
//
//==============================================================================
class AirAngleOnlyTrkMgr : public AngleOnlyTrackManager  
{
    DECLARE_SUBCLASS(AirAngleOnlyTrkMgr,AngleOnlyTrackManager)
public:
    AirAngleOnlyTrkMgr();

protected:
    virtual void processTrackList(const LCreal dt);     // Process the reports into a track list
};


//==============================================================================
// Class: AirAngleOnlyTrkMgrPT
// Base class: Basic::Object -> Basic::Component -> System -> TrackManager -> AngleOnlyTrackManager -> AirAngleOnlyTrkMgrPT
//
// Description: AirAngleOnlyTrkMgr that uses perceived-truth az, el, 
// and (depending on usePerceivedPosVel) pos, vel from IR reports;
// perceived truth in that some reports are IrSensor's merged reports from two or more targets,
// azimuth, elevation, position and velocity may all be "merged" values.
//
// usePerceivedPosVel determines whether track's potentially merged position and velocity are set in the
// track, which would mean that missile would use perceived truth in its flyout, potentially flying
// towards a merged track centroid instead of toward an actual target.
//
// otherwise, uses track target position, which is a ground truth target position for one
// of the entities in the merged track
//==============================================================================
class AirAngleOnlyTrkMgrPT : public AirAngleOnlyTrkMgr
{
    DECLARE_SUBCLASS(AirAngleOnlyTrkMgrPT, AirAngleOnlyTrkMgr)
public:
    AirAngleOnlyTrkMgrPT();

protected:
   virtual void processTrackList(const LCreal dt);                  // Derived class unique
   virtual void updateTrackAges(const LCreal dt);
   virtual void removeAgedTracks();
private:
   bool usePerceivedPosVel;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
