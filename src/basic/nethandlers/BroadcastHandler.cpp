//------------------------------------------------------------------------------
// Class: BroadcastHandler
//------------------------------------------------------------------------------

#if defined(WIN32)
    #include <sys/types.h>
    #include <Winsock2.h>
    #define bzero(a,b) ZeroMemory( a, b )
#else
    #include <arpa/inet.h>
    #include <sys/fcntl.h>
    #include <sys/ioctl.h>
    #ifdef sun
        #include <sys/filio.h> // -- added for Solaris 10
    #endif
    static const int SOCKET_ERROR = -1;
#endif

#include "openeaagles/basic/nethandlers/BroadcastHandler.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// Class: BroadcastHandler
//==============================================================================
IMPLEMENT_SUBCLASS(BroadcastHandler,"BroadcastHandler")

BEGIN_SLOTTABLE(BroadcastHandler)
    "networkMask",       // 1) Host Net Mask   "255.255.255.255"
END_SLOTTABLE(BroadcastHandler)

// Map slot table to handles 
BEGIN_SLOT_MAP(BroadcastHandler)
    ON_SLOT(1,setSlotNetworkMask,String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
BroadcastHandler::BroadcastHandler() : networkMask(0)
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void BroadcastHandler::copyData(const BroadcastHandler& org, const bool cc)
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
void BroadcastHandler::deleteData()
{
    if (networkMask != 0) delete[] networkMask;
    networkMask = 0;
}

//------------------------------------------------------------------------------
// init() -- init the network, the socket and the network address
//------------------------------------------------------------------------------
bool BroadcastHandler::init()
{
    // ---
    // Init the base class
    // ---
    bool ok = BaseClass::init();
    if (!ok) return false;

    // ---
    // Create our socket
    // ---
    socketNum = socket(AF_INET, SOCK_DGRAM, 0);
#if defined(WIN32)
    if (socketNum == INVALID_SOCKET) {
#else
    if (socketNum < 0) {
#endif
        perror("BroadcastHandler::init(): socket error");
        return false;
    }

    // ---
    // Set socket attribute for BROADCAST
    // ---
    {
#if defined(WIN32)
        BOOL optval = 1;
        if( setsockopt(socketNum, SOL_SOCKET, SO_BROADCAST, (const char*) &optval, sizeof(optval)) == SOCKET_ERROR)
#else
        int optval = 1;
        if( setsockopt(socketNum, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
#endif
        {
            perror("BroadcastHandler::init(): error setsockopt(SO_BROADCAST)\n");
            return false;
        }
    }

    return true;
}

// -------------------------------------------------------------
// bindSocket() -
// -------------------------------------------------------------
bool BroadcastHandler::bindSocket()
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
            uint32_t localNetMask = inet_addr(networkMask);
            if (localNetAddr != INADDR_NONE && localNetMask != INADDR_NONE) {
               uint32_t localNet = localNetAddr & localNetMask;
               ba = localNet | ~localNetMask;
               if (isMessageEnabled(MSG_INFO)) {
                  std::cout << std::hex << "Broadcast address: " << ba << std::dec << std::endl;
               }
               setNetAddr( ba ); 
               ok = true;
            }
        }
        if (!ok && isMessageEnabled(MSG_ERROR)) {
            std::cerr << "BroadcastHandler::bindSocket() -- unable to set broadcast address!" << std::endl;
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
       if (getLocalPort() != 0) {
         addr.sin_port = htons (getLocalPort());  
       }
       else {
         addr.sin_port = htons(getPort());
       }
       std::printf("bind() addr = %08x\n", addr.sin_addr.s_addr);
       std::printf("bind() port = %08x\n", addr.sin_port);

       if (bind(socketNum, (const struct sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR) {
         perror("BroadcastHandler::bindSocket(): bind error");
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
bool BroadcastHandler::setSlotNetworkMask(const String* const msg)
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
Object* BroadcastHandler::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& BroadcastHandler::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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

