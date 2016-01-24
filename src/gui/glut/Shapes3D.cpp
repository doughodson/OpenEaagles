//------------------------------------------------------------------------------
// Shapes3D - shapes in 3D (mostly GLU shapes)
//------------------------------------------------------------------------------
#include "openeaagles/gui/glut/Shapes3D.h"

#include "openeaagles/basic/Number.h"

#include <GL/glut.h>
#include <GL/glu.h>

namespace oe {
namespace glut {

IMPLEMENT_SUBCLASS(Sphere,"Sphere")

IMPLEMENT_SUBCLASS(Cylinder,"Cylinder")

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Cone,"Cone")

IMPLEMENT_SUBCLASS(Cube,"Cube")
EMPTY_SERIALIZER(Cone)

IMPLEMENT_SUBCLASS(Torus,"Torus")

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Dodecahedron,"Dodecahedron")
EMPTY_SERIALIZER(Dodecahedron)

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Tetrahedron,"Tetrahedron")
EMPTY_SERIALIZER(Tetrahedron)

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Icosahedron,"Icosahedron")
EMPTY_SERIALIZER(Icosahedron)

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Octahedron,"Octahedron")
EMPTY_SERIALIZER(Octahedron)

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Teapot,"Teapot")
EMPTY_SERIALIZER(Teapot)

//------------------------------------------------------------------------------
// Slot tables
//------------------------------------------------------------------------------

// Sphere --
BEGIN_SLOTTABLE(Sphere)
    "stacks",       // 1: Number of stacks on the circle
END_SLOTTABLE(Sphere)

// Cylinder --
BEGIN_SLOTTABLE(Cylinder)
    "topRadius",    // 1: Radius at the top of the cylinder
    "height",       // 2: Height of the cylinder
END_SLOTTABLE(Cylinder)

// Cube --
BEGIN_SLOTTABLE(Cube)
    "size",       // 1: Size you want the cube to be
END_SLOTTABLE(Cube)

// Torus --
BEGIN_SLOTTABLE(Torus)
    "outerRadius", // 1: Radius of the outer ring
END_SLOTTABLE(Torus)

//------------------------------------------------------------------------------
//  Map slot tables
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Sphere)
    ON_SLOT(1,setSlotStacks,basic::Number)
END_SLOT_MAP()

BEGIN_SLOT_MAP(Cylinder)
    ON_SLOT(1,setSlotTopRadius,basic::Number)
    ON_SLOT(2,setSlotHeight,basic::Number)
END_SLOT_MAP()

BEGIN_SLOT_MAP(Cube)
    ON_SLOT(1,setSlotSize,basic::Number)
END_SLOT_MAP()

BEGIN_SLOT_MAP(Torus)
    ON_SLOT(1,setSlotOuterRadius,basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Sphere::Sphere()
{
    STANDARD_CONSTRUCTOR()
    stacks = 10;
}

Cylinder::Cylinder()
{
    STANDARD_CONSTRUCTOR()
    topRadius = 1;
    height = 1;
}

Cone::Cone()
{
    STANDARD_CONSTRUCTOR()
}

Cube::Cube()
{
    STANDARD_CONSTRUCTOR()
    size = 1;
}

Torus::Torus()
{
    STANDARD_CONSTRUCTOR()
    oRadius = 2;
}

Dodecahedron::Dodecahedron()
{
    STANDARD_CONSTRUCTOR()
}

Tetrahedron::Tetrahedron()
{
    STANDARD_CONSTRUCTOR()
}

Icosahedron::Icosahedron()
{
    STANDARD_CONSTRUCTOR()
}

Octahedron::Octahedron()
{
    STANDARD_CONSTRUCTOR()
}

Teapot::Teapot()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Sphere::copyData(const Sphere& org, const bool)
{
    BaseClass::copyData(org);
    stacks = org.stacks;
}

void Cylinder::copyData(const Cylinder& org, const bool)
{
    BaseClass::copyData(org);
    topRadius = org.topRadius;
    height = org.height;
}

EMPTY_COPYDATA(Cone)

void Cube::copyData(const Cube& org, const bool)
{
    BaseClass::copyData(org);
    size = org.size;
}

void Torus::copyData(const Torus& org, const bool)
{
    BaseClass::copyData(org);
    oRadius = org.oRadius;
}

EMPTY_COPYDATA(Dodecahedron)

EMPTY_COPYDATA(Tetrahedron)

EMPTY_COPYDATA(Icosahedron)

EMPTY_COPYDATA(Octahedron)

EMPTY_COPYDATA(Teapot)

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Sphere)
EMPTY_DELETEDATA(Cylinder)
EMPTY_DELETEDATA(Cone)
EMPTY_DELETEDATA(Cube)
EMPTY_DELETEDATA(Torus)
EMPTY_DELETEDATA(Dodecahedron)
EMPTY_DELETEDATA(Tetrahedron)
EMPTY_DELETEDATA(Icosahedron)
EMPTY_DELETEDATA(Octahedron)
EMPTY_DELETEDATA(Teapot)

