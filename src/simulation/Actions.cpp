
#include "openeaagles/simulation/Actions.h"

#include "openeaagles/simulation/Bomb.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/OnboardComputer.h"
#include "openeaagles/simulation/Sar.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Steerpoint.h"
#include "openeaagles/simulation/StoresMgr.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/LatLon.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {


// =============================================================================
// class: Action
// =============================================================================

IMPLEMENT_SUBCLASS(Action,"Action")
EMPTY_SLOTTABLE(Action)

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
Action::Action()
{
   STANDARD_CONSTRUCTOR()

   manager = nullptr;

   refId = 0;
   completed = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Action::copyData(const Action& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      manager = nullptr;
   }

   refId = org.refId;
   completed = org.completed;

   setManager(nullptr);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Action::deleteData()
{
   setManager(nullptr);
}

//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------

// True if action is ready to start
bool Action::isReadyToStart()
{
   return !isInProgress() && !isCompleted();
}

// True if action is in progress
bool Action::isInProgress()
{
   return (manager != nullptr);
}

// True if action has been completed
bool Action::isCompleted()
{
   return completed;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the completed flag
void Action::setCompleted(const bool flg)
{
   if (flg) setManager(nullptr);
   completed = flg;
}

// Sets the message ref ID
void Action::setRefId(const int id)
{
   refId = id;
}

// Set our route manager
bool Action::setManager(OnboardComputer* const mgr)
{
   manager = mgr;
   return true;
}

//------------------------------------------------------------------------------
// Starts this action
//------------------------------------------------------------------------------
bool Action::trigger(OnboardComputer* const mgr)
{
   setManager(mgr);
   completed = false;
   return true;
}

//------------------------------------------------------------------------------
// Cancels this action
//------------------------------------------------------------------------------
bool Action::cancel()
{
   completed = true;
   setManager(nullptr);
   return true;
}

//------------------------------------------------------------------------------
// process() -- action processing
//------------------------------------------------------------------------------
void Action::process(const LCreal)
{
}

//------------------------------------------------------------------------------
// Execute as an UBF action
// -- Use the 'actor' to find our OBC and 'trigger' this action.
//------------------------------------------------------------------------------
bool Action::execute(Basic::Component* actor)
{
   bool ok = false;
   if (actor != nullptr) {

      // Was the actor our OBC?
      OnboardComputer* obc = dynamic_cast<OnboardComputer*>( actor );

      // If not, was it our ownship ...
      //   and can we get our OBC from our ownship
      if (obc == nullptr) {
         Player* own = dynamic_cast<Player*>( actor );
         if (own != nullptr) {
            obc = own->getOnboardComputer();
         }
      }

      // If we could find our OBC from the actor then trigger this action
      if (obc != nullptr) {
         ok = trigger(obc);
      }

   }
   return ok;
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Action::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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


// =============================================================================
// class: ActionImagingSar
// =============================================================================
IMPLEMENT_SUBCLASS(ActionImagingSar,"ActionImagingSar")

BEGIN_SLOTTABLE(ActionImagingSar)
   "sarLatitude",    //  1) Target's latitude (LatLon)
   "sarLongitude",   //  2) Target's longitude (LatLon)
   "sarElevation",   //  3) Target's elevation (Distance) (default: 0)
   "resolution",     //  4) Image (pixel) resolution   (Distance) (default: 1 meter)
   "imageSize",      //  5) Image size: height & width (pixels) (default: 512)
END_SLOTTABLE(ActionImagingSar)

// Map slot table to handles
BEGIN_SLOT_MAP(ActionImagingSar)
    ON_SLOT(1, setSlotSarLat,     Basic::LatLon)
    ON_SLOT(2, setSlotSarLon,     Basic::LatLon)
    ON_SLOT(3, setSlotSarElev,    Basic::Distance)
    ON_SLOT(4, setSlotResolution, Basic::Distance)
    ON_SLOT(5, setSlotImageSize,  Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
ActionImagingSar::ActionImagingSar()
{
   STANDARD_CONSTRUCTOR()

   sar = nullptr;

   sarLatitude = 0.0;
   sarLongitude = 0.0;
   sarElevation = 0.0;
   resolution = 3;
   imgSize = 512;
   orientation = 0.0;

   timer = 0.0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ActionImagingSar::copyData(const ActionImagingSar& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      sar = nullptr;
   }

   sarLatitude = org.sarLatitude;
   sarLongitude = org.sarLongitude;
   sarElevation = org.sarElevation;
   resolution = org.resolution;
   imgSize = org.imgSize;
   orientation = org.orientation;

   timer = 0.0;
   setSarSystem(nullptr);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void ActionImagingSar::deleteData()
{
}

//------------------------------------------------------------------------------
// Default function: Start the SAR capture, ATR run and Image download process
//------------------------------------------------------------------------------
bool ActionImagingSar::trigger(OnboardComputer* const mgr)
{
   bool ok = false;

   if (mgr != nullptr) {

      // Find our ownship player & SAR system
      Player* ownship = static_cast<Player*>(mgr->findContainerByType(typeid(Player)));
      if (ownship != nullptr) {
         Basic::Pair* pair = ownship->getSensorByType(typeid(Sar));
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Looking for SAR: pair = " << pair << std::endl;
         }
         if (pair != nullptr) {
            setSarSystem( static_cast<Sar*>(pair->object()) );
         }
      }

      if (sar != nullptr && ownship != nullptr) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Requesting an image from the SAR: refId: " << getRefId() << std::endl;
         }
         sar->setStarePoint( getSarLatitude(), getSarLongitude(), static_cast<LCreal>(getSarElevation()) );
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "And resolution: " << getResolution() << std::endl;
         }
         sar->requestImage(getImageSize(), getImageSize(), getResolution() );
         ok = true;
      }

      timer = 0;
   }

   if (ok) setManager(mgr);
   else setManager(nullptr);

   return ok;
}

//------------------------------------------------------------------------------
// Cancels this action
//------------------------------------------------------------------------------
bool ActionImagingSar::cancel()
{
   if (getSarSystem() != nullptr) {
      getSarSystem()->cancel();
   }
   return BaseClass::cancel();
}

//------------------------------------------------------------------------------
// process() -- action processing
//------------------------------------------------------------------------------
void ActionImagingSar::process(const LCreal dt)
{
   BaseClass::process(dt);

   if (isInProgress() && getSarSystem() != nullptr) {

      // Check if SAR has finished
      if ( !getSarSystem()->isImagingInProgress()) {
         // Imaging is NOT in progress, so it must have completed ...
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Processing SAR -- completed!" << std::endl;
         }
         setCompleted(true);
      }

      // Process safety time-out
      timer += dt;
      static const LCreal MAX_SAR_TIME = 120.0f;
      if (timer > MAX_SAR_TIME) {
         // Cancel the SAR
         cancel();
         setCompleted(true);
      }
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the SAR latitude in degrees
bool ActionImagingSar::setSarLatitude(const double v)
{
   bool ok = false;
   if (v >= -90.0 && v <= 90.0) {
      sarLatitude = v;
      ok = true;
   }
   return ok;
}

// Sets the SAR longitude in degrees
bool ActionImagingSar::setSarLongitude(const double v)
{
   bool ok = false;
   if (v >= -180.0 && v <= 180.0) {
      sarLongitude = v;
      ok = true;
   }
   return ok;
}

// Sets the SAR elevation in meters
bool ActionImagingSar::setSarElevation(const double v)
{
   sarElevation = v;
   return true;
}

// Sets the SAR image resolution (meters)
bool ActionImagingSar::setResolution(const LCreal r)
{
   resolution = r;
   return true;
}

// Sets the image size
bool ActionImagingSar::setImageSize(const unsigned int v)
{
   bool ok = false;
   if (v == 512 || v == 1024 || v == 2048 || v == 4096) {
      imgSize = v;
      ok = true;
   }
   return ok;
}

// Sets the completed flag
void ActionImagingSar::setCompleted(const bool flg)
{
   if (flg) setSarSystem(nullptr);
   BaseClass::setCompleted(flg);
}

// Set the SAR system
void ActionImagingSar::setSarSystem(Sar* const p)
{
   if (sar != nullptr) {
      sar->unref();
   }
   sar = p;
   if (sar != nullptr) {
      sar->ref();
   }
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool ActionImagingSar::setSlotSarLat(const Basic::LatLon* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setSarLatitude(msg->getDouble());
   }
   return ok;
}

bool ActionImagingSar::setSlotSarLon(const Basic::LatLon* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setSarLongitude(msg->getDouble());
   }
   return ok;
}

bool ActionImagingSar::setSlotSarElev(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      LCreal elev = Basic::Meters::convertStatic( *msg );
      ok = setSarElevation( elev );
   }
   return ok;
}

