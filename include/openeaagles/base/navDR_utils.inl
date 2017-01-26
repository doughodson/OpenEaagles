//------------------------------------------------------------------------------
// Navigation utilities for dead reckoning
// Description: inline functions
//------------------------------------------------------------------------------

#ifndef __oe_base_navDR_utils_inline__
#define __oe_base_navDR_utils_inline__

//==============================================================================
// Compute R0 Matrix
//==============================================================================
inline bool getR0Matrix(
         const osg::Vec4d RPY,          // body roll, pitch, yaw [radians]
         osg::Matrixd* const pR0
      )
{
   //---------------------------------------------------------------------------
   // Define Local Constants
   //---------------------------------------------------------------------------
   const double Rol    = RPY[0];
   const double Pch    = RPY[1];
   const double Yaw    = RPY[2];
   const double sinRol = std::sin(Rol);
   const double cosRol = std::cos(Rol);
   const double sinPch = std::sin(Pch);
   const double cosPch = std::cos(Pch);
   const double sinYaw = std::sin(Yaw);
   const double cosYaw = std::cos(Yaw);

   //---------------------------------------------------------------------------
   // World to Body orientation matrix
   //---------------------------------------------------------------------------
   (*pR0)(0,0) = cosPch * cosYaw;
   (*pR0)(0,1) = cosPch * sinYaw;
   (*pR0)(0,2) = -sinPch;
           
   (*pR0)(1,0) = sinRol * sinPch * cosYaw - cosRol * sinYaw;
   (*pR0)(1,1) = sinRol * sinPch * sinYaw + cosRol * cosYaw;
   (*pR0)(1,2) = sinRol * cosPch;
          
   (*pR0)(2,0) = cosRol * sinPch * cosYaw + sinRol * sinYaw;
   (*pR0)(2,1) = cosRol * sinPch * sinYaw - sinRol * cosYaw;
   (*pR0)(2,2) = cosRol * cosPch;

   return true;
}

//==============================================================================
// Compute Inv R0 Matrix
//==============================================================================
inline bool getInvR0Matrix(
         const osg::Vec4d RPY,          // body roll, pitch, yaw [radians]
         osg::Matrixd* const pInvR0
      )
{
   
   //---------------------------------------------------------------------------
   // Compute R0 Matrix
   //---------------------------------------------------------------------------
   osg::Matrixd R0;
   getR0Matrix(RPY, &R0);
      
   //---------------------------------------------------------------------------
   // transpose R0 (since R0 is orthogonal it is also the inverse)
   //---------------------------------------------------------------------------
   for (int i=0; i<4; i++) {
      for (int j=0; j<4; j++) {
         (*pInvR0)(i,j) = R0(j,i);
      }
   }
   
   return true;
}

//==============================================================================
// Compute Wwt Matrix
//==============================================================================
inline bool getWwtMatrix(
         const osg::Vec4d AV,           // body angular velocity [rps]
         osg::Matrixd* const pwwT
      )
{
   
   //---------------------------------------------------------------------------
   // Angular Velocities 
   //---------------------------------------------------------------------------
   const double Wx = AV[0];
   const double Wy = AV[1];
   const double Wz = AV[2];

   //---------------------------------------------------------------------------
   // Compute wwT Matrix
   //---------------------------------------------------------------------------
   (*pwwT)(0,0) = Wx * Wx;
   (*pwwT)(0,1) = Wx * Wy;
   (*pwwT)(0,2) = Wx * Wz;
            
   (*pwwT)(1,0) = Wy * Wx;
   (*pwwT)(1,1) = Wy * Wy;
   (*pwwT)(1,2) = Wy * Wz;
            
   (*pwwT)(2,0) = Wz * Wx;
   (*pwwT)(2,1) = Wz * Wy;
   (*pwwT)(2,2) = Wz * Wz;

   return true;
}

//==============================================================================
// Compute Omega Matrix
//==============================================================================
inline bool getOmegaMatrix(
         const osg::Vec4d AV,           // body angular velocity [rps]
         osg::Matrixd* const pOmega
      )
{
   
   //---------------------------------------------------------------------------
   // Angular Velocities 
   //---------------------------------------------------------------------------
   const double Wx = AV[0];
   const double Wy = AV[1];
   const double Wz = AV[2];

   //---------------------------------------------------------------------------
   // Compute Omega Matrix
   //---------------------------------------------------------------------------
   (*pOmega)(0,0) = 0.0;
   (*pOmega)(0,1) = -Wz;
   (*pOmega)(0,2) = +Wy;
               
   (*pOmega)(1,0) = +Wz;
   (*pOmega)(1,1) = 0.0;
   (*pOmega)(1,2) = -Wx;
               
   (*pOmega)(2,0) = -Wy;
   (*pOmega)(2,1) = +Wx;
   (*pOmega)(2,2) = 0.0;

   return true;
}

