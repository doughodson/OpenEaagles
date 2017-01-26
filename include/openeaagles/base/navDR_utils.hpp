
#ifndef __oe_base_navDR_utils_H__
#define __oe_base_navDR_utils_H__

#include "openeaagles/base/osg/Vec3d"
#include "openeaagles/base/osg/Vec4d"
#include "openeaagles/base/osg/Matrixd"
#include <cmath>

namespace oe {
namespace base {
namespace navDR {

// Standard dead reckoning model codes [ 0 .. 9 ]
enum DeadReckoning { 
   OTHER_DRM,        // User defined (i.e., virtual function dynamicsOther())
   STATIC_DRM,       // No dead reckoning 
   FPW_DRM,          // World, No rotation, 1st order linear 
   RPW_DRM,          // World, 1st order rotation, 1st order linear 
   RVW_DRM,          // World, 1st order rotation, 2nd order linear
   FVW_DRM,          // World, No rotation, 2nd order linear
   FPB_DRM,          // Body, No rotation, 1st order linear
   RPB_DRM,          // Body, 1st order rotation, 1st order linear
   RVB_DRM,          // Body, 1st order rotation, 2nd order linear 
   FVB_DRM,          // Body, No rotation, 2nd order linear 
};

//--------------------------------------------------------------------------
// Dead Reckoning Function
//--------------------------------------------------------------------------
bool deadReckoning(
      const double dT,               // IN: time increment (secs)
      const unsigned int drNum,      // IN: dead reckoning codes
      const osg::Vec3d& p0,         // IN: Position vector @ T=0 (meters) (ECEF)
      const osg::Vec3d& v0,         // IN: Velocity vector @ T=0 (m/sec)  (ECEF or Body based on 'drNum')
      const osg::Vec3d& a0,         // IN: Acceleration vector @ T=0 ((m/sec)/sec) (ECEF or Body based on 'drNum')
      const osg::Vec3d& rpy0,       // IN: Euler angles @ T=0 (rad) [ phi theta psi ] (Body/ECEF)
      const osg::Vec3d& av0,        // IN: Angular rates @ T=0 (rad/sec)  [ phi theta psi ] (Body/ECEF)
      osg::Vec3d* const pNewPw,     // OUT: new world position [meters]
      osg::Vec3d* const pNewRPY     // OUT: new body roll, pitch, yaw [radians]
   );

//--------------------------------------------------------------------------
// Compute R0 Matrix
//--------------------------------------------------------------------------
bool getR0Matrix(
      const osg::Vec4d RPY,          // body roll, pitch, yaw [radians]
      osg::Matrixd* const pR0
   );

//--------------------------------------------------------------------------
// Compute Inv R0 Matrix
//--------------------------------------------------------------------------
bool getInvR0Matrix(
      const osg::Vec4d RPY,          // body roll, pitch, yaw [radians]
      osg::Matrixd* const pInvR0
   );

//--------------------------------------------------------------------------
// Compute Wwt Matrix
//--------------------------------------------------------------------------
bool getWwtMatrix(
      const osg::Vec4d AV,           // body angular velocity [rps]
      osg::Matrixd* const pwwT
   );

//--------------------------------------------------------------------------
// Compute Omega Matrix
//--------------------------------------------------------------------------
bool getOmegaMatrix(
      const osg::Vec4d AV,           // body angular velocity [rps]
      osg::Matrixd* const pOmega
   );

//--------------------------------------------------------------------------
// Compute DR Matrix
//--------------------------------------------------------------------------
bool getDrMatrix(
      const double dT,               // time increment (secs)
      const osg::Vec4d AV,           // body angular velocity [rps]
      osg::Matrixd* const pDR
   );

//--------------------------------------------------------------------------
// Compute Quat DR Matrix
//--------------------------------------------------------------------------
bool getQuatDrMatrix(
      const double dT,               // time increment (secs)
      const osg::Vec4d AV,           // body angular velocity [rps]
      osg::Matrixd* const pDR
   );

//--------------------------------------------------------------------------
// Compute R1 Matrix
//--------------------------------------------------------------------------
bool getR1Matrix(
      const double dT,               // time increment (secs)
      const osg::Vec4d AV,           // body angular velocity [rps]
      osg::Matrixd* const pR1
   );

//--------------------------------------------------------------------------
// Compute R2 Matrix
//--------------------------------------------------------------------------
bool getR2Matrix(
      const double dT,               // time increment (secs)
      const osg::Vec4d AV,           // body angular velocity [rps]
      osg::Matrixd* const pR2
   );

//--------------------------------------------------------------------------
// Compute Rwb Matrix
//--------------------------------------------------------------------------
bool getRwbMatrix(
      const double dT,               // time increment (secs)
      const osg::Vec4d RPY,          // body roll, pitch, yaw [radians]
      const osg::Vec4d AV,           // body angular velocity [rps]
      osg::Matrixd* const pRwb
   );




#include "openeaagles/base/navDR_utils.inl"

}
}
}


#endif

