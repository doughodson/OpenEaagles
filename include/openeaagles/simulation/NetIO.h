//------------------------------------------------------------------------------
// Class: NetIO
//------------------------------------------------------------------------------
#ifndef __oe_simulation_NetIO_H__
#define __oe_simulation_NetIO_H__

#include "openeaagles/basic/Component.h"
#include "openeaagles/simulation/Player.h"

namespace oe {
   namespace basic { class Angle; class Distance; class Identifier; class List;
                     class String; class Time;
   }

namespace simulation {
   class Nib;
   class Ntm;
   class Simulation;
   class Station;

//------------------------------------------------------------------------------
// Class:  NetIO
// Description: Abstract interface class for interoperability Network I/O.
//
//    Defines the basic framework for interoperability networks (e.g., DIS, HLA,
//    TENA, etc).  Derived classes will manage the network specific protocols.
//
// Factory name: NetIO
// Slots:
//    networkID            (basic::Number)      ! Network ID number: [ 1 .. MAX_NETWORD_ID ] (default: 1)
//    federationName       (basic::String)      ! Federation name (default: 0)
//    federateName         (basic::String)      ! Name of this federate (default: 0)
//
//    enableInput          (basic::Boolean)     ! Input Enable  accept players from this network (default: true)
//    enableOutput         (basic::Boolean)     ! Output enable: send players to this network (default: true)
//    enableRelay          (basic::Boolean)     ! Relay enable: send other network players to this network (default: true)
//    timeline             (basic::Identifier)  ! Source of the time line { UTC or EXEC } (default: UTC)
//
//    inputEntityTypes     (basic::PairSteam)   ! Incoming entity type mappers (default: 0)
//                                              !   (Ntm objects that map incoming entity types to oe player types)
//
//    outputEntityTypes    (basic::PairSteam)   ! Outgoing entity type mappers (default: 0)
//                                              !   (Ntm objects that map oe player types to outgoing entity types)
//
//    maxTimeDR            (basic::Time)        ! Max DR time (default: 5 seconds)
//    maxPositionError     (basic::Distance)    ! Max DR position error (default: 3 meters)
//    maxOrientationError  (basic::Angle)       ! Max DR angular error (default: 3 degrees)
//    maxAge               (basic::Time)        ! Max age (without update) of networked players (default: 12.5 seconds)
//
//    maxEntityRange       (basic::Distance)    ! Max entity range of networked players,
//                                              !  or zero for no max range (default: 0 -- no range filtering)
//
//
// NetIO class objects:
//
//    An oe application, which is typically controlled by a Station class
//    object, can have multiple (i.e., MAX_NETWORD_ID) NetIO objects.  Each NetIO
//    object handles a different interoperabiltiy network
//
//    Each NetIO object in the simulation has an unique, user defined network ID,
//    which is set using the 'networkID' slot.  The NetIO objects are also
//    identified by their federation and federate names, which are based on HLA.
//    (see "dis/NetIO.h" for functions that will map the DIS exercise number to
//    a federation name, and the DIS site and application numbers to the federate
//    name)
//
//
// Incoming entities:
//
//    Use the 'enableInput' slot to enable or disable the discovery of entities
//    on the interoperability network.
//
//    As incoming entities are discovered, their entity type codes are mapped
//    to oe player types using Network Type Mapping (Ntm) objects.  These
//    Ntm objects are listed in the "incoming entity type mapper" list, which
//    is set using the 'inputEntityTypes' slot.  An entity is ignored if its
//    entity type does not match any of the Ntm objects on the incoming list.
//    (Note: without the incoming Ntm list, all network entities are ignored)
//
//    When an entity's type matches a Ntm object, the matching Ntm's "template
//    player" (see Ntm.h) is cloned to create the surrogate "interoperability
//    player" or IPlayer, which is added to simulation's player list.
//
//    A network specific Network Interface Block (Nib) is created, using the
//    nibFactory() function, to manage the flow of data from the network entity
//    to the oe player.  The incoming Nib objects are managed using the
//    "input Nib" list.
//
//    The incoming surrogate players contain the ID of the NetIO object that
//    created it, and a pointer to the Nib object that is controlling its data
//    flow.
//
//    Incoming entities can be filtered by range using the 'maxEntityRange'
//    slot, which defaults to zero or no range filtering.  Currently, this
//    applies only to new entities; existing IPlayers are not filtered.
//
// Outgoing entities:
//
//    Use the 'enableOutput' slot to enable or disable the sending of our
//    application's local players to this interoperability network.
//
//    As local oe players are discovered by this class, their player
//    types are mapped to network entity type codes using the Ntm obects that
//    are listed in the "outgoing entity type mapper" list, which is set using
//    the "outputEntityTypes" slot.  A local player is not sent to the network
//    if its type does not match any of the Ntm objects on the outgoing list.
//    (Note: without the outgoing Ntm list, no local players will be written
//    to the network)
//
//    A network specific Nib is created, using the nibFactory() function, to
//    manage the flow of data from the oe player to the network entity.
//    The outgoing Nib objects are managed using the "output Nib" list.
//
//    The oe player objects do not contain any state data related to the
//    interoperability networks that their being sent to.  As a result, an
//    oe player can be sent to more than one interoperability network,
//    and the 'enableRelay' slot can be used to enable the relaying of OpenEaagles
//    players that were discovered from other interoperability networks.
//
//
// Input/Output frames:
//
//    The functions inputFrame() and outputFrame() need to be called by our
//    container object (e.g., Station class) to process the incoming and out-
//    going entities, respectively.  For DIS, these can be called by different
//    threads, and for HLA they need to be called from the same thread.
//
//
// Time line:
//
//    Data sent to the network can be marked with universal UTC time, or
//    with relative executive time.
//
//
// Event IDs
//
//    Some network specific outgoing event messages require an unique ID
//    to identify unique events.  The following functions are used to
//    generate event unique IDs.
//
//       unsigned short getNewIffEventID()         // New IFF message event
//       unsigned short getNewEmissionEventID()    // New emission event
//
//
// Note: public and protected sections for Nib and Ntm support are located
//       after the main public and protected sections.
//------------------------------------------------------------------------------
class NetIO : public basic::Component
{
   DECLARE_SUBCLASS(NetIO, basic::Component)

public:
    // Source of the time line
    enum TSource {
       EXEC,         // Using our executive time
       UTC           // Using Coordinated Universal Time (UTC) from the Operating System
    };

