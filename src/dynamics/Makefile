#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeDynamics.a

CPPFLAGS += -I$(JSBSIM_INC_DIR)

OBJS =  \
	Factory.o \
	JSBSimModel.o \
	LaeroModel.o  \
	RacModel.o \
	dynamicsFF.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)
