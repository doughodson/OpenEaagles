
#ifndef __oe_graphics_DirectionReadout_H__
#define __oe_graphics_DirectionReadout_H__

#include "NumericReadout.hpp"

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Class: DirectionReadout
//
// Factory name: DirectionReadout
//
// Notes:
//    1) The 'D' character is used to define the degrees field (required)
//
//    2) The 'M' character is used to define the minutes field (optional, but
//    required when seconds are displayed)
//
//    3) The 'S' character is used to define the seconds field (optional)
//
//    4) The "at symbol" (@) is replaced with a degree symbol (ASCII 0xB0)
//
//    5) The prefix or suffix '+' and '-' characters can be replaced using the
//    'plusChar' and 'minusChar' slots (e.g., to change the '+' to 'E' and
//    the '-' to 'W')
//
// Example formats
//
//    DD.D@          // Degrees
//    0DD.D@         // Degrees with leading zeros
//    +DD.D@         // Degrees with '+' on positive values
//    DD.D@+         // Degrees with the '+' or '-' character as a suffix
//
//    DD@MM          // Degrees and minutes
//    DD@MM.M        // Degrees and minutes
//
//    DD@MM'SS       // Degrees, minutes and seconds
//    DD@MM'SS.S     // Degrees, minutes and seconds
//
//    +0DD@MM'SS.S   // Degrees, minutes and seconds with '+' on positive values
//    0DD@MM'SS.S+   //  ... and with the '+' or '-' character as a suffix
//
//------------------------------------------------------------------------------
class DirectionReadout : public NumericReadout
{
   DECLARE_SUBCLASS(DirectionReadout, NumericReadout)

public:
   enum DirMode { invalid, ddmmss, ddmm, dd };
   DirectionReadout();
   virtual char filterInputEvent(const int event, const int tc) override;
   virtual double getInputValue() const override;

protected:
   virtual void makeText() override;
   virtual void reformat(const char* const example) override;
   DirMode tmode {dd};
};

}
}

#endif
