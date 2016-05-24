//------------------------------------------------------------------------------
// Class:  Station
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Station_H__
#define __oe_simulation_Station_H__

#include "openeaagles/base/Component.h"

namespace oe {
   namespace base {
      class IoHandler;
      class Number;
      class Thread;
      class Time;
   }
namespace simulation {
   class DataRecorder;
   class Simulation;
   class Player;
   class Otw;
   class NetIO;

//------------------------------------------------------------------------------
// Class:  Station
//
// Description:  Application's top level container class with support for the
//               simulation model, controls & displays, network interfaces
//               and the out-the-window visual systems.  Also, contains default
//               support for the time-critical and network threads.
//
// Factory name: Station
// Slots --
//    simulation        <Simulation>      ! Simulation model (default: 0)
//
//    networks     <base::PairStream>    ! List of interoperability network models (DIS, HLA, TENA) (default: 0)
//
//    otw               <Otw>             ! Single Out-The-Window (OTW) visual system (default: 0)
//    otw          <base::PairStream>    ! List of Out-The-Window (OTW) visual systems
//
//    ioHandler    <base::IoHandler>     ! Single I/O data handler (default: 0)
//    ioHandler    <base::PairStream>    ! List of I/O data handlers
//
//    ownship           <base::String>   ! Player name of our ownship (primary) player (default: 0)
//
//    tcRate            <base::Number>   ! Time-critical thread rate (Hz) (default: 50hz)
//    tcPriority        <base::Number>   ! Time-critical thread priority  (default: DEFAULT_TC_THREAD_PRI)
//    tcStackSize       <base::Number>   ! Time-critical thread stack size (default: <system default size>)
//
//    fastForwardRate   <base::Number>   ! Fast forward rate for time critical functions
//                                        ! (i.e., the number of times updateTC() is called per frame).
//                                        ! (default: DEFAULT_FAST_FORWARD_RATE)
//
//    netRate           <base::Number>   ! Network thread rate (Hz) (default: 0hz)
//    netPriority       <base::Number>   ! Network thread priority (default: DEFAULT_NET_THREAD_PRI )
//    netStackSize      <base::Number>   ! Network thread stack size (default: <system default size>)
//
//    bgRate            <base::Number>   ! Background thread rate (Hz) (default: 0 -- no thread)
//    bgPriority        <base::Number>   ! Background thread priority (default: DEFAULT_BG_THREAD_PRI )
//    bgStackSize       <base::Number>   ! Background thread stack size (default: <system default size>)
//
//    startupResetTime  <base::Time>     ! Startup (initial) RESET event timer value (default: no reset event)
//                                        !  (some simulations may need this -- let it run a few initial frames then reset)
//
//    enableUpdateTimers <base::Boolean> ! Enable calling base::Timers::updateTimers() from updateTC() (default: false)
//
//    dataRecorder      <DataRecorder>    ! Our Data Recorder
//
//
// Ownship player:
//
//    The ownship player is the primary focus of our controls & displays (C&D).
//    It is set by the 'ownship' slot, but it can be changed using the
//    setOwnshipByName() function.  For example, a cockpit's C&D can be
//    focused on one local player then it can 'jump' (i.e., switched) to a
//    different local player.  If you're planning to jump between ownships then
//    ensure that your C&D software is written accordingly.
//
//    When the ownship player is changed, an ON_OWNSHIP_DISCONNECT event
//       is sent to the previous ownship player and then an ON_OWNSHIP_CONNECT
//       event is sent to the new ownship player.
//
//
// Threads and main work functions:
//
//    1) Currently supports the creation of three optional threads for (a) time-critical,
//       (b) interoperabilty networks and (c) background tasks.  These are created by
//       the virtual functions createTimeCriticalProcess(), createNetworkProcess()
//       and createBackgroundProcess(), respectively.  Users can derive from Station Class
//       and create their own threads (e.g., to sync any of the threads with an
//       external interrupt).
//
//    2) Thread priorities are from zero (lowest) to one (highest).
//       (see base/Thread.h)
//
//    3) updateTC() -- The main application can use createTimeCriticalProcess()
//       to create a thread, which will run at 'tcRate' Hz and 'tcPriority'
//       priority, that will call our updateTC(); or the application can call
//       our updateTC() function directly.
//
//       a: The updateTC() function calls the updateTC() for the Simulation Class,
//          OTW models, and updates the I/O handlers;
//
//       b: And updateTC() calls the static function base::Timer::updateTimers()
//          if isUpdateTimersEnabled() is true (i.e., slot 'enableUpdateTimers');
//
//       c: To sync hardware I/O with the simulation exec, the two functions,
//          inputDevices() and outputDevices(), are called from our updateTC()
//          just before and just after, respectively, the call to the simulation's
//          updateTC().  Any slow I/O (e.g., RS-232) or blocked I/O should not be
//          in these functions.
//
//    4) updateData() -- The main application will need to call updateData()
//       directly at its required (i.e., as needed) rate.  By default, this
//       function will call the simulation exec and interoperability network
//       updateData() functions.  However, ...
//
//       a: If the background thread rate, 'bgRate', is set greater than zero,
//          then our updateData() will create a separate thread to call the
//          simulation and otw models' updateData() functions, otherwise
//       they're called from our updateData().
//
//       b: The interoperabilty networks (e.g., DIS, HLA, TENA, etc) are updated
//          using the two functions, inputFrame() and outputFrame(), which are
//          called from the processNetworkInputTasks() and processNetworkOutputTasks()
//          functions, respectively.
//
//          If the network thread rate, 'netRate', is set greater than zero,
//          then our updateData() will create a separate thread to call the
//          interoperability network functions, otherwise they're called from
//          our updateData().
//
//    5) The time-critical and background functions for the OTW models are
//       called from the same thread, or threads, as the simulation model's
//       functions are called from.
//
//    6) When subclassing off of this class for your application, the
//       convention is that the updateData() function for any graphic
//       component (see graphics::Graphic) is called by their display
//       manager (see graphics::GlutDisplay) and therefore from the
//       display manager's thread.
//
//
// Shutdown:
//
//    At shutdown, the user application must send a SHUTDOWN_EVENT event
//    to this object.
//
//------------------------------------------------------------------------------
class Station : public base::Component
{
   DECLARE_SUBCLASS(Station,base::Component)

public:
   // Default priorities
   static const double DEFAULT_TC_THREAD_PRI;
   static const double DEFAULT_BG_THREAD_PRI;
   static const double DEFAULT_NET_THREAD_PRI;

public:
   Station();

