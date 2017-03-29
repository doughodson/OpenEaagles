
#include "openeaagles/graphics/Graphic.hpp"
#include "openeaagles/graphics/Display.hpp"
#include "openeaagles/graphics/ColorRotary.hpp"
#include "openeaagles/graphics/Material.hpp"
#include "openeaagles/base/String.hpp"
#include "openeaagles/base/Transforms.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/Rgb.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/Integer.hpp"
#include "openeaagles/base/Float.hpp"

namespace oe {
namespace graphics {

IMPLEMENT_SUBCLASS(Graphic, "Graphic")

double Graphic::fTimer = 0.0;
GLuint Graphic::autoSelName = 0x00800000;

BEGIN_SLOTTABLE(Graphic)
    "color",                //  1: color
    "linewidth",            //  2: linewidth
    "flashRate",            //  3: Flash rate (Hz)
    "transform",            //  4: Coordinate transformations
    "vertices",             //  5: Vertices (2D vectors)            (World coord)
    "normals",              //  6: Normals (3D vectors)
    "texCoord",             //  7: Texture Coordinates (2D vectors)
    "noDisplayList",        //  8: True to disable display list (default false)
    "subcomponentsFirst",   //  9: Draw component graphics first (default: draw own graphics first)
    "selectName",           // 10: GL Select Buffer name , make sure this is < 0x800000, (see glPushName())  (unsigned integer)
    "texture",              // 11: Texture name
    "scissorX",             // 12: Left edge of the scissor box     (World coord)
    "scissorY",             // 13: Bottom edge of the scissor box   (World coord)
    "scissorWidth",         // 14: How far over do we scissor       (World coord)
    "scissorHeight",        // 15: How far up do we scissor         (World coord)
    "stipple",              // 16: Line stippling flag - only used for line, lineloop, and circle when not filled.  It is put up here because of its commonality
    "stippleFactor",        // 17: Line stipple factor, specifies a multiplier for each bit in line stipple pattern
    "stipplePattern",       // 18: Specifies a 16 bit Line stipple pattern; range 0x0000 (0) .. 0xFFFF (65535)
    "visible",              // 19: Visibility flag
    "mask",                 // 20: Color Masking
    "material",             // 21: Sets the current material
    "translateLight",       // 22: Translate our current light to a new position (BEFORE DRAWING)
END_SLOTTABLE(Graphic)

BEGIN_SLOT_MAP(Graphic)
    ON_SLOT( 1, setColor, base::Color)
    ON_SLOT( 1, setColor, base::Identifier)
    ON_SLOT( 2, setSlotLineWidth, base::Number)
    ON_SLOT( 3, setSlotFlashRate, base::Number)
    ON_SLOT( 4, setSlotTransformList,   base::PairStream)
    ON_SLOT( 4, setSlotSingleTransform, base::Transform)
    ON_SLOT( 5, setSlotVertices, base::PairStream)
    ON_SLOT( 6, setSlotNormals, base::PairStream)
    ON_SLOT( 7, setSlotTexCoord, base::PairStream)
    ON_SLOT( 8, setSlotNoDisplayList, base::Number)
    ON_SLOT( 9, setSlotSubcomponentsFirst, base::Number)
    ON_SLOT(10, setSlotSelectName, base::Number)
    ON_SLOT(11, setSlotTextureName, base::Identifier)
    ON_SLOT(12, setSlotScissorX, base::Number)
    ON_SLOT(13, setSlotScissorY, base::Number)
    ON_SLOT(14, setSlotScissorWidth, base::Number)
    ON_SLOT(15, setSlotScissorHeight, base::Number)
    ON_SLOT(16, setSlotStippling, base::Number)
    ON_SLOT(17, setSlotStippleFactor, base::Number)
    ON_SLOT(18, setSlotStipplePattern, base::Number)
    ON_SLOT(19, setSlotVisibility, base::Number)
    ON_SLOT(20, setSlotMask, base::Number)
    ON_SLOT(21, setMaterial, base::Identifier)
    ON_SLOT(21, setMaterial, graphics::Material)
    ON_SLOT(22, setSlotTranslateLight, base::PairStream)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Graphic)
    ON_EVENT_OBJ(SET_COLOR,setColor,base::Color)       // Color given as a base::Color object (e.g., rgb)
    ON_EVENT_OBJ(SET_COLOR,setColor,base::Identifier)  // Color given as a string (e.g., "red")
    ON_EVENT_OBJ(SET_COLOR,setColor,base::Number)      // Color given as a value (for a color rotary, e.g., 4 is the fourth color in the rotary list)
    ON_EVENT_OBJ(SET_MATERIAL,setMaterial,base::Identifier )
    ON_EVENT_OBJ(SET_MATERIAL,setMaterial,graphics::Material)
    ON_EVENT_OBJ(SET_TEXTURE,onSetTextureId,base::Number)
    ON_EVENT_OBJ(SET_LINEWIDTH,onSetLineWidthEvent,base::Number)
    ON_EVENT_OBJ(SET_FLASHRATE,onSetFlashRateEvent,base::Number)
    ON_EVENT_OBJ(SET_VISIBILITY,onSetVisibilityEvent,base::Number)
END_EVENT_HANDLER()

Graphic::Graphic()
{
    STANDARD_CONSTRUCTOR()

    initData();
}

void Graphic::initData()
{
    // No transforms
    m.makeIdentity();
    m1.makeIdentity();

    lightPos.set(0.0, 0.0, 1.0, 0.0);
}

void Graphic::copyData(const Graphic& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();

    // Just reset our display (function getDisplay() should set it)
    displayPtr = nullptr;

    visible  = org.visible;

    // copy color information
    if (org.colorName != nullptr) {
        // Color is a name from the color table
        setColor( org.colorName );
    }
    else {
        // Color only
        setColor( org.color );
    }

    // Post draw flag
    postDraw = org.postDraw;

    // copy linewidth
    linewidth = org.linewidth;

    // Copy select name
    selName = org.selName;

    // copy flash rate
    fRate   = org.fRate;

    // Copy transform and matrix information
    if (transforms != nullptr)     { transforms->unref(); transforms = nullptr; }
    if (org.transforms != nullptr) { transforms = org.transforms->clone(); }
    haveMatrix  = org.haveMatrix;
    haveMatrix1 = org.haveMatrix1;
    m  = org.m;
    m1 = org.m1;

    setVertices(org.vertices, org.nv);
    setNormals(org.norms, org.nn);
    setTextureCoord(org.texCoord, org.ntc);

    // Texture
    texture = 0;
    const base::Identifier* tname = org.texName;
    texName = const_cast<base::Identifier*>(static_cast<const base::Identifier*>(tname));

    // Scissor data
    scissorX = org.scissorX;
    scissorY = org.scissorY;
    scissorHeight = org.scissorHeight;
    scissorWidth = org.scissorWidth;

    setMaterial(org.materialName);
    setMaterial(org.materialObj);

    // display list
    noDisplayList = org.noDisplayList;  // Copy the flag ...
    dlist = 0;                          // but, we need to create our own list

    stipple = org.stipple;
    stippleFactor = org.stippleFactor;
    stipplePattern = org.stipplePattern;

    mask = org.mask;
    lightMoved = org.lightMoved;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Graphic::deleteData()
{
    // Delete display lists
    if (dlist > 0) {
        glDeleteLists(dlist,1);
    }
    dlist = 0;
    noDisplayList = false;

    // Delete list of transformations
    if (transforms != nullptr) transforms->unref();
    transforms = nullptr;
    haveMatrix  = false;
    haveMatrix1 = false;
    m.makeIdentity();
    m1.makeIdentity();

    setVertices(nullptr,0);
    setNormals(nullptr,0);
    setTextureCoord(nullptr,0);

    // delete color table name, but not the color
    if (colorName != nullptr) colorName->unref();
    colorName = nullptr;
    // delete the color if not from the color table (by colorName)
    if (color != nullptr) color->unref();
    color = nullptr;

    // Texture
    texture = 0;
    texName = nullptr;

    linewidth = 0.0f;
    fRate = 0.0;
    visible  = true;
    selName = 0;
    postDraw = false;
    if (materialName != nullptr) {
        materialName->unref();
        materialName = nullptr;
    }
    if (materialObj != nullptr) {
        materialObj->unref();
        materialObj = nullptr;
    }

}

//------------------------------------------------------------------------------
// findBySelectName() -- find one of our components by its GL Select (pick) name
//                    (our children first then grandchildren)
//------------------------------------------------------------------------------
base::Pair* Graphic::findBySelectName(const GLuint name)
{
    base::Pair* q = nullptr;
    base::PairStream* subcomponents = getComponents();
    if (subcomponents != nullptr) {
        const base::List::Item* item = subcomponents->getFirstItem();
        while (item != nullptr && q == nullptr) {
            const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
            const auto gobj = dynamic_cast<Graphic*>(p->object());
            if (gobj != nullptr && gobj->getSelectName() == name) q = p;
            item = item->getNext();
        }
        item = subcomponents->getFirstItem();
        while (item != nullptr && q == nullptr) {
            const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
            const auto gobj = dynamic_cast<Graphic*>(p->object());
            if (gobj != nullptr) q = gobj->findBySelectName(name);
            item = item->getNext();
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }
    return q;
}


//------------------------------------------------------------------------------
// flashTimer() --
//------------------------------------------------------------------------------
double Graphic::flashTimer()
{
    return fTimer;
}

double Graphic::flashTimer(const double dt)
{
    return (fTimer += dt);
}


//------------------------------------------------------------------------------
// flashOn(), flashOff() -- flash control flip/flop
//------------------------------------------------------------------------------
bool Graphic::flashOn() const
{
   double hc = fRate*2.0f;          // Half cycles per second
   double count = fTimer*hc;        // Number of half cycles since start
   return (static_cast<int>(count) & 0x1) != 0; // Flash is ON during odd half cycles
}

bool Graphic::flashOff() const
{
   return !flashOn();
}


//-----------------------------------------------------------------------------
// getDisplay() -- Returns a pointer to our display
//-----------------------------------------------------------------------------
Display* Graphic::getDisplay()
{
    if (displayPtr == nullptr) {
        if ( isClassType(typeid(Display))) {
            // When we're a Display
            displayPtr = static_cast<Display*>(this);
        }
        else {
            // Otherwise, check our (grand) parent container
            displayPtr = dynamic_cast<Display*>( findContainerByType(typeid(Display)) );
        }

    }
    if (displayPtr == nullptr) throw new ExpInvalidDisplayPtr();
    return displayPtr;
}

//------------------------------------------------------------------------------
// getStdLineWidth() - get the standard line width
//------------------------------------------------------------------------------
GLfloat Graphic::getStdLineWidth()
{
    return getDisplay()->getStdLineWidth();
}

//------------------------------------------------------------------------------
// setTextureName() - sets the name of our texture on the fly
//------------------------------------------------------------------------------
bool Graphic::setTextureName(const char* newName)
{
    if (newName != nullptr) {
        if (texName == nullptr)
            texName = new base::Identifier();
        texName->setStr(newName);
    }
    else {
        texName = nullptr;
    }
    texture = 0;
    return true;
}

//------------------------------------------------------------------------------
// setupMaterial -- enable material and set it, depending on the type
//------------------------------------------------------------------------------
void Graphic::setupMaterial()
{
    Material* tempMat = materialObj;

    if (materialName != nullptr && getDisplay() != nullptr) tempMat = getDisplay()->getMaterial(materialName);

    if (tempMat != nullptr) {
        // enable a light
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        // make sure we move our light, if we need to

        // now add all the materials we need

        // set our ambient color
        base::Vec4d amb = tempMat->getAmbientColor();
        GLfloat temp[4] = { static_cast<GLfloat>(amb.x()), static_cast<GLfloat>(amb.y()),
                            static_cast<GLfloat>(amb.z()), static_cast<GLfloat>(amb.w()) };
        glMaterialfv(GL_FRONT, GL_AMBIENT, temp);

        base::Vec4d dif = tempMat->getDiffuseColor();
        // now set the diffuse color
        temp[0] = static_cast<GLfloat>(dif.x());
        temp[1] = static_cast<GLfloat>(dif.y());
        temp[2] = static_cast<GLfloat>(dif.z());
        temp[3] = static_cast<GLfloat>(dif.w());
        glMaterialfv(GL_FRONT, GL_DIFFUSE, temp);

        // now emissive
        base::Vec4d emis = tempMat->getEmissiveColor();
        temp[0] = static_cast<GLfloat>(emis.x());
        temp[1] = static_cast<GLfloat>(emis.y());
        temp[2] = static_cast<GLfloat>(emis.z());
        temp[3] = static_cast<GLfloat>(emis.w());
        glMaterialfv(GL_FRONT, GL_EMISSION, temp);

        // now specular
        base::Vec4d spec = tempMat->getSpecularColor();
        temp[0] = static_cast<GLfloat>(spec.x());
        temp[1] = static_cast<GLfloat>(spec.y());
        temp[2] = static_cast<GLfloat>(spec.z());
        temp[3] = static_cast<GLfloat>(spec.w());
        glMaterialfv(GL_FRONT, GL_SPECULAR, temp);

        // now shininess
        GLfloat tempShine = static_cast<GLfloat>(tempMat->getShininess());
        glMaterialf(GL_FRONT, GL_SHININESS, tempShine);
    }
}


//------------------------------------------------------------------------------
// draw -- draw this object and its children
//------------------------------------------------------------------------------
void Graphic::draw()
{
    Display* display = getDisplay();

    // Flashing: return if flashing and the flash flip/flop is OFF
    if (isFlashing() && flashOff()) return;

    // When this object is visible ...
    if ( !isVisible() ) return;

    // do any required translations, rotations, and/or scaling
    if ( matrixIsActive() ) {
        glPushMatrix();
        lcMultMatrix(m.ptr());
    }

    // if we have a color and no material, switch to that color
    bool setOldColor = false;
    base::Vec4d ocolor;
    if (materialName == nullptr && materialObj == nullptr) {
        if (colorName != nullptr) {
            setOldColor = true;
            ocolor = display->getCurrentColor();
            display->setColor(*colorName);
        }
        else if (color != nullptr) {
            setOldColor = true;
            ocolor = display->getCurrentColor();
            const base::Vec4d* p = *color;
            display->setColor(*p);
        }
    }

    bool haveMaterial = false;
    GLfloat oldPos[4];
    if (lightMoved) {
        GLfloat temp[4] = { static_cast<GLfloat>(lightPos.x()), static_cast<GLfloat>(lightPos.y()),
                            static_cast<GLfloat>(lightPos.z()), static_cast<GLfloat>(lightPos.w()) };
        // get our old position first
        glGetLightfv(GL_LIGHT0, GL_POSITION, oldPos);
        glLightfv(GL_LIGHT0, GL_POSITION, temp);
    }

    // if we have a material, switch to it
    if (materialName != nullptr || materialObj != nullptr) {
        haveMaterial = true;
        setupMaterial();
    }

    // Find our texture, if we have one
    if (texName != nullptr && texture == 0) {
        texture = display->getTextureByName(texName);
        if (texture == 0) {
            // If zero(0) was returned, the texture wasn't found
            // So forget about it.
            texName =nullptr;
        }
    }

    if (texture != 0) {
        // we have a valid texture, enabled texturing and bind our texture.
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        // default to GL_DECAL
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    }

    // set line width
    GLfloat olw = 0.0;
    if (linewidth > 0.0f) {
        GLfloat nlw = linewidth * (display->getStdLineWidth());
        olw = display->setLinewidth(nlw);
    }

    // set line stipple
    if (stipple) {
        glEnable(GL_LINE_STIPPLE);
        if (stippleFactor > 0 && stippleFactor <= 256) glLineStipple(stippleFactor, stipplePattern);
    }
    else glDisable(GL_LINE_STIPPLE);

    // push our "select" name
    if (getSelectName() > 0) glPushName(getSelectName());

    // setup optional scissor box
    if (haveScissorBoxHave()) {
        display->setScissor(scissorX, (scissorX + scissorWidth), scissorY, (scissorY + scissorHeight));
    }
    // if we are masking, turn off our color mask
    if (mask) glColorMask(0,0,0,1);

    // Predraw our graphics before our components (children) graphics
    if (!postDraw) {
        if (dlist > 0)
            glCallList(dlist);
        else
            drawFunc();
    }

    // Draw my children
    base::PairStream* subcomponents = getComponents();
    if (subcomponents != nullptr) {
        Component* s = getSelectedComponent();
        if (s != nullptr) {
            // When we've selected only one
            const auto selected0 = dynamic_cast<Graphic*>(s);
            if (selected0 != nullptr) {
                selected0->draw();
            }
        }
        else {
            // When we should draw them all
            base::List::Item* item = subcomponents->getFirstItem();
            while (item != nullptr) {
                const auto pair = static_cast<base::Pair*>(item->getValue());
                const auto obj = dynamic_cast<Graphic*>( pair->object() );
                if (obj != nullptr) obj->draw();
                item = item->getNext();
            }
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }

    // Postdraw our graphics after our components (children) graphics
    if (postDraw) {
        if (dlist > 0)
            glCallList(dlist);
        else
            drawFunc();
    }

    // undo any changes we made
    if (haveScissorBoxHave()) display->clearScissor();
    if (getSelectName() > 0) glPopName();
    if (linewidth > 0.0f) display->setLinewidth(olw);
    if (setOldColor) display->setColor(ocolor);
    if (texture != 0) glDisable(GL_TEXTURE_2D);
    if (matrixIsActive()) glPopMatrix();
    if (mask) glColorMask(1,1,1,1);
    if (haveMaterial) {
        // if our light moved, move it back when we are finished
        if (lightMoved) glLightfv(GL_LIGHT0, GL_POSITION, oldPos);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_NORMALIZE);
    }
}

//------------------------------------------------------------------------------
// drawFunc -- function to do the actual OpenGL drawing
//------------------------------------------------------------------------------
void Graphic::drawFunc()
{
}


//------------------------------------------------------------------------------
// Set Functions
//------------------------------------------------------------------------------
bool Graphic::setFlashRate(const double r)
{
    if (r >= 0.0) {
        fRate = r;
        return true;
    }
    else return false;
}

bool Graphic::setSelectName(const GLuint v)
{
    selName = v;
    return true;
}

bool Graphic::setLineWidth(const GLfloat v)
{
    linewidth = v;
    return true;
}

bool Graphic::setVisibility(const bool v)
{
    visible = v;
    return true;
}

bool Graphic::setScissorX(const double newX)
{
    scissorX = newX;
    return true;
}

bool Graphic::setScissorWidth(const double newWidth)
{
    scissorWidth = newWidth;
    return true;
}

bool Graphic::setScissorY(const double newY)
{
    scissorY = newY;
    return true;
}

bool Graphic::setScissorHeight(const double newHeight)
{
    scissorHeight = newHeight;
    return true;
}

bool Graphic::setDisplayList(const GLuint v)
{
    dlist = v;
    return true;
}

bool Graphic::setDisableDisplayList(const bool flg)
{
    noDisplayList = flg;
    return true;
}

//------------------------------------------------------------------------------
// cursor() -- Returns true if text cursor should be seen within this
//             object and the position of the cursor.
//------------------------------------------------------------------------------
bool Graphic::cursor(int* ln, int* cp) const
{
   *ln = 0;
   *cp = 0;
   return false;
}


//------------------------------------------------------------------------------
// setupMatrix() -- setup the transformation matrix based on the list of
//                  transforms.
//------------------------------------------------------------------------------
void Graphic::setupMatrix()
{
    // Start out with an identity matrix
    m.makeIdentity();
    haveMatrix  = false;

    // Modify the matrix with all our transformations.
    if (transforms != nullptr) {
        const base::List::Item* item = transforms->getFirstItem();
        while (item != nullptr) {
            const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
            const auto t = dynamic_cast<base::Transform*>(p->object());
            if (t != nullptr) {
                m.preMult( *t );
                haveMatrix = true;
            }
            item = item->getNext();
        }
    }
}

//------------------------------------------------------------------------------
// setColor() -- set this object's color (using an base::Color)
//------------------------------------------------------------------------------
bool Graphic::setColor(const base::Color* cobj)
{
    // Unref old colors
    if (color != nullptr)     { color->unref(); color = nullptr; }
    if (colorName != nullptr) { colorName->unref(); colorName = nullptr; }

    if (cobj != nullptr) {
        // When we're being passed a color ...
        color = cobj->clone();
    }

    return true;
}

//------------------------------------------------------------------------------
// setColor() -- set this object's color (using an base::Identifier)
//------------------------------------------------------------------------------
bool Graphic::setColor(const base::Identifier* cnobj)
{
    // Unref old colors
    if (color != nullptr)     { color->unref(); color = nullptr; }
    if (colorName != nullptr) { colorName->unref(); colorName = nullptr; }

    if (cnobj != nullptr) {
       // When we're being passed a name of a color from the color table ...
       colorName = cnobj->clone();
    }
    return true;
}

//------------------------------------------------------------------------------
// setColor() -- set this object's color (using an base::Number)
// This is used with a color rotary
//------------------------------------------------------------------------------
bool Graphic::setColor(const base::Number* const cnobj)
{
    // Unref our color name (if we have one)
    if (colorName != nullptr) { colorName->unref(); colorName = nullptr; }

    // we have to have a color rotary to do this
    const auto cr = dynamic_cast<ColorRotary*>(color);
    if (cr != nullptr && cnobj != nullptr) {
        cr->determineColor(cnobj->getReal());
    }

    return true;
}

//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------

// handle the SET_TEXTURE event
bool Graphic::onSetTextureId(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setTexture(static_cast<GLuint>(v));
      }
   }
   return ok;
}

// onSetLineWidthEvent -- handle the SET_LINEWIDTH event
bool Graphic::onSetLineWidthEvent(const base::Number* const msg)
{
    return setSlotLineWidth(msg);
}

// onSetFlashRateEvent -- handle the SET_FLASHRATE event
bool Graphic::onSetFlashRateEvent(const base::Number* const msg)
{
    return setSlotFlashRate(msg);
}

// onSetVisibilityEvent -- handle the SET_VISIBILITY event
bool Graphic::onSetVisibilityEvent(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setVisibility( msg->getBoolean() );
   }
   return ok;

}


