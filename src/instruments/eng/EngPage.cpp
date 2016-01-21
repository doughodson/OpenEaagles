#include "openeaagles/instruments/eng/EngPage.h"

namespace oe {
namespace Instruments {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(EngPage,"EngPage")
EMPTY_SERIALIZER(EngPage)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
EngPage::EngPage()
{
    STANDARD_CONSTRUCTOR()

    for (int i = 0; i < NUM_ENG; i++) {
        n1[i] = 0.0;
        n1SD[i].empty();
        n1ROSD[i].empty();
        n1BoxSD[i].empty();

        n2[i] = 0.0;
        n2SD[i].empty();
        n2ROSD[i].empty();
        n2BoxSD[i].empty();

        tit[i] = 0.0;
        titSD[i].empty();
        titROSD[i].empty();
        titBoxSD[i].empty();

        ff[i] = 0.0;
        ffSD[i].empty();
        ffROSD[i].empty();
    }
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void EngPage::copyData(const EngPage& org, const bool)
{
    // Always copy base class stuff first
    BaseClass::copyData(org);

    for (int i = 0; i < NUM_ENG; i++) {
        n1[i] = org.n1[i];
        n1SD[i].empty();
        n1ROSD[i].empty();
        n1BoxSD[i].empty();

        n2[i] = org.n2[i];
        n2SD[i].empty();
        n2ROSD[i].empty();
        n2BoxSD[i].empty();

        tit[i] = org.tit[i];
        titSD[i].empty();
        titROSD[i].empty();
        titBoxSD[i].empty();

        ff[i] = org.ff[i];
        ffSD[i].empty();
        ffROSD[i].empty();
    }
}

//------------------------------------------------------------------------------
//deleteData() -- delete this object's data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(EngPage)

// Set functions
//------------------------------------------------------------------------------
// setEngN1() -
//------------------------------------------------------------------------------
bool EngPage::setEngN1(const int idx, const LCreal newN1)
{
    bool ok = false;
    if (idx > 0 && idx <= NUM_ENG) {
        ok = true;
        n1[idx-1] = newN1;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setEngN2() -
//------------------------------------------------------------------------------
bool EngPage::setEngN2(const int idx, const LCreal newN2)
{
    bool ok = false;
    if (idx > 0 && idx <= NUM_ENG) {
        ok = true;
        n2[idx-1] = newN2;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setEngTit() -
//------------------------------------------------------------------------------
bool EngPage::setEngTit(const int idx, const LCreal newT)
{
    bool ok = false;
    if (idx > 0 && idx <= NUM_ENG) {
        ok = true;
        tit[idx-1] = newT;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setEngFF() -
//------------------------------------------------------------------------------
bool EngPage::setEngFF(const int idx, const LCreal newFF)
{
    bool ok = false;
    if (idx > 0 && idx <= NUM_ENG) {
        ok = true;
        ff[idx-1] = newFF;
    }
    return ok;
}

//------------------------------------------------------------------------------
// updateData() -- update non time-critical threads here
//------------------------------------------------------------------------------
void EngPage::updateData(const LCreal dt)
{
    // update our BaseClass
    BaseClass::updateData(dt);

    // Box visibility flags
    int n1Box[NUM_ENG];
    int n2Box[NUM_ENG];
    int titBox[NUM_ENG];

    for (int i = 0; i < NUM_ENG; i++) {
        n1Box[i] =  (n1[i] > 106.9 || n1[i] < 17);
        n2Box[i] =  (n2[i] > 102.4 || n2[i] < 60);
        titBox[i] = (tit[i] > 916  || tit[i] < 219);
    }

    // send all of our engine 1 n1 values out
    send("eng%1dn1",    UPDATE_INSTRUMENTS, n1, n1SD, NUM_ENG);
    send("eng%1dn1ro",  UPDATE_VALUE, n1, n1ROSD, NUM_ENG);
    send("eng%1dn1box", SET_VISIBILITY, n1Box, n1BoxSD, NUM_ENG);

    //// send all of our engine 1 N2 values out
    send("eng%1dn2",    UPDATE_INSTRUMENTS, n2, n2SD, NUM_ENG);
    send("eng%1dn2ro",  UPDATE_VALUE, n2, n2ROSD, NUM_ENG);
    send("eng%1dn2box", SET_VISIBILITY, n1Box, n2BoxSD, NUM_ENG);

    //// send all of our engine 1 TIT values
    send("eng%1dtit",    UPDATE_INSTRUMENTS, tit, titSD, NUM_ENG);
    send("eng%1dtitro",  UPDATE_VALUE, tit, titROSD, NUM_ENG);
    send("eng%1dtitbox", SET_VISIBILITY, titBox, titBoxSD, NUM_ENG);

    // send all of our engine 1 fuel flow valuew
    LCreal ff1K[NUM_ENG];
    for (int i = 0; i < NUM_ENG; i++) {
        ff1K[i] = ff[i]/1000.0f;    // convert to Klbs/hrs
    }
    send("eng%1dff",   UPDATE_INSTRUMENTS, ff1K, ffSD, NUM_ENG);
    send("eng%1dffro", UPDATE_VALUE,       ff1K, ffROSD, NUM_ENG);

}

}  // end of Instruments namespace
}  // end of oe Namespace
