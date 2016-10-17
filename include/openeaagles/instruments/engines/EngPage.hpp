//------------------------------------------------------------------------------
// Class: EngPage
// Base class: ... -> graphics::Page -> EngPage
//
// Description: Tests the secondary pfd page
//------------------------------------------------------------------------------
#ifndef __oe_instruments_EngPage_H__
#define __oe_instruments_EngPage_H__

#include "openeaagles/graphics/Page.hpp"

namespace oe {
namespace instruments {

class EngPage : public graphics::Page
{
   DECLARE_SUBCLASS(EngPage,graphics::Page)

public:
    EngPage();

    static const int NUM_ENG = 4;

    // set functions: engine index range [ 1 .. NUM_ENG ]
    virtual bool setEngN1(const int idx, const double newN1);       // %RPM
    virtual bool setEngN2(const int idx, const double newN2);       // %RPM
    virtual bool setEngTit(const int idx, const double newT);       // Degs C
    virtual bool setEngFF(const int idx, const double newFF);       // lbs/hrs

    // get functions
    double getEngN1(const int engNum) const { return n1[engNum]; }
    double getEngN2(const int engNum) const { return n2[engNum]; }
    double getEngTit(const int engNum) const { return tit[engNum]; }
    double getEngFF(const int engNum) const { return ff[engNum]; }

    virtual void updateData(const double dt = 0.0) override;

private:

    // engine n1
    double   n1[NUM_ENG];          // %RPM
    SendData n1SD[NUM_ENG];
    SendData n1ROSD[NUM_ENG];
    SendData n1BoxSD[NUM_ENG];

    // engine n2
    double   n2[NUM_ENG];          // %RPM
    SendData n2SD[NUM_ENG];
    SendData n2ROSD[NUM_ENG];
    SendData n2BoxSD[NUM_ENG];

    // engine tit
    double   tit[NUM_ENG];         // Degs C
    SendData titSD[NUM_ENG];
    SendData titROSD[NUM_ENG];
    SendData titBoxSD[NUM_ENG];

    // engin1 fuel flow
    double ff[NUM_ENG];             // lbs/hrs
    SendData ffSD[NUM_ENG];
    SendData ffROSD[NUM_ENG];
};

}
}

#endif
