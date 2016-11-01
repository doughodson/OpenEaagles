
#include "openeaagles/base/Transforms.hpp"
#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/units/Angles.hpp"

namespace oe {
namespace base {

//==============================================================================
// Class: Transform
//==============================================================================

IMPLEMENT_SUBCLASS(Transform,"Transform")

BEGIN_SLOTTABLE(Transform)
        "x", "y", "z", "w"
END_SLOTTABLE(Transform)

//------------------------------------------------------------------------------
//  Map slot table to handles for Transform
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Transform)
    ON_SLOT(1,setComputematrix1,Angle)
    ON_SLOT(1,setComputematrix1,Number)
    ON_SLOT(2,setComputematrix2,Angle)
    ON_SLOT(2,setComputematrix2,Number)
    ON_SLOT(3,setComputematrix3,Angle)
    ON_SLOT(3,setComputematrix3,Number)
    ON_SLOT(4,setComputematrix4,Angle)
    ON_SLOT(4,setComputematrix4,Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Transform::Transform()
{
    STANDARD_CONSTRUCTOR()
    m.makeIdentity();
    nv = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Transform::copyData(const Transform& org, const bool)
{
   BaseClass::copyData(org);
   m  = org.m;
   nv = org.nv;
   for (int i = 0; i < nv; i++) {
      v[i] = org.v[i];
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Transform::deleteData()
{
   m.makeIdentity();
   nv = 0;
}



//------------------------------------------------------------------------------
// getSlotByIndex() for Transform
//------------------------------------------------------------------------------
Object* Transform::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// computeMatrix()
//------------------------------------------------------------------------------
void Transform::computeMatrix()
{
}

//------------------------------------------------------------------------------
// serialize() -- print functions
//------------------------------------------------------------------------------
std::ostream& Transform::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      //indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   BaseClass::serialize(sout,(i+4),slotsOnly);

   if (nv > 0) {
      indent(sout,i+j);
      for (int k = 0; k < nv; k++) {
         sout << v[k] << " ";
      }
      sout << std::endl;
   }

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }
   return sout;
}


//==============================================================================
// Class: Translation
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Translation, "Translation")
EMPTY_SERIALIZER(Translation)

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
Translation::Translation()
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Translation)
EMPTY_DELETEDATA(Translation)

//------------------------------------------------------------------------------
// computeMatrix()
//------------------------------------------------------------------------------
void Translation::computeMatrix()
{
    m.makeIdentity();
    if (nv == 2) {
        // Translate X and Y
        osg::Matrix tt;
        tt.makeTranslate(v[0], v[1], 0.0f);
        m.preMult(tt);
    }
    else if (nv >= 3) {
        // Translate X, Y and Z
        osg::Matrix tt;
        tt.makeTranslate(v[0], v[1], v[2]);
        m.preMult(tt);
    }

}


//==============================================================================
// Class: Rotation
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Rotation, "Rotation")
EMPTY_SERIALIZER(Rotation)

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
Rotation::Rotation()
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Rotation)
EMPTY_DELETEDATA(Rotation)


//------------------------------------------------------------------------------
// computeMatrix()
//------------------------------------------------------------------------------
void Rotation::computeMatrix()
{
    m.makeIdentity();
    if (nv == 1) {
        // Single value: rotate about the Z axis
        osg::Matrix rr;
        rr.makeRotate(v[0], 0.0f, 0.0f, 1.0f);
        m.preMult(rr);
    }
    else if (nv == 4) {
        // Four values: rotate about vector [ v[0] v[1] v[2] ] by v[3] degrees
        osg::Matrix rr;
        rr.makeRotate(v[3], v[0], v[1], v[2]);
        m.preMult(rr);
    }
}


//==============================================================================
// Class: Scale
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Scale, "Scale")
EMPTY_SERIALIZER(Scale)


//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
Scale::Scale()
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Scale)
EMPTY_DELETEDATA(Scale)


