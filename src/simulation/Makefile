#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboesimulation.a

OBJS =  \
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

SUBDIRS = dynamics

.PHONY: all clean subdirs

all: subdirs $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

subdirs:
	for subdir in $(SUBDIRS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

clean:
	-rm -f *.o
	for subdir in $(SUBDIRS); do \
	    echo making $@ in $$subdir; \
	    (cd $$subdir && $(MAKE) $@) || exit 1; \
	done
	-rm -f $(LIB)