//==============================================================================
// Compute DR Matrix
//==============================================================================
inline bool getDrMatrix(
         const double dT,               // time increment (secs)
         const osg::Vec4d AV,           // body angular velocity [rps]
         osg::Matrixd* const pDR
      )
{
   const double EPS = 1.0E-12; 
   
   //---------------------------------------------------------------------------
   // Angular Velocities 
   //---------------------------------------------------------------------------
   const double Wx = AV[0];
   const double Wy = AV[1];
   const double Wz = AV[2];

   const double absW1 = std::sqrt(Wx*Wx + Wy*Wy + Wz*Wz);
   if (absW1 < EPS) {
      osg::Matrixd I3;  // get 4X4 identity matrix (virtual 3x3 matrix)
      *pDR = I3;
   }
   else {
      const double absW2 = absW1*absW1;
      const double cosWT  = std::cos(absW1 * dT);
      const double sinWT  = std::sin(absW1 * dT);

      //---------------------------------------------------------------------------
      // Compute Component Coefficients (K1, K2, K3)
      //---------------------------------------------------------------------------
      const double K1 = (1.0 - cosWT) / absW2;
      const double K2 = cosWT;
      const double K3 = sinWT / absW1;

      //---------------------------------------------------------------------------
      // Compute Component Matrices (wwT, I3, Omega)
      //---------------------------------------------------------------------------
      osg::Matrixd wwT;
      getWwtMatrix(AV, &wwT);  // get wwT matrix

      osg::Matrixd I3;  // get 4X4 identity matrix (virtual 3x3 matrix)

      osg::Matrixd Omega;
      getOmegaMatrix(AV, &Omega);  // get Omega matrix

      //---------------------------------------------------------------------------
      // Compute Dead Reckoning Matrix (DR)
      //---------------------------------------------------------------------------
      *pDR = K1*wwT + K2*I3 - K3*Omega;
   }
   
   return true;
}

//==============================================================================
// Compute Quat DR Matrix
//==============================================================================
inline bool getQuatDrMatrix(
         const double dT,               // time increment (secs)
         const osg::Vec4d AV,           // body angular velocity [rps]
         osg::Matrixd* const pDR
      )
{
   
   //---------------------------------------------------------------------------
   // Calculate quaternions
   //---------------------------------------------------------------------------
   const double Wx = AV[0];
   const double Wy = AV[1];
   const double Wz = AV[2];

   const double absW2 = Wx*Wx + Wy*Wy + Wz*Wz;
   const double absW1 = std::sqrt(absW2);

   const double cosA = Wx / absW1; // ??
   const double cosB = Wy / absW1; // ??
   const double cosC = Wz / absW1; // ??
   
   const double BETA = absW1 * dT;
   const double cosHalfBeta = std::cos(BETA / 2.0);
   const double sinHalfBeta = std::sin(BETA / 2.0);
   
   const double Q0 = cosHalfBeta;
   const double Qx = sinHalfBeta * cosA;
   const double Qy = sinHalfBeta * cosB;
   const double Qz = sinHalfBeta * cosC;
   
   //---------------------------------------------------------------------------
   // Calculate DR matrix in terms of quaternions
   //---------------------------------------------------------------------------
   (*pDR)(0,0) = Q0*Q0 + Qx*Qx - 0.5;
   (*pDR)(0,1) = Qx*Qy - Q0*Qz;
   (*pDR)(0,2) = Qx*Qz + Q0*Qy;
   (*pDR)(1,0) = Qx*Qy + Q0*Qz;
   (*pDR)(1,1) = Q0*Q0 + Qy*Qy - 0.5;
   (*pDR)(1,2) = Qy*Qz - Q0*Qx;
   (*pDR)(2,0) = Qx*Qz - Q0*Qy;
   (*pDR)(2,1) = Qy*Qz + Q0*Qx;
   (*pDR)(2,2) = Q0*Q0 + Qz*Qz - 0.5;
   
   (*pDR) *= 2.0;
   
   return true;
}

