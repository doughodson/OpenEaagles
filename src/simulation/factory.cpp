
#include "openeaagles/simulation/factory.hpp"

#include "openeaagles/base/Object.hpp"

#include "openeaagles/simulation/Aam.hpp"
#include "openeaagles/simulation/Actions.hpp"
#include "openeaagles/simulation/Agm.hpp"
#include "openeaagles/simulation/AirVehicle.hpp"
#include "openeaagles/simulation/AngleOnlyTrackManager.hpp"
#include "openeaagles/simulation/Antenna.hpp"
#include "openeaagles/simulation/Autopilot.hpp"
#include "openeaagles/simulation/AvionicsPod.hpp"
#include "openeaagles/simulation/Bomb.hpp"
#include "openeaagles/simulation/Buildings.hpp"
#include "openeaagles/simulation/Bullseye.hpp"
#include "openeaagles/simulation/CollisionDetect.hpp"
#include "openeaagles/simulation/Datalink.hpp"
#include "openeaagles/simulation/Effects.hpp"
#include "openeaagles/simulation/ExternalStore.hpp"
#include "openeaagles/simulation/FuelTank.hpp"
#include "openeaagles/simulation/Gimbal.hpp"
#include "openeaagles/simulation/GroundVehicle.hpp"
#include "openeaagles/simulation/Gps.hpp"
#include "openeaagles/simulation/Guns.hpp"
#include "openeaagles/simulation/Iff.hpp"
#include "openeaagles/simulation/Ins.hpp"
#include "openeaagles/simulation/IrAtmosphere.hpp"
#include "openeaagles/simulation/IrAtmosphere1.hpp"
#include "openeaagles/simulation/IrSeeker.hpp"
#include "openeaagles/simulation/IrSensor.hpp"
#include "openeaagles/simulation/IrShapes.hpp"
#include "openeaagles/simulation/IrSignature.hpp"
#include "openeaagles/simulation/AircraftIrSignature.hpp"
#include "openeaagles/simulation/Jammer.hpp"
#include "openeaagles/simulation/LifeForms.hpp"
#include "openeaagles/simulation/MergingIrSensor.hpp"
#include "openeaagles/simulation/Missile.hpp"
#include "openeaagles/simulation/Navigation.hpp"
#include "openeaagles/simulation/NavRadios.hpp"
#include "openeaagles/simulation/OnboardComputer.hpp"
#include "openeaagles/simulation/Otw.hpp"
#include "openeaagles/simulation/Pilot.hpp"
#include "openeaagles/simulation/Player.hpp"
#include "openeaagles/simulation/Radar.hpp"
#include "openeaagles/simulation/Radio.hpp"
#include "openeaagles/simulation/RfSensor.hpp"
#include "openeaagles/simulation/Route.hpp"
#include "openeaagles/simulation/Rwr.hpp"
#include "openeaagles/simulation/Sam.hpp"
#include "openeaagles/simulation/SamVehicles.hpp"
#include "openeaagles/simulation/Sar.hpp"
#include "openeaagles/simulation/ScanGimbal.hpp"
#include "openeaagles/simulation/Ships.hpp"
#include "openeaagles/simulation/Signatures.hpp"

#include "openeaagles/simulation/SimAgent.hpp"
#include "openeaagles/simulation/MultiActorAgent.hpp"

#include "openeaagles/simulation/Simulation.hpp"
#include "openeaagles/simulation/SpaceVehicle.hpp"
#include "openeaagles/simulation/StabilizingGimbal.hpp"
#include "openeaagles/simulation/Station.hpp"
#include "openeaagles/simulation/Steerpoint.hpp"
#include "openeaagles/simulation/Stores.hpp"
#include "openeaagles/simulation/StoresMgr.hpp"
#include "openeaagles/simulation/System.hpp"
#include "openeaagles/simulation/TabLogger.hpp"
#include "openeaagles/simulation/TargetData.hpp"
#include "openeaagles/simulation/Track.hpp"
#include "openeaagles/simulation/TrackManager.hpp"
#include "openeaagles/simulation/Weapon.hpp"

#include <string>

namespace oe {
namespace simulation {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // Simulation
    if ( name == Simulation::getFactoryName() ) {
        obj = new Simulation();
    }
    else if ( name == Station::getFactoryName() ) {
        obj = new Station();
    }

    // Players
    else if ( name == Player::getFactoryName() ) {
        obj = new Player();
    }
    else if ( name == AirVehicle::getFactoryName() ) {
        obj = new AirVehicle();
    }
    else if ( name == Building::getFactoryName() ) {
        obj = new Building();
    }
    else if ( name == GroundVehicle::getFactoryName() ) {
        obj = new GroundVehicle();
    }
    else if ( name == LifeForm::getFactoryName() ) {
        obj = new LifeForm();
    }
    else if ( name == Ship::getFactoryName() ) {
        obj = new Ship();
    }
    else if ( name == SpaceVehicle::getFactoryName() ) {
        obj = new SpaceVehicle();
    }

