#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboemodels.a

CPPFLAGS += -I$(JSBSIM_INC_DIR)

SUBDIRS = 

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(JSBSimModel.o) \
	$(LIB)(LaeroModel.o)  \
	$(LIB)(RacModel.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)
