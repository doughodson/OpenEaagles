//------------------------------------------------------------------------------
// Classes: FlowRate
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_FlowRate_H__
#define __Eaagles_Basic_FlowRate_H__

#include "../Number.h"
#include "Volumes.h"
#include "Times.h"

namespace Eaagles {
namespace Basic {
//------------------------------------------------------------------------------
// Classes: FlowRate
// Base class:  Object -> Number -> FlowRate
//
// Description:  Flow Rate as a component of volume and time.
// Form name: FlowRate
// Slots:
//    volume     <Volume>   ! Volume (default: 1)
//    flowTime   <Time>     ! Time (default: 1)
//
// getSlotByIndex() for FlowRate
//     Object* FlowRate::getSlotByIndex(const int si)
//
// set() -- sets our velocity from some other velocity 
//     bool FlowRate::set(const LCreal newFlowRate, Volume* newVolume, Time* newTime)
//
// setSlotVolume() -- sets our volume object.
//     bool FlowRate::setSlotVolume(Volume* newVol)
//
// setSlotTime() -- sets our time object.
//     bool FlowRate::setSlotTime(Time* newTime)
//
// set() -- sets our velocity from some other velocity 
//     bool FlowRate::set(const LCreal newFlowRate, Volume* newVolume, Time* newTime)
//
// convert() -- converts from one volume/time ratio to another
//     LCreal FlowRate::convert(Volume* newVolume, Time* newTime)
//
// serialize() -- print the value of this object to the output stream sout.
//     std::ostream& FlowRate::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
//
//------------------------------------------------------------------------------
class FlowRate : public Number
{
    DECLARE_SUBCLASS(FlowRate, Number)
    
public:
    FlowRate(LCreal newFlowRate, Volume* volume, Time* time);
    FlowRate();

    // dynamic convert routine
    LCreal convert(Volume* newVolume, Time* newTime);
    // get functions
    LCreal getFlowRate() { return flowRate; };
    // set functions
    bool set(const LCreal newFlowRate, Volume* newVolume, Time* newTime);
    // slot table functions
    bool setSlotVolume(Volume* newVol);
    bool setSlotTime(Time* newTime);          
    
    private:
    Volume*   myVolume;       // holds our distance type
    Time*     myTime;         // holds our time type
    LCreal    flowRate;       // holds our velocity
};

} // End Basic namespace
} // End Eaagles namespace

#endif
