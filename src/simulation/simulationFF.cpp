//------------------------------------------------------------------------------
// Form function for the 'simulation' library
//------------------------------------------------------------------------------

#include "openeaagles/simulation/simulationFF.h"

#include "openeaagles/simulation/Aam.h"
#include "openeaagles/simulation/Actions.h"
#include "openeaagles/simulation/Agm.h"
#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/AngleOnlyTrackManager.h"
#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/Autopilot.h"
#include "openeaagles/simulation/AvionicsPod.h"
#include "openeaagles/simulation/Bomb.h"
#include "openeaagles/simulation/Buildings.h"
#include "openeaagles/simulation/Bullseye.h"
#include "openeaagles/simulation/Datalink.h"
#include "openeaagles/simulation/Effects.h"
#include "openeaagles/simulation/ExternalStore.h"
#include "openeaagles/simulation/FuelTank.h"
#include "openeaagles/simulation/Gimbal.h"
#include "openeaagles/simulation/GroundVehicle.h"
#include "openeaagles/simulation/Gps.h"
#include "openeaagles/simulation/Guns.h"
#include "openeaagles/simulation/Iff.h"
#include "openeaagles/simulation/Ins.h"
#include "openeaagles/simulation/IrAtmosphere.h"
#include "openeaagles/simulation/IrAtmosphere1.h"
#include "openeaagles/simulation/IrSeeker.h"
#include "openeaagles/simulation/IrSensor.h"
#include "openeaagles/simulation/IrShapes.h"
#include "openeaagles/simulation/IrSignature.h"
#include "openeaagles/simulation/AircraftIrSignature.h"
#include "openeaagles/simulation/Jammer.h"
#include "openeaagles/simulation/LifeForms.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Navigation.h"
#include "openeaagles/simulation/NavRadios.h"
#include "openeaagles/simulation/OnboardComputer.h"
#include "openeaagles/simulation/Otw.h"
#include "openeaagles/simulation/Pilot.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Radar.h"
#include "openeaagles/simulation/Radio.h"
#include "openeaagles/simulation/RfSensor.h"
#include "openeaagles/simulation/Route.h"
#include "openeaagles/simulation/Rwr.h"
#include "openeaagles/simulation/Sam.h"
#include "openeaagles/simulation/SamVehicles.h"
#include "openeaagles/simulation/Sar.h"
#include "openeaagles/simulation/ScanGimbal.h"
#include "openeaagles/simulation/Ships.h"
#include "openeaagles/simulation/Signatures.h"

#include "openeaagles/simulation/SimAgent.h"
#include "openeaagles/simulation/MultiActorAgent.h"

#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/SpaceDynamicsModel.h"
#include "openeaagles/simulation/SpaceVehicle.h"
#include "openeaagles/simulation/StabilizingGimbal.h"
#include "openeaagles/simulation/Station.h"
#include "openeaagles/simulation/Steerpoint.h"
#include "openeaagles/simulation/Stores.h"
#include "openeaagles/simulation/StoresMgr.h"
#include "openeaagles/simulation/System.h"
#include "openeaagles/simulation/TabLogger.h"
#include "openeaagles/simulation/TargetData.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/TrackManager.h"
#include "openeaagles/simulation/Weapon.h"

