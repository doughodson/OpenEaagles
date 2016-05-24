#include "openeaagles/simulation/Iff.h"

#include "openeaagles/base/Number.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/String.h"

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(Iff,"Iff")
EMPTY_SERIALIZER(Iff)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Iff)
   "mode1",          //  1) Mode 1 Code   (range: 00 to 073 octal)
                     //                    -- first digit 0 to 7, second digit 0 to 3
   "mode2",          //  2) Mode 2 Code   (range: 0000 to 07777 octal)
   "mode3a",         //  3) Mode 3a Code  (range: 0000 to 07777 octal)
   "mode4a",         //  4) Mode 4a Code
   "mode4b",         //  5) Mode 4b Code
   "enableMode1",    //  6) Mode 1 on
   "enableMode2",    //  7) Mode 2 on
   "enableMode3a",   //  8) Mode 3a on
   "enableMode4",    //  9) Mode 4a on
   "whichMode4",     // 10) Which mode 4 are we using (A == false; B == true)
   "enableModeC",    // 11) Mode C on
END_SLOTTABLE(Iff)

// Map slot table to handles
BEGIN_SLOT_MAP(Iff)
   ON_SLOT( 1, setSlotMode1,        base::Number)
   ON_SLOT( 2, setSlotMode2,        base::Number)
   ON_SLOT( 3, setSlotMode3a,       base::Number)
   ON_SLOT( 4, setSlotMode4a,       base::Number)
   ON_SLOT( 5, setSlotMode4b,       base::Number)
   ON_SLOT( 6, setSlotEnableMode1,  base::Number)
   ON_SLOT( 7, setSlotEnableMode2,  base::Number)
   ON_SLOT( 8, setSlotEnableMode3a, base::Number)
   ON_SLOT( 9, setSlotEnableMode4, base::Number)
   ON_SLOT( 10, setSlotWhichMode4,  base::Number)
   ON_SLOT( 11, setSlotEnableModeC, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
Iff::Iff()
{
   STANDARD_CONSTRUCTOR()

   mode1 = 0;
   mode2 = 0;
   mode3a = 0;
   mode4a = 0;
   mode4b = 0;

   enableMode1 = 0;
   enableMode2 = 0;
   enableMode3a = 0;
   enableMode4 = 0;
   enableModeC = 0;

   whichMode4 = false;
   icMode1 = 0;
   icMode2 = 0;
   icMode3a = 0;
   icMode4a = 0;
   icMode4b = 0;
   icMode1Flg = 0;
   icMode2Flg = 0;
   icMode3aFlg = 0;
   icMode4Flg = 0;
   icModeCFlg = 0;
   icWhichMode4 = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Iff::copyData(const Iff& org, const bool)
{
   BaseClass::copyData(org);

   mode1 = org.mode1;
   mode2 = org.mode2;
   mode3a = org.mode3a;
   mode4a = org.mode4a;
   mode4b = org.mode4b;

   enableMode1 = org.enableMode1;
   enableMode2 = org.enableMode2;
   enableMode3a = org.enableMode3a;
   enableMode4 = org.enableMode4;
   enableModeC = org.enableModeC;
   whichMode4 = org.whichMode4;

   icMode1 = org.icMode1;
   icMode2 = org.icMode2;
   icMode3a = org.icMode3a;
   icMode4a = org.icMode4a;
   icMode4b = org.icMode4b;
   icMode1Flg = org.icMode1Flg;
   icMode2Flg = org.icMode2Flg;
   icMode3aFlg = org.icMode3aFlg;
   icMode4Flg = org.icMode4Flg;
   icModeCFlg = org.icModeCFlg;
   icWhichMode4 = org.icWhichMode4;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Iff::deleteData()
{
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Iff::reset()
{
   BaseClass::reset();

   setMode1(icMode1);
   setEnabledMode1(icMode1Flg);

   setMode2(icMode2);
   setEnabledMode2(icMode2Flg);

   setMode3a(icMode3a);
   setEnabledMode3a(icMode3aFlg);

   setMode4A(icMode4a);
   setEnabledMode4(icMode4Flg);
   setMode4B(icMode4b);

   setWhichMode4(icWhichMode4);

   setEnabledModeC(icModeCFlg);

}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool Iff::setMode1(const unsigned short m)
{
   //mode1 = (m && 073);
   mode1 = m;
   return true;
}

bool Iff::setMode2(const unsigned short m)
{
   mode2 = m;
   return true;
}

bool Iff::setMode3a(const unsigned short m)
{
   mode3a = m;
   return true;
}

bool Iff::setMode4A(const unsigned short m)
{
   mode4a = m;
   return true;
}
bool Iff::setMode4B(const unsigned short m)
{
   mode4b = m;
   return true;
}

void Iff::setEnabledMode1(const bool flg)
{
   enableMode1 = flg;
}

void Iff::setEnabledMode2(const bool flg)
{
   enableMode2 = flg;
}

void Iff::setEnabledMode3a(const bool flg)
{
   enableMode3a = flg;
}

void Iff::setEnabledMode4(const bool flg)
{
    enableMode4 = flg;
}
void Iff::setEnabledModeC(const bool flg)
{
   enableModeC = flg;
}
bool Iff::setWhichMode4(const bool newM)
{
    whichMode4 = newM;
    return true;
}

// Slot functions
bool Iff::setSlotEnableMode1(const base::Number* const msg)
{
    if (msg != nullptr) setEnabledMode1(msg->getBoolean());
    // make sure we hold this reset value
    icMode1Flg = enableMode1;
    return true;
}
bool Iff::setSlotEnableMode2(const base::Number* const msg)
{
    if (msg != nullptr) setEnabledMode2(msg->getBoolean());
    icMode2Flg = enableMode2;
    return true;
}
bool Iff::setSlotEnableMode3a(const base::Number* const msg)
{
    if (msg != nullptr) setEnabledMode3a(msg->getBoolean());
    icMode3aFlg = enableMode3a;
    return true;
}
bool Iff::setSlotEnableMode4(const base::Number* const msg)
{
    if (msg != nullptr) setEnabledMode4(msg->getBoolean());
    icMode4Flg = enableMode4;
    return true;
}
bool Iff::setSlotWhichMode4(const base::Number* const msg)
{
    if (msg != nullptr) setWhichMode4(msg->getBoolean());
    icWhichMode4 = whichMode4;
    return true;
}
bool Iff::setSlotEnableModeC(const base::Number* const msg)
{
    if (msg != nullptr) setEnabledModeC(msg->getBoolean());
    icModeCFlg = enableModeC;
    return true;
}
bool Iff::setSlotMode1(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) ok = setMode1(static_cast<unsigned short>(msg->getInt()));
    if (ok) icMode1 = mode1;
    return ok;
}
bool Iff::setSlotMode2(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) ok = setMode2(static_cast<unsigned short>(msg->getInt()));
    if (ok) icMode2 = mode2;
    return ok;
}
bool Iff::setSlotMode3a(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) ok = setMode3a(static_cast<unsigned short>(msg->getInt()));
    if (ok) icMode3a = mode3a;
    return ok;
}
bool Iff::setSlotMode4a(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) ok = setMode4A(static_cast<unsigned short>(msg->getInt()));
    icMode4a = mode4a;
    return ok;
}
bool Iff::setSlotMode4b(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) ok = setMode4B(static_cast<unsigned short>(msg->getInt()));
    icMode4b = mode4b;
    return ok;
}
//bool Iff::setSlotPower(const base::String* const msg)
//{
//    if (strcmp(msg->getString(), "OFF") == 0) pwr = OFF;
//    else if (strcmp(msg->getString(), "STANDBY") == 0) pwr = STANDBY;
//    else if (strcmp(msg->getString(), "LOW") == 0) pwr = LOW;
//    else if (strcmp(msg->getString(), "NORMAL") == 0) pwr = NORMAL;
//    else if (strcmp(msg->getString(), "EMERGENCY") == 0) pwr = EMERGENCY;
//    return true;
//}
bool Iff::zeroMode4Codes()
{
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* Iff::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End simulation namespace
}
