
#include "openeaagles/recorder/NetInput.h"
#include "openeaagles/recorder/protobuf/DataRecord.pb.h"
#include "openeaagles/recorder/DataRecordHandle.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Recorder {

//==============================================================================
// Class NetInput
//==============================================================================
IMPLEMENT_SUBCLASS(NetInput,"RecorderNetInput")
EMPTY_SERIALIZER(NetInput)

BEGIN_SLOTTABLE(NetInput)
   "netHandler",           // 1) Network handler
   "noWait",               // 2) No wait (unblocked) I/O flag (default: false -- blocked I/O)
END_SLOTTABLE(NetInput)

// Map slot table to handles
BEGIN_SLOT_MAP(NetInput)
    ON_SLOT(1, setSlotNetwork,   Eaagles::Basic::NetHandler)
    ON_SLOT(2, setSlotNoWait,    Eaagles::Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
NetInput::NetInput() : netHandler(nullptr)
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void NetInput::initData()
{
   ibuf = new char[MAX_INPUT_BUFFER_SIZE];

   networkInitialized = false;
   networkInitFailed = false;
   noWaitFlag = false;
   firstPassFlg = true;
}


//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void NetInput::copyData(const NetInput& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   noWaitFlag = org.noWaitFlag;

   // We need to init this ourselves, so ...
   netHandler = nullptr;
   networkInitialized = false;
   networkInitFailed = false;
   firstPassFlg = true;
}


//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void NetInput::deleteData()
{
   closeConnections();
   netHandler = nullptr;
   if (ibuf != nullptr) { delete[] ibuf; ibuf = nullptr; }
}


//------------------------------------------------------------------------------
// Returns true if the networks are initialized and connected
//------------------------------------------------------------------------------
bool NetInput::isNetworkEnabled() const
{
   bool ok = networkInitialized && netHandler->isConnected();
   return ok;
}


//------------------------------------------------------------------------------
// Init the network
//------------------------------------------------------------------------------
bool NetInput::initNetworks()
{
   bool ok = false;
   if (netHandler != nullptr) {
      ok = netHandler->initNetwork(noWaitFlag);
      networkInitialized = ok;
      networkInitFailed = !ok;
   }
   return ok;
}

//------------------------------------------------------------------------------
// close the network connection
//------------------------------------------------------------------------------
void NetInput::closeConnections()
{
   if (netHandler != nullptr && networkInitialized) netHandler->closeConnection();
   networkInitialized = false;
   networkInitFailed = false;
}

//------------------------------------------------------------------------------
// Read a record
//------------------------------------------------------------------------------
const DataRecordHandle* NetInput::readRecordImp()
{
   // First pass?  Does the file need to be opened?
   if (firstPassFlg) {
      if ( !networkInitialized ) {
         initNetworks();
      }
      firstPassFlg = false;
   }

   DataRecordHandle* handle = nullptr;

   // When the file is open and ready ...
   if ( networkInitialized && netHandler->isConnected() ) {

      // ---
      // Try to read a message into 'ibuf'
      // ---
      unsigned int n = netHandler->recvData( ibuf, MAX_INPUT_BUFFER_SIZE );

      // ---
      // If we've successfully read a message from the network
      // then parse it as a DataRecord and put it into a Handle.
      // ---
      if (n > 0) {
         // Parse the data record
         std::string wireFormat(ibuf, n);
         Pb::DataRecord* dataRecord = new Pb::DataRecord();
         bool ok = dataRecord->ParseFromString(wireFormat);

         if (ok) {
            // Create a handle for the data record (it now has ownership)
            handle = new DataRecordHandle(dataRecord);
         }

         else if (isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
            std::cerr << "NetInput::readRecord() -- ParseFromString() error" << std::endl;
            delete dataRecord;
            dataRecord = nullptr;
         }

      }
   }
   return handle;
}


//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// Network Handler
bool NetInput::setSlotNetwork(Eaagles::Basic::NetHandler* const msg)
{
   netHandler = msg;
   return true;
}

// No wait (unblocked) I/O flag
bool NetInput::setSlotNoWait(Eaagles::Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      noWaitFlag = msg->getBoolean();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Eaagles::Basic::Object* NetInput::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

} // End Recorder namespace
} // End Eaagles namespace
