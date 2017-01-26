//------------------------------------------------------------------------------
// Class: AbstractPlayer
// Description: inline functions
//------------------------------------------------------------------------------
#ifndef __oe_models_AbstractPlayer_Inline__
#define __oe_models_AbstractPlayer_Inline__

// True if player's ID matches
inline bool AbstractPlayer::isID(const unsigned short tst) const
{
   return (tst == id);
}

// The player's ID
inline unsigned short AbstractPlayer::getID() const
{
   return id;
}

//-----------------------------------------------------------------------------

// True if the player's name matches
inline bool AbstractPlayer::isName(const base::Identifier* const tst) const
{
   return (*tst == pname);
}

// True if the player's name matches
inline bool AbstractPlayer::isName(const char* const tst) const
{
   return (tst == pname);
}

// The player's name
inline const base::Identifier* AbstractPlayer::getName() const
{
   return &pname;
}

//-----------------------------------------------------------------------------

// Current mode ( INACTIVE, ACTIVE, DETONATED ... )
inline AbstractPlayer::Mode AbstractPlayer::getMode() const
{
   return mode;
}

// True if player's mode is active
inline bool AbstractPlayer::isActive() const
{
   return mode == ACTIVE;
}

// True if player's mode is killed
inline bool AbstractPlayer::isKilled() const
{
   return (mode == KILLED);
}

// True if player's mode is crashed
inline bool AbstractPlayer::isCrashed() const
{
   return (mode == CRASHED);
}

// True if player has detonated (weapons only)
inline bool AbstractPlayer::isDetonated() const
{
   return (mode == DETONATED);
}

// True if player's mode is inactive
inline bool AbstractPlayer::isInactive() const
{
   return mode == INACTIVE;
}

// True if player is currently this mode
inline bool AbstractPlayer::isMode(const Mode tst) const
{
   return mode == tst;
}

// True if player is not currently this mode
inline bool AbstractPlayer::isNotMode(const Mode tst) const
{
   return mode != tst;
}

// True if player's mode is dead
inline bool AbstractPlayer::isDead() const
{
   return isKilled() || isCrashed() || isDetonated();
}

//-----------------------------------------------------------------------------

// True if this is a networked player
inline bool AbstractPlayer::isNetworkedPlayer() const
{
   return (nib != nullptr);
}

// True if this is a local player
inline bool AbstractPlayer::isLocalPlayer() const
{
   return (nib == nullptr);
}

// ID of a networked player's controlling network model
inline int AbstractPlayer::getNetworkID() const
{
   return netID;
}

// Networked player's Nib object
inline AbstractNib* AbstractPlayer::getNib()
{
   return nib;
}

// Networked player's Nib object  (const version)
inline const AbstractNib* AbstractPlayer::getNib() const
{
   return nib;
}

// is player output to the network enabled?
inline bool AbstractPlayer::isNetOutputEnabled() const
{
   return enableNetOutput;
}




#endif

