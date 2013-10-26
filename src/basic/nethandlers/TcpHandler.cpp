//------------------------------------------------------------------------------
// Class: TcpHandler
//------------------------------------------------------------------------------

#if defined(WIN32)
    #include <sys/types.h>
    #include <Winsock2.h>
    #include <WS2tcpip.h>
    #define  bzero(a,b)   ZeroMemory(a,b)
#else
    #include <arpa/inet.h>
    #include <sys/fcntl.h>
    #include <sys/ioctl.h>
    #ifdef sun
        #include <sys/filio.h> // -- added for Solaris 10
    #endif
    static const int SOCKET_ERROR = -1;
#endif

#include "openeaagles/basic/nethandlers/TcpHandler.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// Class: TcpHandler
//==============================================================================
IMPLEMENT_SUBCLASS(TcpHandler,"TcpHandler")
EMPTY_SLOTTABLE(TcpHandler)
EMPTY_SERIALIZER(TcpHandler)

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
TcpHandler::TcpHandler()
{
   STANDARD_CONSTRUCTOR()

   connected = false;
   connectionTerminated = false;
   noWait = false;

   #if defined(WIN32)
      tcpSocket = INVALID_SOCKET;
   #else
      tcpSocket = -1;
   #endif
}

TcpHandler::TcpHandler(const LcSocket sn)
{
   STANDARD_CONSTRUCTOR()

   connected = true;
   connectionTerminated = false;
   noWait = false;
   socketNum = sn;
   tcpSocket = sn;
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
   tcpSocket = org.tcpSocket;
   noWait = org.noWait;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void TcpHandler::deleteData()
{
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
    socketNum = socket(AF_INET, SOCK_STREAM, 0);
#if defined(WIN32)
    if (socketNum == INVALID_SOCKET) {
#else
    if (socketNum < 0) {
#endif
        perror("TcpHandler::init(): socket error");
        success = false;
    }

    return success;
}


// -------------------------------------------------------------
// bindSocket() -- bind the socket to an address, and configure
// the send and receive buffers. 
// -------------------------------------------------------------
bool TcpHandler::bindSocket()
{
    // ---
    // Our base class will bind the socket
    // ---
    bool ok = BaseClass::bindSocket();

    if (ok) {
       struct sockaddr_in addr;        // Working address structure
       bzero(&addr, sizeof(addr));
       addr.sin_family = AF_INET;
       addr.sin_addr.s_addr = getLocalAddr();
       if (getLocalPort() != 0) {
           addr.sin_port = htons (getLocalPort());  
       }
       else {
           addr.sin_port = htons(getPort());
       }

      if (bind(socketNum, (const struct sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR) {
        perror("TcpHandler::bindSocket(): bind error");
        return false;
      }

      if (!setSendBuffSize()) return false;
   
      if (!setRecvBuffSize()) return false;
   }

   return ok;
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
    if( closesocket(tcpSocket) == SOCKET_ERROR)
#else
    if( shutdown(tcpSocket, SHUT_RDWR) < 0)
#endif
    {
        perror("TcpHandler::closeConnection(): error! \n");
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

#if defined(WIN32)
    if (tcpSocket == INVALID_SOCKET) return false;
#else
    if (tcpSocket < 0) return false;
#endif

    int result = send(tcpSocket, packet, size, 0);

#if defined(WIN32)
    if (result == SOCKET_ERROR) {
        connected = false;
        connectionTerminated = true;
        int err = WSAGetLastError();
        if (isMessageEnabled(MSG_ERROR)) {
           std::cerr << "TcpHandler::sendData(): sendto error: " << err << " hex=0x" << std::hex << err << std::dec << std::endl;
        }
        return false;
    }
#else
    if (result < 0) {
        connected = false;
        connectionTerminated = true;
        perror("TcpHandler::sendData(): sendto error msg");
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "TcpHandler::sendData(): sendto error result: " << result << std::endl;
        }
        return false;
    }
#endif
    return true;
}

// -------------------------------------------------------------
// recvData() -- Receive data from our connected TCP socket
// -------------------------------------------------------------
unsigned int TcpHandler::recvData(char* const packet, const int maxSize)
{
   if (!isConnected() || hasBeenTerminated()) return 0;

#if defined(WIN32)
    if (tcpSocket == INVALID_SOCKET) return 0;
#else
    if (tcpSocket < 0) return 0;
#endif

   // Try to receive the data
   unsigned int n = 0;
   int result = recvfrom(tcpSocket, packet, maxSize, 0, 0, 0);
   if (result > 0) n = (unsigned int) result;

   return n;
}

} // End Basic namespace
} // End Eaagles namespace