    // Air Vehicles
    else if ( name == Aircraft::getFactoryName() ) {
        obj = new Aircraft();
    }
    else if ( name == Helicopter::getFactoryName() ) {
        obj = new Helicopter();
    }
    else if ( name == UnmannedAirVehicle::getFactoryName() ) {
        obj = new UnmannedAirVehicle();
    }

    // Ground Vehicles
    else if ( name == Tank::getFactoryName() ) {
        obj = new Tank();
    }
    else if ( name == ArmoredVehicle::getFactoryName() ) {
        obj = new ArmoredVehicle();
    }
    else if ( name == WheeledVehicle::getFactoryName() ) {
        obj = new WheeledVehicle();
    }
    else if ( name == Artillery::getFactoryName() ) {
        obj = new Artillery();
    }
    else if ( name == SamVehicle::getFactoryName() ) {
        obj = new SamVehicle();
    }
    else if ( name == GroundStation::getFactoryName() ) {
        obj = new GroundStation();
    }
    else if ( name == GroundStationRadar::getFactoryName() ) {
        obj = new GroundStationRadar();
    }
    else if ( name == GroundStationUav::getFactoryName() ) {
        obj = new GroundStationUav();
    }

    // Space Vehicles
    else if ( name == MannedSpaceVehicle::getFactoryName() ) {
        obj = new MannedSpaceVehicle();
    }
    else if ( name == UnmannedSpaceVehicle::getFactoryName() ) {
        obj = new UnmannedSpaceVehicle();
    }
    else if ( name == BoosterSpaceVehicle::getFactoryName() ) {
        obj = new BoosterSpaceVehicle();
    }

    // System
    else if ( name == System::getFactoryName() ) {
        obj = new System();
    }
    else if ( name == AvionicsPod::getFactoryName() ) {
        obj = new AvionicsPod();
    }

    // Basic Pilot types
    else if ( name == Pilot::getFactoryName() ) {
        obj = new Pilot();
    }
    else if ( name == Autopilot::getFactoryName() ) {
        obj = new Autopilot();
    }

    // Navigation types
    else if ( name == Navigation::getFactoryName() ) {
        obj = new Navigation();
    }
    else if ( name == Ins::getFactoryName() ) {
        obj = new Ins();
    }
    else if ( name == Gps::getFactoryName() ) {
        obj = new Gps();
    }
    else if ( name == Route::getFactoryName() ) {
        obj = new Route();
    }
    else if ( name == Steerpoint::getFactoryName() ) {
        obj = new Steerpoint();
    }

    // Target Data
    else if ( name == TargetData::getFactoryName() ) {
        obj = new TargetData();
    }

    // Bullseye
    else if ( name == Bullseye::getFactoryName() ) {
        obj = new Bullseye();
    }

    // Actions
    else if ( name == ActionImagingSar::getFactoryName() ) {
        obj = new ActionImagingSar();
    }
    else if ( name == ActionWeaponRelease::getFactoryName() ) {
        obj = new ActionWeaponRelease();
    }
    else if ( name == ActionDecoyRelease::getFactoryName() ) {
        obj = new ActionDecoyRelease();
    }
    else if ( name == ActionCamouflageType::getFactoryName() ) {
        obj = new ActionCamouflageType();
    }

    // Bombs and Missiles
    else if ( name == Bomb::getFactoryName() ) {
        obj = new Bomb();
    }
    else if ( name == Missile::getFactoryName() ) {
        obj = new Missile();
    }
    else if ( name == Aam::getFactoryName() ) {
        obj = new Aam();
    }
    else if ( name == Agm::getFactoryName() ) {
        obj = new Agm();
    }
    else if ( name == Sam::getFactoryName() ) {
        obj = new Sam();
    }

    // Effects
    else if ( name == Chaff::getFactoryName() ) {
        obj = new Chaff();
    }
    else if ( name == Decoy::getFactoryName() ) {
        obj = new Decoy();
    }
    else if ( name == Flare::getFactoryName() ) {
        obj = new Flare();
    }

    // Stores, stores manager and external stores (FuelTank, Gun & Bullets (used by the Gun))
    else if ( name == Stores::getFactoryName() ) {
        obj = new Stores();
    }
    else if ( name == SimpleStoresMgr::getFactoryName() ) {
        obj = new SimpleStoresMgr();
    }
    else if ( name == FuelTank::getFactoryName() ) {
        obj = new FuelTank();
    }
    else if ( name == Gun::getFactoryName() ) {
        obj = new Gun();
    }
    else if ( name == Bullet::getFactoryName() ) {
        obj = new Bullet();
    }

    // Data links
    else if ( name == Datalink::getFactoryName() ) {
        obj = new Datalink();
    }

