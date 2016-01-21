//------------------------------------------------------------------------------
// Class: Instrument -> Adi -> GhostHorizon
//
// Description: Takes a pitch and roll in and determines when and where to draw the
// ghost horizon graphic
//------------------------------------------------------------------------------
#ifndef __oe_Instruments_GhostHorizon_H__
#define __oe_Instruments_GhostHorizon_H__

#include "openeaagles/instruments/adi/Adi.h"

namespace oe {
namespace Instruments {

class GhostHorizon : public Adi {
    DECLARE_SUBCLASS(GhostHorizon, Adi)
public:
    GhostHorizon();

    virtual bool setWidth(const LCreal x)           { width = x; return true; }
    virtual bool setHeight(const LCreal x)          { height = x; return true; }

    void drawFunc() override;

    void updateData(const LCreal dt = 0.0) override;

protected:
    bool setSlotSkyColor(const basic::Color* const cobj);
    bool setSlotSkyColor(const basic::String* const cname);
    bool setSlotGroundColor(const basic::String* const cname);
    bool setSlotGroundColor(const basic::Color* const cobj);
    bool setSlotWidth(const basic::Number* const x);
    bool setSlotHeight(const basic::Number* const x);

private:
    osg::Vec3 skyColor;     // color of our sky
    osg::Vec3 groundColor;  // color of our ground
    basic::String* gColorName;   // ground color name
    basic::String* sColorName;   // sky color name
    LCreal width;           // width of our polygon
    LCreal height;          // height of our polygon
};

}; // end Instruments namespace
}; // end oe namespace

#endif
