//------------------------------------------------------------------------------
// Class: Eadi3DFont
//------------------------------------------------------------------------------
#ifndef __oe_instruments_Eadi3DFont_H__
#define __oe_instruments_Eadi3DFont_H__

#include "openeaagles/graphics/Font.h"

namespace oe {
namespace instruments {

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
class Eadi3DFont : public graphics::Font
{
    DECLARE_SUBCLASS(Eadi3DFont,graphics::Font)

public:
    Eadi3DFont();

    static GLenum createEadi3DFont(GLuint fontBase);

    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    virtual void loadFont() override;
};

}
}

#endif
