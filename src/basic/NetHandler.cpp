//------------------------------------------------------------------------------
// Class: NetHandler
//------------------------------------------------------------------------------

#if defined(WIN32)
    #include <sys/types.h>
    #include <Winsock2.h>
    #define bzero(a,b)  ZeroMemory( a, b )
    typedef int Len;
#else
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <sys/fcntl.h>
    #include <sys/ioctl.h>
    #ifdef sun
        #include <sys/filio.h> // -- added for Solaris 10
    #endif
    static const int SOCKET_ERROR = -1;
    typedef socklen_t Len ;
#endif

#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// Class: NetHandler
//==============================================================================
IMPLEMENT_SUBCLASS(NetHandler, "NetHandler")

// Byte order
bool NetHandler::netByteOrder = checkByteOrder();

// Slot Table
BEGIN_SLOTTABLE(NetHandler)
    "localIpAddress",       // 1) String containing the local host's name or its IP
                            //    address in the Internet standard "." (dotted) notation.
                            //    (default: found via local host name)
    "localPort",            // 2) (optional) Local port number
    "port",                 // 3) Number of the port we're connecting to (required)
    "shared",               // 4) Shared (reuse) flag [default: false (not shared)]
    "sendBuffSizeKb",       // 5) Send buffer size in KB's    (default:  32 Kb; max 1024)
    "recvBuffSizeKb",       // 6) Receive buffer size in KB's (default: 128 Kb; max 1024)
    "ignoreSourcePort",     // 7) Ignore message from this source port
END_SLOTTABLE(NetHandler)

// Map slot table to handles
BEGIN_SLOT_MAP(NetHandler)
    ON_SLOT(1, setSlotLocalIpAddress, String)
    ON_SLOT(2, setSlotLocalPort,    Number)
    ON_SLOT(3, setSlotPort,         Number)
    ON_SLOT(4, setSlotShared,       Number)
    ON_SLOT(5, setSlotSendBuffSize, Number)
    ON_SLOT(6, setSlotRecvBuffSize, Number)
    ON_SLOT(7, setSlotIgnoreSourcePort, Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
NetHandler::NetHandler() :
               localIpAddr(0),
               localAddr(INADDR_ANY),
               netAddr(INADDR_ANY),
               fromAddr1(INADDR_NONE),
               port(0),
               localPort(0),
               ignoreSourcePort(0),
               fromPort1(0),
               sharedFlg(false),
               initialized(false),
               sendBuffSizeKb(32),
               recvBuffSizeKb(128)
{
   STANDARD_CONSTRUCTOR()

   #if defined(WIN32)
      socketNum = INVALID_SOCKET;
   #else
      socketNum = -1;
   #endif
}


//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void NetHandler::copyData(const NetHandler& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
      localIpAddr = 0;
    }

    port = org.port;
    localPort = org.localPort;
    ignoreSourcePort = org.ignoreSourcePort;
    sendBuffSizeKb = org.sendBuffSizeKb;
    recvBuffSizeKb = org.recvBuffSizeKb;
    setSharedFlag(org.getSharedFlag());
    netAddr = org.netAddr;
    localAddr = org.localAddr;
    initialized = org.initialized;

    if (localIpAddr != 0) delete[] localIpAddr;
    localIpAddr = 0;
    if (org.localIpAddr != 0) {
        size_t len = strlen(org.localIpAddr);
        localIpAddr = new char[len+1];
        lcStrcpy(localIpAddr,(len+1),org.localIpAddr);
    }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void NetHandler::deleteData()
{
   if (localIpAddr != 0) delete[] localIpAddr;
   localIpAddr = 0;
}

//------------------------------------------------------------------------------
// Initialize the network handler --
//------------------------------------------------------------------------------
bool NetHandler::initNetwork(const bool noWaitFlag)
{
    // Initialize socket
    bool ok =  init();
    if (ok) {
        // Bind and configure socket
        ok = bindSocket();
        if (ok) {
            if (noWaitFlag) {
                ok = setNoWait();
                if (!ok) std::cerr << "initNetwork(): setNoWait() FAILED" << std::endl;
            }
            else {
                ok = setBlocked();
                if (!ok) std::cerr << "initNetwork(): setBlocked() FAILED" << std::endl;
            }
        }
        else {
            std::cerr << "initNetwork(): bindSocket() FAILED" << std::endl;
        }
    }
    else {
        std::cerr << "initNetwork(): Init() FAILED" << std::endl;
    }

    initialized = ok;
    return ok;
}

