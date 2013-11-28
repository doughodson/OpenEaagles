//------------------------------------------------------------------------------
// Class: Graphic
//------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_Graphic_H__
#define __Eaagles_BasicGL_Graphic_H__

#include "openeaagles/basic/Component.h"
#include "openeaagles/basic/osg/Vec2"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/osg/Vec4"
#include "openeaagles/basic/osg/Matrix"
#include <GL/gl.h>

namespace Eaagles {

   namespace Basic {
      class Color;
      class Identifier;
      class Transform;
   }

namespace BasicGL {

class Display;
class Material;

//------------------------------------------------------------------------------
// Display List macros
//------------------------------------------------------------------------------
#define BEGIN_DLIST                             \
   if (isDisplayListEnabled()) {                \
      setDisplayList( glGenLists(1) );          \
      glNewList(getDisplayList(),GL_COMPILE);   \
   }

#define END_DLIST                               \
      if (isDisplayListEnabled()) glEndList();


//------------------------------------------------------------------------------
// Class:       Graphic
// Base class:  Object -> Component -> Graphic
//
// Description: Base class for the graphic objects.  Provides list of vertices,
//              transformation matrix, color, linewidth, etc.
//              NOTE: Graphic objects REQUIRE a Display class to function.
//
// Form name: Graphic
// Slots:
//    color              <Color>        ! Color (default: 0)
//    color              <Identifier>   ! Color by name (default: 0)
//    linewidth          <Number>       ! Linewidth (default: 0.0f)
//    flashRate          <Number>       ! Flash rate (default: 0.0f)
//    transform          <PairStream>   ! List of coordinate transformations (default: 0)
//    transform          <Transform>    ! Single coordinate transformation (default: 0)
//    vertices           <PairStream>   ! List of 3D Coordinates (World coord) (default: 0)
//    normals            <PairStream>   ! List of 3D Vectors of normals at each vertex
//    texCoord           <PairStream>   ! List of 2D Texture Coordinates (default: 0)
//    noDisplayList      <Number>       ! Flag: True to disable display list
//                                      !     (default false)
//    subcomponentsFirst <Number>       ! Flag: Draw component graphics first
//                                      ! (default: draw own graphics first)
//    selectName         <Number>       ! GL Select Buffer name
//                                      ! (see glPushName())  (unsigned integer) (default: 0)
//    texture            <Identifier>   ! Texture name (default: 0)
//    scissorX           <Number>       ! Left edge of the scissor box (World coord) (default: 0)
//    scissorY           <Number>       ! Bottom edge of the scissor box (World coord) (default: 0)
//    scissorWidth       <Number>       ! How far over do we scissor (World coord) (default: 0)
//    scissorHeight      <Number>       ! How far up do we scissor (World coord) (default: 0)
//    stipple            <Number>       ! Line stippling flag - only used for line, lineloop, and circle when not filled.
//    stippleFactor      <Number>       ! Line stipple factor, specifies a multiplier for each bit in line stipple pattern (default: 1)
//    stipplePattern     <Number>       ! Specifies a 16 bit Line stipple pattern; range 0x0000 (0) .. 0xFFFF (65535) (default: 0xFFFF)
//    visible            <Number>       ! Visibility flag
//    mask               <Number>       ! Color Masking
//    material           <Number>       ! Sets the current material
//    translateLight     <Number>       ! Translate our current light to a new position (BEFORE DRAWING)
//
// Events:
//    SET_COLOR         (Color)         ! Sets color
//    SET_COLOR         (Identifier)    ! Sets color by name
//    SET_LINEWIDTH     (Number)        ! Sets line width
//    SET_FLASHRATE     (Number)        ! Sets flash rate
//    SET_VISIBILITY    (Number)        ! Sets visibility flag
//
// Public methods: Base class public methods, plus ...
//
//      Display* getDisplay()
//          Returns a pointer to our Display container.  All Graphics MUST
//          have a Display as a parent or (great) grand parent container.
//
//      draw()
//          Draw the graphic object, and all its components, with the defined
//          attributes (e.g., color, line width, transformation matrix).
//
//      drawFunc()
//          User function to draw the object.
//
//      bool isPostDrawComponents()
//          Returns true if the subcomponents are drawn last.  By default,
//          our subcomponents' draw() functions are called before our drawFunc()
//          is called.  if isPostDrawComponents() returns true, the subcomponents'
//          draw() functions are called after our drawFunc() is called.
//
//      cursor(int* ln, int* cp)
//          Returns true if text cursor should be seen within this
//          object and the position of the cursor. 
//
//
//      Color* getColor()
//      setColor(Color* msg)
//      setColor(Identifier* msg)
//          Gets/Sets the object's color attribute.  Argument types can be Color
//          or Identifier.  The Identifier argument provides a color name that is
//          used to lookup the Color from the color table.
//      setColor(Number* num)
//          Sets a color rotary object, based on the value passed in.. see basicGL/ColorRotary.h for
//          how to set up a list of colors and breakpoints.
//
//
//
// Matrix Functions
//  matrixIsActive()
//    Returns true if we have an active matrix.
//
//  osg::Matrix& getMatrix()
//    Returns the transformation matrix.
//
//      lcRotate(LCreal a)
//      lcRotate(LCreal x, LCreal y, LCreal z, LCreal a)
//          Rotate by 'a' radians about z, or about the x,y,z vector.
//
//      lcTranslate(LCreal x, LCreal y)
//      lcTranslate(LCreal x, LCreal y, LCreal z)
//          Translate x, y, and z.
//
//      lcScale(LCreal s)
//      lcScale(LCreal sx, LCreal sy)
//          Scale by s or by sx, sy.
//
//      lcSaveMatrix()
//      lcRestoreMatrix()
//          Save/Restore current matrix.
//
////Flash Rate Functions
//      int isFlashing()
//          Returns true if this object is flashing
//
//      LCreal getFlashRate()
//      LCreal setFlashRate(LCreal r)
//      setFlashRate(Object* obj)
//          Returns/Sets the object's flash rate.
//
//      LCreal flashTimer();
//      LCreal flashTimer(const LCreal dt)
//          Static functions that Return/Update the flash timer.
//
//      int flashOn()
//      int flashOff()
//          Check flashing visibility.  Based on flash rate and the flash
//          timer, flashOn() returns true if flashing objects are visible.
//
////Display List Functions
//    Gluint getDisplayList()
//      Returns the display list (dlist)
//
//    isDisplayListEnabled()
//      Returns true if display list is enabled.
//
//    isDisplayListDisabled()
//      Returns true if display list is disabled.
//
//    setDisplayList(GLuint v)
//      Sets the display list to v and returns true.
//
//    setDisableDisplayList(bool flg)
//      Disables display list if flg is true and returns true.
//
////Texture functions
//    hasTexture()
//      Returns true if a texture is present.
//
//    GLuint getTexture()
//      Returns the texture.
//
//    GLfloat getStdLineWidth()
//      Returns the standard line width.
//
////Scissor Box Functions
//    haveScissorBoxHave()
//      Returns true if the scissor box has a height and width both greater than 0
//
//    LCreal getScissorX()
//      Returns scissor x value.
//
//    LCreal getScissorWidth()
//      Returns scissor width.
//
//    LCreal getScissorY()
//      Returns scissor y value.
//
//    LCreal getScissorHeight()
//      Returns scissor height.
//
//    setScissorx(LCreal newX)
//      Sets the new scissor x value and returns true.
//
//    setScissorWidth(LCreal newWidth)
//      Sets the scissor width to newWidth and returns true.
//
//    setScissorx(LCreal newY)
//      Sets the new scissor y value and returns true.
//
//    setScissorHeight(LCreal newHeight)
//      Sets the scissor height to newHeight and returns true.
//
////Select (Pick) functions
//      GLuint getSelectName()
//      setSelectName(GLuint v)
//          Gets/Sets the SELECT name (see glPushName()) and (set) returns true.
//
//    Pair* findBySelectName(GLuint name)
//      Finds (and returns) a component by its GL Select (pick) name.
//      (Children first, then grandchildren.)
//
//    GLuint getNewSelectName()
//      Gets a new select name.
//      NOTE: Select name incrementer (for automatic select name generation)
//      Starting value is 0x800000, so all manual select names should be
//      lower than this, so there is no confusion in the pick() routine.
//
//------------------------------------------------------------------------------
class Graphic : public Basic::Component {
    DECLARE_SUBCLASS(Graphic,Basic::Component)

public:
   Graphic();