void Sphere::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidSphere(getRadius(), getSlices(), stacks);
    else glutWireSphere(getRadius(), getSlices(), stacks);
    END_DLIST
}

void Cylinder::drawFunc()
{
    BEGIN_DLIST
    GLUquadricObj *qobj = gluNewQuadric();
    if (isFilled()) gluQuadricDrawStyle(qobj, GLU_FILL);
    else gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
    gluCylinder(qobj, getRadius(), topRadius, height, getSlices(), getStacks());
    gluDeleteQuadric(qobj);
    END_DLIST
}

void Cone::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidCone(getRadius(), getHeight(), getSlices(), getStacks());
    else glutWireCone(getRadius(), getHeight(), getSlices(), getStacks());
    END_DLIST
}

void Cube::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidCube(size);
    else glutWireCube(size);
    END_DLIST
}

void Torus::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidTorus(getRadius(), oRadius, getSlices(), getStacks());
    else glutWireTorus(getRadius(), oRadius, getSlices(), getStacks());
    END_DLIST
}

void Dodecahedron::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidDodecahedron();
    else glutWireDodecahedron();
    END_DLIST
}

void Tetrahedron::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidTetrahedron();
    else glutWireTetrahedron();
    END_DLIST
}

void Icosahedron::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidIcosahedron();
    else glutWireIcosahedron();
    END_DLIST
}

void Octahedron::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidOctahedron();
    else glutWireOctahedron();
    END_DLIST
}

void Teapot::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidTeapot(getSize());
    else glutWireTeapot(getSize());
    END_DLIST
}


//------------------------------------------------------------------------------
//  setSlotStacks() -- for Sphere
//------------------------------------------------------------------------------
bool Sphere::setSlotStacks(const basic::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setStacks(x->getInt());
    return ok;
}
//------------------------------------------------------------------------------
//  setSlotTopRadius() -- for Cylinder
//------------------------------------------------------------------------------
bool Cylinder::setSlotTopRadius(const basic::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setTopRadius(x->getReal());
    return ok;
}
//------------------------------------------------------------------------------
//  setSlotHeight() -- for Cylinder
//------------------------------------------------------------------------------
bool Cylinder::setSlotHeight(const basic::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setHeight(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotSize() -- for Cube
//------------------------------------------------------------------------------
bool Cube::setSlotSize(const basic::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setSize(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotOuterRadius() -- for Torus
//------------------------------------------------------------------------------
bool Torus::setSlotOuterRadius(const basic::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setOuterRadius(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// serialize() -- print functions
//------------------------------------------------------------------------------
std::ostream& Sphere::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      //indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   BaseClass::serialize(sout,i+j,true);

   indent(sout,i+j);
   sout << "stacks: " << stacks << std::endl;

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }
   return sout;
}

std::ostream& Cylinder::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      //indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   BaseClass::serialize(sout,i+j,true);

   indent(sout,i+j);
   sout << "topRadius: " << topRadius << std::endl;

   indent(sout,i+j);
   sout << "height: " << height << std::endl;

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }
   return sout;
}

std::ostream& Cube::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      //indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   BaseClass::serialize(sout,i+j,true);

   indent(sout,i+j);
   sout << "size: " << size << std::endl;

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }
   return sout;
}

std::ostream& Torus::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      //indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   BaseClass::serialize(sout,i+j,true);

   indent(sout,i+j);
   sout << "outerRadius: " << oRadius << std::endl;

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }
   return sout;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
basic::Object* Sphere::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

basic::Object* Cylinder::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

basic::Object* Cube::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

basic::Object* Torus::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End glut namespace
} // End oe namespace