//------------------------------------------------------------------------------
// init() -- initialize the network
//------------------------------------------------------------------------------
bool NetHandler::init()
{
   bool ok = true;
#if defined(WIN32)
   // Init Winsock2
   WSADATA wsaData;
   WORD wVersionRequested = MAKEWORD( 2, 2 );
   int err = ::WSAStartup( wVersionRequested, &wsaData );
   if (err != 0) {
      std::cerr << "NetHandler::init() -- WSAStartup() FAILED" << std::endl;
   }
   ok = (err == 0);
#endif

   // ---
   // Set the local IP address
   // ---
   if (localIpAddr != 0) {
      setLocalAddr(localIpAddr);
   }

   return ok;
}

// -------------------------------------------------------------
// bindSocket() -- bind the socket to an address, and configure
// the send and receive buffers.
// -------------------------------------------------------------
bool NetHandler::bindSocket()
{
#if defined(WIN32)
    if (socketNum == INVALID_SOCKET) return false;
#else
    if (socketNum < 0) return false;
#endif

    // ---
    // Make sure we have a local host IP address
    // ---
    if (localAddr == INADDR_ANY) {
        char localhost[256];
        int result = ::gethostname(localhost, sizeof(localhost));
        if (result == 0) {
            setLocalAddr(localhost);
        }
    }

    // ---
    // Set the reuse socket attribute
    // ---
    {
#if defined(WIN32)
        BOOL optval = getSharedFlag();
        Len optlen = sizeof(optval);
        if( ::setsockopt(socketNum, SOL_SOCKET, SO_REUSEADDR, (const char*) &optval, optlen) == SOCKET_ERROR)
#else
        int optval = getSharedFlag();
        Len optlen = sizeof(optval);
        if( ::setsockopt(socketNum, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) == SOCKET_ERROR)
#endif
        {
            ::perror("NetHandler::bindSocket(): error setsockopt(SO_REUSEADDR)\n");
            return false;
        }
    }

   return true;
}

//------------------------------------------------------------------------------
// Set the output buffer size
//------------------------------------------------------------------------------
bool NetHandler::setSendBuffSize()
{
#if defined(WIN32)
   if (socketNum == INVALID_SOCKET) return false;
#else
   if (socketNum < 0) return false;
#endif

   unsigned int optval = sendBuffSizeKb * 1024;
   Len optlen = sizeof(optval);
#if defined(WIN32)
   if( ::setsockopt(socketNum, SOL_SOCKET, SO_SNDBUF, (const char*) &optval, optlen) == SOCKET_ERROR )
#else
   if( ::setsockopt(socketNum, SOL_SOCKET, SO_SNDBUF, (void*) &optval, optlen) == SOCKET_ERROR )
#endif
   {
      ::perror("NetHandler::setSendBuffSize(): error setting the send buffer size\n");
      return false;
   }
   return true;
}

//------------------------------------------------------------------------------
// Sets the input buffer size
//------------------------------------------------------------------------------
bool NetHandler::setRecvBuffSize()
{
#if defined(WIN32)
   if (socketNum == INVALID_SOCKET) return false;
#else
   if (socketNum < 0) return false;
#endif

   unsigned int optval = recvBuffSizeKb * 1024;
   Len optlen = sizeof (optval);
#if defined(WIN32)
   if( ::setsockopt(socketNum, SOL_SOCKET, SO_RCVBUF, (const char*) &optval, optlen) == SOCKET_ERROR )
#else
   if( ::setsockopt(socketNum, SOL_SOCKET, SO_RCVBUF, (void*) &optval, optlen) == SOCKET_ERROR )
#endif
   {
      ::perror("NetHandler::setRecvBuffSize(): error setting the receive buffer size\n");
      return false;
   }
   return true;
}

