//------------------------------------------------------------------------------
// Class: Instrument -> Adi -> GhostHorizon
//
// Description: Takes a pitch and roll in and determines when and where to draw the
// ghost horizon graphic
//------------------------------------------------------------------------------
#ifndef	__Eaagles_Instruments_GhostHorizon_H__
#define __Eaagles_Instruments_GhostHorizon_H__

#include "openeaagles/instruments/adi/Adi.h"

namespace Eaagles {
namespace Instruments {

class GhostHorizon : public Adi {
    DECLARE_SUBCLASS(GhostHorizon, Adi)
public:
    GhostHorizon();

    // Set functions
    virtual bool setWidth(const LCreal x)           { width = x; return true; }
    virtual bool setHeight(const LCreal x)          { height = x; return true; }

    // BasicGL::Graphic interface
    virtual void drawFunc();
    
    // Basic::Component interface
    virtual void updateData(const LCreal dt = 0.0f);

protected:
    bool setSlotSkyColor(const Basic::Color* const cobj);   
    bool setSlotSkyColor(const Basic::String* const cname);
    bool setSlotGroundColor(const Basic::String* const cname);
    bool setSlotGroundColor(const Basic::Color* const cobj);
    bool setSlotWidth(const Basic::Number* const x);
    bool setSlotHeight(const Basic::Number* const x);

private:
    osg::Vec3 skyColor;     // color of our sky
    osg::Vec3 groundColor;  // color of our ground
    Basic::String* gColorName;   // ground color name
    Basic::String* sColorName;   // sky color name
    LCreal width;           // width of our polygon
    LCreal height;          // height of our polygon
};

}; // end Instruments namespace
}; // end Eaagles namespace

#endif
