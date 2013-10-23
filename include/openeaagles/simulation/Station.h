//------------------------------------------------------------------------------
// Class:  Station
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Station_H__
#define __Eaagles_Simulation_Station_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
   namespace Basic {
      class IoHandler;
      class Number;
      class Thread;
      class Time;
   }
namespace Simulation {
   class DataRecorder;
   class Simulation;
   class Player;
   class Otw;
   class NetIO;
 
//------------------------------------------------------------------------------
// Class:  Station
//
// Description:  Applications's top level container class with support for the
//               simulation model, controls & displays, network interfaces
//               and the out-the-window visual systems.  Also, contains default
//               support for the time-critical and network threads.
//
// Form name: Station
// Slots --
//    simulation        <Simulation>      ! Simulation model (default: 0)
//
//    networks     <Basic::PairStream>    ! List of interoperability network models (DIS, HLA, TENA) (default: 0)
//
//    otw               <Otw>             ! Single Out-The-Window (OTW) visual system (default: 0)
//    otw          <Basic::PairStream>    ! List of Out-The-Window (OTW) visual systems 
//
//    ioHandler    <Basic::IoHandler>     ! Single I/O data handler (default: 0)
//    ioHandler    <Basic::PairStream>    ! List of I/O data handlers 
//
//    ownship           <Basic::String>   ! Player name of our ownship (primary) player (default: 0)
//
//    tcRate            <Basic::Number>   ! Time-critical thread rate (Hz) (default: 50hz)
//    tcPriority        <Basic::Number>   ! Time-critical thread priority  (default: DEFAULT_TC_THREAD_PRI)
//    fastForwardRate   <Basic::Number>   ! Fast forward rate for time critical functions
//                                        ! (i.e., the number of times updateTC() is called per frame).
//                                        ! (default: DEFAULT_FAST_FORWARD_RATE)
//
//    netRate           <Basic::Number>   ! Network thread rate (Hz) (default: 0hz)
//    netPriority       <Basic::Number>   ! Network thread priority (default: DEFAULT_NET_THREAD_PRI )
//
//    bgRate            <Basic::Number>   ! Background thread rate (Hz) (default: 0 -- no thread)
//    bgPriority        <Basic::Number>   ! Background thread priority (default: DEFAULT_BG_THREAD_PRI )
//
//    startupResetTime  <Basic::Time>     ! Startup (initial) RESET event timer value (default: no reset event)
//                                        !  (some simulations may need this -- let it run a few initial frames then reset)
//
//    enableUpdateTimers <Basic::Boolean> ! Enable calling Basic::Timers::updateTimers() from updateTC() (default: false)
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
//       (see basic/Thread.h)
//
//    3) updateTC() -- The main application can use createTimeCriticalProcess()
//       to create a thread, which will run at 'tcRate' Hz and 'tcPriority'
//       priority, that will call our updateTC(); or the application can call
//       our updateTC() function directly.
//
//       a: The updateTC() function calls the updateTC() for the Simulation Class,
//          OTW models, and updates the I/O handlers;
//
//       b: And updateTC() calls the static function Basic::Timer::updateTimers()
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
//       component (see BasicGL::Graphic) is called by their display
//       manager (see BasicGL::GlutDisplay) and therefore from the
//       display manager's thread.
//
//
// Shutdown:
//
//    At shutdown, the user application must send a SHUTDOWN_EVENT event
//    to this object.
//       
//------------------------------------------------------------------------------
class Station : public Basic::Component {
   DECLARE_SUBCLASS(Station,Basic::Component)

public:
   // Default priorities
   static const LCreal DEFAULT_TC_THREAD_PRI;
   static const LCreal DEFAULT_BG_THREAD_PRI;
   static const LCreal DEFAULT_NET_THREAD_PRI;

public:
   Station();

   Simulation* getSimulation();                          // Simulation model
   const Simulation* getSimulation() const;              // Simulation model (const version)

   Basic::PairStream* getPlayers();                      // Simulation's player list; pre-ref()'d
   const Basic::PairStream* getPlayers() const;          // Simulation's player list; pre-ref()'d (const version)

   Player* getOwnship();                                 // The ownship (primary) player
   const Player* getOwnship() const;                     // The ownship (primary) player (const version)

   const Basic::String* getOwnshipName() const;          // The ownship's name
   virtual bool setOwnshipPlayer(Player* const newOS);   // Sets the ownship player
   virtual bool setOwnshipByName(const char* const newOS); // Selects the ownship player by name

   Basic::PairStream* getOutTheWindowList();             // OTW systems
   const Basic::PairStream* getOutTheWindowList() const; // OTW systems (const version)

   Basic::PairStream* getNetworks();                     // Interoperability network handlers
   const Basic::PairStream* getNetworks() const;         // Interoperability network handlers (const version)

   Basic::PairStream* getIoHandlers();                   // I/O handlers
   const Basic::PairStream* getIoHandlers() const;       // I/O handlers (const version)

   DataRecorder* getDataRecorder();                      // Returns the data recorder
   const DataRecorder* getDataRecorder() const;          // Returns the data recorder (const version)
   virtual bool setDataRecorder(DataRecorder* const p);  // Sets the data recorder

   // Is Timer::updateTimers() being called from our updateTC()
   bool isUpdateTimersEnabled() const;
   virtual bool setUpdateTimersEnable(const bool enb);

