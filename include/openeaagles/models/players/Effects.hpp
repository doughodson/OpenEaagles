
#ifndef __oe_models_Effects_H__
#define __oe_models_Effects_H__

#include "openeaagles/models/players/Weapon.hpp"

namespace oe {
namespace models {

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
    DECLARE_SUBCLASS(Effects, Weapon)

public:
    Effects();

    double getDragIndex() const                    { return dragIndex; }
    void setDragIndex(const double v)              { dragIndex = v; }

    virtual const char* getDescription() const override;
    virtual const char* getNickname() const override;
    virtual int getCategory() const override;

    virtual bool collisionNotification(Player* const p) override;
    virtual bool crashNotification() override;

protected:
    bool setSlotDragIndex(base::Number* const p);

    virtual void weaponDynamics(const double dt) override;
    virtual void updateTOF(const double dt) override;

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
    DECLARE_SUBCLASS(Chaff, Effects)

public:
    Chaff();

    virtual const char* getDescription() const override;
    virtual const char* getNickname() const override;
    virtual int getCategory() const override;
};

//------------------------------------------------------------------------------
// Class: Flare
// Description: Generic flare class
// Factory name: Flare
//------------------------------------------------------------------------------
class Flare : public Effects
{
    DECLARE_SUBCLASS(Flare, Effects)

public:
    Flare();

    virtual const char* getDescription() const override;
    virtual const char* getNickname() const override;
    virtual int getCategory() const override;
};

//------------------------------------------------------------------------------
// Class: Decoy
// Description: Generic decoy class
// Factory name: Decoy
//------------------------------------------------------------------------------
class Decoy : public Effects
{
    DECLARE_SUBCLASS(Decoy, Effects)

public:
    Decoy();

    virtual const char* getDescription() const override;
    virtual const char* getNickname() const override;
    virtual int getCategory() const override;
};

}
}

#endif
