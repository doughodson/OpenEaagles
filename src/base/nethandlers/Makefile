#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboebase.a

OBJS =  \
	PosixHandler.o \
	TcpClient.o \
	TcpHandler.o \
	TcpServerMultiple.o \
	TcpServerSingle.o \
	UdpBroadcastHandler.o \
	UdpMulticastHandler.o \
	UdpUnicastHandler.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

