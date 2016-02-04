#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboemodels.a

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(Gmti.o) \
	$(LIB)(Stt.o) \
	$(LIB)(Tws.o)

all: ${OBJS} 

clean:
	-rm -f *.o
	-rm -f $(LIB)