//------------------------------------------------------------------------------
// toNet() -- byte swaps a host to network buffer.  The buffer MUST consist
//            of 'nl' int (4 byte) words followed by 'ns' short (2 byte) words.
//            The parameters 'nl' and 'ns' can be zero.
//------------------------------------------------------------------------------
void NetHandler::toNet(const void* const hostData, void* const netData, const int nl, const int ns)
{
   int i;

   // Compute pointers to the int word (4 byte) and short
   // short word (2 byte) areas of the source (this).
   const u_long* psl = (const u_long*) hostData;
   const u_short* pss = (const u_short*) (psl + nl);

   // Compute pointers to the int word (4 byte) and short
   // short word (2 byte) areas of the destination (netData).
   u_long* pdl = (u_long*) netData;
   u_short* pds = (u_short*) (pdl + nl);

   for (i = 0; i < nl; i++) {
      const u_long kk = *psl++;
      u_long ll = htonl(kk);
      *pdl++ = ll;
   }
   for (i = 0; i < ns; i++) {
      //*pds++ = htons(*pss++);
      const u_short kk = *pss++;
      u_short ss = htons(kk);
      *pds++ = ss;
   }
}

//------------------------------------------------------------------------------
// toHost() -- byte swaps a network to host buffer.  The buffer MUST consist
//            of 'nl' int (4 byte) words followed by 'ns' short (2 byte) words.
//            The parameters 'nl' and 'ns' can be zero.
//------------------------------------------------------------------------------
void NetHandler::toHost(const void* const netData, void* const hostData, const int nl, const int ns)
{
   int i;

   // Compute pointers to the int word (4 byte) and short
   // short word (2 byte) areas of the source (this).
   const u_long* psl = (const u_long*) netData;
   const u_short* pss = (const u_short*) (psl + nl);

   // Compute pointers to the int word (4 byte) and short
   // short word (2 byte) areas of the destination (hostData).
   u_long* pdl = (u_long*) hostData;
   u_short* pds = (u_short*) (pdl + nl);

   for (i = 0; i < nl; i++) {
      *pdl++ = ntohl(*psl++);
   }
   for (i = 0; i < ns; i++) {
      *pds++ = ntohs(*pss++);
   }
}

//------------------------------------------------------------------------------
// checkByteOrder() -- Checks byte order and returns true if in network byte order
//------------------------------------------------------------------------------
bool NetHandler::checkByteOrder()
{
    unsigned short n1 = 1;
    unsigned short n2 = htons(n1);
    return (n1 == n2);     // No difference? Then we already in network order!
}

// -------------------------------------------------------------
// setBlocked() -- Sets blocked I/O mode
// -------------------------------------------------------------
bool NetHandler::setBlocked(const LcSocket s)
{
    LcSocket sock = s;
    if (sock == NET_INVALID_SOCKET) sock = socketNum;
    if (sock == NET_INVALID_SOCKET) return false;

// Set the socket 'sock' to Blocking. Wait I/O.
#if defined(WIN32)
    unsigned long zz = false;
    if ( ::ioctlsocket(sock, FIONBIO, &zz ) == SOCKET_ERROR ) {
        ::perror("NetHandler::setBlocked()");
        return false;
    }
#else
    const int zz = 0;
    if ( ::ioctl(sock, FIONBIO, &zz ) == SOCKET_ERROR ) {
        ::perror("NetHandler::setBlocked()");
        return false;
    }
#endif

   return true;
}

// -------------------------------------------------------------
// setNoWait() -- Sets no wait (non-blocking) I/O mode
// -------------------------------------------------------------
bool NetHandler::setNoWait(const LcSocket s)
{
    LcSocket sock = s;
    if (sock == NET_INVALID_SOCKET) sock = socketNum;
    if (sock == NET_INVALID_SOCKET) return false;

// Set the socket 'sock' to Non-Blocking. Nowait I/O.
#if defined(WIN32)
    unsigned long zz = true;
    if ( ::ioctlsocket(sock, FIONBIO, &zz ) == SOCKET_ERROR ) {
        ::perror("NetHandler::setNoWait()");
        return false;
    }
#else
    const int zz = 1;
    if ( ::ioctl(sock, FIONBIO, &zz ) == SOCKET_ERROR ) {
        ::perror("NetHandler::setNoWait()");
        return false;
    }
#endif

   return true;
}

// -------------------------------------------------------------
// Returns true if the network handler has been initialized
// -------------------------------------------------------------
bool NetHandler::isConnected() const
{
    return initialized;
}

// -------------------------------------------------------------
// Close (un-initialize) this network
// -------------------------------------------------------------
bool NetHandler::closeConnection()
{
    initialized = false;
    return true;
}

