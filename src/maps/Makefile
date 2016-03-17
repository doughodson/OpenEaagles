#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboemaps.a

OBJS =  \
	$(LIB)(factory.o)

SUBDIRS = rpf

all: subdirs ${OBJS}

subdirs:
	for subdir in $(SUBDIRS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

clean:
	-rm -f *.o
	-rm -f $(LIB)

