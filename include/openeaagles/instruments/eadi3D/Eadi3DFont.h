//------------------------------------------------------------------------------
// Class:  Eadi3DFont
//------------------------------------------------------------------------------
#ifndef	__OPENEAAGLES_INSTRUMENTS_EADI3DFONT_H__
#define __OPENEAAGLES_INSTRUMENTS_EADI3DFONT_H__

#include "openeaagles/basicGL/Font.h"

namespace Eaagles {
namespace Instruments {

//------------------------------------------------------------------------------
// Class:  Eadi3DFont
//
// Description:  Eadi3D font
//
//
// public methods (member functions):
//      (All of the classes derived from Font have the same public methods.)
//
//      static GLenum createEadi3DFont(GLuint fontBase)
//          Produces the stroke font map.
//
//------------------------------------------------------------------------------
class Eadi3DFont : public BasicGL::Font {
    DECLARE_SUBCLASS(Eadi3DFont,BasicGL::Font)

public:
    Eadi3DFont();

    static GLenum createEadi3DFont(GLuint fontBase);

    // Basic::Font interface
    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false);
    virtual void loadFont();
};


} // End Instruments namespace
} // End Eaagles namespace

#endif	// __OPENEAAGLES_INSTRUMENTS_EADI3DFONT_H__
