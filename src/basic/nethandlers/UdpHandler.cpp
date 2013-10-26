//------------------------------------------------------------------------------
// Class: UdpHandler
//------------------------------------------------------------------------------

#if defined(WIN32)
    #include <sys/types.h>
    #include <winsock2.h>
    #define bzero(a,b)    ZeroMemory( a, b )
    typedef int Len;
#else
    #include <arpa/inet.h>
    #include <sys/fcntl.h>
    #include <sys/ioctl.h>
    #ifdef sun
        #include <sys/filio.h> // -- added for Solaris 10
    #endif
    static const int SOCKET_ERROR = -1;
    typedef socklen_t Len ;
#endif

#include "openeaagles/basic/nethandlers/UdpHandler.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// Class: UdpHandler
//==============================================================================
IMPLEMENT_SUBCLASS(UdpHandler,"UdpHandler")

BEGIN_SLOTTABLE(UdpHandler)
    "ipAddress",        // 1) String containing the IP address in
                        //    the Internet standard "." (dotted) notation.
END_SLOTTABLE(UdpHandler)

// Map slot table to handles 
BEGIN_SLOT_MAP(UdpHandler)
    ON_SLOT(1,setSlotIpAddress,String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
UdpHandler::UdpHandler() : ipAddr(0)
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void UdpHandler::copyData(const UdpHandler& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        ipAddr = 0;
    }  

    // IP Address
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
void UdpHandler::deleteData()
{
    if (ipAddr != 0) delete[] ipAddr;
    ipAddr = 0;
}

//------------------------------------------------------------------------------
// init() -- init the network, the socket and the network address
//------------------------------------------------------------------------------
bool UdpHandler::init()
{
    // ---
    // Init the base class
    // ---
    bool ok = BaseClass::init();
    if (!ok) {
        return false;
    }

    // ---
    // Find our network address
    // ---
    setNetAddr(ipAddr);

    // ---
    // Create our socket
    // ---
    socketNum = socket(AF_INET, SOCK_DGRAM, 0);
#if defined(WIN32)
    if (socketNum == INVALID_SOCKET) {
#else
    if (socketNum < 0) {
#endif
        perror("UdpHandler::init(): Socket error");
        return false;
    }

    return true;
}

// -------------------------------------------------------------
// bindSocket() -- bind the socket to an address, and configure
// the send and receive buffers. 
// -------------------------------------------------------------
bool UdpHandler::bindSocket()
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
        perror("UdpHandler::bindSocket(): bind error");
        return false;
      }

      if (!setSendBuffSize()) return false;
   
      if (!setRecvBuffSize()) return false;
   }

   return ok;
}

// -------------------------------------------------------------
// Send data to a specific IP/Port
// -------------------------------------------------------------
bool UdpHandler::sendDataTo(
         const char* const packet,  // Data packet
         const int size,            // Size of the data packet
         const uint32_t ip0,        // Destination IP address (this packet only)
         const uint16_t port0       // Destination port (this packet only)
      )
{
#if defined(WIN32)
    if (socketNum == INVALID_SOCKET) return false;
#else
    if (socketNum < 0) return false;
#endif

    // Send the data
    struct sockaddr_in addr;        // Working address structure
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip0;
    addr.sin_port = htons(port0); 
    Len addrlen = sizeof(addr);
    int result = sendto(socketNum, packet, size, 0, (const struct sockaddr *) &addr, addrlen);
#if defined(WIN32)
    if (result == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "UdpHandler::sendDataTo(): sendto error: " << err << " hex=0x" << std::hex << err << std::dec << std::endl;
        }
        return false;
    }
#else
    if (result == SOCKET_ERROR) {
        perror("UdpHandler::sendDataTo(): sendto error msg");
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "UdpHandler::sendDataTo(): sendto error result: " << result << std::endl;
        }
        return false;
    }
#endif
    return true;
}


//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// ipAddress: String containing the IP address
bool UdpHandler::setSlotIpAddress(const String* const msg)
{
    bool ok = false;
    if (msg != 0) { 
        if (ipAddr != 0) delete[] ipAddr;
        ipAddr = msg->getCopyString(); 
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* UdpHandler::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& UdpHandler::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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

