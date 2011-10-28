//******************************************************************************
// Graphic
//******************************************************************************
#include <limits.h>
#include <float.h>
#include "openeaagles/basicGL/Graphic.h"
#include "openeaagles/basicGL/Display.h"
#include "openeaagles/basicGL/ColorRotary.h"
#include "openeaagles/basicGL/Material.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Transforms.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Rgb.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Float.h"

namespace Eaagles {
namespace BasicGL {

IMPLEMENT_SUBCLASS(Graphic,"Graphic")

LCreal Graphic::fTimer = 0.0f;
GLuint Graphic::autoSelName = 0x00800000;

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Graphic)
    "color",                //  1: color
    "linewidth",            //  2: linewidth
    "flashRate",            //  3: Flash rate (Hz)
    "transform",            //  4: Coorinate transformations
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
    "stipple",              // 16: Line stippling - only used for line, lineloop, and circle when not filled.  It is put up here because of it's commanality
    "visible",              // 17: Visibility flag
    "mask",                 // 18: Color Masking
    "material",             // 19: Sets the current material
    "translateLight",       // 20: Translate our current light to a new position (BEFORE DRAWING)
END_SLOTTABLE(Graphic)

//------------------------------------------------------------------------------
//  Map slot table to handles 
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Graphic)
    ON_SLOT( 1, setColor, Basic::Color)        
    ON_SLOT( 1, setColor, Basic::String)
    ON_SLOT( 2, setSlotLineWidth, Basic::Number)
    ON_SLOT( 3, setSlotFlashRate, Basic::Number)
    ON_SLOT( 4, setSlotTransformList,   Basic::PairStream)
    ON_SLOT( 4, setSlotSingleTransform, Basic::Transform)
    ON_SLOT( 5, setSlotVertices, Basic::PairStream)
    ON_SLOT( 6, setSlotNormals, Basic::PairStream)
    ON_SLOT( 7, setSlotTexCoord, Basic::PairStream)
    ON_SLOT( 8, setSlotNoDisplayList, Basic::Number)
    ON_SLOT( 9, setSlotSubcomponentsFirst, Basic::Number)
    ON_SLOT(10, setSlotSelectName, Basic::Number)
    ON_SLOT(11, setSlotTextureName, Basic::Identifier)
    ON_SLOT(12, setSlotScissorX, Basic::Number)
    ON_SLOT(13, setSlotScissorY, Basic::Number)
    ON_SLOT(14, setSlotScissorWidth, Basic::Number)
    ON_SLOT(15, setSlotScissorHeight, Basic::Number)
    ON_SLOT(16, setSlotStippling, Basic::Number)
    ON_SLOT(17, setSlotVisibility, Basic::Number)
    ON_SLOT(18, setSlotMask, Basic::Number)
    ON_SLOT(19, setMaterial, Basic::Identifier)
    ON_SLOT(19, setMaterial, BasicGL::Material)
    ON_SLOT(20, setSlotTranslateLight, Basic::PairStream)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Event Table
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Graphic)
    ON_EVENT_OBJ(SET_COLOR,setColor,Basic::Color)   // Color given as a Basic::Color object (ie.. rgb)
    ON_EVENT_OBJ(SET_COLOR,setColor,Basic::String)  // Color given as a string (ie.. "red")
    ON_EVENT_OBJ(SET_COLOR,setColor,Basic::Number)  // Color given as a value (for a color rotary.. ie.. 4 is the fourth color in the rotary list)
    ON_EVENT_OBJ(SET_MATERIAL,setMaterial,Basic::Identifier )
    ON_EVENT_OBJ(SET_MATERIAL,setMaterial,BasicGL::Material)
    ON_EVENT_OBJ(SET_TEXTURE,onSetTextureId,Basic::Number)
    ON_EVENT_OBJ(SET_LINEWIDTH,onSetLineWidthEvent,Basic::Number)
    ON_EVENT_OBJ(SET_FLASHRATE,onSetFlashRateEvent,Basic::Number)
    ON_EVENT_OBJ(SET_VISIBILITY,onSetVisibilityEvent,Basic::Number)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Graphic::Graphic()
{
    STANDARD_CONSTRUCTOR()

    // Our Display
    displayPtr = 0;

    // This object is visible
    visible  = true;

    // No color
    color = 0;
    colorName = 0;

    // No texture
    texture = 0;
    texName = 0;

    // No linewidth
    linewidth = 0.0f;
    
    // Flash rate
    fRate = 0.0;

    // Select name
    selName = 0;

    // No transforms
    haveMatrix  = false;
    haveMatrix1 = false;
    m.makeIdentity();
    m1.makeIdentity();
    transforms = 0;

    // No vertices
    vertices = 0;
    nv = 0;

    // No texture coordinates
    texCoord = 0;
    ntc = 0;
  
    // No Normals
    norms = 0;
    nn = 0;
  
    // Scissor parameters
    scissorX = 0;
    scissorY = 0;
    scissorHeight = 0;
    scissorWidth = 0;

    // Default display list
    dlist = 0;
    noDisplayList = false;
    postDraw = false;

    stipple = false;
    mask = false;
    materialName = 0;
    materialObj = 0;
    
    lightPos.set(0.0, 0.0, 1.0, 0.0);
    lightMoved = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Graphic::copyData(const Graphic& org, const bool cc)
{
    BaseClass::copyData(org);

    // If called by the copy constructor, init these pointers ...
    if (cc) {
        displayPtr = 0;
        color = 0;
        colorName = 0;
        texName = 0;
        transforms = 0;
        vertices = 0;
        nv = 0;
        texCoord = 0;
        ntc = 0;
        norms = 0;
        nn = 0;
        materialName = 0;
        materialObj = 0;
    }

    // Just reset our display (function getDisplay() should set it)
    displayPtr = 0;

    visible  = org.visible;

    // copy color information
    if (org.colorName != 0) {
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
    if (transforms != 0)     { transforms->unref(); transforms = 0; }
    if (org.transforms != 0) { transforms = (Basic::PairStream*) org.transforms->clone(); }
    haveMatrix  = org.haveMatrix;
    haveMatrix1 = org.haveMatrix1;
    m  = org.m;
    m1 = org.m1;

    setVertices(org.vertices, org.nv);
    setNormals(org.norms, org.nn);
    setTextureCoord(org.texCoord, org.ntc);

    // Texture
    texture = 0;
    const Basic::Identifier* tname = org.texName;
    texName = (Basic::Identifier*) tname;
    
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
    if (transforms != 0) transforms->unref();
    transforms = 0;
    haveMatrix  = false;
    haveMatrix1 = false;
    m.makeIdentity();
    m1.makeIdentity();

    setVertices(0,0);
    setNormals(0,0);
    setTextureCoord(0,0);

    // delete color table name, but not the color
    if (colorName != 0) colorName->unref();
    colorName = 0;
    // delete the color if not from the color table (by colorName)
    if (color != 0) color->unref();
    color = 0;

    // Texture
    texture = 0;
    texName = 0;

    linewidth = 0.0f;
    fRate = 0.0;
    visible  = true;
    selName = 0;
    postDraw = false;
    if (materialName != 0) {
        materialName->unref();
        materialName = 0;
    }
    if (materialObj != 0) {
        materialObj->unref();
        materialObj = 0;
    }

}

//------------------------------------------------------------------------------
// findBySelectName() -- find one of our components by it's GL Select (pick) name
//                    (our children first then grandchildren)
//------------------------------------------------------------------------------
Basic::Pair* Graphic::findBySelectName(const GLuint name)
{
    Basic::Pair* q = 0;
    Basic::PairStream* subcomponents = getComponents();
    if (subcomponents != 0) {
        const Basic::List::Item* item = subcomponents->getFirstItem();
        while (item != 0 && q == 0) {
            Basic::Pair* p = (Basic::Pair*) item->getValue();
            Graphic* gobj = dynamic_cast<Graphic*>(p->object());
            if (gobj != 0 && gobj->getSelectName() == name) q = p;
            item = item->getNext();
        }
        item = subcomponents->getFirstItem();
        while (item != 0 && q == 0) {
            Basic::Pair* p = (Basic::Pair*) item->getValue();
            Graphic* gobj = dynamic_cast<Graphic*>(p->object());
            if (gobj != 0) q = gobj->findBySelectName(name);
            item = item->getNext();
        }
        subcomponents->unref();
        subcomponents = 0;
    }
    return q;
}


//------------------------------------------------------------------------------
// flashTimer() --
//------------------------------------------------------------------------------
LCreal Graphic::flashTimer()
{
    return fTimer;
}

LCreal Graphic::flashTimer(const LCreal dt)
{
    return (fTimer += dt);
}


//------------------------------------------------------------------------------
// flashOn(), flashOff() -- flash control flip/flop
//------------------------------------------------------------------------------
bool Graphic::flashOn() const
{
   LCreal hc = fRate*2.0f;          // Half cycles per second
   LCreal count = fTimer*hc;        // Number of half cycles since start
   return (int(count) & 0x1) != 0; // Flash is ON during odd half cycles
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
    if (displayPtr == 0) {
        if ( isClassType(typeid(Display))) {
            // When we're a Display
            displayPtr = (Display*)( this );
        }
        else {
            // Otherwise, check our (grand) parent container
            displayPtr = dynamic_cast<Display*>( findContainerByType(typeid(Display)) );
        }
        
    }
    if (displayPtr == 0) throw new ExpInvalidDisplayPtr();
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
   if (newName != 0) {
      if (texName == 0)
         texName = new Basic::Identifier();
    texName->setStr(newName);
   }
   else {
      texName = 0;
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

    if (materialName != 0 && getDisplay() != 0) tempMat = getDisplay()->getMaterial(materialName);

    if (tempMat != 0) {
        // enable a light
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        // make sure we move our light, if we need to
        
        // now add all the materials we need

        // set our ambient color
        osg::Vec4 amb = tempMat->getAmbientColor();
        GLfloat temp[4] = { GLfloat(amb.x()), GLfloat(amb.y()), GLfloat(amb.z()), GLfloat(amb.w()) };
        glMaterialfv(GL_FRONT, GL_AMBIENT, temp);

        osg::Vec4 dif = tempMat->getDiffuseColor();
        // now set the diffuse color
        temp[0] = GLfloat(dif.x());
        temp[1] = GLfloat(dif.y());
        temp[2] = GLfloat(dif.z());
        temp[3] = GLfloat(dif.w());
        glMaterialfv(GL_FRONT, GL_DIFFUSE, temp);

        // now emissive
        osg::Vec4 emis = tempMat->getEmissiveColor();
        temp[0] = GLfloat(emis.x());
        temp[1] = GLfloat(emis.y());
        temp[2] = GLfloat(emis.z());
        temp[3] = GLfloat(emis.w());
        glMaterialfv(GL_FRONT, GL_EMISSION, temp);

        // now specular
        osg::Vec4 spec = tempMat->getSpecularColor();
        temp[0] = GLfloat(spec.x());
        temp[1] = GLfloat(spec.y());
        temp[2] = GLfloat(spec.z());
        temp[3] = GLfloat(spec.w());
        glMaterialfv(GL_FRONT, GL_SPECULAR, temp);

        // now shininess
        GLfloat tempShine = (GLfloat)tempMat->getShininess();
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
    osg::Vec4 ocolor;
    if (materialName == 0 && materialObj == 0) {
        if (colorName != 0) {
            setOldColor = true;
            ocolor = display->getCurrentColor();
            display->setColor(*colorName);
        }
        else if (color != 0) {
            setOldColor = true;
            ocolor = display->getCurrentColor();
            const osg::Vec4* p = *color;
            display->setColor(*p);
        }
    }
    
    bool haveMaterial = false;
    GLfloat oldPos[4];
    if (lightMoved) {
        GLfloat temp[4] = { GLfloat(lightPos.x()), GLfloat(lightPos.y()), GLfloat(lightPos.z()), GLfloat(lightPos.w()) };
        // get our old position first
        glGetLightfv(GL_LIGHT0, GL_POSITION, oldPos);
        glLightfv(GL_LIGHT0, GL_POSITION, temp);
    }

    // if we have a material, switch to it
    if (materialName != 0 || materialObj != 0) {
        haveMaterial = true;
        setupMaterial();
    }

    // Find our texture, if we have one
    if (texName != 0 && texture == 0) {
        texture = display->getTextureByName(texName);
        if (texture == 0) {
            // If zero(0) was returned, the texture wasn't found
            // So forget about it.
            texName =0;
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
    Basic::PairStream* subcomponents = getComponents();
    if (subcomponents != 0) {
        Component* s = getSelectedComponent();
        if (s != 0) {
        // When we've selected only one
            Graphic* selected0 = dynamic_cast<Graphic*>(s);
            if (selected0 != 0) {
                selected0->draw();
            }
        }
        else {
            // When we should draw them all
            Basic::List::Item* item = subcomponents->getFirstItem();
            while (item != 0) {
                Basic::Pair* pair = (Basic::Pair*)(item->getValue());
                Graphic* obj = dynamic_cast<Graphic*>( pair->object() );
                if (obj != 0) obj->draw();
                item = item->getNext();
            }
        }
        subcomponents->unref();
        subcomponents = 0;
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
bool Graphic::setFlashRate(const LCreal r)
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

bool Graphic::setScissorX(const LCreal newX)
{
    scissorX = newX;
    return true;
}

bool Graphic::setScissorWidth(const LCreal newWidth)
{
    scissorWidth = newWidth;
    return true;
}

bool Graphic::setScissorY(const LCreal newY)
{
    scissorY = newY;
    return true;
}

bool Graphic::setScissorHeight(const LCreal newHeight)
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
    if (transforms != 0) {
        const Basic::List::Item* item = transforms->getFirstItem();
        while (item != 0) {
            Basic::Pair* p = (Basic::Pair*) item->getValue();
            Basic::Transform* t = dynamic_cast<Basic::Transform*>(p->object());
            if (t != 0) {
                m.preMult( *t );
                haveMatrix = true;
            }
            item = item->getNext();
        }
    }
}

//------------------------------------------------------------------------------
// setColor() -- set this object's color (using an Basic::Color)
//------------------------------------------------------------------------------
bool Graphic::setColor(const Basic::Color* cobj)       
{
    // Unref old colors
    if (color != 0) { color->unref(); color = 0; }
    if (colorName != 0) { colorName->unref(); colorName = 0; }

    if (cobj != 0) {
        // When we're being passed a color ...
        color = (Basic::Color*) cobj->clone();
    }
    
    return true;
}

//------------------------------------------------------------------------------
// setColor() -- set this object's color (using an Basic::Identifier)
//------------------------------------------------------------------------------
bool Graphic::setColor(const Basic::String* cnobj)
{
    // Unref old colors
    if (color != 0) { color->unref(); color = 0; }
    if (colorName != 0) { colorName->unref(); colorName = 0; }

    if (cnobj != 0) {
       // When we're being passed a name of a color from the color table ...
            colorName = (Basic::Identifier*) cnobj->clone();
    }
    return true;
}

//------------------------------------------------------------------------------
// setColor() -- set this object's color (using an Basic::Number)
// This is used with a color rotary
//------------------------------------------------------------------------------
bool Graphic::setColor(const Basic::Number* const cnobj)
{
    // Unref our color name (if we have one)
    if (colorName != 0) { colorName->unref(); colorName = 0; }

    // we have to have a color rotary to do this
    ColorRotary* cr = dynamic_cast<ColorRotary*>(color);
    if (cr != 0 && cnobj != 0) {
        cr->determineColor(cnobj->getReal());
    }

    return true;
}

//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------

// handle the SET_TEXTURE event
bool Graphic::onSetTextureId(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setTexture((GLuint) v);
      }
   }
   return ok;
}

// onSetLineWidthEvent -- handle the SET_LINEWIDTH event
bool Graphic::onSetLineWidthEvent(const Basic::Number* const msg)
{
    return setSlotLineWidth(msg);
}

// onSetFlashRateEvent -- handle the SET_FLASHRATE event
bool Graphic::onSetFlashRateEvent(const Basic::Number* const msg)
{
    return setSlotFlashRate(msg);
}

// onSetVisibilityEvent -- handle the SET_VISIBILITY event
bool Graphic::onSetVisibilityEvent(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
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
            osg::Vec2* ep1,      // Line endpoint #1
            osg::Vec2* ep2,      // Line endpoint #2
            const LCreal minX,   // Clip box min X value
            const LCreal maxX,   // Clip box max X value
            const LCreal minY,   // Clip box min Y value
            const LCreal maxY)   // Clip box max Y value
{
   LCreal x1 = ep1->_v[0];
   LCreal y1 = ep1->_v[1];
   LCreal x2 = ep2->_v[0];
   LCreal y2 = ep2->_v[1];

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
               LCreal tx = x1;
               LCreal ty = y1;
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
bool Graphic::setVertices(const osg::Vec3* const v, const unsigned int n)
{
   // Delete any old vertices
   if (vertices != 0) {
      delete[] vertices;
      vertices = 0;
      nv = 0;
   }

   // Copy vertices
   if (n > 0 && v != 0) {
      nv = n;
      vertices = new osg::Vec3[nv];
      for (unsigned int i = 0; i < nv; i++) {
         vertices[i] = v[i];
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Sets the normals
//------------------------------------------------------------------------------
bool Graphic::setNormals(const osg::Vec3* const v, const unsigned int n)
{
   // Delete any old vertices
   if (norms != 0) {
      delete[] norms;
      norms = 0;
      nn = 0;
   }

   // Copy vertices
   if (n > 0 && v != 0) {
      nn = n;
      norms = new osg::Vec3[nn];
      for (unsigned int i = 0; i < nn; i++) {
         norms[i] = v[i];
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Sets the texture coordinates
//------------------------------------------------------------------------------
bool Graphic::setTextureCoord(const osg::Vec2* const v, const unsigned int n)
{
   // Delete any old texture coordinates
   if (texCoord != 0) {
      delete[] texCoord;
      texCoord = 0;
      ntc = 0;
   }

   // Copy texture coordinates
   if (n > 0 && v != 0) {
      ntc = n;
      texCoord = new osg::Vec2[ntc];
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
bool Graphic::setSlotTransformList(Basic::PairStream* list)
{
    bool ok = true;
    if (transforms != 0) transforms->unref();
    transforms = list;
    if (transforms != 0) {
        transforms->ref();
        Basic::List::Item* item = transforms->getFirstItem();
        while (item != 0) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Basic::Transform* ip = dynamic_cast<Basic::Transform*>( pair->object() );
            if (ip == 0) {
                // It's not a Basic::Transform!!! 
                if (isMessageEnabled(MSG_WARNING)) {
                    std::cerr << "Graphic::setSlotTransformList: a member of the list is not a Basic::Transform!" << std::endl;
                }
                ok = false;
            }
            item = item->getNext();
        }
    }
    setupMatrix();

    return ok;
}

// setSlotSingleTransform() -- makes a list out of a single Basic::Transform
bool Graphic::setSlotSingleTransform(Basic::Transform* const sobj)
{
    Basic::PairStream* list = new Basic::PairStream();
    Basic::Pair* pair = new Basic::Pair("1", sobj);
    list->put(pair);
    bool ok = setSlotTransformList(list);
    pair->unref();
    list->unref();
    return ok;
}

// setSlotTranslateLight() -- tell us where to translate our light
bool Graphic::setSlotTranslateLight(Basic::PairStream* const msg)
{
    if (msg != 0) {
        LCreal temp[4] = { 0, 0, 1, 0 };
        Basic::List::Item* item = msg->getFirstItem();
        int count = 0;
        while (item != 0 && count < 4) {
            Basic::Pair* pair = (Basic::Pair*)item->getValue();
            if (pair != 0) {
                Basic::Number* num = dynamic_cast<Basic::Number*>(pair->object());
                if (num != 0) {
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

bool Graphic::setLightPosition(const LCreal x, const LCreal y, const LCreal z, const LCreal w)
{
    lightPos.set(x, y, z, w);
        lightMoved = true;
    return true;
    }
  
bool Graphic::setLightPosition(osg::Vec4& newPos)
{
    lightPos = newPos;
    return true;
}


// setSlotLineWidth -- set this object's line width
bool Graphic::setSlotLineWidth(const Basic::Number* const msg)
{
    if (msg != 0) return setLineWidth( msg->getFloat() );
    else return false;
}

// setSlotFlashRate -- set this object's flash rate
bool Graphic::setSlotFlashRate(const Basic::Number* const msg)
{
    if (msg != 0) return setFlashRate(msg->getReal());
    else return false;
}


// setSlotNoDisplayList() --  True to disable display list (default false)
bool Graphic::setSlotNoDisplayList(const Basic::Number* const msg)
{
    bool ok = (msg != 0);
    if (ok) ok = setDisableDisplayList( msg->getBoolean() );
    return ok;  
}

//  setSlotSubcomponentsFirst() --  Draw component graphics first (default: draw own graphics first)
bool Graphic::setSlotSubcomponentsFirst(const Basic::Number* const scfobj)
{
    bool ok = (scfobj != 0);
    if (ok) postDraw = scfobj->getBoolean();
    return ok;
}

// setSlotSelectName() -- GL Select Buffer name (e.g., glPushName())  (unsigned integer)
bool Graphic::setSlotSelectName(const Basic::Number* const snobj)
{
    bool ok = (snobj != 0);
    if (ok) {
         int name = snobj->getInt();
         ok = setSelectName( (GLuint) name );
    }
    return ok;
}

// setSlotScissorX() - sets our x point for scissoring
bool Graphic::setSlotScissorX(const Basic::Number* const newX)
{
    bool ok = false;
    if (newX != 0) {
        ok = setScissorX(newX->getReal());
    }
    return ok;
}

// setSlotScissorWidth() - sets how far out we are going to scissor horizontally
bool Graphic::setSlotScissorWidth(const Basic::Number* const newWidth)
{
    bool ok = false;
    if (newWidth != 0) {
        ok = setScissorWidth(newWidth->getReal());
    }
    return ok;
}

// setSlotScissorY() - sets our y point for scissoring
bool Graphic::setSlotScissorY(const Basic::Number* const newY)
{
    bool ok = false;
    if (newY != 0) {
        ok = setScissorY(newY->getReal());
    }
    return ok;
}

// setSlotScissorHeight() - sets how far out we are going to scissor vertically
bool Graphic::setSlotScissorHeight(const Basic::Number* const newHeight)
{
    bool ok = false;
    if (newHeight != 0) {
        ok = setScissorHeight(newHeight->getReal());
    }
    return ok;
}

// setSlotStipple() - sets our stipple boolean value
bool Graphic::setSlotStippling(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) ok = setStippling(msg->getBoolean());
   return ok;
}

// setSlotVisibility() - sets our visibility boolean value
bool Graphic::setSlotVisibility(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) ok = setVisibility(msg->getBoolean());
   return ok;
}
// setSlotMask - determines if we turn off our color guns or not
bool Graphic::setSlotMask(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
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
bool Graphic::setSlotVertices(const Basic::PairStream* const msg)
{
   bool ok = true; 
   
   if (msg != 0) {

        // Clear any old vertices
        setVertices(0,0);

        // allocate space for the vertices
        unsigned int n = msg->entries();
        vertices = new osg::Vec3[n];

        // Get the vertices from the pair stream
        nv = 0;
        const Basic::List::Item* item = msg->getFirstItem();
        while (item != 0 && nv < n) {
            const Basic::Pair* p = dynamic_cast<const Basic::Pair*>(item->getValue());
                if (p != 0) {
                    const Basic::Object* obj2 = p->object();
                    const Basic::List* msg2 = dynamic_cast<const Basic::List*>(obj2);
                    if (msg2 != 0) {
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
                                std::cerr << "Graphic::setVertices: Cooridnates not in [ x y ] or [ x y z ] form!" << std::endl;
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
bool Graphic::setSlotNormals(const Basic::PairStream* const msg)
{
   bool ok = true; 
   
   if (msg != 0) {

        // Clear any old normals
        setNormals(0,0);

        // allocate space for the vertices
        unsigned int n = msg->entries();
        norms = new osg::Vec3[n];

        // Get the normals from the pair stream
        nn = 0;
        const Basic::List::Item* item = msg->getFirstItem();
        while (item != 0 && nn < n) {
            const Basic::Pair* p = dynamic_cast<const Basic::Pair*>(item->getValue());
                if (p != 0) {
                    const Basic::Object* obj2 = p->object();
                    const Basic::List* msg2 = dynamic_cast<const Basic::List*>(obj2);
                    if (msg2 != 0) {
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
                                std::cerr << "Graphic::setVertices: Cooridnates not in [ x y ] or [ x y z ] form!" << std::endl;
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
bool Graphic::setSlotTexCoord(const Basic::PairStream* const msg)
{
   bool ok = true; 
   
   if (msg != 0) {

        // Clear any old texture coords
        setTextureCoord(0,0);

        // allocate space for the vertices
        unsigned int n = msg->entries();
        texCoord = new osg::Vec2[n];

        // Get the vertices from the pair stream
        ntc = 0;
        const Basic::List::Item* item = msg->getFirstItem();
        while (item != 0 && ntc < n) {
            const Basic::Pair* p = dynamic_cast<const Basic::Pair*>(item->getValue());
                if (p != 0) {
                    const Basic::Object* obj2 = p->object();
                    const Basic::List* msg2 = dynamic_cast<const Basic::List*>(obj2);
                    if (msg2 != 0) {
                        float values[2];
                        int n = msg2->getNumberList(values, 2);
                        if (n == 2) {
                            texCoord[ntc].set(values[0],values[1]);
                            ntc++;
                        }
                        else {
                              if (isMessageEnabled(MSG_ERROR)) {
                            std::cerr << "Graphic::setTexCoord: Cooridnates not in [ s t ] form!" << std::endl;
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
bool Graphic::setMaterial(const Basic::Identifier* const msg)
{
    // Unref old materials
    if (materialObj != 0) { materialObj->unref(); materialObj = 0; }
    if (materialName != 0) { materialName->unref(); materialName = 0; }

    if (msg != 0) {
        // When we're being passed a name of a material from the material table...
        materialName = (Basic::Identifier*) msg->clone();
    }
    return true;
}

//------------------------------------------------------------------------------
// setMaterial() -- sets our material (by material object)
//------------------------------------------------------------------------------
bool Graphic::setMaterial(const BasicGL::Material* const msg)       
{
    // Unref old material
    if (materialObj != 0) { materialObj->unref(); materialObj = 0; }
    if (materialName != 0) { materialName->unref(); materialName = 0; }

    if (msg != 0) {
        // When we're being passed a material ...
        materialObj = (BasicGL::Material*) msg->clone();
    }
    
    return true;
}


//------------------------------------------------------------------------------
// setSlotTextureName() -- sets the name of the texture
//------------------------------------------------------------------------------
bool Graphic::setSlotTextureName(Basic::Identifier* obj)
{
    texName = obj;
    return true;
}

// SetStippling() - set the stippling value
bool Graphic::setStippling(const bool x) 
{
   stipple = x;
   return true;
}
    
//------------------------------------------------------------------------------
// processComponets() -- process our components; make sure the are all of
//	type Graphic (or derived); tell them that we are their container
//------------------------------------------------------------------------------
void Graphic::processComponents(
      Basic::PairStream* const list,
      const std::type_info&,
      Basic::Pair* const add,
      Basic::Component* const remove
   )
{
   BaseClass::processComponents(list,typeid(Graphic),add,remove);
}


//------------------------------------------------------------------------------
// getSlotByIndex() for Graphic
//------------------------------------------------------------------------------
Basic::Object* Graphic::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Graphic::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        //indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    if (color != 0) {
        indent(sout,i+j);
        sout << "color: ";
        if (colorName != 0) {
            // When we have the name of a color from the color table
            Basic::Identifier* cn = (Basic::Identifier*) colorName;
            cn->serialize(sout,i+j);
        }
        else {
            // When we have the color
            Basic::Color* cc = (Basic::Color*) color;
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

    if (transforms != 0) {
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
        int name = (int) getSelectName();
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

} // End BasicGL namespace
} // End Eaagles namespace
