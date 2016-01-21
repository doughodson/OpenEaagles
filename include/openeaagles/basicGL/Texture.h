//------------------------------------------------------------------------------
// Class: Texture
//------------------------------------------------------------------------------
#ifndef __oe_BasicGL_Texture_H__
#define __oe_BasicGL_Texture_H__

#include "openeaagles/basicGL/Image.h"

namespace oe {
   namespace Basic { class Number; class Identifier; }

namespace BasicGL {

//------------------------------------------------------------------------------
// Class: Texture
// Base class: Object > Texture
//
// Description: Top level texture class for general (simple) 2D textures.
//
// NOTE: Currently reads only simple, uncompressed Microsoft Bitmap (BMP) files.
//
// NOTE: The width & height of the bitmap is truncated to powers of 2.
// Factory name: Texture
//
// Slots:
//    width       <Number>       ! Size of the texture map (default: 256)
//    height      <Number>       ! Size of the texture map.(default: 256)
//    redScale    <Number>       ! for glPixelTransferf(GL_RED_SCALE) (default: 1.0)
//    redBias     <Number>       ! for glPixelTransferf(GL_RED_BIAS) (default: 0.0)
//    greenScale  <Number>       ! for glPixelTransferf(GL_GREEN_SCALE) (default: 1.0)
//    greenBias   <Number>       ! for glPixelTransferf(GL_GREEN_BIAS) (default: 0.0)
//    blueScale   <Number>       ! for glPixelTransferf(GL_BLUE_SCALE) (default: 1.0)
//    blueBias    <Number>       ! for glPixelTransferf(GL_BLUE_BIAS) (default: 0.0)
//    wrapS       <Identifier>   ! for glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S) (default: GL_REPEAT)
//    wrapT       <Identifier>   ! for glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T) (default: GL_REPEAT)
//    magFilter   <Identifier>   ! for glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER) (default: GL_NEAREST)
//    minFilter   <Identifier>   ! for glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER) (default: GL_NEAREST)
//
//
// Public member functions:
//
//   GLunit getTexture()
//      Returns the texture.
//
// glTexParameteri() Attributes
//   int getWrapS()
//   int getWrapT()
//      Returns the wrapS/wrapT value.
//
//   int getMagFilter()
//      Returns the magFilter value.
//
//   int getMinFilter()
//      Returns the minFilter value.
//
//   bool setWrapS(int v)
//      Sets the wrapS value to v and returns true if successful.
//
//   bool setWrapT(int v)
//      Sets the wrapT value to v and returns true if successful.
//
//   bool setMagFilter(int v)
//      Sets the magFilter value to v and returns true if successful.
//
//   bool setMinFilter(int v)
//      Sets the minFilter value to v and returns true if successful.
//
// glPixelTransferf() Attributes
//   LCreal getRedScale()
//      Returns the redScale value.
//
//   LCreal getRedBias()
//      Returns the redBias value.
//
//   LCreal getGreenScale()
//      Returns the greenScale value.
//
//   LCreal getGreenBias()
//      Returns the greenBias value.
//
//   LCreal getBlueScale()
//      Returns the blueScale value.
//
//   LCreal getBlueBias()
//      Returns the blueBias value.
//
//   bool setRedScale(LCreal s)
//      Sets the redScale value to s and returns true if successful.
//
//   bool setRedBias(LCreal b)
//      Sets the redBias value to b and returns true if successful.
//
//   bool setGreenScale(LCreal s)
//      Sets the greenScale value to s and returns true if successful.
//
//   bool setGreenBias(LCreal b)
//      Sets the greenBias value to b and returns true if successful.
//
//   bool setBlueScale(LCreal s)
//      Sets the blueScale value to s and returns true if successful.
//
//   bool setBlueBias(LCreal b)
//      Sets the blueBias value to b and returns true if successful.
//
//------------------------------------------------------------------------------
class Texture : public Image {
    DECLARE_SUBCLASS(Texture,Image)

public:
   Texture();

   // texture id
   GLuint getTexture() const;
   virtual void setTexture(const GLuint t);

   // glTexParameteri() Attributes
   int getWrapS() const;
   int getWrapT() const;
   int getMagFilter() const;
   int getMinFilter() const;
   virtual bool setWrapS(const int v);
   virtual bool setWrapT(const int v);
   virtual bool setMagFilter(const int v);
   virtual bool setMinFilter(const int v);

   // glPixelTransferf() Attributes
   GLfloat getRedScale() const;
   GLfloat getRedBias() const;
   GLfloat getGreenScale() const;
   GLfloat getGreenBias() const;
   GLfloat getBlueScale() const;
   GLfloat getBlueBias() const;
   virtual bool setRedScale(const GLfloat s);
   virtual bool setRedBias(const GLfloat b);
   virtual bool setGreenScale(const GLfloat s);
   virtual bool setGreenBias(const GLfloat b);
   virtual bool setBlueScale(const GLfloat s);
   virtual bool setBlueBias(const GLfloat b);

   virtual void loadTexture();

   bool setWidth(const unsigned int w) override;
   bool setHeight(const unsigned int h) override;

protected:
   // Slot functions
   bool setSlotWidth(const Basic::Number* const size);
   bool setSlotHeight(const Basic::Number* const size);
   bool setSlotRedScale(const Basic::Number* const s);
   bool setSlotRedBias(const Basic::Number* const b);
   bool setSlotGreenScale(const Basic::Number* const s);
   bool setSlotGreenBias(const Basic::Number* const b);
   bool setSlotBlueScale(const Basic::Number* const s);
   bool setSlotBlueBias(const Basic::Number* const b);
   bool setSlotWrapS(const Basic::Identifier* const v);
   bool setSlotWrapT(const Basic::Identifier* const v);
   bool setSlotMagFilter(const Basic::Identifier* const v);
   bool setSlotMinFilter(const Basic::Identifier* const v);

private:
   void initData();

   GLuint texture;                 // open GL texture identifier

   // glTexParameteri() Attributes
   int         wrapS;              // GL_REPEAT (default) or GL_CLAMP
   int         wrapT;              // GL_REPEAT (default) or GL_CLAMP
   int         magFilter;          // GL_NEAREST (default) or GL_LINEAR
   int         minFilter;          // GL_NEAREST (default) or GL_LINEAR

   // glPixelTransferf() Attributes
   GLfloat      redScale;
   GLfloat      redBias;
   GLfloat      greenScale;
   GLfloat      greenBias;
   GLfloat      blueScale;
   GLfloat      blueBias;
};

//------------------------------------------------------------------------------
// Inline functions
//------------------------------------------------------------------------------

inline GLuint Texture::getTexture() const          { return texture; }
inline int Texture::getWrapS() const               { return wrapS; }
inline int Texture::getWrapT() const               { return wrapT; }
inline int Texture::getMagFilter() const           { return magFilter; }
inline int Texture::getMinFilter() const           { return minFilter; }
inline GLfloat Texture::getRedScale() const        { return redScale; }
inline GLfloat Texture::getRedBias() const         { return redBias; }
inline GLfloat Texture::getGreenScale() const      { return greenScale; }
inline GLfloat Texture::getGreenBias() const       { return greenBias; }
inline GLfloat Texture::getBlueScale() const       { return blueScale; }
inline GLfloat Texture::getBlueBias() const        { return blueBias; }

} // End BasicGL namespace
} // End oe namespace

#endif