   Display* getDisplay(); 
   virtual void draw();
   virtual void drawFunc();

   const osg::Vec3* getVertices() const { return vertices; }            // Vertices
   unsigned int getNumberOfVertices() const { return nv; }              // Number of vertices
   bool setVertices(const osg::Vec3* const v, const unsigned int n);    // Sets the vertices list

   const osg::Vec2* getTextureCoord() const { return texCoord; }        // Texture Coordinates (at vertices)
   unsigned int getNumberOfTextureCoords() const { return ntc; }        // Number of texture coordinates
   bool setTextureCoord(const osg::Vec2* const v, const unsigned int n); // Sets the list of texture coordinates

   const osg::Vec3* getNormals() const { return norms; }                // Normals (at vertices)
   unsigned int getNumberOfNormals() const { return nn; }               // Number of Normals
   bool setNormals(const osg::Vec3* const v, const unsigned int n);     // Sets the list of normal vectors

   bool isVisible() const                 { return visible; }           // Checks the visibility attribute.
   bool setVisibility(const bool v);                                    // Sets the visibility attribute.

   GLfloat getLineWidth() const           { return linewidth; }         // Gets the line width attribute.
   bool setLineWidth(const GLfloat v);                                  // Sets the line width attribute.

   // Color functions
   Basic::Color* getColor()                      { return color; }
   const Basic::Color* getColor() const          { return color; }
   const Basic::Identifier* getColorName() const     { return colorName; }
   virtual bool setColor(const Basic::Color* const msg);
   virtual bool setColor(const Basic::Identifier* const msg);
   virtual bool setColor(const Basic::Number* const msg);

