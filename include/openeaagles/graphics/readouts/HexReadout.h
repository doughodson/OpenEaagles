
#ifndef __oe_graphics_HexReadout_H__
#define __oe_graphics_HexReadout_H__

#include "NumericReadout.h"

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Class: HexReadout
//
// Factory name: HexReadout
//
// Example formats:
//
//    ######    // Hex number
//    00000#    // Hex number w/leading zeros
//
//------------------------------------------------------------------------------
class HexReadout : public NumericReadout
{
   DECLARE_SUBCLASS(HexReadout, NumericReadout)

public:
   HexReadout();

   virtual void makeText() override;
   virtual char filterInputEvent(const int event, const int tc) override;
   virtual double getInputValue() const override;

protected:
   virtual void reformat(const char* const example) override;
};

}
}

#endif
