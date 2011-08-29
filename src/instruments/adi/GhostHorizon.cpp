#include "openeaagles/instruments/adi/GhostHorizon.h"
#include "openeaagles/basic/Tables.h"
#include "openeaagles/basic/Rgba.h"
#include "openeaagles/basicGL/Display.h"

namespace Eaagles {
namespace Instruments {

IMPLEMENT_SUBCLASS(GhostHorizon, "GhostHorizon")
EMPTY_SERIALIZER(GhostHorizon)

// SLOTS
BEGIN_SLOTTABLE(GhostHorizon)
    "skyColor",     // 1) determine our sky color to draw
    "groundColor",  // 2) determine our ground color to draw
    "width",        // 3) width of the polygon to draw for the ground and sky
    "height",       // 4) height of the polyogn to draw for the ground and sky
END_SLOTTABLE(GhostHorizon)

BEGIN_SLOT_MAP(GhostHorizon)
    ON_SLOT(1, setSlotSkyColor, Basic::Color)
    ON_SLOT(1, setSlotSkyColor, Basic::String)
    ON_SLOT(2, setSlotGroundColor, Basic::String)
    ON_SLOT(2, setSlotGroundColor, Basic::Color)
    ON_SLOT(3, setSlotWidth, Basic::Number)
    ON_SLOT(4, setSlotHeight, Basic::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
GhostHorizon::GhostHorizon()
{
    STANDARD_CONSTRUCTOR()
    
    skyColor.set(1, 1, 1);
    groundColor.set(1, 1, 1);
    gColorName = 0;
    sColorName = 0;
    width = 20;
    height = 20;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void GhostHorizon::copyData(const GhostHorizon& org, const bool cc)
{
    BaseClass::copyData(org);
    
    if (cc) {
        gColorName = 0;
        sColorName = 0;
    }

    skyColor = org.skyColor;
    groundColor = org.groundColor;
    width = org.width;
    height = org.height;
    if (org.gColorName != 0) setSlotGroundColor(org.gColorName);
    if (org.sColorName != 0) setSlotSkyColor(org.sColorName);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void GhostHorizon::deleteData()
{
    if (gColorName != 0) gColorName->unref();
    gColorName = 0;
    if (sColorName != 0) sColorName->unref();
    sColorName = 0;
}

// Slot functions
//------------------------------------------------------------------------------
// setSlotSkyColor() - set the color of our Ghost Horizon "sky" by a Color obj.
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotSkyColor(const Basic::Color* const cobj)       
{
    bool ok = false;
    if (cobj != 0) {
        skyColor.set(cobj->red(), cobj->green(), cobj->blue());
        ok = true;
    }
    
    return ok;
}

//------------------------------------------------------------------------------
// setSlotSkyColor() - set the color of our Ghost Horizon "sky" by a string.
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotSkyColor(const Basic::String* const cname)       
{
    bool ok = false;
    if (cname != 0) {
        if (sColorName == 0) sColorName = new Basic::String(cname->getString());
        else sColorName->setStr(cname->getString());
        ok = true;
    }
    
    return ok;
}

//------------------------------------------------------------------------------
// setSlotGroundColor() - set our "ground" color by string
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotGroundColor(const Basic::String* const cname)       
{
    bool ok = false;
    if (cname != 0) {
        if (gColorName == 0) gColorName = new Basic::String(cname->getString());
        else gColorName->setStr(cname->getString());
        ok = true;
    }
    
    return ok;
}

//------------------------------------------------------------------------------
// setSlotGroundColor() - set our "ground" color by Color obj.
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotGroundColor(const Basic::Color* const cobj)       
{
    bool ok = false;
    if (cobj != 0) {
        groundColor.set(cobj->red(), cobj->green(), cobj->blue());
        ok = true;
    }
    
    return ok;
}

//------------------------------------------------------------------------------
// setSlotWidth() - set the width of our horizon
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotWidth(const Basic::Number* const x)       
{
    bool ok = false;
    if (x != 0) {
        ok = setWidth(x->getReal());        
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotHeight() - set the height of our horizon
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotHeight(const Basic::Number* const x)       
{
    bool ok = false;
    if (x != 0) {
        ok = setHeight(x->getReal());        
    }
    return ok;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void GhostHorizon::drawFunc()
{
    GLfloat ocolor[4];
    glGetFloatv(GL_CURRENT_COLOR, ocolor);        
  
    glPushMatrix();

    // if we are negative translation (pitch up), we show the ground background below us
    // ground
    // get our table to determine our min and max value
    LCreal maxPitch = 90;
    LCreal minPitch = -90;
    const Basic::Table1* table = getScalingTable();
    if (table != 0) {
        maxPitch = table->getMaxX();
        minPitch = table->getMinX();    
    }
    
    LCreal value = getPreScaleInstValue();
    
    if (value > maxPitch) {
            glColor3d(groundColor.x(), groundColor.y(), groundColor.z());
            glBegin(GL_POLYGON);
                lcVertex2(-width, -height);
                lcVertex2(-width, 0);
                lcVertex2( width, 0);
                lcVertex2( width, -height);
            glEnd();
        
    }
    // sky
    else if (value < minPitch) {
        glColor3d(skyColor.x(), skyColor.y(), skyColor.z());
        glBegin(GL_POLYGON);
            lcVertex2(-width, height);
            lcVertex2(-width, 0);
            lcVertex2(width, 0);
            lcVertex2(width, height);
        glEnd();
        
    } 
    glPopMatrix();
    
    glColor4fv(ocolor);
}

//------------------------------------------------------------------------------
// updateData() -- update non time-critical threads here
//------------------------------------------------------------------------------
void GhostHorizon::updateData(const LCreal dt)
{
    // Update our base class
    BaseClass::updateData(dt);
        
    // get our table to determine our min and max value
    LCreal maxPitch = 90;
    LCreal minPitch = -90;
    const Basic::Table1* table = getScalingTable();
    if (table != 0) {
        maxPitch = table->getMaxX();
        minPitch = table->getMinX();    
    }
    LCreal value = getPreScaleInstValue();

    if (value <= minPitch/* || value >= pitchLim*/) {
        setVisibility(true);
        // scale our location value
        //location = -(location - margin);
        if (sColorName != 0) {
            BasicGL::Display* d = getDisplay();
            if (d != 0) {
                Basic::Color* c = d->getColor(sColorName->getString());
                if (c != 0) {
                    skyColor.set(c->red(), c->green(), c->blue());
                }
            }
        }
    }
    else if (value >= maxPitch) {
        setVisibility(true);
        //location = -(location + margin);
        if (gColorName != 0) {
            BasicGL::Display* d = getDisplay();
            if (d != 0) {
                Basic::Color* c = d->getColor(gColorName->getString());
                if (c != 0) {
                    groundColor.set(c->red(), c->green(), c->blue());
                }
            }
        }
    }
    else setVisibility(false);
}

//------------------------------------------------------------------------------
// getSlotByIndex() for GhostHorizon
//------------------------------------------------------------------------------
Basic::Object* GhostHorizon::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}; // end of Instruments namespace
}; // end of Eaagles namespace