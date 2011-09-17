//------------------------------------------------------------------------------
// Form function for the 'basicGL' library
//------------------------------------------------------------------------------

#include "openeaagles/basicGL/basicGLFF.h"
#include "openeaagles/basicGL/Graphic.h"
#include "openeaagles/basicGL/Display.h"
#include "openeaagles/basicGL/Shapes.h"
#include "openeaagles/basicGL/Polygon.h"
#include "openeaagles/basicGL/ColorRotary.h"
#include "openeaagles/basicGL/ColorGradient.h"
#include "openeaagles/basicGL/Readouts.h"
#include "openeaagles/basicGL/FtglFonts.h"
#include "openeaagles/basicGL/BitmapFont.h"
#include "openeaagles/basicGL/StrokeFont.h"
#include "openeaagles/basicGL/BmpTexture.h"
#include "openeaagles/basicGL/Rotators.h"
#include "openeaagles/basicGL/Translator.h"
#include "openeaagles/basicGL/Material.h"  


// pages and maps
#include "openeaagles/basicGL/MfdPage.h"
#include "openeaagles/basicGL/MapPage.h"
#include "openeaagles/basicGL/SymbolLoader.h"

namespace Eaagles {
namespace BasicGL {

Basic::Object* basicGLFormFunc(const char* formname)
{
    // Start out by checking the basic classes (forms)
    Basic::Object* newform = 0;

    // General graphics support
    if ( strcmp(formname, Graphic::getFormName()) == 0 ) {
        newform = new Graphic();
    }
    else if ( strcmp(formname, Page::getFormName()) == 0 ) {
        newform = new Page();
    }
    else if ( strcmp(formname, Display::getFormName()) == 0 ) {
        newform = new Display();
    }
    else if ( strcmp(formname, Translator::getFormName()) == 0 ) {
        newform = new Translator();
    }
    else if ( strcmp(formname, Rotators::getFormName()) == 0 ) {
        newform = new Rotators();
    }
    else if ( strcmp(formname, ColorRotary::getFormName()) == 0 ) {
        newform = new ColorRotary();
    }
    else if ( strcmp(formname, ColorGradient::getFormName()) == 0 ) {
        newform = new ColorGradient();
    }

    // Shapes
    else if ( strcmp(formname, Circle::getFormName()) == 0 ) {
        newform = new Circle();
    }
    else if ( strcmp(formname, Point::getFormName()) == 0 ) {
        newform = new Point();
    }
    else if ( strcmp(formname, Polygon::getFormName()) == 0 ) {
        newform = new Polygon();
    }
    else if ( strcmp(formname, LineLoop::getFormName()) == 0 ) {
        newform = new LineLoop();
    }
    else if ( strcmp(formname, Line::getFormName()) == 0 ) {
        newform = new Line();
    }
    else if ( strcmp(formname, Arc::getFormName()) == 0 ) {
        newform = new Arc();
    }
    else if ( strcmp(formname, OcclusionCircle::getFormName()) == 0 ) {
        newform = new OcclusionCircle();
    }
    else if ( strcmp(formname, OcclusionArc::getFormName()) == 0 ) {
        newform = new OcclusionArc();
    }
    else if ( strcmp(formname, Quad::getFormName()) == 0 ) {
        newform = new Quad();
    }
    else if ( strcmp(formname, Triangle::getFormName()) == 0 ) {
        newform = new Triangle();
    }

    // Test Fields
    else if ( strcmp(formname, AsciiText::getFormName()) == 0 ) {
        newform = new AsciiText();
    }
    else if ( strcmp(formname, Cursor::getFormName()) == 0 ) {
        newform = new Cursor();
    }

    // Readouts
    else if ( strcmp(formname, NumericReadout::getFormName()) == 0 ) {
        newform = new NumericReadout();
    }
    else if ( strcmp(formname, HexReadout::getFormName()) == 0 ) {
        newform = new HexReadout();
    }
    else if ( strcmp(formname, OctalReadout::getFormName()) == 0 ) {
        newform = new OctalReadout();
    }
    else if ( strcmp(formname, TimeReadout::getFormName()) == 0 ) {
        newform = new TimeReadout();
    }
    else if ( strcmp(formname, DirectionReadout::getFormName()) == 0 ) {
        newform = new DirectionReadout();
    }
    else if ( strcmp(formname, LatitudeReadout::getFormName()) == 0 ) {
        newform = new LatitudeReadout();
    }
    else if ( strcmp(formname, LongitudeReadout::getFormName()) == 0 ) {
        newform = new LongitudeReadout();
    }
    else if ( strcmp(formname, Rotary::getFormName()) == 0 ) {
        newform = new Rotary();
    }
    else if ( strcmp(formname, Rotary2::getFormName()) == 0 ) {
        newform = new Rotary2();
    }

    // Stroke Font
    else if ( strcmp(formname, StrokeFont::getFormName()) == 0 ) {
        newform = new StrokeFont();
    }

    // Bitmap Font
    else if ( strcmp(formname, BitmapFont::getFormName()) == 0 ) {
        newform = new BitmapFont();
    }

    // FTGL Fonts
    else if ( strcmp(formname, FtglBitmapFont::getFormName()) == 0 ) {
        newform = new FtglBitmapFont();
    }
    else if ( strcmp(formname, FtglOutlineFont::getFormName()) == 0 ) {
        newform = new FtglOutlineFont();
    }
    else if ( strcmp(formname, FtglExtrdFont::getFormName()) == 0 ) {
        newform = new FtglExtrdFont();
    }
    else if ( strcmp(formname, FtglPixmapFont::getFormName()) == 0 ) {
        newform = new FtglPixmapFont();
    }
    else if ( strcmp(formname, FtglPolygonFont::getFormName()) == 0 ) {
        newform = new FtglPolygonFont();
    }
    else if ( strcmp(formname, FtglHaloFont::getFormName()) == 0 ) {
        newform = new FtglHaloFont();
    }
    else if ( strcmp(formname, FtglTextureFont::getFormName()) == 0 ) {
        newform = new FtglTextureFont();
    }

    // Bitmap Textures
    else if ( strcmp(formname, BmpTexture::getFormName()) == 0 ) {
        newform = new BmpTexture();
    }
    // Material
    else if ( strcmp(formname, Material::getFormName()) == 0 ) {
        newform = new Material();
    }
    // pages
    else if ( strcmp(formname, MfdPage::getFormName()) == 0 ) {
        newform = new MfdPage();
    }
    else if ( strcmp(formname, MapPage::getFormName()) == 0 ) {
        newform = new MapPage();
    }
    // Symbol loader
    else if ( strcmp(formname, SymbolLoader::getFormName()) == 0 ) {
        newform = new SymbolLoader();
    }

    return newform;
}

} // End BasicGL namespace
} // End Eaagles namespace
