//------------------------------------------------------------------------------
// Class:  StrokeFont
//------------------------------------------------------------------------------
#ifndef __StrokeFont_H_6AB79747_BC92_4bb6_ACD7_9610EBBDCCB0__
#define __StrokeFont_H_6AB79747_BC92_4bb6_ACD7_9610EBBDCCB0__

#include "Font.h"

namespace Eaagles {
namespace BasicGL {

//------------------------------------------------------------------------------
// Class:  StrokeFont
// Base class:  Object -> Font -> StrokeFont
//
// Description:  (SGI's) Stroke font
// Form name: StrokeFont
//
//
// public methods (member functions):
//      (All of the classes derived from Font have the same public methods.)
//
//      static GLenum createStrokeFont(GLuint fontBase)
//          Produces the stroke font map.
//
//------------------------------------------------------------------------------
class StrokeFont : public Font {
    DECLARE_SUBCLASS(StrokeFont,Font)

public:
    StrokeFont();

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();

    static GLenum createStrokeFont(GLuint fontBase);
};


} // End BasicGL namespace
} // End Eaagles namespace

#endif  /* __StrokeFont_H_6AB79747_BC92_4bb6_ACD7_9610EBBDCCB0__ */
