#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeinstruments.a

OBJS =  \
	adi/Adi.o \
	adi/GhostHorizon.o \
	buttons/Button.o \
	buttons/PushButton.o \
	buttons/RotarySwitch.o \
	buttons/Knob.o \
	buttons/Switch.o \
	buttons/SolenoidSwitch.o \
	dials/AltitudeDial.o \
	dials/AnalogDial.o \
	dials/DialArcSegment.o \
	dials/DialPointer.o \
	dials/DialTickMarks.o \
	dials/GMeterDial.o \
	eadi3d/Eadi3DObjects.o \
	eadi3d/Eadi3DPage.o \
	eadi3d/IrisGLCompat.o \
	eng/EngPage.o \
	gauges/AnalogGauge.o \
	gauges/AoaIndexer.o \
	gauges/GaugeSlider.o \
	gauges/Tape.o \
	gauges/TickMarks.o \
	landinggear/LandingGear.o \
	landinggear/LandingLight.o \
	maps/BearingPointer.o \
	maps/CompassRose.o \
	factory.o \
	Instrument.o

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f adi/*.o
	-rm -f buttons/*.o
	-rm -f dials/*.o
	-rm -f eadi3d/*.o
	-rm -f eng/*.o
	-rm -f gauges/*.o
	-rm -f landinggear/*.o
	-rm -f maps/*.o
	-rm -f *.o
	-rm -f $(LIB)
