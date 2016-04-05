#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboesimulation.a

OBJS =  \
	dynamics/AerodynamicsModel.o \
	dynamics/SpaceDynamicsModel.o \
	Aam.o \
	Actions.o \
	Agm.o \
	AircraftIrSignature.o \
	AirVehicle.o \
	AngleOnlyTrackManager.o \
	Antenna.o \
	Autopilot.o \
	AvionicsPod.o \
	Bomb.o \
	Buildings.o \
	Bullseye.o \
	CollisionDetect.o \
	Datalink.o \
	DataRecorder.o \
	Designator.o \
	DynamicsModel.o \
	Effects.o \
	Emission.o \
	ExternalStore.o \
	factory.o \
	FuelTank.o \
	Gimbal.o \
	Gps.o \
	GroundVehicle.o \
	Guns.o \
	Iff.o \
	Image.o \
	Ins.o \
	IrAtmosphere.o \
	IrAtmosphere1.o \
	IrQueryMsg.o \
	IrSeeker.o \
	IrSensor.o \
	IrShapes.o \
	IrSignature.o \
	IrSystem.o \
	Jammer.o \
	LifeForms.o \
	MergingIrSensor.o \
	Message.o \
	Missile.o \
	MultiActorAgent.o \
	Navigation.o \
	NavRadios.o \
	NetIO.o \
	Nib.o \
	Ntm.o \
	OnboardComputer.o \
	Otw.o \
	Pilot.o \
	Player.o \
	Radar.o \
	Radio.o \
	RfSensor.o \
	RfSystem.o \
	Route.o \
	Rwr.o \
	Sam.o \
	SamVehicles.o \
	Sar.o \
	ScanGimbal.o \
	SensorMsg.o \
	Ships.o \
	Signatures.o \
	SimAgent.o \
	SimLogger.o \
	Simulation.o \
	SpaceVehicle.o \
	StabilizingGimbal.o \
	Station.o \
	Steerpoint.o \
	Stores.o \
	StoresMgr.o \
	SynchronizedState.o \
	System.o \
	TabLogger.o \
	TargetData.o \
	Tdb.o \
	Track.o \
	TrackManager.o \
	Weapon.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f dynamics/*.o
	-rm -f *.o
	-rm -f $(LIB)
