
#ifndef __Eaagles_Basic_lfi_H__
#define __Eaagles_Basic_lfi_H__

#include "openeaagles/basic/support.h"

namespace Eaagles {
namespace Basic {
namespace lfi {

//==============================================================================
// Linear Function Interpolators
//==============================================================================

// ---
// 1D Linear Function Interpolator
//    x       - Independent variable #1
//    x_data  - Table of independent variable #1 breakpoints
//    nx      - Size of x_data table
//    a_data  - Table of dependent variable data
//    eFlg    - Extrapolation enabled flag (optional)
//    xbp     - Pointer to the previous X breakpoint (optional)
// ---

LCreal lfi_1D(const LCreal x,
           const LCreal* x_data, const unsigned int nx,
           const LCreal* a_data,
           const bool eFlg=false,
           unsigned int* const xbp=nullptr
          );

// ---
// 2D Linear Function Interpolator
//    Same as 1D plus ...
//       y       - Independent variable #2
//       y_data  - Table of independent variable #2 breakpoints
//       ny      - Size of y_data table
//       ybp     - Pointer to the previous Y breakpoint (optional)
// ---

LCreal lfi_2D(const LCreal x, const LCreal y,
           const LCreal* x_data, const unsigned int nx,
           const LCreal* y_data, const unsigned int ny,
           const LCreal* a_data,
           const bool eFlg=false,
           unsigned int* const xbp=nullptr,
           unsigned int* const ybp=nullptr
          );

// ---
// 3D Linear Function Interpolator
//    Same as 2D plus ...
//       z       - Independent variable #3
//       z_data  - Table of independent variable #3 breakpoints
//       nz      - Size of z_data table
//       zbp     - Pointer to the previous Z breakpoint (optional)
// ---

LCreal lfi_3D(const LCreal x, const LCreal y, const LCreal z,
           const LCreal* x_data, const unsigned int nx,
           const LCreal* y_data, const unsigned int ny,
           const LCreal* z_data, const unsigned int nz,
           const LCreal* a_data,
           const bool eFlg=false,
           unsigned int* const xbp=nullptr,
           unsigned int* const ybp=nullptr,
           unsigned int* const zbp=nullptr
          );

// ---
// 4D Linear Function Interpolator
//    Same as 3D plus ...
//       w       - Independent variable #4
//       w_data  - Table of independent variable #4 breakpoints
//       nw      - Size of w_data table
//       wbp     - Pointer to the previous W breakpoint (optional)
// ---

LCreal lfi_4D(const LCreal x, const LCreal y, const LCreal z, const LCreal w,
           const LCreal* x_data, const unsigned int nx,
           const LCreal* y_data, const unsigned int ny,
           const LCreal* z_data, const unsigned int nz,
           const LCreal* w_data, const unsigned int nw,
           const LCreal* a_data,
           const bool eFlg=false,
           unsigned int* const xbp=nullptr,
           unsigned int* const ybp=nullptr,
           unsigned int* const zbp=nullptr,
           unsigned int* const wbp=nullptr
          );

// ---
// 5D Linear Function Interpolator
//    Same as 4D plus ...
//       v       - Independent variable #5
//       v_data  - Table of independent variable #5 breakpoints
//       nv      - Size of v_data table
//       vbp     - Pointer to the previous V breakpoint (optional)
// ---

LCreal lfi_5D(const LCreal x, const LCreal y, const LCreal z, const LCreal w, const LCreal v,
         const LCreal* x_data, const unsigned int nx,
         const LCreal* y_data, const unsigned int ny,
         const LCreal* z_data, const unsigned int nz,
         const LCreal* w_data, const unsigned int nw,
         const LCreal* v_data, const unsigned int nv,
         const LCreal* a_data,
         const bool eFlg=false,
         unsigned int* const xbp=nullptr,
         unsigned int* const ybp=nullptr,
         unsigned int* const zbp=nullptr,
         unsigned int* const wbp=nullptr,
         unsigned int* const vbp=nullptr
      );

}
}
}

#endif

