//------------------------------------------------------------------------------
// Classes: TrackManager, AirTrkMgr, GmtiGrkMgr, RwrGrkMgr
//------------------------------------------------------------------------------

#include "openeaagles/simulation/TrackManager.h"

#include "openeaagles/simulation/Emission.h"
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

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class: TrackManager
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(TrackManager,"TrackManager")

// Slot table
BEGIN_SLOTTABLE(TrackManager)
   "maxTracks",        // 1: Maximum number of tracks
   "maxTrackAge",      // 2: Maximum track age (time: sec)
   "firstTrackId",     // 3: First track ID
   "alpha",            // 4: Alpha
   "beta",             // 5: Beta
   "gamma",            // 6: Gamma
   "logTrackUpdates",  // 7: whether to log all updates to tracks (default: true)
END_SLOTTABLE(TrackManager)

//  Map slot table
BEGIN_SLOT_MAP(TrackManager)
   ON_SLOT(1, setSlotMaxTracks,Basic::Number)
   ON_SLOT(2, setSlotMaxTrackAge,Basic::Number)
   ON_SLOT(3, setSlotFirstTrackId,Basic::Number)
   ON_SLOT(4, setSlotAlpha, Basic::Number)
   ON_SLOT(5, setSlotBeta,  Basic::Number)
   ON_SLOT(6, setSlotGamma, Basic::Number)
   ON_SLOT(7, setSlotLogTrackUpdates, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
TrackManager::TrackManager() :
      trkListLock(0), emQueue(MAX_TRKS), snQueue(MAX_TRKS), queueLock(0)
{
   STANDARD_CONSTRUCTOR()

      initData();
}

TrackManager::TrackManager(const TrackManager& org) :
   trkListLock(0), emQueue(MAX_TRKS), snQueue(MAX_TRKS), queueLock(0)
{
   STANDARD_CONSTRUCTOR()
   copyData(org,true);
}

TrackManager::~TrackManager()
{
   STANDARD_DESTRUCTOR()
}

TrackManager& TrackManager::operator=(const TrackManager& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

TrackManager* TrackManager::clone() const
{
   return 0;
}

//------------------------------------------------------------------------------
// initialize the member data
//------------------------------------------------------------------------------
void TrackManager::initData()
{
   nTrks = 0;
   for (unsigned int i = 0; i < MAX_TRKS; i++) tracks[i] = 0;
   maxTrks = MAX_TRKS;
   maxTrackAge = 3;        // default age (2 sec)

   trkListLock = 0;
   queueLock = 0;

   type = 0;
   firstTrkId = 1000;      // default starting track id
   nextTrkId = firstTrkId;

   // clear the A matrix
   haveMatrixA = false;
   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
         A[i][j] = 0.0;

   // Default Parameters
   alpha = 1.0;
   beta = 0.0;
   gamma = 0.0;

   logTrackUpdates = true;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TrackManager::copyData(const TrackManager& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   logTrackUpdates = org.logTrackUpdates;

   maxTrks = org.maxTrks;
   maxTrackAge = org.maxTrackAge;
   clearTracksAndQueues();

   type = org.type;
   firstTrkId = org.firstTrkId;
   nextTrkId = org.nextTrkId;

   // Copy Matrix A
   haveMatrixA = org.haveMatrixA;
   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
         A[i][j] = org.A[i][j];

   // Parameters
   alpha   = org.alpha;
   beta    = org.beta;
   gamma   = org.gamma;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void TrackManager::deleteData()
{
   clearTracksAndQueues();
}

//------------------------------------------------------------------------------
// clearTracksAndQueues() -- Clear all tracks and queues
//------------------------------------------------------------------------------
void TrackManager::clearTracksAndQueues()
{
   // ---
   // Clear out the queue(s)
   // ---
   lcLock(queueLock);
   for (Emission* em = emQueue.get(); em != 0; em = emQueue.get()) {
      em->unref();    // unref() the emission
      snQueue.get();  // and every emission had a S/N value
   }
   lcUnlock(queueLock);

   // ---
   // Clear the track list
   // ---
   lcLock(trkListLock);
   int n = nTrks;
   nTrks = 0;
   for (int i = 0; i < n; i++) {
      if (tracks[i] != 0) {
         tracks[i]->clear();
         tracks[i]->unref();
         tracks[i] = 0;
      }
   }
   lcUnlock(trkListLock);
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void TrackManager::reset()
{
   BaseClass::reset();

   clearTracksAndQueues();
   nextTrkId = firstTrkId;
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool TrackManager::shutdownNotification()
{
   clearTracksAndQueues();

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void TrackManager::process(const LCreal dt)
{
   processTrackList(dt);
   BaseClass::process(dt);
}

//------------------------------------------------------------------------------
// Get track manager attributes
//------------------------------------------------------------------------------
LCreal TrackManager::getMaxTrackAge() const
{
   return maxTrackAge;
}

unsigned int TrackManager::getMaxTracks() const
{
   return maxTrks;
}

unsigned int TrackManager::getNumTracks() const
{
   return nTrks;
}

bool TrackManager::isType(const short t) const
{
   return ((type & t) != 0);
}

short TrackManager::getType() const
{
   return type;
}

void TrackManager::setType(const short t)
{
   type = t;
}

void TrackManager::setSubtype(const short t)
{
   type = (t | type);
}

bool TrackManager::getLogTrackUpdates() const
{
   return logTrackUpdates;
}

bool TrackManager::setMaxTrackAge(const double s)
{
   bool ok = false;
   if (s > 0) {
      maxTrackAge = (LCreal) s;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getTrackList() -- Sets entries in 'tlist' to a maximum of 'max' target
//                  tracks and returns the actual number of tracks.
//------------------------------------------------------------------------------
int TrackManager::getTrackList(SPtr<Track>* const tlist, const unsigned int max) const
{
   int n = 0;

   if (tlist != 0) {
      lcLock(trkListLock);
      for (unsigned int i = 0; i < nTrks && i < max; i++) {
         tlist[n++] = tracks[i];
      }
      lcUnlock(trkListLock);
   }

   return n;
}

//------------------------------------------------------------------------------
// getTrackList() -- Sets entries in 'tlist' to a maximum of 'max' target
//                  tracks and returns the actual number of tracks.
//------------------------------------------------------------------------------
int TrackManager::getTrackList(SPtr<const Track>* const tlist, const unsigned int max) const
{
   int n = 0;

   if (tlist != 0) {
      lcLock(trkListLock);
      for (unsigned int i = 0; i < nTrks && i < max; i++) {
         tlist[n++] = tracks[i];
      }
      lcUnlock(trkListLock);
   }

   return n;
}

//------------------------------------------------------------------------------
// getTrackList() -- returns the track list as an array of pointers
//------------------------------------------------------------------------------
int TrackManager::getTrackList(Track* tlist[], const unsigned int max)
{
   int n = 0;

   if (tlist != 0) {
      lcLock(trkListLock);
      for (unsigned int i = 0; i < nTrks && i < max; i++) {
         tlist[n] = tracks[i];
         tlist[n]->ref();
         n++;
      }
      lcUnlock(trkListLock);
   }

   return n;
}

int TrackManager::getTrackList(const Track* tlist[], const unsigned int max) const
{
   int n = 0;

   if (tlist != 0) {
      lcLock(trkListLock);
      for (unsigned int i = 0; i < nTrks && i < max; i++) {
         tlist[n] = tracks[i];
         tlist[n]->ref();
         n++;
      }
      lcUnlock(trkListLock);
   }

   return n;
}

//------------------------------------------------------------------------------
// killedNotification() -- We were just killed by player 'p'
//------------------------------------------------------------------------------
bool TrackManager::killedNotification(Player* const p)
{
   clearTracksAndQueues();
   return BaseClass::killedNotification(p);
}

//------------------------------------------------------------------------------
// newReport() -- Accept a new emission report
//------------------------------------------------------------------------------
void TrackManager::newReport(Emission* em, LCreal sn)
{
   // Queue up emissions reports
   if (em != 0) {
      lcLock(queueLock);
      if (emQueue.isNotFull()) {
      em->ref();
      emQueue.put(em);
      snQueue.put(sn);
      }
      lcUnlock(queueLock);

   }
}

//------------------------------------------------------------------------------
// getReport() -- Get the next 'new' report of the queue
//------------------------------------------------------------------------------
Emission* TrackManager::getReport(LCreal* const sn)
{
   Emission* em = 0;

   lcLock(queueLock);
   em = emQueue.get();
   if (em != 0) {
      *sn = snQueue.get();
   }
   lcUnlock(queueLock);

   return em;
}

//------------------------------------------------------------------------------
// addTrack() -- Add a track to the list
//------------------------------------------------------------------------------
bool TrackManager::addTrack(Track* const t)
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
// makeMatrixA() -- make standard A matrix
//------------------------------------------------------------------------------
void TrackManager::makeMatrixA(LCreal dt)
{
   // Delta time (default: 50 hz)
   LCreal t = dt;
   if (t == 0) t = 1.0f/50.0f;

   A[0][0] = 1;
   A[0][1] = t;
   A[0][2] = (t*t)/2.0f;

   A[1][0] = 0;
   A[1][1] = 1;
   A[1][2] = t;

   A[2][0] = 0;
   A[2][1] = 0;
   A[2][2] = 1;

   haveMatrixA = true;
}

//------------------------------------------------------------------------------
// setMaxTracks() -- Sets the maximum number of active tracks
//------------------------------------------------------------------------------
bool TrackManager::setSlotMaxTracks(const Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      int max = num->getInt();
      if (max > 0 && max <= (int)MAX_TRKS) {
         maxTrks = (unsigned int) max;
         ok = true;
      }
      else {
         std::cerr << "TrackManager::setMaxTracks: maxTracks is invalid, range: [1 .. " << MAX_TRKS << "]" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotMaxTrackAge() -- Sets the maximum age of tracks
//------------------------------------------------------------------------------
bool TrackManager::setSlotMaxTrackAge(const Basic::Number* const num)
{
   LCreal age = 0.0;
   const Basic::Time* p = dynamic_cast<const Basic::Time*>(num);
   if (p != 0) {
      // We have a time and we want it in seconds ...
      Basic::Seconds seconds;
      age = seconds.convert(*p);
   }
   else if (num != 0) {
      // We have only a number, assume it's in seconds ...
      age = num->getReal();
   }

   // Set the value if it's valid
   bool ok = true;
   if (age > 0.0) {
      ok = setMaxTrackAge(age);
   }
   else {
      std::cerr << "TrackManager::setMaxTrackAge: invalid age, must be greater than zero." << std::endl;
      ok = false;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotFirstTrackId() -- Sets the first (starting) track id number
//------------------------------------------------------------------------------
bool TrackManager::setSlotFirstTrackId(const Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      int first = num->getInt();
      if (first >= 0) {
         firstTrkId = (unsigned int) first;
         nextTrkId = firstTrkId;
         ok = true;
      }
      else {
         std::cerr << "TrackManager::setSlotFirstTrackId: firstTrackId is invalid: must be >= 0" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets alpha
//------------------------------------------------------------------------------
bool TrackManager::setSlotAlpha(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      alpha = msg->getReal();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets beta
//------------------------------------------------------------------------------
bool TrackManager::setSlotBeta(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      beta = msg->getReal();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets gamma
//------------------------------------------------------------------------------
bool TrackManager::setSlotGamma(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      gamma = msg->getReal();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets logTrackUpdates
//------------------------------------------------------------------------------
bool TrackManager::setSlotLogTrackUpdates(const Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      ok = setLogTrackUpdates( num->getBoolean() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets logTrackUpdates; controls output
//------------------------------------------------------------------------------
bool TrackManager::setLogTrackUpdates (const bool b)
{
   logTrackUpdates = b;
   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* TrackManager::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& TrackManager::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   // Max number of tracks
   indent(sout,i+j);
   sout << "maxTracks: ";
   sout << maxTrks;
   sout << std::endl;

   // Max track age (seconds)
   indent(sout,i+j);
   sout << "maxTrackAge: ";
   sout << maxTrackAge;
   sout << std::endl;

   // First track id
   indent(sout,i+j);
   sout << "firstTrackId: ";
   sout << firstTrkId;
   sout << std::endl;

   indent(sout,i+j);
   sout << "alpha: " << alpha << std::endl;

   indent(sout,i+j);
   sout << "beta: " << beta << std::endl;

   indent(sout,i+j);
   sout << "gamma: " << gamma << std::endl;

   indent(sout,i+j);
   sout << "logTrackUpdates: " ;
   if (logTrackUpdates)
      sout << "true" << std::endl;
   else
      sout << "false" << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}


//==============================================================================
// Class: AirTrkMgr
//==============================================================================
IMPLEMENT_SUBCLASS(AirTrkMgr,"AirTrkMgr")

// Slot table
BEGIN_SLOTTABLE(AirTrkMgr)
   "positionGate",     // 1: Position Gate (meters)
   "rangeGate",        // 2: Range Gate (meters)
   "velocityGate",     // 3: Velocity Gate (m/s)
END_SLOTTABLE(AirTrkMgr)

//  Map slot table
BEGIN_SLOT_MAP(AirTrkMgr)
   ON_SLOT(1, setPositionGate, Basic::Number)
   ON_SLOT(2, setRangeGate, Basic::Number)
   ON_SLOT(3, setVelocityGate, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AirTrkMgr::AirTrkMgr()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void AirTrkMgr::initData()
{
   setType( Track::ONBOARD_SENSOR_BIT | Track::AIR_TRACK_BIT );

   posGate =  2.0f * Basic::Distance::NM2M;
   rngGate =  500.0f;
   velGate =   10.0f;

   reportNumMatches = new unsigned int[MAX_REPORTS];
   trackNumMatches = new unsigned int[MAX_TRKS];
   report2TrackMatch = new bool*[MAX_REPORTS];
   for (unsigned int i = 0; i < MAX_REPORTS; i++) {
      reportNumMatches[i] = 0;
      report2TrackMatch[i] = new bool[MAX_TRKS];
      for (unsigned int j = 0; j < MAX_TRKS; j++) {
         report2TrackMatch[i][j] = false;
      }
   }
   for (unsigned int i = 0; i < MAX_TRKS; i++) {
      trackNumMatches[i] = 0;
   }
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void AirTrkMgr::copyData(const AirTrkMgr& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   posGate = org.posGate;
   rngGate = org.rngGate;
   velGate = org.velGate;

   for (unsigned int i = 0; i < MAX_REPORTS; i++) {
      reportNumMatches[i] = org.reportNumMatches[i];
      for (unsigned int j = 0; j < MAX_TRKS; j++) {
         report2TrackMatch[i][j] = org.report2TrackMatch[i][j];
      }
   }
   for (unsigned int i = 0; i < MAX_TRKS; i++) {
      trackNumMatches[i] = org.trackNumMatches[i];
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void AirTrkMgr::deleteData()
{
   if (report2TrackMatch != 0) {
      for (unsigned int i = 0; i < MAX_REPORTS; i++) {
         if (report2TrackMatch[i] != 0) {
            delete[] report2TrackMatch[i];
            report2TrackMatch[i] = 0;
         }
      }
      delete[] report2TrackMatch;
      report2TrackMatch = 0;
   }

   if (reportNumMatches != 0) {
      delete[] reportNumMatches;
      reportNumMatches = 0;
   }

   if (trackNumMatches != 0) {
      delete[] trackNumMatches;
      trackNumMatches = 0;
   }
}

//------------------------------------------------------------------------------
// processTrackList() -- process the track list
//
//  (Based on Hovanessian, "Radar Detection & Tracking Systems")
//
//------------------------------------------------------------------------------
void AirTrkMgr::processTrackList(const LCreal dt)
{
   LCreal tmp;

   // Make sure we have an ownship to work with
   Player* ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
   if (ownship == 0 || dt == 0) return;

   // Make sure we have the A and B matrix
   if (!haveMatrixA) makeMatrixA(dt);

   // ---
   // 1) Apply ownship dynamics to current track positions and age the tracks by delta time
   // ---
   osg::Vec3 osVel = ownship->getVelocity();
   osg::Vec3 osAccel = ownship->getAcceleration();
   LCreal osGndTrk = ownship->getGroundTrack();

   lcLock(trkListLock);
   for (unsigned int i = 0; i < nTrks; i++) {
      tracks[i]->ownshipDynamics(osGndTrk, osVel, osAccel, dt);
      tracks[i]->updateTrackAge(dt);
   }
   lcUnlock(trkListLock);

   // ---
   // 2) Process new reports
   // ---

   // Get each new emission report from the queue
   unsigned int nReports = 0;
   Emission* emissions[MAX_REPORTS];
   LCreal newSignal[MAX_REPORTS];
   LCreal newRdot[MAX_REPORTS];
   osg::Vec3 tgtPos[MAX_REPORTS];
   for (Emission* em = getReport(&tmp); em != 0; em = getReport(&tmp)) {

      if (nReports < MAX_REPORTS) {

      Player* tgt = em->getTarget();

      bool dummy = false;
      if (tgt->isMajorType(Player::WEAPON)) {
         dummy = ((const Weapon*) tgt)->isDummy();
      }

      if ( tgt->isMajorType(Player::AIR_VEHICLE) ||
         tgt->isMajorType(Player::SHIP) ||
         (tgt->isMajorType(Player::WEAPON) && !dummy)
         ) {
            // Using only air vehicles
            emissions[nReports] = em;
            newSignal[nReports] = tmp;
            newRdot[nReports] = emissions[nReports]->getRangeRate();
            reportNumMatches[nReports] = 0;
            tgtPos[nReports] = tgt->getPosition() - ownship->getPosition();
            nReports++;
      }
      else {
         // Free up emissions from other types of players
         em->unref();
      }
   }
      else {
         // ignore -- too many reports
      em->unref();
   }
   }

   // ---
   // 3) Match current tracks to new reports (observations)
   // ---
   lcLock(trkListLock);
   for (unsigned int it = 0; it < nTrks; it++) {
      trackNumMatches[it] = 0;
      const RfTrack* const trk = (const RfTrack*) tracks[it];  // we produce only RfTracks
      const Player* const tgt = trk->getLastEmission()->getTarget();
      for (unsigned int ir = 0; ir < nReports; ir++) {
         if (emissions[ir]->getTarget() == tgt) {
            // We have a new report for the same target as this track ...
            report2TrackMatch[ir][it] = true;
            trackNumMatches[it]++;
            reportNumMatches[ir]++;
         }
         else report2TrackMatch[ir][it] = false;
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
   osg::Vec3 u[MAX_TRKS];
   LCreal age[MAX_TRKS];
   bool haveU[MAX_TRKS];

   lcLock(trkListLock);
   for (unsigned int it = 0; it < nTrks; it++) {
      u[it].set(0,0,0);
      haveU[it] = false;
      if (trackNumMatches[it] > 0) {
         for (unsigned int ir = 0; ir < nReports; ir++) {
            if (report2TrackMatch[ir][it]) {
               RfTrack* const trk = (RfTrack*) tracks[it];  // we produce only RfTracks

               // Update the track's signal
               trk->setSignal(newSignal[ir],emissions[ir]);

               // Create a track input vector
               u[it] = (tgtPos[ir] - trk->getPosition());

               // Track age and flags
               if (!haveU[it]) {
                  age[it] = trk->getTrackAge();
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
   LCreal d2 = posGate * posGate;    // position gate squared
   lcLock(trkListLock);
   for (unsigned int i = 0; i < nTrks; i++) {
      // Save X(k)
      osg::Vec3 tpos = tracks[i]->getPosition();
      osg::Vec3 tvel = tracks[i]->getVelocity();
      osg::Vec3 tacc = tracks[i]->getAcceleration();

      if (haveU[i]) {
         // Have Input vector U, use ...
         // where B is ...
         LCreal b0 = alpha;
         LCreal b1 = 0.0;
         if (age[i] != 0) b1 = beta / age[i];
         LCreal b2 = 0.0;
         //LCreal b2 = gamma * 2.0f / (age[i]*age[i]);
         if (u[i].length2() > d2) {
            // Large position change: just set position
            b0 = 1.0;
            b1 = 0.0;
         }

         // X(k+1) = A*X(k) + B*U(k)
         tracks[i]->setPosition(     (tpos*A[0][0] + tvel*A[0][1] + tacc*A[0][2]) + (u[i]*b0) );
         tracks[i]->setVelocity(     (tpos*A[1][0] + tvel*A[1][1] + tacc*A[1][2]) + (u[i]*b1) );
         tracks[i]->setAcceleration( (tpos*A[2][0] + tvel*A[2][1] + tacc*A[2][2]) + (u[i]*b2) );

         // Object 1: player, Object 2: Track Data
         if (getLogTrackUpdates()) {
            BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_TRACK_DATA )
               SAMPLE_2_OBJECTS( ownship, tracks[i] )
            END_RECORD_DATA_SAMPLE()
         }

         // TabLogger is deprecated
         if (getLogTrackUpdates()  &&  (getAnyEventLogger() != 0)) {
            TabLogger::TabLogEvent* evt = new TabLogger::LogActiveTrack(2, this,tracks[i]); // type 2 for "update"
            getAnyEventLogger()->log(evt);
            evt->unref();
         }
      }
      else {
         // Do not have Input vector U, use ...
         // X(k+1) = A*X(k)
         tracks[i]->setPosition(     (tpos*A[0][0] + tvel*A[0][1] + tacc*A[0][2]));
         tracks[i]->setVelocity(     (tpos*A[1][0] + tvel*A[1][1] + tacc*A[1][2]));
         tracks[i]->setAcceleration( (tpos*A[2][0] + tvel*A[2][1] + tacc*A[2][2]));
      }
   }
   lcUnlock(trkListLock);

   // ---
   // 7) For tracks with new observation reports, reset their age.
   //    Remove tracks that have aged beyond the limit.
   // ---
   lcLock(trkListLock);
   for (unsigned int it = 0; it < nTrks; /* update 'it' below */ ) {
      RfTrack* const trk = (RfTrack*) tracks[it];  // we produce only RfTracks
      if (trk->getTrackAge() >= getMaxTrackAge()) {

         // Object 1: player, Object 2: Track Data
         BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_TRACK_REMOVED )
            SAMPLE_2_OBJECTS( ownship, tracks[it] )
         END_RECORD_DATA_SAMPLE()

         if (getAnyEventLogger() != 0) {
            // TabLogger is deprecated
            TabLogger::TabLogEvent* evt = new TabLogger::LogActiveTrack(3, this,trk); // type 3 for "remove"
            getAnyEventLogger()->log(evt);
            evt->unref();
         }

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Removed Aged AIR track: player: " << trk->getLastEmission()->getTarget() << ", refCnt = " << trk->getLastEmission()->getTarget()->getRefCount() << std::endl;
         }

         // Track has timed out -- delete the track and ...
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
   // 8) Create new tracks from unmatched reports and free up emissions
   // ---
   lcLock(trkListLock);
   for (unsigned int i = 0; i < nReports; i++) {
      if ((reportNumMatches[i] == 0) && (nTrks < maxTrks)) {
         // This is a new report, so create a new track for it
         RfTrack* newTrk = new RfTrack();
         newTrk->setTrackID( getNewTrackID() );
         newTrk->setTarget( emissions[i]->getTarget() );
         newTrk->setType(Track::AIR_TRACK_BIT | Track::ONBOARD_SENSOR_BIT);
         newTrk->setPosition(tgtPos[i]);
         newTrk->ownshipDynamics(osGndTrk, osVel, osAccel, 0.0f);
         newTrk->setRangeRate(newRdot[i]);
         newTrk->setSignal(newSignal[i],emissions[i]);

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "New AIR track[it] = [" << nTrks << "] id = " << newTrk->getTrackID() << std::endl;
         }

         BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_NEW_TRACK )
            SAMPLE_2_OBJECTS( ownship, tracks[i] )
         END_RECORD_DATA_SAMPLE()

         if (getAnyEventLogger() != 0) {
            // TabLogger is deprecated
            TabLogger::TabLogEvent* evt = new TabLogger::LogActiveTrack(1, this,newTrk); // type 1 for "new"
            getAnyEventLogger()->log(evt);
            evt->unref();
         }
         tracks[nTrks++] = newTrk;
      }
      // Free the emission report
      emissions[i]->unref();
   }
   lcUnlock(trkListLock);
}

//------------------------------------------------------------------------------
// setPositionGate() -- Sets the size of the position gate
//------------------------------------------------------------------------------
bool AirTrkMgr::setPositionGate(const Basic::Number* const num)
{
   LCreal value = 0.0;
   const Basic::Distance* p = dynamic_cast<const Basic::Distance*>(num);
   if (p != 0) {
      // We have a distance and we want it in meters ...
      Basic::Meters meters;
      value = meters.convert(*p);
   }
   else if (num != 0) {
      // We have only a number, assume it's in meters ...
      value = num->getReal();
   }

   // Set the value if it's valid
   bool ok = true;
   if (value > 0.0) {
      posGate = value;
   }
   else {
      std::cerr << "TrackManager::setPositionGate: invalid gate, must be greater than zero." << std::endl;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setRangeGate() -- Sets the size of the range gate
//------------------------------------------------------------------------------
bool AirTrkMgr::setRangeGate(const Basic::Number* const num)
{
   LCreal value = 0.0;
   const Basic::Distance* p = dynamic_cast<const Basic::Distance*>(num);
   if (p != 0) {
      // We have a distance and we want it in meters ...
      Basic::Meters meters;
      value = meters.convert(*p);
   }
   else if (num != 0) {
      // We have only a number, assume it's in meters ...
      value = num->getReal();
   }

   // Set the value if it's valid
   bool ok = true;
   if (value > 0.0) {
      rngGate = value;
   }
   else {
      std::cerr << "TrackManager::setRangeGate: invalid gate, must be greater than zero." << std::endl;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setVelocityGate() -- Sets the size of the velocity gate
//------------------------------------------------------------------------------
bool AirTrkMgr::setVelocityGate(const Basic::Number* const num)
{
   LCreal value = 0.0;
   if (num != 0) {
      // We have only a number, assume it's in meters ...
      value = num->getReal();
   }

   // Set the value if it's valid
   bool ok = true;
   if (value > 0.0) {
      velGate = value;
   }
   else {
      std::cerr << "TrackManager::setVelocityGate: invalid gate, must be greater than zero." << std::endl;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* AirTrkMgr::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& AirTrkMgr::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "positionGate: " << posGate << std::endl;

   indent(sout,i+j);
   sout << "rangeGate: " << rngGate << std::endl;

   indent(sout,i+j);
   sout << "velocityGate: " << velGate << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}


//==============================================================================
// Class: GmtiTrkMgr
//==============================================================================
IMPLEMENT_SUBCLASS(GmtiTrkMgr,"GmtiTrkMgr")
EMPTY_SLOTTABLE(GmtiTrkMgr)
EMPTY_SERIALIZER(GmtiTrkMgr)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
GmtiTrkMgr::GmtiTrkMgr()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void GmtiTrkMgr::initData()
{
   setType( Track::ONBOARD_SENSOR_BIT | Track::GND_TRACK_BIT );

   reportNumMatches = new unsigned int[MAX_REPORTS];
   trackNumMatches = new unsigned int[MAX_TRKS];
   report2TrackMatch = new bool*[MAX_REPORTS];
   for (unsigned int i = 0; i < MAX_REPORTS; i++) {
      reportNumMatches[i] = 0;
      report2TrackMatch[i] = new bool[MAX_TRKS];
      for (unsigned int j = 0; j < MAX_TRKS; j++) {
         report2TrackMatch[i][j] = false;
      }
   }
   for (unsigned int i = 0; i < MAX_TRKS; i++) {
      trackNumMatches[i] = 0;
   }
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void GmtiTrkMgr::copyData(const GmtiTrkMgr& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   for (unsigned int i = 0; i < MAX_REPORTS; i++) {
      reportNumMatches[i] = org.reportNumMatches[i];
      for (unsigned int j = 0; j < MAX_TRKS; j++) {
         report2TrackMatch[i][j] = org.report2TrackMatch[i][j];
      }
   }
   for (unsigned int i = 0; i < MAX_TRKS; i++) {
      trackNumMatches[i] = org.trackNumMatches[i];
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void GmtiTrkMgr::deleteData()
{
   if (report2TrackMatch != 0) {
      for (unsigned int i = 0; i < MAX_REPORTS; i++) {
         if (report2TrackMatch[i] != 0) {
            delete[] report2TrackMatch[i];
            report2TrackMatch[i] = 0;
         }
      }
      delete[] report2TrackMatch;
      report2TrackMatch = 0;
   }

   if (reportNumMatches != 0) {
      delete[] reportNumMatches;
      reportNumMatches = 0;
   }

   if (trackNumMatches != 0) {
      delete[] trackNumMatches;
      trackNumMatches = 0;
   }
}

//------------------------------------------------------------------------------
// processTrackList() -- process the track list
//------------------------------------------------------------------------------
void GmtiTrkMgr::processTrackList(const LCreal dt)
{
   LCreal tmp;

   // Make sure we have an ownship to work with
   Player* ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
   if (ownship == 0 || dt == 0) return;

   // Make sure we have the A and B matrix
   if (!haveMatrixA) makeMatrixA(dt);

   // ---
   // 1) Apply ownship dynamics to current track positions and age the tracks by delta time
   // ---
   osg::Vec3 osVel = ownship->getVelocity();
   osg::Vec3 osAccel = ownship->getAcceleration();
   LCreal osGndTrk = ownship->getGroundTrack();
   lcLock(trkListLock);
   for (unsigned int i = 0; i < nTrks; i++) {
      tracks[i]->ownshipDynamics(osGndTrk, osVel, osAccel, dt);
      tracks[i]->updateTrackAge(dt);
   }
   lcUnlock(trkListLock);

   // ---
   // 2) Process new reports
   // ---

   // Get each new emission report from the queue
   unsigned int nReports = 0;
   Emission* emissions[MAX_REPORTS];
   LCreal newSignal[MAX_REPORTS];
   LCreal newRdot[MAX_REPORTS];
   osg::Vec3 tgtPos[MAX_REPORTS];
   for (Emission* em = getReport(&tmp); em != 0; em = getReport(&tmp)) {
      if (nReports < MAX_REPORTS) {
      Player* tgt = em->getTarget();
      if (tgt->isMajorType(Player::GROUND_VEHICLE)) {
         // Using only Ground vehicles
         emissions[nReports] = em;
         newSignal[nReports] = tmp;
         newRdot[nReports] = emissions[nReports]->getRangeRate();
         reportNumMatches[nReports] = 0;
         tgtPos[nReports] = tgt->getPosition() - ownship->getPosition();
         nReports++;
      }
      else {
         // Free up emissions from other types of players
         em->unref();
      }
   }
      else {
         // ignore -- too many reports
      em->unref();
   }
   }

   // ---
   // 3) Match current tracks to new reports (observations)
   // ---
   lcLock(trkListLock);
   for (unsigned int it = 0; it < nTrks; it++) {
      trackNumMatches[it] = 0;
      const RfTrack* const trk = (const RfTrack*) tracks[it];  // we produce only RfTracks
      const Player* const tgt = trk->getLastEmission()->getTarget();
      for (unsigned int ir = 0; ir < nReports; ir++) {
         if (emissions[ir]->getTarget() == tgt) {
            // We have a new report for the same target as this track ...
            report2TrackMatch[ir][it] = true;
            trackNumMatches[it]++;
            reportNumMatches[ir]++;
         }
         else report2TrackMatch[ir][it] = false;
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
   osg::Vec3 u[MAX_TRKS];
   LCreal age[MAX_TRKS];
   bool haveU[MAX_TRKS];
   lcLock(trkListLock);
   for (unsigned int it = 0; it < nTrks; it++) {
      u[it].set(0,0,0);
      haveU[it] = false;
      if (trackNumMatches[it] > 0) {
         for (unsigned int ir = 0; ir < nReports; ir++) {
            if (report2TrackMatch[ir][it]) {

               RfTrack* const trk = (RfTrack*) tracks[it];  // we produce only RfTracks

               // Update the track's signal
               trk->setSignal(newSignal[ir],emissions[ir]);

               // Create a track input vector
               u[it] = (tgtPos[ir] - trk->getPosition());

               // Track age and flags
               if (!haveU[it]) {
                  age[it] = trk->getTrackAge();
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
      osg::Vec3 tpos = tracks[i]->getPosition();
      osg::Vec3 tvel = tracks[i]->getVelocity();
      osg::Vec3 tacc = tracks[i]->getAcceleration();

      if (haveU[i]) {
         // Have Input vector U, use ...
         // X(k+1) = A*X(k) + B*U(k)
         // where B is ...
         LCreal b0 = alpha;
         LCreal b1 = 0.0;
         if (age[i] != 0) b1 = beta / age[i];
         LCreal b2 = 0.0;
         //LCreal b2 = gamma * 2.0 / (age[i]*age[i]);
         tracks[i]->setPosition(     (tpos*A[0][0] + tvel*A[0][1] + tacc*A[0][2]) + (u[i]*b0) );
         tracks[i]->setVelocity(     (tpos*A[1][0] + tvel*A[1][1] + tacc*A[1][2]) + (u[i]*b1) );
         tracks[i]->setAcceleration( (tpos*A[2][0] + tvel*A[2][1] + tacc*A[2][2]) + (u[i]*b2) );

         if (getLogTrackUpdates()) {
            // Object 1: player, Object 2: Track Data
            BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_TRACK_DATA )
               SAMPLE_2_OBJECTS( ownship, tracks[i] )
            END_RECORD_DATA_SAMPLE()
         }

         // TabLogger is deprecated
         if (getLogTrackUpdates()  &&  (getAnyEventLogger() != 0)) {
            TabLogger::TabLogEvent* evt = new TabLogger::LogActiveTrack(2, this,tracks[i]); // type 2 for "update"
            getAnyEventLogger()->log(evt);
            evt->unref();
         }
      }
      else {
         // Do not have Input vector U, use ...
         // X(k+1) = A*X(k)
         tracks[i]->setPosition(     (tpos*A[0][0] + tvel*A[0][1] + tacc*A[0][2]));
         tracks[i]->setVelocity(     (tpos*A[1][0] + tvel*A[1][1] + tacc*A[1][2]));
         tracks[i]->setAcceleration( (tpos*A[2][0] + tvel*A[2][1] + tacc*A[2][2]));
      }
   }
   lcUnlock(trkListLock);

   // ---
   // 7) For tracks with new observation reports, reset their age.
   //    Remove tracks that have aged beyond the limit.
   // ---
   lcLock(trkListLock);
   for (unsigned int it = 0; it < nTrks; /* update 'it' below */ ) {
      if (tracks[it]->getTrackAge() >= getMaxTrackAge()) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Removed Aged GND track[it] = [" << it << "] id = " << tracks[it]->getTrackID() << std::endl;
         }

         // Object 1: player, Object 2: Track Data
         BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_TRACK_REMOVED )
            SAMPLE_2_OBJECTS( ownship, tracks[it] )
         END_RECORD_DATA_SAMPLE()

         // TabLogger is deprecated
         if (getAnyEventLogger() != 0) {
            TabLogger::TabLogEvent* evt = new TabLogger::LogActiveTrack(3, this,tracks[it]); // type 3 for "remove"
            getAnyEventLogger()->log(evt);
            evt->unref();
         }
         // Track has timed out -- delete the track and ...
         tracks[it]->unref();
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
   // 8) Create new tracks from unmatched reports and free up emissions
   // ---
   lcLock(trkListLock);
   for (unsigned int i = 0; i < nReports; i++) {
      if ((reportNumMatches[i] == 0) && (nTrks < maxTrks)) {
         // This is a new report, so create a new track for it
         RfTrack* newTrk = new RfTrack();
         newTrk->setTrackID( getNewTrackID() );
         newTrk->setTarget( emissions[i]->getTarget() );
         newTrk->setType(Track::GND_TRACK_BIT | Track::ONBOARD_SENSOR_BIT);
         newTrk->setPosition(tgtPos[i]);
         newTrk->ownshipDynamics(osGndTrk, osVel, osAccel, 0.0f);
         newTrk->setRangeRate(newRdot[i]);
         newTrk->setSignal(newSignal[i],emissions[i]);

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "New GND track[it] = [" << nTrks << "] id = " << newTrk->getTrackID() << std::endl;
         }

         // Object 1: player, Object 2: Track Data
         BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_NEW_TRACK )
            SAMPLE_2_OBJECTS( ownship, newTrk )
         END_RECORD_DATA_SAMPLE()

         // TabLogger is deprecated
         if (getAnyEventLogger() != 0) {
            TabLogger::TabLogEvent* evt = new TabLogger::LogActiveTrack(1, this,newTrk); // type 1 for "new"
            getAnyEventLogger()->log(evt);
            evt->unref();
         }
         tracks[nTrks++] = newTrk;
      }
      // Free the emission report
      emissions[i]->unref();
   }
   lcUnlock(trkListLock);
}


//==============================================================================
// Class: RwrTrkMgr
//==============================================================================
IMPLEMENT_SUBCLASS(RwrTrkMgr,"RwrTrkMgr")
EMPTY_SLOTTABLE(RwrTrkMgr)
EMPTY_SERIALIZER(RwrTrkMgr)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
RwrTrkMgr::RwrTrkMgr()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void RwrTrkMgr::initData()
{
   setType( Track::ONBOARD_SENSOR_BIT | Track::RWR_TRACK_BIT );

   reportNumMatches = new unsigned int[MAX_REPORTS];
   trackNumMatches = new unsigned int[MAX_TRKS];
   report2TrackMatch = new bool*[MAX_REPORTS];
   for (unsigned int i = 0; i < MAX_REPORTS; i++) {
      reportNumMatches[i] = 0;
      report2TrackMatch[i] = new bool[MAX_TRKS];
      for (unsigned int j = 0; j < MAX_TRKS; j++) {
         report2TrackMatch[i][j] = false;
      }
   }
   for (unsigned int i = 0; i < MAX_TRKS; i++) {
      trackNumMatches[i] = 0;
   }
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void RwrTrkMgr::copyData(const RwrTrkMgr& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   for (unsigned int i = 0; i < MAX_REPORTS; i++) {
      reportNumMatches[i] = org.reportNumMatches[i];
      for (unsigned int j = 0; j < MAX_TRKS; j++) {
         report2TrackMatch[i][j] = org.report2TrackMatch[i][j];
      }
   }
   for (unsigned int i = 0; i < MAX_TRKS; i++) {
      trackNumMatches[i] = org.trackNumMatches[i];
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void RwrTrkMgr::deleteData()
{
   if (report2TrackMatch != 0) {
      for (unsigned int i = 0; i < MAX_REPORTS; i++) {
         if (report2TrackMatch[i] != 0) {
            delete[] report2TrackMatch[i];
            report2TrackMatch[i] = 0;
         }
      }
      delete[] report2TrackMatch;
      report2TrackMatch = 0;
   }

   if (reportNumMatches != 0) {
      delete[] reportNumMatches;
      reportNumMatches = 0;
   }

   if (trackNumMatches != 0) {
      delete[] trackNumMatches;
      trackNumMatches = 0;
   }
}

//------------------------------------------------------------------------------
// processTrackList() -- process the track list
//------------------------------------------------------------------------------
void RwrTrkMgr::processTrackList(const LCreal dt)
{
   LCreal tmp;

   // Make sure we have an ownship to work with
   Player* ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
   if (ownship == 0 || dt == 0) return;

   // Make sure we have the A and B matrix
   if (!haveMatrixA) makeMatrixA(dt);

   // ---
   // 1) Apply ownship dynamics to current track positions and age the tracks by delta time
   // ---
   osg::Vec3 osVel = ownship->getVelocity();
   osg::Vec3 osAccel = ownship->getAcceleration();
   LCreal osGndTrk = ownship->getGroundTrack();
   lcLock(trkListLock);
   for (unsigned int i = 0; i < nTrks; i++) {
      tracks[i]->ownshipDynamics(osGndTrk, osVel, osAccel, dt);
      tracks[i]->updateTrackAge(dt);
   }
   lcUnlock(trkListLock);

   // ---
   // 2) Process new reports
   // ---

   // Get each new emission report from the queue
   unsigned int nReports = 0;
   Emission* emissions[MAX_REPORTS];
   LCreal newSignal[MAX_REPORTS];
   LCreal newRdot[MAX_REPORTS];
   osg::Vec3 tgtPos[MAX_REPORTS];
   for (Emission* em = getReport(&tmp); em != 0; em = getReport(&tmp)) {
      if (nReports < MAX_REPORTS) {
         // save the report
      Player* tgt = em->getOwnship();  // The emissions ownship is our target!
      emissions[nReports] = em;
      newSignal[nReports] = tmp;
      newRdot[nReports] = emissions[nReports]->getRangeRate();
      reportNumMatches[nReports] = 0;
      tgtPos[nReports] = tgt->getPosition() - ownship->getPosition();
      nReports++;
   }
      else {
         // ignore -- too many reports
      em->unref();
      }
   }

   // ---
   // 3) Match current tracks to new reports (observations)
   // ---
   lcLock(trkListLock);
   for (unsigned int it = 0; it < nTrks; it++) {
      trackNumMatches[it] = 0;
      const RfTrack* const trk = (const RfTrack*) tracks[it];        // we produce only RfTracks
      const Player* const tgt = trk->getLastEmission()->getOwnship(); // The emissions ownship is our target!
      for (unsigned int ir = 0; ir < nReports; ir++) {
         if (emissions[ir]->getOwnship() == tgt) {  // The emissions ownship is our target!
            // We have a new report for the same target as this track ...
            report2TrackMatch[ir][it] = true;
            trackNumMatches[it]++;
            reportNumMatches[ir]++;
         }
         else report2TrackMatch[ir][it] = false;
      }
   }
   lcUnlock(trkListLock);

   // ---
   // 4) Apply rules to associate the proper report to track.
   // ---

   // No Rules -- Just Right


   // ---
   // 5) Create input vectors for the current tracks
   // ---
   osg::Vec3 u[MAX_TRKS];
   //LCreal age[MAX_TRKS];
   bool haveU[MAX_TRKS];
   lcLock(trkListLock);
   for (unsigned int it = 0; it < nTrks; it++) {
      u[it].set(0,0,0);
      haveU[it] = false;
      if (trackNumMatches[it] > 0) {
         for (unsigned int ir = 0; ir < nReports; ir++) {
            if (report2TrackMatch[ir][it]) {

               // Update the track's signal
               RfTrack* const trk = (RfTrack*) tracks[it];        // we produce only RfTracks
               trk->setSignal(newSignal[ir],emissions[ir]);

               // Create a track input vector
               u[it] = (tgtPos[ir] - tracks[it]->getPosition());
               //osg::Vec3 trkPos = tracks[it]->getPosition();

               // Track age and flags
               if (!haveU[it]) {
                  //age[it] = trk->getTrackAge();
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
      osg::Vec3 tpos = tracks[i]->getPosition();
      osg::Vec3 tvel = tracks[i]->getVelocity();
      osg::Vec3 tacc = tracks[i]->getAcceleration();

      if (haveU[i]) {
         // Have Input vector U, use ...
         // X(k+1) = A*X(k) + B*U(k)
         // where B is ...
         LCreal b0 = alpha;
         LCreal b1 = 0.0;
         LCreal b2 = 0.0;
         tracks[i]->setPosition(     (tpos*A[0][0] + tvel*A[0][1] + tacc*A[0][2]) + (u[i]*b0) );
         tracks[i]->setVelocity(     (tpos*A[1][0] + tvel*A[1][1] + tacc*A[1][2]) + (u[i]*b1) );
         tracks[i]->setAcceleration( (tpos*A[2][0] + tvel*A[2][1] + tacc*A[2][2]) + (u[i]*b2) );

         if (getLogTrackUpdates()) {
            BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_TRACK_DATA )
               SAMPLE_2_OBJECTS( ownship, tracks[i] )
            END_RECORD_DATA_SAMPLE()
         }

         // TabLogger is deprecated
         if (getLogTrackUpdates()  &&  (getAnyEventLogger() != 0)) {
            TabLogger::TabLogEvent* evt = new TabLogger::LogPassiveTrack(2, this,tracks[i]); // type 2 for "update"
            getAnyEventLogger()->log(evt);
            evt->unref();
         }
      }
      else {
         // Do not have Input vector U, use ...
         // X(k+1) = A*X(k)
         tracks[i]->setPosition(     (tpos*A[0][0] + tvel*A[0][1] + tacc*A[0][2]));
         tracks[i]->setVelocity(     (tpos*A[1][0] + tvel*A[1][1] + tacc*A[1][2]));
         tracks[i]->setAcceleration( (tpos*A[2][0] + tvel*A[2][1] + tacc*A[2][2]));
      }
   }
   lcUnlock(trkListLock);

   // ---
   // 7) For tracks with new observation reports, reset their age.
   //    Remove tracks that have aged beyond the limit.
   // ---
   lcLock(trkListLock);
   for (unsigned int it = 0; it < nTrks; /* update 'it' below */ ) {
      if (tracks[it]->getTrackAge() >= getMaxTrackAge()) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Removed Aged RWR track[it] = [" << it << "] id = " << tracks[it]->getTrackID() << std::endl;
         }

         BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_TRACK_REMOVED )
            SAMPLE_2_OBJECTS( ownship, tracks[it] )
         END_RECORD_DATA_SAMPLE()

         // TabLogger is deprecated
         if (getAnyEventLogger() != 0) {
            TabLogger::TabLogEvent* evt = new TabLogger::LogPassiveTrack(3, this, tracks[it]); // type 3 for "removed"
            getAnyEventLogger()->log(evt);
            evt->unref();
         }
         // Track has timed out -- delete the track and ...
         tracks[it]->unref();
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
   // 8) Create new tracks from unmatched reports and free up emissions
   // ---
   lcLock(trkListLock);
   for (unsigned int i = 0; i < nReports; i++) {
      if ((reportNumMatches[i] == 0) && (nTrks < maxTrks)) {
         // This is a new report, so create a new track for it
         RfTrack* newTrk = new RfTrack();
         newTrk->setTrackID( getNewTrackID() );
         newTrk->setTarget( emissions[i]->getOwnship() );  // The emissions ownship is our target!
         newTrk->setType(Track::RWR_TRACK_BIT  | Track::ONBOARD_SENSOR_BIT);
         newTrk->setPosition(tgtPos[i]);
         newTrk->ownshipDynamics(osGndTrk, osVel, osAccel, 0.0f);
         newTrk->setRangeRate(newRdot[i]);
         newTrk->setSignal(newSignal[i],emissions[i]);

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "New RWR track[it] = [" << nTrks << "] id = " << newTrk->getTrackID() << std::endl;
         }

         BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_NEW_TRACK )
            SAMPLE_2_OBJECTS( ownship, newTrk )
         END_RECORD_DATA_SAMPLE()

         // TabLogger is deprecated
         if (getAnyEventLogger() != 0) {
            TabLogger::TabLogEvent* evt = new TabLogger::LogPassiveTrack(1,this,newTrk); // type 1 for "new"
            getAnyEventLogger()->log(evt);
            evt->unref();
         }
         tracks[nTrks++] = newTrk;
      }
      // Free the emission report
      emissions[i]->unref();
   }
   lcUnlock(trkListLock);
}

} // End Simulation namespace
} // End Eaagles namespace
