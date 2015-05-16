#include "openeaagles/simulation/Tdb.h"

#include "openeaagles/simulation/Gimbal.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Terrain.h"
#include "openeaagles/basic/units/Distances.h"

#include <cmath>

namespace Eaagles {
namespace Simulation {

//==============================================================================
//  Class: Tdb
//==============================================================================

IMPLEMENT_PARTIAL_SUBCLASS(Tdb,"Gimbal_Tdb")
EMPTY_SLOTTABLE(Tdb)
EMPTY_SERIALIZER(Tdb)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Tdb::Tdb(const unsigned int mt, const Gimbal* const gp)
{
   STANDARD_CONSTRUCTOR()
   initData();
   setGimbal(gp);
   resizeArrays(mt);
}

Tdb::Tdb()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

Tdb::Tdb(const Tdb& org)
{
    STANDARD_CONSTRUCTOR()
    copyData(org,true);
}

Tdb::~Tdb()
{
   STANDARD_DESTRUCTOR()
}

Tdb& Tdb::operator=(const Tdb& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

Tdb* Tdb::clone() const
{
    return new Tdb(*this);
}

void Tdb::initData()
{
   ownship = nullptr;
   gimbal = nullptr;
   usingEcefFlg = false;

   targets = nullptr;
   maxTargets = 0;
   numTgts = 0;

   ranges = nullptr;
   rngRates = nullptr;
   losG = nullptr;
   losO2T = nullptr;
   losT2O = nullptr;
   aar = nullptr;
   aazr = nullptr;
   aelr = nullptr;

   xa = nullptr;
   ya = nullptr;
   za = nullptr;
   ra2 = nullptr;
   ra = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Tdb::copyData(const Tdb& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setGimbal(org.gimbal);

   // Reallocate the space as needed
   resizeArrays(org.maxTargets);

   if (targets != nullptr) {
      for (unsigned int i = 0; i < org.numTgts; i++) {
         org.targets[i]->ref();
         targets[i] = org.targets[i];
         ranges[i] = org.ranges[i];
         rngRates[i] = org.rngRates[i];
         losG[i] = org.losG[i];
         losO2T[i] = org.losO2T[i];
         losT2O[i] = org.losT2O[i];
         aar[i] = org.aar[i];
         aazr[i] = org.aazr[i];
         aelr[i] = org.aelr[i];
      }
   }
   numTgts = org.numTgts;
   usingEcefFlg = org.usingEcefFlg;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Tdb::deleteData()
{
   resizeArrays(0);
   setGimbal(nullptr);
}

//------------------------------------------------------------------------------
// Clear the target data arrays
//------------------------------------------------------------------------------
void Tdb::clearArrays()
{
   // We just want to unref() our targets and set numTgts to zero.
   // -- we really don't care about the other data if numTgts is zero
   if (targets != nullptr) {
      while (numTgts > 0) {
         --numTgts;
         if (targets[numTgts] != nullptr) {
            targets[numTgts]->unref();
            targets[numTgts] = nullptr;
         }
      }
   }
   else {
      numTgts = 0;
   }
}

//------------------------------------------------------------------------------
// Resize the target data arrays (0 .. Gimbal::MAX_PLAYERS)
// -- old data is lost
//------------------------------------------------------------------------------
bool Tdb::resizeArrays(const unsigned int newSize)
{
   bool ok = false;
   if (newSize <= Gimbal::MAX_PLAYERS) {

      // Clear out the old data
      clearArrays();

      if (newSize != maxTargets) {

         // Free up the old memory
         if (ranges   != nullptr)   { delete[] ranges;   ranges   = nullptr; }
         if (rngRates != nullptr)   { delete[] rngRates; rngRates = nullptr; }
         if (losG     != nullptr)   { delete[] losG;     losG     = nullptr; }
         if (losO2T   != nullptr)   { delete[] losO2T;   losO2T   = nullptr; }
         if (losT2O   != nullptr)   { delete[] losT2O;   losT2O   = nullptr; }
         if (aar      != nullptr)   { delete[] aar;      aar      = nullptr; }
         if (aazr     != nullptr)   { delete[] aazr;     aazr     = nullptr; }
         if (aelr     != nullptr)   { delete[] aelr;     aelr     = nullptr; }

         if (targets != nullptr)    { delete[] targets;  targets  = nullptr; }
         maxTargets = 0;

         if (xa  != nullptr)  { delete[] xa;  xa  = nullptr; }
         if (ya  != nullptr)  { delete[] ya;  ya  = nullptr; }
         if (za  != nullptr)  { delete[] za;  za  = nullptr; }
         if (ra2 != nullptr)  { delete[] ra2; ra2 = nullptr; }
         if (ra  != nullptr)  { delete[] ra;  ra  = nullptr; }

         // Allocate new memory
         if (newSize > 0) {
            ranges   = new double[newSize];
            rngRates = new double[newSize];
            losG     = new osg::Vec3d[newSize];
            losO2T   = new osg::Vec3d[newSize];
            losT2O   = new osg::Vec3d[newSize];
            aar      = new double[newSize];
            aazr     = new double[newSize];
            aelr     = new double[newSize];
            targets  = new Player*[newSize];
            for (unsigned int i = 0; i < newSize; i++) {
               targets[i] = nullptr;
            }
            maxTargets = newSize;
            xa = new double[newSize];
            ya = new double[newSize];
            za = new double[newSize];
            ra2 = new double[newSize];
            ra = new double[newSize];
         }

      }

      ok = true;
   }
   return ok;
}


//------------------------------------------------------------------------------
// Process players-of-interest ---  Scan the provided player list and generates
// a sublist of target players that were filtered by player type, max range,
// max angles, etc.
// (Background task)
//------------------------------------------------------------------------------
unsigned int Tdb::processPlayers(Basic::PairStream* const players)
{
   // ---
   // Early out checks (no ownship, no players of interest, no target data arrays)
   // ---
   if (gimbal == nullptr || ownship == nullptr || players == nullptr || maxTargets == 0) return 0;

   // ---
   // Terrain occulting check setup
   // ---
   const Basic::Terrain* terrain = nullptr;
   if (gimbal->isTerrainOccultingEnabled()) {
      const Simulation* const sim = ownship->getSimulation();
      terrain = sim->getTerrain();
   }

   // ---
   // Player of interest parameters
   // ---
   const double maxRange = gimbal->getMaxRange2PlayersOfInterest();
   const double maxAngle = gimbal->getMaxAngle2PlayersOfInterest();
   const double cosMaxFov = std::cos(maxAngle);
   const unsigned int mask = gimbal->getPlayerOfInterestTypes();
   const bool localOnly = gimbal->isLocalPlayersOfInterestOnly();
   const bool useWorld = gimbal->isUsingWorldCoordinates();
   const bool ownHdgOnly = gimbal->isUsingHeadingOnly();
   const double earthRadius = gimbal->getEarthRadius();
   const bool checkHorizon = gimbal->isHorizonCheckEnabled();

   // ---
   // Get the matrices
   // ---

   // World (ECEF) to local (NED)
   osg::Matrixd wm = ownship->getWorldMat();

   // Local (NED) to gimbal coord
   osg::Matrixd rm = gimbal->getRotMat();
   if (ownHdgOnly) {
      // Using heading only, ignore ownship roll and pitch
      osg::Matrixd rr;
      rr.makeRotate( ownship->getHeading(), 0, 0, 1);
      rm *= rr;
   }
   else {
      // Using ownship roll, pitch and heading
      rm *= ownship->getRotMat();
   }

   // ---
   // Get our position and velocity vectors
   // ## using ownship position for now; should include the location of the gimbal ##
   // ---

   // We're using world (ECEF) coordinates when the 'useWorld' flag is true or
   // our ownship's local gaming area position is not valid.
   usingEcefFlg = useWorld || !(ownship->isPositionVectorValid());

   // Position Vector (ECEF or local gaming area NED
   osg::Vec3d p0;
   if (usingEcefFlg) {
      // Using geocentric position (ECEF)
      p0 = ownship->getGeocPosition();
   }
   else {
      // Using local gaming area position vector (NED)
      p0 = ownship->getPosition();
   }

   // Geodetic position of our ownship
   const double osLat = ownship->getLatitude();
   const double osLon = ownship->getLongitude();
   const double osAlt = ownship->getAltitudeM();

   // If we're using ECEF coordinates then we compute the distance
   // to the earth horizon and the tangent of the angle from our
   // local level to the earth horizon
   double hDist = 1000000.0 * Basic::Distance::NM2M;  // Distance to horizon (m) (default: really far away)
   double hTanAng = 0;                                // Tangent of the angle to horizon (positive down)
   if (usingEcefFlg) {
      // Our vertical offset from our ownship is the inverse of the 'z'
      // translation component from our gimbal to NED matrix.
      const double vertOffset = -rm(3,2);    // Z translation (but positive up)

      // distance from the center of the earth
      const double distEC = vertOffset + osAlt + earthRadius;
      const double distEC2 = distEC * distEC;  // squared

      // earth radius squared
      const double er2 = earthRadius * earthRadius;

      // distance to horizon squared
      const double dh2 = distEC2 - er2;

      // the distance and the tangent of the angle to the horizon
      hDist = std::sqrt(dh2);
      hTanAng = ( hDist / earthRadius ); // positive angles are below level (down)
   }

   // Are we a space vehicle?
   const bool osSpaceVehicle = ownship->isMajorType(Player::SPACE_VEHICLE);

   // ---
   // 1) Scan the player list ---
   // ---
   bool finished = false;
   for (Basic::List::Item* item = players->getFirstItem(); item != nullptr && numTgts < maxTargets && !finished; item = item->getNext()) {

      // Get the pointer to the target player
      Basic::Pair* pair = static_cast<Basic::Pair*>(item->getValue());
      Player* target = static_cast<Player*>(pair->object());

      // Did we complete the local only players?
      finished = localOnly && target->isNetworkedPlayer();

      // We should process this target if ...
      const bool processTgt =
         !finished &&                                       // we're not finished AND
         target != ownship &&                               // its not our ownship AND
         target->isActive() &&                              // the target is active AND
         target->isMajorType(mask) &&                       // the target is one of the selected types AND
         (usingEcefFlg || target->isPositionVectorValid()); // we're using ECEF or the target's position vector is valid

      if ( processTgt ) {

         // Target Line-Of-Sight (LOS) vector
         osg::Vec3d tlos;
         if (usingEcefFlg) tlos = target->getGeocPosition() - p0;
         else tlos = target->getPosition() - p0;

         // Normalized and compute length: unit LOS vector and range (meters)
         const double range = tlos.normalize();

         // In-range check (only if maxRange is greater than zero)
         bool inRange = (maxRange == 0);
         if ( !inRange ) {
            inRange = range <= maxRange;
         }

         if (inRange) {

            // LOS vector in local tangent plane NED
            osg::Vec3d losNED = tlos;
            if (usingEcefFlg) {
               // LOS vector: ECEF to NED
               losNED = wm * tlos;
            }

            // Compute the tangent of the angle from our local level to
            // the target (positive angles are down)
            double tanTgtAng = 999999.9; // initial tangent (down)
            double xyRng = std::sqrt(losNED[0]*losNED[0] + losNED[1]*losNED[1]);
            if (xyRng > 0) tanTgtAng = losNED[2]/xyRng;
            else if (losNED[2] <= 0) tanTgtAng = -999999.9; // up

            // Horizon check
            bool aboveHorizon = true;
            if (usingEcefFlg && checkHorizon) {
               // We can see targets that are above the horizon or
               // targets that are on or above the earth and are closer
               // than the horizon
               aboveHorizon = (tanTgtAng <= hTanAng) || (range <= hDist);
            }

            if (aboveHorizon) {

               // In FOV check (only if maxAngle is greater than zero)
               bool inFov = (maxAngle == 0);
               if ( !inFov ) {
                  // LOS vector: NED to gimbal coordinates
                  const osg::Vec3d losG = rm * losNED;
                  inFov = (losG.x() >= cosMaxFov);
               }

               if (inFov) {

                  // Terrain occulting if we have terrain data and we're not a space vehicle
                  bool occulted = false;
                  if (terrain != nullptr && !osSpaceVehicle) {

                     const double tgtLat = target->getLatitude();
                     const double tgtLon = target->getLongitude();
                     const double tgtAlt = target->getAltitudeM();

                     // Is the target a space vehicle?
                     if ( target->isMajorType(Player::SPACE_VEHICLE) ) {
                        // Get the true, great-circle bearing to the target
                        double tbrg(0), distNM(0);
                        Basic::Nav::vll2bd(osLat, osLon, tgtLat, tgtLon, &tbrg, &distNM);

                        // Set the distance to check to 60 nm
                        double dist = 60.0 * Basic::Distance::NM2M;

                        // Terrain occulting check toward the space vehicle
                        occulted = terrain->targetOcculting2(osLat, osLon, osAlt, tbrg, dist, -tanTgtAng);
                     }
                     else {
                        // Occulting check between two standard player
                        occulted = terrain->targetOcculting(osLat, osLon, static_cast<LCreal>(osAlt),
                                                            tgtLat, tgtLon, static_cast<LCreal>(tgtAlt));
                     }
                  }

                  if (!occulted) {
                     // !!! All is well with this target !!!

                     // Ref() and save the target pointer
                     target->ref();
                     targets[numTgts++] = target;
                  }
               }
            }
         }
      }
   }

   return numTgts;
}


//------------------------------------------------------------------------------
// Compute Boresight Data --- Scan the target list, which as been pre-processed by
// processPlayers(), and compute gimbal boresight data.
// (Time-critical task -- in sync with gimbal dynamics & transmit phases)
//
//------------------------------------------------------------------------------
unsigned int Tdb::computeBoresightData()
{
   // ---
   // Early out checks (no ownship, no players of interest, no target data arrays)
   // ---
   if (gimbal == nullptr || ownship == nullptr || numTgts == 0) return 0;

   // If 'ownHdgOnly' is true (default) then only the ownship's heading angle is used,
   // which earth stabilizes the gimbal in roll and pitch, otherwise the full
   // ownship rotational matrix is used.
   const bool ownHdgOnly = gimbal->isUsingHeadingOnly();

   // ---
   // Compute normalized LOS vector (NED), range and ranging rate
   // ---
   {
      // Vectors (ECEF or local gaming area NED)
      osg::Vec3d p0;  // Position Vector
      osg::Vec3d v0;  // Velocity vector [ x y z ] (meters/second)

      // World (ECEF) to local tanget plane (NED)
      osg::Matrixd wm = ownship->getWorldMat();

      if (usingEcefFlg) {
         // Using ECEF
         p0 = ownship->getGeocPosition();  // Geocentric position (ECEF)
         v0 = ownship->getGeocVelocity();  // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
      }
      else {
         // Using local gaming area positions
         p0 = ownship->getPosition();  // Local gaming area position vector (NED)
         v0 = ownship->getVelocity();  // Local gaming area velocity vector (NED)
      }

      for (unsigned int i = 0; i < numTgts; i++) {

         // Target vectors (ECEF or local gaming area NED)
         osg::Vec3d pt;  // Position Vector
         osg::Vec3d vt;  // Velocity vector [ x y z ] (meters/second)
         if (usingEcefFlg) {
            // Using ECEF
            pt = targets[i]->getGeocPosition();  // Geocentric position (ECEF)
            vt = targets[i]->getGeocVelocity();  // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
         }
         else {
            // Using local gaming area positions
            pt = targets[i]->getPosition();  // Local gaming area position vector (NED)
            vt = targets[i]->getVelocity();  // Local gaming area velocity vector (NED)
         }

         // Target LOS vector
         osg::Vec3d los = (pt - p0);

         // Normalized and compute length [unit vector and range(meters)]
         ranges[i] = los.normalize();

         // Computer range rate (meters/sec)
         rngRates[i] = ( (vt - v0) * los );

         // Save the LOS vectors (own to tgt) and (tgt back to own)
         if (usingEcefFlg) {
            // Rotate the LOS vectors into their local tangent planes
            losO2T[i] = wm * los;
            losT2O[i] = targets[i]->getWorldMat() * (-los);
         }
         else {
            losO2T[i] =  los;
            losT2O[i] = -los;
         }
      }

   }

   // ---
   // Transform LOS vectors to antenna coordinates
   // ---
   {
      // ---
      // 1) Create the transformation matrix, 'mm'
      // ---

      // Start with the body to gimbal matrix
      osg::Matrixd mm = gimbal->getRotMat();

      // Post multi by the inertial (NED) to body matrix
      if (ownHdgOnly) {
         // Using heading only, ignore ownship roll and pitch
         osg::Matrixd rr;
         rr.makeRotate( ownship->getHeading(), 0, 0, 1);
         mm *= rr;
      }
      else {
         // Using ownship roll, pitch and heading
         mm *= ownship->getRotMat();
      }

      // ---
      // 2) Transform the ownship to target LOS vector into gimbal coordinate system
      //       losG = mm * losO2T;
      // ---
      postMultVec3Array(losO2T,mm,losG,numTgts);
   }

   // ---
   // Get gimbal coordinate component arrays and x-y range squared
   // ---
   for (unsigned int i = 0; i < numTgts; i++) {
         xa[i] = losG[i].x();
         ya[i] = losG[i].y();
         za[i] = -losG[i].z();
         ra2[i] = xa[i]*xa[i] + ya[i]*ya[i];
   }

   // ---
   // Compute range along antenna x-y plane
   // ---
   sqrtArray(ra2,ra,numTgts);

   // ---
   // Compute angle off antenna boresight
   // ---
   acosArray(xa, aar, numTgts);

   // ---
   // Compute azimuth off boresight
   // ---
   atan2Array(ya,xa,aazr,numTgts);

   // ---
   // Compute elevation off boresight
   // ---
   atan2Array(za,ra,aelr,numTgts);

   return numTgts;
}

//------------------------------------------------------------------------------
// Sets our Gimbal
//------------------------------------------------------------------------------
void Tdb::setGimbal(const Gimbal* const newGimbal)
{
   // Unref() the old, set and ref() the new
   if (ownship != nullptr) { ownship->unref(); ownship = nullptr; }
   if (gimbal != nullptr) gimbal->unref();

   gimbal = newGimbal;

   if (gimbal != nullptr) {
      gimbal->ref();
      ownship = gimbal->getOwnship();
      if (ownship != nullptr) ownship->ref();
   }
}

} // End Simulation namespace
} // End Eaagles namespace
