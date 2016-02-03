#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboesimulation.a

OBJS =  \
	$(LIB)(Aam.o) \
	$(LIB)(Actions.o) \
	$(LIB)(Agm.o) \
	$(LIB)(AircraftIrSignature.o) \
	$(LIB)(AirVehicle.o) \
	$(LIB)(AngleOnlyTrackManager.o) \
	$(LIB)(Antenna.o) \
	$(LIB)(Autopilot.o) \
	$(LIB)(AvionicsPod.o) \
	$(LIB)(Bomb.o) \
	$(LIB)(Buildings.o) \
	$(LIB)(Bullseye.o) \
	$(LIB)(CollisionDetect.o) \
	$(LIB)(Datalink.o) \
	$(LIB)(DataRecorder.o) \
	$(LIB)(Designator.o) \
	$(LIB)(DynamicsModel.o) \
	$(LIB)(Effects.o) \
	$(LIB)(Emission.o) \
	$(LIB)(ExternalStore.o) \
	$(LIB)(Factory.o) \
	$(LIB)(FuelTank.o) \
	$(LIB)(Gimbal.o) \
	$(LIB)(Gps.o) \
	$(LIB)(GroundVehicle.o) \
	$(LIB)(Guns.o) \
	$(LIB)(Iff.o) \
	$(LIB)(Image.o) \
	$(LIB)(Ins.o) \
	$(LIB)(IrAtmosphere.o) \
	$(LIB)(IrAtmosphere1.o) \
	$(LIB)(IrQueryMsg.o) \
	$(LIB)(IrSeeker.o) \
	$(LIB)(IrSensor.o) \
	$(LIB)(IrShapes.o) \
	$(LIB)(IrSignature.o) \
	$(LIB)(IrSystem.o) \
	$(LIB)(Jammer.o) \
	$(LIB)(LifeForms.o) \
	$(LIB)(MergingIrSensor.o) \
	$(LIB)(Message.o) \
	$(LIB)(Missile.o) \
	$(LIB)(MultiActorAgent.o) \
	$(LIB)(Navigation.o) \
	$(LIB)(NavRadios.o) \
	$(LIB)(NetIO.o) \
	$(LIB)(Nib.o) \
	$(LIB)(Ntm.o) \
	$(LIB)(OnboardComputer.o) \
	$(LIB)(Otw.o) \
	$(LIB)(Pilot.o) \
	$(LIB)(Player.o) \
	$(LIB)(Radar.o) \
	$(LIB)(Radio.o) \
	$(LIB)(RfSensor.o) \
	$(LIB)(RfSystem.o) \
	$(LIB)(Route.o) \
	$(LIB)(Rwr.o) \
	$(LIB)(Sam.o) \
	$(LIB)(SamVehicles.o) \
	$(LIB)(Sar.o) \
	$(LIB)(ScanGimbal.o) \
	$(LIB)(SensorMsg.o) \
	$(LIB)(Ships.o) \
	$(LIB)(Signatures.o) \
	$(LIB)(SimAgent.o) \
	$(LIB)(SimLogger.o) \
	$(LIB)(Simulation.o) \
	$(LIB)(SpaceVehicle.o) \
	$(LIB)(StabilizingGimbal.o) \
	$(LIB)(Station.o) \
	$(LIB)(Steerpoint.o) \
	$(LIB)(Stores.o) \
	$(LIB)(StoresMgr.o) \
	$(LIB)(SynchronizedState.o) \
	$(LIB)(System.o) \
	$(LIB)(TabLogger.o) \
	$(LIB)(TargetData.o) \
	$(LIB)(Tdb.o) \
	$(LIB)(Track.o) \
	$(LIB)(TrackManager.o) \
	$(LIB)(Weapon.o)

SUBDIRS = dynamics

all: ${OBJS}

all: subdirs ${OBJS}

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
