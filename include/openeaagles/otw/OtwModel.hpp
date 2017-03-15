
#ifndef __oe_otw_OtwModel_H__
#define __oe_otw_OtwModel_H__

#include "openeaagles/base/Object.hpp"
#include "openeaagles/base/safe_ptr.hpp"

namespace oe {
namespace base { class String; }
namespace models { class Player; }
namespace otw {
class Otm;

//------------------------------------------------------------------------------
// Class: OtwModel
// Description: General class that associates an active, in-range player with the
//              OTW system's model object or height-of-terrain request.
//              Manages player to OTW model relationships
//
// Notes:
//    1) The 'checked' flag and functions are used by the Otw class when
//       rechecking this model with the associated player.
//
//    2) The age counters are used by the OTW unique handlers (derived from
//       the Otw class).
// Factory name: OtwModel
//------------------------------------------------------------------------------
class OtwModel : public base::Object
{
    DECLARE_SUBCLASS(OtwModel, base::Object)

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

    models::Player* getPlayer()              { return player; }         // The player object associated with this model
    const models::Player* getPlayer() const  { return player; }         // The player object associated with this model (const version)

    const Otm* getTypeMapper() const         { return typeMapper; }     // OTW unique model type mapper

    unsigned short getPlayerID() const       { return playerID; }         // Player ID for the player associated with this model
    const base::String* getFederateName() const { return federateName; } // Player's federate name (if networked)

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
    virtual void initialize(models::Player* const p, const Otm** const otwModelTable = nullptr, const unsigned int numModels = 0);

    // Clear out this model (we're INACTIVE)
    virtual void clear();

protected:
    // Sets the player object, p, associated with this model
    virtual void setPlayer(models::Player* const p);

private:
    models::Player* player {};     // This player
    State state {INACTIVE};        // Model Active flag
    int ageCount {};               // Age counter (how many times have we've been overlooked)
    bool checked {};               // Model was checked
    const Otm* typeMapper {};      // OTW unique model type mapper

    int rcount {};                 // HOT request counter (how many times have we asked)
    bool hotActive {};             // HOT entry is active

    // Model IDs  -- Comparisons in this order --
    unsigned short playerID {};                        // Player ID
    base::safe_ptr<const base::String> federateName;   // Federate name
};

}
}

#endif
