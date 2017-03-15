
#ifndef __oe_instruments_EngPage_H__
#define __oe_instruments_EngPage_H__

#include "openeaagles/graphics/Page.hpp"
#include <array>

namespace oe {
namespace instruments {

//------------------------------------------------------------------------------
// Class: EngPage
//
// Description: Tests the secondary pfd page
//------------------------------------------------------------------------------
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
    std::array<double, NUM_ENG> n1 {};          // %RPM
    std::array<SendData, NUM_ENG> n1SD;
    std::array<SendData, NUM_ENG> n1ROSD;
    std::array<SendData, NUM_ENG> n1BoxSD;

    // engine n2
    std::array<double, NUM_ENG> n2 {};          // %RPM
    std::array<SendData, NUM_ENG> n2SD;
    std::array<SendData, NUM_ENG> n2ROSD;
    std::array<SendData, NUM_ENG> n2BoxSD;

    // engine tit
    std::array<double, NUM_ENG> tit {};        // Degs C
    std::array<SendData, NUM_ENG> titSD;
    std::array<SendData, NUM_ENG> titROSD;
    std::array<SendData, NUM_ENG> titBoxSD;

    // engin1 fuel flow
    std::array<double, NUM_ENG> ff;            // lbs/hrs
    std::array<SendData, NUM_ENG> ffSD;
    std::array<SendData, NUM_ENG> ffROSD;
};

}
}

#endif
