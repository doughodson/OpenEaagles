//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/basicGL/Factory.h"

#include "openeaagles/basic/Object.h"

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

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = 0;

    // General graphics support
    if ( strcmp(name, Graphic::getFactoryName()) == 0 ) {
        obj = new Graphic();
    }
    else if ( strcmp(name, Page::getFactoryName()) == 0 ) {
        obj = new Page();
    }
    else if ( strcmp(name, Display::getFactoryName()) == 0 ) {
        obj = new Display();
    }
    else if ( strcmp(name, Translator::getFactoryName()) == 0 ) {
        obj = new Translator();
    }
    else if ( strcmp(name, Rotators::getFactoryName()) == 0 ) {
        obj = new Rotators();
    }
    else if ( strcmp(name, ColorRotary::getFactoryName()) == 0 ) {
        obj = new ColorRotary();
    }
    else if ( strcmp(name, ColorGradient::getFactoryName()) == 0 ) {
        obj = new ColorGradient();
    }

    // Shapes
    else if ( strcmp(name, Circle::getFactoryName()) == 0 ) {
        obj = new Circle();
    }
    else if ( strcmp(name, Point::getFactoryName()) == 0 ) {
        obj = new Point();
    }
    else if ( strcmp(name, Polygon::getFactoryName()) == 0 ) {
        obj = new Polygon();
    }
    else if ( strcmp(name, LineLoop::getFactoryName()) == 0 ) {
        obj = new LineLoop();
    }
    else if ( strcmp(name, Line::getFactoryName()) == 0 ) {
        obj = new Line();
    }
    else if ( strcmp(name, Arc::getFactoryName()) == 0 ) {
        obj = new Arc();
    }
    else if ( strcmp(name, OcclusionCircle::getFactoryName()) == 0 ) {
        obj = new OcclusionCircle();
    }
    else if ( strcmp(name, OcclusionArc::getFactoryName()) == 0 ) {
        obj = new OcclusionArc();
    }
    else if ( strcmp(name, Quad::getFactoryName()) == 0 ) {
        obj = new Quad();
    }
    else if ( strcmp(name, Triangle::getFactoryName()) == 0 ) {
        obj = new Triangle();
    }

    // Test Fields
    else if ( strcmp(name, AsciiText::getFactoryName()) == 0 ) {
        obj = new AsciiText();
    }
    else if ( strcmp(name, Cursor::getFactoryName()) == 0 ) {
        obj = new Cursor();
    }

    // Readouts
    else if ( strcmp(name, NumericReadout::getFactoryName()) == 0 ) {
        obj = new NumericReadout();
    }
    else if ( strcmp(name, HexReadout::getFactoryName()) == 0 ) {
        obj = new HexReadout();
    }
    else if ( strcmp(name, OctalReadout::getFactoryName()) == 0 ) {
        obj = new OctalReadout();
    }
    else if ( strcmp(name, TimeReadout::getFactoryName()) == 0 ) {
        obj = new TimeReadout();
    }
    else if ( strcmp(name, DirectionReadout::getFactoryName()) == 0 ) {
        obj = new DirectionReadout();
    }
    else if ( strcmp(name, LatitudeReadout::getFactoryName()) == 0 ) {
        obj = new LatitudeReadout();
    }
    else if ( strcmp(name, LongitudeReadout::getFactoryName()) == 0 ) {
        obj = new LongitudeReadout();
    }
    else if ( strcmp(name, Rotary::getFactoryName()) == 0 ) {
        obj = new Rotary();
    }
    else if ( strcmp(name, Rotary2::getFactoryName()) == 0 ) {
        obj = new Rotary2();
    }

    // Stroke Font
    else if ( strcmp(name, StrokeFont::getFactoryName()) == 0 ) {
        obj = new StrokeFont();
    }

    // Bitmap Font
    else if ( strcmp(name, BitmapFont::getFactoryName()) == 0 ) {
        obj = new BitmapFont();
    }

    // FTGL Fonts
    else if ( strcmp(name, FtglBitmapFont::getFactoryName()) == 0 ) {
        obj = new FtglBitmapFont();
    }
    else if ( strcmp(name, FtglOutlineFont::getFactoryName()) == 0 ) {
        obj = new FtglOutlineFont();
    }
    else if ( strcmp(name, FtglExtrdFont::getFactoryName()) == 0 ) {
        obj = new FtglExtrdFont();
    }
    else if ( strcmp(name, FtglPixmapFont::getFactoryName()) == 0 ) {
        obj = new FtglPixmapFont();
    }
    else if ( strcmp(name, FtglPolygonFont::getFactoryName()) == 0 ) {
        obj = new FtglPolygonFont();
    }
    else if ( strcmp(name, FtglHaloFont::getFactoryName()) == 0 ) {
        obj = new FtglHaloFont();
    }
    else if ( strcmp(name, FtglTextureFont::getFactoryName()) == 0 ) {
        obj = new FtglTextureFont();
    }

    // Bitmap Textures
    else if ( strcmp(name, BmpTexture::getFactoryName()) == 0 ) {
        obj = new BmpTexture();
    }
    // Material
    else if ( strcmp(name, Material::getFactoryName()) == 0 ) {
        obj = new Material();
    }
    // pages
    else if ( strcmp(name, MfdPage::getFactoryName()) == 0 ) {
        obj = new MfdPage();
    }
    else if ( strcmp(name, MapPage::getFactoryName()) == 0 ) {
        obj = new MapPage();
    }
    // Symbol loader
    else if ( strcmp(name, SymbolLoader::getFactoryName()) == 0 ) {
        obj = new SymbolLoader();
    }

    return obj;
}

}  // end namespace BasicGL
}  // end namespace Eaagles

