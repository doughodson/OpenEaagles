//------------------------------------------------------------------------------
// Classes: GroundVehicle, Tank, ArmoredVehicle, WheeledVehicle, Artillery,
//          GroundStation, GroundStationRadar, GroundStationUav
//
// Description: Generic Ground Vehicles
//
//------------------------------------------------------------------------------
#if !defined(__GROUNDVEHICLE_H_12DE4387_0EC8_407c_A4B9_4974E8223847__)
#define __GROUNDVEHICLE_H_12DE4387_0EC8_407c_A4B9_4974E8223847__

#include "openeaagles/simulation/Player.h"

namespace Eaagles {
   namespace Basic {
      class Distance;
      class Identifier;
      class Time;
   }
namespace Simulation {

//==============================================================================
// Class GroundVehicle
//
// Form name: GroundVehicle
// Slots:
//    commandedPosition <Identifier>   ! Launcher's init commanded position [UP, DOWN, NONE] (default: NONE)
//    launcherDownAngle <Angle>        ! Min (down) Launcher angle (default: 0.0)
//    launcherUpAngle   <Angle>        ! Max (up) Launcher angle (default: PI/2.0)
//    launcherMoveTime  <Time>         ! Max time to move between 'down' and 'up' positions (default: 10.0f)
//
// Notes:
//   (1) The launcher move time should be greater than zero seconds, and the up
//       angle should be greater than the down angle.
//   (2) This launcher is a temp solution -- this will be removed.
//
// GUID: (12DE4387-0EC8-407c-A4B9-4974E8223847)
//==============================================================================
class GroundVehicle : public Player  
{
    DECLARE_SUBCLASS(GroundVehicle,Player)

public:
   enum LauncherCommand { NONE, DOWN, UP };

public:
    GroundVehicle();

    virtual LCreal getGrossWeight() const;              // wt: lbs
    virtual LCreal getFuelWt() const;                   // wt: lbs
    virtual LCreal getFuelWtMax() const;                // wt: lbs

    virtual LCreal getLauncherPosition() const;        // Launcher elevation angle (rad)
    virtual LCreal getLauncherRate() const;            // Launcher rate (rad/sec)
    virtual LauncherCommand getLauncherCommand() const; // Returns the launcher's command position (UP/DOWN) or NONE if not commanded


    // Commands the launcher to the up/down position
    virtual bool commandLauncher(const LauncherCommand cmd);

    // Sets the launcher elevation angle (rad), and removes the old position command
    virtual bool setLauncherPosition(const LCreal rad);

    // Slot functions
    virtual bool setSlotCommandedPosition(const Basic::Identifier* const msg);
    virtual bool setSlotLauncherDownAngle(const Basic::Angle* const msg);
    virtual bool setSlotLauncherUpAngle(const Basic::Angle* const msg);
    virtual bool setSlotLauncherMoveTime(const Basic::Time* const msg);

    // Player interface
    virtual unsigned int getMajorType() const;

    // Component Interface
    virtual void reset();

protected:
   // Launcher dynamics -- moves launcher to its commanded position
   virtual void launcherDynamics(const LCreal dt); 

   // Player class (protected) interface
   virtual void dynamics(const LCreal  dt = 0.0);

private:
   LCreal            lnchrAngle;       // Current launcher angle  (rad)
   LCreal            lnchrRate;        // Rate the launcher is moving (rad/sec)
   LCreal            lnchrDownAngle;   // Min launcher angle      (rad)
   LCreal            lnchrUpAngle;     // Max launcher angle      (rad)
   LCreal            lnchrMoveTime;    // Time to move between min & max angles (sec)
   LauncherCommand   initLnchrPos;     // Initial launcher position
   LauncherCommand   cmdLnchrPos;      // Commanded launcher position
};


//==============================================================================
// Class Tank
// Form name: Tank
// GUID: (1707667D-6228-4df6-A69F-3A1DD586E4F1)
//==============================================================================
class Tank : public GroundVehicle  
{
    DECLARE_SUBCLASS(Tank,GroundVehicle)

public:
    Tank();
};

//==============================================================================
// Class ArmoredVehicle
// Form name: ArmoredVehicle
// GUID: {351F40BF-1AB4-43c2-98E8-69ADB8C08C2D}
//==============================================================================
class ArmoredVehicle : public GroundVehicle  
{
    DECLARE_SUBCLASS(ArmoredVehicle,GroundVehicle)

public:
    ArmoredVehicle();
};

//==============================================================================
// Class WheeledVehicle
// Form name: WheeledVehicle
// GUID: {1A69ADA9-7ADA-4002-9E46-3009B45D41EC}
//==============================================================================
class WheeledVehicle : public GroundVehicle  
{
    DECLARE_SUBCLASS(WheeledVehicle,GroundVehicle)

public:
    WheeledVehicle();
};

//==============================================================================
// Class Artillery
// Form name: Artillery
// GUID: {E8A3B240-1910-4bdf-8E6C-7C4C75012FAA}
//==============================================================================
class Artillery : public GroundVehicle  
{
    DECLARE_SUBCLASS(Artillery,GroundVehicle)

public:
    Artillery();
};


//==============================================================================
// Class GroundStation
// Form name: GroundStation
// GUID: {0B6D5395-D834-4251-A4B3-B14FE59D267C}
//==============================================================================
class GroundStation : public GroundVehicle  
{
    DECLARE_SUBCLASS(GroundStation,GroundVehicle)

public:
    GroundStation();
};

//==============================================================================
// Class GroundStationRadar
// Form name: GroundStationRadar
// GUID: {07F53C9F-E017-46a3-9DF2-6D02F100967C}
//==============================================================================
class GroundStationRadar : public GroundStation  
{
    DECLARE_SUBCLASS(GroundStationRadar,GroundStation)

public:
    GroundStationRadar();
};

//==============================================================================
// Class GroundStationUav
// Form name: GroundStationUav
// GUID: {6EC343F0-304C-4e9e-BF4F-C48B90719826}
//==============================================================================
class GroundStationUav : public GroundStation  
{
    DECLARE_SUBCLASS(GroundStationUav,GroundStation)

public:
    GroundStationUav();
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // !defined(__GROUNDVEHICLE_H_12DE4387_0EC8_407c_A4B9_4974E8223847__)
