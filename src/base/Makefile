
#----------------------------------------------------------------------------------------
# To build parser and scanner:
#
# dos2unix edl.y
# dos2unix edl.l
# bison -d edl.y -o EdlParser.cpp  // creates EdlParser.cpp and EdlParser.hpp
# flex -f -oEdlScanner.cpp edl.l   // creates EdlScanner.cpp (EdlScanner.h already exists)
#
# search for "FlexLexer.h" in EdlScanner.cpp - fix include path
#----------------------------------------------------------------------------------------

include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboebase.a

OBJS =  \
	Boolean.o \
	Cie.o \
	Cmy.o \
	Color.o \
	Complex.o \
	Component.o \
	Decibel.o \
	EarthModel.o \
	EdlParser.o \
	EdlScanner.o \
	factory.o \
	FileReader.o \
	Float.o \
	Hls.o \
	Hsva.o \
	Hsv.o \
	Identifier.o \
	Integer.o \
	IoAdapter.o \
	IoData.o \
	IoDevice.o \
	IoHandler.o \
	LatLon.o \
	List.o \
	Locus.o \
	Logger.o \
	Matrix.o \
	MonitorMetrics.o \
	Nav.o \
	NavDR.o \
	NetHandler.o \
	Number.o \
	Object.o \
	Operators.o \
	Pair.o \
	PairStream.o \
	Rgba.o \
	Rgb.o \
	Rng.o \
	SlotTable.o \
	Stack.o \
	Statistic.o \
	StateMachine.o \
	String.o \
	Terrain.o \
	Thread.o \
	ThreadPool.o \
	Timers.o \
	Transforms.o \
	Vectors.o \
	Yiq.o

SUBDIRS = distributions functors nethandlers osg platform ubf units util

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

