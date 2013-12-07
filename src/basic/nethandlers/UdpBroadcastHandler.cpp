//------------------------------------------------------------------------------
// Class: UdpBroadcastHandler
//------------------------------------------------------------------------------

// M$ WinSock has slightly different return types, some different calling, and
// is missing some of the calls that are standard in Berkely and POSIX socket
// implementation.  These slight differences will be handled in setting basic
// typedefs, defines, and constants that will make each convention match for
// use later in the code.  This will save a lot of pre-processor intervention
// and make the code that much more enjoyable to read!
#if defined(WIN32)
    #include <sys/types.h>
    #include <Winsock2.h>
    #define bzero(a,b) ZeroMemory( a, b )
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

#include "openeaagles/basic/nethandlers/UdpBroadcastHandler.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// Class: UdpBroadcastHandler
//==============================================================================
IMPLEMENT_SUBCLASS(UdpBroadcastHandler, "UdpBroadcastHandler")

BEGIN_SLOTTABLE(UdpBroadcastHandler)
    "networkMask",       // 1) Host Net Mask   "255.255.255.255"
END_SLOTTABLE(UdpBroadcastHandler)

// Map slot table to handles 
BEGIN_SLOT_MAP(UdpBroadcastHandler)
    ON_SLOT(1,setSlotNetworkMask,String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
UdpBroadcastHandler::UdpBroadcastHandler() : networkMask(0)
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void UdpBroadcastHandler::copyData(const UdpBroadcastHandler& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        networkMask = 0;
    }

    if (networkMask != 0) delete[] networkMask;
    networkMask = 0;
    if (org.networkMask != 0) {
        size_t len = strlen(org.networkMask);
        networkMask = new char[len+1];
        lcStrcpy(networkMask,(len+1),org.networkMask);
    }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void UdpBroadcastHandler::deleteData()
{
    if (networkMask != 0) delete[] networkMask;
    networkMask = 0;
}

//------------------------------------------------------------------------------
// init() -- init the network, the socket and the network address
//------------------------------------------------------------------------------
bool UdpBroadcastHandler::init()
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
        ::perror("UdpBroadcastHandler::init(): socket error");
        return false;
    }

    // ---
    // Set socket attribute for BROADCAST
    // ---
    {
#if defined(WIN32)
        BOOL optval = 1;
        if (::setsockopt(socketNum, SOL_SOCKET, SO_BROADCAST, (const char*) &optval, sizeof(optval)) == SOCKET_ERROR) {
#else
        int optval = 1;
        if (::setsockopt(socketNum, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == SOCKET_ERROR) {
#endif
            ::perror("UdpBroadcastHandler::init(): error setsockopt(SO_BROADCAST)\n");
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------
// bindSocket() -
// -------------------------------------------------------------
bool UdpBroadcastHandler::bindSocket()
{
    // ---
    // Our base class will bind the socket
    // ---
    bool ok = BaseClass::bindSocket();

    // ---
    // Next, find and set the broadcast address
    // ---
    if (ok) {
        ok = false;
        uint32_t ba = 0;
        if (networkMask != 0) {
            // User defined broadcast address
            uint32_t localNetAddr = getLocalAddr();
            uint32_t localNetMask = ::inet_addr(networkMask);
            if (localNetAddr != INADDR_NONE && localNetMask != INADDR_NONE) {
               uint32_t localNet = localNetAddr & localNetMask;
               ba = localNet | ~localNetMask;
               if (isMessageEnabled(MSG_INFO)) {
                  std::cout << std::hex << "UdpBroadcast::bindSocket() -- address: " << ba << std::dec << std::endl;
               }
               setNetAddr( ba ); 
               ok = true;
            }
        }
        if (!ok && isMessageEnabled(MSG_ERROR)) {
            std::cerr << "UdpBroadcast::bindSocket() -- unable to set broadcast address!" << std::endl;
        }
    }

    if (ok) {
       // ---
       // Bind the socket to the address in 'localAddr', which was configured by our derived class,
       // and the local port number
       // ---

       struct sockaddr_in addr;        // Working address structure
       bzero(&addr, sizeof(addr));
       addr.sin_family = AF_INET;
#if defined(WIN32)
       addr.sin_addr.s_addr = getLocalAddr();
#else
       addr.sin_addr.s_addr = getNetAddr();
#endif
       if (getLocalPort() != 0) addr.sin_port = htons (getLocalPort());  
       else addr.sin_port = htons(getPort());

       if (bind(socketNum, (const struct sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR) {
          ::perror("UdpBroadcast::bindSocket(): bind error");
          return false;
       }

       if (!setSendBuffSize()) return false;

       if (!setRecvBuffSize()) return false;
    }

    return ok;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// networkMask: Host Net Mask   "255.255.255.255"
bool UdpBroadcastHandler::setSlotNetworkMask(const String* const msg)
{
    bool ok = false;
    if (msg != 0) { 
        networkMask = msg->getCopyString(); 
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* UdpBroadcastHandler::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& UdpBroadcastHandler::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    if (networkMask != 0) {
        indent(sout,i+j);
        sout << "networkMask: \"";
        sout << networkMask;
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

