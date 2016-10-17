
#ifndef __oe_base_util_osg_utils_H__
#define __oe_base_util_osg_utils_H__

//------------------------------------------------------------------------------
// General purpose functions for OSG classes
//------------------------------------------------------------------------------

namespace oe {

namespace osg {
   class Vec3d; class Vec3f;
   class Vec4d; class Vec4f;
   class Matrixd; class Matrixf;
}

namespace base {

// pre-multiply an array of 4D vectors with a 4x4 matrix

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

// pre-multiply an array of 3D vectors with a 4x4 matrix

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

// post-multiply an array of 4D vectors with a 4x4 matrix

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

// post-multiply an array of 3D vectors with a 4x4 matrix

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

