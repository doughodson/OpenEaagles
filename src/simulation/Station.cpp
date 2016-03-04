//------------------------------------------------------------------------------
// Class:  Station
//------------------------------------------------------------------------------

#include "openeaagles/simulation/Station.h"

#include "openeaagles/simulation/DataRecorder.h"
#include "openeaagles/simulation/NetIO.h"
#include "openeaagles/simulation/Otw.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"

#include "openeaagles/base/Color.h"
#include "openeaagles/base/IoHandler.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Thread.h"
#include "openeaagles/base/Timers.h"
#include "openeaagles/base/units/Times.h"

#include <ctime>

// #define NET_TIMING_TEST

namespace oe {
namespace simulation {

static const unsigned int DEFAULT_FAST_FORWARD_RATE = 1;

//=============================================================================
// Declare the thread classes
//=============================================================================

// ---
// Time-critical thread
// ---
class TcThread : public basic::ThreadPeriodicTask {
   DECLARE_SUBCLASS(TcThread,basic::ThreadPeriodicTask)
   public: TcThread(basic::Component* const parent, const LCreal priority, const LCreal rate);
   private: virtual unsigned long userFunc(const LCreal dt);
};

// ---
// Interoperability Networks thread
// ---
class NetThread : public basic::ThreadPeriodicTask {
   DECLARE_SUBCLASS(NetThread,basic::ThreadPeriodicTask)
   public: NetThread(basic::Component* const parent, const LCreal priority, const LCreal rate);
   private: virtual unsigned long userFunc(const LCreal dt);
};

// ---
// Background thread
// ---
class BgThread : public basic::ThreadPeriodicTask {
   DECLARE_SUBCLASS(BgThread,basic::ThreadPeriodicTask)
   public: BgThread(basic::Component* const parent, const LCreal priority, const LCreal rate);
   private: virtual unsigned long userFunc(const LCreal dt);
};

//=============================================================================
// Station class
//=============================================================================
IMPLEMENT_SUBCLASS(Station,"Station")

const LCreal Station::DEFAULT_TC_THREAD_PRI  = 0.8;
const LCreal Station::DEFAULT_BG_THREAD_PRI  = 0.5;
const LCreal Station::DEFAULT_NET_THREAD_PRI = 0.5;

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Station)
   "simulation",        //  1: Simulation model
   "networks",          //  2: List of Network models
   "otw",               //  3: Out-The-Window (OTW) visual system  [ Otw or basic::PairStream ]
   "ioHandler",         //  4: I/O data handler(s)  [ basic::IoHandler or basic::PairStream ]
   "ownship",           //  5: Player name of our ownship (primary) player
   "tcRate",            //  6: Time-critical rate (Hz) (basic::Number, default: 50hz)
   "tcPriority",        //  7: Time-critical thread priority (zero(0) is lowest, one(1) is highest)
   "tcStackSize",       //  8: Time-critical thread stack size (default: <system default size>)
   "fastForwardRate",   //  9: Fast forward rate (i.e., number of times updateTC() is called per frame)
   "netRate",           // 10: Network thread rate (Hz)  (basic::Number, default: 20 hz)
   "netPriority",       // 11: Network thread priority (zero(0) is lowest, one(1) is highest)
   "netStackSize",      // 12: Network thread stack size (default: <system default size>)
   "bgRate",            // 13: Background thread rate (Hz) (default: 0.0 -- no thread)
   "bgPriority",        // 14: Background thread priority
   "bgStackSize",       // 15: Background thread stack size (default: <system default size>)
   "startupResetTimer", // 16: Startup (initial) RESET event timer value (basic::Time) (default: no reset event)
   "enableUpdateTimers",// 17: Enable calling basic::Timers::updateTimers() from updateTC() (default: false)
   "dataRecorder",      // 18) Our Data Recorder
END_SLOTTABLE(Station)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Station)
   ON_SLOT( 1,  setSlotSimulation,            Simulation)

   ON_SLOT( 2,  setSlotNetworks,              basic::PairStream)

   ON_SLOT( 3,  setSlotOutTheWindow,          Otw)
   ON_SLOT( 3,  setSlotOutTheWindow,          basic::PairStream)

   ON_SLOT( 4,  setSlotIoHandler,             basic::IoHandler)
   ON_SLOT( 4,  setSlotIoHandler,             basic::PairStream)

   ON_SLOT( 5,  setSlotOwnshipName,           basic::String)

   ON_SLOT( 6,  setSlotTimeCriticalRate,      basic::Number)
   ON_SLOT( 7,  setSlotTimeCriticalPri,       basic::Number)
   ON_SLOT( 8,  setSlotTimeCriticalStackSize, basic::Number)
   ON_SLOT( 9,  setSlotFastForwardRate,       basic::Number)

   ON_SLOT(10,  setSlotNetworkRate,           basic::Number)
   ON_SLOT(11,  setSlotNetworkPri,            basic::Number)
   ON_SLOT(12,  setSlotNetworkStackSize,      basic::Number)

   ON_SLOT(13,  setSlotBackgroundRate,        basic::Number)
   ON_SLOT(14,  setSlotBackgroundPri,         basic::Number)
   ON_SLOT(15,  setSlotBackgroundStackSize,   basic::Number)

   ON_SLOT(16,  setSlotStartupResetTime,      basic::Time)
   ON_SLOT(17,  setSlotEnableUpdateTimers,    basic::Number)

   ON_SLOT(18, setDataRecorder,            DataRecorder)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Station::Station()
{
   STANDARD_CONSTRUCTOR()

   initData();
}