//==============================================================================
// Compute R1 Matrix
//==============================================================================
inline bool getR1Matrix(
         const double dT,               // time increment (secs)
         const osg::Vec4d AV,           // body angular velocity [rps]
         osg::Matrixd* const pR1
      )
{
   const double EPS = 1.0E-12;
   
   //---------------------------------------------------------------------------
   // Angular Velocities 
   //---------------------------------------------------------------------------
   const double Wx = AV[0];
   const double Wy = AV[1];
   const double Wz = AV[2];

   const double absW1 = std::sqrt(Wx*Wx + Wy*Wy + Wz*Wz);
   if (absW1 < EPS) {
      osg::Matrixd I3;  // get 4X4 identity matrix (virtual 3x3 matrix)
      *pR1 = I3;
   }
   else {
      const double absW2 = absW1 * absW1;
      const double absW3 = absW2 * absW1;

      const double cosWT = std::cos(absW1 * dT);
      const double sinWT = std::sin(absW1 * dT);

      //---------------------------------------------------------------------------
      // Compute Matrix Coefficients (K1, K2, K3)
      //---------------------------------------------------------------------------
      const double K1 = (absW1 * dT - sinWT) / absW3;
      const double K2 = sinWT / absW1;
      const double K3 = (1.0 - cosWT) / absW2;

      //---------------------------------------------------------------------------
      // Compute Component Matrices (wwT, I3, Omega)
      //---------------------------------------------------------------------------
      osg::Matrixd wwT;
      getWwtMatrix(AV, &wwT);

      osg::Matrixd I3;

      osg::Matrixd Omega;
      getOmegaMatrix(AV, &Omega);

      //---------------------------------------------------------------------------
      // Compute R1 Matrix
      //---------------------------------------------------------------------------
      *pR1 = K1*wwT + K2*I3 + K3*Omega;
   }
   
   return true;
}

//==============================================================================
// Compute R2 Matrix
//==============================================================================
inline bool getR2Matrix(
         const double dT,               // time increment (secs)
         const osg::Vec4d AV,           // body angular velocity [rps]
         osg::Matrixd* const pR2
      )
{
   const double EPS = 1.0E-12;
   
   //---------------------------------------------------------------------------
   // Angular Velocities 
   //---------------------------------------------------------------------------
   const double Wx = AV[0];
   const double Wy = AV[1];
   const double Wz = AV[2];

   const double absW1 = std::sqrt(Wx*Wx + Wy*Wy + Wz*Wz);
   if (absW1 < EPS) {
      osg::Matrixd I3;  // get 4X4 identity matrix (virtual 3x3 matrix)
      *pR2 = I3;
   } else {
      const double absW2 = absW1 * absW1;
      const double absW3 = absW1 * absW2;
      const double absW4 = absW1 * absW3;

      const double cosWT = std::cos(absW1 * dT);
      const double sinWT = std::sin(absW1 * dT);

      //---------------------------------------------------------------------------
      // Get Matrix Coefficients (K1, K2, K3)
      //---------------------------------------------------------------------------
      const double K1 = (0.5*absW2*dT*dT - cosWT - absW1*dT*sinWT + 1.0) / absW4;
      const double K2 = (cosWT + absW1*dT*sinWT - 1.0) / absW2;
      const double K3 = (sinWT - absW1*dT*cosWT) / absW3;

      //---------------------------------------------------------------------------
      // Get Component Matrices (wwT, I3, Omega)
      //---------------------------------------------------------------------------
      osg::Matrixd wwT;
      getWwtMatrix(AV, &wwT);       // gets the wwT matrix

      osg::Matrixd I3;              // gets a 4x4 identity matrix (virtual 3x3)

      osg::Matrixd Omega;
      getOmegaMatrix(AV, &Omega);   // gets the Omega matrix

      //---------------------------------------------------------------------------
      // Compute R2 Matrix
      //---------------------------------------------------------------------------
      *pR2 = K1*wwT + K2*I3 + K3*Omega;
   }
   
   return true;
}

//==============================================================================
// Compute Rwb Matrix
//==============================================================================
inline bool getRwbMatrix(
         const double dT,               // time increment (secs)
         const osg::Vec4d RPY,          // body roll, pitch, yaw [radians]
         const osg::Vec4d AV,           // body angular velocity [rps]
         osg::Matrixd* const pRwb
      )
{
   //---------------------------------------------------------------------------
   // Compute Rwb Matrix
   //---------------------------------------------------------------------------
   osg::Matrixd R0;
   osg::Matrixd DR;

   getR0Matrix(RPY, &R0);
   getDrMatrix(dT, AV, &DR);

   *pRwb = DR * R0;

   return true;
}

#endif
