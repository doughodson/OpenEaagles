#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeMaps.a

OBJS =  \
	Factory.o \
	mapsFF.o

SUBDIRS = rpfMap

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

