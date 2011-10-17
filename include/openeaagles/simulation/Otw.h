//------------------------------------------------------------------------------
// Classes:	
//    Otw      -- Abstract Out-The-Window (OTW) visual system interface class
//    OtwModel -- Model class; manages player to OTW model relationships
//    Otm      -- Type Mapper class; maps player types to OTW model types
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Otw_H__
#define __Eaagles_Simulation_Otw_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
   namespace Basic {
      class Distance;
      class Identifier;
      class Number;
      class PairStream;
      class String;
   }
namespace Simulation {

class Player;
class OtwModel;
class Otm;

//------------------------------------------------------------------------------
// Class:	Otw
// Base class:	Basic::Object -> Basic::Component -> Otw
// Description: Abstract Out-The-Window (OTW) visual system interface class
//
// Notes:
//    1) Derived versions of this class are used to interface with various
//       visual systems and HUDs (see the Otw project).
//
//    2) One or more Otw objects can be attached to and managed by a Station object.
//
//    3) To allow different visual database to be used with the same scenario,
//       the visual database reference point, which is used by this class,
//       is a different reference point then the Simulation class.
//       Player's (see Player.h) position vectors [ x y z ] are north(x),
//       east(y) and down(z) from the reference point.
//
//
// Form name: Otw
// Slots:
//    maxRange       <Distance>     ! Max range of visual system (default: 20000.0f)
//    maxRange       <Number>       ! Max range of visual system (meters)
//
//    maxModels      <Number>       ! Max number of active, in-range player/models (default: 0)
//
//    maxElevations  <Number>       ! Max number of player terrain elevation requests (default: 0)
//
//    latitude       <Number>       ! Visual database reference latitude (deg) (default: 0.0)
//
//    longitude      <Number>       ! Visual database reference longitude (deg) (default: 0.0)
//
//    otwModelTypes  <PairStream>   ! OTW system's model type IDs (list of Otm objects) (default: 0)
//
//------------------------------------------------------------------------------
class Otw : public Basic::Component
{
    DECLARE_SUBCLASS(Otw,Basic::Component)

public:
    // Model table types
    enum TableType {
       MODEL_TABLE,     // Model table -- maps players to visual system models
       HOT_TABLE        // Height-Of-Terrain request table
    };  

public:
    Otw();

    const Player* getOwnship() const      { return ownship; }        // Our ownship -- the player that we're following
    unsigned int getMaxModels() const     { return maxModels; }      // Max number of active, in-range player/models
    unsigned int getMaxElevations() const { return maxElevations; }  // Max number of terrain elevation requests
    LCreal getMaxRange() const            { return maxRange; }       // Max range of active player/models
    double getRefLatitude() const         { return refLat; }         // Visual database reference latitude  (degs)
    double getRefLongitude() const        { return refLon; }         // Visual database reference longitude (degs)
    virtual bool isResetInProgress() const;                          // True if visual system is resetting

    virtual bool setRefLatitude(const double v);                     // Sets the visual database reference latitude  (degs)
    virtual bool setRefLongitude(const double v);                    // Sets the visual database reference longitude (degs)
    virtual void setPlayerList(Basic::PairStream* const newPlayerList); // Sets the player list that we're to use to generate player/models

    bool setMaxRange(const LCreal r);                                // Sets the max range (meters)
    bool setMaxModels(const unsigned int n);                         // Sets the max number of active, in-range player/models
    bool setMaxElevations(const unsigned int n);                     // Sets the max number of player terrain elevation requests

    // Sets our ownship pointer; public version, which is usually called by the Station class.  Derived classes
    // can override this function and control the switching of the ownship using setOwnship0()
    virtual void setOwnship(Player* const newOwnship);

