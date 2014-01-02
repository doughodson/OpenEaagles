//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/simulation/Factory.h"

#include "openeaagles/basic/Object.h"

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
#include "openeaagles/simulation/MergingIrSensor.h"
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

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = 0;

    // Basic Simulations
    if ( strcmp(name, Simulation::getFactoryName()) == 0 ) {
        obj = new Simulation();
    }
    else if ( strcmp(name, Station::getFactoryName()) == 0 ) {
        obj = new Station();
    }

    // Basic Player types
    else if ( strcmp(name, Player::getFactoryName()) == 0 ) {
        obj = new Player();
    }
    else if ( strcmp(name, AirVehicle::getFactoryName()) == 0 ) {
        obj = new AirVehicle();
    }
    else if ( strcmp(name, Building::getFactoryName()) == 0 ) {
        obj = new Building();
    }
    else if ( strcmp(name, GroundVehicle::getFactoryName()) == 0 ) {
        obj = new GroundVehicle();
    }
    else if ( strcmp(name, LifeForm::getFactoryName()) == 0 ) {
        obj = new LifeForm();
    }
    else if ( strcmp(name, Ship::getFactoryName()) == 0 ) {
        obj = new Ship();
    }
    else if ( strcmp(name, SpaceVehicle::getFactoryName()) == 0 ) {
        obj = new SpaceVehicle();
    }

    // General Air Vehicles
    else if ( strcmp(name, Aircraft::getFactoryName()) == 0 ) {
        obj = new Aircraft();
    }
    else if ( strcmp(name, Helicopter::getFactoryName()) == 0 ) {
        obj = new Helicopter();
    }
    else if ( strcmp(name, UnmannedAirVehicle::getFactoryName()) == 0 ) {
        obj = new UnmannedAirVehicle();
    }

    // General Ground Vehicles
    else if ( strcmp(name, Tank::getFactoryName()) == 0 ) {
        obj = new Tank();
    }
    else if ( strcmp(name, ArmoredVehicle::getFactoryName()) == 0 ) {
        obj = new ArmoredVehicle();
    }
    else if ( strcmp(name, WheeledVehicle::getFactoryName()) == 0 ) {
        obj = new WheeledVehicle();
    }
    else if ( strcmp(name, Artillery::getFactoryName()) == 0 ) {
        obj = new Artillery();
    }
    else if ( strcmp(name, SamVehicle::getFactoryName()) == 0 ) {
        obj = new SamVehicle();
    }
    else if ( strcmp(name, GroundStation::getFactoryName()) == 0 ) {
        obj = new GroundStation();
    }
    else if ( strcmp(name, GroundStationRadar::getFactoryName()) == 0 ) {
        obj = new GroundStationRadar();
    }
    else if ( strcmp(name, GroundStationUav::getFactoryName()) == 0 ) {
        obj = new GroundStationUav();
    }

    // General Space Vehicles
    else if ( strcmp(name, MannedSpaceVehicle::getFactoryName()) == 0 ) {
        obj = new MannedSpaceVehicle();
    }
    else if ( strcmp(name, UnmannedSpaceVehicle::getFactoryName()) == 0 ) {
        obj = new UnmannedSpaceVehicle();
    }
    else if ( strcmp(name, BoosterSpaceVehicle::getFactoryName()) == 0 ) {
        obj = new BoosterSpaceVehicle();
    }

    // System
    else if ( strcmp(name, System::getFactoryName()) == 0 ) {
        obj = new System();
    }
    else if ( strcmp(name, AvionicsPod::getFactoryName()) == 0 ) {
        obj = new AvionicsPod();
    }

    // Basic Pilot types
    else if ( strcmp(name, Pilot::getFactoryName()) == 0 ) {
        obj = new Pilot();
    }
    else if ( strcmp(name, Autopilot::getFactoryName()) == 0 ) {
        obj = new Autopilot();
    }

    // Navigation types
    else if ( strcmp(name, Navigation::getFactoryName()) == 0 ) {
        obj = new Navigation();
    }
    else if ( strcmp(name, Ins::getFactoryName()) == 0 ) {
        obj = new Ins();
    }
    else if ( strcmp(name, Gps::getFactoryName()) == 0 ) {
        obj = new Gps();
    }
    else if ( strcmp(name, Route::getFactoryName()) == 0 ) {
        obj = new Route();
    }
    else if ( strcmp(name, Steerpoint::getFactoryName()) == 0 ) {
        obj = new Steerpoint();
    }
    
    // Target Data
    else if (strcmp(name, TargetData::getFactoryName()) == 0 ) {
        obj = new TargetData();
    }

    // Bullseyes
    else if (strcmp(name, Bullseye::getFactoryName()) == 0 ) {
        obj = new Bullseye();
    }

    // Actions
    else if ( strcmp(name, ActionImagingSar::getFactoryName()) == 0 ) {
        obj = new ActionImagingSar();
    }
    else if ( strcmp(name, ActionWeaponRelease::getFactoryName()) == 0 ) {
        obj = new ActionWeaponRelease();
    }
    else if ( strcmp(name, ActionDecoyRelease::getFactoryName()) == 0 ) {
        obj = new ActionDecoyRelease();
    }
    else if ( strcmp(name, ActionCamouflageType::getFactoryName()) == 0 ) {
        obj = new ActionCamouflageType();
    }

    // Bombs and Missiles
    else if ( strcmp(name, Bomb::getFactoryName()) == 0 ) {
        obj = new Bomb();
    }
    else if ( strcmp(name, Missile::getFactoryName()) == 0 ) {
        obj = new Missile();
    }
    else if ( strcmp(name, Aam::getFactoryName()) == 0 ) {
        obj = new Aam();
    }
    else if ( strcmp(name, Agm::getFactoryName()) == 0 ) {
        obj = new Agm();
    }
    else if ( strcmp(name, Sam::getFactoryName()) == 0 ) {
        obj = new Sam();
    }

    // Effects
    else if ( strcmp(name, Chaff::getFactoryName()) == 0 ) {
        obj = new Chaff();
    }
    else if ( strcmp(name, Decoy::getFactoryName()) == 0 ) {
        obj = new Decoy();
    }
    else if ( strcmp(name, Flare::getFactoryName()) == 0 ) {
        obj = new Flare();
    }

    // Stores, stores manager and external stores (FuelTank, Gun & Bullets (used by the Gun))
    else if ( strcmp(name, Stores::getFactoryName()) == 0 ) {
        obj = new Stores();
    }
    else if ( strcmp(name, SimpleStoresMgr::getFactoryName()) == 0 ) {
        obj = new SimpleStoresMgr();
    }
    else if ( strcmp(name, FuelTank::getFactoryName()) == 0 ) {
        obj = new FuelTank();
    }
    else if ( strcmp(name, Gun::getFactoryName()) == 0 ) {
        obj = new Gun();
    }
    else if ( strcmp(name, Bullet::getFactoryName()) == 0 ) {
        obj = new Bullet();
    }

    // Data links
    else if ( strcmp(name, Datalink::getFactoryName()) == 0 ) {
        obj = new Datalink();
    }

    // Gimbals, Antennas and Optics
    else if ( strcmp(name, Gimbal::getFactoryName()) == 0 ) {
        obj = new Gimbal();
    }
    else if ( strcmp(name, ScanGimbal::getFactoryName()) == 0 ) {
        obj = new ScanGimbal();
    }
    else if ( strcmp(name, StabilizingGimbal::getFactoryName()) == 0 ) {
        obj = new StabilizingGimbal();
    }
    else if ( strcmp(name, Antenna::getFactoryName()) == 0 ) {
        obj = new Antenna();
    }
    else if ( strcmp(name, IrSeeker::getFactoryName()) == 0 ) {
        obj = new IrSeeker();
    }

    // IR Atmospheres
    else if ( strcmp(name, IrAtmosphere::getFactoryName()) == 0 ) {
        obj = new IrAtmosphere;
    }
    else if ( strcmp(name, IrAtmosphere1::getFactoryName()) == 0 ) {
        obj = new IrAtmosphere1;
    }

    // R/F Signatures
    else if ( strcmp(name, SigConstant::getFactoryName()) == 0 ) {
        obj = new SigConstant();
    }
    else if ( strcmp(name, SigSphere::getFactoryName()) == 0 ) {
        obj = new SigSphere();
    }
    else if ( strcmp(name, SigPlate::getFactoryName()) == 0 ) {
        obj = new SigPlate();
    }
    else if ( strcmp(name, SigDihedralCR::getFactoryName()) == 0 ) {
        obj = new SigDihedralCR();
    }
    else if ( strcmp(name, SigTrihedralCR::getFactoryName()) == 0 ) {
        obj = new SigTrihedralCR();
    }
    else if ( strcmp(name, SigSwitch::getFactoryName()) == 0 ) {
        obj = new SigSwitch();
    }
    else if ( strcmp(name, SigAzEl::getFactoryName()) == 0 ) {
        obj = new SigAzEl();
    }
    // IR Signatures
    else if ( strcmp(name, IrSignature::getFactoryName()) == 0 ) {
        obj = new IrSignature();
    }
    else if ( strcmp(name, AircraftIrSignature::getFactoryName()) == 0 ) {
        obj = new AircraftIrSignature;
    }
    else if ( strcmp(name, IrShape::getFactoryName()) == 0 ) {
        obj = new IrShape();
    }
    else if ( strcmp(name, IrSphere::getFactoryName()) == 0 ) {
        obj = new IrSphere();
    }
    else if ( strcmp(name, IrBox::getFactoryName()) == 0 ) {
        obj = new IrBox();
    }

    // Onboard Computers
    else if ( strcmp(name, OnboardComputer::getFactoryName()) == 0 ) {
        obj = new OnboardComputer();
    }

    // Radios
    else if ( strcmp(name, Radio::getFactoryName()) == 0 ) {
        obj = new Radio();
    }
    else if ( strcmp(name, CommRadio::getFactoryName()) == 0 ) {
        obj = new CommRadio();
    }
    else if ( strcmp(name, NavRadio::getFactoryName()) == 0 ) {
        obj = new NavRadio();
    }
    else if ( strcmp(name, TacanRadio::getFactoryName()) == 0 ) {
        obj = new TacanRadio();
    }
    else if ( strcmp(name, IlsRadio::getFactoryName()) == 0 ) {
        obj = new IlsRadio();
    }
    else if ( strcmp(name, Iff::getFactoryName()) == 0 ) {
        obj = new Iff();
    }

    // Sensors
    else if ( strcmp(name, RfSensor::getFactoryName()) == 0 ) {
        obj = new RfSensor();
    }
    else if ( strcmp(name, SensorMgr::getFactoryName()) == 0 ) {
        obj = new SensorMgr();
    }
    else if ( strcmp(name, Radar::getFactoryName()) == 0 ) {
        obj = new Radar();
    }
    else if ( strcmp(name, Rwr::getFactoryName()) == 0 ) {
        obj = new Rwr();
    }
    else if ( strcmp(name, Sar::getFactoryName()) == 0 ) {
        obj = new Sar();
    }
    else if ( strcmp(name, Jammer::getFactoryName()) == 0 ) {
        obj = new Jammer();
    }
    else if ( strcmp(name, IrSensor::getFactoryName()) == 0 ) {
        obj = new IrSensor();
    }
    else if ( strcmp(name, MergingIrSensor::getFactoryName()) == 0 ) {
        obj = new MergingIrSensor();
    }

    // Tracks
    else if ( strcmp(name, Track::getFactoryName()) == 0 ) {
        obj = new Track();
    }

    // Track Managers
    else if ( strcmp(name, GmtiTrkMgr::getFactoryName()) == 0 ) {
        obj = new GmtiTrkMgr();
    }
    else if ( strcmp(name, AirTrkMgr::getFactoryName()) == 0 ) {
        obj = new AirTrkMgr();
    }
    else if ( strcmp(name, RwrTrkMgr::getFactoryName()) == 0 ) {
        obj = new RwrTrkMgr();
    }
    else if ( strcmp(name, AirAngleOnlyTrkMgr::getFactoryName()) == 0 ) {
        obj = new AirAngleOnlyTrkMgr();
    }

    // UBF Agents
    else if ( strcmp(name, SimAgent::getFactoryName()) == 0 ) {
        obj = new SimAgent();
    }
    else if ( strcmp(name, MultiActorAgent::getFactoryName()) == 0 ) {
        obj = new MultiActorAgent();
    }

    else if ( strcmp(name, TabLogger::getFactoryName()) == 0 ) {
        obj = new TabLogger();
    }
    else if ( strcmp(name, Otm::getFactoryName()) == 0 ) {
        obj = new Otm();
    }

    return obj;
}

}  // end namespace Simulation
}  // end namespace Eaagles
