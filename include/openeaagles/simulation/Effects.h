//------------------------------------------------------------------------------
// Classes: Effects, Chaff, Flare and Decoy
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Effects_H__
#define __Eaagles_Simulation_Effects_H__

#include "openeaagles/simulation/Weapon.h"

namespace Eaagles {
namespace Simulation {

//------------------------------------------------------------------------------
// Class: Effects
// Description: Base class for effects (chaff, flares, decoys, etc). 
//
//    Even though they're not weapons, the Effects class is derived from
//    the Weapon class because their basic behaviours are the same.  That is,
//    they can be jettisoned and released, or pre-released() and be become
//    an independent player.  They can also be managed by a Stores class.
//
// Factory name: Effects
// Slots:
//    dragIndex   <Number>   ! drag index used by default dynamics (default: 0.0006f)
//
//------------------------------------------------------------------------------
class Effects : public Weapon  
{
    DECLARE_SUBCLASS(Effects,Weapon)

public:
    Effects();

    LCreal getDragIndex() const                    { return dragIndex; }
    void setDragIndex(const LCreal v)              { dragIndex = v; }

    // Weapon interface
    virtual const char* getDescription() const;
    virtual const char* getNickname() const;
    virtual int getCategory() const;

    // Player interface
    virtual bool collisionNotification(Player* const p);
    virtual bool crashNotification();

protected:
   bool setSlotDragIndex(Basic::Number* const p);

   // Weapon interface
   virtual void weaponDynamics(const LCreal dt);
   virtual void updateTOF(const LCreal dt);

private:
    LCreal dragIndex;             // Drag Index
};

//------------------------------------------------------------------------------
// Class: Chaff
// Description: Generic chaff class
// Factory name: Chaff
//------------------------------------------------------------------------------
class Chaff : public Effects
{
    DECLARE_SUBCLASS(Chaff,Effects)

public:
    Chaff();

    // Weapon interface
    virtual const char* getDescription() const;
    virtual const char* getNickname() const;
    virtual int getCategory() const;
};

//------------------------------------------------------------------------------
// Class: Flare
// Description: Generic flare class
// Factory name: Flare
//------------------------------------------------------------------------------
class Flare : public Effects
{
    DECLARE_SUBCLASS(Flare,Effects)

public:
    Flare();

    // Weapon interface
    virtual const char* getDescription() const;
    virtual const char* getNickname() const;
    virtual int getCategory() const;
};

//------------------------------------------------------------------------------
// Class: Decoy
// Description: Generic decoy class
// Factory name: Decoy
//------------------------------------------------------------------------------
class Decoy : public Effects  
{
    DECLARE_SUBCLASS(Decoy,Effects)

public:
    Decoy();

    // Weapon interface
    virtual const char* getDescription() const;
    virtual const char* getNickname() const;
    virtual int getCategory() const;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