   // material functions
   const Basic::Identifier* getMaterialName() const { return materialName; } // returns a pointer to our material name
   BasicGL::Material* getMaterial()                 { return materialObj; } // returns our material object!
   const BasicGL::Material* getMaterial() const     { return materialObj; }
   virtual bool setMaterial(const Basic::Identifier* const msg);
   virtual bool setMaterial(const BasicGL::Material* const msg);

   // Flash rate functions
   bool flashOn() const;
   bool flashOff() const;
   bool isFlashing() const                          { return fRate > 0.0; }
   LCreal getFlashRate() const                      { return fRate; }
   bool setFlashRate(const LCreal r);
   static LCreal flashTimer();
   static LCreal flashTimer(const LCreal dt);

   // Display List functions
   GLuint getDisplayList() const                    { return dlist; }
   bool isDisplayListEnabled() const                { return !noDisplayList; }
   bool isDisplayListDisabled() const               { return noDisplayList; }
   bool setDisplayList(const GLuint v);
   bool setDisableDisplayList(const bool flg);

   // Matrix functions
   bool matrixIsActive()                            { return haveMatrix; }
   const osg::Matrix& getMatrix() const             { return m; }
   void lcRotate(const LCreal a);
   void lcRotate(const LCreal x, const LCreal y, const LCreal z, const LCreal a);
   void lcTranslate(const LCreal, const LCreal);
   void lcTranslate(const LCreal, const LCreal, const LCreal);
   void lcScale(const LCreal);
   void lcScale(const LCreal, const LCreal);

   void lcSaveMatrix();
   void lcRestoreMatrix();
   
   // Texture functions
   bool hasTexture() const                          { return (texture != 0); }
   GLuint getTexture() const                        { return texture; }
   bool setTextureName(const char* newName);
   
   // Set the GL texture id (Make sure you create a texture before you call this function!)   
   virtual bool setTexture(const GLuint newTex);

   // Standard line width
   GLfloat getStdLineWidth();

   // Scissor box functions
   bool haveScissorBoxHave() const                  { return (scissorWidth > 0 && scissorHeight > 0); }
   LCreal getScissorX() const                       { return scissorX; }
   LCreal getScissorWidth() const                   { return scissorY; }
   LCreal getScissorY() const                       { return scissorWidth; }
   LCreal getScissorHeight() const                  { return scissorHeight; }
   bool setScissorX(const LCreal newX);
   bool setScissorWidth(const LCreal newWidth);
   bool setScissorY(const LCreal newY);
   bool setScissorHeight(const LCreal newHeight);

   // Line stippling functions
   bool isStippling()                               { return stipple; }
   GLuint getStippleFactor()                        { return stippleFactor; }
   GLushort getStipplePattern()                     { return stipplePattern; }
   bool setStippling(const bool x);
   bool setStippleFactor(const GLuint x);
   bool setStipplePattern(const GLushort x);
   
   // Light functions
   bool setLightPosition(const LCreal x, const LCreal y, const LCreal z = 1, const LCreal w = 0);
   bool setLightPosition(osg::Vec4& newPos);
   osg::Vec4 getLightPos()                          { return lightPos; }
   

   // Select (pick) functions
   GLuint getSelectName() const                     { return selName; }
   bool setSelectName(const GLuint v);
   virtual Basic::Pair* findBySelectName(const GLuint name);

   // Subcomponent graphics
   bool isPostDrawComponents() const                { return postDraw; }

