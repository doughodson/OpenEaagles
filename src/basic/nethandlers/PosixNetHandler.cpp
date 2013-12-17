//------------------------------------------------------------------------------
// Class: PosixNetHandler
//------------------------------------------------------------------------------
//
// M$ WinSock has slightly different return types, some different calling, and
// is missing some of the calls that are standard in Berkely and POSIX socket
// implementation.  These slight differences will be handled in setting basic
// typedefs, defines, and constants that will make each convention match for
// use later in the code.  This will save a lot of pre-processor intervention
// and make the code that much more enjoyable to read!
//
#if defined(WIN32)
    #include <sys/types.h>
    #include <Winsock2.h>
    #define bzero(a,b)  ZeroMemory( a, b )
    typedef int socklen_t;
#else
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <sys/fcntl.h>
    #include <sys/ioctl.h>
    #ifdef sun
        #include <sys/filio.h> // -- added for Solaris 10
    #endif
    static const int INVALID_SOCKET = -1; // Always -1 and errno is set
    static const int SOCKET_ERROR   = -1;
#endif

#include "openeaagles/basic/nethandlers/PosixNetHandler.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// Class: PosixNetHandler
//==============================================================================
IMPLEMENT_SUBCLASS(PosixNetHandler, "PosixNetHandler")

// Slot Table
BEGIN_SLOTTABLE(PosixNetHandler)
    "localIpAddress",       // 1) String containing the local host's name or its IP
                            //    address in the Internet standard "." (dotted) notation.
                            //    (default: found via local host name)
    "localPort",            // 2) (optional) Local port number
    "port",                 // 3) Number of the port we're connecting to (required)
    "shared",               // 4) Shared (reuse) flag [default: false (not shared)]
    "sendBuffSizeKb",       // 5) Send buffer size in KB's    (default:  32 Kb; max 1024)
    "recvBuffSizeKb",       // 6) Receive buffer size in KB's (default: 128 Kb; max 1024)
    "ignoreSourcePort",     // 7) Ignore message from this source port
END_SLOTTABLE(PosixNetHandler)