    // Slot functions
    virtual bool setSlotMaxRange(const Basic::Distance* const msg);       // Sets the max range (Distance) slot
    virtual bool setSlotMaxRange(const Basic::Number* const msg);         // Sets the max range (meters) slot
    virtual bool setSlotMaxModels(const Basic::Number* const msg);        // Sets the max number of active, in-range player/models slot
    virtual bool setSlotMaxElevations(const Basic::Number* const msg);    // Sets the max number of player terrain elevation requests slot
    virtual bool setSlotRefLatitude(const Basic::Number* const msg);      // Sets the visual database reference latitude  (degs) slot
    virtual bool setSlotRefLongitude(const Basic::Number* const msg);     // Sets the visual database reference longitude (degs) slot
    virtual bool setSlotOtwModelTypes(const Basic::PairStream* const msg); // Sets the list of OTW model type IDs (Otm objects)

    // Basic::Component interface
    virtual void updateTC(const LCreal dt = 0.0f);
    virtual void reset();

protected:
    virtual void setOwnship0(Player* const newOwnship);     // Sets our ownship player

    // Computers the range (meters) from our ownship to this player.
    virtual LCreal computeRangeToPlayer(const Player* const ip) const;

    // Find a player's model object in table 'type' by the player IDs
    virtual OtwModel* findModel(const unsigned short playerID, const Basic::String* const federateName, const TableType type);

    // Find a player's model object in table 'type' using a pointer to the player
    virtual OtwModel* findModel(const Player* const player, const TableType type);

    // Add a player's model object to table 'type'
    virtual bool addModelToList(OtwModel* const model, const TableType type);

    // Remove a player's model object from table 'type'
    virtual void removeModelFromList(OtwModel* const model, const TableType type);

    // Remove model object at index, 'idx', from table 'type'
    virtual void removeModelFromList(const int idx, const TableType type);

    // Derived class' access to the tables
    unsigned int getModelTableSize() const      { return nModels; }
    unsigned int getElevationTableSize() const  { return nHots; }
    OtwModel** getModelTable()                  { return modelTbl; }
    OtwModel** getElevationTable()              { return hotTbl; }

    // Create OtwModel objects to manage player/models
    // OTW/HUD system unique versions must be implemented by derived classes
    virtual OtwModel* modelFactory()=0;

    // Create OtwModel objects to manage player terrain elevation requests
    // OTW/HUD system unique versions must be implemented by derived classes
    virtual OtwModel* hotFactory()=0;

    // Manages the sending of ownship and player/model state data
    // OTW/HUD system unique versions must be implemented by derived classes
    virtual void sendOwnshipAndModels() =0;

    // Manages the sending of player terrain elevation requests
    // OTW/HUD system unique versions must be implemented by derived classes
    virtual void sendElevationRequests()=0;

    // Handles received player terrain elevation data
    // OTW/HUD system unique versions must be implemented by derived classes
    virtual void recvElevations()=0;

    // Send frame sync (if any)
    // OTW/HUD system unique versions must be implemented by derived classes
    virtual void frameSync()=0;


private:
   static const int MAX_MODELS = 400;             // Max model table size
   static const int MAX_MODELS_TYPES = 400;       // Max OTW model type table size

   void processesModels();                        // Process ownship & player models
   void processesElevations();                    // Process terrain elevation requests
   void resetTables();                            // Resets the tables
   void clearOtwModelTypes();                     // Clear the OTW model types table
   void mapPlayerList2ModelTable();               // Map the player list to the model table
   void mapPlayers2ElevTable();                   // Map player list to terrain elevation table
   OtwModel* newModelEntry(Player* const ip);     // Create a new model entry for this player & return the table index
   OtwModel* newElevEntry(Player* const ip);      // Create a new elevation entry for this player & return the table index

   // Parameters
   LCreal         maxRange;                        // Max range of visual system  (meters)
   unsigned int   maxModels;                       // Max number of models (must be <= MAX_MODELS)
   unsigned int   maxElevations;                   // Max number of terrain elevation requests

   // Ref position
   double         refLat;                          // Visual database reference latitude (deg)
   double         refLon;                          // Visual database reference longitude (deg)