//------------------------------------------------------------------------------
// Init member data
//------------------------------------------------------------------------------
void Station::initData()
{
   sim = nullptr;
   otw = nullptr;
   networks = nullptr;
   ioHandlers = nullptr;
   ownshipName = nullptr;
   ownship = nullptr;
   dataRecorder = nullptr;

   tcRate = 50.0;                   // default time-critical thread rate
   tcPri = DEFAULT_TC_THREAD_PRI;
   tcStackSize = 0;
   tcThread = nullptr;
   fastForwardRate = DEFAULT_FAST_FORWARD_RATE;

   netRate = 0.0;                   // default network thread rate
   netPri = DEFAULT_NET_THREAD_PRI;
   netStackSize = 0;
   netThread = nullptr;

   bgRate = 0.0;                    // default network thread rate
   bgPri = DEFAULT_BG_THREAD_PRI;
   bgStackSize = 0;
   bgThread = nullptr;

   tmrUpdateEnbl = false;

   startupResetTimer0 = nullptr;
   startupResetTimer = -1.0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Station::copyData(const Station& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // Terminate any old threads
   setTcThread(nullptr);
   setNetThread(nullptr);
   setBgThread(nullptr);

   // Set the simulation exec
   if (org.sim != nullptr) {
      Simulation* copy = org.sim->clone();
      setSlotSimulation( copy );
      copy->unref();
   }
   else {
      setSlotSimulation(nullptr);
   }

   // Copy the OTW handlers
   if (org.otw != nullptr) {
      basic::PairStream* copy = org.otw->clone();
      setSlotOutTheWindow( copy );
      copy->unref();
   }
   else {
      setSlotOutTheWindow(static_cast<basic::PairStream*>(nullptr));
   }

   // Copy the networks
   if (org.networks != nullptr) {
      basic::PairStream* copy = org.networks->clone();
      setSlotNetworks( copy );
      copy->unref();
   }
   else {
      setSlotNetworks(static_cast<basic::PairStream*>(nullptr));
   }

   // Copy the I/O handlers
   if (org.ioHandlers != nullptr) {
      basic::PairStream* copy = org.ioHandlers->clone();
      setSlotIoHandler( copy );
      copy->unref();
   }
   else {
      setSlotIoHandler(static_cast<basic::PairStream*>(nullptr));
   }

   {  // clone the data recorder
      DataRecorder* copy = nullptr;
      if (org.dataRecorder != nullptr) copy = org.dataRecorder->clone();
      setDataRecorder(copy);
      if (copy != nullptr) copy->unref();
   }

   tcRate = org.tcRate;
   tcPri = org.tcPri;
   tcStackSize = org.tcStackSize;
   fastForwardRate = org.fastForwardRate;

   netRate = org.netRate;
   netPri = org.netPri;
   netStackSize = org.netStackSize;

   bgRate = org.bgRate;
   bgPri = org.bgPri;
   bgStackSize = org.bgStackSize;

   tmrUpdateEnbl = org.tmrUpdateEnbl;

   if (org.startupResetTimer0!= nullptr) {
      basic::Time* copy = org.startupResetTimer0->clone();
      setSlotStartupResetTime( copy );
      copy->unref();
   }
   else {
      setSlotStartupResetTime(nullptr);
   }

   startupResetTimer = org.startupResetTimer;

   // Unref our old stuff (if any)
   if (ownshipName != nullptr) { ownshipName->unref(); ownshipName = nullptr; }
   if (ownship != nullptr)     { ownship->unref(); ownship = nullptr; }

   // Copy own ownship name
   if (org.ownshipName != nullptr) {
      ownshipName = org.ownshipName->clone();
   }

   // Attach our ownship
   if (ownshipName != nullptr) {
      setOwnshipByName( *ownshipName );
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Station::deleteData()
{
   // Terminate any old threads
   setTcThread(nullptr);
   setNetThread(nullptr);
   setBgThread(nullptr);

   // Clear our pointers
   setOwnshipPlayer(nullptr);
   setSlotOutTheWindow(static_cast<basic::PairStream*>(nullptr));
   setSlotNetworks(nullptr);
   setSlotIoHandler(static_cast<basic::PairStream*>(nullptr));
   setSlotSimulation(nullptr);
   setSlotStartupResetTime(nullptr);
   setDataRecorder(nullptr);
}

//------------------------------------------------------------------------------
// reset() -- Reset the station
//------------------------------------------------------------------------------
void Station::reset()
{
   if (isMessageEnabled(MSG_INFO)) {
      std::cout << "Station::reset()" << std::endl;
   }

   // Reset our major subsystems
   if (sim != nullptr) sim->event(RESET_EVENT);

   // ---
   // Reset the ownship pointer
   // ---
   if (ownshipName != nullptr) {
      setOwnshipByName( *ownshipName );
      if (ownship == nullptr) {
         // Ok, we had a list of players and an ownship player name, but still
         // don't have an ownship pointer -- print an error message.
         std::cerr << "Station::reset(): ownship not found: " << *ownshipName << std::endl;
      }
   }

   // Reset the I/O Handlers
   if (ioHandlers != nullptr) {
      basic::List::Item* item = ioHandlers ->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         basic::IoHandler* p = static_cast<basic::IoHandler*>(pair->object());
         p->event(RESET_EVENT);
         item = item->getNext();
      }
   }

   // Reset the OTW subsystems
   if (otw != nullptr) {
      basic::List::Item* item = otw ->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         Otw* p = static_cast<Otw*>(pair->object());
         p->event(RESET_EVENT);
         item = item->getNext();
      }
   }

   // Reset the networks
   if (networks != nullptr) {
      basic::List::Item* item = networks ->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         NetIO* p = static_cast<NetIO*>(pair->object());
         p->event(RESET_EVENT);
         item = item->getNext();
      }
   }

   // ---
   // Reset the data recorder
   // ---
   if (dataRecorder != nullptr) dataRecorder->event(RESET_EVENT);

   BaseClass::reset();
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void Station::updateTC(const LCreal dt)
{
   // Update the basic::Timers
   if (isUpdateTimersEnabled()) {
      basic::Timer::updateTimers(dt);
   }

   // The I/O handlers
   if (ioHandlers != nullptr) {
      basic::List::Item* item = ioHandlers ->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         basic::IoHandler* p = static_cast<basic::IoHandler*>(pair->object());
         p->tcFrame(dt);
         item = item->getNext();
      }
   }

   // Process station inputs
   inputDevices(dt);

   // Update the simulation
   if (sim != nullptr) sim->tcFrame(dt);

   // Process station outputs
   outputDevices(dt);

   // Our major subsystems
   if (sim != nullptr && otw != nullptr) {
      basic::PairStream* playerList = sim->getPlayers();
      basic::List::Item* item = otw->getFirstItem();
      while (item != nullptr) {

         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         Otw* p = static_cast<Otw*>(pair->object());

         // Set ownship & player list
         p->setOwnship(ownship);
         p->setPlayerList(playerList);

         // TC frame
         p->tcFrame(dt);

         item = item->getNext();
      }
      if (playerList != nullptr) playerList->unref();
   }

   // Startup RESET timer --
   //    Sends an initial RESET pulse after timeout
   //    (Some simulation may need this)
   if (startupResetTimer >= 0) {
      startupResetTimer -= dt;
      if (startupResetTimer < 0) {
         this->event(RESET_EVENT);
      }
   }

   // Update the base class data
   BaseClass::updateTC(dt);
}

