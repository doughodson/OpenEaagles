//------------------------------------------------------------------------------
// Classes: IrSeeker, TdbIr
//------------------------------------------------------------------------------

#include "openeaagles/models/systems/IrSeeker.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/systems/IrSensor.hpp"
#include "openeaagles/models/systems/TrackManager.hpp"
#include "openeaagles/models/systems/OnboardComputer.hpp"
#include "openeaagles/models/IrQueryMsg.hpp"

#include "openeaagles/models/environment/IrAtmosphere.hpp"

#include "openeaagles/base/Integer.hpp"
#include "openeaagles/base/List.hpp"
#include "openeaagles/base/PairStream.hpp"

#include "openeaagles/base/util/nav_utils.hpp"
#include "openeaagles/base/util/unit_utils.hpp"

#include <cmath>

// Requirements:
// An irSeeker can have multiple irSensors - so an irSeeker is not
// associated with any single irSensor; each irQueryMsg will contain ptr to the irSensor
// associated with any particular interaction.
//
// Owning player must have an OnboardComputer,which contains a TrackManager.

namespace oe {
namespace models {

//==============================================================================
// Class: IrSeeker
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(IrSeeker, "IrSeeker")
EMPTY_SERIALIZER(IrSeeker)
EMPTY_SLOTTABLE(IrSeeker)
EMPTY_COPYDATA(IrSeeker)

//------------------------------------------------------------------------------
// Event() map
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(IrSeeker)
    ON_EVENT_OBJ(IR_QUERY_RETURN, irQueryReturnEvent,IrQueryMsg)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IrSeeker::IrSeeker() :
               freeQueryStack(MAX_QUERIES), freeQueryLock(0),
               inUseQueryQueue(MAX_QUERIES), inUseQueryLock(0)
{
   STANDARD_CONSTRUCTOR()
}

IrSeeker::IrSeeker(const IrSeeker& org) :
               freeQueryStack(MAX_QUERIES), freeQueryLock(0),
               inUseQueryQueue(MAX_QUERIES), inUseQueryLock(0)
{
   STANDARD_CONSTRUCTOR()
   copyData(org,true);
}


IrSeeker::~IrSeeker()
{
   STANDARD_DESTRUCTOR()
}

IrSeeker& IrSeeker::operator=(const IrSeeker& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

IrSeeker* IrSeeker::clone() const
{
   return new IrSeeker(*this);
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------

void IrSeeker::deleteData()
{
   clearQueues();
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool IrSeeker::shutdownNotification()
{
   clearQueues();
   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void IrSeeker::reset()
{
    BaseClass::reset();
    clearQueues();
}


#ifdef USE_TDBIR
//------------------------------------------------------------------------------
// Process the Players-Of-Interest (POI) list
//------------------------------------------------------------------------------
unsigned int IrSeeker::processPlayersOfInterest(base::PairStream* const poi)
{
   const auto tdb0 = new TdbIr(getMaxPlayersOfInterest(), this);

   unsigned int ntgts = tdb0->processPlayers(poi);
   setCurrentTdb(tdb0);
   tdb0->unref();

   return ntgts;
}
#endif

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void IrSeeker::process(const double dt)
{
   BaseClass::process(dt);

   // ---
   // Update IR query queues: from 'in-use' to 'free'
   // ---
   base::lock(inUseQueryLock);
   int n = inUseQueryQueue.entries();
   for (int i = 0; i < n; i++) {
      IrQueryMsg* query = inUseQueryQueue.get();
      if (query != nullptr) {
         if (query->getRefCount() <= 1) {
            // No one else is referencing the query, push on free stack
            query->clear();
            base::lock(freeQueryLock);
            if (freeQueryStack.isNotFull()) {
               freeQueryStack.push(query);
            }
            else {
               query->unref();
            }
            base::unlock(freeQueryLock);
         }
         else {
            // Others are still referencing the query, put back on in-use queue
            inUseQueryQueue.put(query);
         }
      }
   }
   base::unlock(inUseQueryLock);
}


//------------------------------------------------------------------------------
// clearQueues() -- clear out all queues
//------------------------------------------------------------------------------
void IrSeeker::clearQueues()
{
   base::lock(freeQueryLock);
   IrQueryMsg* query = freeQueryStack.pop();
   while (query != nullptr) {
      query->unref();
      query = freeQueryStack.pop();
   }
   base::unlock(freeQueryLock);

   base::lock(inUseQueryLock);
   query = inUseQueryQueue.get();
   while (query != nullptr) {
      query->unref();
      query = inUseQueryQueue.get();
   }
   base::unlock(inUseQueryLock);
}

//------------------------------------------------------------------------------
// TRANSMIT AND RECEIVE FUNCTION (SENSOR STUFF)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// irRequestSignature() -- Send an IR query packet at all active players to request an IR signature
//------------------------------------------------------------------------------
void IrSeeker::irRequestSignature(IrQueryMsg* const irQuery)
{
   // Need something to store the required data for the IR signatures and someone to send to

   Tdb* tdb0 = getCurrentTDB();
   Player* ownship = getOwnship();
   if (irQuery == nullptr || tdb0 == nullptr || ownship == nullptr) {
      // Clean up and leave
      if (tdb0 != nullptr) tdb0->unref();
      return;
   }

   // ---
   // Compute gimbal boresight data for our targets
   // ---

   // FAB - cannot use ownHdgOnly
   unsigned int ntgts = tdb0->computeBoresightData();
   if (ntgts > MAX_PLAYERS) ntgts = MAX_PLAYERS;

   // ---
   // If we have targets
   // ---
   const base::Vec3d* losG = tdb0->getGimbalLosVectors();
   if (ntgts > 0 && losG != nullptr) {

      // Fetch the required data arrays from the TargetDataBlock
      const double* ranges = tdb0->getTargetRanges();
      const double* rngRates = tdb0->getTargetRangeRates();
      const double* anglesOffBoresight = tdb0->getBoresightErrorAngles();
      const base::Vec3d* losO2T = tdb0->getLosVectors();
      const base::Vec3d* losT2O = tdb0->getTargetLosVectors();
      Player** targets = tdb0->getTargets();
      const double maximumRange = irQuery->getMaxRangeNM()*base::distance::NM2M;

      // ---
      // Send query packets to the targets
      // ---
      for (unsigned int i = 0; i < ntgts; i++) {

         // filter on sensor max range
         // can't filter on sensor range in processPlayers - different sensors can have different max range
         if (maximumRange > 0.0 && ranges[i] > maximumRange)
            continue;

         // Get a free query packet
         base::lock(freeQueryLock);
         IrQueryMsg* query = freeQueryStack.pop();
         base::unlock(freeQueryLock);

         if (query == nullptr) {
            query = new IrQueryMsg();
            //if (ownship->getID() != 1) {
            //    static tcnt = 0;
            //    tcnt++;

            //if (isMessageEnabled(MSG_INFO)) {
            //    std::cout << "new IrQueryMsg(" << this << "): " << tcnt << ", inused: " << inUseEmQueue.entries() << ", em = " << em << std::endl;
            //}

            //}
         }

         // Send the IR query message to the other player
         if (query != nullptr) {

            // a) Copy the template query msg
            *query = *irQuery;

            // b) Set target unique data
            query->setGimbal(this);
            query->setOwnship(ownship);

            query->setRange( static_cast<double>(ranges[i]) );
            query->setLosVec( losO2T[i] );
            query->setTgtLosVec( losT2O[i] );
            query->setRangeRate( static_cast<double>(rngRates[i]) );
            query->setTarget(targets[i]);
            query->setAngleOffBoresight( static_cast<double>(anglesOffBoresight[i]) );

            query->setGimbalAzimuth( static_cast<double>(getAzimuth()) );
            query->setGimbalElevation( static_cast<double>(getElevation()) );

            // c) Send the query to the target
            targets[i]->event(IR_QUERY, query);

            // d) Dispose of the query
            if (query->getRefCount() <= 1) {
               // Recycle the query packet
               query->clear();
               base::lock(freeQueryLock);
               if (freeQueryStack.isNotFull()) {
                  freeQueryStack.push(query);
               }
               else {
                  query->unref();
               }
               base::unlock(freeQueryLock);
            }
            else {
               // Store for future reference
               base::lock(inUseQueryLock);
               if (inUseQueryQueue.isNotFull()) {
                  inUseQueryQueue.put(query);
               }
               else {
                  // Just forget it
                  query->unref();
               }
               base::unlock(inUseQueryLock);
            }
         }
         else {
            // When we couldn't get a free query packet
            if (isMessageEnabled(MSG_WARNING)) {
               std::cerr << "IR Seeker: OUT OF Query messages!" << std::endl;
            }
         }
      }
   }

   // Unref() the TDB
   tdb0->unref();
}

//------------------------------------------------------------------------------
// irQueryReturnEvent() -- process the return of an IR query message
//------------------------------------------------------------------------------
bool IrSeeker::irQueryReturnEvent(IrQueryMsg* const msg)
{
   // IrSeeker does not have any real role in processing return, so IrSeeker forwards to IrSensor
   // This maintains the pattern used in RF code.
   msg->getSendingSensor()->calculateIrQueryReturn(msg);
   return true;
}


#ifdef USE_TDBIR


//==============================================================================
// Class: Tdb
//==============================================================================

IMPLEMENT_SUBCLASS(TdbIr,"Seeker_TdbIr")
EMPTY_SLOTTABLE(TdbIr)
EMPTY_SERIALIZER(TdbIr)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
TdbIr::TdbIr(const unsigned int mt, const Gimbal* const gp) : Tdb(mt,gp)
{
   STANDARD_CONSTRUCTOR()
}

TdbIr::TdbIr()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TdbIr::copyData(const TdbIr& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void TdbIr::deleteData()
{
}

//------------------------------------------------------------------------------
// Process players-of-interest --- Scan the provided player list and compute range,
// range rate, normalized Line-Of-Sight (LOS) vectors for each target player.
// (Background task)
//------------------------------------------------------------------------------
unsigned int TdbIr::processPlayers(base::PairStream* const players)
{
   // Clear the old data
   clearArrays();

   // ---
   // Early out checks (no ownship, no players of interest, no target data arrays)
   // ---
   if (gimbal == 0 || ownship == nullptr || players == nullptr || maxTargets == 0) return 0;

   //const base::Pair* p = ((Player*)ownship)->getIrSystemByType( typeid(IrSensor) );
   //if (p == 0) return 0;

   // FAB - refactored
   //const IrSensor* irSensor = (const IrSensor*)( p->object() );
   //if (irSensor == 0)
      //return 0;

   // FAB - limit is +/- (1/2 FORtheta + 1/2 IFOVtheta) (but both get..Theta() actually return 1/2 Theta)
   //double fieldOfRegardTheta = irSensor->getFieldOfRegardTheta() + irSensor->getIFOVTheta();
   //double maxRange = irSensor->getMaximumRange();

   // FAB - basically the same as TDB/gimbal version:
   const double maxRange = gimbal->getMaxRange2PlayersOfInterest();
   const double maxAngle = gimbal->getMaxAngle2PlayersOfInterest();

   // ---
   // Get our position and velocity vectors
   // ## using ownship position for now; should include the location of the gimbal ##
   // ---
   osg::Vec3 p0 = ownship->getPosition();  // Position Vector
   osg::Vec3 v0 = ownship->getVelocity();  // Ownship Velocity Vector

   // ---
   // 1) Scan the player list --- compute the normalized Line-Of-Sight (LOS) vectors,
   // range, and range rate for each target.
   // ---
   for (base::List::Item* item = players->getFirstItem(); item != 0 && numTgts < maxTargets; item = item->getNext()) {


      // Get the pointer to the target player
      base::Pair* pair = (base::Pair*)(item->getValue());
      Player* target = (Player*)(pair->object());

     // FAB - testing - exclude our launch vehicle in tdb
     //if (ownship->isMajorType(Player::WEAPON) && ((Weapon*)ownship)->getLaunchVehicle() == target)
     //continue;

      if (target != ownship && target->isActive()) {
         bool aboveHorizon = true;
         aboveHorizon = horizonCheck (ownship->getPosition(), target->getPosition());

         // FAB - refactored - don't continue if we know we're excluding this target
         if (!aboveHorizon) continue;

         // Determine if target is within azimuth and elevation checks. If it is, keep it.
         // Otherwise, reject.

         osg::Vec3 targetPosition = target->getPosition();
         osg::Vec3 losVector = targetPosition - p0;
         //osg::Vec3 xlos = -losVector;
       double aazr;
       double aelr;
       double ra;

      if (irSensor->getSeeker()->getOwnHeadingOnly()) {
         // FAB - this calc for gimbal ownHeadingOnly true
         // compute ranges
         double gndRng2 = losVector.x()*losVector.x() + losVector.y()*losVector.y();
         ra = std::sqrt(gndRng2);

         // compute angles
         double los_az = std::atan2(losVector.y(),losVector.x());
         double hdng = ownship->getHeadingR();
         aazr = base::Angle::aepcdRad(los_az - static_cast<float>(hdng));
         aelr = std::atan2(-losVector.z(), ra);
      }
      else {
         // FAB - this calc for gimbal ownHeadingOnly false
         //osg::Vec4 los0( losVector.x(), losVector.y(), losVector.z(), 0.0 );
         //osg::Vec4 aoi = ownship->getRotMat() * los0;
         osg::Vec3 aoi = ownship->getRotMat() * losVector;
         // 3) Compute the azimuth and elevation angles of incidence (AOI)

         // 3-a) Get the aoi vector values & compute range squared
         double xa = aoi.x();
         double ya = aoi.y();
         double za = -aoi.z();

         ra = std::sqrt(xa*xa + ya*ya);
         // 3-b) Compute azimuth: az = atan2(ya, xa)
         aazr = std::atan2(ya, xa);
         // 3-c) Compute elevation: el = atan2(za, ra), where 'ra' is sqrt of xa*xa & ya*ya
         aelr = std::atan2(za,ra);
      }

         double absoluteAzimuth = aazr;

         if (aazr < 0) absoluteAzimuth = -aazr;

         double absoluteElevation = aelr;
         if (aelr < 0) absoluteElevation = -aelr;

         bool withinView = true;

      //   double fieldOfRegardTheta = 0;
      //   double sensorMaxRange = 0;
      //   {
      //      //const base::Pair* p = ((Player*)ownship)->getIrSystemByType( typeid(IrSensor) );
      //      //if (p != nullptr) {
      //         //const IrSensor* irSensor = (const IrSensor*)( p->object() );
      //         // fieldOfRegardTheta = irSensor->getFieldOfRegardTheta();
          //// FAB - limit is +/- (1/2 FORtheta + 1/2 IFOVtheta) (but both get..Theta() actually return 1/2 Theta)
          //fieldOfRegardTheta = irSensor->getFieldOfRegardTheta() + irSensor->getIFOVTheta();
      //         sensorMaxRange = irSensor->getMaximumRange();
      //      //}
      //   }

         if ((absoluteAzimuth > maxAngle) ||  // outside field of view
            (absoluteElevation > maxAngle) ||
            (ra > maxRange) ||    // beyond max range of sensor
            !aboveHorizon)
            withinView = false;

         if (withinView) {

            // Ref() and save the target pointer
            // (## It must be unref()'d by the owner/manager of the Tdb array ##)
            target->ref();
            targets[numTgts] = target;

          // FAB - these seem to be unnecessary - recalc'd by Tdb::computeBoresightData anyway
            // Line-Of-Sight (LOS) vector (world)
            //losO2T[numTgts] = target->getPosition() - p0;

            // Normalized and compute length [unit vector and range(meters)]
            //ranges[numTgts] = losO2T[numTgts].normalize();

            // Computer range rate (meters/sec)
            //rngRates[numTgts] = (double) ((target->getVelocity() - v0) * losO2T[numTgts]);

            // Save the target pointer (for quick access)
            numTgts++;
         }
      } //(target != ownship && target->isActive())
   }

   return numTgts;
}

//------------------------------------------------------------------------------
// Check to see if two positions are over the horizon from each other.
// return TRUE if they are both within horizon distance, FALSE if they
// are over the horizon.
//------------------------------------------------------------------------------
bool TdbIr::horizonCheck(const osg::Vec3& position1, const osg::Vec3& position2)
{

   bool aboveHorizon = true;

   //LET .FIRST.NODE.DISTANCE.TO.HORIZON
   //         = SQRT.F(MAX.F (2.0 * EARTH.RADIUS * .FIRST.NODE.POSITION(3), 1.0) )

   double distance1 = std::sqrt( static_cast<double>(2.0f * base::nav::ERADM * -position1.z()) );
   if (distance1 < 1.0f) distance1 = 1.0f;


   //      LET .SECOND.NODE.DISTANCE.TO.HORIZON
   //         = SQRT.F(MAX.F (2.0 * EARTH.RADIUS * .SECOND.NODE.POSITION(3), 1.0) )

   double distance2 = std::sqrt( static_cast<double>(2.0f * base::nav::ERADM * -position2.z()) );
   if (distance2 < 1.0f) distance2 = 1.0f;

   //LET .RELATIVE.POSITION(*)
   //   = UT.LINEAR.COMBINATION.OF.VECTORS.F(1.0, .FIRST.NODE.POSITION(*),
   //                                        -1.0, .SECOND.NODE.POSITION(*))
   // LET .RELATIVE.POSITION(3) = 0.0

   // LET .GROUND.TRACK.RANGE = UT.NORM.F(.RELATIVE.POSITION(*))

   osg::Vec3 groundVec = position1 - position2;

   double gndRng = std::sqrt ((groundVec.x() * groundVec.x())
                     + (groundVec.y() * groundVec.y()));

   //IF .GROUND.TRACK.RANGE < .FIRST.NODE.DISTANCE.TO.HORIZON
   //                        + .SECOND.NODE.DISTANCE.TO.HORIZON

   if (gndRng >= distance1 + distance2)
      aboveHorizon = false;

    return aboveHorizon;
}
#endif

}
}