// Map slot table to handles
BEGIN_SLOT_MAP(PosixNetHandler)
    ON_SLOT(1, setSlotLocalIpAddress,   String)
    ON_SLOT(2, setSlotLocalPort,        Number)
    ON_SLOT(3, setSlotPort,             Number)
    ON_SLOT(4, setSlotShared,           Number)
    ON_SLOT(5, setSlotSendBuffSize,     Number)
    ON_SLOT(6, setSlotRecvBuffSize,     Number)
    ON_SLOT(7, setSlotIgnoreSourcePort, Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
PosixNetHandler::PosixNetHandler() :
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

   // Since INVALID_SOCKET is set to the invalid socket state above for both
   // WIN32 and POSIX, there is no need to differentiate between them at this
   // level or further in the source.
   socketNum = INVALID_SOCKET;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void PosixNetHandler::copyData(const PosixNetHandler& org, const bool cc)
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
void PosixNetHandler::deleteData()
{
   if (localIpAddr != 0) delete[] localIpAddr;
   localIpAddr = 0;
}

//------------------------------------------------------------------------------
// Initialize the network handler --
//------------------------------------------------------------------------------
bool PosixNetHandler::initNetwork(const bool noWaitFlag)
{
    // Initialize socket
    bool ok =  init();
    if (ok) {
        // Bind and configure socket
        ok = bindSocket();
        if (ok) {
            if (noWaitFlag) {
                ok = setNoWait();
                if (!ok) std::cerr << "PosixNetHandler::initNetwork(): setNoWait() FAILED" << std::endl;
            }
            else {
                ok = setBlocked();
                if (!ok) std::cerr << "PosixNetHandler::initNetwork(): setBlocked() FAILED" << std::endl;
            }
        }
        else {
            std::cerr << "PosixNetHandler::initNetwork(): bindSocket() FAILED" << std::endl;
        }
    }
    else {
        std::cerr << "PosixNetHandler::initNetwork(): init() FAILED" << std::endl;
    }

    initialized = ok;
    if (initialized && isMessageEnabled(MSG_DEBUG)) {
        std::cout << "PosixNetHandler::initNetwork() -- network initialized successfully" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// init() -- initialize the network
//------------------------------------------------------------------------------
bool PosixNetHandler::init()
{
    bool ok = BaseClass::init();

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
bool PosixNetHandler::bindSocket()
{
    if (socketNum == INVALID_SOCKET) return false;

    // ---
    // Set the reuse socket attribute
    // ---
    {
#if defined(WIN32)
        BOOL optval = getSharedFlag();
        socklen_t optlen = sizeof(optval);
        if (::setsockopt(socketNum, SOL_SOCKET, SO_REUSEADDR, (const char*) &optval, optlen) == SOCKET_ERROR) {
#else
        int optval = getSharedFlag();
        socklen_t optlen = sizeof(optval);
        if (::setsockopt(socketNum, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) == SOCKET_ERROR) {
#endif
            ::perror("PosixNetHandler::bindSocket(): error setsockopt(SO_REUSEADDR)\n");
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
// Set the output buffer size
//------------------------------------------------------------------------------
bool PosixNetHandler::setSendBuffSize()
{
   if (socketNum == INVALID_SOCKET) return false;

   unsigned int optval = sendBuffSizeKb * 1024;
   socklen_t optlen = sizeof(optval);
#if defined(WIN32)
   if (::setsockopt(socketNum, SOL_SOCKET, SO_SNDBUF, (const char*) &optval, optlen) == SOCKET_ERROR) {
#else
   if (::setsockopt(socketNum, SOL_SOCKET, SO_SNDBUF, (void*) &optval, optlen) == SOCKET_ERROR) {
#endif
      ::perror("PosixNetHandler::setSendBuffSize(): error setting the send buffer size\n");
      return false;
   }
   return true;
}

//------------------------------------------------------------------------------
// Sets the input buffer size
//------------------------------------------------------------------------------
bool PosixNetHandler::setRecvBuffSize()
{
   if (socketNum == INVALID_SOCKET) return false;

   unsigned int optval = recvBuffSizeKb * 1024;
   socklen_t optlen = sizeof (optval);
#if defined(WIN32)
   if (::setsockopt(socketNum, SOL_SOCKET, SO_RCVBUF, (const char*) &optval, optlen) == SOCKET_ERROR) {
#else
   if (::setsockopt(socketNum, SOL_SOCKET, SO_RCVBUF, (void*) &optval, optlen) == SOCKET_ERROR) {
#endif
      ::perror("PosixNetHandler::setRecvBuffSize(): error setting the receive buffer size\n");
      return false;
   }
   return true;
}

// -------------------------------------------------------------
// setBlocked() -- Sets blocked I/O mode
// -------------------------------------------------------------
bool PosixNetHandler::setBlocked()
{
    if (socketNum == NET_INVALID_SOCKET) return false;

// Set the socket 'sock' to Blocking. Wait I/O.
#if defined(WIN32)
    unsigned long zz = false;
    if (::ioctlsocket(socketNum, FIONBIO, &zz) == SOCKET_ERROR) {
        ::perror("PosixNetHandler::setBlocked()");
        return false;
    }
#else
    const int zz = 0;
    if (::ioctl(socketNum, FIONBIO, &zz) == SOCKET_ERROR) {
        ::perror("PosixNetHandler::setBlocked()");
        return false;
    }
#endif

   return true;
}

// -------------------------------------------------------------
// setNoWait() -- Sets no wait (non-blocking) I/O mode
// -------------------------------------------------------------
bool PosixNetHandler::setNoWait()
{
    if (socketNum == NET_INVALID_SOCKET) return false;

// Set the socket 'sock' to Non-Blocking. Nowait I/O.
#if defined(WIN32)
    unsigned long zz = true;
    if (::ioctlsocket(socketNum, FIONBIO, &zz ) == SOCKET_ERROR) {
        ::perror("PosixNetHandler::setNoWait()");
        return false;
    }
#else
    const int zz = 1;
    if (::ioctl(socketNum, FIONBIO, &zz ) == SOCKET_ERROR) {
        ::perror("PosixNetHandler::setNoWait()");
        return false;
    }
#endif

   return true;
}

// -------------------------------------------------------------
// Returns true if the network handler has been initialized
// -------------------------------------------------------------
bool PosixNetHandler::isConnected() const
{
    return initialized;
}

// -------------------------------------------------------------
// Close (un-initialize) this network
// -------------------------------------------------------------
bool PosixNetHandler::closeConnection()
{
    initialized = false;
    return true;
}

// -------------------------------------------------------------
// sendData() -- Send data
// -------------------------------------------------------------
bool PosixNetHandler::sendData(const char* const packet, const int size)
{
    if (socketNum == INVALID_SOCKET) return false;

    // Send the data
    struct sockaddr_in addr;        // Working address structure
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = netAddr;
    addr.sin_port = htons(port);
    socklen_t addrlen = sizeof(addr);
    int result = ::sendto(socketNum, packet, size, 0, (const struct sockaddr *) &addr, addrlen);
    if (result == SOCKET_ERROR) {
#if defined(WIN32)
        int err = ::WSAGetLastError();
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "PosixNetHandler::sendData(): sendto error: " << err << " hex=0x" << std::hex << err << std::dec << std::endl;
        }
#else
        ::perror("PosixNetHandler::sendData(): sendto error msg");
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "PosixNetHandler::sendData(): sendto error result: " << result << std::endl;
        }
#endif
        return false;
    }
    return true;
}

// -------------------------------------------------------------
// recvData() -- Receive data and possible ignore our own
//               local port messages.
// -------------------------------------------------------------
unsigned int PosixNetHandler::recvData(char* const packet, const int maxSize)
{
   unsigned int n = 0;

   if (socketNum == INVALID_SOCKET) return 0;

   fromAddr1 = INADDR_NONE;
   fromPort1 = 0;

   bool tryAgain = true;
   while (tryAgain) {
      tryAgain = false;

      // Try to receive the data
      struct sockaddr_in raddr;       // IP address
      socklen_t addrlen = sizeof(raddr);
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
void PosixNetHandler::setSharedFlag(const bool b)
{
   sharedFlg = b;
}

// Set port number
bool PosixNetHandler::setPort(const uint16_t n1)
{
   port = n1;
   return true;
}

// Set local (source) port number
bool PosixNetHandler::setLocalPort(const uint16_t n1)
{
   localPort = n1;
   return true;
}

// Sets the network IP address
bool PosixNetHandler::setNetAddr(const uint32_t addr0)
{
    bool ok = false;
    if (addr0 != INADDR_NONE) {
        netAddr = addr0;
        ok = true;
    }
    return ok;
}

// Sets the network IP address using hostname or the Internet standard "." (dotted) notation
bool PosixNetHandler::setNetAddr(const char* const hostname)
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
               std::cout << "PosixNetHandler::setNetAddr(): Looking up host name: " << hostname;
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
bool PosixNetHandler::setLocalAddr(const uint32_t addr0)
{
    bool ok = false;
    if (addr0 != INADDR_NONE) {
        localAddr = addr0;
        ok = true;
    }
    return ok;
}

// Sets the local IP address using hostname or the Internet standard "." (dotted) notation
bool PosixNetHandler::setLocalAddr(const char* const hostname)
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
                std::cout << "PosixNetHandler::setLocalAddr(): Looking up host name: " << hostname;
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
bool PosixNetHandler::setSlotLocalIpAddress(const String* const msg)
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
bool PosixNetHandler::setSlotPort(const Number* const msg)
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
bool PosixNetHandler::setSlotLocalPort(const Number* const msg)
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
bool PosixNetHandler::setSlotShared(const Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        setSharedFlag( msg->getBoolean() );
        ok = true;
    }
    return ok;
}

// sendBuffSizeKb: Send buffer size in KB's    (default:  32 Kb)
bool PosixNetHandler::setSlotSendBuffSize(const Number* const msg)
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
bool PosixNetHandler::setSlotRecvBuffSize(const Number* const msg)
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
bool PosixNetHandler::setSlotIgnoreSourcePort(const Number* const msg)
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
Object* PosixNetHandler::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& PosixNetHandler::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if (!slotsOnly) {
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

    if (!slotsOnly) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Basic namespace
} // End Eaagles namespace

