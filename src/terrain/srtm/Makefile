#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeTerrain.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeTerrain.a
endif

OBJS =  $(LIB)(SrtmHgtFile.o) 

all: ${OBJS} 

clean:
	-rm -f *.o
