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
#include "openeaagles/simulation/CollisionDetect.h"
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

#include <cstring>

namespace Eaagles {
namespace Simulation {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = nullptr;

    // Basic Simulations
    if ( std::strcmp(name, Simulation::getFactoryName()) == 0 ) {
        obj = new Simulation();
    }
    else if ( std::strcmp(name, Station::getFactoryName()) == 0 ) {
        obj = new Station();
    }

    // Basic Player types
    else if ( std::strcmp(name, Player::getFactoryName()) == 0 ) {
        obj = new Player();
    }
    else if ( std::strcmp(name, AirVehicle::getFactoryName()) == 0 ) {
        obj = new AirVehicle();
    }
    else if ( std::strcmp(name, Building::getFactoryName()) == 0 ) {
        obj = new Building();
    }
    else if ( std::strcmp(name, GroundVehicle::getFactoryName()) == 0 ) {
        obj = new GroundVehicle();
    }
    else if ( std::strcmp(name, LifeForm::getFactoryName()) == 0 ) {
        obj = new LifeForm();
    }
    else if ( std::strcmp(name, Ship::getFactoryName()) == 0 ) {
        obj = new Ship();
    }
    else if ( std::strcmp(name, SpaceVehicle::getFactoryName()) == 0 ) {
        obj = new SpaceVehicle();
    }

    // General Air Vehicles
    else if ( std::strcmp(name, Aircraft::getFactoryName()) == 0 ) {
        obj = new Aircraft();
    }
    else if ( std::strcmp(name, Helicopter::getFactoryName()) == 0 ) {
        obj = new Helicopter();
    }
    else if ( std::strcmp(name, UnmannedAirVehicle::getFactoryName()) == 0 ) {
        obj = new UnmannedAirVehicle();
    }

    // General Ground Vehicles
    else if ( std::strcmp(name, Tank::getFactoryName()) == 0 ) {
        obj = new Tank();
    }
    else if ( std::strcmp(name, ArmoredVehicle::getFactoryName()) == 0 ) {
        obj = new ArmoredVehicle();
    }
    else if ( std::strcmp(name, WheeledVehicle::getFactoryName()) == 0 ) {
        obj = new WheeledVehicle();
    }
    else if ( std::strcmp(name, Artillery::getFactoryName()) == 0 ) {
        obj = new Artillery();
    }
    else if ( std::strcmp(name, SamVehicle::getFactoryName()) == 0 ) {
        obj = new SamVehicle();
    }
    else if ( std::strcmp(name, GroundStation::getFactoryName()) == 0 ) {
        obj = new GroundStation();
    }
    else if ( std::strcmp(name, GroundStationRadar::getFactoryName()) == 0 ) {
        obj = new GroundStationRadar();
    }
    else if ( std::strcmp(name, GroundStationUav::getFactoryName()) == 0 ) {
        obj = new GroundStationUav();
    }

    // General Space Vehicles
    else if ( std::strcmp(name, MannedSpaceVehicle::getFactoryName()) == 0 ) {
        obj = new MannedSpaceVehicle();
    }
    else if ( std::strcmp(name, UnmannedSpaceVehicle::getFactoryName()) == 0 ) {
        obj = new UnmannedSpaceVehicle();
    }
    else if ( std::strcmp(name, BoosterSpaceVehicle::getFactoryName()) == 0 ) {
        obj = new BoosterSpaceVehicle();
    }

    // System
    else if ( std::strcmp(name, System::getFactoryName()) == 0 ) {
        obj = new System();
    }
    else if ( std::strcmp(name, AvionicsPod::getFactoryName()) == 0 ) {
        obj = new AvionicsPod();
    }

    // Basic Pilot types
    else if ( std::strcmp(name, Pilot::getFactoryName()) == 0 ) {
        obj = new Pilot();
    }
    else if ( std::strcmp(name, Autopilot::getFactoryName()) == 0 ) {
        obj = new Autopilot();
    }

    // Navigation types
    else if ( std::strcmp(name, Navigation::getFactoryName()) == 0 ) {
        obj = new Navigation();
    }
    else if ( std::strcmp(name, Ins::getFactoryName()) == 0 ) {
        obj = new Ins();
    }
    else if ( std::strcmp(name, Gps::getFactoryName()) == 0 ) {
        obj = new Gps();
    }
    else if ( std::strcmp(name, Route::getFactoryName()) == 0 ) {
        obj = new Route();
    }
    else if ( std::strcmp(name, Steerpoint::getFactoryName()) == 0 ) {
        obj = new Steerpoint();
    }

