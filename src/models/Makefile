#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboemodels.a

SUBDIRS = dynamics sensors

OBJS =  \
	factory.o

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
	for subdir in $(SUBDIRS); do \
	    echo making $@ in $$subdir; \
	    (cd $$subdir && $(MAKE) $@) || exit 1; \
	done
	-rm -f $(LIB)

