
#include "openeaagles/otw/cigi/CigiClNetwork.hpp"

#include "openeaagles/otw/cigi/OtwCigiCl.hpp"

#include "openeaagles/base/concurrent/SingleTask.hpp"
#include "openeaagles/base/network/NetHandler.hpp"

#include "cigicl/CigiBaseSignalProcessing.h"

#include "cigicl/CigiEntityCtrlV3.h"
#include "cigicl/CigiCompCtrlV3.h"
#include "cigicl/CigiIGCtrlV3.h"
#include "cigicl/CigiLosVectReqV3.h"
#include "cigicl/CigiViewCtrlV3.h"
#include "cigicl/CigiViewDefV3.h"
#include "cigicl/CigiArtPartCtrlV3.h"
#include "cigicl/CigiHatHotReqV3.h"
#include "cigicl/CigiSOFV3.h"
#include "cigicl/CigiHatHotRespV3.h"
#include "cigicl/CigiHatHotRespV3_2.h"
#include "cigicl/CigiHatHotXRespV3_2.h"
#include "cigicl/CigiLosRespV3.h"
#include "cigicl/CigiCollDetSegRespV3.h"
#include "cigicl/CigiCollDetVolRespV3.h"
#include "cigicl/CigiSensorRespV3.h"
#include "cigicl/CigiIGMsgV3.h"
#include "cigicl/CigiHostSession.h"
#include "cigicl/CigiSensorCtrlV3.h"

namespace oe {
namespace otw {

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
static const int MAX_BUF_SIZE = 1472;
static const double LOS_REQ_TIMEOUT = 2.0;     // one second timeout

//==============================================================================
// CigiClNetworkSignalProcessing class
//==============================================================================
class CigiClNetworkSignalProcessing : public CigiBaseSignalProcessing {
public:
   inline CigiClNetworkSignalProcessing(OtwCigiCl* parent) : p(parent) { }
   virtual ~CigiClNetworkSignalProcessing();

