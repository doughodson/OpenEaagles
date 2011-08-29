#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeMaps.a

OBJS =  \
	$(LIB)(mapsFF.o) 

SUBDIRS = rpfMap vpfMap


all: subdirs ${OBJS}

subdirs:
	for subdir in $(SUBDIRS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

clean:
	-rm -f *.o
	-rm -f $(LIB)

