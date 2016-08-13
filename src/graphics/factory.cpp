
#include "openeaagles/graphics/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/graphics/Graphic.h"
#include "openeaagles/graphics/Display.h"
#include "openeaagles/graphics/Shapes.h"
#include "openeaagles/graphics/Polygon.h"
#include "openeaagles/graphics/ColorRotary.h"
#include "openeaagles/graphics/ColorGradient.h"
#include "openeaagles/graphics/FtglFonts.h"
#include "openeaagles/graphics/BitmapFont.h"
#include "openeaagles/graphics/StrokeFont.h"
#include "openeaagles/graphics/BmpTexture.h"
#include "openeaagles/graphics/Rotators.h"
#include "openeaagles/graphics/Translator.h"
#include "openeaagles/graphics/Material.h"

// fields and readouts
#include "openeaagles/graphics/readouts/AsciiText.h"
#include "openeaagles/graphics/readouts/Cursor.h"
#include "openeaagles/graphics/readouts/Rotary.h"
#include "openeaagles/graphics/readouts/Rotary2.h"
#include "openeaagles/graphics/readouts/DirectionReadout.h"
#include "openeaagles/graphics/readouts/HexReadout.h"
#include "openeaagles/graphics/readouts/LatitudeReadout.h"
#include "openeaagles/graphics/readouts/LongitudeReadout.h"
#include "openeaagles/graphics/readouts/NumericReadout.h"
#include "openeaagles/graphics/readouts/OctalReadout.h"
#include "openeaagles/graphics/readouts/TimeReadout.h"

// pages and maps
#include "openeaagles/graphics/MfdPage.h"
#include "openeaagles/graphics/MapPage.h"
#include "openeaagles/graphics/SymbolLoader.h"



#include <string>

namespace oe {
namespace graphics {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // General graphics support
    if ( name == Graphic::getFactoryName() ) {
        obj = new Graphic();
    }
    else if ( name == Page::getFactoryName() ) {
        obj = new Page();
    }
    else if ( name == Display::getFactoryName() ) {
        obj = new Display();
    }
    else if ( name == Translator::getFactoryName() ) {
        obj = new Translator();
    }
    else if ( name == Rotators::getFactoryName() ) {
        obj = new Rotators();
    }
    else if ( name == ColorRotary::getFactoryName() ) {
        obj = new ColorRotary();
    }
    else if ( name == ColorGradient::getFactoryName() ) {
        obj = new ColorGradient();
    }

    // Shapes
    else if ( name == Circle::getFactoryName() ) {
        obj = new Circle();
    }
    else if ( name == Point::getFactoryName() ) {
        obj = new Point();
    }
    else if ( name == Polygon::getFactoryName() ) {
        obj = new Polygon();
    }
    else if ( name == LineLoop::getFactoryName() ) {
        obj = new LineLoop();
    }
    else if ( name == Line::getFactoryName() ) {
        obj = new Line();
    }
    else if ( name == Arc::getFactoryName() ) {
        obj = new Arc();
    }
    else if ( name == OcclusionCircle::getFactoryName() ) {
        obj = new OcclusionCircle();
    }
    else if ( name == OcclusionArc::getFactoryName() ) {
        obj = new OcclusionArc();
    }
    else if ( name == Quad::getFactoryName() ) {
        obj = new Quad();
    }
    else if ( name == Triangle::getFactoryName() ) {
        obj = new Triangle();
    }

    // Fields
    else if ( name == AsciiText::getFactoryName() ) {
        obj = new AsciiText();
    }
    else if ( name == Cursor::getFactoryName() ) {
        obj = new Cursor();
    }

    // Readouts
    else if ( name == NumericReadout::getFactoryName() ) {
        obj = new NumericReadout();
    }
    else if ( name == HexReadout::getFactoryName() ) {
        obj = new HexReadout();
    }
    else if ( name == OctalReadout::getFactoryName() ) {
        obj = new OctalReadout();
    }
    else if ( name == TimeReadout::getFactoryName() ) {
        obj = new TimeReadout();
    }
    else if ( name == DirectionReadout::getFactoryName() ) {
        obj = new DirectionReadout();
    }
    else if ( name == LatitudeReadout::getFactoryName() ) {
        obj = new LatitudeReadout();
    }
    else if ( name == LongitudeReadout::getFactoryName() ) {
        obj = new LongitudeReadout();
    }
    else if ( name == Rotary::getFactoryName() ) {
        obj = new Rotary();
    }
    else if ( name == Rotary2::getFactoryName() ) {
        obj = new Rotary2();
    }

    // Stroke Font
    else if ( name == StrokeFont::getFactoryName() ) {
        obj = new StrokeFont();
    }

    // Bitmap Font
    else if ( name == BitmapFont::getFactoryName() ) {
        obj = new BitmapFont();
    }

    // FTGL Fonts
    else if ( name == FtglBitmapFont::getFactoryName() ) {
        obj = new FtglBitmapFont();
    }
    else if ( name == FtglOutlineFont::getFactoryName() ) {
        obj = new FtglOutlineFont();
    }
    else if ( name == FtglExtrdFont::getFactoryName() ) {
        obj = new FtglExtrdFont();
    }
    else if ( name == FtglPixmapFont::getFactoryName() ) {
        obj = new FtglPixmapFont();
    }
    else if ( name == FtglPolygonFont::getFactoryName() ) {
        obj = new FtglPolygonFont();
    }
    else if ( name == FtglHaloFont::getFactoryName() ) {
        obj = new FtglHaloFont();
    }
    else if ( name == FtglTextureFont::getFactoryName() ) {
        obj = new FtglTextureFont();
    }

    // Bitmap Textures
    else if ( name == BmpTexture::getFactoryName() ) {
        obj = new BmpTexture();
    }
    // Material
    else if ( name == Material::getFactoryName() ) {
        obj = new Material();
    }
    // pages
    else if ( name == MfdPage::getFactoryName() ) {
        obj = new MfdPage();
    }
    else if ( name == MapPage::getFactoryName() ) {
        obj = new MapPage();
    }
    // Symbol loader
    else if ( name == SymbolLoader::getFactoryName() ) {
        obj = new SymbolLoader();
    }

    return obj;
}

}
}