   // Simulation inputs
   Player*        ownship;                         // Current ownship
   Basic::PairStream*  playerList;               // Current player list
   bool           rstFlg;                          // Reset in progress
   bool           rstReq;                          // Reset request flag

   // Model table
   OtwModel*      modelTbl[MAX_MODELS];            // The table of models
   unsigned int   nModels;                         // Number of models

   // Height-Of-Terrain request table
   OtwModel*      hotTbl[MAX_MODELS];              // Height-Of-Terrain request table
   unsigned int   nHots;                           // Number of HOTs requests
   
   // OtwModel quick lookup key
    struct OtwModelKey {
      OtwModelKey(const unsigned short pid, const Basic::String* const federateName);
      // OtwModel IDs  -- Comparisons in this order --
      unsigned short  playerID;   // Player ID
      SPtr<const Basic::String> fName;  // Federate name
   };

   // OTW model type table
   const Otm*     otwModelTypes[MAX_MODELS_TYPES]; // Table of pointers to OTW type mappers; Otm objects
   unsigned int   nOtwModelTypes;                  // Number of type mappers (Otm objects) in the table, 'otwModelTable'

   // bsearch callbacks: object name compare function --
   //   True types are (const OtwModelKey* key, const OtwModel** model)
   static int compareKey2Model(const void* key, const void* nib);
};


//------------------------------------------------------------------------------
// Class:	OtwModel
// Base class:	Basic::Object -> OtwModel
// Description: General class that associates an active, in-range player with the
//              OTW system's model object or height-of-terrain request.
//
// Notes:
//    1) The 'checked' flag and functions are used by the Otw class when
//       rechecking this model with the associated player.
//
//    2) The age counters are used by the OTW unique handlers (derived from
//       the Otw class).
// Form name: OtwModel
//------------------------------------------------------------------------------
class OtwModel : public Basic::Object
{
    DECLARE_SUBCLASS(OtwModel,Basic::Object)

public:
    enum State {
       INACTIVE,        // Unused model object
       ACTIVE,          // Player is alive and in-range
       DEAD,            // player is dead or destoryed
       OUT_OF_RANGE,    // Player is alive but out of range
       CLEARED          // Request removel from table
    };

public:
    OtwModel();

    bool isState(const State tst) const      { return (tst == state); } // True if the model is this state ( INACTIVE, ACTIVE, etc. }
    State getState() const                   { return state; }          // Model's state  ( INACTIVE, ACTIVE, etc. }
    void setState(const State newState)      { state = newState; }      // Sets the model's state  ( INACTIVE, ACTIVE, etc. }
    
    Player* getPlayer()                      { return player; }         // The player object associated with this model
    const Player* getPlayer() const          { return player; }         // The player object associated with this model (const version)

    const Otm* getTypeMapper() const         { return typeMapper; }     // OTW unique model type mapper

    unsigned short getPlayerID() const       { return playerID; }       // Player ID for the player associated with this model
    const Basic::String* getFederateName() const { return federateName; } // Player's federate name (if networked)
    
    int getAgeCount() const                  { return ageCount; }       // Age counter value (number of OTW frames since last OTW update)
    int incAgeCount()                        { return ++ageCount; }     // Increments the age counter
    void setAgeCount(const int v)            { ageCount = v; }          // Sets the age counter

    bool isHotActive() const                 { return hotActive; }      // True if a Height-Of-Terrain request is active for this model
    void setHotActive(const bool b)          { hotActive = b; }         // Sets the  Height-Of-Terrain request active flag for this model
    
    int getReqCount() const                  { return rcount; }         // HOT request age counter value (number of OTW frames since last HOT request)
    int incReqCount()                        { return ++rcount; }       // Increments the HOT request age counter
    void setReqCount(const int v)            { rcount = v; }            // Sets the HOT request age counter