    // Max network ID
    static const unsigned int MAX_NETWORD_ID = 2;

    //  Max number of new, outgoing players published per frame
    static const unsigned int MAX_NEW_OUTGOING = OE_CONFIG_MAX_NETIO_NEW_OUTGOING;

public:
   NetIO();

   // Updates the 'input' side of the network
   virtual void inputFrame(const LCreal dt);

   // Updates the 'output' side of the network
   virtual void outputFrame(const LCreal dt);

   // Network ID number
   unsigned short getNetworkID() const { return netID; }

   // Federate name as String
   virtual const basic::String* getFederateName() const;

   // Federation name as String
   virtual const basic::String* getFederationName() const;

   // Current time (seconds: UTC or EXEC)
   double getCurrentTime();

   // Timeline (UTC or EXEC)
   TSource getTimeline() const { return timeline; }

   // True if inputs are enabled
   bool isInputEnabled() const { return inputFlg; }

   // True if outputs are enabled
   bool isOutputEnabled() const { return outputFlg; }

   // True if relaying is enabled
   bool isRelayEnabled() const { return (relayFlg && isInputEnabled() && isOutputEnabled()); }

   // Entity filter: Returns max entity ranged (meters)
   virtual LCreal getMaxEntityRange(const Nib* const nib = 0) const;

   // Entity filter: Returns max entity ranged squared (meters^2)
   virtual LCreal getMaxEntityRangeSquared(const Nib* const nib = 0) const;

   // Dead-Reckoning: Returns max DR time before next 'heart beat' (seconds)
   virtual LCreal getMaxTimeDR(const Nib* const nib = 0) const;

   // Dead-Reckoning: Returns max DR position error (meters)
   virtual LCreal getMaxPositionErr(const Nib* const nib = 0) const;

