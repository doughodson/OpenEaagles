//------------------------------------------------------------------------------
// Class: TcpServerSingle
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

#include "openeaagles/basic/nethandlers/TcpServerSingle.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

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

//------------------------------------------------------------------------------
// listenForConnections() -- puts the socket into listen mode
//------------------------------------------------------------------------------
bool TcpServerSingle::listenForConnections()
{
#if defined(WIN32)
    if (socketNum == INVALID_SOCKET) return false;
    if( listen(socketNum, 1) == SOCKET_ERROR)
#else
    if (socketNum < 0) return false;
    if( listen(socketNum, 1) < 0)
#endif
    {
        perror("TcpServerSingle::listenForConnections(): error! \n");
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
// listenForConnections() -- puts the socket into listen mode
//------------------------------------------------------------------------------
bool TcpServerSingle::acceptConnection()
{
   struct sockaddr_in clientAddr;

#if defined(WIN32)
   if (socketNum == INVALID_SOCKET) return 0;
   int cAddrSize = sizeof(clientAddr);
   if (isMessageEnabled(MSG_INFO)) {
       std::cout << "Waiting to accept connection on " << getPort() << " ... " << std::flush;
   }
   tcpSocket = accept(socketNum, (struct sockaddr *) &clientAddr, &cAddrSize);
   if(tcpSocket == INVALID_SOCKET) {
      if (isMessageEnabled(MSG_INFO)) {
          std::cout << " failed!" << std::endl;
      }
      return false;
   }
#else
   if (socketNum < 0) return 0;
   socklen_t cAddrSize = sizeof(clientAddr);
   if (isMessageEnabled(MSG_INFO)) {
       std::cout << "Waiting to accept connection on " << getPort() << " ... " << std::flush;
   }
   tcpSocket = accept(socketNum, (struct sockaddr *) &clientAddr, &cAddrSize);
   if(tcpSocket  < 0) {
      if (isMessageEnabled(MSG_INFO)) {
          std::cout << " failed!" << std::endl;
      }
      return false;
   }
#endif
   if (isMessageEnabled(MSG_INFO)) {
       std::cout << "Accepted";
       char* ip = inet_ntoa(clientAddr.sin_addr);
       if (ip != 0) {
           std::cout << " connection from " << ip;
       }
       std::cout << std::endl;
   }

   connected = true;
   connectionTerminated = false;

   // Set blocked or no-wait
   if (noWait) setNoWait(tcpSocket);
   else setBlocked(tcpSocket);

   if (isMessageEnabled(MSG_INFO)) {
       std::cout << "TcpServerSingle::acceptConnection: socketNum = " << socketNum << ", tcpSocket = " << tcpSocket << std::endl;
   }

   return true;
}

} // End Basic namespace
} // End Eaagles namespace

