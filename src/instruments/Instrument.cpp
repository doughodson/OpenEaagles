#include "openeaagles/instruments/Instrument.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basicGL/ColorRotary.h"
#include "openeaagles/basic/functors/Tables.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Pair.h"

namespace oe {
namespace Instruments {

IMPLEMENT_SUBCLASS(Instrument, "Instrument")
EMPTY_SERIALIZER(Instrument)

BEGIN_SLOTTABLE(Instrument)
    "scalingTable",         // 1) table for figuring linear interpolation (if not a linear scale)
    "instVal",              // 2) our instrument value
    "allowComponentPass",   // 3) if this is true, we will send all instrument values we receive down to our components.
END_SLOTTABLE(Instrument)

//------------------------------------------------------------------------------
//  Map slot table to handles for Instrument
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Instrument)
    ON_SLOT(1, setSlotScalingTable, basic::Table1)
    ON_SLOT(2, setSlotInstVal, basic::Number)
    ON_SLOT(3, setSlotAllowValPass, basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
//  Event table
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Instrument)
    ON_EVENT_OBJ(UPDATE_INSTRUMENTS, onUpdateInstVal, basic::Number)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Instrument::Instrument()
{
    STANDARD_CONSTRUCTOR()
    myTable = 0;
    instVal = 0;
    allowPassing = true;
    preScaleInstVal = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Instrument::copyData(const Instrument& org, const bool cc)
{
    // Copy our baseclass stuff first
    BaseClass::copyData(org);

    if (cc) myTable = 0;

    if (org.myTable != 0) {
        basic::Table1* copy = org.myTable->clone();
        setSlotScalingTable( copy );
        copy->unref();
    }
    else {
        setSlotScalingTable(0);
    }

    instVal = org.instVal;
    allowPassing = org.allowPassing;
    preScaleInstVal = org.preScaleInstVal;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Instrument::deleteData()
{
   if (myTable != 0) {
      myTable->unref();
      myTable = 0;
   }
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotScalingTable() --
//------------------------------------------------------------------------------
bool Instrument::setSlotScalingTable(const basic::Table1* const newTable)
{
   bool ok = false;
   if (newTable != 0) {
      if (myTable != 0) myTable->unref();
      myTable = newTable;
      if (myTable != 0) myTable->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotInstVal() -- sets our instrument value slot
//------------------------------------------------------------------------------
bool Instrument::setSlotInstVal(const basic::Number* const newVal)
{
    bool ok = false;
    if (newVal != 0) ok = setInstVal(newVal->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotAllowValPass() --
//------------------------------------------------------------------------------
bool Instrument::setSlotAllowValPass(const basic::Number* const newAVP)
{
    bool ok = false;
    if (newAVP != 0) ok = setAllowValPass(newAVP->getBoolean());
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
bool Instrument::onUpdateInstVal(const basic::Number* const newPos)
{
    bool ok = false;
    // now call our set function
    ok = setInstVal(newPos->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setInstVal() -- sets our instrument val
//------------------------------------------------------------------------------
bool Instrument::setInstVal(const LCreal newPos)
{
    // store our raw instrument value, in case some instruments need them
    preScaleInstVal = newPos;

    // do we have a table to use?
    if (myTable != 0) instVal = myTable->lfi(newPos);
    else instVal = newPos;
    return true;
}

//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void Instrument::updateData(const LCreal dt)
{
   // update our base class
   BaseClass::updateData(dt);

   // check for a color rotary, just in case we need one
   BasicGL::ColorRotary* cr = dynamic_cast<BasicGL::ColorRotary*>(getColor());
   if (cr != 0) cr->determineColor(preScaleInstVal);

   // only tell the rest of our instruments our value if we want them to know it
   if (allowPassing) {
      // sort out the instruments from our components
      basic::PairStream* ps = getComponents();
      if (ps != 0) {
         basic::List::Item* item = ps->getFirstItem();
         while(item != 0) {
            basic::Pair* pair = (basic::Pair*) item->getValue();
            if (pair != 0) {
               // send the value down to all of our instrument components
               Instrument* myInst = dynamic_cast<Instrument*>(pair->object());
               basic::Number n = preScaleInstVal;
               if (myInst != 0) myInst->event(UPDATE_INSTRUMENTS, &n);
            }
            item = item->getNext();
         }
         ps->unref();
         ps = 0;
      }
   }
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Instrument
//------------------------------------------------------------------------------
basic::Object* Instrument::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end Instruments namespace
}  // end oe namespace
