#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeLinearSys.a


OBJS1 =  \
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


all: ${OBJS1}

clean:
	-rm -f *.o
	-rm -f $(LIB)