   virtual void OnSOF(CigiBasePacket* packet);
   virtual void OnCollDetSegResp(CigiBasePacket* packet);
   virtual void OnCollDetVolResp(CigiBasePacket* packet);
   virtual void OnHatHotResp(CigiBasePacket* packet);
   virtual void OnHotResp(CigiBasePacket *Packet);
   virtual void OnIGMsg(CigiBasePacket* packet);
   virtual void OnLosResp(CigiBasePacket* packet);
   virtual void OnSensorResp(CigiBasePacket* packet);

private:
   OtwCigiCl* p {};
};

CigiClNetworkSignalProcessing::~CigiClNetworkSignalProcessing()
{
   p = nullptr;
}

//------------------------------------------------------------------------------
// callbacks -- Pass control to the correct object (session's) member function
//------------------------------------------------------------------------------

void CigiClNetworkSignalProcessing::OnSOF(CigiBasePacket* packet)
{
   if (p != nullptr) p->startOfFrame( dynamic_cast<CigiSOFV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnCollDetSegResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->collisionSegmentResp( dynamic_cast<CigiCollDetSegRespV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnCollDetVolResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->collisionVolumeResp( dynamic_cast<CigiCollDetVolRespV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnHotResp(CigiBasePacket* packet)
{
   if (p != nullptr) {

      const auto m3 = dynamic_cast<CigiHatHotRespV3*> (packet);
      if (m3 != nullptr) {
         p->hatHotResp( m3 );
      }
      else {

         const auto m3_2 = dynamic_cast<CigiHatHotRespV3_2*> (packet);
         if (m3_2 != nullptr) {
            CigiHatHotRespV3 resp;
            resp.SetHatHotID(m3_2->GetHatHotID());
            resp.SetHot(m3_2->GetHot());
            resp.SetValid(m3_2->GetValid());
            p->hatHotResp( &resp );
         }
         else {
            const auto mx3_2 = dynamic_cast<CigiHatHotXRespV3_2*> (packet);
            if (mx3_2 != nullptr) {
               CigiHatHotRespV3 resp;
               resp.SetHatHotID(mx3_2->GetHatHotID());
               resp.SetHot(mx3_2->GetHot());
               resp.SetValid(mx3_2->GetValid());
               p->hatHotResp( &resp );
            }
         }

      }
   }
}

void CigiClNetworkSignalProcessing::OnHatHotResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->hatHotResp( dynamic_cast<CigiHatHotRespV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnIGMsg(CigiBasePacket* packet)
{
   if (p != nullptr) p->igResponse( dynamic_cast<CigiIGMsgV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnLosResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->losResp( dynamic_cast<CigiLosRespV3*> (packet) );
}

void CigiClNetworkSignalProcessing::OnSensorResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->sensorResp( dynamic_cast<CigiSensorRespV3*> (packet) );
}

//==============================================================================
// CigiClNetwork main loop thread
//==============================================================================

class NetThread : public base::SingleTask
{
   DECLARE_SUBCLASS(NetThread, base::SingleTask)
   public: NetThread(base::Component* const parent, const double priority);
   private: virtual unsigned long userFunc() override;
};

IMPLEMENT_SUBCLASS(NetThread, "NetThread")
EMPTY_SLOTTABLE(NetThread)
EMPTY_COPYDATA(NetThread)
EMPTY_DELETEDATA(NetThread)
EMPTY_SERIALIZER(NetThread)

NetThread::NetThread(base::Component* const parent, const double priority): base::SingleTask(parent, priority)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long NetThread::userFunc()
{
   const auto otwNet = dynamic_cast<CigiClNetwork*>( getParent() );
   if (otwNet != nullptr) {
      if (otwNet->isMessageEnabled(MSG_INFO)) {
         std::cout << "NetThread::userFunc(): CigiClNetwork = " << otwNet << std::endl;
      }
      otwNet->mainLoop();
   }
   return 0;
}

//==============================================================================
// CigiClNetwork class
//==============================================================================

IMPLEMENT_SUBCLASS(CigiClNetwork, "CigiClNetwork")
EMPTY_SERIALIZER(CigiClNetwork)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(CigiClNetwork)
"netInput",             // 1) Network input handler
"netOutput",            // 2) Network output handler
END_SLOTTABLE(CigiClNetwork)

// Map slot table to handles
BEGIN_SLOT_MAP(CigiClNetwork)
   ON_SLOT(1, setSlotNetInput,  base::NetHandler)
   ON_SLOT(2, setSlotNetOutput, base::NetHandler)
END_SLOT_MAP()

CigiClNetwork::CigiClNetwork()
{
   STANDARD_CONSTRUCTOR()
}

void CigiClNetwork::copyData(const CigiClNetwork& org, const bool)
{
   BaseClass::copyData(org);

   // We need to init this ourselves, so ...
   netInput = nullptr;
   netOutput = nullptr;
   networkInitialized = false;
   networkInitFailed = false;
   sigProcessor = nullptr;

   if (thread != nullptr) {
      thread->terminate();
      thread = nullptr;
   }
}

void CigiClNetwork::deleteData()
{
   netInput = nullptr;
   netOutput = nullptr;
   if (sigProcessor != nullptr) {
      delete sigProcessor;
   }
   if (thread != nullptr) {
      thread->terminate();
      thread = nullptr;
   }
   sigProcessor = nullptr;
}

//------------------------------------------------------------------------------
// initialize() -- Initialize this CIGI session
//------------------------------------------------------------------------------
bool CigiClNetwork::initialize(OtwCigiCl* const p)
{
   bool ok = BaseClass::initialize(p);

   if (ok) {
      // ---
      // Set the callback functions
      // ---
      if (sigProcessor == nullptr) {
         OtwCigiCl* parent = getOtwCigi();
         sigProcessor = new CigiClNetworkSignalProcessing(parent);
         if (sigProcessor == nullptr) {
            return false;
         }
      }

      CigiHostSession* hostSession = getSession();
      msgOut = &(hostSession->GetOutgoingMsgMgr());
      msgIn = &(hostSession->GetIncomingMsgMgr());
      msgIn->SetReaderCigiVersion(2);
      msgIn->UsingIteration(false);
      msgIn->RegisterSignalProcessor(sigProcessor);

      // ---
      // Initialize the network handlers
      // ---
      if (!networkInitialized && !networkInitFailed) {
         networkInitialized = initCigiNetwork();
         networkInitFailed = !networkInitialized;
      }

      // ---
      // Create a network thread to handle the blocked network traffic
      // ---
      if (ok && networkInitialized && (thread == nullptr)) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "CREATING CIGI process" << std::endl;
         }
         createCigiProcess();
      }

      if (isMessageEnabled(MSG_INFO)) {
         std::cout << "CigiClNetwork::initialize(): CIGI handler initialized!" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// createCigiProcess() -- Create the CIGI thread
//------------------------------------------------------------------------------
bool CigiClNetwork::createCigiProcess()
{
   if ( thread == nullptr ) {

      // parent -> our OTW manager
      thread = new NetThread(this, 0.6);
      thread->unref(); // 'thread' is a safe_ptr<>

      bool ok = thread->create();
      if (!ok) {
         thread = nullptr;
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "CigiClNetwork::createCigiProcess(): ERROR, failed to create the thread!" << std::endl;
         }
      }
   }
   return (thread != nullptr);
}

//------------------------------------------------------------------------------
// True if we've been successfully initialized
//------------------------------------------------------------------------------
bool CigiClNetwork::isInitialized()
{
   return BaseClass::isInitialized() && networkInitialized && (thread != nullptr);
}

//------------------------------------------------------------------------------
// initCigiClNetwork() -- Init the network
//------------------------------------------------------------------------------
bool CigiClNetwork::initCigiNetwork()
{
   bool ok = true;

   // Initialize network input handler
   if (netInput != nullptr) {
      if (netInput->initNetwork(false)) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "netInput Initialize OK" << std::endl;
         }
      }
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "CigiClNetwork::initNetwork(): failure to find the network input handler (see slot 'netInput')" << std::endl;
      }
      ok = false;
   }

   // Initialize network output handler
   if (netOutput != nullptr) {
      if (netOutput->initNetwork(true)) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "netOutput Initialize OK" << std::endl;
         }
      }
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "CigiClNetwork::initNetwork(): failure to find the network output handler (see slot 'netOutput')" << std::endl;
      }
      ok = false;
   }

