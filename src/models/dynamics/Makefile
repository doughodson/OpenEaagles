#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboemodels.a

CPPFLAGS += -I$(JSBSIM_INC_DIR)

SUBDIRS = 

OBJS =  \
	JSBSimModel.o \
	LaeroModel.o  \
	RacModel.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
