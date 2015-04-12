//------------------------------------------------------------------------------
// Class: UdpMulticastHandler
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

#include "openeaagles/basic/nethandlers/UdpMulticastHandler.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include <cstdio>
#include <cstring>

namespace Eaagles {
namespace Basic {

//==============================================================================
// Class: UdpMulticastHandler
//==============================================================================
IMPLEMENT_SUBCLASS(UdpMulticastHandler, "UdpMulticastHandler")

BEGIN_SLOTTABLE(UdpMulticastHandler)

    "multicastGroup",           // 1) String containing the multicast IP address in
                                //    the Internet standard "." (dotted) notation.
                                //    IP multicast addresses range from 224.0.0.0
                                //    through 239.255.255.255 (e.g., "225.0.0.251")

    "ttl",                      // 2) Multicast Time-To-Live (TTL) value; default: 1

    "loopback",                 // 3) Multicast Loopback flag; default: 1 (on)

END_SLOTTABLE(UdpMulticastHandler)

// Map slot table to handles
BEGIN_SLOT_MAP(UdpMulticastHandler)
    ON_SLOT(1,setSlotMulticastGroup,String)
    ON_SLOT(2,setSlotTTL,Number)
    ON_SLOT(3,setSlotLoopback,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
UdpMulticastHandler::UdpMulticastHandler() : multicastGroup(0)
{
    STANDARD_CONSTRUCTOR()
    setTTL(1);
    setLoopback(true);
    initialized = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void UdpMulticastHandler::copyData(const UdpMulticastHandler& org, const bool)
{
    BaseClass::copyData(org);

    // Multicast Stuff
    multicastGroup = 0;
    if (org.multicastGroup != 0) {
        size_t len = std::strlen(org.multicastGroup);
        multicastGroup = new char[len+1];
        lcStrcpy(multicastGroup,(len+1),org.multicastGroup);
    }
    setTTL(org.getTTL());
    setLoopback(org.getLoopback());
    initialized = org.initialized;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void UdpMulticastHandler::deleteData()
{
    if (multicastGroup != 0) delete[] multicastGroup;
    multicastGroup = 0;
}

//------------------------------------------------------------------------------
// Initialize this multicast handler --
//------------------------------------------------------------------------------
bool UdpMulticastHandler::initNetwork(const bool noWaitFlag)
{
    bool ok = BaseClass::initNetwork(noWaitFlag);
    if (ok) {
        ok = joinTheGroup();
        if (!ok) std::cerr << "initNetwork(): joinTheGroup() FAILED" << std::endl;
    }
    initialized = ok;
    return ok;
}

//------------------------------------------------------------------------------
// init() -- init the network, the socket and the network address
//------------------------------------------------------------------------------
bool UdpMulticastHandler::init()
{
    // ---
    // Init the base class
    // ---
    bool ok = BaseClass::init();
    if (!ok) return false;

    // ---
    // Create our socket
    // ---
    socketNum = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socketNum == INVALID_SOCKET) {
        std::perror("UdpMulticastHandler::init(): socket error");
        return false;
    }

    // ---
    // Set the Multicast loopback attribute
    // ---
    {
#if defined(WIN32)
        BOOL optval = getLoopback();
        if (::setsockopt(socketNum, IPPROTO_IP, IP_MULTICAST_LOOP, (const char*) &optval, sizeof(optval)) == SOCKET_ERROR) {
#else
        int optval = getLoopback();
        if (::setsockopt(socketNum, IPPROTO_IP, IP_MULTICAST_LOOP, &optval, sizeof(optval)) == SOCKET_ERROR) {
#endif
            std::perror("UdpMulticastHandler::init(): error setsockopt(IP_MULTICAST_LOOP)\n");
            return false;
        }
    }

    // ---
    // Set the Multicast Time-To-Live (TTL) attribute
    // ---
    {
#if defined(WIN32)
        int optval = getTTL();
        if (::setsockopt(socketNum, IPPROTO_IP, IP_MULTICAST_TTL, (const char*) &optval, sizeof(optval)) == SOCKET_ERROR) {
#else
        int optval = getTTL();
        if (::setsockopt(socketNum, IPPROTO_IP, IP_MULTICAST_TTL, &optval, sizeof(optval)) == SOCKET_ERROR) {
#endif
            std::perror("UdpMulticastHandler::init(): error setsockopt(IP_MULTICAST_TTL)\n");
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------
// bindSocket() -- bind the socket to an address, and configure
// the send and receive buffers.
// -------------------------------------------------------------
bool UdpMulticastHandler::bindSocket()
{
    // Must have a group
    if (multicastGroup == 0) return false;

    // ---
    // Our base class will bind the socket
    // ---
    bool ok = BaseClass::bindSocket();

    if (ok) {
       struct sockaddr_in addr;        // Working address structure
       bzero(&addr, sizeof(addr));
       addr.sin_family = AF_INET;
#if defined(WIN32)
       addr.sin_addr.s_addr = INADDR_ANY;
#else
       addr.sin_addr.s_addr = ::inet_addr(multicastGroup);
#endif
       if (getLocalPort() != 0) addr.sin_port = htons (getLocalPort());
       else addr.sin_port = htons(getPort());

       if (::bind(socketNum, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
           std::perror("UdpMulticastHandler::bindSocket(): bind error");
           return false;
       }

       if (!setSendBuffSize()) return false;

       if (!setRecvBuffSize()) return false;
   }

   return ok;
}


// -------------------------------------------------------------
// joinTheGroup() -- Join the multicast group
// -------------------------------------------------------------
bool UdpMulticastHandler::joinTheGroup()
{
   if (socketNum == INVALID_SOCKET) return false;

   // Find our network address
   uint32_t mg = htonl (INADDR_NONE);
   if (multicastGroup != 0) mg = ::inet_addr(multicastGroup);
   if (mg != INADDR_NONE) {
      setNetAddr(mg);
   }
   else {
      std::cerr << "joinTheGroup() -- invalid multicast group" << std::endl;
      return false;
   }

   // Use our local IP address to select which interface to use
   uint32_t iface = getLocalAddr();

   // Set the socket option to add membership to the multicast group
   struct ip_mreq mreq;
   mreq.imr_multiaddr.s_addr = getNetAddr();
   mreq.imr_interface.s_addr = iface;
   int result = ::setsockopt(socketNum, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<const char*>(&mreq), sizeof(mreq));
   if (result == SOCKET_ERROR) {
      std::perror("UdpMulticastHandler::joinTheGroup(): setsockopt mreq");
      return false;
   }

   return true;
}

// -------------------------------------------------------------
// Returns true if the network handler has been initialized
// -------------------------------------------------------------
bool UdpMulticastHandler::isConnected() const
{
    return initialized && BaseClass::isConnected();
}

// -------------------------------------------------------------
// Close (un-initialize) this network
// -------------------------------------------------------------
bool UdpMulticastHandler::closeConnection()
{
    initialized = false;
    BaseClass::closeConnection();
    return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// multicastGroup: String containing the multicast IP address
bool UdpMulticastHandler::setSlotMulticastGroup(const String* const msg)
{
    bool ok = false;
    if (msg != 0) {
        multicastGroup = msg->getCopyString();
        ok = true;
    }
    return ok;
}

// ttl: Time-To-Live value
bool UdpMulticastHandler::setSlotTTL(const Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        setTTL( msg->getInt() );
        ok = true;
    }
    return ok;
}

// loopback: Loopback flag
bool UdpMulticastHandler::setSlotLoopback(const Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        setLoopback( msg->getBoolean() );
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* UdpMulticastHandler::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& UdpMulticastHandler::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if (!slotsOnly) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    if (multicastGroup != 0) {
        indent(sout,i+j);
        sout << "multicastGroup: \"";
        sout << multicastGroup;
        sout << "\"" << std::endl;
    }

    indent(sout,i+j);
    sout << "ttl: " << getTTL() << std::endl;

    indent(sout,i+j);
    sout << "loopback: " << getLoopback() << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if (!slotsOnly) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Basic namespace
} // End Eaagles namespace

