//------------------------------------------------------------------------------
// Class: StrokeFont
// Description: Modified version of SGI's stroke font
//------------------------------------------------------------------------------
#include "openeaagles/graphics/StrokeFont.h"

namespace oe {
namespace graphics {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(StrokeFont,"StrokeFont")
EMPTY_SERIALIZER(StrokeFont)

// SGI font scaled so six stroke font points (units used below) is 1 screen unit
static const double XSCALE = (1.0/6.0);
static const double YSCALE = (1.0/6.0);

// Default font sizes
const double defaultFontWidth = 6.0f * XSCALE;
const double defaultFontHeight = 9.0f * YSCALE;

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
StrokeFont::StrokeFont()
{
    STANDARD_CONSTRUCTOR()

    setFontWidth( defaultFontWidth );
    setFontHeight( defaultFontHeight );
    setCharacterSpacing( defaultFontWidth );
    setLineSpacing( defaultFontHeight );
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void StrokeFont::copyData(const StrokeFont& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void StrokeFont::deleteData()
{
}

//------------------------------------------------------------------------------
// outputText() -- Text output routines
//------------------------------------------------------------------------------
void StrokeFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    const int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;

    // Set the base
    glListBase(getBase());

    // output the text
    glPushMatrix();
    GLfloat lw = 0;
    glGetFloatv(GL_LINE_WIDTH, &lw);
    glLineWidth(2.0f);
    glTranslated(x,y,0.0);
    glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
    if (vf) {
        // Vertical text
        char cc[2];
        cc[1] = '\0';
        GLdouble dy = getLineSpacing();
        if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
        for (int i = 0; i < nn; i++) {
            cc[0] = cbuf[i];
            glPushMatrix();
            glTranslated(0.0, -(dy * i), 0.0);
            glCallLists(1, GL_UNSIGNED_BYTE, cc);
            glPopMatrix();
        }
    }
    else {
        // Normal text
        glCallLists(nn, GL_UNSIGNED_BYTE, cbuf);
    }
    glLineWidth(lw);
    glPopMatrix();
}

void StrokeFont::outputText(const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN];
    const int nn = xferChars(cbuf,MSG_BUF_LEN,txt,n);
    if (nn <= 0) return;

    // Set the base
    glListBase(getBase());

    // output and scale the text
    glPushMatrix();
    GLfloat lw = 0;
    glGetFloatv(GL_LINE_WIDTH, &lw);
    glLineWidth(2.0f);
    glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
    if (vf) {
        // Vertical text
        char cc[2];
        cc[1] = '\0';
        GLdouble dy = getLineSpacing();
        if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
        for (int i = 0; i < nn; i++) {
            cc[0] = cbuf[i];
            glPushMatrix();
            glTranslated(0.0, -(dy * i), 0.0);
            glCallLists(1, GL_UNSIGNED_BYTE, cc);
            glPopMatrix();
        }
    }
    else {
        // Normal text
        glCallLists(nn, GL_UNSIGNED_BYTE, cbuf);
    }
    glLineWidth(lw);
    glPopMatrix();
}

//------------------------------------------------------------------------------
// loadFont() --
//------------------------------------------------------------------------------
void StrokeFont::loadFont()
{
    if (isLoaded()) return;

    // create the stroke font
    setBase( glGenLists(256) );
    createStrokeFont( getBase() );
    setFontLoaded();
}


//------------------------------------------------------------------------------
// Default stroke font ---
//------------------------------------------------------------------------------

/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */


enum {
    FONT_BEGIN = 1,
    FONT_NEXT,
    FONT_END,
    FONT_ADVANCE
};

static const unsigned int MAX_STROKES = 256;
static const unsigned int END_OF_LIST = 256;

// Displays so two (2) points on Y is our origin
static const double XOFFSET = 0.0;
static const double YOFFSET = 2.0;


static GLdouble strokeFont[][1+MAX_STROKES*3] = {
   {
      1, // down arrow
      FONT_BEGIN, 0, 4,
      FONT_NEXT, 2, 2,
      FONT_END, 4, 4,
      FONT_BEGIN, 2, 8,
      FONT_END, 2, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      2, // Alpha symbol
      FONT_BEGIN, 0, 3,
      FONT_NEXT, 0, 5,
      FONT_NEXT, 1, 6,
      FONT_NEXT, 2, 6,
      FONT_NEXT, 3, 5,
      FONT_END, 4, 6,
      FONT_BEGIN, 3, 5,
      FONT_NEXT, 3, 3,
      FONT_END, 4, 2,
      FONT_BEGIN, 3, 3,
      FONT_NEXT, 2, 2,
      FONT_NEXT, 1, 2,
      FONT_END, 0, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      3, // Beta symbol
      FONT_BEGIN, 0, 0,
      FONT_NEXT, 1, 1,
      FONT_NEXT, 1, 5,
      FONT_NEXT, 2, 6,
      FONT_NEXT, 4, 6,
      FONT_NEXT, 5, 5,
      FONT_NEXT, 4, 4,
      FONT_END, 1, 4,
      FONT_BEGIN, 4, 4,
      FONT_NEXT, 5, 3,
      FONT_NEXT, 4, 2,
      FONT_END, 1, 2,
      FONT_ADVANCE, 7, 0
   },
   {
      4, // Up carret
      FONT_BEGIN, 0, 4,
      FONT_NEXT, 2, 6,
      FONT_END, 4, 4,
      FONT_ADVANCE, 6, 0
   },
   {
      5,
      FONT_BEGIN, 0, 5,
      FONT_NEXT, 4, 5,
      FONT_END, 4, 4,
      FONT_ADVANCE, 6, 0
   },
   {
      6, // epsilon symbol
      FONT_BEGIN, 1, 4,
      FONT_END, 3, 4,
      FONT_BEGIN, 3, 6,
      FONT_NEXT, 2, 6,
      FONT_NEXT, 1, 5,
      FONT_NEXT, 1, 3,
      FONT_NEXT, 2, 2,
      FONT_END, 3, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      7, // Pi symbol
      FONT_BEGIN, 1, 2,
      FONT_END, 1, 6,
      FONT_BEGIN, 0, 6,
      FONT_END, 4, 6,
      FONT_BEGIN, 3, 6,
      FONT_END, 3, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      8, // lambda symbol
      FONT_BEGIN, 0, 2,
      FONT_END, 2, 4,
      FONT_BEGIN, 0, 7,
      FONT_NEXT, 0, 6,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
   10, // back space
      FONT_ADVANCE, 0, -9
   },
   {
      11,   // integration symbol
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 1, 1,
      FONT_NEXT, 2, 2,
      FONT_NEXT, 2, 7,
      FONT_NEXT, 3, 8,
      FONT_END, 4, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      12,   // plus (+) over minus (-) symbol
      FONT_BEGIN, 0, 3,
      FONT_END, 4, 3,
      FONT_BEGIN, 4, 6,
      FONT_END, 0, 6,
      FONT_BEGIN, 2, 8,
      FONT_END, 2, 4,
      FONT_ADVANCE, 6, 0
   },
   {
      14,   // infinity symbol
      FONT_BEGIN, 1, 3,
      FONT_NEXT, 2, 4,
      FONT_NEXT, 2, 5,
      FONT_NEXT, 3, 6,
      FONT_NEXT, 4, 5,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 3, 3,
      FONT_END, 2, 4,
      FONT_BEGIN, 2, 5,
      FONT_NEXT, 1, 6,
      FONT_NEXT, 0, 5,
      FONT_NEXT, 0, 4,
      FONT_END, 1, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      15,   //
      FONT_BEGIN, 0, 3,
      FONT_NEXT, 0, 4,
      FONT_NEXT, 1, 5,
      FONT_NEXT, 3, 5,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 4, 6,
      FONT_NEXT, 2, 8,
      FONT_END, 1, 8,
      FONT_BEGIN, 4, 4,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 1, 2,
      FONT_END, 0, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      16,   // rotated right large U
      FONT_BEGIN, 4, 7,
      FONT_NEXT, 1, 7,
      FONT_NEXT, 0, 6,
      FONT_NEXT, 0, 4,
      FONT_NEXT, 1, 3,
      FONT_END, 4, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      17,   // rotated left large U
      FONT_BEGIN, 0, 3,
      FONT_NEXT, 3, 3,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 4, 6,
      FONT_NEXT, 3, 7,
      FONT_END, 0, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      18,   // Upside down small u
      FONT_BEGIN, 0, 4,
      FONT_NEXT, 0, 6,
      FONT_NEXT, 1, 7,
      FONT_NEXT, 3, 7,
      FONT_NEXT, 4, 6,
      FONT_END, 4, 4,
      FONT_ADVANCE, 6, 0
   },
   {
      19,   // small u
      FONT_BEGIN, 0, 7,
      FONT_NEXT, 0, 5,
      FONT_NEXT, 1, 4,
      FONT_NEXT, 3, 4,
      FONT_NEXT, 4, 5,
      FONT_END, 4, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      20,   // Upside down large A
      FONT_BEGIN, 0, 8,
      FONT_NEXT, 2, 2,
      FONT_END, 4, 8,
      FONT_BEGIN, 3, 6,
      FONT_END, 1, 6,
      FONT_ADVANCE, 6, 0
   },
   {
      21,   // Backward E
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 4, 2,
      FONT_NEXT, 4, 5,
      FONT_END, 1, 5,
      FONT_BEGIN, 0, 8,
      FONT_NEXT, 4, 8,
      FONT_END, 4, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      22,   // small x inside circle
      FONT_BEGIN, 0, 4,
      FONT_NEXT, 0, 6,
      FONT_NEXT, 1, 7,
      FONT_NEXT, 3, 7,
      FONT_NEXT, 4, 6,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 3, 3,
      FONT_NEXT, 1, 3,
      FONT_END, 0, 4,
      FONT_BEGIN, 1, 4,
      FONT_END, 3, 6,
      FONT_BEGIN, 1, 6,
      FONT_END, 3, 4,
      FONT_ADVANCE, 6, 0
   },
   {
      23,   // right arrow over left arrow
      FONT_BEGIN, 2, 0,
      FONT_NEXT, 0, 2,
      FONT_NEXT, 4, 6,
      FONT_END, 2, 8,
      FONT_BEGIN, 0, 6,
      FONT_END, 4, 6,
      FONT_BEGIN, 0, 2,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      24,   // underline
      FONT_BEGIN, 0, 0,
      FONT_END, 4, 0,
      FONT_ADVANCE, 6, 0
   },
   {
      25,   // right arrow
      FONT_BEGIN, 2, 3,
      FONT_NEXT, 4, 5,
      FONT_END, 2, 7,
      FONT_BEGIN, 0, 5,
      FONT_END, 4, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      26,   // tilda
      FONT_BEGIN, 0, 8,
      FONT_NEXT, 1, 9,
      FONT_NEXT, 2, 9,
      FONT_NEXT, 2, 8,
      FONT_NEXT, 3, 8,
      FONT_END, 4, 9,
      FONT_ADVANCE, 6, 0
   },
   {
   27,   // not equal symbol
      FONT_BEGIN, 0, 1,
      FONT_END, 4, 7,
      FONT_BEGIN, 4, 5,
      FONT_END, 0, 5,
      FONT_BEGIN, 0, 3,
      FONT_END, 4, 3,
      FONT_ADVANCE, 6, 1
   },
   {
      28,   // less than or equal symbol
      FONT_BEGIN, 1, 2,
      FONT_END, 3, 2,
      FONT_BEGIN, 3, 4,
      FONT_NEXT, 1, 6,
      FONT_END, 3, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      29,   // greater than or equal symbol
      FONT_BEGIN, 1, 2,
      FONT_END, 3, 2,
      FONT_BEGIN, 1, 4,
      FONT_NEXT, 3, 6,
      FONT_END, 1, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      30,   // three horiz lines
      FONT_BEGIN, 0, 3,
      FONT_END, 4, 3,
      FONT_BEGIN, 4, 5,
      FONT_END, 0, 5,
      FONT_BEGIN, 0, 7,
      FONT_END, 4, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      31,   // small v
      FONT_BEGIN, 0, 6,
      FONT_NEXT, 2, 4,
      FONT_END, 4, 6,
      FONT_ADVANCE, 6, 0
   },
   {
      32,   // space
      FONT_ADVANCE, 6, 0
   },
   {
      33,   // !
      FONT_BEGIN, 2, 1,
      FONT_END, 2, 2,
      FONT_BEGIN, 2, 4,
      FONT_END, 2, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      34,   // "
      FONT_BEGIN, 1, 7,
      FONT_END, 1, 9,
      FONT_BEGIN, 3, 9,
      FONT_END, 3, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      35,   // #
      FONT_BEGIN, 1, 2,
      FONT_END, 1, 7,
      FONT_BEGIN, 3, 7,
      FONT_END, 3, 2,
      FONT_BEGIN, 4, 3,
      FONT_END, 0, 3,
      FONT_BEGIN, 0, 6,
      FONT_END, 4, 6,
      FONT_ADVANCE, 6, 0
   },
   {
      36,   // $
      FONT_BEGIN, 2, 1,
      FONT_END, 2, 9,
      FONT_BEGIN, 4, 7,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 0, 6,
      FONT_NEXT, 1, 5,
      FONT_NEXT, 3, 5,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 4, 2,
      FONT_NEXT, 3, 1,
      FONT_NEXT, 1, 1,
      FONT_END, 0, 2,
      FONT_ADVANCE, 6, -1
   },
   {
      37,   // %
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 0, 3,
      FONT_NEXT, 4, 7,
      FONT_END, 4, 8,
      FONT_BEGIN, 1, 8,
      FONT_NEXT, 0, 8,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 1, 7,
      FONT_END, 1, 8,
      FONT_BEGIN, 4, 3,
      FONT_NEXT, 5, 3,
      FONT_NEXT, 5, 2,
      FONT_NEXT, 4, 2,
      FONT_END, 5, 2,
      FONT_ADVANCE, 8, 1
   },
   {
      38,   // &
      FONT_BEGIN, 4, 4,
      FONT_NEXT, 2, 2,
      FONT_NEXT, 1, 2,
      FONT_NEXT, 0, 3,
      FONT_NEXT, 0, 4,
      FONT_NEXT, 2, 6,
      FONT_NEXT, 2, 7,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 0, 6,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      39,   // '
      FONT_BEGIN, 0, 7,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 1, 9,
      FONT_NEXT, 2, 9,
      FONT_NEXT, 2, 8,
      FONT_END, 1, 8,
      FONT_ADVANCE, 6, 1
   },
   {
      40,   // (
      FONT_BEGIN, 4, 2,
      FONT_NEXT, 2, 4,
      FONT_NEXT, 2, 6,
      FONT_END, 4, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      41,   // )
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 2, 4,
      FONT_NEXT, 2, 6,
      FONT_END, 0, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      42,   // *
      FONT_BEGIN, 2, 2,
      FONT_END, 2, 8,
      FONT_BEGIN, 0, 7,
      FONT_END, 4, 3,
      FONT_BEGIN, 4, 5,
      FONT_END, 0, 5,
      FONT_BEGIN, 0, 3,
      FONT_END, 4, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      43,   // +
      FONT_BEGIN, 2, 3,
      FONT_END, 2, 7,
      FONT_BEGIN, 0, 5,
      FONT_END, 4, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      44,   // '
      FONT_BEGIN, 0, 1,
      FONT_NEXT, 1, 2,
      FONT_END, 1, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      45,   // -
      FONT_BEGIN, 0, 5,
      FONT_END, 4, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      46,   // .
      FONT_BEGIN, 1, 2,
      FONT_END, 2, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      47,   // '/'
      FONT_BEGIN, 0, 3,
      FONT_END, 4, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      48,   // 0
      FONT_BEGIN, 4, 7,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 0, 3,
      FONT_NEXT, 1, 2,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 4, 3,
      FONT_END, 4, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      49,   // 1
      FONT_BEGIN, 1, 2,
      FONT_END, 3, 2,
      FONT_BEGIN, 2, 2,
      FONT_NEXT, 2, 8,
      FONT_END, 1, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      50,   // 2
      FONT_BEGIN, 0, 7,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 4, 6,
      FONT_NEXT, 0, 2,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      51,   // 3
      FONT_BEGIN, 0, 3,
      FONT_NEXT, 1, 2,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 3, 5,
      FONT_END, 2, 5,
      FONT_BEGIN, 3, 5,
      FONT_NEXT, 4, 6,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 1, 8,
      FONT_END, 0, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      52,   // 4
      FONT_BEGIN, 3, 2,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 0, 5,
      FONT_NEXT, 0, 4,
      FONT_END, 4, 4,
      FONT_ADVANCE, 6, 0
   },
   {
      53,   // 5
      FONT_BEGIN, 0, 3,
      FONT_NEXT, 1, 2,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 4, 5,
      FONT_NEXT, 3, 6,
      FONT_NEXT, 0, 6,
      FONT_NEXT, 0, 8,
      FONT_END, 4, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      54,   // 6
      FONT_BEGIN, 0, 5,
      FONT_NEXT, 0, 3,
      FONT_NEXT, 1, 2,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 3, 5,
      FONT_NEXT, 0, 5,
      FONT_NEXT, 0, 6,
      FONT_NEXT, 2, 8,
      FONT_END, 3, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      55,   // 7
      FONT_BEGIN, 0, 8,
      FONT_NEXT, 4, 8,
      FONT_NEXT, 2, 4,
      FONT_END, 2, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      56,   // 8
      FONT_BEGIN, 1, 5,
      FONT_NEXT, 0, 4,
      FONT_NEXT, 0, 3,
      FONT_NEXT, 1, 2,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 3, 5,
      FONT_NEXT, 1, 5,
      FONT_NEXT, 0, 6,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 4, 6,
      FONT_END, 3, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      57,   // 9
      FONT_BEGIN, 1, 2,
      FONT_NEXT, 2, 2,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 0, 6,
      FONT_NEXT, 1, 5,
      FONT_END, 4, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      58,   // :
      FONT_BEGIN, 0, 3,
      FONT_END, 1, 3,
      FONT_BEGIN, 1, 6,
      FONT_END, 0, 6,
      FONT_ADVANCE, 6, 0
   },
   {
      59,   // ;
      FONT_BEGIN, 0, 1,
      FONT_NEXT, 1, 2,
      FONT_END, 1, 3,
      FONT_BEGIN, 1, 6,
      FONT_END, 0, 6,
      FONT_ADVANCE, 6, 0
   },
   {
      60,   // <
      FONT_BEGIN, 3, 3,
      FONT_NEXT, 1, 5,
      FONT_END, 3, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      61,   // =
      FONT_BEGIN, 0, 4,
      FONT_END, 4, 4,
      FONT_BEGIN, 4, 6,
      FONT_END, 0, 6,
      FONT_ADVANCE, 6, 0
   },
   {
      62,   // >
      FONT_BEGIN, 1, 7,
      FONT_NEXT, 3, 5,
      FONT_END, 1, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      63,   // ?
      FONT_BEGIN, 1, 2,
      FONT_END, 2, 2,
      FONT_BEGIN, 2, 4,
      FONT_NEXT, 2, 5,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 1, 8,
      FONT_END, 0, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      64,   // @
      FONT_BEGIN, 3, 2,
      FONT_NEXT, 1, 2,
      FONT_NEXT, 0, 3,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 2, 4,
      FONT_NEXT, 2, 6,
      FONT_END, 4, 6,
      FONT_ADVANCE, 6, 0
   },
   {
      65,   // A
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 4, 7,
      FONT_END, 4, 2,
      FONT_BEGIN, 0, 5,
      FONT_END, 4, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      66,   // B
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 3, 5,
      FONT_END, 0, 5,
      FONT_BEGIN, 3, 5,
      FONT_NEXT, 4, 6,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 0, 8,
      FONT_END, 0, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      67,   // C
      FONT_BEGIN, 4, 7,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 0, 3,
      FONT_NEXT, 1, 2,
      FONT_NEXT, 3, 2,
      FONT_END, 4, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      68,   // D
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 3, 8,
      FONT_END, 0, 8,
      FONT_BEGIN, 1, 8,
      FONT_END, 1, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      69,   // E
      FONT_BEGIN, 4, 2,
      FONT_NEXT, 0, 2,
      FONT_NEXT, 0, 8,
      FONT_END, 4, 8,
      FONT_BEGIN, 3, 5,
      FONT_END, 0, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      70,   // F
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 0, 8,
      FONT_END, 4, 8,
      FONT_BEGIN, 3, 5,
      FONT_END, 0, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      71,   // G
      FONT_BEGIN, 3, 4,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 1, 2,
      FONT_NEXT, 0, 3,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 3, 8,
      FONT_END, 4, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      72,   // H
      FONT_BEGIN, 0, 2,
      FONT_END, 0, 8,
      FONT_BEGIN, 0, 5,
      FONT_END, 4, 5,
      FONT_BEGIN, 4, 8,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      73,   // I
      FONT_BEGIN, 1, 2,
      FONT_END, 3, 2,
      FONT_BEGIN, 2, 2,
      FONT_END, 2, 8,
      FONT_BEGIN, 1, 8,
      FONT_END, 3, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      74,   // J
      FONT_BEGIN, 0, 3,
      FONT_NEXT, 1, 2,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 4, 3,
      FONT_END, 4, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      75,   // K
      FONT_BEGIN, 0, 2,
      FONT_END, 0, 8,
      FONT_BEGIN, 4, 8,
      FONT_NEXT, 1, 5,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      76,   // L
      FONT_BEGIN, 0, 8,
      FONT_NEXT, 0, 2,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      77,   // M
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 0, 8,
      FONT_NEXT, 2, 6,
      FONT_NEXT, 4, 8,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      78,   // N
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 0, 8,
      FONT_NEXT, 4, 2,
      FONT_END, 4, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      79,   // O
      FONT_BEGIN, 0, 3,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 1, 2,
      FONT_END, 0, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      80,   // P
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 0, 8,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 4, 6,
      FONT_NEXT, 3, 5,
      FONT_END, 0, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      81,   // Q
      FONT_BEGIN, 0, 3,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 2, 2,
      FONT_NEXT, 1, 2,
      FONT_END, 0, 3,
      FONT_BEGIN, 2, 5,
      FONT_END, 4, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      82,   // R
      FONT_BEGIN, 0, 2,
      FONT_NEXT, 0, 8,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 4, 7,
      FONT_NEXT, 4, 6,
      FONT_NEXT, 3, 5,
      FONT_END, 0, 5,
      FONT_BEGIN, 1, 5,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      83,   // S
      FONT_BEGIN, 4, 7,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 0, 7,
      FONT_NEXT, 0, 6,
      FONT_NEXT, 1, 5,
      FONT_NEXT, 3, 5,
      FONT_NEXT, 4, 4,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 1, 2,
      FONT_END, 0, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      84,   // T
      FONT_BEGIN, 0, 8,
      FONT_END, 4, 8,
      FONT_BEGIN, 2, 8,
      FONT_END, 2, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      85,   // U
      FONT_BEGIN, 0, 3,
      FONT_END, 0, 8,
      FONT_BEGIN, 4, 8,
      FONT_NEXT, 4, 3,
      FONT_NEXT, 3, 2,
      FONT_NEXT, 1, 2,
      FONT_END, 0, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      86,   // V
      FONT_BEGIN, 0, 8,
      FONT_NEXT, 0, 5,
      FONT_NEXT, 2, 2,
      FONT_NEXT, 4, 5,
      FONT_END, 4, 8,
      FONT_ADVANCE, 6, 0
   },
   {
      87,   // W
      FONT_BEGIN, 0, 2,
      FONT_END, 0, 8,
      FONT_BEGIN, 4, 8,
      FONT_NEXT, 4, 2,
      FONT_NEXT, 2, 4,
      FONT_END, 0, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      88,   // X
      FONT_BEGIN, 0, 2,
      FONT_END, 4, 8,
      FONT_BEGIN, 0, 8,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      89,   // Y
      FONT_BEGIN, 2, 2,
      FONT_NEXT, 2, 5,
      FONT_END, 0, 8,
      FONT_BEGIN, 4, 8,
      FONT_END, 2, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      90,   // Z
      FONT_BEGIN, 0, 8,
      FONT_NEXT, 4, 8,
      FONT_NEXT, 0, 2,
      FONT_END, 4, 2,
      FONT_ADVANCE, 6, 0
   },
   {
      91,   // [
      FONT_BEGIN, 3, 1,
      FONT_NEXT, 1, 1,
      FONT_NEXT, 1, 9,
      FONT_END, 3, 9,
      FONT_ADVANCE, 6, 0
   },
   {
      92,   // '\'
      FONT_BEGIN, 0, 7,
      FONT_END, 4, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      93,   // ]
      FONT_BEGIN, 1, 9,
      FONT_NEXT, 3, 9,
      FONT_NEXT, 3, 1,
      FONT_END, 1, 1,
      FONT_ADVANCE, 6, 0
   },
   {
      94,   // ^
      FONT_BEGIN, 2, 2,
      FONT_END, 2, 8,
      FONT_BEGIN, 0, 6,
      FONT_NEXT, 2, 8,
      FONT_END, 4, 6,
      FONT_ADVANCE, 6, 0
   },
   {
      95,
      FONT_BEGIN, 2, 3,
      FONT_NEXT, 0, 5,
      FONT_END, 2, 7,
      FONT_BEGIN, 0, 5,
      FONT_END, 4, 5,
      FONT_ADVANCE, 6, 0
   },
   {
      96,   // `
      FONT_BEGIN, 3, 8,
      FONT_NEXT, 2, 8,
      FONT_NEXT, 2, 9,
      FONT_NEXT, 3, 9,
      FONT_NEXT, 3, 8,
      FONT_END, 4, 7,
      FONT_ADVANCE, 6, 0
   },
   {
      123,  // {
      FONT_BEGIN, 4, 9,
      FONT_NEXT, 3, 8,
      FONT_NEXT, 3, 6,
      FONT_NEXT, 2, 5,
      FONT_NEXT, 3, 4,
      FONT_NEXT, 3, 2,
      FONT_END, 4, 1,
      FONT_ADVANCE, 6, 0
   },
   {
      124,  // |
      FONT_BEGIN, 2, 9,
      FONT_END, 2, 0,
      FONT_ADVANCE, 6, 0
   },
   {
      125,  // }
      FONT_BEGIN, 2, 2,
      FONT_NEXT, 2, 3,
      FONT_NEXT, 0, 5,
      FONT_NEXT, 2, 7,
      FONT_END, 2, 8,
      FONT_BEGIN, 2, 7,
      FONT_NEXT, 4, 5,
      FONT_END, 2, 3,
      FONT_ADVANCE, 6, 0
   },
   {
      126,
      FONT_BEGIN, 0, 9,
      FONT_NEXT, 1, 8,
      FONT_NEXT, 1, 6,
      FONT_NEXT, 2, 5,
      FONT_NEXT, 1, 4,
      FONT_NEXT, 1, 2,
      FONT_END, 0, 1,
      FONT_ADVANCE, 6, 0
   },
   {
      176,  // Degree symbol
      FONT_BEGIN, 1.4, 7.4,
      FONT_BEGIN, 0.4, 7.0,
      FONT_NEXT,  0.0, 6.0,
      FONT_NEXT,  0.4, 5.0,
      FONT_NEXT,  1.4, 4.6,
      FONT_NEXT,  2.4, 5.0,
      FONT_NEXT,  2.8, 6.0,
      FONT_NEXT,  2.4, 7.0,
      FONT_END,   1.4, 7.4,
      FONT_ADVANCE, 6, 0
   },
   {
      END_OF_LIST
   }
};


GLenum StrokeFont::createStrokeFont(GLuint fontBase)
{
   for (GLint i = 0; strokeFont[i][0] != END_OF_LIST; i++) {
      glNewList(fontBase+static_cast<unsigned int>(strokeFont[i][0]), GL_COMPILE);
      for (GLint j = 1; strokeFont[i][j]; j += 3) {
         const unsigned int mode = static_cast<unsigned int>(strokeFont[i][j]);
         if (mode == FONT_BEGIN) {
            glBegin(GL_LINE_STRIP);
            glVertex2d(static_cast<double>(strokeFont[i][j+1]-XOFFSET)*XSCALE,
                       static_cast<double>(strokeFont[i][j+2]-YOFFSET)*YSCALE);
         } else if (mode == FONT_NEXT) {
            glVertex2d(static_cast<double>(strokeFont[i][j+1]-XOFFSET)*XSCALE,
                       static_cast<double>(strokeFont[i][j+2]-YOFFSET)*YSCALE);
         } else if (mode == FONT_END) {
            glVertex2d(static_cast<double>(strokeFont[i][j+1]-XOFFSET)*XSCALE,
                       static_cast<double>(strokeFont[i][j+2]-YOFFSET)*YSCALE);
            glEnd();
         } else if (mode == FONT_ADVANCE) {
            glTranslated(static_cast<double>(strokeFont[i][j+1])*XSCALE,
                         static_cast<double>(strokeFont[i][j+2])*YSCALE, 0.0);
            break;
         }
      }
      glEndList();
   }
   return GL_TRUE;
}

}
}

