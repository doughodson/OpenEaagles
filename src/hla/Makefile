#
include ../makedefs

LIB = $(EAAGLES_LIB_DIR)/libHla.a
OBJS =  \
	$(LIB)(BaseAmbassador.o) \
	$(LIB)(HlaIO.o) \
	$(LIB)(hlaFF.o)

SUBDIRS = mc02_fom

CPPFLAGS += -DEAAGLES_WITH_HLA -DRTI_USES_STD_FSTREAM -I$(RTI_HOME)/$(RTI_BUILD_TYPE)/include

all: subdirs ${OBJS} 

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
