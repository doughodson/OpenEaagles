#
include ../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeLinearSys.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeLinearSys.a
endif

OBJS =  \
	$(LIB)(DiffEquation.o) \
	$(LIB)(FirstOrderTf.o) \
	$(LIB)(LagFilter.o) \
	$(LIB)(Limit01.o) \
	$(LIB)(Limit11.o) \
	$(LIB)(Limit.o) \
	$(LIB)(LimitFunc.o) \
	$(LIB)(LowpassFilter.o) \
	$(LIB)(SaH.o) \
	$(LIB)(ScalerFunc.o) \
	$(LIB)(SecondOrderTf.o) \
	$(LIB)(Sz1.o) \
	$(LIB)(Sz2.o)


all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

