//------------------------------------------------------------------------------
// Class: EngPage
// Base class: ... -> graphics::Page -> EngPage
//
// Description: Tests the secondary pfd page
//------------------------------------------------------------------------------
#ifndef __oe_instruments_EngPage_H__
#define __oe_instruments_EngPage_H__

#include "openeaagles/graphics/Page.h"

namespace oe {
namespace instruments {

class EngPage : public graphics::Page
{
   DECLARE_SUBCLASS(EngPage,graphics::Page)

public:
    EngPage();

    static const int NUM_ENG = 4;

    // set functions: engine index range [ 1 .. NUM_ENG ]
    virtual bool setEngN1(const int idx, const LCreal newN1);       // %RPM
    virtual bool setEngN2(const int idx, const LCreal newN2);       // %RPM
    virtual bool setEngTit(const int idx, const LCreal newT);       // Degs C
    virtual bool setEngFF(const int idx, const LCreal newFF);       // lbs/hrs

    // get functions
    LCreal getEngN1(const int engNum) const { return n1[engNum]; }
    LCreal getEngN2(const int engNum) const { return n2[engNum]; }
    LCreal getEngTit(const int engNum) const { return tit[engNum]; }
    LCreal getEngFF(const int engNum) const { return ff[engNum]; }

    void updateData(const LCreal dt = 0.0) override;

private:

    // engine n1
    LCreal   n1[NUM_ENG];          // %RPM
    SendData n1SD[NUM_ENG];
    SendData n1ROSD[NUM_ENG];
    SendData n1BoxSD[NUM_ENG];

    // engine n2
    LCreal   n2[NUM_ENG];          // %RPM
    SendData n2SD[NUM_ENG];
    SendData n2ROSD[NUM_ENG];
    SendData n2BoxSD[NUM_ENG];

    // engine tit
    LCreal   tit[NUM_ENG];         // Degs C
    SendData titSD[NUM_ENG];
    SendData titROSD[NUM_ENG];
    SendData titBoxSD[NUM_ENG];

    // engin1 fuel flow
    LCreal ff[NUM_ENG];             // lbs/hrs
    SendData ffSD[NUM_ENG];
    SendData ffROSD[NUM_ENG];
};

}  // end instruments namespace
}  // end oe namespace

#endif