//------------------------------------------------------------------------------
// updateData() -- update non-time critical stuff here
//
// Note:
//   1) If bgRate and netRate are greater than zero then threads are created
//      to manage these tasks, otherwise we'll do it.
//   2) updateData() for components of graphical displays are handled by
//      their display managers (e.g., graphics::GlutDisplay).
//------------------------------------------------------------------------------
void Station::updateData(const LCreal dt)
{
   // Create a background thread (if needed)
   if (getBackgroundRate() > 0 && !doWeHaveTheBgThread()) {
      createBackgroundProcess();
   }

   // Our simulation model and OTW interfaces (if no separate thread)
   if (getBackgroundRate() == 0 && !doWeHaveTheBgThread()) {
      processBackgroundTasks(dt);
   }

   // Create a network thread (if needed)
   if (getNetworkRate() > 0 && networks != nullptr && !doWeHaveTheNetThread()) {
      createNetworkProcess();
   }

   // Our interoperability networks (if no separate thread)
   if (getNetworkRate() == 0 && networks != nullptr && !doWeHaveTheNetThread()) {
      processNetworkInputTasks(dt);
      processNetworkOutputTasks(dt);
   }

   // ---
   // Background processing of the data recorders
   // ---
   if (dataRecorder != nullptr) dataRecorder->processRecords();

   // Update base class data
   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool Station::shutdownNotification()
{
   // Tell the interoperability networks that we're shutting down
   if (networks != nullptr) {
      basic::List::Item* item = networks->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         basic::Component* p = static_cast<basic::Component*>(pair->object());
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
   }

   // Tell the I/O devices that we're shutting down
   if (ioHandlers != nullptr) {
      basic::List::Item* item = ioHandlers->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         basic::Component* p = static_cast<basic::Component*>(pair->object());
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
   }
   setSlotIoHandler(static_cast<basic::PairStream*>(nullptr));

   // Tell our simulation to shut down
   Simulation* s = getSimulation();
   if (s != nullptr) {
      s->event(SHUTDOWN_EVENT);
   }
   setOwnshipPlayer(nullptr);

   // Inform our OTW interfaces
   if (otw != nullptr) {
      basic::List::Item* item = otw ->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         basic::Component* p = static_cast<basic::Component*>(pair->object());
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
   }
   setSlotOutTheWindow(static_cast<basic::PairStream*>(nullptr));

   // Zero (unref) our thread objects (of any).  The thread's functions have ref()'d
   // these objects, so they won't be deleted until the threads terminate, which they
   // will based on our BaseClass::isShutdown() function.  But at least we won't
   // mistakenly think that they're still around.
   tcThread = nullptr;
   netThread = nullptr;
   bgThread = nullptr;

   // propagate shutdown event to base/component, and all subcomponents
   bool shutdown = BaseClass::shutdownNotification();

   // probably should move all setSlot...(0) lines here,
   // but networks was the only obviously crashing problem
   setSlotNetworks(nullptr);

   // remove the reset timer
   setSlotStartupResetTime(nullptr);

   // Shutdown the data recorder
   if (dataRecorder != nullptr) dataRecorder->event(SHUTDOWN_EVENT);

   return shutdown;
}


//------------------------------------------------------------------------------
// inputDevices() -- Process station inputs outputs
//------------------------------------------------------------------------------
void Station::inputDevices(const LCreal dt)
{
   if (ioHandlers != nullptr) {
     basic::List::Item* item = ioHandlers ->getFirstItem();
     while (item != nullptr) {
        basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
        basic::IoHandler* p = static_cast<basic::IoHandler*>(pair->object());
        p->inputDevices(dt);
        item = item->getNext();
     }
   }
}

//------------------------------------------------------------------------------
// outputDevices() -- Process station hardware outputs
//------------------------------------------------------------------------------
void Station::outputDevices(const LCreal dt)
{
   if (ioHandlers != nullptr) {
     basic::List::Item* item = ioHandlers ->getFirstItem();
     while (item != nullptr) {
        basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
        basic::IoHandler* p = static_cast<basic::IoHandler*>(pair->object());
        p->outputDevices(dt);
        item = item->getNext();
     }
   }
}

//------------------------------------------------------------------------------
// createTimeCriticalProcess() -- Create the time-critical thread
//------------------------------------------------------------------------------
void Station::createTimeCriticalProcess()
{
   if ( tcThread == nullptr ) {
      tcThread = new TcThread(this, getTimeCriticalPriority(), getTimeCriticalRate());
      tcThread->unref(); // 'tcThread' is a safe_ptr<>

      if (tcStackSize > 0) tcThread->setStackSize( tcStackSize );

      bool ok = tcThread->create();
      if (!ok) {
         tcThread = nullptr;
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Station::createTimeCriticalProcess(): ERROR, failed to create the thread!" << std::endl;
         }
      }
    }
}

