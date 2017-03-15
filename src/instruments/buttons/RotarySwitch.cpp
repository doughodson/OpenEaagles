
#include "openeaagles/instruments/buttons/RotarySwitch.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/Number.hpp"

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(RotarySwitch, "RotarySwitch")
EMPTY_SERIALIZER(RotarySwitch)
EMPTY_DELETEDATA(RotarySwitch)

BEGIN_SLOTTABLE(RotarySwitch)
    "angles",           // 1) List of angles to use
    "startPosition",    // 2) Starting position we are using
END_SLOTTABLE(RotarySwitch)

BEGIN_SLOT_MAP(RotarySwitch)
   ON_SLOT(1, setSlotAngles, base::PairStream)
   ON_SLOT(2, setSlotStartPosition, base::Number)
END_SLOT_MAP()

RotarySwitch::RotarySwitch()
{
    STANDARD_CONSTRUCTOR()
    angleSD.empty();
}

void RotarySwitch::copyData(const RotarySwitch& org, const bool)
{
    BaseClass::copyData(org);
    numAngs = org.numAngs;
    angles = org.angles;
    angleSD.empty();
    currentPosition = org.currentPosition;
    startPosition = org.startPosition;
}

//------------------------------------------------------------------------------
// setSlotNumPositions() - sets number of positions for switch
//------------------------------------------------------------------------------
bool RotarySwitch::setSlotAngles(const base::PairStream* const x)
{
    bool ok = false;
    numAngs = 0;
    for (int i = 0; i < MAX_ANGLES; i++) angles[i] = 0;
    if (x != nullptr) {
        ok = true;
        const base::List::Item* item = x->getFirstItem();
        while(item != nullptr) {
            base::Pair* pair = (base::Pair*)item->getValue();
            if (pair != nullptr) {
                const auto n = dynamic_cast<base::Number*>(pair->object());
                if (n != nullptr) {
                    angles[numAngs++] = n->getReal();
                }
            }
            item = item->getNext();
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotStartPosition() - set the position we start from
//------------------------------------------------------------------------------
bool RotarySwitch::setSlotStartPosition(const base::Number* const x)
{
    if (x != nullptr) {
        startPosition = x->getInt();
        currentPosition = startPosition;
    }
    return true;
}

//------------------------------------------------------------------------------
// onSingleClick() ie left mouse click - rotate switch to next position
//------------------------------------------------------------------------------
bool RotarySwitch::onSingleClick()
{
   //are we past last switch position?  if yes move to start position if no move to next switch position
   if(currentPosition == numAngs){
      currentPosition = 1;
   }
   else {
      currentPosition++;
   }

   // let our parent button do the rest
   BaseClass::onSingleClick();

   return true;
}

void RotarySwitch::updateData(const double dt)
{
    BaseClass::updateData(dt);

    send("switch", UPDATE_VALUE6, angles[currentPosition - 1], angleSD);
}

}
}
