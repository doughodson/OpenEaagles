//------------------------------------------------------------------------------
// Classes: Effects, Chaff, Flare and Decoy
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Effects_H__
#define __oe_simulation_Effects_H__

#include "openeaagles/simulation/Weapon.h"

namespace oe {
namespace simulation {

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

    double getDragIndex() const                    { return dragIndex; }
    void setDragIndex(const double v)              { dragIndex = v; }

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;

    bool collisionNotification(Player* const p) override;
    bool crashNotification() override;

protected:
   bool setSlotDragIndex(base::Number* const p);

   void weaponDynamics(const double dt) override;
   void updateTOF(const double dt) override;

private:
    double dragIndex;             // Drag Index
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

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
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

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
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

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
};

} // End simulation namespace
} // End oe namespace

#endif
