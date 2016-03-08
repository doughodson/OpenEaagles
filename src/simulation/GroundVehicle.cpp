#include "openeaagles/simulation/GroundVehicle.h"

#include "openeaagles/base/List.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/osg/Matrix"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Distances.h"
#include "openeaagles/base/units/Times.h"

#include <cmath>

namespace oe {
namespace simulation {

//==============================================================================
// class GroundVehicle
//==============================================================================
IMPLEMENT_SUBCLASS(GroundVehicle,"GroundVehicle")
EMPTY_SERIALIZER(GroundVehicle)

static const double DEFAULT_LAUNCHER_UP_ANGLE = static_cast<double>(base::PI/2.0);  // Default max launcher angle (rad)
static const double DEFAULT_LAUNCHER_DOWN_ANGLE = 0.0;                              // Default min launcher angle (rad)
static const double DEFAULT_LAUNCHER_MOVE_TIME  = 10.0f;                            // Default max launcher movement time (sec)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(GroundVehicle)
    "commandedPosition",      // 1: Launcher's init commanded position [ "up" "down" ] (base::Identifier)
    "launcherDownAngle",      // 2: Min (down) Launcher angle (base::Angle)
    "launcherUpAngle",        // 3: Max (up) Launcher angle (base::Angle)
    "launcherMoveTime",       // 4: Max time to move between 'down' and 'up' positions (base::Time)
END_SLOTTABLE(GroundVehicle)

// Map slot table to handles
BEGIN_SLOT_MAP(GroundVehicle)
    ON_SLOT(1, setSlotCommandedPosition, base::Identifier)
    ON_SLOT(2, setSlotLauncherDownAngle, base::Angle)
    ON_SLOT(3, setSlotLauncherUpAngle,   base::Angle)
    ON_SLOT(4, setSlotLauncherMoveTime,  base::Time)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
GroundVehicle::GroundVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericGroundVehicle");
    setType(&generic);

   lnchrDownAngle = DEFAULT_LAUNCHER_DOWN_ANGLE;
   lnchrUpAngle   = DEFAULT_LAUNCHER_UP_ANGLE;
   lnchrMoveTime  = DEFAULT_LAUNCHER_MOVE_TIME;
   lnchrAngle     = 0.0;
   lnchrRate      = 0.0;
   initLnchrPos   = NONE;
   cmdLnchrPos    = NONE;

