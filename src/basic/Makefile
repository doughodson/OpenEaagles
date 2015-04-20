#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a

OBJS =  \
	$(LIB)(support.o) \
	$(LIB)(basicFF.o) \
	$(LIB)(Boolean.o) \
	$(LIB)(Cie.o) \
	$(LIB)(Cmy.o) \
	$(LIB)(Color.o) \
	$(LIB)(Complex.o) \
	$(LIB)(Component.o) \
	$(LIB)(Decibel.o) \
	$(LIB)(EarthModel.o) \
	$(LIB)(Factory.o) \
	$(LIB)(FileReader.o) \
	$(LIB)(Float.o) \
	$(LIB)(Function.o) \
	$(LIB)(Functions.o) \
	$(LIB)(Hls.o) \
	$(LIB)(Hsva.o) \
	$(LIB)(Hsv.o) \
	$(LIB)(Identifier.o) \
	$(LIB)(Integer.o) \
	$(LIB)(IoAdapter.o) \
	$(LIB)(IoData.o) \
	$(LIB)(IoDevice.o) \
	$(LIB)(IoHandler.o) \
	$(LIB)(LatLon.o) \
	$(LIB)(Lexical.o) \
	$(LIB)(List.o) \
	$(LIB)(Locus.o) \
	$(LIB)(Logger.o) \
	$(LIB)(Matrix.o) \
	$(LIB)(Nav.o) \
	$(LIB)(NavDR.o) \
	$(LIB)(NetHandler.o) \
	$(LIB)(Number.o) \
	$(LIB)(Object.o) \
	$(LIB)(Operators.o) \
	$(LIB)(Pair.o) \
	$(LIB)(PairStream.o) \
	$(LIB)(Parser.o) \
	$(LIB)(Rgba.o) \
	$(LIB)(Rgb.o) \
	$(LIB)(Rng.o) \
	$(LIB)(SlotTable.o) \
	$(LIB)(Stack.o) \
	$(LIB)(Statistic.o) \
	$(LIB)(StateMachine.o) \
	$(LIB)(String.o) \
	$(LIB)(Table.o) \
	$(LIB)(Tables.o) \
	$(LIB)(Terrain.o) \
	$(LIB)(Thread.o) \
	$(LIB)(ThreadPool.o) \
	$(LIB)(Timers.o) \
	$(LIB)(Transforms.o) \
	$(LIB)(Vectors.o) \
	$(LIB)(Yiq.o)

SUBDIRS = distributions functors nethandlers osg ubf units util

all: subdirs ${OBJS}

subdirs:
	for subdir in $(SUBDIRS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

$(LIB)(Lexical.o): Lexical.cpp Parser.cpp Parser.hpp

#Parser.cpp:  Parser.y
#	dos2unix Parser.y
#	bison -t Parser.y -o Parser.cpp

#Lexical.cpp: Lexical.l
#	dos2unix Lexical.l
#	flex -f -oLexical.cpp Lexical.l

clean:
	-rm -f *.o
	for subdir in $(SUBDIRS); do \
	    echo making $@ in $$subdir; \
	    (cd $$subdir && $(MAKE) $@) || exit 1; \
	done
	-rm -f $(LIB)