//------------------------------------------------------------------------------
// createNetworkProcess() -- Create the network thread
//------------------------------------------------------------------------------
void Station::createNetworkProcess()
{
   if ( netThread == nullptr ) {
      netThread = new NetThread(this, getNetworkPriority(), getNetworkRate());
      netThread->unref(); // 'netThread' is a safe_ptr<>

      if (netStackSize > 0) netThread->setStackSize( netStackSize );

      bool ok = netThread->create();
      if (!ok) {
         netThread = nullptr;
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Station::createNetworkProcess(): ERROR, failed to create the thread!" << std::endl;
         }
      }
   }
}

//------------------------------------------------------------------------------
// createBackgroundProcess() -- Create the background task thread
//------------------------------------------------------------------------------
void Station::createBackgroundProcess()
{
   if ( bgThread == nullptr ) {
      bgThread = new BgThread(this, getBackgroundPriority(), getBackgroundRate());
      bgThread->unref(); // 'bgThread' is a safe_ptr<>

      if (bgStackSize > 0) bgThread->setStackSize( bgStackSize );

      bool ok = bgThread->create();
      if (!ok) {
         bgThread = nullptr;
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Station::createBackgroundProcess(): ERROR, failed to create the thread!" << std::endl;
         }
      }
   }
}

//------------------------------------------------------------------------------
// processTimeCriticalTasks() -- Process T/C tasks
//------------------------------------------------------------------------------
void Station::processTimeCriticalTasks(const LCreal dt)
{
   for (unsigned int jj = 0; jj < getFastForwardRate(); jj++) {
      tcFrame( dt );
   }
}

//------------------------------------------------------------------------------
// processBackgroundTasks() -- Process the background models and interfaces
//------------------------------------------------------------------------------
void Station::processBackgroundTasks(const LCreal dt)
{
   // Note: interoperability networks are handled by
   // processNetworkInputTasks() and processNetworkOutputTasks()

   // The I/O handlers
   if (ioHandlers != nullptr) {
      basic::List::Item* item = ioHandlers ->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         basic::IoHandler* p = static_cast<basic::IoHandler*>(pair->object());
         p->updateData(dt);
         item = item->getNext();
      }
   }

   // Our simulation model
   if (sim != nullptr) sim->updateData(dt);

   // Our OTW interfaces
   if (otw != nullptr) {
      basic::List::Item* item = otw ->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         Otw* p = static_cast<Otw*>(pair->object());
         p->updateData(dt);
         item = item->getNext();
      }
   }
}