   // Dead-Reckoning: Returns max DR orientation error (radians)
   virtual LCreal getMaxOrientationErr(const Nib* const nib = 0) const;

   // Dead-Reckoning: Returns max age before a networked player is removed (seconds)
   virtual LCreal getMaxAge(const Nib* const nib = 0) const;

   // Network initialization
   bool isNetworkInitialized() const { return netInit; }
   bool didInitializationFail() const { return netInitFail; }
   virtual bool networkInitialization();

   // Other components
   Station* getStation() { return station; }
   const Station* getStation() const { return station; }

   Simulation* getSimulation() { return simulation; }
   const Simulation* getSimulation() const { return simulation; }

   // Event IDs
   unsigned short getNewIffEventID() { return ++iffEventID; }
   unsigned short getNewEmissionEventID() { return ++emEventID; }

   // IPlayer factory: creates a networked player based on NIB data
   virtual Player* createIPlayer(Nib* const nib);

   void reset() override;

protected:
   // Derived class callbacks
   virtual bool initNetwork()=0;                            // Initialize the network (pure functions!)
   virtual void netInputHander()=0;                         // Network input handler (pure functions!)
   virtual void processInputList()=0;                       // Update players/systems from the Input-list (pure functions!)

   // Create output packets from Output-List
   virtual void processOutputList();

   // Set functions
   virtual bool setNetworkID(const unsigned short v);       // Sets the Network's ID
   virtual bool setTimeline(const TSource ts);              // Sets the timeline (UTC or EXEC)
   virtual bool setMaxTimeDR(const LCreal v);               // Sets the max dead-rec time; forces next update (sec)
   virtual bool setMaxPositionErr(const LCreal v);          // Sets the max positional error (meters)
   virtual bool setMaxOrientationErr(const LCreal v);       // Sets the max orientation error (rad)
   virtual bool setMaxAge(const LCreal v);                  // Sets the max age; for removal (sec)
   virtual bool setMaxEntityRange(const LCreal v);          // Sets the max entity range (meters)
   virtual bool setFederateName(const basic::String* const msg);   // Sets our federate name
   virtual bool setFederationName(const basic::String* const msg); // Sets our federation name

   // Set slot routines
   virtual bool setSlotNetworkID(const basic::Number* const p);                // Sets the network ID
   virtual bool setSlotFederateName(const basic::String* const msg);           // Sets our federate name
   virtual bool setSlotFederationName(const basic::String* const msg);         // Sets our federation name
   virtual bool setSlotEnableInput(const basic::Number* const p);              // Sets input enabled flag
   virtual bool setSlotEnableOutput(const basic::Number* const p);             // Sets output enabled flag
   virtual bool setSlotEnableRelay(const basic::Number* const p);              // Sets relay enabled flag
   virtual bool setSlotTimeline(const basic::Identifier* const p);             // Sets the source of the time ( UTC or EXEC )
   virtual bool setSlotInputEntityTypes(basic::PairStream* const msg);         // Sets the table of input entity to player mapper objects
   virtual bool setSlotOutputEntityTypes(basic::PairStream* const msg);        // Sets the table of output entity to player mapper objects
   virtual bool setSlotMaxTimeDR(const basic::Time* const msg);                // Sets the mac DR time(s)
   virtual bool setSlotMaxPositionErr(const basic::Distance* const msg);       // Sets the max positional error(s)
   virtual bool setSlotMaxOrientationErr(const basic::Angle* const msg);       // Sets the max orientation error(s)
   virtual bool setSlotMaxAge(const basic::Time* const msg);                   // Sets the max age(s)
   virtual bool setSlotMaxEntityRange(const basic::Distance* const msg);       // Sets the max entity range(s)

   bool shutdownNotification() override;

//------------------------------------------------------------------------------
// Network Interface Block (NIB) maintenance sections
//------------------------------------------------------------------------------
public:
    // NIB I/O type
    enum IoType {
       INPUT_NIB,    // NIB is used to map incoming entities to 'networked' players (IPlayer)
       OUTPUT_NIB    // NIB is used to map outgoing players to networked entities.
    };

public:
   // NIB support
   virtual Nib* findNib(const unsigned short playerID, const basic::String* const federateName, const IoType ioType);
   virtual Nib* findNib(const Player* const player, const IoType ioType);
   virtual bool addNibToList(Nib* const nib, const IoType ioType);
   virtual void removeNibFromList(Nib* const nib, const IoType ioType);