   Simulation* getSimulation();                              // Simulation model
   const Simulation* getSimulation() const;                  // Simulation model (const version)

   base::PairStream* getPlayers();                          // Simulation's player list; pre-ref()'d
   const base::PairStream* getPlayers() const;              // Simulation's player list; pre-ref()'d (const version)

   Player* getOwnship();                                     // The ownship (primary) player
   const Player* getOwnship() const;                         // The ownship (primary) player (const version)

   const base::String* getOwnshipName() const;              // The ownship's name
   virtual bool setOwnshipPlayer(Player* const newOS);       // Sets the ownship player
   virtual bool setOwnshipByName(const char* const newOS);   // Selects the ownship player by name

   base::PairStream* getOutTheWindowList();                 // OTW systems
   const base::PairStream* getOutTheWindowList() const;     // OTW systems (const version)

   base::PairStream* getNetworks();                         // Interoperability network handlers
   const base::PairStream* getNetworks() const;             // Interoperability network handlers (const version)

   base::PairStream* getIoHandlers();                       // I/O handlers
   const base::PairStream* getIoHandlers() const;           // I/O handlers (const version)

   DataRecorder* getDataRecorder();                          // Returns the data recorder
   const DataRecorder* getDataRecorder() const;              // Returns the data recorder (const version)
   virtual bool setDataRecorder(DataRecorder* const p);      // Sets the data recorder

   // Is Timer::updateTimers() being called from our updateTC()
   bool isUpdateTimersEnabled() const;
   virtual bool setUpdateTimersEnable(const bool enb);

   // ---
   // Use these functions to process the time-critical, background and network
   // tasks if you're managing your own thread(s) from your main application
   // or a derived Station class.
   // ---
   virtual void processTimeCriticalTasks(const double dt);
   virtual void processBackgroundTasks(const double dt);
   virtual void processNetworkInputTasks(const double dt);
   virtual void processNetworkOutputTasks(const double dt);

   // ---
   // Time-critical thread support
   // ---
   double getTimeCriticalRate() const;                       // Time-critical thread rate (Hz)
   double getTimeCriticalPriority() const;                   // Time-critical thread priority
   unsigned int getTimeCriticalStackSize() const;            // Time-critical thread stack size
   bool setTimeCriticalStackSize(const unsigned int bytes);  // Set Time-critical thread stack size  (bytes or zero for default)

   // Optionally called by the main application  to create a thread
   // that will call 'updateTC()' at 'getTimeCriticalRate()' Hz
   virtual void createTimeCriticalProcess();
   bool doWeHaveTheTcThread() const;                         // Do we have a T/C thread?

   // Fast forward rates used by processTimeCriticalTasks().
   //   (i.e., number of times Station::tcFrame() is called per frame)
   unsigned int getFastForwardRate() const { return fastForwardRate; } // Hz
   virtual bool setFastForwardRate(const unsigned int r);              // Hz

