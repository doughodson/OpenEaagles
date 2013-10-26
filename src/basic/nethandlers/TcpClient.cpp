//------------------------------------------------------------------------------
// Class: TcpClient
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

#include "openeaagles/basic/nethandlers/TcpClient.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(TcpClient, "TcpClient")

BEGIN_SLOTTABLE(TcpClient)
    "ipAddress",    // 1) String containing the IP address in
                    //    the Internet standard "." (dotted) notation.
END_SLOTTABLE(TcpClient)

// Map slot table to handles 
BEGIN_SLOT_MAP(TcpClient)
    ON_SLOT(1,setSlotIpAddress,String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
TcpClient::TcpClient() : ipAddr(0)
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TcpClient::copyData(const TcpClient& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
      ipAddr = 0;
    }

    if (ipAddr != 0) delete[] ipAddr;
    ipAddr = 0;
    if (org.ipAddr != 0) {
        size_t len = strlen(org.ipAddr);
        ipAddr = new char[len+1];
        lcStrcpy(ipAddr,(len+1),org.ipAddr);
    }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void TcpClient::deleteData()
{
    if (ipAddr != 0) delete[] ipAddr;
    ipAddr = 0;
}

//------------------------------------------------------------------------------
// Initialize this multicast handler -- 
//------------------------------------------------------------------------------
bool TcpClient::initNetwork(const bool noWaitFlag)
{
    noWait = noWaitFlag;
    bool ok = BaseClass::initNetwork(false);
    if (ok) {
        ok = connectToServer();
        if (!ok && isMessageEnabled(MSG_ERROR)) {
           std::cerr << "initNetwork(): connectToServer() FAILED" << std::endl;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// init() -- init the network, the socket and the network address
//------------------------------------------------------------------------------
bool TcpClient::init()
{
   // Init the base class
   bool success = BaseClass::init();
   if (!success) return false;

   // Clients use the same socket number
   tcpSocket = socketNum;
      
   // Find our network address
   success = setNetAddr(ipAddr);

   return success;
}

//------------------------------------------------------------------------------
// connectToServer() -- attempt to connect to the server
//------------------------------------------------------------------------------
bool TcpClient::connectToServer()
{
   connected = false;
   connectionTerminated = false;

   if (ipAddr == 0) return false;

#if defined(WIN32)
   if (tcpSocket == INVALID_SOCKET) return false;
#else
   if (tcpSocket < 0) return false;
#endif

   struct sockaddr_in addr;        // Working address structure
   bzero(&addr, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = getNetAddr();
   addr.sin_port = htons(getPort()); 

   if (isMessageEnabled(MSG_INFO)) {
      std::cout << "Connecting to TCP server at " << ipAddr << ":" << getPort() << " ... " << std::flush;
   }

#if defined(WIN32)
   if( connect(tcpSocket, (const struct sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR)
#else
   if( connect(tcpSocket, (const struct sockaddr *) &addr, sizeof(addr)) < 0)
#endif
   {
      if (isMessageEnabled(MSG_INFO)) {
          std::cout << "Failed!" << std::endl;
      }
   }
   else {
      if (isMessageEnabled(MSG_INFO)) {
          std::cout << "Connected!" << std::endl;
      }
      connected = true;
   }
   if (isMessageEnabled(MSG_INFO)) {
      std::cout << "TcpClient::connectToServer: socketNum = " << socketNum << ", tcpSocket = " << tcpSocket << std::endl;
   }

   // Set blocked or no-wait
   if (noWait) setNoWait(tcpSocket);
   else setBlocked(tcpSocket);

   return connected;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// ipAddress: String containing the IP address
bool TcpClient::setSlotIpAddress(const String* const msg)
{
    bool ok = false;
    if (msg != 0) { 
        ipAddr = msg->getCopyString(); 
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* TcpClient::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& TcpClient::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    if (ipAddr != 0) {
        indent(sout,i+j);
        sout << "ipAddress: \"";
        sout << ipAddr;
        sout << "\"" << std::endl;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Basic namespace
} // End Eaagles namespace

