#include "openeaagles/instruments/maps/CompassRose.h"

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(CompassRose, "CompassRose")
EMPTY_SERIALIZER(CompassRose)

// Event handler
BEGIN_EVENT_HANDLER(CompassRose)
    ON_EVENT_OBJ(UPDATE_VALUE, onUpdateRotDeg, base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE2, onUpdateRadius, base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE3, onUpdateCenRad, base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE4, onUpdateDecRadius, base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE5, onUpdateDisplacement, base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE6, onUpdateCentered, base::Number)
END_EVENT_HANDLER()

BEGIN_SLOTTABLE(CompassRose)
    "centeredRad",      // our centered radius
    "decenteredRad",    // decentered radius
    "displacement",     // how far to translate
END_SLOTTABLE(CompassRose)       

//------------------------------------------------------------------------------
//  Map slot table to handles for Analog Dial
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(CompassRose)
    ON_SLOT(1, setSlotCenteredRadius, base::Number)
    ON_SLOT(2, setSlotDeCenteredRadius, base::Number)
    ON_SLOT(3, setSlotDisplacement, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
CompassRose::CompassRose()
{
    STANDARD_CONSTRUCTOR()
    centered = true;
    cenRadius = 0.5;
    decRadius = 1.0;
    displacement = -0.5;
    sTicksSD.empty();
    lTicksSD.empty();
    dialSD.empty();
    rot = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void CompassRose::copyData(const CompassRose& org, const bool)
{
    // copy our base class stuff first
    BaseClass::copyData(org);
    
    centered = org.centered;
    cenRadius = org.cenRadius;
    decRadius = org.decRadius;
    displacement = org.displacement;
    sTicksSD.empty();
    lTicksSD.empty();
    dialSD.empty();
    rot = org.rot;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(CompassRose)

// Slot functions
//------------------------------------------------------------------------------
// setSlotCenteredRadius() - sets our radius
//------------------------------------------------------------------------------
bool CompassRose::setSlotCenteredRadius(const base::Number* const newR)
{
    bool ok = false;
    if (newR != nullptr) ok = setCenteredRadius(newR->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotDeCenteredRadius() - sets our decentered radius
//------------------------------------------------------------------------------
bool CompassRose::setSlotDeCenteredRadius(const base::Number* const newR)
{
    bool ok = false;
    if (newR != nullptr) ok = setDeCenteredRadius(newR->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotDisplacement() - how far to displace when we decenter
//------------------------------------------------------------------------------
bool CompassRose::setSlotDisplacement(const base::Number* const newD)
{
    bool ok = false;
    if (newD != nullptr) ok = setDisplacement(newD->getReal());
    return ok;
}

// Set functions
//------------------------------------------------------------------------------
// setRotationDeg() - 
//------------------------------------------------------------------------------
bool CompassRose::setRotationDeg(const double newR)
{
    rot = newR * static_cast<double>(base::Angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setRotationRad() - 
//------------------------------------------------------------------------------
bool CompassRose::setRotationRad(const double newR)
{
    rot = newR;
    return true;
}

//------------------------------------------------------------------------------
// setCenteredRadius() - 
//------------------------------------------------------------------------------
bool CompassRose::setCenteredRadius(const double newR)
{
    cenRadius = newR;
    return true;
}

//------------------------------------------------------------------------------
// seDeCenteredRadius() - 
//------------------------------------------------------------------------------
bool CompassRose::setDeCenteredRadius(const double newDR)
{
    decRadius = newDR;
    return true; 
}

//------------------------------------------------------------------------------
// setDisplacement() - 
//------------------------------------------------------------------------------
bool CompassRose::setDisplacement(const double newD)
{
    displacement = newD;
    return true; 
}

//------------------------------------------------------------------------------
// setCentered() - 
//------------------------------------------------------------------------------
bool CompassRose::setCentered(const bool newC)
{
    centered = newC;
    return true;
}

// event functions
//------------------------------------------------------------------------------
// onUpdateRotDeg() - update rotation (degrees)
//------------------------------------------------------------------------------
bool CompassRose::onUpdateRotDeg(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setRotationDeg(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateRadius() - change our radius
//------------------------------------------------------------------------------
bool CompassRose::onUpdateRadius(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setRotationRad(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateCenRad() - change our centered radius 
//------------------------------------------------------------------------------
bool CompassRose::onUpdateCenRad(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setCenteredRadius(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateDecRadius() - change our decentered radius
//------------------------------------------------------------------------------
bool CompassRose::onUpdateDecRadius(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setDeCenteredRadius(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateDisplacement() - change our displacement
//------------------------------------------------------------------------------
bool CompassRose::onUpdateDisplacement(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setDisplacement(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateCentered() - change our centered status
//------------------------------------------------------------------------------
bool CompassRose::onUpdateCentered(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setCentered(x->getBoolean());
    return ok;
}


//------------------------------------------------------------------------------
// draw() -- draws the object(s) using the graphic matrix
//------------------------------------------------------------------------------
void CompassRose::draw()
{
    lcSaveMatrix();
        if (!centered) lcTranslate(0, displacement);    
        lcRotate(rot);
        graphics::Graphic::draw();
    lcRestoreMatrix();
}

//------------------------------------------------------------------------------
// updateData() - update our non time-critical stuff here
//------------------------------------------------------------------------------
void CompassRose::updateData(const double dt)
{
    // update baseclass stuff first
    BaseClass::updateData(dt);

    double rad = 0;
    if (centered) rad = cenRadius;
    else rad = decRadius;

    // send our tick marks and dial arc segments the proper data
    send("longticks", UPDATE_VALUE, rad, lTicksSD);
    send("shortticks", UPDATE_VALUE, rad, sTicksSD);
    send("dial", UPDATE_VALUE, rad, dialSD);
}

//------------------------------------------------------------------------------
// getSlotByIndex() for CompassRose
//------------------------------------------------------------------------------
base::Object* CompassRose::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end instruments namespace
}  // end oe namespace
