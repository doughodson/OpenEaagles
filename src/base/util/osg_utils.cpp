
#include "openeaagles/base/util/osg_utils.hpp"

#include "openeaagles/base/osg/Vec3"
#include "openeaagles/base/osg/Vec4"
#include "openeaagles/base/osg/Matrix"

namespace oe {
namespace base {

//------------
// Pre-multiply an array of 4D vectors with a 4x4 matrix
//------------
void preMultVec4Array(
      const osg::Vec4d orig[],
      const osg::Matrixd& matrix,
      osg::Vec4d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

void preMultVec4Array(
      const osg::Vec4f orig[],
      const osg::Matrixf& matrix,
      osg::Vec4f result[],
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
      const osg::Vec3d orig[],
      const osg::Matrixd& matrix,
      osg::Vec3d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

void preMultVec3Array(
      const osg::Vec3f orig[],
      const osg::Matrixf& matrix,
      osg::Vec3f result[],
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
      const osg::Vec4d orig[],
      const osg::Matrixd& matrix,
      osg::Vec4d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

void postMultVec4Array(
      const osg::Vec4f orig[],
      const osg::Matrixf& matrix,
      osg::Vec4f result[],
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
      const osg::Vec3d orig[],
      const osg::Matrixd& matrix,
      osg::Vec3d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

void postMultVec3Array(
      const osg::Vec3f orig[],
      const osg::Matrixf& matrix,
      osg::Vec3f result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

}
}