//------------------------------------------------------------------------------
// processNetworkInputTasks() -- Process network input tasks
//------------------------------------------------------------------------------
void Station::processNetworkInputTasks(const LCreal dt)
{
   basic::safe_ptr<basic::PairStream> networks( getNetworks() );
   if (networks != nullptr) {
      basic::List::Item* item = networks->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         NetIO* p = static_cast<NetIO*>(pair->object());

         p->inputFrame( dt );

         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// processNetworkOutputTasks() -- Process network output tasks
//------------------------------------------------------------------------------
void Station::processNetworkOutputTasks(const LCreal dt)
{
   basic::safe_ptr<basic::PairStream> networks( getNetworks() );
   if (networks != nullptr) {
      basic::List::Item* item = networks->getFirstItem();
      while (item != nullptr) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         NetIO* p = static_cast<NetIO*>(pair->object());

         p->outputFrame( dt );

         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Returns the simulation model
Simulation* Station::getSimulation()
{
   return sim;
}

// Returns the simulation model (const version)
const Simulation* Station::getSimulation() const
{
   return sim;
}

// Returns the ownship (primary) player
Player* Station::getOwnship()
{
   return ownship;
}

// Returns the ownship (primary) player (const version)
const Player* Station::getOwnship() const
{
   return ownship;
}

// Returns the ownship's name
const basic::String* Station::getOwnshipName() const
{
   return ownshipName;
}

// Get the player list; pre-ref()'d by the Simulation class
basic::PairStream* Station::getPlayers()
{
    return ((getSimulation() != nullptr) ? getSimulation()->getPlayers() : nullptr);
}

// Get the player list (const version); pre-ref()'d by the Simulation class
const basic::PairStream* Station::getPlayers() const
{
    return ((getSimulation() != nullptr) ? getSimulation()->getPlayers() : nullptr);
}

// Returns the list of OTW systems
basic::PairStream* Station::getOutTheWindowList()
{
   return otw;
}

// Returns the list of OTW systems (const version)
const basic::PairStream* Station::getOutTheWindowList() const
{
   return otw;
}

// List of interoperability network handlers (e.g., DIS, HLA, TENA)
basic::PairStream* Station::getNetworks()
{
   return networks;
}

// List of interoperability network handlers (e.g., DIS, HLA, TENA) (const version)
const basic::PairStream* Station::getNetworks() const
{
   return networks;
}

// I/O handlers
basic::PairStream* Station::getIoHandlers()
{
   return ioHandlers;
}

// I/O handlers (const version)
const basic::PairStream* Station::getIoHandlers() const
{
   return ioHandlers;
}

// Returns the data recorder
DataRecorder* Station::getDataRecorder()
{
   return dataRecorder;
}

// Returns the data recorder (const version)
const DataRecorder* Station::getDataRecorder() const
{
   return dataRecorder;
}

// Time-critical thread rate (Hz)
LCreal Station::getTimeCriticalRate() const
{
   return tcRate;
}

// Time-critical thread priority
LCreal Station::getTimeCriticalPriority() const
{
   return tcPri;
}

// Time-critical thread stack size (bytes or zero for default)
unsigned int Station::getTimeCriticalStackSize() const
{
   return tcStackSize;
}

// Do we have a T/C thread?
bool Station::doWeHaveTheTcThread() const
{
   return (tcThread != nullptr);
}

// Pre-ref() pointer to the T/Cthread
basic::Thread* Station::getTcThread()
{
   basic::Thread* p = nullptr;
   if (tcThread != nullptr) {
      p = tcThread.getRefPtr();
   }
   return p;
}

// Background thread rate (Hz)
LCreal Station::getBackgroundRate() const
{
   return bgRate;
}

// Background thread priority
LCreal Station::getBackgroundPriority() const
{
   return bgPri;
}

// Background thread stack size (bytes or zero for default)
unsigned int Station::getBackgroundStackSize() const
{
   return bgStackSize;
}

// Do we have a background thread?
bool Station::doWeHaveTheBgThread() const
{
   return (bgThread != nullptr);
}

// Pre-ref() pointer to the Background thread
basic::Thread* Station::getBgThread()
{
   basic::Thread* p = nullptr;
   if (bgThread != nullptr) {
      p = bgThread.getRefPtr();
   }
   return p;
}

// Network thread rate (Hz)
LCreal Station::getNetworkRate() const
{
   return netRate;
}

// Network thread priority
LCreal Station::getNetworkPriority() const
{
   return netPri;
}

// Network thread stack size (bytes or zero for default)
unsigned int Station::getNetworkStackSize() const
{
   return netStackSize;
}

// Do we have a network thread?
bool Station::doWeHaveTheNetThread() const
{
   return (netThread != nullptr);
}

// Pre-ref() pointer to the Network thread
basic::Thread* Station::getNetThread()
{
   basic::Thread* p = nullptr;
   if (netThread != nullptr) {
      p = netThread.getRefPtr();
   }
   return p;
}

// Is Timer::updateTimers() being called from our updateTC()
bool Station::isUpdateTimersEnabled() const
{
   return tmrUpdateEnbl;
}

//------------------------------------------------------------------------------
// Set updateTimers() call enable flag
//------------------------------------------------------------------------------
bool Station::setUpdateTimersEnable(const bool enb)
{
   tmrUpdateEnbl = enb;
   return true;
}

//------------------------------------------------------------------------------
// Set thread stack sizes
//------------------------------------------------------------------------------
bool Station::setTimeCriticalStackSize(const unsigned int bytes)
{
   tcStackSize = bytes;
   return true;
}

bool Station::setNetworkStackSize(const unsigned int bytes)
{
   netStackSize = bytes;
   return true;
}

bool Station::setBackgroundStackSize(const unsigned int bytes)
{
   bgStackSize = bytes;
   return true;
}

//------------------------------------------------------------------------------
// Set thread handle functions
//------------------------------------------------------------------------------
void Station::setTcThread(basic::Thread* h)
{
   if (tcThread != nullptr) tcThread->terminate();
   tcThread = h;
}

void Station::setNetThread(basic::Thread* h)
{
   if (netThread != nullptr) netThread->terminate();
   netThread = h;
}

void Station::setBgThread(basic::Thread* h)
{
   if (bgThread != nullptr) bgThread->terminate();
   bgThread = h;
}

//------------------------------------------------------------------------------
// setOwnshipByName() -- set our ownship by name
//------------------------------------------------------------------------------
bool Station::setOwnshipByName(const char* const newOS)
{
   bool set = false;
   basic::PairStream* pl = nullptr;
   if (sim != nullptr) pl = sim->getPlayers();

   // Look for this ownship in our list of players
   if (pl != nullptr) {
      if (newOS != nullptr) {
         basic::Pair* p = pl->findByName(newOS);
         if (p != nullptr) {
            Player* newOwnship = static_cast<Player*>(p->object());
            if (newOwnship != ownship) {
               // Ok, we found the new ownship and it IS a different
               // player then the previous ownship ...
               setOwnshipPlayer( newOwnship );
               set = true;
            }
         }

      }

      // Cleanup
      pl->unref();
      pl = nullptr;
   }

   return set;
}

//------------------------------------------------------------------------------
// setOwnshipPlayer() -- set this player as our ownship
//------------------------------------------------------------------------------
bool Station::setOwnshipPlayer(Player* const newOS)
{
    // Is it already own ownship?  Yes, then nothing else to do.
    if (newOS == ownship) return true;

    // When we're just setting a null(0) ownship ...
    if (newOS == nullptr) {
        // Unref the old player
        if (ownshipName != nullptr) { ownshipName->unref(); ownshipName = nullptr; }
        if (ownship != nullptr) {
            ownship->event(ON_OWNSHIP_DISCONNECT);
            ownship->unref();
            ownship = nullptr;
        }
        return true;
    }

    // Look for this ownship in our list of players
    bool set = false;
    basic::PairStream* pl = sim->getPlayers();
    if (pl != nullptr) {
        basic::List::Item* item = pl->getFirstItem();
        while (item != nullptr && !set) {
            basic::Pair* pair = dynamic_cast<basic::Pair*>(item->getValue());
            if (pair != nullptr) {
                Player* ip = dynamic_cast<Player*>( pair->object() );
                if (ip == newOS && ip->isLocalPlayer()) {
                    // Unref the old stuff
                    if (ownshipName != nullptr) { ownshipName->unref(); ownshipName = nullptr; }
                    if (ownship != nullptr) {
                        ownship->event(ON_OWNSHIP_DISCONNECT);
                        ownship->unref();
                        ownship = nullptr;
                    }
                    // Ok, we found the player -- make it our ownship
                    ownship = newOS;
                    ownship->ref();
                    ownshipName = pair->slot();
                    ownshipName->ref();
                    ownship->event(ON_OWNSHIP_CONNECT);
                    set = true;
                }
            }
            item = item->getNext();
        }

        pl->unref();
        pl = nullptr;
    }
    return set;
}

//------------------------------------------------------------------------------
// Sets the data recorder
//------------------------------------------------------------------------------
bool Station::setDataRecorder(DataRecorder* const p)
{
   if (dataRecorder != nullptr) { dataRecorder->container(nullptr); dataRecorder->unref(); }
   dataRecorder = p;
   if (dataRecorder != nullptr) { dataRecorder->container(this); dataRecorder->ref(); }
   return true;
}


//-----------------------------------------------------------------------------
// setSlotSimulation() -- Sets a pointer to our simulation subsystem
//-----------------------------------------------------------------------------
bool Station::setSlotSimulation(Simulation* const p)
{
    if (sim != nullptr) {
        sim->container(nullptr);
        sim->unref();
    }
    sim = p;
    if (sim != nullptr) {
        sim->ref();
        sim->container(this);
    }
    return true;
}

//-----------------------------------------------------------------------------
// setSlotOutTheWindow() -- Sets a list of Out-The-Window subsystems
//-----------------------------------------------------------------------------
bool Station::setSlotOutTheWindow(Otw* const p)
{
    basic::PairStream* list = new basic::PairStream();
    basic::Pair* pair = new basic::Pair("1",p);
    list->put( pair );
    pair->unref();
    bool ok = setSlotOutTheWindow(list);
    list->unref();
    return ok;
}

bool Station::setSlotOutTheWindow(basic::PairStream* const list)
{
   basic::PairStream* newList = nullptr;

   // Make sure the new list only has OTW type objects
   if (list != nullptr) {
      for (basic::List::Item* item = list->getFirstItem(); item != nullptr; item = item->getNext()) {
            basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
            Otw* p = dynamic_cast<Otw*>(pair->object());
            if (p != nullptr) {
            if (newList == nullptr) {
               newList = new basic::PairStream();
            }
            newList->put(pair);  // Add this OTW to our new OTW list
                p->container(this);
            }
         else if (isMessageEnabled(MSG_WARNING)) {
                // Not of the proper type
                std::cerr << "Player::setOutTheWindow: OTW at slot \"" << pair->slot() << "\" is not of type Otw" << std::endl;
            }
        }
    }

   // Remove the old OTW interfaces
   if (otw != nullptr) {

      basic::safe_ptr<basic::PairStream> oldList( otw );
      otw = nullptr;

      // we are no longer the container for these old OTW interfaces
      for (basic::List::Item* item = oldList->getFirstItem(); item != nullptr; item = item->getNext()) {
         basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
         Component* p = static_cast<Component*>(pair->object());
         p->container(nullptr);
      }
   }

   // Set the pointer to the list of OTW interfaces
   otw = newList;

   return true;
}

//-----------------------------------------------------------------------------
// setSlotIoHandler() -- Sets a list of I/O handlers
//-----------------------------------------------------------------------------
bool Station::setSlotIoHandler(basic::IoHandler* const p)
{
    basic::PairStream* list = new basic::PairStream();
    list->put( new basic::Pair("1",p) );
    return setSlotIoHandler(list);
}

bool Station::setSlotIoHandler(basic::PairStream* const list)
{
    bool ok = true;

    // Remove the old list
    if (ioHandlers != nullptr) {
        // we are no longer the container for these handlers
        for (basic::List::Item* item = ioHandlers->getFirstItem(); item != nullptr; item = item->getNext()) {
            basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
            basic::Component* p = static_cast<basic::Component*>(pair->object());
            p->container(nullptr);
        }
        ioHandlers = nullptr;
    }

    // Set our list pointer
    ioHandlers = list;

    // Make sure the new list is setup correctly
    if (ioHandlers != nullptr) {
        for (basic::List::Item* item = ioHandlers->getFirstItem(); item != nullptr; item = item->getNext()) {
            basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
            basic::IoHandler* p = dynamic_cast<basic::IoHandler*>(pair->object());
            if (p != nullptr) {
                // We are its container
                p->container(this);
            }
            else {
                // Not of the proper type
                std::cerr << "Player::setSlotIoHandler: Slot \"" << pair->slot() << "\" is not of type basic::IoHandler" << std::endl;
                ok = false;
            }
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotOwnshipName() -- sets the ownship name to the new string
//------------------------------------------------------------------------------
bool Station::setSlotOwnshipName(const basic::String* const newName)
{
   if (ownshipName != nullptr) ownshipName->unref();
   ownshipName = newName;
   if (ownshipName != nullptr) ownshipName->ref();
   return true;
}

//------------------------------------------------------------------------------
// setSlotNetworks() -- Set our list of networks
//------------------------------------------------------------------------------
bool Station::setSlotNetworks(basic::PairStream* const a)
{
    bool ok = true;

    // Remove the old networks list
    if (networks != nullptr) {
        // we are no longer the container for these networks
        for (basic::List::Item* item = networks->getFirstItem(); item != nullptr; item = item->getNext()) {
            basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
            NetIO* p = static_cast<NetIO*>(pair->object());
            p->container(nullptr);
        }
    }

    // Set our network list pointer
    networks = a;

    // Make sure the new network list is setup correctly
    if (networks != nullptr) {
        for (basic::List::Item* item = networks->getFirstItem(); item != nullptr; item = item->getNext()) {
            basic::Pair* pair = static_cast<basic::Pair*>(item->getValue());
            NetIO* p = dynamic_cast<NetIO*>(pair->object());
            if (p != nullptr) {
                // We are this network's container
                p->container(this);
            }
            else {
                // Not of the proper type
                std::cerr << "Player::setSlotNetworks: network at slot \"" << pair->slot() << "\" is not of class type NetIO" << std::endl;
                ok = false;
            }
        }
    }
    return ok;
}


//------------------------------------------------------------------------------
// setSlotTimeCriticalRate() -- Sets the T/C thread rate (hz)
//------------------------------------------------------------------------------
bool Station::setSlotTimeCriticalRate(const basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        LCreal rate = num->getReal();
        if (rate > 0) {
            tcRate = rate;
            ok = true;
        }
        else {
            std::cerr << "Station::setTimeCriticalRate: Thread rate is invalid; must be greater than zero" << std::endl;
        }
    }
    return ok;
}


//------------------------------------------------------------------------------
// setSlotTimeCriticalPri() -- Sets the T/C thread priority
//------------------------------------------------------------------------------
bool Station::setSlotTimeCriticalPri(const basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        LCreal pri = num->getReal();
        if (pri >= 0 && pri <= 1.0f) {
            tcPri = pri;
            ok = true;
        }
        else {
            std::cerr << "Station::setTimeCriticalPri: Priority is invalid, range: [0 .. 1]" << std::endl;
        }
    }
    return ok;
}

bool Station::setSlotTimeCriticalStackSize(const basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        int isize = num->getInt();
        if (isize >= 0) {
            ok = setTimeCriticalStackSize(static_cast<unsigned int>(isize));
        }
    }
    return ok;
}


//------------------------------------------------------------------------------
// setSlotNetworkRate() -- Sets the network thread rate (hz)
//------------------------------------------------------------------------------
bool Station::setSlotNetworkRate(const basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        LCreal rate = num->getReal();
        if (rate > 0) {
            netRate = rate;
            ok = true;
        }
        else {
            std::cerr << "Station::setNetworkRate: Thread rate is invalid; must be greater than zero" << std::endl;
        }
    }
    return ok;
}


//------------------------------------------------------------------------------
// setSlotNetworkPri() -- Sets the network thread priority
//------------------------------------------------------------------------------
bool Station::setSlotNetworkPri(const basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        LCreal pri = num->getReal();
        if (pri >= 0 && pri <= 1.0f) {
            netPri = pri;
            ok = true;
        }
        else {
            std::cerr << "Station::setSlotNetworkPri: Priority is invalid, range: [0 .. 1]" << std::endl;
        }
    }
    return ok;
}

bool Station::setSlotNetworkStackSize(const basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        int isize = num->getInt();
        if (isize >= 0) {
            ok = setNetworkStackSize(static_cast<unsigned int>(isize));
        }
    }
    return ok;
}


//------------------------------------------------------------------------------
// setSlotBackgroundRate() -- Sets the background thread rate (hz)
//------------------------------------------------------------------------------
bool Station::setSlotBackgroundRate(const basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        LCreal rate = num->getReal();
        if (rate >= 0 ) {
            bgRate = rate;
            ok = true;
        }
        else {
            std::cerr << "Station::setSlotBackgroundRate(): Thread rate is invalid; must be greater than or equal zero." << std::endl;
        }
    }
    return ok;
}


