#
include ../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeTerrain.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeTerrain.a
endif

OBJS =  \
	$(LIB)(DataFile.o) \
	$(LIB)(Factory.o) \
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
