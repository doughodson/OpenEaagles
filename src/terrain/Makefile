#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeterrain.a

OBJS =  \
	$(LIB)(DataFile.o) \
	$(LIB)(factory.o) \
	$(LIB)(QuadMap.o)

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