   // More NIB support
   virtual Nib* createNewInputNib();
   virtual Nib* createNewOutputNib(Player* const player);
   virtual void destroyInputNib(Nib* const nib);
   virtual void destroyOutputNib(Nib* const nib);
   virtual bool addNib2InputList(Nib* const nib);

protected:
   // Maximum number of active objects
   static const int MAX_OBJECTS = OE_CONFIG_MAX_NETIO_ENTITIES;

   // Create NIB unique to protocol (pure functions!)
   virtual Nib* nibFactory(const NetIO::IoType ioType)=0;

   // Create a new Network Interface Block (NIB) for 'player' and insert it
   // in the output list.  Returns a pointer to the new NIB or 0.
   Nib* insertNewOutputNib(Player* const player);

   // Number of NIBs on the input list
   unsigned int getInputListSize() const {
      return nInNibs;
   }

   // Returns the idx'th NIB from the input list
   Nib* getInputNib(const unsigned int idx) {
      return (idx < nInNibs) ? inputList[idx] : 0;
   }

   // Returns the idx'th NIB from the input list (const version)
   const Nib* getInputNib(const unsigned int idx) const  {
      return (idx < nInNibs) ? inputList[idx] : 0;
   }

   // Returns the input list
   Nib** getInputList() {
      return inputList;
   }

   // Number of NIBs on the output list
   unsigned int getOutputListSize() const {
      return nOutNibs;
   }

   // Returns the input list
   Nib** getOutputList() {
      return outputList;
   }

   // Returns the idx'th NIB from the output list
   Nib* getOutputNib(const unsigned int idx) {
      return (idx < nOutNibs) ? outputList[idx] : 0;
   }

   // Returns the idx'th NIB from the output list (const version)
   const Nib* getOutputNib(const unsigned int idx) const {
      return (idx < nOutNibs) ? outputList[idx] : 0;
   }


//------------------------------------------------------------------------------
// Network Type Mapper (NTM) maintenance sections
//------------------------------------------------------------------------------
public:

   // Finds the network type mapper by NIB type codes
   virtual const Ntm* findNetworkTypeMapper(const Nib* const nib) const;

   // Finds the network type mapper by Player
   virtual const Ntm* findNetworkTypeMapper(const Player* const p) const;

   // NTM input node
   class NtmInputNode : public basic::Object {
      DECLARE_SUBCLASS(NtmInputNode,basic::Object)
   public:
      NtmInputNode();
      virtual const Ntm* findNetworkTypeMapper(const Nib* const p) const =0;
      virtual bool add2OurLists(Ntm* const ntm) =0;
      virtual void print(std::ostream& sout, const int icnt=0) const =0;
   };

   // NTM output node
   class NtmOutputNode : public basic::Object {
      DECLARE_SUBCLASS(NtmOutputNode,basic::Object)
   public:
      NtmOutputNode();
      virtual const Ntm* findNetworkTypeMapper(const Player* const p) const =0;
      virtual bool add2OurLists(Ntm* const ntm) =0;
      virtual void print(std::ostream& sout, const int icnt=0) const =0;
   };

protected:
   virtual bool addOutputEntityType(Ntm* const item);          // Adds an item to the output entity type table
   virtual bool addInputEntityType(Ntm* const item);           // Adds an item to the input entity type table
   virtual bool clearOutputEntityTypes();                      // Clears the output entity type table
   virtual bool clearInputEntityTypes();                       // Clears the input entity type table

   // ---
   // Quick look trees
   // ---
   const NtmOutputNode* getRootNtmOutputNode() const;          // Return the quick look root node for outgoing entity types
   virtual NtmOutputNode* rootNtmOutputNodeFactory() const;    // Outgoing quick look root node factory

   const NtmInputNode* getRootNtmInputNode() const;            // Return the quick look root node for incoming entity types
   virtual NtmInputNode* rootNtmInputNodeFactory() const =0;   // Incoming quick look root node factory

