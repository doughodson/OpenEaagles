
#if defined(WIN32)
    #include <sys/types.h>
    #include <Winsock2.h>
    #include <WS2tcpip.h>
    #define  bzero(a,b) ZeroMemory(a,b)
#else
    #include <arpa/inet.h>
    #include <sys/fcntl.h>
    #include <sys/ioctl.h>
    #ifdef sun
        #include <sys/filio.h> // -- added for Solaris 10
    #endif
    static const int SOCKET_ERROR = -1;
#endif

#include "openeaagles/basic/TcpHandler.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// Class: TcpHandler
//==============================================================================
IMPLEMENT_SUBCLASS(TcpHandler,"TcpHandler")
EMPTY_SLOTTABLE(TcpHandler)
EMPTY_SERIALIZER(TcpHandler)

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
TcpHandler::TcpHandler()
{
   STANDARD_CONSTRUCTOR()

   connected = false;
   connectionTerminated = false;
   noWait = false;

   #if defined(WIN32)
      tcpSocket = INVALID_SOCKET;
   #else
      tcpSocket = -1;
   #endif
}

TcpHandler::TcpHandler(const LcSocket sn)
{
   STANDARD_CONSTRUCTOR()

   connected = true;
   connectionTerminated = false;
   noWait = false;
   socketNum = sn;
   tcpSocket = sn;
   setSendBuffSize();
   setRecvBuffSize();
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TcpHandler::copyData(const TcpHandler& org, const bool)
{
   BaseClass::copyData(org);

   connected = org.connected;
   connectionTerminated = org.connectionTerminated;
   tcpSocket = org.tcpSocket;
   noWait = org.noWait;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void TcpHandler::deleteData()
{
}

//------------------------------------------------------------------------------
// init() -- init the network, the socket and the network address
//------------------------------------------------------------------------------
bool TcpHandler::init()
{
    // ---
    // Init the base class
    // ---
    bool success = BaseClass::init();
    if (!success) return false;

    // ---
    // Create our stream socket
    // ---
    socketNum = socket(AF_INET, SOCK_STREAM, 0);
#if defined(WIN32)
    if (socketNum == INVALID_SOCKET) {
#else
    if (socketNum < 0) {
#endif
        perror("init(): socket error");
        success = false;
    }

    return success;
}


// -------------------------------------------------------------
// bindSocket() -- bind the socket to an address, and configure
// the send and receive buffers. 
// -------------------------------------------------------------
bool TcpHandler::bindSocket()
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
        perror("bindSocket(): bind error");
        return false;
      }

      if (!setSendBuffSize()) return false;
   
      if (!setRecvBuffSize()) return false;
   }

   return ok;
}


// -------------------------------------------------------------
// Returns true if the network handler has been initialized and connected.
// -------------------------------------------------------------
bool TcpHandler::isConnected() const
{
    return connected && BaseClass::isConnected();
}

//------------------------------------------------------------------------------
// closeConnection() -- request that the connection is closed (shutdown)
//------------------------------------------------------------------------------
bool TcpHandler::closeConnection()
{
    bool success = true;

#if defined(WIN32)
    if( closesocket(tcpSocket) == SOCKET_ERROR)
#else
    if( shutdown(tcpSocket, SHUT_RDWR) < 0)
#endif
    {
        perror("close(): error! \n");
        success = false;
    }

    connected = false;
    connectionTerminated = true;

    return BaseClass::closeConnection() && success;
}

// -------------------------------------------------------------
// sendData() -- Send data to our connected TCP socket
// -------------------------------------------------------------
bool TcpHandler::sendData(const char* const packet, const int size)
{
   if (!isConnected() || hasBeenTerminated()) return false;

#if defined(WIN32)
    if (tcpSocket == INVALID_SOCKET) return false;
#else
    if (tcpSocket < 0) return false;
#endif

    int result = send(tcpSocket, packet, size, 0);

#if defined(WIN32)
   if (result == SOCKET_ERROR) {
        connected = false;
        connectionTerminated = true;
        int err = WSAGetLastError();
        if (isMessageEnabled(MSG_ERROR)) {
           std::cerr << "sendData(): sendto error: " << err << " hex=0x" << std::hex << err << std::dec << std::endl;
        }
        return false;
    }
#else
   if (result < 0) {
        connected = false;
        connectionTerminated = true;
        perror("sendto error");
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "sendData(): sendto error: result: " << result << std::endl;
        }
        return false;
    }
#endif
    return true;
}

// -------------------------------------------------------------
// recvData() -- Receive data from our connected TCP socket
// -------------------------------------------------------------
unsigned int TcpHandler::recvData(char* const packet, const int maxSize)
{
   if (!isConnected() || hasBeenTerminated()) return 0;

#if defined(WIN32)
    if (tcpSocket == INVALID_SOCKET) return 0;
#else
    if (tcpSocket < 0) return 0;
#endif

   // Try to receive the data
   unsigned int n = 0;
   int result = recvfrom(tcpSocket, packet, maxSize, 0, 0, 0);
   if (result > 0) n = (unsigned int) result;

   return n;
}

//==============================================================================
// Class: TcpClient
//==============================================================================
IMPLEMENT_SUBCLASS(TcpClient,"TcpClient")

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

//==============================================================================
// Class: TcpServerMulti
//==============================================================================
IMPLEMENT_SUBCLASS(TcpServerMulti,"TcpServerMulti")

BEGIN_SLOTTABLE(TcpServerMulti)
    	"backlog",    // 1) Listen socket's max backlog (default: 1)
END_SLOTTABLE(TcpServerMulti)

// Map slot table to handles 
BEGIN_SLOT_MAP(TcpServerMulti)
    ON_SLOT(1 ,setSlotBacklog, Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
TcpServerMulti::TcpServerMulti()
{
   STANDARD_CONSTRUCTOR()

   backlog = 1;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TcpServerMulti::copyData(const TcpServerMulti& org, const bool)
{
    BaseClass::copyData(org);
    backlog = org.backlog;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void TcpServerMulti::deleteData()
{
}

//------------------------------------------------------------------------------
// Initialize this multicast handler -- 
//------------------------------------------------------------------------------
bool TcpServerMulti::initNetwork(const bool noWaitFlag)
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
bool TcpServerMulti::listenForConnections()
{
#if defined(WIN32)
    if (socketNum == INVALID_SOCKET) return false;
    if( listen(socketNum, getBacklog()) == SOCKET_ERROR)
#else
    if (socketNum < 0) return false;
    if( listen(socketNum, getBacklog()) < 0)
#endif
    {
        perror("listen(): error! \n");
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
// listenForConnections() -- puts the socket into listen mode
//------------------------------------------------------------------------------
TcpHandler* TcpServerMulti::acceptConnection()
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
bool TcpServerMulti::setBacklog(const unsigned int value)
{
   backlog = value;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool TcpServerMulti::setSlotBacklog(const Number* const msg)
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
Object* TcpServerMulti::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& TcpServerMulti::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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


//==============================================================================
// Class: TcpServerSingle
//==============================================================================
IMPLEMENT_SUBCLASS(TcpServerSingle,"TcpServerSingle")
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
        perror("listen(): error! \n");
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
