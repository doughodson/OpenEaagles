#include "openeaagles/instruments/dials/AltitudeDial.h"

namespace oe {
namespace instruments {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(AltitudeDial,"AltitudeDial")
EMPTY_SERIALIZER(AltitudeDial)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AltitudeDial::AltitudeDial()
{
    STANDARD_CONSTRUCTOR()

    dialAltSD.empty();
    altHundredsSD.empty();
    altOneThousandsSD.empty();
    altTenThousandsSD.empty();
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void AltitudeDial::copyData(const AltitudeDial& org, const bool)
{
    BaseClass::copyData(org);

    dialAltSD.empty();
    altHundredsSD.empty();
    altOneThousandsSD.empty();
    altTenThousandsSD.empty();
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(AltitudeDial)

//------------------------------------------------------------------------------
// updateData() - update non time-critical threads here
//------------------------------------------------------------------------------
void AltitudeDial::updateData(const LCreal dt)
{
    // update base class first
    BaseClass::updateData(dt);

    // figure our rotation with the new "raw" instrument value
    LCreal alt = getInstValue();

    // send that data to the tape gauge
    LCreal altTens = ((alt/100) - static_cast<int>(alt/100)) * 10;
    LCreal altHundreds = ((alt/1000) - static_cast<int>(alt/1000)) * 10;
    // now figure out our ONE thousands!
    LCreal altOneThousands =  alt / 1000;
    // figure alt TEN thousands!
    LCreal altTenThousands = alt / 10000;
    if (altOneThousands >= 10) altOneThousands = altOneThousands - static_cast<int>(altTenThousands) * 10;
    // now figure the rest of the number
//    int altRest = int(alt/99.9999);

    if (altTens < 9) altHundreds = static_cast<LCreal>(static_cast<int>(altHundreds));
    else {
        LCreal x = altTens - static_cast<int>(altTens);
        altHundreds = static_cast<int>(altHundreds) + x;
    }
    if (altHundreds < 9) altOneThousands = static_cast<LCreal>(static_cast<int>(altOneThousands));
    else {
        // scale our hundreds value to lock with the tens
        LCreal x = altHundreds - static_cast<int>(altHundreds);
        altOneThousands = static_cast<int>(altOneThousands) + x;
    }
    if (altOneThousands < 9) altTenThousands = static_cast<LCreal>(static_cast<int>(altTenThousands));
    else {
        // scale our hundreds value to lock with the tens
        LCreal x = altOneThousands - static_cast<int>(altOneThousands);
        altTenThousands = static_cast<int>(altTenThousands) + x;
    }

    // send our data to our components
    send("dialalt", UPDATE_INSTRUMENTS, alt, dialAltSD);
    send("hundreds", UPDATE_INSTRUMENTS, altHundreds, altHundredsSD);
    send("onethousands", UPDATE_INSTRUMENTS, altOneThousands, altOneThousandsSD);
    send("tenthousands", UPDATE_INSTRUMENTS, altTenThousands, altTenThousandsSD);
}

//------------------------------------------------------------------------------
// figureRotation() - figures out where to rotate our gauge
// and translate our tape based on our altitude
//------------------------------------------------------------------------------
bool AltitudeDial::figureRotation(const LCreal thisAlt)
{
    // break down our number to tenThousands, thousands, and hundreds
    LCreal hundreds = 0;
    LCreal thousands = 0;
    LCreal tenThousands = 0;

    tenThousands = thisAlt/10000;
    thousands = (thisAlt/1000) - (static_cast<int>(thisAlt/10000)* 10);
    hundreds = ((thisAlt - (static_cast<int>(thisAlt/1000) * 1000)) / 100);

    return true;
}

}  // end instruments namespace
}  // end oe namespace

