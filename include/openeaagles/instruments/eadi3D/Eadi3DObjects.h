//------------------------------------------------------------------------------
// Class:  Eadi3DObjects
//------------------------------------------------------------------------------
#ifndef __oe_Instruments_Eadi3DObjects_H__
#define __oe_Instruments_Eadi3DObjects_H__

#include "openeaagles/basic/Object.h"
#include "openeaagles/instruments/eadi3D/IrisGLCompat.h"

namespace oe {
namespace Instruments {

//------------------------------------------------------------------------------
// Class:  Eadi3DObjects
// Base class:  Object -> Eadi3DObjects
//
// Description:  Eadi3D font
//
// public methods (member functions):
//      (All of the classes derived from Objects have the same public methods.)
//
//      static GLenum createEadi3DObjects(GLuint fontBase)
//          Produces the stroke font map.
//
//------------------------------------------------------------------------------
class Eadi3DObjects : public Basic::Object {
    DECLARE_SUBCLASS(Eadi3DObjects,Basic::Object)

public:
    Eadi3DObjects(float radius = 1.625);

    enum EadiObj {
        FONT_0 = 0,    // Desc: Character '0' 
        FONT_1 = 1,    // Desc: Character '1' 
        FONT_2 = 2,    // Desc: Character '2' 
        FONT_3 = 3,    // Desc: Character '3' 
        FONT_4 = 4,    // Desc: Character '4' 
        FONT_5 = 5,    // Desc: Character '5' 
        FONT_6 = 6,    // Desc: Character '6' 
        FONT_7 = 7,    // Desc: Character '7' 
        FONT_8 = 8,    // Desc: Character '8' 
        FONT_9 = 9,    // Desc: Character '9' 
        PUNC05 = 45,    // Desc: Character '-' 
        PUNC06 = 46,    // Desc: Character '.' 
        PUNC07 = 47,    // Desc: Character '/' 
        FONT_A = 65,    // Desc: Character 'A' 
        FONT_B = 66,    // Desc: Character 'B' 
        FONT_C = 67,    // Desc: Character 'C' 
        FONT_D = 68,    // Desc: Character 'D' 
        FONT_E = 69,    // Desc: Character 'E' 
        FONT_F = 70,    // Desc: Character 'F' 
        FONT_G = 71,    // Desc: Character 'G' 
        FONT_H = 72,    // Desc: Character 'H' 
        FONT_I = 73,    // Desc: Character 'I' 
        FONT_J = 74,    // Desc: Character 'J' 
        FONT_K = 75,    // Desc: Character 'K' 
        FONT_L = 76,    // Desc: Character 'L' 
        FONT_M = 77,    // Desc: Character 'M' 
        FONT_N = 78,    // Desc: Character 'N' 
        FONT_O = 79,    // Desc: Character 'O' 
        FONT_P = 80,    // Desc: Character 'P' 
        FONT_Q = 81,    // Desc: Character 'Q' 
        FONT_R = 82,    // Desc: Character 'R' 
        FONT_S = 83,    // Desc: Character 'S' 
        FONT_T = 84,    // Desc: Character 'T' 
        FONT_U = 85,    // Desc: Character 'U' 
        FONT_V = 86,    // Desc: Character 'V' 
        FONT_W = 87,    // Desc: Character 'W' 
        FONT_X = 88,    // Desc: Character 'X' 
        FONT_Y = 89,    // Desc: Character 'Y' 
        FONT_Z = 90,    // Desc: Character 'Z' 
        UP_ARROW = 100,
        DOWN_ARROW,
        GROUND_POINTER,
        BALL_FRAME,
        AC_REF,
        BORDER,
        HEADING_SCALE,
        LEFT_CARET,
        UP_CARET,
        UP_TRIANGLE,
        GLIDESLOPE,
        LOCALIZER,
        TURN_RATE,
        SLIP_IND,
        SLIP_BALL,
        ALPHA_SYMB,
        GLOBE,
        GS_TEXT1,
        GS_TEXT2,
        LOC_TEXT1,
        LOC_TEXT2,
        GROUND_POINTER_2,
        AC_REF2,
        UP_ARROW_OUTLINE,
        DOWN_ARROW_OUTLINE,
        UP_ARROW_SKINNY,
        DOWN_ARROW_SKINNY,
        __MAX_EADI_OBJECTS__
    };

    void makeObjects(void);
    void drawObj(EadiObj obj);
    IrisGLCompat irisgl;

    // drawing functions
    void makeFTZRO(unsigned int obj);
    void makeFT1(unsigned int obj);
    void makeFT2(unsigned int obj);
    void makeFT3(unsigned int obj);
    void makeFT4(unsigned int obj);
    void makeFT5(unsigned int obj);
    void makeFT6(unsigned int obj);
    void makeFT7(unsigned int obj);
    void makeFT8(unsigned int obj);
    void makeFT9(unsigned int obj);
    void makeFTMIN(unsigned int obj);
    void makeFTPER(unsigned int obj);
    void makeFTSLH(unsigned int obj);
    void makeFTA(unsigned int obj);
    void makeFTB(unsigned int obj);
    void makeFTC(unsigned int obj);
    void makeFTD(unsigned int obj);
    void makeFTE(unsigned int obj);
    void makeFTF(unsigned int obj);
    void makeFTG(unsigned int obj);
    void makeFTH(unsigned int obj);
    void makeFTI(unsigned int obj);
    void makeFTJ(unsigned int obj);
    void makeFTK(unsigned int obj);
    void makeFTL(unsigned int obj);
    void makeFTM(unsigned int obj);
    void makeFTN(unsigned int obj);
    void makeFTO(unsigned int obj);
    void makeFTP(unsigned int obj);
    void makeFTQ(unsigned int obj);
    void makeFTR(unsigned int obj);
    void makeFTS(unsigned int obj);
    void makeFTT(unsigned int obj);
    void makeFTU(unsigned int obj);
    void makeFTV(unsigned int obj);
    void makeFTW(unsigned int obj);
    void makeFTX(unsigned int obj);
    void makeFTY(unsigned int obj);
    void makeFTZ(unsigned int obj);

private:
    void makeFont(void);
    void makeGlobe(void);
    void drawArrow(float bAngle, float s, float c);
    void drawArrow2(float bAngle, float s, float c);

    unsigned int base;
    float radius;
    bool made;
};

} // End Instruments namespace
} // End oe namespace

#endif
