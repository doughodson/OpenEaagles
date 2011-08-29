#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeTerrain.a

OBJS =  \
	$(LIB)(DataFile.o) \
	$(LIB)(QuadMap.o) \
	$(LIB)(terrainFF.o)

SUBDIRS = ded dted srtm

all: subdirs ${OBJS}

subdirs:
	for subdir in $(SUBDIRS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

clean:
	-rm -f *.o
	-rm -f $(LIB)
