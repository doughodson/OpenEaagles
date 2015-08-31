#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a

OBJS =  \
	PosixHandler.o \
	TcpClient.o \
	TcpHandler.o \
	TcpServerMultiple.o \
	TcpServerSingle.o \
	UdpBroadcastHandler.o \
	UdpMulticastHandler.o \
	UdpUnicastHandler.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o