    bool isChecked() const                   { return checked; }        // True if this model has been checked with the associated player object
    bool isNotChecked() const                { return !checked; }       // True if this model has not been checked with the associated player object
    void setCheckedFlag(const bool flg)      { checked = flg; }         // Sets the 'checked' flag

    // Initializes this model for player, 'p' (we're ACTIVE), and
    // looks up the OTW model type ID in the OTW model table, 'otwModelTable'.
    // If the size of the OTW model table is zero(0), then the model type ID is not set.
    virtual void initialize(Player* const p, const Otm** const otwModelTable = 0, const unsigned int numModels = 0);

    // Clear out this model (we're INACTIVE)
    virtual void clear();

protected:
    // Sets the player object, p, associated with this model
    virtual void setPlayer(Player* const p);

private:
    Player*       player;        // This player
    State         state;         // Model Active flag
    int           ageCount;      // Age counter (how many times have we've been overlooked)
    bool          checked;       // Model was checked
    const Otm*    typeMapper;    // OTW unique model type mapper

    int           rcount;        // HOT request counter (how many times have we asked)
    bool          hotActive;     // HOT entry is active
   
    // Model IDs  -- Comparisons in this order --
    unsigned short playerID;     // Player ID
    SPtr<const Basic::String> federateName; // Federate name
};

//------------------------------------------------------------------------------
// Class:	Otm
// Base class:	Basic::Object -> Otm
//
// Description: OTW-Type-Mapper (OTM) -- 
//              Maps player's form name and type string to a OTW entity type ID.
//              
//
// Form name: Otm
// Slots:
//     formName      <Identifier>  ! Reference form name (default: 0)
//     typeName      <String>      ! Reference type name (default: 0)
//     modelTypeId   <Number>      ! Entity type ID number (unsigned int) (default: 0)
//
//
// Examples:
//    ( Otm  Aircraft  "B-1B"          203 )
//    ( Otm  Aircraft  "F-14"          309 )
//    ( Otm  Aircraft     modelTypeId: 302 )       // Default aircraft type
//
//
// Notes:
//   1) If typeName isn't given, only the formName will be used.
//   2) Derived class (OTW unique) can use 'modelTypeId' or add their own IDs
//
//
// Notes on comparing player types -- isMatchingPlayerType()
//
//    1) Derived classes and more specific type should be checked first
//       (see #2 & #5 below -- we get matches with the base classes and
//        the more general type names)
//
//    2) Form names will also match base class form names
//
//    3) Form names are compared first and then the type names are compared
//
//    4) It's considered a match if the form names match and no type name as given.
//
//    5) For type names, we compare at most the length of our type name ...
//          (e.g., Test player's "F-16C" would match our "F-16C"
//          (e.g., Test player's "F-16C" would match our general "F-16" type)
//          (e.g., Test player's general "F-16" type would not match our "F-16C")
//
//------------------------------------------------------------------------------
class Otm : public Basic::Object
{
    DECLARE_SUBCLASS(Otm,Basic::Object)

public:
    Otm();

    unsigned int getTypeId() const           { return typeId; }         // OTW entity type ID number
    virtual bool setTypeId(const unsigned int newType);                 // Sets the OTW entity type number
    virtual bool setSlotTypeId(const Basic::Number* const msg);              // Sets the OTW entity type number

    const Basic::Identifier* getRefFormName() const    { return refFormName; }    // Reference form name
    virtual bool setSlotRefFormName(const Basic::Identifier* const msg);          // Sets the Reference form name

    const Basic::String* getRefTypeName() const   { return refTypeName; }    // Reference type name
    virtual bool setSlotRefTypeName(const Basic::String* const msg);         // Sets the Reference type name

    // True if player's form & type names match our reference form and type names.
    virtual bool isMatchingPlayerType(const Player* const p) const;

private:
    SPtr<const Basic::Identifier>  refFormName;     // Reference form name
    SPtr<const Basic::String> refTypeName;     // Reference type name (e.g., "F-16C", "T-71"
    unsigned int typeId;                  // OTW entity type ID number
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
