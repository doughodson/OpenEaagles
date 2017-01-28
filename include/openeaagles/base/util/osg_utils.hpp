
#ifndef __oe_base_util_osg_utils_H__
#define __oe_base_util_osg_utils_H__

//------------------------------------------------------------------------------
// General purpose functions for OSG classes
//------------------------------------------------------------------------------

namespace oe {

namespace base {
class Vec3d; class Vec3f;
class Vec4d; class Vec4f;
class Matrixd; class Matrixf;

// pre-multiply an array of 4D vectors with a 4x4 matrix

void preMultVec4Array(
      const Vec4d orig[],
      const Matrixd& matrix,
      Vec4d result[],
      const unsigned int n
   );

void preMultVec4Array(
      const Vec4f orig[],
      const Matrixf& matrix,
      Vec4f result[],
      const unsigned int n
   );

// pre-multiply an array of 3D vectors with a 4x4 matrix

void preMultVec3Array(
      const Vec3d orig[],
      const Matrixd& matrix,
      Vec3d result[],
      const unsigned int n
   );

void preMultVec3Array(
      const Vec3f orig[],
      const Matrixf& matrix,
      Vec3f result[],
      const unsigned int n
   );

// post-multiply an array of 4D vectors with a 4x4 matrix

void postMultVec4Array(
      const Vec4d orig[],
      const Matrixd& matrix,
      Vec4d result[],
      const unsigned int n
   );

void postMultVec4Array(
      const Vec4f orig[],
      const Matrixf& matrix,
      Vec4f result[],
      const unsigned int n
   );

// post-multiply an array of 3D vectors with a 4x4 matrix

void postMultVec3Array(
      const Vec3d orig[],
      const Matrixd& matrix,
      Vec3d result[],
      const unsigned int n
   );

void postMultVec3Array(
      const Vec3f orig[],
      const Matrixf& matrix,
      Vec3f result[],
      const unsigned int n
   );

}
}

#endif

