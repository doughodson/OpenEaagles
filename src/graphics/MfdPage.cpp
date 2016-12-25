#include "openeaagles/graphics/MfdPage.hpp"

namespace oe {
namespace graphics {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MfdPage,"MfdPage")
EMPTY_SERIALIZER(MfdPage)
EMPTY_COPYDATA(MfdPage)
EMPTY_DELETEDATA(MfdPage)

//------------------------------------------------------------------------------
// event handler macro - default - replaces event()
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(MfdPage)

   // ---
   // MFD switch hit events; supports up to a 10x10 MFD
   // ---

    ON_EVENT(OSB_T1, onOsbT1)
    ON_EVENT(OSB_T2, onOsbT2)
    ON_EVENT(OSB_T3, onOsbT3)
    ON_EVENT(OSB_T4, onOsbT4)
    ON_EVENT(OSB_T5, onOsbT5)
    ON_EVENT(OSB_T6, onOsbT6)
    ON_EVENT(OSB_T7, onOsbT7)
    ON_EVENT(OSB_T8, onOsbT8)
    ON_EVENT(OSB_T9, onOsbT9)
    ON_EVENT(OSB_T10,onOsbT10)

    ON_EVENT(OSB_R1, onOsbR1)
    ON_EVENT(OSB_R2, onOsbR2)
    ON_EVENT(OSB_R3, onOsbR3)
    ON_EVENT(OSB_R4, onOsbR4)
    ON_EVENT(OSB_R5, onOsbR5)
    ON_EVENT(OSB_R6, onOsbR6)
    ON_EVENT(OSB_R7, onOsbR7)
    ON_EVENT(OSB_R8, onOsbR8)
    ON_EVENT(OSB_R9, onOsbR9)
    ON_EVENT(OSB_R10,onOsbR10)

    ON_EVENT(OSB_B1, onOsbB1)
    ON_EVENT(OSB_B2, onOsbB2)
    ON_EVENT(OSB_B3, onOsbB3)
    ON_EVENT(OSB_B4, onOsbB4)
    ON_EVENT(OSB_B5, onOsbB5)
    ON_EVENT(OSB_B6, onOsbB6)
    ON_EVENT(OSB_B7, onOsbB7)
    ON_EVENT(OSB_B8, onOsbB8)
    ON_EVENT(OSB_B9, onOsbB9)
    ON_EVENT(OSB_B10,onOsbB10)

    ON_EVENT(OSB_L1, onOsbL1)
    ON_EVENT(OSB_L2, onOsbL2)
    ON_EVENT(OSB_L3, onOsbL3)
    ON_EVENT(OSB_L4, onOsbL4)
    ON_EVENT(OSB_L5, onOsbL5)
    ON_EVENT(OSB_L6, onOsbL6)
    ON_EVENT(OSB_L7, onOsbL7)
    ON_EVENT(OSB_L8, onOsbL8)
    ON_EVENT(OSB_L9, onOsbL9)
    ON_EVENT(OSB_L10,onOsbL10)
END_EVENT_HANDLER()


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
MfdPage::MfdPage()
{
    STANDARD_CONSTRUCTOR()
}  

//------------------------------------------------------------------------------
// Default OSB event handlers (just returns false -- not used)
//------------------------------------------------------------------------------
bool MfdPage::onOsbT1()    { return false; }
bool MfdPage::onOsbT2()    { return false; }
bool MfdPage::onOsbT3()    { return false; }
bool MfdPage::onOsbT4()    { return false; }
bool MfdPage::onOsbT5()    { return false; }
bool MfdPage::onOsbT6()    { return false; }
bool MfdPage::onOsbT7()    { return false; }
bool MfdPage::onOsbT8()    { return false; }
bool MfdPage::onOsbT9()    { return false; }
bool MfdPage::onOsbT10()   { return false; }

bool MfdPage::onOsbR1()    { return false; }
bool MfdPage::onOsbR2()    { return false; }
bool MfdPage::onOsbR3()    { return false; }
bool MfdPage::onOsbR4()    { return false; }
bool MfdPage::onOsbR5()    { return false; }
bool MfdPage::onOsbR6()    { return false; }
bool MfdPage::onOsbR7()    { return false; }
bool MfdPage::onOsbR8()    { return false; }
bool MfdPage::onOsbR9()    { return false; }
bool MfdPage::onOsbR10()   { return false; }

bool MfdPage::onOsbB1()    { return false; }
bool MfdPage::onOsbB2()    { return false; }
bool MfdPage::onOsbB3()    { return false; }
bool MfdPage::onOsbB4()    { return false; }
bool MfdPage::onOsbB5()    { return false; }
bool MfdPage::onOsbB6()    { return false; }
bool MfdPage::onOsbB7()    { return false; }
bool MfdPage::onOsbB8()    { return false; }
bool MfdPage::onOsbB9()    { return false; }
bool MfdPage::onOsbB10()   { return false; }

bool MfdPage::onOsbL1()    { return false; }
bool MfdPage::onOsbL2()    { return false; }
bool MfdPage::onOsbL3()    { return false; }
bool MfdPage::onOsbL4()    { return false; }
bool MfdPage::onOsbL5()    { return false; }
bool MfdPage::onOsbL6()    { return false; }
bool MfdPage::onOsbL7()    { return false; }
bool MfdPage::onOsbL8()    { return false; }
bool MfdPage::onOsbL9()    { return false; }
bool MfdPage::onOsbL10()   { return false; }

}
}
