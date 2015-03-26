//------------------------------------------------------------------------------
// Classes: GroundVehicle, Tank, ArmoredVehicle, WheeledVehicle, Artillery,
//          GroundStation, GroundStationRadar, GroundStationUav
//
// Description: Generic Ground Vehicles
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_GroundVehicle_H__
#define __Eaagles_Simulation_GroundVehicle_H__

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
// Factory name: GroundVehicle
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
    unsigned int getMajorType() const override;

    // Component Interface
    void reset() override;

protected:
   // Launcher dynamics -- moves launcher to its commanded position
   virtual void launcherDynamics(const LCreal dt); 

   // Player class (protected) interface
   void dynamics(const LCreal  dt = 0.0) override;

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
// Factory name: Tank
//==============================================================================
class Tank : public GroundVehicle  
{
    DECLARE_SUBCLASS(Tank,GroundVehicle)

public:
    Tank();
};

//==============================================================================
// Class ArmoredVehicle
// Factory name: ArmoredVehicle
//==============================================================================
class ArmoredVehicle : public GroundVehicle  
{
    DECLARE_SUBCLASS(ArmoredVehicle,GroundVehicle)

public:
    ArmoredVehicle();
};

//==============================================================================
// Class WheeledVehicle
// Factory name: WheeledVehicle
//==============================================================================
class WheeledVehicle : public GroundVehicle  
{
    DECLARE_SUBCLASS(WheeledVehicle,GroundVehicle)

public:
    WheeledVehicle();
};

//==============================================================================
// Class Artillery
// Factory name: Artillery
//==============================================================================
class Artillery : public GroundVehicle  
{
    DECLARE_SUBCLASS(Artillery,GroundVehicle)

public:
    Artillery();
};


//==============================================================================
// Class GroundStation
// Factory name: GroundStation
//==============================================================================
class GroundStation : public GroundVehicle  
{
    DECLARE_SUBCLASS(GroundStation,GroundVehicle)

public:
    GroundStation();
};

//==============================================================================
// Class GroundStationRadar
// Factory name: GroundStationRadar
//==============================================================================
class GroundStationRadar : public GroundStation  
{
    DECLARE_SUBCLASS(GroundStationRadar,GroundStation)

public:
    GroundStationRadar();
};

//==============================================================================
// Class GroundStationUav
// Factory name: GroundStationUav
//==============================================================================
class GroundStationUav : public GroundStation  
{
    DECLARE_SUBCLASS(GroundStationUav,GroundStation)

public:
    GroundStationUav();
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
