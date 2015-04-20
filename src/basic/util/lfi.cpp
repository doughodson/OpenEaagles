
#include "openeaagles/basic/util/lfi.h"

namespace Eaagles {
namespace Basic {
namespace lfi {

//------------------------------------------------------------------------------
// lfi - One dimensional Linear Function Interpolator
//------------------------------------------------------------------------------
LCreal lfi_1D(
         const LCreal x,            // Independent variable #1
         const LCreal* x_data,      // Table of independent variable #1 breakpoints
         const unsigned int nx,     // Size of x_data table
         const LCreal* a_data,      // Table of dependent variable data
         const bool eFlg,           // Extrapolation is enabled beyond the table
         unsigned int* const xbp    // Previous X breakpoint (optional)
      )
{
   // ---
   // Only one point?
   // ---
   if (nx == 1) return a_data[0];   // Early out with the only point

   // ---
   // Check increasing vs decreasing order of the breakpoints
   // ---
   unsigned int low = 0;
   unsigned int high = nx - 1;
   int delta = 1;
   if (x_data[1] < x_data[0]) {
      // Reverse order of breakpoints
      low = nx - 1;
      high = 0;
      delta = -1;
   }

   // ---
   // Find the breakpoints with endpoint checks
   // ---
   unsigned int x2 = 0;
   if (x <= x_data[low]) {
      // At or below the 'low' end
      x2 = low + delta;
      if (!eFlg) {
         // No extrapolate; early out with the low end point
         if (xbp != nullptr) *xbp = x2;
         return(a_data[low]);
      }
   }
   else if (x >= x_data[high]) {
      // At or above the 'high' end
      x2 = high;
      if (!eFlg) {
         // No extrapolate; early out with the high end point
         if (xbp != nullptr) *xbp = x2;
         return(a_data[high]);
      }
   }
   else {
      if (xbp == nullptr) {
         // Simple linear search
         x2 = low + delta;
         while (x > x_data[x2]) { x2 += delta; }
      }
      else {
         // Start at the previous breakpoint
         x2 = *xbp;
         if (x2 >= nx) x2 = 0;                         // safety check
         while (x > x_data[x2]) { x2 += delta; }       // search up
         while (x < x_data[x2-delta]) { x2 -= delta; } // search down
         *xbp = x2;
      }
   }

   // ---
   // Linear interpolation
   // ---
   const unsigned int x1 = x2 - delta;
   const LCreal m = (x - x_data[x1]) / (x_data[x2] - x_data[x1]);
   return m * (a_data[x2] - a_data[x1]) + a_data[x1];
}

//------------------------------------------------------------------------------
// lfi - Two dimensional Linear Function Interpolator
//------------------------------------------------------------------------------
LCreal lfi_2D(
         const LCreal x,            // Independent variable #1
         const LCreal y,            // Independent variable #2
         const LCreal* x_data,      // Table of independent variable #1 breakpoints
         const unsigned int nx,     // Size of x_data table
         const LCreal* y_data,      // Table of independent variable #2 breakpoints
         const unsigned int ny,     // Size of y_data table
         const LCreal* a_data,      // Table of dependent variable data
         const bool eFlg,           // Extrapolation is enabled beyond the table
         unsigned int* const xbp,   // Previous X breakpoint (optional)
         unsigned int* const ybp    // Previous Y breakpoint (optional)
      )
{
   // ---
   // Only one point?
   // ---
   if (ny == 1) {
      return lfi_1D(x, x_data, nx, &a_data[0], eFlg, xbp);
   }

   // ---
   // Check increasing vs decreasing order of the breakpoints
   // ---
   unsigned int low = 0;
   unsigned int high = ny - 1;
   int delta = 1;
   if (y_data[1] < y_data[0]) {
      // Reverse order of breakpoints
      low = ny - 1;
      high = 0;
      delta = -1;
   }

   // ---
   // Find the breakpoints with endpoint checks
   // ---
   unsigned int y2 = 0;
   if (y <= y_data[low]) {
      // At or below the 'low' end
      y2 = low + delta;
      if (!eFlg) {
         // Early out at the 'low' end
         if (ybp != nullptr) *ybp = y2;
         unsigned int ax = nx * low;
         return lfi_1D(x, x_data, nx, &a_data[ax], eFlg, xbp);
      }
   }
   else if (y >= y_data[high]) {
      // At or above the 'high' end
      y2 = high;
      if (!eFlg) {
         // Early out with the 'high' end
         if (ybp != nullptr) *ybp = y2;
         unsigned int ax = nx * high;
         return lfi_1D(x, x_data, nx, &a_data[ax], eFlg, xbp);
      }
   }
   else {
      // Find the breakpoints we're between
      if (ybp == nullptr) {
         // Simple linear search
         y2 = low + delta;
         while (y > y_data[y2]) { y2 += delta; }
      }
      else {
         // Start at the previous breakpoint
         y2 = *ybp;
         if (y2 >= ny) y2 = 0;                         // safety check
         while (y > y_data[y2]) { y2 += delta; }       // search up
         while (y < y_data[y2-delta]) { y2 -= delta; } // search down
         *ybp = y2;
      }
   }

   // ---
   // Interpolate the breakpoints at one dimension lower
   // ---
   const unsigned int y1 = y2 - delta;
   const unsigned int ax1 = nx * y1;
   const LCreal a1 = lfi_1D(x, x_data, nx, &a_data[ax1], eFlg, xbp);
   const unsigned int ax2 = nx * y2;
   const LCreal a2 = lfi_1D(x, x_data, nx, &a_data[ax2], eFlg, xbp);

   // ---
   // Final linear interpolation
   // ---
   const LCreal m = (y - y_data[y1]) / (y_data[y2] - y_data[y1]);
   return m * (a2 - a1) + a1;
}

//------------------------------------------------------------------------------
// lfi - Three dimensional Linear Function Interpolator
//------------------------------------------------------------------------------
LCreal lfi_3D(
         const LCreal x,            // Independent variable #1
         const LCreal y,            // Independent variable #2
         const LCreal z,            // Independent variable #3
         const LCreal* x_data,      // Table of independent variable #1 breakpoints
         const unsigned int nx,     // Size of x_data table
         const LCreal* y_data,      // Table of independent variable #2 breakpoints
         const unsigned int ny,     // Size of y_data table
         const LCreal* z_data,      // Table of independent variable #3 breakpoints
         const unsigned int nz,     // Size of z_data table
         const LCreal* a_data,      // Table of dependent variable data
         const bool eFlg,           // Extrapolation is enabled beyond the table
         unsigned int* const xbp,   // Previous X breakpoint (optional)
         unsigned int* const ybp,   // Previous Y breakpoint (optional)
         unsigned int* const zbp    // Previous Z breakpoint (optional)
      )
{
   // ---
   // Only one point?
   // ---
   if (nz == 1) {
      return lfi_2D(x, y, x_data, nx, y_data, ny, &a_data[0], eFlg, xbp, ybp);
   }

   // ---
   // Check increasing vs decreasing order of the breakpoints
   // ---
   unsigned int low = 0;
   unsigned int high = nz - 1;
   int delta = 1;
   if (z_data[1] < z_data[0]) {
      // Reverse order of breakpoints
      low = nz - 1;
      high = 0;
      delta = -1;
   }

   // ---
   // Find the breakpoints with endpoint checks
   // ---
   unsigned int z2 = 0;
   if (z <= z_data[low]) {
      // At or below the 'low' end
      z2 = low + delta;
      if (!eFlg) {
         // Early out with the 'low' end
         if (zbp != nullptr) *zbp = z2;
         unsigned int ax = nx * ny * low;
         return lfi_2D(x, y, x_data, nx, y_data, ny, &a_data[ax], eFlg, xbp, ybp);
      }
   }
   else if (z >= z_data[high]) {
      // At or above the 'high' end
      z2 = high;
      if (!eFlg) {
         // Early out with the 'high' end
         if (zbp != nullptr) *zbp = z2;
         unsigned int ax = nx * ny * high;
         return lfi_2D(x, y, x_data, nx, y_data, ny, &a_data[ax], eFlg, xbp, ybp);
      }
   }
   else {
      // Find the breakpoints we're between
      if (zbp == nullptr) {
         // Simple linear search
         z2 = low + delta;
         while (z > z_data[z2]) { z2 += delta; }
      }
      else {
         // Start at the previous breakpoint
         z2 = *zbp;
         if (z2 >= nz) z2 = 0;                         // safety check
         while (z > z_data[z2]) { z2 += delta; }       // search up
         while (z < z_data[z2-delta]) { z2 -= delta; } // search down
         *zbp = z2;
      }
   }


   // ---
   // Interpolate the breakpoints at one dimension lower
   // ---
   const unsigned int z1 = z2 - delta;
   const unsigned int ax1 = nx * ny * z1;
   const LCreal a1 = lfi_2D(x, y, x_data, nx, y_data, ny, &a_data[ax1], eFlg, xbp, ybp);
   const unsigned int ax2 = nx * ny * z2;
   const LCreal a2 = lfi_2D(x, y, x_data, nx, y_data, ny, &a_data[ax2], eFlg, xbp, ybp);

   // ---
   // Final linear interpolation
   // ---
   const LCreal m = (z - z_data[z1]) / (z_data[z2] - z_data[z1]);
   return m * (a2 - a1) + a1;
}

//------------------------------------------------------------------------------
// lfi - Four dimension Linear Function Interpolator
//------------------------------------------------------------------------------
LCreal lfi_4D(
         const LCreal x,            // Independent variable #1
         const LCreal y,            // Independent variable #2
         const LCreal z,            // Independent variable #3
         const LCreal w,            // Independent variable #4
         const LCreal* x_data,      // Table of independent variable #1 breakpoints
         const unsigned int nx,     // Size of x_data table
         const LCreal* y_data,      // Table of independent variable #2 breakpoints
         const unsigned int ny,     // Size of y_data table
         const LCreal* z_data,      // Table of independent variable #3 breakpoints
         const unsigned int nz,     // Size of z_data table
         const LCreal* w_data,      // Table of independent variable #4 breakpoints
         const unsigned int nw,     // Size of w_data table
         const LCreal* a_data,      // Table of dependent variable data
         const bool eFlg,           // Extrapolation is enabled beyond the table
         unsigned int* const xbp,   // Previous X breakpoint (optional)
         unsigned int* const ybp,   // Previous Y breakpoint (optional)
         unsigned int* const zbp,   // Previous Z breakpoint (optional)
         unsigned int* const wbp    // Previous W breakpoint (optional)
      )
{
   // ---
   // Only one point?
   // ---
   if (nw == 1) {
      return lfi_3D(x, y, z, x_data, nx, y_data, ny, z_data, nz, &a_data[0], eFlg, xbp, ybp, zbp);
   }

   // ---
   // Check increasing vs decreasing order of the breakpoints
   // ---
   unsigned int low = 0;
   unsigned int high = nw - 1;
   int delta = 1;
   if (w_data[1] < w_data[0]) {
      // Reverse order of breakpoints
      low = nw - 1;
      high = 0;
      delta = -1;
   }

   // ---
   // Find the breakpoints with endpoint checks
   // ---
   unsigned int w2 = 0;
   if (w <= w_data[low]) {
      // At or below the 'low' end
      w2 = low + delta;
      if (!eFlg) {
         // Early out with the 'low' end
         if (wbp != nullptr) *wbp = w2;
         unsigned int ax = nx * ny * nz * low;
         return lfi_3D(x, y, z, x_data, nx, y_data, ny, z_data, nz, &a_data[ax], eFlg, xbp, ybp, zbp);
      }
   }
   else if (w >= w_data[high]) {
      // At or above the 'high' end
      w2 = high;
      if (!eFlg) {
         // Early out with the 'high' end
         if (wbp != nullptr) *wbp = w2;
         unsigned int ax = nx* ny * nz * high;
         return lfi_3D(x, y, z, x_data, nx, y_data, ny, z_data, nz, &a_data[ax], eFlg, xbp, ybp, zbp);
      }
   }
   else {
      // Find the breakpoints we're between
      //w2 = low + delta;
      //while (w >= w_data[w2]) { w2 += delta; }
      if (wbp == nullptr) {
         // Simple linear search
         w2 = low + delta;
         while (w > w_data[w2]) { w2 += delta; }
      }
      else {
         // Start at the previous breakpoint
         w2 = *wbp;
         if (w2 >= nw) w2 = 0;                         // safety check
         while (w > w_data[w2]) { w2 += delta; }       // search up
         while (w < w_data[w2-delta]) { w2 -= delta; } // search down
         *wbp = w2;
      }
   }

   // ---
   // Interpolate the breakpoints at one dimension lower
   // ---
   const unsigned int w1 = w2 - delta;
   const unsigned int ax1 = nx * ny * nz * w1;
   const LCreal a1 = lfi_3D(x, y, z, x_data, nx, y_data, ny, z_data, nz, &a_data[ax1], eFlg, xbp, ybp, zbp);
   const unsigned int ax2 = nx * ny * nz * w2;
   const LCreal a2 = lfi_3D(x, y, z, x_data, nx, y_data, ny, z_data, nz, &a_data[ax2], eFlg, xbp, ybp, zbp);

   // ---
   // Final linear interpolation
   // ---
   const LCreal m  = (w - w_data[w1]) / (w_data[w2] - w_data[w1]);
   return m * (a2 - a1) + a1;
}

//------------------------------------------------------------------------------
// lfi - Five dimension Linear Function Interpolator
//------------------------------------------------------------------------------
LCreal lfi_5D(
         const LCreal x,            // Independent variable #1
         const LCreal y,            // Independent variable #2
         const LCreal z,            // Independent variable #3
         const LCreal w,            // Independent variable #4
         const LCreal v,            // Independent variable #5
         const LCreal* x_data,      // Table of independent variable #1 breakpoints
         const unsigned int nx,     // Size of x_data table
         const LCreal* y_data,      // Table of independent variable #2 breakpoints
         const unsigned int ny,     // Size of y_data table
         const LCreal* z_data,      // Table of independent variable #3 breakpoints
         const unsigned int nz,     // Size of z_data table
         const LCreal* w_data,      // Table of independent variable #4 breakpoints
         const unsigned int nw,     // Size of w_data table
         const LCreal* v_data,      // Table of independent variable #5 breakpoints
         const unsigned int nv,     // Size of v_data table
         const LCreal* a_data,      // Table of dependent variable data
         const bool eFlg,           // Extrapolation is enabled beyond the table
         unsigned int* const xbp,   // Previous X breakpoint (optional)
         unsigned int* const ybp,   // Previous Y breakpoint (optional)
         unsigned int* const zbp,   // Previous Z breakpoint (optional)
         unsigned int* const wbp,   // Previous W breakpoint (optional)
         unsigned int* const vbp    // Previous V breakpoint (optional)
      )
{
   // ---
   // Only one point?
   // ---
   if (nv == 1) {
        return lfi_4D(x, y, z, w, x_data, nx, y_data, ny, z_data, nz, w_data, nw, &a_data[0], eFlg, xbp, ybp, zbp, wbp);
   }

   // ---
   // Check increasing vs decreasing order of the breakpoints
   // ---
   unsigned int low = 0;
   unsigned int high = nv - 1;
   int delta = 1;
   if (v_data[1] < v_data[0]) {
      // Reverse order of breakpoints
      low = nv - 1;
      high = 0;
      delta = -1;
   }

   // ---
   // Find the breakpoints with endpoint checks
   // ---
   unsigned int v2 = 0;
   if (v <= v_data[low]) {
      // At or below the 'low' end
      v2 = low + delta;
      if (!eFlg) {
         // Early out with the 'low' end
         if (vbp != nullptr) *vbp = v2;
         unsigned int ax = nx * ny * nz * nw * low;
         return lfi_4D(x, y, z, w, x_data, nx, y_data, ny, z_data, nz, w_data, nw, &a_data[ax], eFlg, xbp, ybp, zbp, wbp);
      }
   }
   else if (v >= v_data[high]) {
      // At or above the 'high' end
      v2 = high;
      if (!eFlg) {
         // Early out with the 'high' end
         if (vbp != nullptr) *vbp = v2;
         unsigned int ax = nx* ny * nz * nw * high;
         return lfi_4D(x, y, z, w, x_data, nx, y_data, ny, z_data, nz, w_data, nw, &a_data[ax], eFlg, xbp, ybp, zbp, wbp);
      }
   }
   else {
      // Find the breakpoints we're between
      //v2 = low + delta;
      //while (v >= v_data[v2]) { v2 += delta; }
      if (vbp == nullptr) {
         // Simple linear search
         v2 = low + delta;
         while (v > v_data[v2]) { v2 += delta; }
      }
      else {
         // Start at the previous breakpoint
         v2 = *vbp;
         if (v2 >= nv) v2 = 0;                         // safety check
         while (v > v_data[v2]) { v2 += delta; }       // search up
         while (v < v_data[v2-delta]) { v2 -= delta; } // search down
         *vbp = v2;
      }
   }


   // ---
   // Interpolate the breakpoints at one dimension lower
   // ---
   const unsigned int v1 = v2 - delta;
   const unsigned int ax1 = nx * ny * nz * nw * v1;
   const LCreal a1 = lfi_4D(x, y, z, w, x_data, nx, y_data, ny, z_data, nz, w_data, nw, &a_data[ax1], eFlg, xbp, ybp, zbp, wbp);
   const unsigned int ax2 = nx * ny * nz * nw * v2;
   const LCreal a2 = lfi_4D(x, y, z, w, x_data, nx, y_data, ny, z_data, nz, w_data, nw, &a_data[ax2], eFlg, xbp, ybp, zbp, wbp);

   // ---
   // Final linear interpolation
   // ---
   const LCreal m  = (v - v_data[v1]) / (v_data[v2] - v_data[v1]);
   return m * (a2 - a1) + a1;
}

}
}
}