   virtual bool cursor(int* ln, int* cp) const;
   
   // Select name incrementer (for automatic select name generation)
   // Note:  Starting value is 0x800000, so all manual select names
   // should be lower than this, so there is no confusion in the pick()
   // routine.
   static GLuint getNewSelectName();

   // clipLine2D() -- General purpose 2D line clipping functions
   //    Clips the line defined by two endpoints (ep1 and ep2) against a 2D clip box.
   //    Returns true if all or part of the original line is within the clip box.
   //    Returns false if the original line is outside of the clip box, and therefore
   //    the endpoints are not changed.
   static bool clipLine2D(
         osg::Vec2* ep1,      // Line endpoint #1
         osg::Vec2* ep2,      // Line endpoint #2
         const LCreal minX,   // Clip box min X value
         const LCreal maxX,   // Clip box max X value
         const LCreal minY,   // Clip box min Y value
         const LCreal maxY);  // Clip box max Y value

   // Functions for native LCreal resolutions
   // Since LCreal is deprecated, we're only defining the double functions
   static void lcVertex2(const LCreal x, const LCreal y)                 { glVertex2d(x,y); }
   static void lcVertex3(const LCreal x, const LCreal y, const LCreal z) { glVertex3d(x,y,z); }
   static void lcNormal3(const LCreal x, const LCreal y, const LCreal z) { glNormal3d(x,y,z); }
   static void lcColor3(const LCreal r, const LCreal g, const LCreal b)  { glColor3d(r,g,b); }
   static void lcColor4(const LCreal r, const LCreal g, const LCreal b, const LCreal a)  { glColor4d(r,g,b,a); }

   static void lcVertex2v(const LCreal* v)      { glVertex2dv(v); }
   static void lcVertex3v(const LCreal* v)      { glVertex3dv(v); }
   static void lcNormal3v(const LCreal* v)      { glNormal3dv(v); }
   static void lcColor3v(const LCreal* v)       { glColor3dv(v); }
   static void lcColor4v(const LCreal* v)       { glColor4dv(v); }
   static void lcMultMatrix(const LCreal* m)    { glMultMatrixd(m); }
   static void lcTexCoord2v(const LCreal* v)    { glTexCoord2dv(v); }
   static void lcTexCoord3v(const LCreal* v)    { glTexCoord3dv(v); }
   static void lcTexCoord4v(const LCreal* v)    { glTexCoord4dv(v); }


   // Component interface
   virtual bool event(const int event, Object* const obj = 0);

public: // Exceptions
    class ExpInvalidDisplayPtr : public Object::Exception {
        public:
            ExpInvalidDisplayPtr() : Exception() {}
            virtual const char* getDescription() const     { return "display(): display/screen pointer is not set"; }
    };

protected:
       
    // event handlers
    virtual bool onSetTextureId(const Basic::Number* const msg);
    virtual bool onSetLineWidthEvent(const Basic::Number* const msg);
    virtual bool onSetFlashRateEvent(const Basic::Number* const msg);
    virtual bool onSetVisibilityEvent(const Basic::Number* const msg);

    // set slot functions 
    virtual bool setSlotFlashRate(const Basic::Number* const msg);
    virtual bool setSlotLineWidth(const Basic::Number* const msg);
    virtual bool setSlotSelectName(const Basic::Number* const msg);
    virtual bool setSlotTransformList(Basic::PairStream* msg);      // Set the transformation list
    virtual bool setSlotSingleTransform(Basic::Transform* const msg);
    virtual bool setSlotNoDisplayList(const Basic::Number* const msg);
    virtual bool setSlotSubcomponentsFirst(const Basic::Number* const msg);
    virtual bool setSlotVertices(const Basic::PairStream* const msg);
    virtual bool setSlotNormals(const Basic::PairStream* const msg);
    virtual bool setSlotTexCoord(const Basic::PairStream* const msg);
    virtual bool setSlotMask(const Basic::Number* const msg);
    virtual bool setSlotTextureName(Basic::Identifier* msg);
    virtual bool setSlotScissorX(const Basic::Number* const msg);
    virtual bool setSlotScissorY(const Basic::Number* const msg);
    virtual bool setSlotScissorWidth(const Basic::Number* const msg);
    virtual bool setSlotScissorHeight(const Basic::Number* const msg);
    virtual bool setSlotStippling(const Basic::Number* const msg);
    virtual bool setSlotStippleFactor(const Basic::Number* const msg);
    virtual bool setSlotStipplePattern(const Basic::Number* const msg);
    virtual bool setSlotVisibility(const Basic::Number* const msg);
    virtual bool setSlotTranslateLight(Basic::PairStream* const msg);

