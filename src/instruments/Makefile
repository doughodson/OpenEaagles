#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeinstruments.a

OBJS =  \
	factory.o \
	Instrument.o

SUBDIRS = adi buttons dials eadi3D eng gauges landingGear maps

.PHONY: all clean subdirs $(SUBDIRS)

.NOTPARALLEL:

all: subdirs $(LIB)

$(LIB): $(OBJS)
	ar r $(LIB) $(OBJS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	-rm -f *.o
	for subdir in $(SUBDIRS); do \
	    echo making $@ in $$subdir; \
	    (cd $$subdir && $(MAKE) $@) || exit 1; \
	done
	-rm -f $(LIB)

