#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a

OBJS =  \
	support.o \
	basicFF.o \
	Boolean.o \
	Cie.o \
	Cmy.o \
	Color.o \
	Complex.o \
	Component.o \
	Decibel.o \
	EarthModel.o \
	Factory.o \
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
	Lexical.o \
	List.o \
	Locus.o \
	Logger.o \
	Matrix.o \
	Nav.o \
	NavDR.o \
	NetHandler.o \
	Number.o \
	Object.o \
	Operators.o \
	Pair.o \
	PairStream.o \
	Parser.o \
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

SUBDIRS = distributions functors nethandlers osg ubf units util

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: $(SUBDIRS) ${OBJS}
    
archive:
	for subdir in $(SUBDIRS); do \
		$(MAKE) -C $$subdir $(MAKECMDGOALS); \
	done
	ar rv $(LIB) ${OBJS}

Lexical.o: Lexical.cpp Parser.cpp Parser.hpp

clean: $(SUBDIRS)
	-rm -f *.o
	-rm -f $(LIB)
    
$(SUBDIRS)::
	$(MAKE) -C $@ $(MAKECMDGOALS)

