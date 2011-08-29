#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeInstruments.a

OBJS =  \
	$(LIB)(instrumentsFF.o) \
	$(LIB)(Instrument.o) 

SUBDIRS = adi buttons dials landingGear maps eng gauges eadi3D 


all: subdirs ${OBJS}

subdirs:
	for subdir in $(SUBDIRS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

clean:
	-rm -f *.o
	-rm -f $(LIB)