   // ---
   // Use these functions to process the time-critical, background and network
   // tasks if you're managing your own thread(s) from your main application
   // or a derived Station class.
   // ---
   virtual void processTimeCriticalTasks(const LCreal dt);
   virtual void processBackgroundTasks(const LCreal dt);
   virtual void processNetworkInputTasks(const LCreal dt);
   virtual void processNetworkOutputTasks(const LCreal dt);

   // ---
   // Time-critical thread support
   // ---
   LCreal getTimeCriticalRate() const;        // Time-critical thread rate (Hz)
   LCreal getTimeCriticalPriority() const;    // Time-critical thread priority

   // Optionally called by the main application  to create a thread
   // that will call 'updateTC()' at 'getTimeCriticalRate()' Hz
   virtual void createTimeCriticalProcess();
   bool doWeHaveTheTcThread() const;         // Do we have a T/C thread?

   // Fast forward rates used by processTimeCriticalTasks().
   //   (i.e., number of times Station::tcFrame() is called per frame)
   unsigned int getFastForwardRate() const { return fastForwardRate; } // Hz
   virtual bool setFastForwardRate(const unsigned int r);              // Hz

   // ---
   // Interoperability network(s) thread support
   // ---
   LCreal getNetworkRate() const;      // Network thread rate (Hz)
   LCreal getNetworkPriority() const;     // Network thread priority
   bool doWeHaveTheNetThread() const;     // Do we have a network thread?

   // ---
   // Background thread support.
   // ---
   LCreal getBackgroundRate() const;      // Background thread rate (Hz)
   LCreal getBackgroundPriority() const;  // Background thread priority
   bool doWeHaveTheBgThread() const;      // Do we have a background thread?

   // ---
   // Slot functions
   // ---
   virtual bool setSlotSimulation(Simulation* const);
   virtual bool setSlotOutTheWindow(Otw* const);
   virtual bool setSlotOutTheWindow(Basic::PairStream* const);
   virtual bool setSlotIoHandler(Basic::IoHandler* const);
   virtual bool setSlotIoHandler(Basic::PairStream* const);
   virtual bool setSlotNetworks(Basic::PairStream* const);
   virtual bool setSlotTimeCriticalRate(const Basic::Number* const hz);
   virtual bool setSlotTimeCriticalPri(const Basic::Number* const);
   virtual bool setSlotNetworkRate(const Basic::Number* const hz);
   virtual bool setSlotNetworkPri(const Basic::Number* const);
   virtual bool setSlotBackgroundRate(const Basic::Number* const hz);
   virtual bool setSlotBackgroundPri(const Basic::Number* const);
   virtual bool setSlotStartupResetTime(const Basic::Time* const);
   virtual bool setSlotOwnshipName(const Basic::String* const);
   virtual bool setSlotFastForwardRate(const Basic::Number* const);
   virtual bool setSlotEnableUpdateTimers(const Basic::Number* const);

   // ---
   // Basic::Component functions
   // ---
   virtual void updateTC(const LCreal dt = 0.0f);
   virtual void updateData(const LCreal dt = 0.0f);
   virtual void reset();

protected:
   virtual void inputDevices(const LCreal dt);    // Handle device inputs
   virtual void outputDevices(const LCreal dt);   // Handle device output

   Basic::Thread* getTcThread();    // Pre-ref() pointer to the Time-critical thread
   void setTcThread(Basic::Thread* h);

   Basic::Thread* getNetThread();   // Pre-ref() pointer to the Network thread
   void setNetThread(Basic::Thread* h);

   Basic::Thread* getBgThread();    // Pre-ref() pointer to the Background thread
   void setBgThread(Basic::Thread* h);

   // Basic::Component protected functions
   virtual bool shutdownNotification();

private:
   void initData();

   virtual void createNetworkProcess(); // Creates a network thread
   virtual void createBackgroundProcess(); // Creates a B/G thread

   Simulation* sim;                    // Simulation model
   SPtr<Basic::PairStream> otw;        // List of  Out-The-Window visual system interfaces
   SPtr<Basic::PairStream> networks;   // List of networks
   SPtr<Basic::PairStream> ioHandlers; // List of I/O data handlers
   Player* ownship;                    // Ownship (primary) player
   const Basic::String* ownshipName;   // Name of our ownship player
   bool tmrUpdateEnbl;                 // Enable Basic::Timers::updateTimers() call from updateTC()
   DataRecorder* dataRecorder;         // Data Recorder

   LCreal tcRate;                      // Time-critical thread Rate (hz)
   LCreal tcPri;                       // Priority of the time-critical thread (0->lowest, 1->highest)
   SPtr<Basic::Thread> tcThread;       // The Time-critical thread
   unsigned int fastForwardRate;       // Time-critical thread fast forward rate

   LCreal netRate;                     // Network thread Rate (hz)
   LCreal netPri;                      // Priority of the Network thread (0->lowest, 1->highest)
   SPtr<Basic::Thread> netThread;      // The optional network thread

   LCreal bgRate;                      // Background thread Rate (hz)
   LCreal bgPri;                       // Priority of the Background thread (0->lowest, 1->highest)
   SPtr<Basic::Thread> bgThread;       // The optional background thread

   LCreal startupResetTimer;           // Startup RESET timer (sends a RESET_EVENT after timeout)
   const Basic::Time* startupResetTimer0;  // Init value of the startup RESET timer
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
