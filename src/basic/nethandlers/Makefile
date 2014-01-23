#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeBasic.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a
endif

OBJS =  \
	$(LIB)(PosixHandler.o) \
	$(LIB)(TcpClient.o) \
	$(LIB)(TcpHandler.o) \
	$(LIB)(TcpServerMultiple.o) \
	$(LIB)(TcpServerSingle.o) \
	$(LIB)(UdpBroadcastHandler.o) \
	$(LIB)(UdpMulticastHandler.o) \
	$(LIB)(UdpUnicastHandler.o)

all: ${OBJS}

clean:
	-rm -f *.o