//------------------------------------------------------------------------------
// clipLine2D() -- General purpose 2D line clipping function
//    Clips the line defined by two endpoints (ep1 and ep2) against a 2D clip box.
//    Returns true if all or part of the original line is within the clip box.
//    Returns false if the original line is outside of the clip box, and therefore
//    the endpoints are not changed.
//------------------------------------------------------------------------------
bool Graphic::clipLine2D(
            base::Vec2d* ep1,     // Line endpoint #1
            base::Vec2d* ep2,     // Line endpoint #2
            const double minX,    // Clip box min X value
            const double maxX,    // Clip box max X value
            const double minY,    // Clip box min Y value
            const double maxY)    // Clip box max Y value
{
   double x1 = ep1->_v[0];
   double y1 = ep1->_v[1];
   double x2 = ep2->_v[0];
   double y2 = ep2->_v[1];

   bool accept = false;
   bool reject = false;

   while ( !(accept || reject) ) {

      // Check end points against the limits
      bool o1_1 = y1 > maxY;
      bool o1_2 = y1 < minY;
      bool o1_3 = x1 > maxX;
      bool o1_4 = x1 < minX;
      bool o2_1 = y2 > maxY;
      bool o2_2 = y2 < minY;
      bool o2_3 = x2 > maxX;
      bool o2_4 = x2 < minX;

      // When both end-points of the line segment are outside on the same edge,
      // then the line segment is rejected (outside the box)
      if ((o1_1 && o2_1) || (o1_2 && o2_2) || (o1_3 && o2_3) || (o1_4 && o2_4))
         reject = true;
      else
         reject = false;


      if (!reject) {
         // When not rejected ...

         // If both end-points oof the line segment are within the box, then
         // we can accept this line sigment.
         if (o1_1 || o1_2 || o1_3 || o1_4 || o2_1 || o2_2 || o2_3 || o2_4)
            accept = false;
         else
            accept = true;


         if (!accept) {
            // When not rejected or accepted (i.e., one end-point is inside box
            // and the other is outside the box), we clip ...

            // We're going to clip off end-point #1, and we'll swap the
            // end-points to make sure that #1 is the one being clipped.
            if (!(o1_1 || o1_2 || o1_3 || o1_4)) {
               double tx = x1;
               double ty = y1;
               x1 = x2;
               y1 = y2;
               x2 = tx;
               y2 = ty;

               bool to = o1_1;
               o1_1 = o2_1;
               o2_1 = to;

               to = o1_2;
               o1_2 = o2_2;
               o2_2 = to;

               to = o1_3;
               o1_3 = o2_3;
               o2_3 = to;

               to = o1_4;
               o1_4 = o2_4;
               o2_4 = to;
            }

            if (o1_1) {
               // Clip against the max Y (top)
               x1 = x1 + (x2 - x1) * (maxY - y1) / (y2 - y1);
               y1 = maxY;
            }
            else if (o1_2) {
               // Clip against the min Y (bottom)
               x1 = x1 + (x2 - x1) * (minY - y1) / (y2 - y1);
               y1 = minY;
            }
            else if (o1_3) {
               // Clip against the max X (right)
               y1 = y1 + (y2 - y1) * (maxX - x1) / (x2 - x1);
               x1 = maxX;
            }
            else if (o1_4) {
               // Clip against the min X (left)
               y1 = y1 + (y2 - y1) * (minX - x1) / (x2 - x1);
               x1 = minX;
            }
         }
      }
   }

   if (accept) {
      ep1->_v[0] = x1;
      ep1->_v[1] = y1;
      ep2->_v[0] = x2;
      ep2->_v[1] = y2;
   }

   return accept;
}

