
#include "openeaagles/instruments/Instrument.hpp"
#include "openeaagles/base/Number.hpp"
#include "openeaagles/graphics/ColorRotary.hpp"
#include "openeaagles/base/functors/Tables.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Pair.hpp"

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(Instrument, "Instrument")
EMPTY_SERIALIZER(Instrument)

BEGIN_SLOTTABLE(Instrument)
    "scalingTable",         // 1) table for figuring linear interpolation (if not a linear scale)
    "instVal",              // 2) our instrument value
    "allowComponentPass",   // 3) if this is true, we will send all instrument values we receive down to our components.
END_SLOTTABLE(Instrument)

BEGIN_SLOT_MAP(Instrument)
    ON_SLOT(1, setSlotScalingTable, base::Table1)
    ON_SLOT(2, setSlotInstVal, base::Number)
    ON_SLOT(3, setSlotAllowValPass, base::Number)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Instrument)
    ON_EVENT_OBJ(UPDATE_INSTRUMENTS, onUpdateInstVal, base::Number)
END_EVENT_HANDLER()

Instrument::Instrument()
{
    STANDARD_CONSTRUCTOR()
    myTable = nullptr;
    instVal = 0;
    allowPassing = true;
    preScaleInstVal = 0;
}

void Instrument::copyData(const Instrument& org, const bool cc)
{
    // Copy our baseclass stuff first
    BaseClass::copyData(org);

    if (cc) myTable = nullptr;

    if (org.myTable != nullptr) {
        base::Table1* copy = org.myTable->clone();
        setSlotScalingTable( copy );
        copy->unref();
    }
    else {
        setSlotScalingTable(nullptr);
    }

    instVal = org.instVal;
    allowPassing = org.allowPassing;
    preScaleInstVal = org.preScaleInstVal;
}

void Instrument::deleteData()
{
   if (myTable != nullptr) {
      myTable->unref();
      myTable = nullptr;
   }
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotScalingTable() --
//------------------------------------------------------------------------------
bool Instrument::setSlotScalingTable(const base::Table1* const newTable)
{
   bool ok = false;
   if (newTable != nullptr) {
      if (myTable != nullptr) myTable->unref();
      myTable = newTable;
      if (myTable != nullptr) myTable->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotInstVal() -- sets our instrument value slot
//------------------------------------------------------------------------------
bool Instrument::setSlotInstVal(const base::Number* const newVal)
{
    bool ok = false;
    if (newVal != nullptr) ok = setInstVal(newVal->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotAllowValPass() --
//------------------------------------------------------------------------------
bool Instrument::setSlotAllowValPass(const base::Number* const newAVP)
{
    bool ok = false;
    if (newAVP != nullptr) ok = setAllowValPass(newAVP->getBoolean());
    return ok;
}

// SET Functions
//------------------------------------------------------------------------------
// setAllowValPass() -- allow our values to be passed down
//------------------------------------------------------------------------------
bool Instrument::setAllowValPass(const bool newVP)
{
    allowPassing = newVP;
    return true;
}

// EVENT
// Update our gauge if necessary, then send the event down to all of our graphic components who need it
//------------------------------------------------------------------------------
// onUpdateInstVal() -- update our instrument value
//------------------------------------------------------------------------------
bool Instrument::onUpdateInstVal(const base::Number* const newPos)
{
    bool ok = false;
    // now call our set function
    ok = setInstVal(newPos->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setInstVal() -- sets our instrument val
//------------------------------------------------------------------------------
bool Instrument::setInstVal(const double newPos)
{
    // store our raw instrument value, in case some instruments need them
    preScaleInstVal = newPos;

    // do we have a table to use?
    if (myTable != nullptr) instVal = myTable->lfi(newPos);
    else instVal = newPos;
    return true;
}

//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void Instrument::updateData(const double dt)
{
   // update our base class
   BaseClass::updateData(dt);

   // check for a color rotary, just in case we need one
   auto cr = dynamic_cast<graphics::ColorRotary*>(getColor());
   if (cr != nullptr) cr->determineColor(preScaleInstVal);

   // only tell the rest of our instruments our value if we want them to know it
   if (allowPassing) {
      // sort out the instruments from our components
      base::PairStream* ps = getComponents();
      if (ps != nullptr) {
         base::List::Item* item = ps->getFirstItem();
         while(item != nullptr) {
            auto pair = dynamic_cast<base::Pair*>(item->getValue());
            if (pair != nullptr) {
               // send the value down to all of our instrument components
               auto myInst = dynamic_cast<Instrument*>(pair->object());
               base::Number n = preScaleInstVal;
               if (myInst != nullptr) myInst->event(UPDATE_INSTRUMENTS, &n);
            }
            item = item->getNext();
         }
         ps->unref();
         ps = nullptr;
      }
   }
}

}
}
