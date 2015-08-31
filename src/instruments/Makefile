#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeInstruments.a

OBJS =  \
	Factory.o \
	instrumentsFF.o \
	Instrument.o 

SUBDIRS = adi buttons dials landingGear maps eng gauges eadi3D 

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: $(SUBDIRS) ${OBJS}
    
archive:
	for subdir in $(SUBDIRS); do \
		$(MAKE) -C $$subdir $(MAKECMDGOALS); \
	done
	ar rv $(LIB) ${OBJS}

clean: $(SUBDIRS)
	-rm -f *.o
	-rm -f $(LIB)
    
$(SUBDIRS)::
	$(MAKE) -C $@ $(MAKECMDGOALS)