// -------------------------------------------------------------
// sendData() -- Send data
// -------------------------------------------------------------
bool NetHandler::sendData(const char* const packet, const int size)
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
    addr.sin_addr.s_addr = netAddr;
    addr.sin_port = htons(port);
    Len addrlen = sizeof(addr);
    int result = ::sendto(socketNum, packet, size, 0, (const struct sockaddr *) &addr, addrlen);
#if defined(WIN32)
    if (result == SOCKET_ERROR) {
        int err = ::WSAGetLastError();
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "NetHandler::sendData(): sendto error: " << err << " hex=0x" << std::hex << err << std::dec << std::endl;
        }
        return false;
    }
#else
    if (result == SOCKET_ERROR) {
        ::perror("NetHandler::sendData(): sendto error msg");
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "NetHandler::sendData(): sendto error result: " << result << std::endl;
        }
        return false;
    }
#endif
    return true;
}


// -------------------------------------------------------------
// recvData() -- Receive data and possible ignore our own
//               local port messages.
// -------------------------------------------------------------
unsigned int NetHandler::recvData(char* const packet, const int maxSize)
{
   unsigned int n = 0;

#if defined(WIN32)
   if (socketNum == INVALID_SOCKET) return 0;
#else
   if (socketNum < 0) return 0;
#endif

   fromAddr1 = INADDR_NONE;
   fromPort1 = 0;

   bool tryAgain = true;
   while (tryAgain) {
      tryAgain = false;

      // Try to receive the data
      struct sockaddr_in raddr;       // IP address
      Len addrlen = sizeof(raddr);
      int result = ::recvfrom(socketNum, packet, maxSize, 0, (struct sockaddr *) &raddr, &addrlen);

      if (result > 0 && ignoreSourcePort != 0) {
         // Ok we have one; make sure it's not one we should ignore
         uint16_t rport = ntohs(raddr.sin_port);
         if (rport == ignoreSourcePort) {
            tryAgain = true;
         }
      }

      // set number of bytes received
      if (result > 0 && !tryAgain) {
         n = result;
         fromAddr1 = raddr.sin_addr.s_addr;
         fromPort1 = ntohs(raddr.sin_port);
      }
   }

   return n;
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Set the shared flag
void NetHandler::setSharedFlag(const bool b)
{
   sharedFlg = b;
}

// Set port number
bool NetHandler::setPort(const uint16_t n1)
{
   port = n1;
   return true;
}

// Set local (source) port number
bool NetHandler::setLocalPort(const uint16_t n1)
{
   localPort = n1;
   return true;
}

// Sets the network IP address
bool NetHandler::setNetAddr(const uint32_t addr0)
{
    bool ok = false;
    if (addr0 != INADDR_NONE) {
        netAddr = addr0;
        ok = true;
    }
    return ok;
}

// Sets the network IP address using hostname or the Internet standard "." (dotted) notation
bool NetHandler::setNetAddr(const char* const hostname)
{
    bool ok = false;
    if (hostname != 0) {
        uint32_t addr0 = INADDR_NONE;
        if (::isdigit(hostname[0])) {
            // If 'hostname' starts with a number then first try to use it as an IP address
            addr0 = ::inet_addr(hostname);
            ok = (addr0 != INADDR_NONE);
        }
        if (addr0 == INADDR_NONE) {
            // Didn't work, try to find the host IP address by name
            if (isMessageEnabled(MSG_DEBUG)) {
               std::cout << "setNetAddr(): Looking up host name: " << hostname;
            }
            const hostent* const p = gethostbyname(hostname);
            if (p != 0 && p->h_length > 0) {

                const unsigned int* const q = (unsigned int*) (p->h_addr_list[0]);
                if (q != 0) {
                    struct in_addr in;
                    in.s_addr = *q;
                    addr0 = in.s_addr;
                    const char* const ipAddr = ::inet_ntoa(in);
                    if (ipAddr != 0) {
                        if (isMessageEnabled(MSG_DEBUG)) {
                           std::cout << " -- IP Address: " << ipAddr << std::endl;
                        }
                        ok = true;
                    }
                }
            }
            if (!ok && isMessageEnabled(MSG_DEBUG)) {
                std::cout << " -- HOST NOT FOUND!" << std::endl;
            }
        }
        if (addr0 != INADDR_NONE) ok = setNetAddr(addr0);
    }
    return ok;
}

// Sets the local IP address
bool NetHandler::setLocalAddr(const uint32_t addr0)
{
    bool ok = false;
    if (addr0 != INADDR_NONE) {
        localAddr = addr0;
        ok = true;
    }
    return ok;
}

// Sets the local IP address using hostname or the Internet standard "." (dotted) notation
bool NetHandler::setLocalAddr(const char* const hostname)
{
    bool ok = false;
    if (hostname != 0) {
        uint32_t addr0 = INADDR_NONE;
        if (isdigit(hostname[0])) {
            // If 'hostname' starts with a number then first try to use it as an IP address
            addr0 = ::inet_addr(hostname);
            ok = (addr0 != INADDR_NONE);
        }
        if (addr0 == INADDR_NONE) {
            // Didn't work, try to find the host IP address by name
            if (isMessageEnabled(MSG_DEBUG)) {
                std::cout << "setLocalAddr(): Looking up host name: " << hostname;
            }
            const hostent* p = gethostbyname(hostname);
            if (p != 0 && p->h_length > 0) {
                const unsigned int* const q = (unsigned int*) (p->h_addr_list[0]);
                if (q != 0) {
                    struct in_addr in;
                    in.s_addr = *q;
                    addr0 = in.s_addr;

                    char* ipAddr = ::inet_ntoa(in);
                    if (ipAddr != 0) {
                        if (isMessageEnabled(MSG_DEBUG)) {
                           std::cout << " -- IP Address: " << ipAddr << std::endl;
                        }
                        ok = true;
                    }
                }
            }
            if (!ok && isMessageEnabled(MSG_DEBUG)) {
                std::cout << " -- HOST NOT FOUND!" << std::endl;
            }
        }
        if (addr0 != INADDR_NONE) ok = setLocalAddr(addr0);
    }
    return ok;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// localIpAddress: String containing the local IP address
bool NetHandler::setSlotLocalIpAddress(const String* const msg)
{
    bool ok = false;
    if (msg != 0) {
        if (localIpAddr != 0) delete[] localIpAddr;
        localIpAddr = msg->getCopyString();
        ok = true;
    }
    return ok;
}

// port: Port number
bool NetHandler::setSlotPort(const Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        int ii = msg->getInt();
        if (ii >= 0x0 && ii <= 0xffff) {
            ok = setPort( (uint16_t) ii );
        }
    }
    return ok;
}

// localPort: Local (source) port number
bool NetHandler::setSlotLocalPort(const Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        int ii = msg->getInt();
        if (ii >= 0x0 && ii <= 0xffff) {
            ok = setLocalPort( (uint16_t) ii );
        }
    }
    return ok;
}

