
#ifndef __oe_simulation_AbstractPlayer_H__
#define __oe_simulation_AbstractPlayer_H__

#include "openeaagles/base/Component.hpp"

#include "openeaagles/base/Identifier.hpp"

namespace oe {
namespace simulation {
class AbstractNib;

//------------------------------------------------------------------------------
// Class: AbstractPlayer
//
// Description: Abstract interface for all players
//
//
//------------------------------------------------------------------------------
// Factory name: AbstractPlayer
// Slots --
//
//    ! ---
//    ! Player's id and mode parameters
//    ! ---
//    id             <base::Integer>     ! Player id  [ 1 .. 65535 ] (default: 0)
//    mode           <base::String>      ! Initial player mode ( INACTIVE, ACTIVE, DEAD ) (default: ACTIVE)
//------------------------------------------------------------------------------
class AbstractPlayer : public base::Component
{
   DECLARE_SUBCLASS(AbstractPlayer, base::Component)

public:
   AbstractPlayer();

   // Player mode
   enum Mode {
      INACTIVE,         // Player is not being updated and is not being sent to the networks
      ACTIVE,           // Player is being updated and is being sent to the networks
      KILLED,           // Player was killed   (One of the dead conditions)
      CRASHED,          // Player crashed      (One of the dead conditions)
      DETONATED,        // Weapon player has detonated (One of the dead conditions) (Original & flyout weapons)
      PRE_RELEASE,      // Weapon player is created but not released (Flyout weapons only)
      LAUNCHED,         // Weapon player has been launched (Original weapons only)
      DELETE_REQUEST    // Request player removal from the active player list
   };

   // ---
   // Player ID
   // ---
   virtual void setID(const unsigned short newId);                // Sets the player's ID
   bool isID(const unsigned short tst) const;                     // True if player's ID matches
   unsigned short getID() const;                                  // The player's ID

   // ---
   // Player name
   // ---
   virtual void setName(const base::Identifier& newName);         // Sets the player's name (base::String version)
   virtual void setName(const char* const newName);               // Sets the player's name (char* version)
   bool isName(const base::Identifier* const) const;              // True if the player's name matches
   bool isName(const char* const) const;                          // True if the player's name matches
   const base::Identifier* getName() const;                       // The player's name

   // ---
   // Player mode
   // ---
   virtual void setMode(const Mode newMode);                      // Sets the player's current mode
   virtual void setInitMode(const Mode newMode);                  // Sets the player's initial mode (after reset)
   Mode getMode() const;                                          // Current mode ( INACTIVE, ACTIVE, DETONATED ... )
   bool isActive() const;                                         // True if player's mode is active
   bool isKilled() const;                                         // True if player's mode is killed
   bool isCrashed() const;                                        // True if player's mode is crashed
   bool isDetonated() const;                                      // True if player has detonated (weapons only)
   bool isInactive() const;                                       // True if player's mode is inactive
   bool isMode(const Mode tst) const;                             // True if player is currently this mode
   bool isNotMode(const Mode tst) const;                          // True if player is not currently this mode
   bool isDead() const;                                           // True if player's mode is dead (Killed, Crashed, Detonated, etc....)

   // ---
   // Interoperability network data
   // ---
   bool isNetworkedPlayer() const;                                    // True if this is a networked player (IPlayer)
   bool isLocalPlayer() const;                                        // True if this is a local player

   int getNetworkID() const;                                          // ID of a networked player's controlling network model
   AbstractNib* getNib();                                             // Networked player's Nib object
   const AbstractNib* getNib() const;                                 // Networked player's Nib object  (const version)

   bool isNetOutputEnabled() const;                                   // Is player output to the network enabled?
   AbstractNib* getLocalNib(const unsigned int netId);                // Player's outgoing NIB(s)
   const AbstractNib* getLocalNib(const unsigned int netId) const;    // Player's outgoing NIB(s)  (const version)

   // helper methods
   virtual bool setNib(AbstractNib* const p);                                 // Sets the networked player's Nib object
   virtual bool setEnableNetOutput(const bool f);                             // Sets the network output enabled flag
   virtual bool setOutgoingNib(AbstractNib* const p, const unsigned int id);  // Sets the outgoing NIB for network 'id'

   virtual void reset() override;

protected:

   virtual bool shutdownNotification() override;

   virtual bool setSlotID(const base::Number* const msg);
   virtual bool setSlotInitMode(base::String* const msg);

   Mode mode {ACTIVE};           // Player mode (see above)
   AbstractNib* nib {};          // Network Interface Block (ref()'d)

private:
   void initData();

   // player identity
   unsigned short id {};          // ID
   base::Identifier pname;        // Name

   Mode initMode {ACTIVE};        // Initial mode

   // incoming network support
   int netID {};                  // Network id

   // outgoing network support data
   AbstractNib** nibList {};      // Pointer to a list of outgoing NIBs
   bool enableNetOutput {true};   // Allow output to the network
};

#include "openeaagles/simulation/AbstractPlayer.inl"

}
}

#endif
