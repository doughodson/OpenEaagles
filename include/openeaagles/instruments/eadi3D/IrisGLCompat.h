//------------------------------------------------------------------------------
// Class:  IrisGLCompat
//------------------------------------------------------------------------------
#ifndef __OPENEAAGLES_INSTRUMENTS_IRISGLCOMPAT_H__
#define __OPENEAAGLES_INSTRUMENTS_IRISGLCOMPAT_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Instruments {

//------------------------------------------------------------------------------
// Class:  IrisGLCompat
// Description:  IrisGL compatability
//
//------------------------------------------------------------------------------
class IrisGLCompat : public Basic::Object {
    DECLARE_SUBCLASS(IrisGLCompat,Basic::Object)
public:
    IrisGLCompat();

    // functions for quick drawings.
    void arc(float x, float y, float r, float startAng, float endAng);
    void arcf(float x, float y, float r, float startAng, float endAng);
    void circ(float x, float y, float r);
    void circf(float x, float y, float r);
    void washerf(float x, float y, float r1, float r2);
    void rect(float x1, float y1, float x2, float y2);

    void rpmv2(float x, float y);
    void rpdr2(float x, float y);
    void rmv2(float x, float y);
    void rdr2(float x, float y);

private:
    float vertexX;
    float vertexY;
};

} // End Instruments namespace
} // End Eaagles namespace

#endif // __OPENEAAGLES_INSTRUMENTS_IRISGLCOMPAT_H__
