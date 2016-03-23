#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboemodels.a

CPPFLAGS += -I$(JSBSIM_INC_DIR)

OBJS =  \
	dynamics/JSBSimModel.o \
	dynamics/LaeroModel.o  \
	dynamics/RacModel.o \
	sensors/Gmti.o \
	sensors/Stt.o \
	sensors/Tws.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f dynamics/*.o
	-rm -f sensors/*.o
	-rm -f *.o
	-rm -f $(LIB)

