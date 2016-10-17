
#ifndef __oe_base_util_lfi_utils_H__
#define __oe_base_util_lfi_utils_H__

//------------------------------------------------------------------------------
// Linear Function Interpolators
//------------------------------------------------------------------------------

namespace oe {
namespace base {

// ---
// 1D Linear Function Interpolator
//    x       - Independent variable #1
//    x_data  - Table of independent variable #1 breakpoints
//    nx      - Size of x_data table
//    a_data  - Table of dependent variable data
//    eFlg    - Extrapolation enabled flag (optional)
//    xbp     - Pointer to the previous X breakpoint (optional)
// ---

double lfi_1D(const double x,
           const double* x_data, const unsigned int nx,
           const double* a_data,
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

double lfi_2D(const double x, const double y,
           const double* x_data, const unsigned int nx,
           const double* y_data, const unsigned int ny,
           const double* a_data,
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

double lfi_3D(const double x, const double y, const double z,
           const double* x_data, const unsigned int nx,
           const double* y_data, const unsigned int ny,
           const double* z_data, const unsigned int nz,
           const double* a_data,
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

double lfi_4D(const double x, const double y, const double z, const double w,
           const double* x_data, const unsigned int nx,
           const double* y_data, const unsigned int ny,
           const double* z_data, const unsigned int nz,
           const double* w_data, const unsigned int nw,
           const double* a_data,
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

double lfi_5D(const double x, const double y, const double z, const double w, const double v,
         const double* x_data, const unsigned int nx,
         const double* y_data, const unsigned int ny,
         const double* z_data, const unsigned int nz,
         const double* w_data, const unsigned int nw,
         const double* v_data, const unsigned int nv,
         const double* a_data,
         const bool eFlg=false,
         unsigned int* const xbp=nullptr,
         unsigned int* const ybp=nullptr,
         unsigned int* const zbp=nullptr,
         unsigned int* const wbp=nullptr,
         unsigned int* const vbp=nullptr
      );

}
}

#endif

