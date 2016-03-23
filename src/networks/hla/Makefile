#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboehla.a

CPPFLAGS += -I$(HLA_INC_DIR)
CPPFLAGS += -DRTI_USES_STD_FSTREAM
#CPPFLAGS += -DEAAGLES_WITH_HLA -DRTI_USES_STD_FSTREAM -I$(RTI_HOME)/$(RTI_BUILD_TYPE)/include

OBJS =  \
	Ambassador.o \
	factory.o \
	NetIO.o \
	Nib.o

SUBDIRS = rprfom

.PHONY: all clean subdirs

.NOTPARALLEL:

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