    // Target Data
    else if ( std::strcmp(name, TargetData::getFactoryName()) == 0 ) {
        obj = new TargetData();
    }

    // Bullseye
    else if ( std::strcmp(name, Bullseye::getFactoryName()) == 0 ) {
        obj = new Bullseye();
    }

    // Actions
    else if ( std::strcmp(name, ActionImagingSar::getFactoryName()) == 0 ) {
        obj = new ActionImagingSar();
    }
    else if ( std::strcmp(name, ActionWeaponRelease::getFactoryName()) == 0 ) {
        obj = new ActionWeaponRelease();
    }
    else if ( std::strcmp(name, ActionDecoyRelease::getFactoryName()) == 0 ) {
        obj = new ActionDecoyRelease();
    }
    else if ( std::strcmp(name, ActionCamouflageType::getFactoryName()) == 0 ) {
        obj = new ActionCamouflageType();
    }

    // Bombs and Missiles
    else if ( std::strcmp(name, Bomb::getFactoryName()) == 0 ) {
        obj = new Bomb();
    }
    else if ( std::strcmp(name, Missile::getFactoryName()) == 0 ) {
        obj = new Missile();
    }
    else if ( std::strcmp(name, Aam::getFactoryName()) == 0 ) {
        obj = new Aam();
    }
    else if ( std::strcmp(name, Agm::getFactoryName()) == 0 ) {
        obj = new Agm();
    }
    else if ( std::strcmp(name, Sam::getFactoryName()) == 0 ) {
        obj = new Sam();
    }

    // Effects
    else if ( std::strcmp(name, Chaff::getFactoryName()) == 0 ) {
        obj = new Chaff();
    }
    else if ( std::strcmp(name, Decoy::getFactoryName()) == 0 ) {
        obj = new Decoy();
    }
    else if ( std::strcmp(name, Flare::getFactoryName()) == 0 ) {
        obj = new Flare();
    }

    // Stores, stores manager and external stores (FuelTank, Gun & Bullets (used by the Gun))
    else if ( std::strcmp(name, Stores::getFactoryName()) == 0 ) {
        obj = new Stores();
    }
    else if ( std::strcmp(name, SimpleStoresMgr::getFactoryName()) == 0 ) {
        obj = new SimpleStoresMgr();
    }
    else if ( std::strcmp(name, FuelTank::getFactoryName()) == 0 ) {
        obj = new FuelTank();
    }
    else if ( std::strcmp(name, Gun::getFactoryName()) == 0 ) {
        obj = new Gun();
    }
    else if ( std::strcmp(name, Bullet::getFactoryName()) == 0 ) {
        obj = new Bullet();
    }

    // Data links
    else if ( std::strcmp(name, Datalink::getFactoryName()) == 0 ) {
        obj = new Datalink();
    }

    // Gimbals, Antennas and Optics
    else if ( std::strcmp(name, Gimbal::getFactoryName()) == 0 ) {
        obj = new Gimbal();
    }
    else if ( std::strcmp(name, ScanGimbal::getFactoryName()) == 0 ) {
        obj = new ScanGimbal();
    }
    else if ( std::strcmp(name, StabilizingGimbal::getFactoryName()) == 0 ) {
        obj = new StabilizingGimbal();
    }
    else if ( std::strcmp(name, Antenna::getFactoryName()) == 0 ) {
        obj = new Antenna();
    }
    else if ( std::strcmp(name, IrSeeker::getFactoryName()) == 0 ) {
        obj = new IrSeeker();
    }

    // IR Atmospheres
    else if ( std::strcmp(name, IrAtmosphere::getFactoryName()) == 0 ) {
        obj = new IrAtmosphere;
    }
    else if ( std::strcmp(name, IrAtmosphere1::getFactoryName()) == 0 ) {
        obj = new IrAtmosphere1;
    }

