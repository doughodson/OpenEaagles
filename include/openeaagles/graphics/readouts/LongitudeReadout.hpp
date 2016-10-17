
#ifndef __oe_graphics_LongitudeReadout_H__
#define __oe_graphics_LongitudeReadout_H__

#include "DirectionReadout.hpp"

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Class: LongitudeReadout
// Base class:  Object > Graphic > Field > NumericReadout > DirectionReadout > LongitudeReadout
//
// Factory name: LongitudeReadout
//
// Notes:
//    1) see the 'DirectionReadout' notes
//    2) The '+ char is replaced with 'E', and the '-' char is replaced with 'W'
//
// Example formats:
//
//    0DD@            // Degrees
//    0DD.D@          // Degrees
//
//    0DD@MM          // Degrees and minutes
//    0DD@MM.MM       // Degrees and minutes
//
//    0DD@MM'SS       // Degrees, minutes and seconds
//    0DD@MM'SS.SSS   // Degrees, minutes and seconds
//
//    +0DD@MM'SS.S    // Degrees, minutes and seconds with '+' on positive values
//    0DD@MM'SS.S+    //  ... and with the '+' or '-' character as a suffix
//
//------------------------------------------------------------------------------
class LongitudeReadout : public DirectionReadout
{
    DECLARE_SUBCLASS(LongitudeReadout, DirectionReadout)
public:
    LongitudeReadout();
    virtual char filterInputEvent(const int event, const int tc) override;
protected:
   //virtual void makeText();
};

}
}

#endif
