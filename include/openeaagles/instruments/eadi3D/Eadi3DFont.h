//------------------------------------------------------------------------------
// Class: Eadi3DFont
//------------------------------------------------------------------------------
#ifndef __oe_Instruments_Eadi3DFont_H__
#define __oe_Instruments_Eadi3DFont_H__

#include "openeaagles/basicGL/Font.h"

namespace oe {
namespace Instruments {

//------------------------------------------------------------------------------
// Class:  Eadi3DFont
//
// Description:  Eadi3D font
//
// public methods (member functions):
//      (All of the classes derived from Font have the same public methods.)
//
//      static GLenum createEadi3DFont(GLuint fontBase)
//          Produces the stroke font map.
//
//------------------------------------------------------------------------------
class Eadi3DFont : public BasicGL::Font
{
    DECLARE_SUBCLASS(Eadi3DFont,BasicGL::Font)

public:
    Eadi3DFont();

    static GLenum createEadi3DFont(GLuint fontBase);

    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void loadFont() override;
};


} // End Instruments namespace
} // End oe namespace

#endif