//------------------------------------------------------------------------------
// setSlotBackgroundPri() -- Sets the background thread priority
//------------------------------------------------------------------------------
bool Station::setSlotBackgroundPri(const basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        LCreal pri = num->getReal();
        if (pri >= 0 && pri <= 1.0f) {
            bgPri = pri;
            ok = true;
        }
        else {
            std::cerr << "Station::setSlotBackgroundPri: Priority is invalid, range: [0 .. 1]" << std::endl;
        }
    }
    return ok;
}

bool Station::setSlotBackgroundStackSize(const basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        int isize = num->getInt();
        if (isize >= 0) {
            ok = setBackgroundStackSize(static_cast<unsigned int>(isize));
        }
    }
    return ok;
}


//------------------------------------------------------------------------------
// setSlotStartupResetTime() -- Sets the startup RESET pulse timer
//------------------------------------------------------------------------------
bool Station::setSlotStartupResetTime(const basic::Time* const num)
{
    if (startupResetTimer0 != nullptr) {
        startupResetTimer0->unref();
        startupResetTimer0 = nullptr;
        startupResetTimer = -1.0;
    }
    startupResetTimer0 = num;
    if (startupResetTimer0 != nullptr) {
        startupResetTimer0->ref();
        startupResetTimer = basic::Seconds::convertStatic(*startupResetTimer0);
    }
    return true;
}


