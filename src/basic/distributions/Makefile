#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeBasic.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a
endif

OBJS =  \
	$(LIB)(Exponential.o) \
	$(LIB)(Lognormal.o) \
	$(LIB)(Pareto.o) \
	$(LIB)(Uniform.o)

all: ${OBJS}

clean:
	-rm -f *.o

