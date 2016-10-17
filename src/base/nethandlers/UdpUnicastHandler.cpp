//------------------------------------------------------------------------------
// Class: UdpUnicastHandler
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
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #include <sys/types.h>
    #include <winsock2.h>
    #define bzero(a,b)    ZeroMemory( a, b )
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

#include "openeaagles/base/nethandlers/UdpUnicastHandler.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/String.hpp"
#include <cstdio>
#include <cstring>

namespace oe {
namespace base {

//==============================================================================
// Class: UdpUnicastHandler
//==============================================================================
IMPLEMENT_SUBCLASS(UdpUnicastHandler, "UdpUnicastHandler")

BEGIN_SLOTTABLE(UdpUnicastHandler)
    "ipAddress",        // 1) String containing the IP address in
                        //    the Internet standard "." (dotted) notation.
END_SLOTTABLE(UdpUnicastHandler)

// Map slot table to handles
BEGIN_SLOT_MAP(UdpUnicastHandler)
    ON_SLOT(1,setSlotIpAddress,String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
UdpUnicastHandler::UdpUnicastHandler() : ipAddr(nullptr)
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void UdpUnicastHandler::copyData(const UdpUnicastHandler& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        ipAddr = nullptr;
    }

    // IP Address
    if (ipAddr != nullptr) delete[] ipAddr;
    ipAddr = nullptr;
    if (org.ipAddr != nullptr) {
        size_t len = std::strlen(org.ipAddr);
        ipAddr = new char[len+1];
        utStrcpy(ipAddr,(len+1), org.ipAddr);
    }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void UdpUnicastHandler::deleteData()
{
    if (ipAddr != nullptr) delete[] ipAddr;
    ipAddr = nullptr;
}

//------------------------------------------------------------------------------
// init() -- init the network, the socket and the network address
//------------------------------------------------------------------------------
bool UdpUnicastHandler::init()
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
    socketNum = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socketNum == INVALID_SOCKET) {
        std::perror("UdpUnicastHandler::init(): Socket error");
        return false;
    }

    return true;
}

// -------------------------------------------------------------
// bindSocket() -- bind the socket to an address, and configure
// the send and receive buffers.
// -------------------------------------------------------------
bool UdpUnicastHandler::bindSocket()
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
       if (getLocalPort() != 0) addr.sin_port = htons (getLocalPort());
       else addr.sin_port = htons(getPort());

       if (::bind(socketNum, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
           std::perror("UdpUnicastHandler::bindSocket(): bind error");
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
bool UdpUnicastHandler::sendDataTo(
         const char* const packet,  // Data packet
         const int size,            // Size of the data packet
         const uint32_t ip0,        // Destination IP address (this packet only)
         const uint16_t port0       // Destination port (this packet only)
      )
{
    if (socketNum == INVALID_SOCKET) return false;

    // Send the data
    struct sockaddr_in addr;        // Working address structure
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip0;
    addr.sin_port = htons(port0);
    socklen_t addrlen = sizeof(addr);
    int result = ::sendto(socketNum, packet, size, 0, reinterpret_cast<const struct sockaddr*>(&addr), addrlen);
    if (result == SOCKET_ERROR) {
#if defined(WIN32)
        int err = WSAGetLastError();
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "UdpUnicastHandler::sendDataTo(): sendto error: " << err << " hex=0x" << std::hex << err << std::dec << std::endl;
        }
#else
        std::perror("UdpHandler::sendDataTo(): sendto error msg");
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "UdpUnicastHandler::sendDataTo(): sendto error result: " << result << std::endl;
        }
#endif
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// ipAddress: String containing the IP address
bool UdpUnicastHandler::setSlotIpAddress(const String* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        if (ipAddr != nullptr) delete[] ipAddr;
        ipAddr = msg->getCopyString();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* UdpUnicastHandler::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& UdpUnicastHandler::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    if (ipAddr != nullptr) {
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

}
}

