/****************************************************************************
 * dsFlightModelData.hxx
 *
 * Author:	              Westfall, Orion
 * Creation Date:         12-Mar-2001
 *
 *
 * Purpose:        Data structure to hold data from flight model
 *
 * Description:
 *
 * Notes:
 *
 * Date          Name                  Description
 * -----------   -------------------   ----------------------------------------
 * 13-Mar-2001   Orion Westfall        Created
 *
 *****************************************************************************/

#ifndef DSFLIGHTMODELDATA
#define DSFLIGHTMODELDATA

#include "geoTypes.h"

class dsFlightModelData
{
public:
  float_8          m_x_cg;
  float_8          m_y_cg;
  float_8          m_z_cg;          // altitude

  float_8          m_pilot_eye_x;
  float_8          m_pilot_eye_y;
  float_8          m_pilot_eye_z;   // altitude

  float_8          m_alphad;
  float_8          m_betad;
  float_8          m_mach;
  float_8          m_runalt;

  float_4          m_theta;
  float_4          m_phi;
  float_4          m_psi;
  float_4          m_airspeed;      // knots

  float_4          m_heading;

  float_4          m_dlg;           // landing gear position 90 is down (scaled to 0-1)
  float_4          m_dsb;           // speed break 60 is out (scaled to 0-1)
  float_4          m_nz;

  float_4          m_aetrc;         // Commanded throttle position
  int_4            m_afterburner;   // logical, true in in A/B

  int_4            m_target_id;

  int_2            m_id_self;       // make use of a hole
  int_2            m_flags;

  float_8          m_target_x;
  float_8          m_target_y;
  float_8          m_target_z;

  float_4          m_target_theta;
  float_4          m_target_phi;
  float_4          m_target_psi;

  float_4          m_target_uearth;
  float_4          m_target_vearth;
  float_4          m_target_wearth;
  float_4          m_target_vcas;

  float_4          m_spares[4];
};


#endif
