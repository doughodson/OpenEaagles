#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboe_models.a

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
	player/Aam.o \
	player/AbstractWeapon.o \
	player/Agm.o \
	player/AirVehicle.o \
	player/Bomb.o \
	player/Buildings.o \
	player/Bullet.o \
	player/Effects.o \
	player/GroundVehicle.o \
	player/LifeForms.o \
	player/Missile.o \
	player/Player.o \
	player/Sam.o \
	player/SamVehicles.o \
	player/Ships.o \
	player/SpaceVehicle.o \
	sensor/Gmti.o \
	sensor/Stt.o \
	sensor/Tws.o \
	system/AngleOnlyTrackManager.o \
	system/Antenna.o \
	system/Autopilot.o \
	system/AvionicsPod.o \
	system/CollisionDetect.o \
	system/Datalink.o \
	system/ExternalStore.o \
	system/FuelTank.o \
	system/Gimbal.o \
	system/Guns.o \
	system/Iff.o \
	system/IrSeeker.o \
	system/IrSensor.o \
	system/IrSystem.o \
	system/Jammer.o \
	system/MergingIrSensor.o \
	system/OnboardComputer.o \
	system/Pilot.o \
	system/Radar.o \
	system/Radio.o \
	system/RfSensor.o \
	system/RfSystem.o \
	system/Rwr.o \
	system/Sar.o \
	system/ScanGimbal.o \
	system/StabilizingGimbal.o \
	system/Stores.o \
	system/StoresMgr.o \
	system/System.o \
	system/TrackManager.o \
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

