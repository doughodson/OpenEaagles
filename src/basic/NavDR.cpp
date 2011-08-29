
#include "openeaagles/basic/NavDR.h"

namespace Eaagles {
namespace Basic {

//==============================================================================
// Dead Reckoning Function
//==============================================================================
bool NavDR::deadReckoning(
         const double dT,               // IN: time increment (secs)
         const unsigned int drNum,      // IN: dead reckoning code
         const osg::Vec3d& p0,         // IN: Position vector @ T=0 (meters) (ECEF)
         const osg::Vec3d& v0,         // IN: Velocity vector @ T=0 (m/sec)  (ECEF or Body based on 'drNum')
         const osg::Vec3d& a0,         // IN: Acceleration vector @ T=0 ((m/sec)/sec) (ECEF or Body based on 'drNum')
         const osg::Vec3d& rpy0,       // IN: Euler angles @ T=0 (rad) [ phi theta psi ] (Body/ECEF)
         const osg::Vec3d& av0,        // IN: Angular rates @ T=0 (rad/sec)  [ phi theta psi ] (Body/ECEF)
         osg::Vec3d* const pNewPw,     // OUT: new world position [meters]
         osg::Vec3d* const pNewRPY     // OUT: new body roll, pitch, yaw [radians]
      )
{   
   // Using 4D vectors with the forth element (w) zero
   const osg::Vec4d p(p0, 0.0);
   const osg::Vec4d v(v0, 0.0);
   const osg::Vec4d a(a0, 0.0);
   const osg::Vec4d rpy(rpy0, 0.0);
   const osg::Vec4d av(av0, 0.0);

   // new (output) vectors
   osg::Vec4d newP = p;
   osg::Vec4d newRPY = rpy;


   switch (drNum) {

      //--------------------------------------------------------------
      // DRM = STATIC -- No dead reckoning 
      //--------------------------------------------------------------
      case STATIC_DRM:  {
         break;
      }
      
      //--------------------------------------------------------------
      // DRM = FPW -- World, No rotation, 1st order linear 
      //--------------------------------------------------------------
      case FPW_DRM:  {
         // get new world position
         newP = p + v*dT;                       // new P
         break;
      }
      
      //--------------------------------------------------------------
      // DRM = RPW -- World, 1st order rotation, 1st order linear 
      //--------------------------------------------------------------
      case RPW_DRM:  {
         // get new world position
         newP = p + v*dT;                       // new P

         // get new roll, pitch, yaw
         osg::Matrixd Rwb;
         getRwbMatrix(dT, rpy, av, &Rwb);
         getRpyVector(Rwb, &newRPY);            // newRPY
         break;
      }
      
      //--------------------------------------------------------------
      // DRM = RVW -- World, 1st order rotation, 2nd order linear
      //--------------------------------------------------------------
      case RVW_DRM:  {
         // get new world position
         newP = p + v*dT + a*dT*dT*0.5;        // new P

         // get new roll, pitch, yaw
         osg::Matrixd Rwb;
         getRwbMatrix(dT, rpy, av, &Rwb);
         getRpyVector(Rwb, &newRPY);            // new RPY
         break;
      }
      
      //--------------------------------------------------------------
      // DRM = FVW -- World, No rotation, 2nd order linear
      //--------------------------------------------------------------
      case FVW_DRM:  {
         // get new world position
         newP = p + v*dT + a*dT*dT*0.5;        // new P
         break;
      }

      //--------------------------------------------------------------
      // DRM = FPB -- Body, No rotation, 1st order linear
      //--------------------------------------------------------------
      case FPB_DRM:  {
         // get new world position
         osg::Matrixd R1,InvR0;
         getR1Matrix(dT, av, &R1);
         getInvR0Matrix(rpy, &InvR0);
         newP = p + InvR0*R1*v;                // new P
         break;
      }
      
      //--------------------------------------------------------------
      // DRM = RPB -- Body, 1st order rotation, 1st order linear
      //--------------------------------------------------------------
      case RPB_DRM:  {
         // get new world position
         osg::Matrixd R1,InvR0;
         getR1Matrix(dT, av, &R1);
         getInvR0Matrix(rpy, &InvR0);
         newP = p + InvR0*R1*v;                // new P

         // get new roll, pitch, yaw
         osg::Matrixd Rwb;
         getRwbMatrix(dT, rpy, av, &Rwb);
         getRpyVector(Rwb, &newRPY);            // new RPY
         break;
      }
      
      //--------------------------------------------------------------
      // DRM = RVB -- Body, 1st order rotation, 2nd order linear 
      //--------------------------------------------------------------
      case RVB_DRM:  {
         // get new world position
         osg::Matrixd R1,R2,InvR0;
         getR1Matrix(dT, av, &R1);
         getR2Matrix(dT, av, &R2);
         getInvR0Matrix(rpy, &InvR0);
         newP = p + InvR0*(R1*v + R2*a);      // new P

         // get new roll, pitch, yaw
         osg::Matrixd Rwb;
         getRwbMatrix(dT, rpy, av, &Rwb);
         getRpyVector(Rwb, &newRPY);            // new RPY
         break;
      }

      //--------------------------------------------------------------
      // DRM = FVB -- Body, No rotation, 2nd order linear 
      //--------------------------------------------------------------
      case FVB_DRM:  {
         // get new world position
         osg::Matrixd R1,R2,InvR0;
         getR1Matrix(dT, av, &R1);
         getR2Matrix(dT, av, &R2);
         getInvR0Matrix(rpy, &InvR0);
         newP = p + InvR0*(R1*v + R2*a);      // new P
         break;
      }
      
      //--------------------------------------------------------------
      // DRM = User defined or something else
      //--------------------------------------------------------------
      case OTHER_DRM:
      default: {
         break;
      }
   }

   // Return the new P and RPY vectors
   pNewPw->set( newP.x(), newP.y(), newP.z() );
   pNewRPY->set( newRPY.x(), newRPY.y(), newRPY.z() );
   
   return true;
}

}  // end namespace Basic
}  // end namespace Eaagles
