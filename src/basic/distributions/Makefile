#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a

OBJS =  \
	Exponential.o \
	Lognormal.o \
	Pareto.o \
	Uniform.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o

