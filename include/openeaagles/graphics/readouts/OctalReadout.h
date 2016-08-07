
#ifndef __oe_graphics_OctalReadout_H__
#define __oe_graphics_OctalReadout_H__

#include "NumericReadout.h"

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Class: OctalReadout
//
// Factory name: OctalReadout
//
// Example formats:
//
//    ######    // Octal number
//    00000#    // Octal number w/leading zeros
//
//------------------------------------------------------------------------------
class OctalReadout : public NumericReadout
{
   DECLARE_SUBCLASS(OctalReadout, NumericReadout)

public:
   OctalReadout();
   virtual void makeText() override;
   virtual char filterInputEvent(const int event, const int tc) override;
   virtual double getInputValue() const override;

protected:
   virtual void reformat(const char* const example) override;
};

}
}

#endif
