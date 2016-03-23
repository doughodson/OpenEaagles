#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeterrain.a

OBJS =  \
	DataFile.o \
	factory.o \
	QuadMap.o

SUBDIRS = ded dted srtm

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
