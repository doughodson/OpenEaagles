#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeinstruments.a

OBJS =  \
	factory.o \
	Instrument.o 

SUBDIRS = adi buttons dials landingGear maps eng gauges eadi3D 

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
	-rm -f $(LIB)

