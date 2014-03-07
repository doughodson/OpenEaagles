//------------------------------------------------------------------------------
// Class: TcpHandler
//------------------------------------------------------------------------------
//
// M$ WinSock has slightly different return types, some different calling, and
// is missing some of the calls that are standard in Berkeley and POSIX socket
// implementation.  These slight differences will be handled in setting basic
// typedefs, defines, and constants that will make each convention match for
// use later in the code.  This will save a lot of pre-processor intervention
// and make the code that much more enjoyable to read!
//
#if defined(WIN32)
    #include <sys/types.h>
    #include <Winsock2.h>
    #include <WS2tcpip.h>
    #define  bzero(a,b)   ZeroMemory(a,b)
    typedef int socklen_t;
#else
    #include <arpa/inet.h>
    #include <sys/fcntl.h>
    #include <sys/ioctl.h>
    #ifdef sun
        #include <sys/filio.h> // -- added for Solaris 10
    #endif
    static const int INVALID_SOCKET = -1; // Always -1 and errno is set
    static const int SOCKET_ERROR   = -1;
#endif

#include "openeaagles/basic/nethandlers/TcpHandler.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

#include <cstdio>

namespace Eaagles {
namespace Basic {

//==============================================================================
// Class: TcpHandler
//==============================================================================
IMPLEMENT_SUBCLASS(TcpHandler,"TcpHandler")
EMPTY_SLOTTABLE(TcpHandler)
EMPTY_SERIALIZER(TcpHandler)
EMPTY_DELETEDATA(TcpHandler)

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
TcpHandler::TcpHandler()
{
   STANDARD_CONSTRUCTOR()

   connected = false;
   connectionTerminated = false;
   noWait = false;
}

TcpHandler::TcpHandler(const LcSocket sn)
{
   STANDARD_CONSTRUCTOR()

   connected = true;
   connectionTerminated = false;
   noWait = false;
   socketNum = sn;
   setSendBuffSize();
   setRecvBuffSize();
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TcpHandler::copyData(const TcpHandler& org, const bool)
{
   BaseClass::copyData(org);

   connected = org.connected;
   connectionTerminated = org.connectionTerminated;
   noWait = org.noWait;
}

//------------------------------------------------------------------------------
// init() -- init the network, the socket and the network address
//------------------------------------------------------------------------------
bool TcpHandler::init()
{
    // ---
    // Init the base class
    // ---
    bool success = BaseClass::init();
    if (!success) return false;

    // ---
    // Create our stream socket
    // ---
    socketNum = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socketNum == INVALID_SOCKET) {
        std::perror("TcpHandler::init(): socket error");
        success = false;
    }

    return success;
}


// -------------------------------------------------------------
// Returns true if the network handler has been initialized and connected.
// -------------------------------------------------------------
bool TcpHandler::isConnected() const
{
    return connected && BaseClass::isConnected();
}

//------------------------------------------------------------------------------
// closeConnection() -- request that the connection is closed (shutdown)
//------------------------------------------------------------------------------
bool TcpHandler::closeConnection()
{
    bool success = true;

#if defined(WIN32)
    if (::closesocket(socketNum) == SOCKET_ERROR) {
#else
    if (::shutdown(socketNum, SHUT_RDWR) == SOCKET_ERROR) {
#endif
        std::perror("TcpHandler::closeConnection(): error! \n");
        success = false;
    }

    connected = false;
    connectionTerminated = true;

    return BaseClass::closeConnection() && success;
}

// -------------------------------------------------------------
// sendData() -- Send data to our connected TCP socket
// -------------------------------------------------------------
bool TcpHandler::sendData(const char* const packet, const int size)
{
    if (!isConnected() || hasBeenTerminated()) return false;

    if (socketNum == INVALID_SOCKET) return false;

    int result = ::send(socketNum, packet, size, 0);
    if (result == SOCKET_ERROR) {
        connected = false;
        connectionTerminated = true;
#if defined(WIN32)
        int err = WSAGetLastError();
        if (isMessageEnabled(MSG_ERROR)) {
           std::cerr << "TcpHandler::sendData(): sendto error: " << err << " hex=0x" << std::hex << err << std::dec << std::endl;
        }
#else
        std::perror("TcpHandler::sendData(): sendto error msg");
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "TcpHandler::sendData(): sendto error result: " << result << std::endl;
        }
#endif
        return false;
    }
    return true;
}

// -------------------------------------------------------------
// recvData() -- Receive data from our connected TCP socket
// -------------------------------------------------------------
unsigned int TcpHandler::recvData(char* const packet, const int maxSize)
{
    if (!isConnected() || hasBeenTerminated()) return 0;

    if (socketNum == INVALID_SOCKET) return 0;

    // Try to receive the data
    unsigned int n = 0;
    int result = ::recvfrom(socketNum, packet, maxSize, 0, 0, 0);
    if (result > 0) n = (unsigned int) result;

    return n;
}

} // End Basic namespace
} // End Eaagles namespace

