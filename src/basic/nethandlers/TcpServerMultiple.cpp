//------------------------------------------------------------------------------
// Class: TcpServerMultiple
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

#include "openeaagles/basic/nethandlers/TcpServerMultiple.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(TcpServerMultiple, "TcpServerMultiple")

BEGIN_SLOTTABLE(TcpServerMultiple)
    "backlog",    // 1) Listen socket's max backlog (default: 1)
END_SLOTTABLE(TcpServerMultiple)

// Map slot table to handles 
BEGIN_SLOT_MAP(TcpServerMultiple)
    ON_SLOT(1 ,setSlotBacklog, Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
TcpServerMultiple::TcpServerMultiple()
{
   STANDARD_CONSTRUCTOR()

   backlog = 1;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TcpServerMultiple::copyData(const TcpServerMultiple& org, const bool)
{
    BaseClass::copyData(org);
    backlog = org.backlog;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void TcpServerMultiple::deleteData()
{
}

//------------------------------------------------------------------------------
// Initialize this multicast handler -- 
//------------------------------------------------------------------------------
bool TcpServerMultiple::initNetwork(const bool noWaitFlag)
{
    noWait = noWaitFlag;
    bool ok = BaseClass::initNetwork(false);
    if (ok) {
        ok = listenForConnections();
    }
    return ok;
}

//------------------------------------------------------------------------------
// listenForConnections() -- puts the socket into listen mode
//------------------------------------------------------------------------------
bool TcpServerMultiple::listenForConnections()
{
#if defined(WIN32)
    if (socketNum == INVALID_SOCKET) return false;
    if( listen(socketNum, getBacklog()) == SOCKET_ERROR)
#else
    if (socketNum < 0) return false;
    if( listen(socketNum, getBacklog()) < 0)
#endif
    {
        perror("TcpHandler::listenForConnections(): error! \n");
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
// listenForConnections() -- puts the socket into listen mode
//------------------------------------------------------------------------------
TcpHandler* TcpServerMultiple::acceptConnection()
{
   TcpHandler* newHandler = 0;
   struct sockaddr_in clientAddr;

#if defined(WIN32)
   int cAddrSize = sizeof(clientAddr);
   LcSocket newSocket = INVALID_SOCKET;
#else
   socklen_t cAddrSize = sizeof(clientAddr);
   LcSocket newSocket = 0;
#endif

   if (isMessageEnabled(MSG_INFO)) {
       std::cout << "Waiting to accept connection on " << getPort() << " ... " << std::endl;
   }
   newSocket = accept(socketNum, (struct sockaddr *) &clientAddr, &cAddrSize);
   if(newSocket > 0) {
      newHandler = new TcpHandler(newSocket);
   }

   // Set blocked or no-wait
   if (noWait) newHandler->setNoWait();
   else newHandler->setBlocked();

   return newHandler;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool TcpServerMultiple::setBacklog(const unsigned int value)
{
   backlog = value;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool TcpServerMultiple::setSlotBacklog(const Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setBacklog( msg->getInt() );
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* TcpServerMultiple::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& TcpServerMultiple::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "backlog: ";
    sout << getBacklog();
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

