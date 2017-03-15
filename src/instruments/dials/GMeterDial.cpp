
#include "openeaagles/instruments/dials/GMeterDial.hpp"

#include "openeaagles/base/PairStream.hpp"

namespace oe {
namespace instruments {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(GMeterDial,"GMeterDial")
EMPTY_SERIALIZER(GMeterDial)
EMPTY_DELETEDATA(GMeterDial)

GMeterDial::GMeterDial()
{
    STANDARD_CONSTRUCTOR()
    gsSD.empty();
    minGSD.empty();
    maxGSD.empty();
}

void GMeterDial::copyData(const GMeterDial& org, const bool)
{
    BaseClass::copyData(org);
    rotation = org.rotation;
    maxG = org.maxG;
    minG = org.minG;
    gsSD.empty();
    minGSD.empty();
    maxGSD.empty();
}

//------------------------------------------------------------------------------
// reset() -- re-initialize our data
//------------------------------------------------------------------------------
void GMeterDial::reset()
{
    maxG = 0;
    minG = 0;
    gsSD.empty();
    minGSD.empty();
    maxGSD.empty();
    BaseClass::reset();
}

//------------------------------------------------------------------------------
// drawFunc()
//------------------------------------------------------------------------------
void GMeterDial::drawFunc()
{
   base::PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
      subcomponents->unref();
      subcomponents = nullptr;
      return;
   }

    rotation = getInstValue();
    if (rotation != 0) {
      if (rotation < maxG) maxG = rotation;
      if (rotation > minG) minG = rotation;
    }

    GLfloat currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, &currentColor[0]);
    // we have to make three needles.  One for the actual G's being pulled, and
    // two for the max and min
    // actual G's
    glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        glRotatef(static_cast<GLfloat>(rotation), 0.0f, 0.0f, 1.0f);
        glBegin(GL_POLYGON);
            glVertex2f(0.0f, -0.05f);
            glVertex2f(0.0f, 0.05f);
            glVertex2f(0.5f, 0.0f);
        glEnd();
    glPopMatrix();
    // Max G's
    glPushMatrix();
        glColor3f(0.9f, 0.9f, 0.9f);
        glRotatef(static_cast<GLfloat>(maxG), 0.0f, 0.0f, 1.0f);
        glBegin(GL_POLYGON);
            glVertex2f(0.0f, -0.05f);
            glVertex2f(0.0f, 0.05f);
            glVertex2f(0.5f, 0.0f);
        glEnd();
    glPopMatrix();
    // Min G's
    glPushMatrix();
        glColor3f(0.8f, 0.8f, 0.8f);
        glRotatef(static_cast<GLfloat>(minG), 0.0f, 0.0f, 1.0f);
        glBegin(GL_POLYGON);
            glVertex2f(0.0f, -0.05f);
            glVertex2f(0.0f, 0.05f);
            glVertex2f(0.5f, 0.0f);
        glEnd();
    glPopMatrix();

    glColor4fv(currentColor);
}

//------------------------------------------------------------------------------
// updateData() -
//------------------------------------------------------------------------------
void GMeterDial::updateData(const double dt)
{
    BaseClass::updateData(dt);

    //if (isMessageEnabled(MSG_DEBUG)) {
    //std::cout << "ROTATION = " << rotation << std::endl;
    //}

    // update our values here

    {
       base::PairStream* subcomponents = getComponents();
       if (subcomponents == nullptr) return;
       subcomponents->unref();
       subcomponents = nullptr;
    }

    rotation = getPreScaleInstValue();
    if (rotation != 0) {
      if (rotation < maxG) maxG = rotation;
      if (rotation > minG) minG = rotation;
    }

    // send our data down
    send("gs", UPDATE_INSTRUMENTS, rotation, gsSD);
    send("minG", UPDATE_INSTRUMENTS, minG, minGSD);
    send("maxG", UPDATE_INSTRUMENTS, maxG, maxGSD);

}

}
}