   // ---
   // Raw lists
   // ---
   const Ntm* getOutputEntityTypes(const unsigned int) const;  // Return a outgoing entity type by index
   const Ntm* getInputEntityType(const unsigned int) const;    // Return a incoming entity type by index
   unsigned int getNumOutputEntityTypes() const;               // Number of input types
   unsigned int getNumInputEntityTypes() const;                // Number of output types

   virtual void testOutputEntityTypes(const unsigned int n);   // Test rig for outgoing quick lookup
   virtual void testInputEntityTypes(const unsigned int n);    // Test rig for incoming quick lookup



//------------------------------------------------------------------------------
// Private data
//------------------------------------------------------------------------------
private:
   void updateOutputList();                             // Update the Output-List from the simulation player list (Background thread)
   void cleanupInputList();                             // Clean-up the Input-List (remove out of date items)

   // Network Model IDs
   unsigned short netID;                                // Networkd ID
   basic::safe_ptr<const basic::String> federationName; // Federation name
   basic::safe_ptr<const basic::String> federateName;   // Federate name

   basic::safe_ptr<Station>    station;                 // Our station class
   basic::safe_ptr<Simulation> simulation;              // Our simulation class
   TSource          timeline;                           // Source of our timeline
   unsigned short   iffEventID;                         // IFF event ID (as needed)
   unsigned short   emEventID;                          // Emission event ID (as needed)

   // Network Model mode flags
   bool              inputFlg;         // Network input enabled
   bool              outputFlg;        // Network output enabled
   bool              relayFlg;         // Network relay enabled
   bool              netInit;          // Network has been initialized
   bool              netInitFail;      // Initialization attempt failed

   // Distance filter by entity kind/domain
   LCreal            maxEntityRange;   // Max range from ownship           (meters)
   LCreal            maxEntityRange2;  // Max range squared from ownship   (meters^2)

   // Dead Reckoning (DR) parameters by entity kind/domain
   LCreal            maxTimeDR;          // Maximum DR time                  (seconds)
   LCreal            maxPositionErr;     // Maximum position error           (meters)
   LCreal            maxOrientationErr;  // Maximum orientation error        (radians)
   LCreal            maxAge;             // Maximum age of networked players (seconds)

private: // Nib related private
   // input tables
   Nib*  inputList[MAX_OBJECTS];    // Table of input objects in name order
   unsigned int   nInNibs;          // Number of input objects in both tables

   // output tables
   Nib*  outputList[MAX_OBJECTS];   // Table of output objects in name order
   unsigned int   nOutNibs;         // Number of output objects in both tables

   // NIB quick lookup key
   struct NibKey {
      NibKey(const unsigned short playerId, const basic::String* const federateName) {
         fName = federateName;
         id = playerId;
      }
      // NIB IDs  -- Comparisons in this order --
      unsigned short id;                           // Player id
      basic::safe_ptr<const basic::String> fName;  // Federate name
   };

   // Search callbacks: object name compare function --
   //   True types are (const NibKey* key, const Nib** nib)
   static int compareKey2Nib(const void* key, const void* nib);

private:  // Ntm related private
   static const unsigned int MAX_ENTITY_TYPES = OE_CONFIG_MAX_NETIO_ENTITY_TYPES;

   NtmInputNode* inputNtmTree;   // Input NTM quick lookup tree
   NtmOutputNode* outputNtmTree; // Output NTM quick lookuptree

   // Input entity type table
   const Ntm*     inputEntityTypes[MAX_ENTITY_TYPES]; // Table of pointers to input entity type mappers; Ntm objects
   unsigned int   nInputEntityTypes;                  // Number of input entity mappers (Ntm objects) in the table, 'inputEntityTypes'

   // Output entity type table
   const Ntm*     outputEntityTypes[MAX_ENTITY_TYPES]; // Table of pointers to output entity type mappers; Ntm objects
   unsigned int   nOutputEntityTypes;                  // Number of output entity mappers (Ntm objects) in the table, 'outputEntityTypes'
};


}
}

#endif

