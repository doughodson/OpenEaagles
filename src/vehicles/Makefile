#
include ../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeVehicles.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeVehicles.a
endif

CPPFLAGS += -I$(JSBSIM_INC_DIR)

SUBDIRS = 

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(JSBSimModel.o) \
	$(LIB)(LaeroModel.o)  \
	$(LIB)(RacModel.o) \
	$(LIB)(vehiclesFF.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)
