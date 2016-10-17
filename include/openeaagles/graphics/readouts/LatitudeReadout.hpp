
#ifndef __oe_graphics_LatitudeReadout_H__
#define __oe_graphics_LatitudeReadout_H__

#include "DirectionReadout.hpp"

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Class: LatitudeReadout
//
// Factory name: LatitudeReadout
//
// Notes:
//    1) see the 'DirectionReadout' notes
//    2) The '+ char is replaced with 'N', and the '-' char is replaced with 'S'
//
// Example formats:
//
//    0D.D@          // Degrees
//
//    0D@MM          // Degrees and minutes
//    0D@MM.MM       // Degrees and minutes
//
//    0D@MM'SS       // Degrees, minutes and seconds
//    0D@MM'SS.SSS   // Degrees, minutes and seconds
//
//    +0D@MM'SS.S    // Degrees, minutes and seconds with '+' on positive values
//    0D@MM'SS.S+    //  ... and with the '+' or '-' character as a suffix
//
//------------------------------------------------------------------------------
class LatitudeReadout : public DirectionReadout
{
    DECLARE_SUBCLASS(LatitudeReadout, DirectionReadout)

public:
    LatitudeReadout();
    virtual char filterInputEvent(const int event, const int tc) override;
protected:
   //virtual void makeText();
};

}
}

#endif