bool ActionImagingSar::setSlotResolution(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      LCreal res = Basic::Meters::convertStatic( *msg );
      ok = setResolution( res );
   }
   return ok;
}

bool ActionImagingSar::setSlotImageSize(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setImageSize( msg->getInt() );
      if (!ok) {
         if (isMessageEnabled(MSG_INFO)) {
         std::cout << "  ActionImagingSar::setSlotImageSize must be 512, 1024, 2048 or 4096, not " << msg->getInt() << std::endl;
      }
   }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Computes the planned image orientation (degs)
//------------------------------------------------------------------------------
LCreal ActionImagingSar::computeOrientation(const Steerpoint* const wp)
{
   if (wp != nullptr) {
      double slat = wp->getLatitude();
      double slon = wp->getLongitude();
      double dlat = getSarLatitude();
      double dlon = getSarLongitude();
      double brg, distNM;
      Basic::Nav::gll2bd(slat, slon, dlat, dlon, &brg, &distNM);
      orientation = static_cast<LCreal>(-brg);
   }
   return orientation;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* ActionImagingSar::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& ActionImagingSar::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "sarLatitude: " << sarLatitude << std::endl;

    indent(sout,i+j);
    sout << "sarLongitude: " << sarLongitude << std::endl;

    indent(sout,i+j);
    sout << "sarElevation: " << sarElevation << std::endl;

    indent(sout,i+j);
    sout << "resolution: " << resolution << std::endl;

    indent(sout,i+j);
    sout << "imageSize: " << imgSize << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}


// =============================================================================
// class: ActionWeaponRelease
// =============================================================================

IMPLEMENT_SUBCLASS(ActionWeaponRelease,"ActionWeaponRelease")
//EMPTY_SLOTTABLE(ActionWeaponRelease)

BEGIN_SLOTTABLE(ActionWeaponRelease)
   "targetLatitude",    //  1) Target's latitude (LatLon)
   "targetLongitude",   //  2) Target's longitude (LatLon)
   "targetElevation",   //  3) Target's elevation (feet)
   "station",           //  4) Station number to use (default to next available)
END_SLOTTABLE(ActionWeaponRelease)

// Map slot table to handles
BEGIN_SLOT_MAP(ActionWeaponRelease)
    ON_SLOT(1, setSlotTargetLat, Basic::LatLon)
    ON_SLOT(2, setSlotTargetLon, Basic::LatLon)
    ON_SLOT(3, setSlotTargetElev, Basic::Number)
    ON_SLOT(4, setSlotStationNum, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
ActionWeaponRelease::ActionWeaponRelease()
{
   STANDARD_CONSTRUCTOR()

   station = 0;
   targetLatitude = 0.0;
   targetLongitude = 0.0;
   targetElevation = 0.0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ActionWeaponRelease::copyData(const ActionWeaponRelease& org, const bool)
{
   BaseClass::copyData(org);

   station = org.station;
   targetLatitude = org.targetLatitude;
   targetLongitude = org.targetLongitude;
   targetElevation = org.targetElevation;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void ActionWeaponRelease::deleteData()
{
}

//------------------------------------------------------------------------------
// Default function: Launch a weapon
//------------------------------------------------------------------------------
bool ActionWeaponRelease::trigger(OnboardComputer* const mgr)
{
   bool ok = false;

   if (mgr != nullptr) {
      Player* own = static_cast<Player*>(mgr->findContainerByType(typeid(Player)));
      if (own != nullptr) {

         StoresMgr* sms = own->getStoresManagement();
         if (sms != nullptr) {

            // Get the simulation ref point
            Simulation* sim = own->getSimulation();
            const double refLat = sim->getRefLatitude();
            const double refLon = sim->getRefLongitude();

            // Computes the target's position vector
            osg::Vec3 tgtPos;                            // Target position  (m) NED
            Basic::Nav::convertLL2PosVec(
               refLat, refLon,                           // Ref point (at sea level)
               getTargetLatitude(), getTargetLongitude(), getTargetElevation(),
               &tgtPos); // x,y,z  NED

            if (isMessageEnabled(MSG_INFO)) {
               std::cout << "ActionWeaponRelease::trigger() Release weapon at [ " << tgtPos[0] << ", } " << tgtPos[1] << " ]" << std::endl;
            }

            // Release the weapon and set the target
            Weapon* flyout = sms->releaseOneBomb();
            if (flyout != nullptr) {
                flyout->setTargetPosition(tgtPos);
            }
         }

         BaseClass::setCompleted(true);
         ok = true;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the target location (latitude & longitude in radians, elevation in meters)
bool ActionWeaponRelease::setTargetLocation(const double tgtLat, const double tgtLon, const double tgtElev)
{
   targetLatitude = tgtLat;
   targetLongitude = tgtLon;
   targetElevation = tgtElev;
   return true;
}

// Set planned station number
bool ActionWeaponRelease::setStation(const unsigned int num)
{
   station = num;
   return true;
}

// Slot functions
bool ActionWeaponRelease::setSlotTargetLat(const Basic::LatLon* newLat)
{
    bool ok = false;
    if (newLat != nullptr) {
        targetLatitude = newLat->getDouble();
        ok = true;
    }
    return ok;
}
bool ActionWeaponRelease::setSlotTargetLon(const Basic::LatLon* newLon)
{
    bool ok = false;
    if (newLon != nullptr) {
        targetLongitude = newLon->getDouble();
        ok = true;
    }
    return ok;
}
bool ActionWeaponRelease::setSlotTargetElev(const Basic::Number* newElev)
{
    bool ok = false;
    if (newElev != nullptr) {
        targetElevation = newElev->getReal();
        ok = true;
    }
    return ok;
}
bool ActionWeaponRelease::setSlotStationNum(const Basic::Number* newStation)
{
    bool ok = false;
    if (newStation != nullptr) {
        station = newStation->getInt();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* ActionWeaponRelease::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& ActionWeaponRelease::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "targetLatitude: " << targetLatitude << std::endl;

    indent(sout,i+j);
    sout << "targetLongitude: " << targetLongitude << std::endl;

    indent(sout,i+j);
    sout << "targetElevation: " << targetElevation << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

// =============================================================================
// class: ActionDecoyRelease
// =============================================================================

IMPLEMENT_SUBCLASS(ActionDecoyRelease,"ActionDecoyRelease")
BEGIN_SLOTTABLE(ActionDecoyRelease)
   "numToLaunch",    //  1) number of decoys to launch
   "interval",       //  2) time, in seconds, between launches
END_SLOTTABLE(ActionDecoyRelease)

// Map slot table to handles
BEGIN_SLOT_MAP(ActionDecoyRelease)
    ON_SLOT(1, setSlotNumToLaunch, Basic::Number)
    ON_SLOT(2, setSlotInterval, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
ActionDecoyRelease::ActionDecoyRelease()
{
   STANDARD_CONSTRUCTOR()
   interval = 0.0;
   numToLaunch = 1;
   tod = 0.0;
   startTOD = 0.0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ActionDecoyRelease::copyData(const ActionDecoyRelease& org, const bool)
{
   BaseClass::copyData(org);
   interval = org.interval;
   numToLaunch = org.numToLaunch;
   tod = org.tod;
   startTOD = org.startTOD;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void ActionDecoyRelease::deleteData()
{
}

//------------------------------------------------------------------------------
//Default function: Launch a decoy
//------------------------------------------------------------------------------
bool ActionDecoyRelease::trigger(OnboardComputer* const mgr)
{
   bool ok = false;

   if (mgr != nullptr) {
      Player* own = static_cast<Player*>(mgr->findContainerByType(typeid(Player)));
      if (own != nullptr) {
         StoresMgr* sms = own->getStoresManagement();
         if (sms != nullptr) {
         if (numToLaunch > 1) {
                if (interval == 0.0) {
                    // launch all at once
                    ok = true;
                    for (int i = 0; i < numToLaunch; i++) {
                        sms->releaseOneDecoy();
                    }
                }
                else {
                    // release one, then start counting
                    sms->releaseOneDecoy();
                    tod = own->getSimulation()->getSimTimeOfDay();
                    startTOD = tod;
                    // decrease our number to launch
                    numToLaunch--;
                    // set our route manager
                    setManager(mgr);
                }
            }
            else {
                ok = true;
                // only one to launch, then just launch one!
                sms->releaseOneDecoy();
            }
         }
      }
   }

   // do we need to launch more at a certain interval?  If so, we aren't complete yet!

   BaseClass::setCompleted(ok);

   return ok;
}

void ActionDecoyRelease::process(const LCreal)
{
    // keep counting until we have our "interval" of seconds
    OnboardComputer* mgr = getManager();
    if (mgr != nullptr) {
        Player* own = static_cast<Player*>(mgr->findContainerByType(typeid(Player)));
        if (own != nullptr) {
            tod = own->getSimulation()->getSimTimeOfDay();
            if (interval < (tod - startTOD)) {
                // we have hit our interval, release another decoy
                StoresMgr* sms = own->getStoresManagement();
                if (sms != nullptr) {
                    sms->releaseOneDecoy();
                    numToLaunch--;
                    if (numToLaunch == 0) {
                        setManager(nullptr);
                        BaseClass::setCompleted(true);
                    }
                    else {
                        // keep going, start counting again!
                        startTOD = tod;
                    }
                }
            }
        }
    }
}

// Slot functions
bool ActionDecoyRelease::setSlotNumToLaunch(const Basic::Number* x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = setNumToLaunch(x->getInt());
    }
    return ok;
}
bool ActionDecoyRelease::setSlotInterval(const Basic::Number* x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = setInterval(x->getReal());
    }
    return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* ActionDecoyRelease::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& ActionDecoyRelease::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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


// =============================================================================
// class: ActionCamouflageType
// =============================================================================

IMPLEMENT_SUBCLASS(ActionCamouflageType,"ActionCamouflageType")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(ActionCamouflageType)
   "camouflageType",    //  1) User defined camouflage type (positive integer or zero for none)
END_SLOTTABLE(ActionCamouflageType)

// Map slot table to handles
BEGIN_SLOT_MAP(ActionCamouflageType)
    ON_SLOT( 1, setSlotCamouflageType, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
ActionCamouflageType::ActionCamouflageType()
{
   STANDARD_CONSTRUCTOR()

   camouflage = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void ActionCamouflageType::copyData(const ActionCamouflageType& org, const bool)
{
   BaseClass::copyData(org);

   camouflage = org.camouflage;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void ActionCamouflageType::deleteData()
{
}

//------------------------------------------------------------------------------
// Default function: Change our ownship's camouflage type (one-shot)
//------------------------------------------------------------------------------
bool ActionCamouflageType::trigger(OnboardComputer* const mgr)
{
   bool ok = false;

   if (mgr != nullptr) {
      Player* own = static_cast<Player*>(mgr->findContainerByType(typeid(Player)));
      if (own != nullptr) {

         // Set our ownship's camouflage type
         own->setCamouflageType( getCamouflageType() );
         // that's all folks
         BaseClass::setCompleted(true);
         ok = true;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the camouflage type for this player
bool ActionCamouflageType::setCamouflageType(const unsigned int v)
{
   camouflage = v;
   return true;
}

// Sets user defined camouflage type
bool ActionCamouflageType::setSlotCamouflageType(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int ii = msg->getInt();
      if (ii >= 0) {
         ok = setCamouflageType( ii );
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* ActionCamouflageType::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& ActionCamouflageType::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "camouflageType: " << camouflage << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Simulation namespace
} // End Eaagles namespace
