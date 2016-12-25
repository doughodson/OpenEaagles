
#ifndef __oe_graphics_MfdPage_H__
#define __oe_graphics_MfdPage_H__

#include "openeaagles/graphics/Page.hpp"

namespace oe {
namespace graphics {

// -------------------------------------------------------------------------------
// Class: MfdPage
//
// Description: Base class page for any derived page that wishes to have OSB hits
// automatically recognized and used.  This sets up 40 OSB hits, similar to a 10 x
// 10 bezel display.  Then any event that is passed with the OSB hits will call
// the respective function, and derived classes may override the function and
// do specific actions.
//
// Factory name: MfdPage
// -------------------------------------------------------------------------------
class MfdPage : public graphics::Page
{
    DECLARE_SUBCLASS(MfdPage, graphics::Page)

public:
    MfdPage();

    virtual bool event(const int event, base::Object* const obj = nullptr) override;

protected:
    // ---
    // MFD button event functions (see "openeaagles/base/eventTokens.h")
    //  -- These are virtual and do not get a unique name because
    //     they may need to be overridden by subclass pages.
    //  -- Supports up to a 10x10 MFD
    // ---
    virtual bool onOsbT1();
    virtual bool onOsbT2();
    virtual bool onOsbT3();
    virtual bool onOsbT4();
    virtual bool onOsbT5();
    virtual bool onOsbT6();
    virtual bool onOsbT7();
    virtual bool onOsbT8();
    virtual bool onOsbT9();
    virtual bool onOsbT10();
    virtual bool onOsbR1();
    virtual bool onOsbR2();
    virtual bool onOsbR3();
    virtual bool onOsbR4();
    virtual bool onOsbR5();
    virtual bool onOsbR6();
    virtual bool onOsbR7();
    virtual bool onOsbR8();
    virtual bool onOsbR9();
    virtual bool onOsbR10();
    virtual bool onOsbB1();
    virtual bool onOsbB2();
    virtual bool onOsbB3();
    virtual bool onOsbB4();
    virtual bool onOsbB5();
    virtual bool onOsbB6();
    virtual bool onOsbB7();
    virtual bool onOsbB8();
    virtual bool onOsbB9();
    virtual bool onOsbB10();
    virtual bool onOsbL1();
    virtual bool onOsbL2();
    virtual bool onOsbL3();
    virtual bool onOsbL4();
    virtual bool onOsbL5();
    virtual bool onOsbL6();
    virtual bool onOsbL7();
    virtual bool onOsbL8();
    virtual bool onOsbL9();
    virtual bool onOsbL10();
};

}
}

#endif
