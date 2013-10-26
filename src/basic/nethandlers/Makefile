#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a

OBJS =  \
	$(LIB)(BroadcastHandler.o) \
	$(LIB)(MulticastHandler.o) \
	$(LIB)(TcpClient.o) \
	$(LIB)(TcpHandler.o) \
	$(LIB)(TcpServerMultiple.o) \
	$(LIB)(TcpServerSingle.o) \
	$(LIB)(UdpHandler.o)

all: ${OBJS}

clean:
	-rm -f *.o