//------------------------------------------------------------------------------
// computeMatrix()
//------------------------------------------------------------------------------
void Scale::computeMatrix()
{
    m.makeIdentity();
    if (nv == 1) {
       // Single value: scale X and Y by the value and hold Z constant
        osg::Matrix ss;
        ss.makeScale(v[0], v[0], 1.0f);
        m.preMult(ss);
    }
    if (nv == 2) {
        // Two values: scale X and Y by the values and hold Z constant
        osg::Matrix ss;
        ss.makeScale(v[0], v[1], 1.0f);
        m.preMult(ss);
    }
    else if (nv >= 3) {
        // Three values: scale X, Y and Z
        osg::Matrix ss;
        ss.makeScale(v[0], v[1], v[2]);
        m.preMult(ss);
    }
}


//------------------------------------------------------------------------------
// setComputematrix1 () - for Angle types
//------------------------------------------------------------------------------
bool Transform::setComputematrix1(const Angle* const sc1obj)
{
    bool ok = true;
    if (nv == 0 && isClassType(typeid(Rotation))) {
        Radians rad;
        v[nv++] = static_cast<double>(rad.convert(*sc1obj));
        computeMatrix();
    }
    else {
        std::cerr << "Transform::setComputematrix1: Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setComputematrix1 () - for Number types
//------------------------------------------------------------------------------
bool Transform::setComputematrix1(const Number* const sc1obj)
{
    bool ok = true;
    if (sc1obj != nullptr) {
        v[nv++] = sc1obj->getReal();
        computeMatrix();
    }
    else {
        std::cerr << "Transform::setComputematrix1:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setComputematrix2 () - for Angle types
//------------------------------------------------------------------------------
bool Transform::setComputematrix2(const Angle* const sc2obj)
{
    bool ok = true;
    if (nv == 0 && isClassType(typeid(Rotation))) {
        Radians rad;
        v[nv++] = static_cast<double>(rad.convert(*sc2obj));
        computeMatrix();
    }
    else {
        std::cerr << "Transform::setComputematrix2:  Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setComputematrix2 () - for Number types
//------------------------------------------------------------------------------
bool Transform::setComputematrix2(const Number* const sc2obj)
{
    bool ok = true;
    if (sc2obj != nullptr) {
        v[nv++] = sc2obj->getReal();
        computeMatrix();
    }
    else {
        std::cerr << "Transform::setComputematrix2:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setComputematrix3 () - for Angle types
//------------------------------------------------------------------------------
bool Transform::setComputematrix3(const Angle* const sc3obj)
{
    bool ok = true;
    if (nv == 0 && isClassType(typeid(Rotation))) {
        Radians rad;
        v[nv++] = static_cast<double>(rad.convert(*sc3obj));
        computeMatrix();
    }
    else {
        std::cerr << "Transform::setComputematrix3:  Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setComputematrix3 () - for Number types
//------------------------------------------------------------------------------
bool Transform::setComputematrix3(const Number* const sc3obj)
{
    bool ok = true;
    if (sc3obj != nullptr) {
        v[nv++] = sc3obj->getReal();
        computeMatrix();
    }
    else {
        std::cerr << "Transform::setComputematrix3:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setComputematrix4 () - for Angle types
//------------------------------------------------------------------------------
bool Transform::setComputematrix4(const Angle* const sc4obj)
{
    bool ok = true;
    if (nv == 0 && isClassType(typeid(Rotation))) {
        Radians rad;
        v[nv++] = static_cast<double>(rad.convert(*sc4obj));
        computeMatrix();
    }
    else {
        std::cerr << "Transform::setComputematrix4:  Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setComputematrix4 () - for Number types
//------------------------------------------------------------------------------
bool Transform::setComputematrix4(const Number* const sc4obj)
{
    bool ok = true;
    if (sc4obj != nullptr) {
        v[nv++] = sc4obj->getReal();
        computeMatrix();
    }
    else {
        std::cerr << "Transform::setComputematrix4:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

}
}
