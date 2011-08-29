#include "openeaagles/simulation/FuelTank.h"

#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// FuelTank class -- Generic external fuel tank
//==============================================================================
IMPLEMENT_SUBCLASS(FuelTank, "FuelTank")

BEGIN_SLOTTABLE(FuelTank)
    "fuelWt",     // 1: Fuel wt (lb)
    "capacity",   // 2: Tank capacity (lb)
END_SLOTTABLE(FuelTank)

BEGIN_SLOT_MAP(FuelTank)
    ON_SLOT(1, setSlotFuelWt, Basic::Number)
    ON_SLOT(2, setSlotCapacity, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
FuelTank::FuelTank()
{
    STANDARD_CONSTRUCTOR()

    capacity   = -1;
    fuelWt     = -1;
    initFuelWt = -1;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void FuelTank::copyData(const FuelTank& org, const bool)
{
    BaseClass::copyData(org);

    capacity   = org.capacity;
    fuelWt     = org.fuelWt;
    initFuelWt = org.initFuelWt;
}

EMPTY_DELETEDATA(FuelTank)

//------------------------------------------------------------------------------
// revert to our initial fuel weight
//------------------------------------------------------------------------------
void FuelTank::reset()
{
    BaseClass::reset();

    fuelWt = initFuelWt;
}

//------------------------------------------------------------------------------
// Tank capacity (lb) functioms
//------------------------------------------------------------------------------
LCreal FuelTank::getCapacity() const            { return capacity;        }
bool   FuelTank::setCapacity(const LCreal cap)  { capacity = cap; return true; }
bool   FuelTank::isCapacityValid() const        { return (capacity >= 0); }

//------------------------------------------------------------------------------
// Fuel weight (lb) functioms
//------------------------------------------------------------------------------
LCreal FuelTank::getFuelWt() const              { return fuelWt;        }
bool   FuelTank::setFuelWt(const LCreal wt)     { fuelWt = wt; return true; }
bool   FuelTank::isFuelWtValid() const          { return (fuelWt >= 0 && fuelWt <= capacity); }

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool FuelTank::setSlotFuelWt(const Basic::Number* const msg)
{
    if (msg == 0) return false;
    return setFuelWt( msg->getReal() );
}

bool FuelTank::setSlotCapacity(const Basic::Number* const msg)
{
    if (msg == 0) return false;
    return setCapacity( msg->getReal() );
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* FuelTank::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& FuelTank::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "fuelWt: " << initFuelWt << std::endl;

    BaseClass::serialize(sout,i+j,true);
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }
    return sout;
}

} // end Simulation namespace
} // end Eaagles namespace

