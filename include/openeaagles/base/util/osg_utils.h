
//------------------------------------------------------------------------------
// OSG utility functions
//------------------------------------------------------------------------------

#ifndef __oe_base_util_osg_utils_H__
#define __oe_base_util_osg_utils_H__

#include "openeaagles/base/osg/Vec3"
#include "openeaagles/base/osg/Vec4"
#include "openeaagles/base/osg/Matrix"

namespace oe {
namespace base {

// Pre-multiply an array of 4D vectors with a 4x4 matrix

void preMultVec4Array(
      const osg::Vec4d orig[],
      const osg::Matrixd& matrix,
      osg::Vec4d result[],
      const unsigned int n
   );

void preMultVec4Array(
      const osg::Vec4f orig[],
      const osg::Matrixf& matrix,
      osg::Vec4f result[],
      const unsigned int n
   );

// Pre-multiply an array of 3D vectors with a 4x4 matrix

void lcPreMultVec3Array(
      const osg::Vec3 orig[],
      const osg::Matrix& matrix,
      osg::Vec3 result[],
      const int n
   );

void preMultVec3Array(
      const osg::Vec3d orig[],
      const osg::Matrixd& matrix,
      osg::Vec3d result[],
      const unsigned int n
   );

void preMultVec3Array(
      const osg::Vec3f orig[],
      const osg::Matrixf& matrix,
      osg::Vec3f result[],
      const unsigned int n
   );

// Post-multiply an array of 4D vectors with a 4x4 matrix

void lcPostMultVec4Array(
      const osg::Vec4 orig[],
      const osg::Matrix& matrix,
      osg::Vec4 result[],
      const int n
   );

void postMultVec4Array(
      const osg::Vec4d orig[],
      const osg::Matrixd& matrix,
      osg::Vec4d result[],
      const unsigned int n
   );

void postMultVec4Array(
      const osg::Vec4f orig[],
      const osg::Matrixf& matrix,
      osg::Vec4f result[],
      const unsigned int n
   );

// Post-multiply an array of 3D vectors with a 4x4 matrix

void lcPostMultVec3Array(
      const osg::Vec3 orig[],
      const osg::Matrix& matrix,
      osg::Vec3 result[],
      const int n
   );

void postMultVec3Array(
      const osg::Vec3d orig[],
      const osg::Matrixd& matrix,
      osg::Vec3d result[],
      const unsigned int n
   );

void postMultVec3Array(
      const osg::Vec3f orig[],
      const osg::Matrixf& matrix,
      osg::Vec3f result[],
      const unsigned int n
   );

}
}

#endif

