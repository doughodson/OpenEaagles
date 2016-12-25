
#ifndef __oe_models_AngleOnlyTrackManager_H__
#define __oe_models_AngleOnlyTrackManager_H__

#include "openeaagles/models/systems/TrackManager.hpp"
#include "openeaagles/base/safe_queue.hpp"

namespace oe {
namespace models {
   class IrQueryMsg;
   class Track;

//------------------------------------------------------------------------------
// Class: AngleOnlyTrackManager
// Description: Abstract class for Angle Only Track Managers
//
// Factory name: AngleOnlyTrackManager
// Slots:
//    azimuthBin    <Number>   ! Azimuth Bin (default: PI)
//    elevationBin  <Number>   ! Elevation Bin (default: PI)
//
//------------------------------------------------------------------------------
class AngleOnlyTrackManager : public TrackManager
{
   DECLARE_SUBCLASS(AngleOnlyTrackManager, TrackManager)

public:
   AngleOnlyTrackManager();

   virtual void newReport(IrQueryMsg* q, double snDbl);

   virtual void clearTracksAndQueues() override;
   virtual bool addTrack(Track* const t) override;

protected:
   virtual IrQueryMsg* getQuery(double* const sn);                     // Get the next 'new' report from the queue

   virtual bool setSlotAzimuthBin(const base::Number* const num);      // Sets azimuth bin
   virtual bool setSlotElevationBin(const base::Number* const num);    // Sets elevation bin

   virtual bool setSlotAlpha(const base::Number* const num) override;
   virtual bool setSlotBeta(const base::Number* const num) override;

   virtual bool shutdownNotification() override;

   // Prediction parameters
   double              azimuthBin;         // Azimuth Bin
   double              elevationBin;       // Elevation Bin
   double              oneMinusAlpha;      // 1 - Alpha parameter
   double              oneMinusBeta;       // 1 - Beta parameter

private:
   base::safe_queue<IrQueryMsg*> queryQueue;  // Emission input queue (used with the
                                              //   TrackManager::queueLock semaphore)

};

//------------------------------------------------------------------------------
// Class: AirAngleOnlyTrkMgr
//
// Description: Track Manager for A/A modes (e.g., TWS, ACM, SST)
// Factory name: AirAngleOnlyTrkMgr
//
//------------------------------------------------------------------------------
class AirAngleOnlyTrkMgr : public AngleOnlyTrackManager
{
   DECLARE_SUBCLASS(AirAngleOnlyTrkMgr, AngleOnlyTrackManager)

public:
   AirAngleOnlyTrkMgr();

protected:
   virtual void processTrackList(const double dt) override;
};


//------------------------------------------------------------------------------
// Class: AirAngleOnlyTrkMgrPT
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
//------------------------------------------------------------------------------
class AirAngleOnlyTrkMgrPT : public AirAngleOnlyTrkMgr
{
   DECLARE_SUBCLASS(AirAngleOnlyTrkMgrPT, AirAngleOnlyTrkMgr)
public:
   AirAngleOnlyTrkMgrPT();

protected:
   virtual void processTrackList(const double dt) override;

   virtual void updateTrackAges(const double dt);
   virtual void removeAgedTracks();
private:
   bool usePerceivedPosVel;
};

}
}

#endif