    // Basic::Component interface
    virtual void processComponents(            // Process our subcomponent list (which should be other Graphics)
         Basic::PairStream* const list,        // Source list of components
         const std::type_info& filter,         // Type filter
         Basic::Pair* const add = 0,           // Optional pair to add
         Basic::Component* const remove = 0    // Optional subcomponent to remove
       );

private:
    void          initData();
    void          setupMatrix();
    void          setupMaterial();
    
    Basic::PairStream* transforms;  // transformations
    osg::Matrix   m;                // transformation matrix
    osg::Matrix   m1;               // saved 'm'
    bool          haveMatrix;       // Have a transformation matrix flag
    bool          haveMatrix1;      // saved 'haveMatrix'
    
    bool          postDraw;         // Post draw component (child) graphic
    GLuint        texture;          // Texture
    SPtr<Basic::Identifier> texName;   // Texture name

    bool          visible;          // True if this object is visible
    GLfloat       linewidth;        // Linewidth
    GLuint        selName;          // Select name
    LCreal        fRate;            // Flash rate

    Basic::Color*  color;           // Color
    Basic::Identifier*  colorName;  // Color name (if from color table)

    osg::Vec3*       vertices;      // Vertices
    unsigned int     nv;            // Number of vertices

    osg::Vec2*       texCoord;      // Texture Coordinates
    unsigned int     ntc;           // Number of texture coordinates

    osg::Vec3*       norms;         // normals at each vertices
    unsigned int     nn;            // number of normal vectors

    BasicGL::Display* displayPtr; // Points to our Display
    
    GLuint         dlist;            // Possible Display list
    bool           noDisplayList;    // True to disable Display List

    LCreal        scissorX;         // Left edge of the scissor box
    LCreal        scissorY;         // Bottom edge of the scissor box
    LCreal        scissorWidth;     // How far to extend the scissor horizontally
    LCreal        scissorHeight;    // How far to extend the scissor vertically

    static LCreal fTimer;           // Flash control timer
    static GLuint autoSelName;      // our automatic select name counter

    bool          stipple;          // line stipple flag (line, lineloop, circle (not filled))
    GLuint        stippleFactor;    // line stipple factor (multiplier for each bit in line stipple pattern)
    GLushort      stipplePattern;   // line stipple pattern (16-bit pattern for which fragments of line to draw)
    bool          mask;             // are we masking?
    Basic::Identifier* materialName;   // our material name
    BasicGL::Material* materialObj;   // material object, if we have one.
    
    osg::Vec4 lightPos;             // light position relative to us (default is leave it where it was)
    bool lightMoved;                // our light is moving!
};

inline GLuint Graphic::getNewSelectName()
{
    return autoSelName++;
}

inline void Graphic::lcSaveMatrix()
{
    m1 = m;
    haveMatrix1 = haveMatrix;
}

inline void Graphic::lcRestoreMatrix()
{
    m = m1;
    haveMatrix = haveMatrix1;
}

inline void Graphic::lcRotate(const LCreal a)
{
    osg::Matrix rr;
    rr.makeRotate(a, 0.0f, 0.0f, 1.0f);
    m.preMult(rr);
    haveMatrix = true;
}

inline void Graphic::lcRotate(const LCreal x, const LCreal y, const LCreal z, const LCreal a)
{
    osg::Matrix rr;
    rr.makeRotate(a, x, y, z);
    m.preMult(rr);
    haveMatrix = true;
}

inline void Graphic::lcScale(const LCreal s)
{
    osg::Matrix ss;
    ss.makeScale(s,s,s);
    m.preMult(ss);
    haveMatrix = true;
}

inline void Graphic::lcScale(const LCreal sx, const LCreal sy)
{
    osg::Matrix ss;
    ss.makeScale(sx,sy,1.0f);
    m.preMult(ss);
    haveMatrix = true;
}

inline void Graphic::lcTranslate(const LCreal x, const LCreal y)
{
    osg::Matrix tt;
    tt.makeTranslate(x,y,0.0f);
    m.preMult(tt);
    haveMatrix = true;
}

inline void Graphic::lcTranslate(const LCreal x, const LCreal y, const LCreal z)
{
    osg::Matrix tt;
    tt.makeTranslate(x,y,z);
    m.preMult(tt);
    haveMatrix = true;
}

} // End BasicGL namespace
} // End Eaagles namespace

#endif
