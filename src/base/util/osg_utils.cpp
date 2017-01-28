
#include "openeaagles/base/util/osg_utils.hpp"

#include "openeaagles/base/osg/Vec3d"
#include "openeaagles/base/osg/Vec3f"
#include "openeaagles/base/osg/Vec4d"
#include "openeaagles/base/osg/Vec4f"
#include "openeaagles/base/osg/Matrixf"
#include "openeaagles/base/osg/Matrixd"

namespace oe {
namespace base {

//------------
// Pre-multiply an array of 4D vectors with a 4x4 matrix
//------------
void preMultVec4Array(
      const Vec4d orig[],
      const Matrixd& matrix,
      Vec4d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

void preMultVec4Array(
      const Vec4f orig[],
      const Matrixf& matrix,
      Vec4f result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

//------------
// Pre-multiply an array of 3D vectors with a 4x4 matrix
//------------

void preMultVec3Array(
      const Vec3d orig[],
      const Matrixd& matrix,
      Vec3d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

void preMultVec3Array(
      const Vec3f orig[],
      const Matrixf& matrix,
      Vec3f result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

//------------
// Post-multiply an array of 4D vectors with a 4x4 matrix
//------------

void postMultVec4Array(
      const Vec4d orig[],
      const Matrixd& matrix,
      Vec4d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

void postMultVec4Array(
      const Vec4f orig[],
      const Matrixf& matrix,
      Vec4f result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

//------------
// Post-multiply an array of 3D vectors with a 4x4 matrix
//------------

void postMultVec3Array(
      const Vec3d orig[],
      const Matrixd& matrix,
      Vec3d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

void postMultVec3Array(
      const Vec3f orig[],
      const Matrixf& matrix,
      Vec3f result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

}
}