   return ok;
}

//------------------------------------------------------------------------------
// CIGI functions
//------------------------------------------------------------------------------

// Start the message.  This MUST come before any CigiAddPacket*() functions.
void CigiClNetwork::startMessage()
{
   msgOut->BeginMsg();
}

// End the message.
void CigiClNetwork::endMessage()
{
#if 1
   msgOut->UpdateFrameCntr();
   msgOut->LockMsg();

   int sendSize = 0;
   unsigned char* sendBuff = msgOut->GetMsg(sendSize);
   netOutput->sendData( reinterpret_cast<char*>(sendBuff), sendSize );

   msgOut->UnlockMsg();
#else
   // CGB this is sync mode, and I think it only works for version 3 in the class lib right now
   msgOut->LockMsg();

   int sendSize = 0;
   unsigned char* sendBuff = msgOut->GetMsg(sendSize);
   unsigned char* recvBuff = msgIn->GetMsgBuffer();

   msgOut->UpdateFrameCntr(sendBuff, recvBuff);

   netOutput->sendData( reinterpret_cast<char*>(sendBuff), sendSize );

   msgOut->UnlockMsg();
#endif
}

int CigiClNetwork::getOutgoingBufferSize()
{
   int sendSize = 0;
   msgOut->GetMsg(sendSize);
   return sendSize;
}

// Add an IG Control packet
void CigiClNetwork::addPacketIGCtrl(CigiIGCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a View Control packet
void CigiClNetwork::addPacketSensorCtrl(CigiSensorCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a View Control packet
void CigiClNetwork::addPacketViewCtrl(CigiViewCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a View Definition packet
void CigiClNetwork::addPacketViewDef(CigiViewDefV3* const p)
{
   *msgOut << *p;
}

// Add an entity control packet
void CigiClNetwork::addPacketEntityCtrl(CigiEntityCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a Component Control packet
void CigiClNetwork::addPacketComponentCtrl(CigiCompCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a Articulated Part Control packet
void CigiClNetwork::addPacketArtPartCtrl(CigiArtPartCtrlV3* const p)
{
   *msgOut << *p;
}

// Add a Height Of Terrain (HOT) Request packet
void CigiClNetwork::addPacketHatHotReq(CigiHatHotReqV3* const p)
{
   *msgOut << *p;
}

// Add a LOS/Range Request packet
void CigiClNetwork::addPacketLosRangeReq(CigiLosVectReqV3* const p)
{
   *msgOut << *p;
}

//=============================================================================
// Main loop
//=============================================================================
void CigiClNetwork::mainLoop()
{
   // ---
   // Receive and process CIGI packets from the visual system
   // ---
   if (netInput != nullptr && netOutput != nullptr) {
      while ( !getOtwCigi()->isShutdown() ) {

         msgIn->AdvanceCrntBuffer();
         unsigned char* rcvbuff = msgIn->GetMsgBuffer();
         int recvCnt = netInput->recvData(reinterpret_cast<char*>(rcvbuff), MAX_BUF_SIZE);

         if (recvCnt > 0) {
            msgIn->SetCrntMsgSize(recvCnt);
            msgIn->ProcessIncomingMsg();
         }
         else {
            msgIn->SetCrntMsgSize(0);
         }
      }
   }
}

//------------------------------------------------------------------------------
// get a pre-ref'd pointers to the network handlers
//------------------------------------------------------------------------------
base::NetHandler* CigiClNetwork::getInputHandler()
{
   return netInput.getRefPtr();
}

base::NetHandler* CigiClNetwork::getOutputHandler()
{
   return netOutput.getRefPtr();
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

// Set Network Input Handler
bool CigiClNetwork::setSlotNetInput(base::NetHandler* const msg)
{
   netInput = msg;
   return true;
}

// Net Network Output Handler
bool CigiClNetwork::setSlotNetOutput(base::NetHandler* const msg)
{
   netOutput = msg;
   return true;
}

}
}
