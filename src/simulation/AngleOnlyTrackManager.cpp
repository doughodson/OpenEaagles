

#include "openeaagles/simulation/AngleOnlyTrackManager.h"
#include "openeaagles/simulation/IrQueryMsg.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/TabLogger.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/units/Times.h"

#include "openeaagles/simulation/DataRecorder.h"
#include "openeaagles/simulation/Simulation.h"

#include <cmath>

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class: AngleOnlyTrackManager
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(AngleOnlyTrackManager,"AngleOnlyTrackManager")

// Slot table
BEGIN_SLOTTABLE(AngleOnlyTrackManager)
    "azimuthBin",       // 1: Azimuth Bin
    "elevationBin",     // 2: Elevation Bin
END_SLOTTABLE(AngleOnlyTrackManager)

//  Map slot table
BEGIN_SLOT_MAP(AngleOnlyTrackManager)
    ON_SLOT(1, setSlotAzimuthBin, Basic::Number)
    ON_SLOT(2, setSlotElevationBin, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
AngleOnlyTrackManager::AngleOnlyTrackManager() : queryQueue(MAX_TRKS)
{
    STANDARD_CONSTRUCTOR()

    oneMinusAlpha = 0.0;
    oneMinusBeta = 1.0;
    azimuthBin = static_cast<LCreal>(PI);
    elevationBin = static_cast<LCreal>(PI);
}

AngleOnlyTrackManager::AngleOnlyTrackManager(const AngleOnlyTrackManager& org) : queryQueue(MAX_TRKS)
{
    STANDARD_CONSTRUCTOR()
    copyData(org, true);
}

AngleOnlyTrackManager::~AngleOnlyTrackManager()
{
    STANDARD_DESTRUCTOR()
}

AngleOnlyTrackManager& AngleOnlyTrackManager::operator=(const AngleOnlyTrackManager& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

AngleOnlyTrackManager* AngleOnlyTrackManager::clone() const
{
    return nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void AngleOnlyTrackManager::copyData(const AngleOnlyTrackManager& org, const bool)
{
    BaseClass::copyData(org);

    azimuthBin = org.azimuthBin;
    elevationBin = org.elevationBin;
    oneMinusAlpha = org.oneMinusAlpha;
    oneMinusBeta = org.oneMinusBeta;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void AngleOnlyTrackManager::deleteData()
{
    clearTracksAndQueues();
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::shutdownNotification()
{
   clearTracksAndQueues();
   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// clearTracksAndQueues() -- Clear all tracks and queues
//------------------------------------------------------------------------------
void AngleOnlyTrackManager::clearTracksAndQueues()
{
    // ---
    // Clear out the queue(s)
    // ---
    lcLock(queueLock);
    for (IrQueryMsg* q = queryQueue.get(); q != nullptr; q = queryQueue.get()) {
        q->unref();     // unref() the IR query message
        snQueue.get();  // and every IR query message had a S/N value
    }
    lcUnlock(queueLock);

    // ---
    // Clear the track list
    // ---
    lcLock(trkListLock);
    unsigned int n = nTrks;
    nTrks = 0;
    for (unsigned int i = 0; i < n; i++) {
        tracks[i]->clear();
        tracks[i]->unref();
        tracks[i] = nullptr;
    }
    lcUnlock(trkListLock);
}

//------------------------------------------------------------------------------
// newReport() -- Accept a new IR Query Message report
//------------------------------------------------------------------------------
void AngleOnlyTrackManager::newReport(IrQueryMsg* q, LCreal sn)
{
    // Queue up IR query messages reports
    if (q != nullptr) {
        q->ref();
        lcLock(queueLock);
        queryQueue.put(q);
        snQueue.put(sn);
        lcUnlock(queueLock);
    }
}

//------------------------------------------------------------------------------
// getQuery() -- Get the next 'new' report of the queue
//------------------------------------------------------------------------------
IrQueryMsg* AngleOnlyTrackManager::getQuery(LCreal* const sn)
{
    IrQueryMsg* q = nullptr;

    lcLock(queueLock);
    q = queryQueue.get();
    if (q != nullptr) {
        *sn = snQueue.get();
    }
    lcUnlock(queueLock);

    return q;
}

//------------------------------------------------------------------------------
// addTrack() -- Add a track to the list
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::addTrack(Track* const t)
{
    bool ok = false;

    lcLock(trkListLock);
    if (nTrks < maxTrks) {
        t->ref();
        tracks[nTrks++] = t;
        ok = true;
    }
    lcUnlock(trkListLock);

    return ok;
}

//------------------------------------------------------------------------------
// Sets azimuth bin
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::setSlotAzimuthBin(const Basic::Number* const msg)
{
    LCreal value = 0.0;

    const Basic::Angle* a = dynamic_cast<const Basic::Angle*>(msg);
    if (a != nullptr) {
        Basic::Radians r;
        value = static_cast<LCreal>(r.convert(*a));
    }
    else if (msg != nullptr) {
        value = msg->getReal();
    }

    azimuthBin = value;

    return true;
}

//------------------------------------------------------------------------------
// Sets azimuth bin
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::setSlotElevationBin(const Basic::Number* const msg)
{
    LCreal value = 0.0;

    const Basic::Angle* a = dynamic_cast<const Basic::Angle*>(msg);
    if (a != nullptr) {
        Basic::Radians r;
        value = static_cast<LCreal>(r.convert(*a));
    }
    else if (msg != nullptr) {
        value = msg->getReal();
    }

    elevationBin = value;

    return true;
}

//------------------------------------------------------------------------------
// Sets alpha
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::setSlotAlpha(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = true;
        alpha = msg->getReal();
        oneMinusAlpha = 1.0 - alpha;
    }
    return ok;
}

//------------------------------------------------------------------------------
// Sets beta
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::setSlotBeta(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = true;
        beta = msg->getReal();
        oneMinusBeta = 1.0 - beta;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* AngleOnlyTrackManager::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& AngleOnlyTrackManager::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    // Max number of tracks
    indent(sout,i+j);
    sout << "azimuthBin: ";
    sout << azimuthBin;
    sout << std::endl;

    // Max track age (seconds)
    indent(sout,i+j);
    sout << "elevationBin: ";
    sout << elevationBin;
    sout << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}


//==============================================================================
// Class: AirAngleOnlyTrkMgr
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(AirAngleOnlyTrkMgr,"AirAngleOnlyTrkMgr")

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AirAngleOnlyTrkMgr::AirAngleOnlyTrkMgr()
{
    STANDARD_CONSTRUCTOR()
    setType( Track::ONBOARD_SENSOR_BIT | Track::AIR_TRACK_BIT );
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgr::copyData(const AirAngleOnlyTrkMgr& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgr::deleteData()
{
}

//------------------------------------------------------------------------------
// processTrackList() -- process the track list
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgr::processTrackList(const LCreal dt)
{
    LCreal tmp;

    // Make sure we have an ownship to work with
    Player* ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
    if (ownship == nullptr || dt == 0.0) return;

    // Make sure we have the A and B matrix
    //if (!haveMatrixA) makeMatrixA(dt);   // not used

    // Report to Track matching matrix
    unsigned int report2TrackMatch[MAX_REPORTS][MAX_TRKS];    // Report/Track matched matrix
    unsigned int reportNumMatches[MAX_REPORTS];               // Number of matches for each report
    unsigned int trackNumMatches[MAX_TRKS];                   // Number of matcher for each track

    // ---
    // 1)Age the tracks by delta time
    // ---
    const osg::Vec3 osVel = ownship->getVelocity();
    const osg::Vec3 osAccel = ownship->getAcceleration();
    const LCreal osGndTrk = ownship->getGroundTrack();
    lcLock(trkListLock);
    for (unsigned int i = 0; i < nTrks; i++) {
        tracks[i]->ownshipDynamics(osGndTrk, osVel, osAccel, dt);
        tracks[i]->updateTrackAge(dt);
    }
    lcUnlock(trkListLock);

    // ---
    // 2) Process new reports
    // ---

    // Get each new IR query message report from the queue
    unsigned int nReports = 0;
    IrQueryMsg* queryMessages[MAX_REPORTS];
    LCreal newSignal[MAX_REPORTS];
    LCreal newElevation[MAX_REPORTS];
    LCreal newAzimuth[MAX_REPORTS];
    for (IrQueryMsg* q = getQuery(&tmp); q != nullptr && nReports < MAX_REPORTS; q = getQuery(&tmp)) {
        Player* tgt = q->getTarget();

        bool dummy = false;
        if (tgt->isMajorType(Player::WEAPON)) {
            dummy = (static_cast<const Weapon*>(tgt))->isDummy();
        }

        if ( tgt->isMajorType(Player::AIR_VEHICLE) ||
            tgt->isMajorType(Player::SHIP) ||
            (tgt->isMajorType(Player::WEAPON) && !dummy)
            ) {
                // Using only air vehicles
                queryMessages[nReports] = q;
                newSignal[nReports] = tmp;
                //           newAzimuth[nReports] = queryMessages[nReports]->getAzimuthAoi();
                reportNumMatches[nReports] = 0;

                newAzimuth[nReports] = queryMessages[nReports]->getRelativeAzimuth();
                newElevation[nReports] = queryMessages[nReports]->getRelativeElevation();
                nReports++;
        }
        else {
            // Free up IR query messages from other types of players
            q->unref();
        }
    }

    // Clean out the rest of the queue, if we had more than the maximum IR query messages
    for (IrQueryMsg* q = getQuery(&tmp); q != nullptr; q = getQuery(&tmp)) {
        q->unref();
    }

    // ---
    // 3) Match current tracks to new reports (observations)
    // ---
    lcLock(trkListLock);
    for (unsigned int it = 0; it < nTrks; it++) {
        trackNumMatches[it] = 0;
        const IrTrack* const trk = static_cast<const IrTrack*>(tracks[it]);  // we produce only IrTracks

        for (unsigned int ir = 0; ir < nReports; ir++) {
            // use ground truth to match new reports to existing tracks
            if (trk->getTarget() == queryMessages[ir]->getTarget()) {
                // We have a new report for the same target as this track ...
                report2TrackMatch[ir][it] = 1;
                trackNumMatches[it]++;
                reportNumMatches[ir]++;
            }
            else report2TrackMatch[ir][it] = 0;
        }
    }
    lcUnlock(trkListLock);

    // ---
    // 4) Apply rules to associate the proper report to track.
    // ---

    // No Rules -- Just Right

    // ---
    // 5) Create inputs for current tracks
    // ---
    LCreal uAzimuth[MAX_TRKS];
    LCreal uElevation[MAX_TRKS];
    LCreal age[MAX_TRKS];
    bool haveU[MAX_TRKS];
    lcLock(trkListLock);
    for (unsigned int it = 0; it < nTrks; it++) {
        haveU[it] = false;
        age[it] = tracks[it]->getTrackAge();
        if (trackNumMatches[it] > 0) {
            for (unsigned int ir = 0; ir < nReports; ir++) {
                if (report2TrackMatch[ir][it] > 0) {
                    IrTrack* const trk = static_cast<IrTrack*>(tracks[it]);  // we produce only IrTracks

                    // Update the track's signal
                    trk->setSignal(newSignal[ir],queryMessages[ir]);

                    uAzimuth[it] = newAzimuth[ir] - trk->getRelAzimuth();
                    uElevation[it] = newElevation[ir] - trk->getElevation();

                    // Track age and flags
                    if ((!haveU[it]) && (dt > 0.0f)) {
                        tracks[it]->resetTrackAge();
                        haveU[it] = true;
                    }
                }
            }
        }
    }
    lcUnlock(trkListLock);

    // ---
    // 6) Smooth and predict position for the next frame
    //
    //    X(k+1) = A*X(k) + B*U(k)
    //    where:
    //      X(k) is the state vector [ pos vel accel ]
    //      U(k) is the difference between the observed & predicted positions
    // ---
    lcLock(trkListLock);
    for (unsigned int i = 0; i < nTrks; i++) {
        // Save X(k)
        LCreal taz = tracks[i]->getRelAzimuth();
        LCreal tazRate = tracks[i]->getRelAzimuthRate();
        LCreal tazAccel = tracks[i]->getRelAzimuthAcceleration();
        LCreal tel = tracks[i]->getElevation();
        LCreal telRate = tracks[i]->getElevationRate();
        LCreal telAccel = tracks[i]->getElevationAcceleration();

        LCreal ageAtNextTimeStep = dt;

        if (haveU[i]) {
            tracks[i]->setPosition(tracks[i]->getTarget()->getPosition() - ownship->getPosition());

            // If the track is updated set the new azimuth and elevation data based on
            // a weighted average of the updated value and the predicted value from the
            // previous time step
            const LCreal pTaz = tracks[i]->getPredictedAzimuth();
            const LCreal pTazRate = tracks[i]->getPredictedAzimuthRate();
            const LCreal pTel = tracks[i]->getPredictedElevation();
            const LCreal pTelRate = tracks[i]->getPredictedElevationRate();

            tracks[i]->setRelAzimuth((pTaz * oneMinusAlpha) + ((taz + uAzimuth[i]) * alpha));
            tracks[i]->setRelAzimuthRate((pTazRate * oneMinusBeta) + ((uAzimuth[i] / age[i]) * beta ));
            tracks[i]->setRelAzimuthAcceleration((tracks[i]->getRelAzimuthRate() - tazRate) / age[i]);
            tracks[i]->setElevation((pTel * oneMinusAlpha) + ((tel + uElevation[i]) * alpha));
            tracks[i]->setElevationRate((pTelRate * oneMinusBeta) + ((uElevation[i] / age[i]) * beta ));
            tracks[i]->setElevationAcceleration((tracks[i]->getElevationRate() - telRate) / age[i]);

            if (getLogTrackUpdates()) {
               // Object 1: player, Object 2: Track Data
               BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_TRACK_DATA )
                  SAMPLE_2_OBJECTS( ownship, tracks[i] )
               END_RECORD_DATA_SAMPLE()
            }

            // TabLogger is deprecated
            if (getLogTrackUpdates()  &&  (getAnyEventLogger() != nullptr)) {
                TabLogger::TabLogEvent* evt = new TabLogger::LogPassiveTrack(2, this,tracks[i]); // type 2 for "update"
                getAnyEventLogger()->log(evt);
                evt->unref();
            }

            taz = tracks[i]->getRelAzimuth();
            tazRate = tracks[i]->getRelAzimuthRate();
            tazAccel = tracks[i]->getRelAzimuthAcceleration();
            tel = tracks[i]->getElevation();
            telRate = tracks[i]->getElevationRate();
            telAccel = tracks[i]->getElevationAcceleration();

        }
        else {
            ageAtNextTimeStep += age[i];
        }

        // Project the values for the next time step

        const LCreal deltaTimeSquaredOverTwo = (ageAtNextTimeStep* ageAtNextTimeStep) / 2.0f;

        tracks[i]->setPredictedAzimuth(taz + tazRate*ageAtNextTimeStep
            + tazAccel*deltaTimeSquaredOverTwo);
        tracks[i]->setPredictedAzimuthRate(tazRate + tazAccel*ageAtNextTimeStep);
        tracks[i]->setPredictedElevation(tel + telRate*ageAtNextTimeStep
            + telAccel*deltaTimeSquaredOverTwo);
        tracks[i]->setPredictedElevationRate(telRate + telAccel*ageAtNextTimeStep);

    }
    lcUnlock(trkListLock);

    // ---
    // 7) For tracks with new observation reports, reset their age.
    //    Remove tracks that have age beyond the limit.
    // ---
    lcLock(trkListLock);
    for (unsigned int it = 0; it < nTrks; /* update 'it' below */ ) {
        IrTrack* const trk = static_cast<IrTrack*>(tracks[it]);  // we produce only IrTracks
        if (trk->getTrackAge() >= getMaxTrackAge()) {
            //if (isMessageEnabled(MSG_INFO)) {
            //   std::cout << "Removed Aged AIR track[it] = [" << it << "] id = " << trk->getTrackID() << std::endl;
            //}

             // Object 1: player, Object 2: Track Data
            Player* ownship = getOwnship();
            BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_TRACK_REMOVED )
               SAMPLE_2_OBJECTS( ownship, trk )
            END_RECORD_DATA_SAMPLE()

            // TabLogger is deprecated
            if (getAnyEventLogger() != nullptr) {
                TabLogger::TabLogEvent* evt = new TabLogger::LogPassiveTrack(3, this,trk); // type 3 for "remove"
                getAnyEventLogger()->log(evt);
                evt->unref();
            }

            // Track has timed out -- delete the track and ...
            if (isMessageEnabled(MSG_INFO)) {
                std::cout << "Removed Aged AIR track: player: " << trk->getLastQuery()->getTarget();
                std::cout << ", refCnt = " << trk->getLastQuery()->getTarget()->getRefCount() << std::endl;
            }

            trk->setType(0);
            trk->unref();
            nTrks--;
            // move all other tracks down in the list.
            for (unsigned int it2 = it; it2 < nTrks; it2++) {
                tracks[it2] = tracks[it2+1];
            }
        }
        else {
            // update track index, it
            it++;
        }
    }
    lcUnlock(trkListLock);

    // ---
    // 8) Create new tracks from unmatched reports and free up IR query messages
    // ---
    lcLock(trkListLock);
    for (unsigned int i = 0; i < nReports; i++) {
        if ((reportNumMatches[i] == 0) && (nTrks < maxTrks)) {
            // This is a new report, so create a new track for it
            IrTrack* newTrk = new IrTrack();
            newTrk->setTrackID( getNewTrackID() );
            newTrk->setTarget( queryMessages[i]->getTarget() );
            newTrk->setType(Track::AIR_TRACK_BIT | Track::ONBOARD_SENSOR_BIT);
            newTrk->setPosition(queryMessages[i]->getTarget()->getPosition() - ownship->getPosition());
            newTrk->setVelocity(queryMessages[i]->getTarget()->getVelocity() - ownship->getVelocity());
            newTrk->setRelAzimuth(queryMessages[i]->getRelativeAzimuth());
            newTrk->setElevation(queryMessages[i]->getRelativeElevation());
            newTrk->setPredictedAzimuth(queryMessages[i]->getRelativeAzimuth());
            newTrk->setPredictedElevation(queryMessages[i]->getRelativeElevation());
            newTrk->setSignal(newSignal[i],queryMessages[i]);

            if (isMessageEnabled(MSG_INFO)) {
                std::cout << "New AIR track[it] = [" << nTrks << "] id = " << newTrk->getTrackID() << std::endl;
            }

            // Object 1: player, Object 2: Track Data
            BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_NEW_TRACK )
               SAMPLE_2_OBJECTS( ownship, newTrk )
            END_RECORD_DATA_SAMPLE()

            // TabLogger is deprecated
            if (getAnyEventLogger() != nullptr) {
                TabLogger::TabLogEvent* evt = new TabLogger::LogPassiveTrack(1, this,newTrk); // type 1 for "new"
                getAnyEventLogger()->log(evt);
                evt->unref();
            }
            tracks[nTrks++] = newTrk;
        }
        // Free the IR query message report
        queryMessages[i]->unref();
    }
    lcUnlock(trkListLock);

}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& AirAngleOnlyTrkMgr::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}



//==============================================================================
// Class: AirAngleOnlyTrkMgrPT - perceived truth version of above
//==============================================================================

// this tm uses merged tracks for its attempts at managing tracks. This means it is
// perceived az and el for managing tracks.

// usePerceivedData determines whether track's potentially merged position and velocity are set in the
// track, which would mean that missile would use perceived truth in its flyout, potentially flying
// towards a merged track centroid instead of toward an actual target.
//
// if !usePerceivedData, it uses track target position, which is a ground truth target position for one
// of the entities in the merged track

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(AirAngleOnlyTrkMgrPT,"AirAngleOnlyTrkMgrPT")
EMPTY_DELETEDATA(AirAngleOnlyTrkMgrPT)
EMPTY_SERIALIZER(AirAngleOnlyTrkMgrPT)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AirAngleOnlyTrkMgrPT::AirAngleOnlyTrkMgrPT()
{
    STANDARD_CONSTRUCTOR()
    usePerceivedPosVel = true;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgrPT::copyData(const AirAngleOnlyTrkMgrPT& org, const bool)
{
    BaseClass::copyData(org);
    usePerceivedPosVel = org.usePerceivedPosVel;
}


//------------------------------------------------------------------------------
// Age the tracks by delta time
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgrPT::updateTrackAges(const LCreal dt)
{
    const osg::Vec3 osVel = getOwnship()->getVelocity();
    const osg::Vec3 osAccel = getOwnship()->getAcceleration();
    const LCreal osGndTrk = getOwnship()->getGroundTrack();
    lcLock(trkListLock);
    for (unsigned int i = 0; i < nTrks; i++) {
        tracks[i]->ownshipDynamics(osGndTrk, osVel, osAccel, dt);
        tracks[i]->updateTrackAge(dt);
    }
    lcUnlock(trkListLock);
}

//------------------------------------------------------------------------------
// Remove tracks that have age beyond the limit.
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgrPT::removeAgedTracks()
{
    lcLock(trkListLock);
    for (unsigned int it = 0; it < nTrks; /* update 'it' below */ ) {
        IrTrack* const trk = static_cast<IrTrack*>(tracks[it]);  // we produce only IrTracks
        if (trk->getTrackAge() >= getMaxTrackAge()) {
            //if (isMessageEnabled(MSG_INFO)) {
            //   std::cout << "Removed Aged AIR track[it] = [" << it << "] id = " << trk->getTrackID() << std::endl;
            //}
            // Object 1: player, Object 2: Track Data
           Player* ownship = getOwnship();
           BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_TRACK_REMOVED )
              SAMPLE_2_OBJECTS( ownship, trk )
           END_RECORD_DATA_SAMPLE()

            // TabLogger is deprecated
            if (getAnyEventLogger() != nullptr) {
                TabLogger::TabLogEvent* evt = new TabLogger::LogPassiveTrack(3, this,trk); // type 3 for "remove"
                getAnyEventLogger()->log(evt);
                evt->unref();
            }
            // Track has timed out -- delete the track and ...
            if (isMessageEnabled(MSG_INFO)) {
                std::cout << "Removed Aged AIR track: player: " << trk->getLastQuery()->getTarget();
                std::cout << ", refCnt = " << trk->getLastQuery()->getTarget()->getRefCount() << std::endl;
            }

            trk->setType(0);
            trk->unref();
            nTrks--;
            // move all other tracks down in the list.
            for (unsigned int it2 = it; it2 < nTrks; it2++) {
                tracks[it2] = tracks[it2+1];
            }
        }
        else {
            // update track index, it
            it++;
        }
    }
    lcUnlock(trkListLock);
}

//------------------------------------------------------------------------------
// processTrackList() -- process the track list
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgrPT::processTrackList(const LCreal dt)
{
    // Make sure we have an ownship to work with
    Player* ownship = getOwnship();
    if (ownship == nullptr) return;

    IrQueryMsg* queryMessages[MAX_REPORTS];
    unsigned int reportNumMatches[MAX_REPORTS];               // Number of matches for each report
    unsigned int nReports = 0;

    // ---
    // 1)Age existing tracks by delta time
    // ---
    updateTrackAges(dt);


    // ---
    // 2) Process new reports
    // ---
    // Get each new IR query message report from the queue
    LCreal tmp;
    for (IrQueryMsg* q = getQuery(&tmp); q != nullptr && nReports < MAX_REPORTS; q = getQuery(&tmp)) {
        Player* tgt = q->getTarget();

        bool dummy = false;
        if (tgt->isMajorType(Player::WEAPON)) {
            dummy = (static_cast<const Weapon*>(tgt))->isDummy();
        }

        if ( tgt->isMajorType(Player::AIR_VEHICLE) ||
            tgt->isMajorType(Player::SHIP) ||
            (tgt->isMajorType(Player::WEAPON) && !dummy)
            ) {
                // Using only air vehicles
                queryMessages[nReports] = q;
                reportNumMatches[nReports] = 0;
                nReports++;
        }
        else {
            // Free up IR query messages from other types of players
            q->unref();
        }
    }

    // Clean out the rest of the queue, if we had more than the maximum IR query messages
    for (IrQueryMsg* q = getQuery(&tmp); q != nullptr; q = getQuery(&tmp)) {
        q->unref();
    }


    if (nTrks > 0) {
        // Report to Track matching matrix
        unsigned int report2TrackMatch[MAX_REPORTS][MAX_TRKS];    // Report/Track matched matrix
        unsigned int trackNumMatches[MAX_TRKS];                   // Number of matcher for each track
        for (unsigned int ii = 0; ii < MAX_TRKS; ii++) trackNumMatches[ii] = 0;

        if (nReports > 0) {

            // ---
            // 3) Match new reports (observations) to all potential track matches
            // ---
            lcLock(trkListLock);
            for (unsigned int it = 0; it < nTrks; it++) {
                trackNumMatches[it] = 0;
                const IrTrack* const trk = static_cast<const IrTrack*>(tracks[it]);  // we produce only IrTracks
                for (unsigned int ir = 0; ir < nReports; ir++) {
                    LCreal azDiff = queryMessages[ir]->getRelativeAzimuth() - trk->getPredictedAzimuth();
                    if (azDiff < 0.0f) azDiff = 0.0f - azDiff;
                    LCreal elDiff = queryMessages[ir]->getRelativeElevation() - trk->getPredictedElevation();
                    if (elDiff < 0.0f) elDiff = 0.0f - elDiff;
                    if ((azDiff < azimuthBin) && (elDiff < elevationBin)) {
                        // We have a new report for the same target as this track ...
                        report2TrackMatch[ir][it] = 1;
                        trackNumMatches[it]++;
                        reportNumMatches[ir]++;
                    }
                    else
                        report2TrackMatch[ir][it] = 0;
                }
            }
            lcUnlock(trkListLock);


            // ---
            // 4) Apply rules to associate the proper report to track.
            // ---

            // prevent one report assigned to multiple tracks
            // and attempt to distribute reports so that one report per track
            // and not two reports in one track and none in another
            // algorithm does not otherwise prevent multiple reports to one track
            // and note that 5) below only uses the last of multiple reports matched to a track

            lcLock(trkListLock);

            // using two passes so that we prioritize assigning reports to later tracks for both situations
            // avoid assigning multiple reports to one track
            // attempt to distribute reports to tracks 1->1
            // with priority to later tracks

            // this pass deals with case if one report matches to multiple tracks,
            // and also one track matches to multiple reports
            // by prioritizing assignment of reports to newest tracks
            // this does no assignment of reports to tracks, it removes prospective assignments
            for (unsigned int it = 0; it < nTrks; it++) {
                for (unsigned int ir = 0; ir < nReports; ir++) {
                    if (report2TrackMatch[ir][it]==1 && reportNumMatches[ir]>1 && trackNumMatches[it]>1) {
                        // there are multiple matches for this report and for this track
                        // disable this match, since report and track each have a subsequent match to use
                        // iterating up, prioritizes assignment of reports to later tracks
                        report2TrackMatch[ir][it] = 0;
                        trackNumMatches[it]--;
                        reportNumMatches[ir]--;
                    }
                }
            }

            // prevent matching one report to multiple tracks;
            // prioritize matching to the last of the matching tracks
            for (int it = (nTrks-1); it >= 0; it--) {
                for (unsigned int ir = 0; ir < nReports; ir++) {
                    if (report2TrackMatch[ir][it] == 1) {
                        if (reportNumMatches[ir]>1 && trackNumMatches[it] == 1) {
                            // assign this report to this track but prevent assigning to any others
                            reportNumMatches[ir]=0;
                        }
                        else if (reportNumMatches[ir]==0) {
                            // this report previously assigned to another track - disable this match
                            report2TrackMatch[ir][it] = 0;
                        }
                    }
                }
            }
            lcUnlock(trkListLock);

        } // endif nReports > 0

        // ---
        // 5) Create inputs for current tracks
        //
        // note that 5) and 6) are now processed together in one loop on track[]
        // ---
        lcLock(trkListLock);
        for (unsigned int it = 0; it < nTrks; it++) {
            osg::Vec3 uPosition;
            osg::Vec3 uVelocity;
            LCreal uAzimuth(0.0);
            LCreal uElevation(0.0);
            //LCreal age = tracks[it]->getTrackAge();
            bool haveU = false;

            // ---
            // Process new input data for current tracks
            // ---
            if (trackNumMatches[it] > 0) {
                for (unsigned int ir = 0; ir < nReports; ir++) {
                    if (report2TrackMatch[ir][it] > 0) {
                        IrTrack* const irTrk = static_cast<IrTrack*>(tracks[it]);  // we produce only IrTracks

                        // Update the track's signal
                        irTrk->setSignal(queryMessages[ir]->getSignalToNoiseRatio(), queryMessages[ir]);

                        if (isMessageEnabled(MSG_DEBUG)) {
                            if (irTrk->getLastQuery()->getQueryMergeStatus() == IrQueryMsg::MERGED)
                                std::cout << it << " snDbl-merged " << queryMessages[ir]->getSignalToNoiseRatio() << std::endl;
                            else
                                std::cout << it << " snDbl " << queryMessages[ir]->getSignalToNoiseRatio() << std::endl;
                        }

                        uAzimuth = queryMessages[ir]->getRelativeAzimuth() - irTrk->getRelAzimuth();
                        uElevation = queryMessages[ir]->getRelativeElevation() - irTrk->getElevation();

                        if (usePerceivedPosVel) {
                            // to use PT data, need to store PT pos and vel from query for use in track prediction (next)
                            uPosition = queryMessages[ir]->getPosVec();
                            uVelocity = queryMessages[ir]->getVelocityVec();
                        }

                        // Track age and flags
                        //if (age[it]>0.03 && (!haveU[it]) && (dt > 0.0f)) { // force gaps in data to test prediction
                        if ((!haveU) && (dt > 0.0)) {
                            tracks[it]->resetTrackAge();
                            haveU = true;
                        }
                    }
                }
            }

            // finished processing new inputs for this track.
            // but note that we only look at the last report that is matched to this track...


            // ---
            // 6) Smooth and predict position for the next frame
            //
            //   X(k+1) = A X(k) + (1-A)pX(k) for az and el
            //   X(k+1) = B X(k) + (1-B)pX(k) for azrate and elrate

            // Save X(k)
            LCreal taz = tracks[it]->getRelAzimuth();
            LCreal tazRate = tracks[it]->getRelAzimuthRate();
            LCreal tazAccel = tracks[it]->getRelAzimuthAcceleration();
            LCreal tel = tracks[it]->getElevation();
            LCreal telRate = tracks[it]->getElevationRate();
            LCreal telAccel = tracks[it]->getElevationAcceleration();

            LCreal ageAtNextTimeStep = dt;

            if (haveU) {
                // missiles need pos + vel for guidance; update pos and vel for missile dynamics, using reported/perceived position and velocity
                if (usePerceivedPosVel) {
                    tracks[it]->setPosition(uPosition - getOwnship()->getPosition());
                    tracks[it]->setVelocity(uVelocity - getOwnship()->getVelocity());
                }
                else {
                    // using ground truth to calc position and velocity
                    tracks[it]->setPosition(tracks[it]->getTarget()->getPosition() - getOwnship()->getPosition());
                    tracks[it]->setVelocity(tracks[it]->getTarget()->getVelocity() - getOwnship()->getVelocity());
                }

                // a track's target can change w/o track changing - sync track target with target from most recent query return
                IrTrack* trk = dynamic_cast<IrTrack*>(tracks[it]);
                if ( trk && (trk->getLastQuery()->getTarget() != trk->getTarget()) ) {
                    const Player* tgt = trk->getLastQuery()->getTarget();
                    Player *ttgt = const_cast<Player*>(tgt);
                    // if track contained merged targets, a track's target may change
                    tracks[it]->setTarget( ttgt );
                    if (isMessageEnabled(MSG_INFO))
                        std::cout << "AirAngleOnlyTrkMgrPT: track " << it << " changed target " << tracks[it]->getTarget()->getID() << std::endl;
                }
            }
            else if (dt > 0.0)
            {
                // update az/el for ownship dynamics only, since no new report
                // do not update for ownship dynamics if sim is frozen
                // assume same los, but use GT rotmat to generate new rel az and rel el
                const osg::Vec3 los_vec = getOwnship()->getRotMat() * tracks[it]->getLosVec();
                const LCreal ra = lcSqrt(los_vec.x() * los_vec.x() + los_vec.y()*los_vec.y());
                const double az = lcAtan2(los_vec.y(), los_vec.x());
                const double el = lcAtan2(-los_vec.z(), ra);
                uAzimuth = az - taz;
                uElevation = el - tel;
                haveU = true;
            }

            if (haveU)
            {
                const LCreal pTaz = tracks[it]->getPredictedAzimuth();
                const LCreal pTazRate = tracks[it]->getPredictedAzimuthRate();
                const LCreal pTel = tracks[it]->getPredictedElevation();
                const LCreal pTelRate = tracks[it]->getPredictedElevationRate();

                // If the track is updated set the new azimuth and elevation data based on
                // a weighted average of the updated value and the predicted value from the
                // previous time step
                tracks[it]->setRelAzimuth((pTaz * oneMinusAlpha) + ((taz + uAzimuth) * alpha));
                tracks[it]->setRelAzimuthRate((pTazRate * oneMinusBeta) + ((uAzimuth / dt) * beta ));
                tracks[it]->setRelAzimuthAcceleration((tracks[it]->getRelAzimuthRate() - tazRate) / dt);
                tracks[it]->setElevation((pTel * oneMinusAlpha) + ((tel + uElevation) * alpha));
                tracks[it]->setElevationRate((pTelRate * oneMinusBeta) + ((uElevation / dt) * beta ));
                tracks[it]->setElevationAcceleration((tracks[it]->getElevationRate() - telRate) / dt);

                taz = tracks[it]->getRelAzimuth();
                tazRate = tracks[it]->getRelAzimuthRate();
                tazAccel = tracks[it]->getRelAzimuthAcceleration();
                tel = tracks[it]->getElevation();
                telRate = tracks[it]->getElevationRate();
                telAccel = tracks[it]->getElevationAcceleration();
            }

            if (ageAtNextTimeStep > 0.0) {
                // Project the values for the next time step
                const LCreal deltaTimeSquaredOverTwo = (ageAtNextTimeStep* ageAtNextTimeStep) / 2.0f;

                tracks[it]->setPredictedAzimuth(taz + tazRate*ageAtNextTimeStep
                    + tazAccel*deltaTimeSquaredOverTwo);
                tracks[it]->setPredictedAzimuthRate(tazRate + tazAccel*ageAtNextTimeStep);
                tracks[it]->setPredictedElevation(tel + telRate*ageAtNextTimeStep
                    + telAccel*deltaTimeSquaredOverTwo);
                tracks[it]->setPredictedElevationRate(telRate + telAccel*ageAtNextTimeStep);
            }
        } // end for (unsigned int it = 0; it < nTrks; it++) {
        lcUnlock(trkListLock);


        // ---
        // 7) Remove tracks that have age beyond the limit.
        // ---
        removeAgedTracks();

    } // end if (nTrks > 0)


    // ---
    // 8) Create new tracks from unmatched reports and free up IR query messages
    // ---
    lcLock(trkListLock);
    for (unsigned int i = 0; i < nReports; i++) {
        if ((reportNumMatches[i] == 0) && (nTrks < maxTrks)) {
            // This is a new report, so create a new track for it
            IrTrack* newTrk = new IrTrack();
            newTrk->setTrackID( getNewTrackID() );
            newTrk->setTarget( queryMessages[i]->getTarget() );
            newTrk->setType(Track::AIR_TRACK_BIT | Track::ONBOARD_SENSOR_BIT);

            if (usePerceivedPosVel) {
                // using reported/perceived position and velocity
                newTrk->setPosition(queryMessages[i]->getPosVec() - getOwnship()->getPosition());
                newTrk->setVelocity(queryMessages[i]->getVelocityVec() - getOwnship()->getVelocity());
            }
            else {
                // missile tracking requires position and velocity, using GT data
                newTrk->setPosition(queryMessages[i]->getTarget()->getPosition() - getOwnship()->getPosition());
                newTrk->setVelocity(queryMessages[i]->getTarget()->getVelocity() - getOwnship()->getVelocity());
            }

            newTrk->setRelAzimuth(queryMessages[i]->getRelativeAzimuth());
            newTrk->setElevation(queryMessages[i]->getRelativeElevation());
            newTrk->setPredictedAzimuth(queryMessages[i]->getRelativeAzimuth());
            newTrk->setPredictedElevation(queryMessages[i]->getRelativeElevation());
            newTrk->setSignal(queryMessages[i]->getSignalToNoiseRatio(), queryMessages[i]);

            if (isMessageEnabled(MSG_INFO)) {
                std::cout << "New AIR track[it] = [" << nTrks << "] id = " << newTrk->getTrackID() << std::endl;
            }

             // Object 1: player, Object 2: Track Data
            BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_NEW_TRACK )
               SAMPLE_2_OBJECTS( ownship, newTrk )
            END_RECORD_DATA_SAMPLE()

            // TabLogger is deprecated
            if (getAnyEventLogger() != nullptr) {
                TabLogger::TabLogEvent* evt = new TabLogger::LogPassiveTrack(1, this,newTrk); // type 1 for "new"
                getAnyEventLogger()->log(evt);
                evt->unref();
            }
            tracks[nTrks++] = newTrk;
        }
        // Free the IR query message report
        queryMessages[i]->unref();
    }
    lcUnlock(trkListLock);
}

} // End Simulation namespace
} // End Eaagles namespace