   // ---
   // Interoperability network(s) thread support
   // ---
   double getNetworkRate() const;                            // Network thread rate (Hz)
   double getNetworkPriority() const;                        // Network thread priority
   unsigned int getNetworkStackSize() const;                 // Network thread stack size
   bool setNetworkStackSize(const unsigned int bytes);       // Network thread stack size (bytes or zero for default)
   bool doWeHaveTheNetThread() const;                        // Do we have a network thread?

   // ---
   // Background thread support.
   // ---
   double getBackgroundRate() const;                         // Background thread rate (Hz)
   double getBackgroundPriority() const;                     // Background thread priority
   unsigned int getBackgroundStackSize() const;              // Background thread stack size
   bool setBackgroundStackSize(const unsigned int bytes);    // Background thread stack size (bytes or zero for default)
   bool doWeHaveTheBgThread() const;                         // Do we have a background thread?

   // ---
   // Slot functions
   // ---
   virtual bool setSlotSimulation(Simulation* const);
   virtual bool setSlotOutTheWindow(Otw* const);
   virtual bool setSlotOutTheWindow(base::PairStream* const);
   virtual bool setSlotIoHandler(base::IoHandler* const);
   virtual bool setSlotIoHandler(base::PairStream* const);
   virtual bool setSlotNetworks(base::PairStream* const);
   virtual bool setSlotTimeCriticalRate(const base::Number* const hz);
   virtual bool setSlotTimeCriticalPri(const base::Number* const);
   virtual bool setSlotTimeCriticalStackSize(const base::Number* const);
   virtual bool setSlotNetworkRate(const base::Number* const hz);
   virtual bool setSlotNetworkPri(const base::Number* const);
   virtual bool setSlotNetworkStackSize(const base::Number* const);
   virtual bool setSlotBackgroundRate(const base::Number* const hz);
   virtual bool setSlotBackgroundPri(const base::Number* const);
   virtual bool setSlotBackgroundStackSize(const base::Number* const);
   virtual bool setSlotStartupResetTime(const base::Time* const);
   virtual bool setSlotOwnshipName(const base::String* const);
   virtual bool setSlotFastForwardRate(const base::Number* const);
   virtual bool setSlotEnableUpdateTimers(const base::Number* const);

   void updateTC(const double dt = 0.0) override;
   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:
   virtual void inputDevices(const double dt);    // Handle device inputs
   virtual void outputDevices(const double dt);   // Handle device output

   base::Thread* getTcThread();                  // Pre-ref() pointer to the Time-critical thread
   void setTcThread(base::Thread* h);

   base::Thread* getNetThread();                 // Pre-ref() pointer to the Network thread
   void setNetThread(base::Thread* h);

   base::Thread* getBgThread();                  // Pre-ref() pointer to the Background thread
   void setBgThread(base::Thread* h);

   // base::Component protected functions
   bool shutdownNotification() override;

private:
   void initData();

   virtual void createNetworkProcess();    // Creates a network thread
   virtual void createBackgroundProcess(); // Creates a B/G thread

   Simulation* sim;                               // Simulation model
   base::safe_ptr<base::PairStream> otw;        // List of  Out-The-Window visual system interfaces
   base::safe_ptr<base::PairStream> networks;   // List of networks
   base::safe_ptr<base::PairStream> ioHandlers; // List of I/O data handlers
   Player* ownship;                          // Ownship (primary) player
   const base::String* ownshipName;         // Name of our ownship player
   bool tmrUpdateEnbl;                       // Enable base::Timers::updateTimers() call from updateTC()
   DataRecorder* dataRecorder;               // Data Recorder

   double tcRate;                            // Time-critical thread Rate (hz)
   double tcPri;                             // Priority of the time-critical thread (0->lowest, 1->highest)
   unsigned int tcStackSize;                 // Time-critical thread stack size (bytes or zero for system default size)
   base::safe_ptr<base::Thread> tcThread;  // The Time-critical thread
   unsigned int fastForwardRate;             // Time-critical thread fast forward rate

   double netRate;                           // Network thread Rate (hz)
   double netPri;                            // Priority of the Network thread (0->lowest, 1->highest)
   unsigned int netStackSize;                // Network thread stack size (bytes or zero for system default size)
   base::safe_ptr<base::Thread> netThread; // The optional network thread

   double bgRate;                            // Background thread Rate (hz)
   double bgPri;                             // Priority of the Background thread (0->lowest, 1->highest)
   unsigned int bgStackSize;                 // Background thread stack size (bytes or zero for system default size)
   base::safe_ptr<base::Thread> bgThread;  // The optional background thread

   double startupResetTimer;               // Startup RESET timer (sends a RESET_EVENT after timeout)
   const base::Time* startupResetTimer0;  // Init value of the startup RESET timer
};

}
}

#endif
