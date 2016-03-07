#include "openeaagles/instruments/gauges/Tape.h"
#include "openeaagles/base/Number.h"

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(Tape, "Tape")
EMPTY_SERIALIZER(Tape)

BEGIN_SLOTTABLE(Tape)
    "range",            // 1) Total range (in units) of the scale
    "height",           // 2) Height of the total amount of clipped area you have
    "increment",        // 3) How much spaced (units) between each number?
    "vertical",         // 4) do we show negative numbers?
    "maximum",          // 5) Maximum value before we don't display anymore
    "minimum",          // 6) minimum value before we don't display anymore
    "convert",          // 7) convert our max and min values to make a "rotating" display (like a heading tape, for example
    // 360 degrees would be 0 degrees, and -10 degrees would be 350 degrees, and so on)
END_SLOTTABLE(Tape)
//
//------------------------------------------------------------------------------
//  Map slot table to handles for Analog Dial
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Tape)
    ON_SLOT(1, setSlotRange, base::Number)
    ON_SLOT(2, setSlotHeight, base::Number)
    ON_SLOT(3, setSlotIncrement, base::Number)
    ON_SLOT(4, setSlotVertical, base::Number)
    ON_SLOT(5, setSlotMaxNum, base::Number)
    ON_SLOT(6, setSlotMinNum, base::Number)
    ON_SLOT(7, setSlotConvert, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Tape::Tape()
{
    STANDARD_CONSTRUCTOR()

    transTapeGraphicSD.empty();
    transTapeGraphicVSD.empty();

    for (int i = 0; i < MAX_NUMBERS; i++) {
        numberValsHunds[i] = 0;
        numberValsHundsVis[i] = 1;
        numberValsThous[i] = 0;
        numberValsThousVis[i] = 1;
        numberValsHundsSD[i].empty();
        numberValsHundsVisSD[i].empty();
        numberValsThousSD[i].empty();
        numberValsThousVisSD[i].empty();
    }
    range = 0;
    height = 0;
    increment = 1;
    vertical = true;
    // these values are ignored
    maxNum = -1;
    minNum = -1;
    convert = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Tape::copyData(const Tape& org, const bool)
{
    // Copy our baseclass stuff first
    BaseClass::copyData(org);

    transTapeGraphicSD.empty();
    transTapeGraphicVSD.empty();

    for (int i = 0; i < MAX_NUMBERS; i++) {
        numberValsHunds[i] = org.numberValsHunds[i];
        numberValsHundsVis[i] = org.numberValsHundsVis[i];
        numberValsThous[i] = org.numberValsThous[i];
        numberValsThousVis[i] = org.numberValsThousVis[i];
        numberValsHundsSD[i].empty();
        numberValsHundsVisSD[i].empty();
        numberValsThousSD[i].empty();
        numberValsThousVisSD[i].empty();
    }

    range = org.range;
    height = org.height;
    increment = org.increment;
    vertical = org.vertical;
    maxNum = org.maxNum;
    minNum = org.minNum;
    convert = org.convert;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Tape)

// SLOT Functions
//------------------------------------------------------------------------------
// setSlotRange() - set the range of our data
//------------------------------------------------------------------------------
bool Tape::setSlotRange(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setRange(x->getInt());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotHeight() - set the height of our viewable tape area
//------------------------------------------------------------------------------
bool Tape::setSlotHeight(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setHeight(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotIncrement() - increment of our tape
//------------------------------------------------------------------------------
bool Tape::setSlotIncrement(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setIncrement(x->getInt());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotVertical() - vertical or horizontal tape
//------------------------------------------------------------------------------
bool Tape::setSlotVertical(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setVertical(x->getBoolean());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotMaxNum() - set the tape's maximum value
//------------------------------------------------------------------------------
bool Tape::setSlotMaxNum(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setMaxNumber(x->getReal());
    return ok;
}


//------------------------------------------------------------------------------
// setSlotMinNum() - set the tape's minimum value
//------------------------------------------------------------------------------
bool Tape::setSlotMinNum(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setMinNumber(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotConvert() - conver to degrees instead of units?  (for circular tapes)
//------------------------------------------------------------------------------
bool Tape::setSlotConvert(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setConvert(x->getBoolean());
    return ok;
}

//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void Tape::updateData(const double dt)
{
   // update our base class
   BaseClass::updateData(dt);

    // std::cout << "INSTRUMENT VALUE = " << getInstValue() << std::endl;
    double x = getInstValue();

    // we take our range, add another for the 0, and then add 2 more for fillers
    int perRange = static_cast<int>(range / increment) + 3;
    // based on that we know how many numbers we have

    // we are diving by 100, so we need to find the nearest 100!
    double temp = x / increment;
    int nearest = nint(temp);

    //std::cout << "NEAREST = " << nearest << std::endl;
    // we know we have 11 total number vals, and we start at the low side
    int val = (nearest * increment) - static_cast<int>(perRange/2) * increment;
    //if (val > 100) {
    //  if (200 - val > 100) val = val - 100;
   //}

    //std::cout << "PER RANGE = " << perRange << std::endl;
    // this if for the hundreds
    double tempVal = 0;
    int vis = 0;
    for (int i = 0; i < MAX_NUMBERS; i++) {
        vis = 0;
        if (maxNum != -1 && minNum != -1) {
            // inside our max and min num
            if (val <= maxNum && val >= minNum) {
                vis = 1;
                numberValsHunds[i] = val;
            }
            // if we are ouside our max and min numbers, we do some conversion
            if (convert) {
                vis = 1;
                if (val > maxNum) {
                    tempVal = val - maxNum - increment;
                    // find out how many increments we are passed our last value
                    numberValsHunds[i] = static_cast<int>(tempVal);
                }
                else if (val < minNum) {
                    tempVal = val + maxNum + increment;
                    numberValsHunds[i] = static_cast<int>(tempVal);
                }
            }
        }
        else {
            vis = 1;
            numberValsHunds[i] = val;
        }

        numberValsHundsVis[i] = vis;

        // no max and min, we show everything!
        // now determine our thousands values
        tempVal = (static_cast<double>(val) / 1000);
        //if (tempVal < 1 && !showNegative) numberValsThousVis[i] = 0;
        //else numberValsThousVis[i] = 0;
        numberValsThousVis[i] = vis;
        numberValsThous[i] = static_cast<int>(tempVal);
        val += increment;
    }

    send("number%dhunds", UPDATE_VALUE, numberValsHunds, numberValsHundsSD, MAX_NUMBERS);
    send("number%dthous", UPDATE_VALUE, numberValsThous, numberValsThousSD, MAX_NUMBERS);
    send("number%dhunds", SET_VISIBILITY, numberValsHundsVis, numberValsHundsVisSD, MAX_NUMBERS);
    send("number%dthous", SET_VISIBILITY, numberValsThousVis, numberValsThousVisSD, MAX_NUMBERS);

    double unitsOfHeightPerRange = height/range;
    double newVal = (nearest * increment) - x;
    newVal *= unitsOfHeightPerRange;
        //std::cout << "NEW VALUE = " << newVal << std::endl;


    if (vertical) send("tapegraphic", UPDATE_VALUE2, newVal, transTapeGraphicSD);
    else send("tapegraphic", UPDATE_VALUE, newVal, transTapeGraphicVSD);
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Tape
//------------------------------------------------------------------------------
base::Object* Tape::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end instruments namespace
}  // end oe namespace
