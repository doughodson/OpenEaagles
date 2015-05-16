//------------------------------------------------------------------------------
// Class: TcpServerSingle
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

#include "openeaagles/basic/nethandlers/TcpServerSingle.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

#include <cstdio>

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(TcpServerSingle, "TcpServerSingle")
EMPTY_SLOTTABLE(TcpServerSingle)
EMPTY_SERIALIZER(TcpServerSingle)

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
TcpServerSingle::TcpServerSingle()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TcpServerSingle::copyData(const TcpServerSingle& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void TcpServerSingle::deleteData()
{
}

//------------------------------------------------------------------------------
// Initialize this multicast handler --
//------------------------------------------------------------------------------
bool TcpServerSingle::initNetwork(const bool noWaitFlag)
{
    noWait = noWaitFlag;
    bool ok = BaseClass::initNetwork(false);
    if (ok) {
        ok = listenForConnections();
        if (ok) {
           ok = acceptConnection();
        }
        else if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "initNetwork(): listenForConnections() FAILED" << std::endl;
        }
    }
    return ok;
}

// -------------------------------------------------------------
// bindSocket() -- bind the socket to an address, and configure
// the send and receive buffers.
// -------------------------------------------------------------
bool TcpServerSingle::bindSocket()
{
   // ---
   // Our base class will bind the socket
   // ---
   bool ok = BaseClass::bindSocket();

   if (ok) {
      struct sockaddr_in addr;        // Working address structure
      bzero(&addr, sizeof(addr));
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = INADDR_ANY;
      if (getLocalAddr() != 0) addr.sin_addr.s_addr = getLocalAddr ();
      if (getLocalPort() != 0) addr.sin_port = htons (getLocalPort());
      else addr.sin_port = htons(getPort());

      // Only in server do we bind
      if (::bind(socketNum, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR ) {
         std::perror("TcpHandler::bindSocket(): bind error");
         return false;
      }

      if (!setSendBuffSize()) return false;

      if (!setRecvBuffSize()) return false;
   }

   return ok;
}

//------------------------------------------------------------------------------
// listenForConnections() -- puts the socket into listen mode
//------------------------------------------------------------------------------
bool TcpServerSingle::listenForConnections()
{
    if (socketNum == INVALID_SOCKET) return false;
    if (::listen(socketNum, 1) == SOCKET_ERROR) {
        std::perror("TcpServerSingle::listenForConnections(): error! \n");
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
// listenForConnections() -- puts the socket into listen mode
//------------------------------------------------------------------------------
bool TcpServerSingle::acceptConnection()
{
   if (socketNum == INVALID_SOCKET) return 0;

   struct sockaddr_in clientAddr;
   socklen_t cAddrSize = sizeof(clientAddr);
   if (isMessageEnabled(MSG_INFO)) {
       std::cout << "Waiting to accept connection on " << getPort() << " ... " << std::flush;
   }
   LcSocket tcpSocket = ::accept(socketNum, reinterpret_cast<struct sockaddr*>(&clientAddr), &cAddrSize);
   if (tcpSocket == INVALID_SOCKET) {
      if (isMessageEnabled(MSG_INFO)) {
          std::cout << " failed!" << std::endl;
      }
      return false;
   }

   if (isMessageEnabled(MSG_INFO)) {
       std::cout << "Accepted";
       char* ip = ::inet_ntoa(clientAddr.sin_addr);
       if (ip != nullptr) {
           std::cout << " connection from " << ip;
       }
       std::cout << std::endl;
   }

   // After accepting a connection we close the original opened socket and
   // we then assign socketNum to our local tcpSocket.
#if defined(WIN32)
   if (::closesocket(socketNum) == SOCKET_ERROR) {
#else
   if (::shutdown(socketNum, SHUT_RDWR) == SOCKET_ERROR) {
#endif
      std::perror("TcpServerSingle::acceptConnection(): shutdown original error! \n");
   }

   socketNum = tcpSocket;
   connected = true;
   connectionTerminated = false;

   // Set blocked or no-wait
   if (noWait) setNoWait();
   else setBlocked();

   if (isMessageEnabled(MSG_INFO)) {
       std::cout << "TcpServerSingle::acceptConnection: new socketNum = " << socketNum << std::endl;
   }

   return true;
}

} // End Basic namespace
} // End Eaagles namespace

