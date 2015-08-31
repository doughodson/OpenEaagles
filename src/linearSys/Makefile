#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeLinearSys.a

OBJS =  \
	DiffEquation.o \
	FirstOrderTf.o \
	LagFilter.o \
	Limit01.o \
	Limit11.o \
	Limit.o \
	LimitFunc.o \
	LowpassFilter.o \
	SaH.o \
	ScalerFunc.o \
	SecondOrderTf.o \
	Sz1.o \
	Sz2.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