   setTerrainOffset(1.5);      // default offset from terrain to CG
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void GroundVehicle::copyData(const GroundVehicle& org, const bool)
{
   BaseClass::copyData(org);

   lnchrDownAngle = org.lnchrDownAngle;
   lnchrUpAngle = org.lnchrUpAngle;
   lnchrMoveTime  = org.lnchrMoveTime;
   lnchrAngle = org.lnchrAngle;
   lnchrRate = org.lnchrRate;
   initLnchrPos = org.initLnchrPos;
   cmdLnchrPos = org.cmdLnchrPos;
}

void GroundVehicle::deleteData()
{
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int GroundVehicle::getMajorType() const
{
    return GROUND_VEHICLE;
}

//------------------------------------------------------------------------------
// Reset()
//------------------------------------------------------------------------------
void GroundVehicle::reset()
{
   BaseClass::reset();

   if (isLocalPlayer()) {
      if (initLnchrPos == UP) {
         cmdLnchrPos = UP;
         lnchrAngle = lnchrUpAngle;
         lnchrRate = 0.0;
      }
      else if (initLnchrPos == DOWN) {
         cmdLnchrPos = DOWN;
         lnchrAngle = lnchrDownAngle;
         lnchrRate = 0.0;
      }
      else {
         cmdLnchrPos = NONE;
         lnchrAngle = 0.0;
         lnchrRate = 0.0;
      }
   }
}

//------------------------------------------------------------------------------
// dynamics() -- update vehicle dynamics
//------------------------------------------------------------------------------
void GroundVehicle::dynamics(const double dt)
{
   BaseClass::dynamics(dt);
   launcherDynamics(dt);
}

//------------------------------------------------------------------------------
// Launcher dynamics -- moves launcher to its commanded position
//------------------------------------------------------------------------------
void GroundVehicle::launcherDynamics(const double dt)
{
   if (lnchrMoveTime > 0 && cmdLnchrPos != NONE) {

      double rate = (lnchrUpAngle - lnchrDownAngle) / lnchrMoveTime;
      double angle = lnchrAngle;

      if (cmdLnchrPos == UP && lnchrAngle != lnchrUpAngle) {
         angle = lnchrAngle + (rate * dt);
         if (angle >= lnchrUpAngle) {
            angle = lnchrUpAngle;
            rate = 0.0;
         }
      }
      else if (cmdLnchrPos == DOWN && lnchrAngle != lnchrDownAngle) {
         angle = lnchrAngle - (rate * dt);
         if (angle <= lnchrDownAngle) {
            angle = lnchrDownAngle;
            rate = 0.0;
         }
      }

      lnchrAngle = angle;
      lnchrRate = rate;
   }
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------
double GroundVehicle::getGrossWeight() const
{
    return 0.0;
}

double GroundVehicle::getFuelWt() const
{
    return 0.0;
}

double GroundVehicle::getFuelWtMax() const
{
    return 0.0;
}

// Launcher position (rad)
double GroundVehicle::getLauncherPosition() const
{
   return lnchrAngle;
}

// Launcher rate (rad/sec)
double GroundVehicle::getLauncherRate() const
{
   return lnchrRate;
}

// Returns the launcher's command position
GroundVehicle::LauncherCommand GroundVehicle::getLauncherCommand() const
{
   return cmdLnchrPos;
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool GroundVehicle::commandLauncher(const LauncherCommand cmd)
{
   cmdLnchrPos = cmd;
   return true;
}

// Sets the launcher elevation angle (rad), and removes the old position command
bool GroundVehicle::setLauncherPosition(const double rad)
{
   lnchrAngle = rad;
   lnchrRate = 0.0;
   cmdLnchrPos = NONE;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// commandedPosition: Launcher's init commanded position [ "up" "down" ] (base::Identifier)
bool GroundVehicle::setSlotCommandedPosition(const base::Identifier* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      if (*msg == "up" || *msg == "UP") {
         initLnchrPos = UP;
         cmdLnchrPos = UP;
         ok = true;
      }
      else if (*msg == "down" || *msg == "DOWN") {
         initLnchrPos = DOWN;
         cmdLnchrPos = DOWN;
         ok = true;
      }
   }
   return ok;
}

// launcherDownAngle: Min (down) Launcher angle (base::Angle)
bool GroundVehicle::setSlotLauncherDownAngle(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      lnchrDownAngle = static_cast<double>(base::Radians::convertStatic( *msg ));
      ok = true;
   }
   return ok;
}

// launcherUpAngle: Max (up) Launcher angle (base::Angle)
bool GroundVehicle::setSlotLauncherUpAngle(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      lnchrUpAngle = static_cast<double>(base::Radians::convertStatic( *msg ));
      ok = true;
   }
   return ok;
}

// launcherMoveTime: Max time to move between 'down' and 'up' positions (base::Time)
bool GroundVehicle::setSlotLauncherMoveTime(const base::Time* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      lnchrMoveTime = base::Seconds::convertStatic( *msg );
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* GroundVehicle::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}



//==============================================================================
// class Tank
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Tank,"Tank")
EMPTY_SERIALIZER(Tank)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Tank::Tank()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericTank");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Tank::copyData(const Tank& org, const bool)
{
    BaseClass::copyData(org);
}

void Tank::deleteData()
{
}


//==============================================================================
// class ArmoredVehicle
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(ArmoredVehicle,"ArmoredVehicle")
EMPTY_SERIALIZER(ArmoredVehicle)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
ArmoredVehicle::ArmoredVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericArmoredVehicle");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void ArmoredVehicle::copyData(const ArmoredVehicle& org, const bool)
{
    BaseClass::copyData(org);
}

void ArmoredVehicle::deleteData()
{
}

//==============================================================================
// class WheeledVehicle
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(WheeledVehicle,"WheeledVehicle")
EMPTY_SERIALIZER(WheeledVehicle)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
WheeledVehicle::WheeledVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericWheeledVehicle");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void WheeledVehicle::copyData(const WheeledVehicle& org, const bool)
{
    BaseClass::copyData(org);
}

void WheeledVehicle::deleteData()
{
}

//==============================================================================
// class Artillery
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Artillery,"Artillery")
EMPTY_SERIALIZER(Artillery)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Artillery::Artillery()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericArtillery");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Artillery::copyData(const Artillery& org, const bool)
{
    BaseClass::copyData(org);
}

void Artillery::deleteData()
{
}


//==============================================================================
// class GroundStation
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(GroundStation,"GroundStation")
EMPTY_SERIALIZER(GroundStation)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
GroundStation::GroundStation()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GroundStation");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void GroundStation::copyData(const GroundStation& org, const bool)
{
    BaseClass::copyData(org);
}

void GroundStation::deleteData()
{
}

//==============================================================================
// class GroundStationRadar
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(GroundStationRadar,"GroundStationRadar")
EMPTY_SERIALIZER(GroundStationRadar)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
GroundStationRadar::GroundStationRadar()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GroundStationRadar");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void GroundStationRadar::copyData(const GroundStationRadar& org, const bool)
{
    BaseClass::copyData(org);
}

void GroundStationRadar::deleteData()
{
}

//==============================================================================
// class GroundStationUav
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(GroundStationUav,"GroundStationUav")
EMPTY_SERIALIZER(GroundStationUav)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
GroundStationUav::GroundStationUav()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GroundStationUav");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void GroundStationUav::copyData(const GroundStationUav& org, const bool)
{
    BaseClass::copyData(org);
}

void GroundStationUav::deleteData()
{
}

} // End simulation namespace
} // End oe namespace
