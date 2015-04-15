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

#include <cstring>

namespace Eaagles {
namespace BasicGL {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = nullptr;

    // General graphics support
    if ( std::strcmp(name, Graphic::getFactoryName()) == 0 ) {
        obj = new Graphic();
    }
    else if ( std::strcmp(name, Page::getFactoryName()) == 0 ) {
        obj = new Page();
    }
    else if ( std::strcmp(name, Display::getFactoryName()) == 0 ) {
        obj = new Display();
    }
    else if ( std::strcmp(name, Translator::getFactoryName()) == 0 ) {
        obj = new Translator();
    }
    else if ( std::strcmp(name, Rotators::getFactoryName()) == 0 ) {
        obj = new Rotators();
    }
    else if ( std::strcmp(name, ColorRotary::getFactoryName()) == 0 ) {
        obj = new ColorRotary();
    }
    else if ( std::strcmp(name, ColorGradient::getFactoryName()) == 0 ) {
        obj = new ColorGradient();
    }

    // Shapes
    else if ( std::strcmp(name, Circle::getFactoryName()) == 0 ) {
        obj = new Circle();
    }
    else if ( std::strcmp(name, Point::getFactoryName()) == 0 ) {
        obj = new Point();
    }
    else if ( std::strcmp(name, Polygon::getFactoryName()) == 0 ) {
        obj = new Polygon();
    }
    else if ( std::strcmp(name, LineLoop::getFactoryName()) == 0 ) {
        obj = new LineLoop();
    }
    else if ( std::strcmp(name, Line::getFactoryName()) == 0 ) {
        obj = new Line();
    }
    else if ( std::strcmp(name, Arc::getFactoryName()) == 0 ) {
        obj = new Arc();
    }
    else if ( std::strcmp(name, OcclusionCircle::getFactoryName()) == 0 ) {
        obj = new OcclusionCircle();
    }
    else if ( std::strcmp(name, OcclusionArc::getFactoryName()) == 0 ) {
        obj = new OcclusionArc();
    }
    else if ( std::strcmp(name, Quad::getFactoryName()) == 0 ) {
        obj = new Quad();
    }
    else if ( std::strcmp(name, Triangle::getFactoryName()) == 0 ) {
        obj = new Triangle();
    }

    // Test Fields
    else if ( std::strcmp(name, AsciiText::getFactoryName()) == 0 ) {
        obj = new AsciiText();
    }
    else if ( std::strcmp(name, Cursor::getFactoryName()) == 0 ) {
        obj = new Cursor();
    }

    // Readouts
    else if ( std::strcmp(name, NumericReadout::getFactoryName()) == 0 ) {
        obj = new NumericReadout();
    }
    else if ( std::strcmp(name, HexReadout::getFactoryName()) == 0 ) {
        obj = new HexReadout();
    }
    else if ( std::strcmp(name, OctalReadout::getFactoryName()) == 0 ) {
        obj = new OctalReadout();
    }
    else if ( std::strcmp(name, TimeReadout::getFactoryName()) == 0 ) {
        obj = new TimeReadout();
    }
    else if ( std::strcmp(name, DirectionReadout::getFactoryName()) == 0 ) {
        obj = new DirectionReadout();
    }
    else if ( std::strcmp(name, LatitudeReadout::getFactoryName()) == 0 ) {
        obj = new LatitudeReadout();
    }
    else if ( std::strcmp(name, LongitudeReadout::getFactoryName()) == 0 ) {
        obj = new LongitudeReadout();
    }
    else if ( std::strcmp(name, Rotary::getFactoryName()) == 0 ) {
        obj = new Rotary();
    }
    else if ( std::strcmp(name, Rotary2::getFactoryName()) == 0 ) {
        obj = new Rotary2();
    }

    // Stroke Font
    else if ( std::strcmp(name, StrokeFont::getFactoryName()) == 0 ) {
        obj = new StrokeFont();
    }

    // Bitmap Font
    else if ( std::strcmp(name, BitmapFont::getFactoryName()) == 0 ) {
        obj = new BitmapFont();
    }

    // FTGL Fonts
    else if ( std::strcmp(name, FtglBitmapFont::getFactoryName()) == 0 ) {
        obj = new FtglBitmapFont();
    }
    else if ( std::strcmp(name, FtglOutlineFont::getFactoryName()) == 0 ) {
        obj = new FtglOutlineFont();
    }
    else if ( std::strcmp(name, FtglExtrdFont::getFactoryName()) == 0 ) {
        obj = new FtglExtrdFont();
    }
    else if ( std::strcmp(name, FtglPixmapFont::getFactoryName()) == 0 ) {
        obj = new FtglPixmapFont();
    }
    else if ( std::strcmp(name, FtglPolygonFont::getFactoryName()) == 0 ) {
        obj = new FtglPolygonFont();
    }
    else if ( std::strcmp(name, FtglHaloFont::getFactoryName()) == 0 ) {
        obj = new FtglHaloFont();
    }
    else if ( std::strcmp(name, FtglTextureFont::getFactoryName()) == 0 ) {
        obj = new FtglTextureFont();
    }

    // Bitmap Textures
    else if ( std::strcmp(name, BmpTexture::getFactoryName()) == 0 ) {
        obj = new BmpTexture();
    }
    // Material
    else if ( std::strcmp(name, Material::getFactoryName()) == 0 ) {
        obj = new Material();
    }
    // pages
    else if ( std::strcmp(name, MfdPage::getFactoryName()) == 0 ) {
        obj = new MfdPage();
    }
    else if ( std::strcmp(name, MapPage::getFactoryName()) == 0 ) {
        obj = new MapPage();
    }
    // Symbol loader
    else if ( std::strcmp(name, SymbolLoader::getFactoryName()) == 0 ) {
        obj = new SymbolLoader();
    }

    return obj;
}

}  // end namespace BasicGL
}  // end namespace Eaagles