//------------------------------------------------------------------------------
// Sets the fast forward rate
//------------------------------------------------------------------------------
bool Station::setFastForwardRate(const unsigned int r)
{
   fastForwardRate = r;
   return true;
}

//------------------------------------------------------------------------------
// Sets the fast forward rate
//------------------------------------------------------------------------------
bool Station::setSlotFastForwardRate(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const int ii = msg->getInt();
      if (ii >= 0) {
         ok = setFastForwardRate( ii );
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Enables/disables the basic::Timer::updateTimers() call
//------------------------------------------------------------------------------
bool Station::setSlotEnableUpdateTimers(const basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setUpdateTimersEnable( msg->getBoolean() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
basic::Object* Station::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Station::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    // simulation model
    if (sim != nullptr) {
        indent(sout,i+j);
        sout << "simulation: " << std::endl;

        sim->serialize(sout,(i+j));
    }

    // I/O handlers
    if (ioHandlers != nullptr) {
        indent(sout,i+j);
        sout << "ioHandler: {" << std::endl;
        ioHandlers->serialize(sout,i+j+4);
        indent(sout,i+j);
        sout << "}" << std::endl;
    }

    // OTW subsystems
    if (otw != nullptr) {
        indent(sout,i+j);
        sout << "otw: {" << std::endl;
        otw->serialize(sout,i+j+4);
        indent(sout,i+j);
        sout << "}" << std::endl;
    }

    // networks
    if (networks != nullptr) {
        indent(sout,i+j);
        sout << "networks: {" << std::endl;
        networks->serialize(sout,i+j+4);
        indent(sout,i+j);
        sout << "}" << std::endl;
    }

    // tcRate: Time-critical thread update rate (Hz)
    indent(sout,i+j);
    sout << "tcRate: " << tcRate << std::endl;

    // tcPriority: Time-critical thread priority
    indent(sout,i+j);
    sout << "tcPriority: " << tcPri << std::endl;

    // netRate: Network thread rate (Hz)
    indent(sout,i+j);
    sout << "netRate: " << netRate << std::endl;

    // netPriority: Network thread priority
    indent(sout,i+j);
    sout << "netPriority: " << netPri << std::endl;

    // fastForwardRate:
    if (fastForwardRate != DEFAULT_FAST_FORWARD_RATE) {
      indent(sout,i+j);
      sout << "fastForwardRate: " << fastForwardRate << std::endl;
    }

    // startupResetTime: Startup (initial) RESET pulse timer value (basic::Time)
    if (startupResetTimer0 != nullptr) {
        indent(sout,i+j);
        sout << "startupResetTime: " << *startupResetTimer0 << std::endl;
    }

    if (ownshipName != nullptr) {
        indent(sout,i+j);
        sout << "ownship: " << *ownshipName << std::endl;
    }

    // don't care about component stuff right now
    //BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
    }

    return sout;
}

//=============================================================================
// Time-critical thread
//=============================================================================
IMPLEMENT_SUBCLASS(TcThread,"TcThread")
EMPTY_SLOTTABLE(TcThread)
EMPTY_COPYDATA(TcThread)
EMPTY_DELETEDATA(TcThread)
EMPTY_SERIALIZER(TcThread)

TcThread::TcThread(basic::Component* const parent, const LCreal priority, const LCreal rate)
      : basic::ThreadPeriodicTask(parent, priority, rate)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long TcThread::userFunc(const LCreal dt)
{
   Station* station = static_cast<Station*>(getParent());
   station->processTimeCriticalTasks(dt);
   return 0;
}

//=============================================================================
// Interoperability Networks thread
//=============================================================================
IMPLEMENT_SUBCLASS(NetThread,"NetThread")
EMPTY_SLOTTABLE(NetThread)
EMPTY_COPYDATA(NetThread)
EMPTY_DELETEDATA(NetThread)
EMPTY_SERIALIZER(NetThread)

NetThread::NetThread(basic::Component* const parent, const LCreal priority, const LCreal rate)
      : basic::ThreadPeriodicTask(parent, priority, rate)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long NetThread::userFunc(const LCreal dt)
{
   Station* station = static_cast<Station*>(getParent());
   station->processNetworkInputTasks(dt);
   station->processNetworkOutputTasks(dt);
   return 0;
}

//=============================================================================
// Background thread
//=============================================================================
IMPLEMENT_SUBCLASS(BgThread,"BgThread")
EMPTY_SLOTTABLE(BgThread)
EMPTY_COPYDATA(BgThread)
EMPTY_DELETEDATA(BgThread)
EMPTY_SERIALIZER(BgThread)

BgThread::BgThread(basic::Component* const parent, const LCreal priority, const LCreal rate)
      : basic::ThreadPeriodicTask(parent, priority, rate)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long BgThread::userFunc(const LCreal dt)
{
   Station* station = static_cast<Station*>(getParent());
   station->processBackgroundTasks(dt);
   return 0;
}

} // End simulation namespace
} // End oe namespace