// shared: Reuse the port
bool NetHandler::setSlotShared(const Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        setSharedFlag( msg->getBoolean() );
        ok = true;
    }
    return ok;
}

// sendBuffSizeKb: Send buffer size in KB's    (default:  32 Kb)
bool NetHandler::setSlotSendBuffSize(const Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        int ii = msg->getInt();
        if (ii >= 0 && ii <= 1024) {
           sendBuffSizeKb = ii;
           ok = true;
        }
    }
    return ok;
}

// recvBuffSizeKb: Receive buffer size in KB's (default: 128 Kb)
bool NetHandler::setSlotRecvBuffSize(const Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        int ii = msg->getInt();
        if (ii >= 0 && ii <= 1024) {
           recvBuffSizeKb = ii;
           ok = true;
        }
    }
    return ok;
}

// setSlotIgnoreSourcePort: Ignore message from our this source port number
bool NetHandler::setSlotIgnoreSourcePort(const Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        int ii = msg->getInt();
        if (ii >= 0x0 && ii <= 0xffff) {
            ignoreSourcePort = uint16_t(ii);
            ok = true;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* NetHandler::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& NetHandler::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    if (port != 0) {
        indent(sout,i+j);
        sout << "port: ";
        sout << port;
        sout << std::endl;
    }

    if (localPort != 0) {
        indent(sout,i+j);
        sout << "localPort: ";
        sout << localPort;
        sout << std::endl;
    }

    indent(sout,i+j);
    sout << "shared: ";
    sout << getSharedFlag();
    sout << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Basic namespace
} // End Eaagles namespace