    // R/F Signatures
    else if ( std::strcmp(name, SigConstant::getFactoryName()) == 0 ) {
        obj = new SigConstant();
    }
    else if ( std::strcmp(name, SigSphere::getFactoryName()) == 0 ) {
        obj = new SigSphere();
    }
    else if ( std::strcmp(name, SigPlate::getFactoryName()) == 0 ) {
        obj = new SigPlate();
    }
    else if ( std::strcmp(name, SigDihedralCR::getFactoryName()) == 0 ) {
        obj = new SigDihedralCR();
    }
    else if ( std::strcmp(name, SigTrihedralCR::getFactoryName()) == 0 ) {
        obj = new SigTrihedralCR();
    }
    else if ( std::strcmp(name, SigSwitch::getFactoryName()) == 0 ) {
        obj = new SigSwitch();
    }
    else if ( std::strcmp(name, SigAzEl::getFactoryName()) == 0 ) {
        obj = new SigAzEl();
    }
    // IR Signatures
    else if ( std::strcmp(name, IrSignature::getFactoryName()) == 0 ) {
        obj = new IrSignature();
    }
    else if ( std::strcmp(name, AircraftIrSignature::getFactoryName()) == 0 ) {
        obj = new AircraftIrSignature;
    }
    else if ( std::strcmp(name, IrShape::getFactoryName()) == 0 ) {
        obj = new IrShape();
    }
    else if ( std::strcmp(name, IrSphere::getFactoryName()) == 0 ) {
        obj = new IrSphere();
    }
    else if ( std::strcmp(name, IrBox::getFactoryName()) == 0 ) {
        obj = new IrBox();
    }

    // Onboard Computers
    else if ( std::strcmp(name, OnboardComputer::getFactoryName()) == 0 ) {
        obj = new OnboardComputer();
    }

    // Radios
    else if ( std::strcmp(name, Radio::getFactoryName()) == 0 ) {
        obj = new Radio();
    }
    else if ( std::strcmp(name, CommRadio::getFactoryName()) == 0 ) {
        obj = new CommRadio();
    }
    else if ( std::strcmp(name, NavRadio::getFactoryName()) == 0 ) {
        obj = new NavRadio();
    }
    else if ( std::strcmp(name, TacanRadio::getFactoryName()) == 0 ) {
        obj = new TacanRadio();
    }
    else if ( std::strcmp(name, IlsRadio::getFactoryName()) == 0 ) {
        obj = new IlsRadio();
    }
    else if ( std::strcmp(name, Iff::getFactoryName()) == 0 ) {
        obj = new Iff();
    }

    // Sensors
    else if ( std::strcmp(name, RfSensor::getFactoryName()) == 0 ) {
        obj = new RfSensor();
    }
    else if ( std::strcmp(name, SensorMgr::getFactoryName()) == 0 ) {
        obj = new SensorMgr();
    }
    else if ( std::strcmp(name, Radar::getFactoryName()) == 0 ) {
        obj = new Radar();
    }
    else if ( std::strcmp(name, Rwr::getFactoryName()) == 0 ) {
        obj = new Rwr();
    }
    else if ( std::strcmp(name, Sar::getFactoryName()) == 0 ) {
        obj = new Sar();
    }
    else if ( std::strcmp(name, Jammer::getFactoryName()) == 0 ) {
        obj = new Jammer();
    }
    else if ( std::strcmp(name, IrSensor::getFactoryName()) == 0 ) {
        obj = new IrSensor();
    }
    else if ( std::strcmp(name, MergingIrSensor::getFactoryName()) == 0 ) {
        obj = new MergingIrSensor();
    }

    // Tracks
    else if ( std::strcmp(name, Track::getFactoryName()) == 0 ) {
        obj = new Track();
    }

    // Track Managers
    else if ( std::strcmp(name, GmtiTrkMgr::getFactoryName()) == 0 ) {
        obj = new GmtiTrkMgr();
    }
    else if ( std::strcmp(name, AirTrkMgr::getFactoryName()) == 0 ) {
        obj = new AirTrkMgr();
    }
    else if ( std::strcmp(name, RwrTrkMgr::getFactoryName()) == 0 ) {
        obj = new RwrTrkMgr();
    }
    else if ( std::strcmp(name, AirAngleOnlyTrkMgr::getFactoryName()) == 0 ) {
        obj = new AirAngleOnlyTrkMgr();
    }

    // UBF Agents
    else if ( std::strcmp(name, SimAgent::getFactoryName()) == 0 ) {
        obj = new SimAgent();
    }
    else if ( std::strcmp(name, MultiActorAgent::getFactoryName()) == 0 ) {
        obj = new MultiActorAgent();
    }

    // Collision detection component
    else if ( std::strcmp(name, CollisionDetect::getFactoryName()) == 0 ) {
        obj = new CollisionDetect();
    }

    else if ( std::strcmp(name, TabLogger::getFactoryName()) == 0 ) {
        obj = new TabLogger();
    }
    else if ( std::strcmp(name, Otm::getFactoryName()) == 0 ) {
        obj = new Otm();
    }

    return obj;
}

}  // end namespace Simulation
}  // end namespace Eaagles