namespace Eaagles {
namespace Simulation {

Basic::Object* simulationFormFunc(const char* formname)
{
    Basic::Object* newform = 0;

    // Basic Simulations
    if ( strcmp(formname, Simulation::getFormName()) == 0 ) {
        newform = new Simulation();
    }
    else if ( strcmp(formname, Station::getFormName()) == 0 ) {
        newform = new Station();
    }

    // Basic Player types
    else if ( strcmp(formname, Player::getFormName()) == 0 ) {
        newform = new Player();
    }
    else if ( strcmp(formname, AirVehicle::getFormName()) == 0 ) {
        newform = new AirVehicle();
    }
    else if ( strcmp(formname, Building::getFormName()) == 0 ) {
        newform = new Building();
    }
    else if ( strcmp(formname, GroundVehicle::getFormName()) == 0 ) {
        newform = new GroundVehicle();
    }
    else if ( strcmp(formname, LifeForm::getFormName()) == 0 ) {
        newform = new LifeForm();
    }
    else if ( strcmp(formname, Ship::getFormName()) == 0 ) {
        newform = new Ship();
    }
    else if ( strcmp(formname, SpaceVehicle::getFormName()) == 0 ) {
        newform = new SpaceVehicle();
    }

    // General Air Vehicles
    else if ( strcmp(formname, Aircraft::getFormName()) == 0 ) {
        newform = new Aircraft();
    }
    else if ( strcmp(formname, Helicopter::getFormName()) == 0 ) {
        newform = new Helicopter();
    }
    else if ( strcmp(formname, UnmannedAirVehicle::getFormName()) == 0 ) {
        newform = new UnmannedAirVehicle();
    }

    // General Ground Vehicles
    else if ( strcmp(formname, Tank::getFormName()) == 0 ) {
        newform = new Tank();
    }
    else if ( strcmp(formname, ArmoredVehicle::getFormName()) == 0 ) {
        newform = new ArmoredVehicle();
    }
    else if ( strcmp(formname, WheeledVehicle::getFormName()) == 0 ) {
        newform = new WheeledVehicle();
    }
    else if ( strcmp(formname, Artillery::getFormName()) == 0 ) {
        newform = new Artillery();
    }
    else if ( strcmp(formname, SamVehicle::getFormName()) == 0 ) {
        newform = new SamVehicle();
    }
    else if ( strcmp(formname, GroundStation::getFormName()) == 0 ) {
        newform = new GroundStation();
    }
    else if ( strcmp(formname, GroundStationRadar::getFormName()) == 0 ) {
        newform = new GroundStationRadar();
    }
    else if ( strcmp(formname, GroundStationUav::getFormName()) == 0 ) {
        newform = new GroundStationUav();
    }

    // General Space Vehicles
    else if ( strcmp(formname, MannedSpaceVehicle::getFormName()) == 0 ) {
        newform = new MannedSpaceVehicle();
    }
    else if ( strcmp(formname, UnmannedSpaceVehicle::getFormName()) == 0 ) {
        newform = new UnmannedSpaceVehicle();
    }
    else if ( strcmp(formname, BoosterSpaceVehicle::getFormName()) == 0 ) {
        newform = new BoosterSpaceVehicle();
    }

    // System
    else if ( strcmp(formname, System::getFormName()) == 0 ) {
        newform = new System();
    }
   else if ( strcmp(formname, AvionicsPod::getFormName()) == 0 ) {
      newform = new AvionicsPod();
   }

    // Basic Pilot types
   else if ( strcmp(formname, Pilot::getFormName()) == 0 ) {
      newform = new Pilot();
   }
    else if ( strcmp(formname, Autopilot::getFormName()) == 0 ) {
        newform = new Autopilot();
    }

    // Navigation types
    else if ( strcmp(formname, Navigation::getFormName()) == 0 ) {
        newform = new Navigation();
    }
    else if ( strcmp(formname, Ins::getFormName()) == 0 ) {
        newform = new Ins();
    }
    else if ( strcmp(formname, Gps::getFormName()) == 0 ) {
        newform = new Gps();
    }
    else if ( strcmp(formname, Route::getFormName()) == 0 ) {
        newform = new Route();
    }
    else if ( strcmp(formname, Steerpoint::getFormName()) == 0 ) {
        newform = new Steerpoint();
    }
    
    // Target Data
    else if (strcmp(formname, TargetData::getFormName()) == 0 ) {
        newform = new TargetData();
    }

    // Bullseyes
    else if (strcmp(formname, Bullseye::getFormName()) == 0 ) {
        newform = new Bullseye();
    }

    // Actions
    else if ( strcmp(formname, ActionImagingSar::getFormName()) == 0 ) {
      newform = new ActionImagingSar();
    }
    else if ( strcmp(formname, ActionWeaponRelease::getFormName()) == 0 ) {
        newform = new ActionWeaponRelease();
    }
    else if ( strcmp(formname, ActionDecoyRelease::getFormName()) == 0 ) {
        newform = new ActionDecoyRelease();
    }
    else if ( strcmp(formname, ActionCamouflageType::getFormName()) == 0 ) {
        newform = new ActionCamouflageType();
    }

   // Bombs and Missiles
   else if ( strcmp(formname, Bomb::getFormName()) == 0 ) {
      newform = new Bomb();
   }
    else if ( strcmp(formname, Missile::getFormName()) == 0 ) {
        newform = new Missile();
    }
    else if ( strcmp(formname, Aam::getFormName()) == 0 ) {
        newform = new Aam();
    }
    else if ( strcmp(formname, Agm::getFormName()) == 0 ) {
        newform = new Agm();
    }
    else if ( strcmp(formname, Sam::getFormName()) == 0 ) {
        newform = new Sam();
    }

    // Effects
    else if ( strcmp(formname, Chaff::getFormName()) == 0 ) {
        newform = new Chaff();
    }
    else if ( strcmp(formname, Decoy::getFormName()) == 0 ) {
        newform = new Decoy();
    }
    else if ( strcmp(formname, Flare::getFormName()) == 0 ) {
        newform = new Flare();
    }

   // Stores, stores manager and external stores (FuelTank, Gun & Bullets (used by the Gun))
   else if ( strcmp(formname, Stores::getFormName()) == 0 ) {
      newform = new Stores();
    }
   else if ( strcmp(formname, SimpleStoresMgr::getFormName()) == 0 ) {
      newform = new SimpleStoresMgr();
    }
   else if ( strcmp(formname, FuelTank::getFormName()) == 0 ) {
      newform = new FuelTank();
    }
   else if ( strcmp(formname, Gun::getFormName()) == 0 ) {
      newform = new Gun();
    }
   else if ( strcmp(formname, Bullet::getFormName()) == 0 ) {
      newform = new Bullet();
    }

    // Data links
    else if ( strcmp(formname, Datalink::getFormName()) == 0 ) {
        newform = new Datalink();
    }

    // Gimbals, Antennas and Optics
    else if ( strcmp(formname, Gimbal::getFormName()) == 0 ) {
        newform = new Gimbal();
    }
    else if ( strcmp(formname, ScanGimbal::getFormName()) == 0 ) {
        newform = new ScanGimbal();
    }
    else if ( strcmp(formname, StabilizingGimbal::getFormName()) == 0 ) {
        newform = new StabilizingGimbal();
    }
    else if ( strcmp(formname, Antenna::getFormName()) == 0 ) {
        newform = new Antenna();
    }
    else if ( strcmp(formname, IrSeeker::getFormName()) == 0 ) {
      newform = new IrSeeker();
    }

    // IR Atmospheres
    else if ( strcmp(formname, IrAtmosphere::getFormName()) == 0 ) {
      newform = new IrAtmosphere;
    }
    else if ( strcmp(formname, IrAtmosphere1::getFormName()) == 0 ) {
      newform = new IrAtmosphere1;
    }

    // R/F Signatures
    else if ( strcmp(formname, SigConstant::getFormName()) == 0 ) {
        newform = new SigConstant();
    }
    else if ( strcmp(formname, SigSphere::getFormName()) == 0 ) {
        newform = new SigSphere();
    }
    else if ( strcmp(formname, SigPlate::getFormName()) == 0 ) {
        newform = new SigPlate();
    }
    else if ( strcmp(formname, SigDihedralCR::getFormName()) == 0 ) {
        newform = new SigDihedralCR();
    }
    else if ( strcmp(formname, SigTrihedralCR::getFormName()) == 0 ) {
        newform = new SigTrihedralCR();
    }
    else if ( strcmp(formname, SigSwitch::getFormName()) == 0 ) {
        newform = new SigSwitch();
    }
    else if ( strcmp(formname, SigAzEl::getFormName()) == 0 ) {
        newform = new SigAzEl();
    }
    // IR Signatures
    else if ( strcmp(formname, IrSignature::getFormName()) == 0 ) {
        newform = new IrSignature();
    }
    else if ( strcmp(formname, AircraftIrSignature::getFormName()) == 0 ) {
        newform = new AircraftIrSignature;
    }
    else if ( strcmp(formname, IrShape::getFormName()) == 0 ) {
        newform = new IrShape();
    }
    else if ( strcmp(formname, IrSphere::getFormName()) == 0 ) {
        newform = new IrSphere();
    }
    else if ( strcmp(formname, IrBox::getFormName()) == 0 ) {
        newform = new IrBox();
    }

    // Onboard Computers
    else if ( strcmp(formname, OnboardComputer::getFormName()) == 0 ) {
        newform = new OnboardComputer();
    }

    // Radios
    else if ( strcmp(formname, Radio::getFormName()) == 0 ) {
        newform = new Radio();
    }
    else if ( strcmp(formname, CommRadio::getFormName()) == 0 ) {
        newform = new CommRadio();
    }
    else if ( strcmp(formname, NavRadio::getFormName()) == 0 ) {
        newform = new NavRadio();
    }
    else if ( strcmp(formname, TacanRadio::getFormName()) == 0 ) {
        newform = new TacanRadio();
    }
    else if ( strcmp(formname, IlsRadio::getFormName()) == 0 ) {
        newform = new IlsRadio();
    }
    else if ( strcmp(formname, Iff::getFormName()) == 0 ) {
        newform = new Iff();
    }

    // Sensors
    else if ( strcmp(formname, RfSensor::getFormName()) == 0 ) {
        newform = new RfSensor();
    }
    else if ( strcmp(formname, SensorMgr::getFormName()) == 0 ) {
        newform = new SensorMgr();
    }
    else if ( strcmp(formname, Radar::getFormName()) == 0 ) {
        newform = new Radar();
    }
    else if ( strcmp(formname, Rwr::getFormName()) == 0 ) {
        newform = new Rwr();
    }
    else if ( strcmp(formname, Sar::getFormName()) == 0 ) {
        newform = new Sar();
    }
    else if ( strcmp(formname, Jammer::getFormName()) == 0 ) {
        newform = new Jammer();
    }
   else if ( strcmp(formname, IrSensor::getFormName()) == 0 ) {
      newform = new IrSensor();
    }
   else if ( strcmp(formname, MergingIrSensor::getFormName()) == 0 ) {
      newform = new MergingIrSensor();
    }

    // Tracks
    else if ( strcmp(formname, Track::getFormName()) == 0 ) {
        newform = new Track();
    }

    // Track Managers
    else if ( strcmp(formname, GmtiTrkMgr::getFormName()) == 0 ) {
        newform = new GmtiTrkMgr();
    }
    else if ( strcmp(formname, AirTrkMgr::getFormName()) == 0 ) {
        newform = new AirTrkMgr();
    }
    else if ( strcmp(formname, RwrTrkMgr::getFormName()) == 0 ) {
        newform = new RwrTrkMgr();
    }
   else if ( strcmp(formname, AirAngleOnlyTrkMgr::getFormName()) == 0 ) {
      newform = new AirAngleOnlyTrkMgr();
   }

    // UBF Agents
    else if ( strcmp(formname, SimAgent::getFormName()) == 0 ) {
        newform = new SimAgent();
    }
    else if ( strcmp(formname, MultiActorAgent::getFormName()) == 0 ) {
        newform = new MultiActorAgent();
    }


   else if ( strcmp(formname, TabLogger::getFormName()) == 0 ) {
      newform = new TabLogger();
   }
   else if ( strcmp(formname, Otm::getFormName()) == 0 ) {
      newform = new Otm();
   }

    return newform;
}

} // End Simulation namespace
} // End Eaagles namespace