//------------------------------------------------------------------------------
// Sets the vertices
//------------------------------------------------------------------------------
bool Graphic::setVertices(const base::Vec3d* const v, const unsigned int n)
{
   // Delete any old vertices
   if (vertices != nullptr) {
      delete[] vertices;
      vertices = nullptr;
      nv = 0;
   }

   // Copy vertices
   if (n > 0 && v != nullptr) {
      nv = n;
      vertices = new base::Vec3d[nv];
      for (unsigned int i = 0; i < nv; i++) {
         vertices[i] = v[i];
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Sets the normals
//------------------------------------------------------------------------------
bool Graphic::setNormals(const base::Vec3d* const v, const unsigned int n)
{
   // Delete any old vertices
   if (norms != nullptr) {
      delete[] norms;
      norms = nullptr;
      nn = 0;
   }

   // Copy vertices
   if (n > 0 && v != nullptr) {
      nn = n;
      norms = new base::Vec3d[nn];
      for (unsigned int i = 0; i < nn; i++) {
         norms[i] = v[i];
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Sets the texture coordinates
//------------------------------------------------------------------------------
bool Graphic::setTextureCoord(const base::Vec2d* const v, const unsigned int n)
{
   // Delete any old texture coordinates
   if (texCoord != nullptr) {
      delete[] texCoord;
      texCoord = nullptr;
      ntc = 0;
   }

   // Copy texture coordinates
   if (n > 0 && v != nullptr) {
      ntc = n;
      texCoord = new base::Vec2d[ntc];
      for (unsigned int i = 0; i < ntc; i++) {
         texCoord[i] = v[i];
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// this is for creating your own texture... make sure you create a texture before
// you call this function!
//------------------------------------------------------------------------------
bool Graphic::setTexture(const GLuint newTex)
{
   texture = newTex;
   return true;
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

// setSlotTransformList() -- set the list of transformations
bool Graphic::setSlotTransformList(base::PairStream* list)
{
    bool ok = true;
    if (transforms != nullptr) transforms->unref();
    transforms = list;
    if (transforms != nullptr) {
        transforms->ref();
        base::List::Item* item = transforms->getFirstItem();
        while (item != nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto ip = dynamic_cast<base::Transform*>( pair->object() );
            if (ip == nullptr) {
                // It's not a base::Transform!!!
                if (isMessageEnabled(MSG_WARNING)) {
                    std::cerr << "Graphic::setSlotTransformList: a member of the list is not a base::Transform!" << std::endl;
                }
                ok = false;
            }
            item = item->getNext();
        }
    }
    setupMatrix();

    return ok;
}

// setSlotSingleTransform() -- makes a list out of a single base::Transform
bool Graphic::setSlotSingleTransform(base::Transform* const sobj)
{
    const auto list = new base::PairStream();
    const auto pair = new base::Pair("1", sobj);
    list->put(pair);
    bool ok = setSlotTransformList(list);
    pair->unref();
    list->unref();
    return ok;
}

// setSlotTranslateLight() -- tell us where to translate our light
bool Graphic::setSlotTranslateLight(base::PairStream* const msg)
{
    if (msg != nullptr) {
        double temp[4] = { 0, 0, 1, 0 };
        base::List::Item* item = msg->getFirstItem();
        int count = 0;
        while (item != nullptr && count < 4) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            if (pair != nullptr) {
                const auto num = dynamic_cast<base::Number*>(pair->object());
                if (num != nullptr) {
                    temp[count++] = num->getReal();
                }
            }
            item = item->getNext();
        }
        // W value is always 0
        temp[3] = 0;
        setLightPosition(temp[0],temp[1], temp[2], temp[3]);
    }

    return true;
}

bool Graphic::setLightPosition(const double x, const double y, const double z, const double w)
{
    lightPos.set(x, y, z, w);
    lightMoved = true;
    return true;
    }

bool Graphic::setLightPosition(base::Vec4d& newPos)
{
    lightPos = newPos;
    return true;
}


// setSlotLineWidth -- set this object's line width
bool Graphic::setSlotLineWidth(const base::Number* const msg)
{
    if (msg != nullptr) return setLineWidth( msg->getFloat() );
    else return false;
}

// setSlotFlashRate -- set this object's flash rate
bool Graphic::setSlotFlashRate(const base::Number* const msg)
{
    if (msg != nullptr) return setFlashRate(msg->getReal());
    else return false;
}


// setSlotNoDisplayList() --  True to disable display list (default false)
bool Graphic::setSlotNoDisplayList(const base::Number* const msg)
{
    bool ok = (msg != nullptr);
    if (ok) ok = setDisableDisplayList( msg->getBoolean() );
    return ok;
}

//  setSlotSubcomponentsFirst() --  Draw component graphics first (default: draw own graphics first)
bool Graphic::setSlotSubcomponentsFirst(const base::Number* const scfobj)
{
    bool ok = (scfobj != nullptr);
    if (ok) postDraw = scfobj->getBoolean();
    return ok;
}

// setSlotSelectName() -- GL Select Buffer name (e.g., glPushName())  (unsigned integer)
bool Graphic::setSlotSelectName(const base::Number* const snobj)
{
    bool ok = (snobj != nullptr);
    if (ok) {
         int name = snobj->getInt();
         ok = setSelectName(static_cast<GLuint>(name));
    }
    return ok;
}

// setSlotScissorX() - sets our x point for scissoring
bool Graphic::setSlotScissorX(const base::Number* const newX)
{
    bool ok = false;
    if (newX != nullptr) {
        ok = setScissorX(newX->getReal());
    }
    return ok;
}

// setSlotScissorWidth() - sets how far out we are going to scissor horizontally
bool Graphic::setSlotScissorWidth(const base::Number* const newWidth)
{
    bool ok = false;
    if (newWidth != nullptr) {
        ok = setScissorWidth(newWidth->getReal());
    }
    return ok;
}

// setSlotScissorY() - sets our y point for scissoring
bool Graphic::setSlotScissorY(const base::Number* const newY)
{
    bool ok = false;
    if (newY != nullptr) {
        ok = setScissorY(newY->getReal());
    }
    return ok;
}

// setSlotScissorHeight() - sets how far out we are going to scissor vertically
bool Graphic::setSlotScissorHeight(const base::Number* const newHeight)
{
    bool ok = false;
    if (newHeight != nullptr) {
        ok = setScissorHeight(newHeight->getReal());
    }
    return ok;
}

// setSlotStippling() - sets our stipple boolean value
bool Graphic::setSlotStippling(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) ok = setStippling(msg->getBoolean());
   return ok;
}

// setSlotStippleFactor() - sets our stipple factor integer value
bool Graphic::setSlotStippleFactor(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
       ok = setStippleFactor(static_cast<GLuint>(msg->getInt()));
   }
   return ok;
}

// setSlotStipplePattern() - sets our stipple pattern integer value
bool Graphic::setSlotStipplePattern(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xffff) {
         ok = setStipplePattern(static_cast<GLushort>(v));
      }
      else {
         std::cerr << "Graphic::setSlotStipplePattern() - invalid value: " << v << "; must be a 16 bit value; range 0x0000 (0) to 0xFFFF (65535)" << std::endl;
      }
   }
   return ok;
}

// setSlotVisibility() - sets our visibility boolean value
bool Graphic::setSlotVisibility(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) ok = setVisibility(msg->getBoolean());
   return ok;
}
// setSlotMask - determines if we turn off our color guns or not
bool Graphic::setSlotMask(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
        mask = msg->getBoolean();
        ok = true;
   }
   return ok;
}


//------------------------------------------------------------------------------
// setVertices() -- sets the vertices of a graphic object
//
// example --
//     vertices: { [ 1 2 3 ]  [ 4 5 6 ] [ 7 8 9 ] }
// ---
//------------------------------------------------------------------------------
bool Graphic::setSlotVertices(const base::PairStream* const msg)
{
   bool ok = true;

   if (msg != nullptr) {

        // Clear any old vertices
        setVertices(nullptr,0);

        // allocate space for the vertices
        unsigned int n = msg->entries();
        vertices = new base::Vec3d[n];

        // Get the vertices from the pair stream
        nv = 0;
        const base::List::Item* item = msg->getFirstItem();
        while (item != nullptr && nv < n) {
            const auto p = dynamic_cast<const base::Pair*>(item->getValue());
            if (p != nullptr) {
                const base::Object* obj2 = p->object();
                const auto msg2 = dynamic_cast<const base::List*>(obj2);
                if (msg2 != nullptr) {
                    float values[3];
                    int n = msg2->getNumberList(values, 3);
                    if (n == 2) {
                        vertices[nv].set(values[0],values[1],0.0f);
                        nv++;
                    }
                    else if (n == 3) {
                        vertices[nv].set(values[0],values[1],values[2]);
                        nv++;
                    }
                    else {
                        if (isMessageEnabled(MSG_WARNING)) {
                            std::cerr << "Graphic::setVertices: Coordinates not in [ x y ] or [ x y z ] form!" << std::endl;
                         }
                            ok = false;
                    }
                }
            }
            item = item->getNext();
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotNormals() -- sets the normals of the vertices of the graphic object
//
// example --
//     normals: { [ 1 2 3 ]  [ 4 5 6 ] [ 7 8 9 ] }
//------------------------------------------------------------------------------
bool Graphic::setSlotNormals(const base::PairStream* const msg)
{
   bool ok = true;

   if (msg != nullptr) {

        // Clear any old normals
        setNormals(nullptr,0);

        // allocate space for the vertices
        unsigned int n = msg->entries();
        norms = new base::Vec3d[n];

        // Get the normals from the pair stream
        nn = 0;
        const base::List::Item* item = msg->getFirstItem();
        while (item != nullptr && nn < n) {
            const auto p = dynamic_cast<const base::Pair*>(item->getValue());
                if (p != nullptr) {
                    const base::Object* obj2 = p->object();
                    const auto msg2 = dynamic_cast<const base::List*>(obj2);
                    if (msg2 != nullptr) {
                        float values[3];
                        int n = msg2->getNumberList(values, 3);
                        if (n == 2) {
                            norms[nn].set(values[0],values[1],0.0f);
                            nn++;
                        }
                        else if (n == 3) {
                                norms[nn].set(values[0],values[1],values[2]);
                                nn++;
                        }
                        else {
                              if (isMessageEnabled(MSG_ERROR)) {
                                std::cerr << "Graphic::setVertices: Coordinates not in [ x y ] or [ x y z ] form!" << std::endl;
                              }
                              ok = false;
                        }
                    }
                }
        item = item->getNext();
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setTexCoord() -- sets the vertices of a graphic object
//
// example --
//     texCoord: { [ 1 2 ]  [ 4 5 ] [ 7 8 ] }
//
//------------------------------------------------------------------------------
bool Graphic::setSlotTexCoord(const base::PairStream* const msg)
{
   bool ok = true;

   if (msg != nullptr) {

        // Clear any old texture coords
        setTextureCoord(nullptr,0);

        // allocate space for the vertices
        unsigned int n = msg->entries();
        texCoord = new base::Vec2d[n];

        // Get the vertices from the pair stream
        ntc = 0;
        const base::List::Item* item = msg->getFirstItem();
        while (item != nullptr && ntc < n) {
            const auto p = dynamic_cast<const base::Pair*>(item->getValue());
                if (p != nullptr) {
                    const base::Object* obj2 = p->object();
                    const auto msg2 = dynamic_cast<const base::List*>(obj2);
                    if (msg2 != nullptr) {
                        float values[2];
                        int n = msg2->getNumberList(values, 2);
                        if (n == 2) {
                            texCoord[ntc].set(values[0],values[1]);
                            ntc++;
                        }
                        else {
                            if (isMessageEnabled(MSG_ERROR)) {
                                std::cerr << "Graphic::setTexCoord: Coordinates not in [ s t ] form!" << std::endl;
                            }
                            ok = false;
                        }
                    }
                }
        item = item->getNext();
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setMaterial() -- sets our material (by name)
//------------------------------------------------------------------------------
bool Graphic::setMaterial(const base::Identifier* const msg)
{
    // Unref old materials
    if (materialObj != nullptr) { materialObj->unref(); materialObj = nullptr; }
    if (materialName != nullptr) { materialName->unref(); materialName = nullptr; }

    if (msg != nullptr) {
        // When we're being passed a name of a material from the material table...
        materialName = msg->clone();
    }
    return true;
}

//------------------------------------------------------------------------------
// setMaterial() -- sets our material (by material object)
//------------------------------------------------------------------------------
bool Graphic::setMaterial(const graphics::Material* const msg)
{
    // Unref old material
    if (materialObj != nullptr) { materialObj->unref(); materialObj = nullptr; }
    if (materialName != nullptr) { materialName->unref(); materialName = nullptr; }

    if (msg != nullptr) {
        // When we're being passed a material ...
        materialObj = msg->clone();
    }

    return true;
}


//------------------------------------------------------------------------------
// setSlotTextureName() -- sets the name of the texture
//------------------------------------------------------------------------------
bool Graphic::setSlotTextureName(base::Identifier* obj)
{
    texName = obj;
    return true;
}

// setStippling() - set the stippling value
bool Graphic::setStippling(const bool x)
{
   stipple = x;
   return true;
}

// setStippleFactor() - set the stipple factor value
bool Graphic::setStippleFactor(const GLuint x)
{
   stippleFactor = x;
   return true;
}

// setStipplePattern() - set the stipple pattern value
bool Graphic::setStipplePattern(const GLushort x)
{
   stipplePattern = x;
   return true;
}

//------------------------------------------------------------------------------
// processComponets() -- process our components; make sure the are all of
// type Graphic (or derived); tell them that we are their container
//------------------------------------------------------------------------------
void Graphic::processComponents(
      base::PairStream* const list,
      const std::type_info&,
      base::Pair* const add,
      base::Component* const remove
   )
{
   base::Component::processComponents(list, typeid(Graphic),add,remove);
}

std::ostream& Graphic::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        //indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    if (color != nullptr) {
        indent(sout,i+j);
        sout << "color: ";
        if (colorName != nullptr) {
            // When we have the name of a color from the color table
            base::Identifier* cn = static_cast<base::Identifier*>(colorName);
            cn->serialize(sout,i+j);
        }
        else {
            // When we have the color
            base::Color* cc = static_cast<base::Color*>(color);
            cc->serialize(sout,i+j);
        }
        sout << std::endl;
    }

    if (linewidth > 0.0) {
        indent(sout,i+j);
        sout << "linewidth: ";
        sout << linewidth;
        sout << std::endl;
    }

    if ( isFlashing() ) {
        indent(sout,i+j);
        sout << "flashRate: " << getFlashRate() << std::endl;
    }

    if (transforms != nullptr) {
        indent(sout,i+j);
        sout << "transform: {" << std::endl;
        transforms->serialize(sout,i+j+4,slotsOnly);
        indent(sout,i+j);
        sout << "}" << std::endl;
    }

    if (nv > 0) {
        indent(sout,i+j);
        sout << "vertices: ";
        sout << "{ " << std::endl;;
        for (unsigned int ii = 0; ii < nv; ii++) {
            indent(sout,i+j+4);
            sout << "[ ";
            sout << vertices[ii][0] << " ";
            sout << vertices[ii][1] << " ";
            sout << vertices[ii][2] << " ";
            sout << " ]";
            sout << std::endl;
        }
        indent(sout,i+j);
        sout << "}" << std::endl;;
    }

    if (nn > 0) {
        indent(sout,i+j);
        sout << "normals: ";
        sout << "{ " << std::endl;;
        for (unsigned int ii = 0; ii < nn; ii++) {
            indent(sout,i+j+4);
            sout << "[ ";
            sout << norms[ii][0] << " ";
            sout << norms[ii][1] << " ";
            sout << norms[ii][2] << " ";
            sout << " ]";
            sout << std::endl;
        }
        indent(sout,i+j);
        sout << "}" << std::endl;;
    }

    if (ntc > 0) {
        indent(sout,i+j);
        sout << "texCoord: ";
        sout << "{ " << std::endl;;
        for (unsigned int ii = 0; ii < ntc; ii++) {
            indent(sout,i+j+4);
            sout << "[ ";
            sout << texCoord[ii][0] << " ";
            sout << texCoord[ii][1] << " ";
            sout << " ]";
            sout << std::endl;
        }
        indent(sout,i+j);
        sout << "}" << std::endl;;
    }

    if (noDisplayList) {
        indent(sout,i+j);
        sout << "noDisplayList: 1" << std::endl;;
    }

    if (postDraw) {
        indent(sout,i+j);
        sout << "setSlotSubcomponentsFirst: 1" << std::endl;;
    }

    if (getSelectName() > 0) {
        int name = static_cast<int>(getSelectName());
        indent(sout,i+j);
        sout << "selectName: " << name << std::endl;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