    // Gimbals, Antennas and Optics
    else if ( name == Gimbal::getFactoryName() ) {
        obj = new Gimbal();
    }
    else if ( name == ScanGimbal::getFactoryName() ) {
        obj = new ScanGimbal();
    }
    else if ( name == StabilizingGimbal::getFactoryName() ) {
        obj = new StabilizingGimbal();
    }
    else if ( name == Antenna::getFactoryName() ) {
        obj = new Antenna();
    }
    else if ( name == IrSeeker::getFactoryName() ) {
        obj = new IrSeeker();
    }

    // IR Atmospheres
    else if ( name == IrAtmosphere::getFactoryName() ) {
        obj = new IrAtmosphere;
    }
    else if ( name == IrAtmosphere1::getFactoryName() ) {
        obj = new IrAtmosphere1;
    }

    // R/F Signatures
    else if ( name == SigConstant::getFactoryName() ) {
        obj = new SigConstant();
    }
    else if ( name == SigSphere::getFactoryName() ) {
        obj = new SigSphere();
    }
    else if ( name == SigPlate::getFactoryName() ) {
        obj = new SigPlate();
    }
    else if ( name == SigDihedralCR::getFactoryName() ) {
        obj = new SigDihedralCR();
    }
    else if ( name == SigTrihedralCR::getFactoryName() ) {
        obj = new SigTrihedralCR();
    }
    else if ( name == SigSwitch::getFactoryName() ) {
        obj = new SigSwitch();
    }
    else if ( name == SigAzEl::getFactoryName() ) {
        obj = new SigAzEl();
    }
    // IR Signatures
    else if ( name == IrSignature::getFactoryName() ) {
        obj = new IrSignature();
    }
    else if ( name == AircraftIrSignature::getFactoryName() ) {
        obj = new AircraftIrSignature;
    }
    else if ( name == IrShape::getFactoryName() ) {
        obj = new IrShape();
    }
    else if ( name == IrSphere::getFactoryName() ) {
        obj = new IrSphere();
    }
    else if ( name == IrBox::getFactoryName() ) {
        obj = new IrBox();
    }

    // Onboard Computers
    else if ( name == OnboardComputer::getFactoryName() ) {
        obj = new OnboardComputer();
    }

    // Radios
    else if ( name == Radio::getFactoryName() ) {
        obj = new Radio();
    }
    else if ( name == CommRadio::getFactoryName() ) {
        obj = new CommRadio();
    }
    else if ( name == NavRadio::getFactoryName() ) {
        obj = new NavRadio();
    }
    else if ( name == TacanRadio::getFactoryName() ) {
        obj = new TacanRadio();
    }
    else if ( name == IlsRadio::getFactoryName() ) {
        obj = new IlsRadio();
    }
    else if ( name == Iff::getFactoryName() ) {
        obj = new Iff();
    }

    // Sensors
    else if ( name == RfSensor::getFactoryName() ) {
        obj = new RfSensor();
    }
    else if ( name == SensorMgr::getFactoryName() ) {
        obj = new SensorMgr();
    }
    else if ( name == Radar::getFactoryName() ) {
        obj = new Radar();
    }
    else if ( name == Rwr::getFactoryName() ) {
        obj = new Rwr();
    }
    else if ( name == Sar::getFactoryName() ) {
        obj = new Sar();
    }
    else if ( name == Jammer::getFactoryName() ) {
        obj = new Jammer();
    }
    else if ( name == IrSensor::getFactoryName() ) {
        obj = new IrSensor();
    }
    else if ( name == MergingIrSensor::getFactoryName() ) {
        obj = new MergingIrSensor();
    }

    // Tracks
    else if ( name == Track::getFactoryName() ) {
        obj = new Track();
    }

    // Track Managers
    else if ( name == GmtiTrkMgr::getFactoryName() ) {
        obj = new GmtiTrkMgr();
    }
    else if ( name == AirTrkMgr::getFactoryName() ) {
        obj = new AirTrkMgr();
    }
    else if ( name == RwrTrkMgr::getFactoryName() ) {
        obj = new RwrTrkMgr();
    }
    else if ( name == AirAngleOnlyTrkMgr::getFactoryName() ) {
        obj = new AirAngleOnlyTrkMgr();
    }

    // UBF Agents
    else if ( name == SimAgent::getFactoryName() ) {
        obj = new SimAgent();
    }
    else if ( name == MultiActorAgent::getFactoryName() ) {
        obj = new MultiActorAgent();
    }

    // Collision detection component
    else if ( name == CollisionDetect::getFactoryName() ) {
        obj = new CollisionDetect();
    }

    else if ( name == TabLogger::getFactoryName() ) {
        obj = new TabLogger();
    }
    else if ( name == Otm::getFactoryName() ) {
        obj = new Otm();
    }

    return obj;
}

}
}

