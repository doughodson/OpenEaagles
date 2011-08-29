#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeVehicles.a

CPPFLAGS += -I$(JSBSIM_INC_DIR)

SUBDIRS = 

OBJS =  \
	$(LIB)(vehiclesFF.o) \
	$(LIB)(JSBSimModel.o) \
	$(LIB)(RacModel.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)
