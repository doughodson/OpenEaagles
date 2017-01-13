#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboemodels.a

CPPFLAGS += -I$(JSBSIM_INC_DIR)

OBJS =  \
	dynamics/AerodynamicsModel.o \
	dynamics/DynamicsModel.o \
	dynamics/JSBSimModel.o \
	dynamics/LaeroModel.o  \
	dynamics/RacModel.o \
	dynamics/SpaceDynamicsModel.o \
	environment/AbstractAtmosphere.o \
	environment/IrAtmosphere.o \
	environment/IrAtmosphere1.o \
	navigation/Bullseye.o \
	navigation/Gps.o \
	navigation/Ins.o \
	navigation/Navigation.o \
	navigation/Route.o \
	navigation/Steerpoint.o \
	players/Aam.o \
	players/AbstractWeapon.o \
	players/Agm.o \
	players/AirVehicle.o \
	players/Bomb.o \
	players/Buildings.o \
	players/Bullet.o \
	players/Effects.o \
	players/GroundVehicle.o \
	players/LifeForms.o \
	players/Missile.o \
	players/Player.o \
	players/Sam.o \
	players/SamVehicles.o \
	players/Ships.o \
	players/SpaceVehicle.o \
	sensors/Gmti.o \
	sensors/Stt.o \
	sensors/Tws.o \
	systems/AngleOnlyTrackManager.o \
	systems/Antenna.o \
	systems/Autopilot.o \
	systems/AvionicsPod.o \
	systems/CollisionDetect.o \
	systems/Datalink.o \
	systems/ExternalStore.o \
	systems/FuelTank.o \
	systems/Gimbal.o \
	systems/Guns.o \
	systems/Iff.o \
	systems/IrSeeker.o \
	systems/IrSensor.o \
	systems/IrSystem.o \
	systems/Jammer.o \
	systems/MergingIrSensor.o \
	systems/NavRadios.o \
	systems/OnboardComputer.o \
	systems/Pilot.o \
	systems/Radar.o \
	systems/Radio.o \
	systems/RfSensor.o \
	systems/RfSystem.o \
	systems/Rwr.o \
	systems/Sar.o \
	systems/ScanGimbal.o \
	systems/StabilizingGimbal.o \
	systems/Stores.o \
	systems/StoresMgr.o \
	systems/System.o \
	systems/TrackManager.o \
	Actions.o \
	AircraftIrSignature.o \
	Designator.o \
	Emission.o \
	Image.o \
	IrQueryMsg.o \
	IrShapes.o \
	IrSignature.o \
	Message.o \
	MultiActorAgent.o \
	SensorMsg.o \
	Signatures.o \
	SimAgent.o \
	SimAgent.o \
	Simulation.o \
	SynchronizedState.o \
	TargetData.o \
	Tdb.o \
	Track.o \
	WorldModel.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f dynamics/*.o
	-rm -f environment/*.o
	-rm -f navigation/*.o
	-rm -f players/*.o
	-rm -f sensors/*.o
	-rm -f systems/*.o
	-rm -f *.o
	-rm -f $(LIB)

