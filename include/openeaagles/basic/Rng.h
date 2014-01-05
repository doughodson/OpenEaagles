//------------------------------------------------------------------------------
// Class: Rng - Mersenne Twister random number generator
//------------------------------------------------------------------------------

#ifndef __Eaagles_Basic_Rng_H__
#define __Eaagles_Basic_Rng_H__

#include "openeaagles/basic/Object.h"
#include <cmath>

namespace Eaagles {
namespace Basic {

// C++ include file for MT19937, with initialization improved 2002/1/26.
// Coded by Takuji Nishimura and Makoto Matsumoto.
// Ported to C++ by Jasper Bedaux 2003/1/1 (see http://www.bedaux.net/mtrand/).
// The generators returning floating point numbers are based on
// a version by Isaku Wada, 2002/01/09
//
// Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// 3. The names of its contributors may not be used to endorse or promote
//    products derived from this software without specific prior written
//    permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Any feedback is very welcome.
// http://www.math.keio.ac.jp/matumoto/emt.html
// email: matumoto@math.keio.ac.jp
//
// Feedback about the C++ port should be sent to Jasper Bedaux,
// see http://www.bedaux.net/mtrand/ for e-mail address and info.

//
// class structure modified to include all draw methods in
// a single class. Also class name changed to MTRng
// lots of other mods as well... D. Hodson  18 Jun 2007
//

class Number;

//------------------------------------------------------------------------------
// Class:  Rng
// Description:  Random Number Generator
//
// Factory name: Rng
// Slots:
//    seed      <Number>  ! seed (default: 5489UL first instance only)
//------------------------------------------------------------------------------
class Rng : public Object
{
   DECLARE_SUBCLASS(Rng, Object)

public:

   //============================================================================
   // Rng class constructors
   //============================================================================

   //----
   // default constructor: uses default seed only if this is the first instance
   //----
   Rng();
   
   //----
   // constructor with 32 bit int as seed
   //----
   Rng(unsigned int s);

   //----
   // constructor with array of size 32 bit ints as seed
   //----
   Rng(const unsigned int* array, int size);


   //============================================================================
   // Rng class member functions
   //============================================================================

   //-----------------------------------------------------------------
   // the two seed functions
   //-----------------------------------------------------------------
   void seed(unsigned int);                  // seed with 32 bit integer
   void seed(const unsigned int*, int size); // seed with array

   //-----------------------------------------------------------------
   // drawInt32() -- generate 32 bit random integer
   //-----------------------------------------------------------------
   unsigned int drawInt32();
  
   //-----------------------------------------------------------------
   // draw() -- this will be defined in the distribution classes
   //-----------------------------------------------------------------
   virtual double draw();
  
   //-----------------------------------------------------------------
   // drawClosed() -- generates double floating point numbers in the 
   // closed interval [0, 1]
   // mean = 1/2; variance = 1/12
   // 4294967295 = 2^32 - 1
   //-----------------------------------------------------------------
   double drawClosed();
  
   //-----------------------------------------------------------------
   // drawOpen() -- generates double floating point numbers in the 
   // open interval (0, 1)
   // mean = 1/2; variance = 1/12
   // 4294967296 = 2^32
   //-----------------------------------------------------------------
   double drawOpen();
  
   //-----------------------------------------------------------------
   // drawHalfOpen() -- generates double floating point numbers in the
   // half-open interval [0, 1)
   // mean = 1/2; variance = 1/12
   // 4294967296 = 2^32
   //-----------------------------------------------------------------
   double drawHalfOpen();
  
   //-----------------------------------------------------------------
   // drawHalfOpen53() -- generates 53 bit resolution doubles in the 
   // half-open interval [0, 1)
   // mean = 1/2; variance = 1/12
   // 67108864 = 2^26
   // 9007199254740992 = 2^53
   //-----------------------------------------------------------------
   double drawHalfOpen53();


   //-----------------------------------------------------------------
   // drawExponential - Exponential Probability Distribution Function
   // generates double floating point numbers in the half open interval 
   // [0, infinity)
   // mean = 1/lambda; variance = 1/(lambda^2) 
   //-----------------------------------------------------------------
   double drawExponential(const double lambda=1.0);

   //-----------------------------------------------------------------
   // drawGeometric - Geometric Probability Distribution Function
   // generates double floating point numbers in the half open interval 
   // [0, infinity)
   // mean = (1-p)/p; variance = (1-p)/(p^2) 
   //-----------------------------------------------------------------
   unsigned int drawGeometric(const double p=0.0);

   //-----------------------------------------------------------------
   // drawUniformCont - Continuous Uniform Probability Distribution Function
   // generates double floating point numbers in the interval [a, b] 
   // mean = (a + b)/2; variance = ((b - a)^2)/12
   //-----------------------------------------------------------------
   double drawUniformCont(const double a=0, const double b=1.0);

   //-----------------------------------------------------------------
   // drawUniformDisc - Discrete Uniform Probability Distribution Function
   // generates double floating point numbers in the interval [a, b] 
   // mean = (a + b)/2; variance = ((b - a)^2)/12
   //-----------------------------------------------------------------
   int drawUniformDisc(const int i=0, const int j=1);

   //-----------------------------------------------------------------
   // drawRayleigh - Rayleigh Probability Distribution Function
   // generates double floating point numbers in the half open interval 
   // [0, infinity)
   // mean = alpha*sqrt(PI/2); variance = (alpha^2)*(2 - PI/2)
   //-----------------------------------------------------------------
   double drawRayleigh(const double alpha=1.0);

   //-----------------------------------------------------------------
   // drawGauss() - Gauss (Normal) Probability Distribution Function
   // generates double floating point numbers in the open interval 
   // (-infinity, infinity)
   // mean = mu; variance = sigma^2
   //-----------------------------------------------------------------
   double drawGauss(const double mu=0.0, const double sigma=1.0);

   //-----------------------------------------------------------------
   // drawBernoulli() - Bernoulli Probability Distribution Function
   // generates double floating point numbers in the open interval 
   // (0, infinity)
   // mean = p; variance = p*(1 - p) 
   //-----------------------------------------------------------------
   unsigned int drawBernoulli(const double p);

   //-----------------------------------------------------------------
   // drawBinomial() - Binomial Probability Distribution Function
   // generates double floating point numbers in the half open interval 
   // [0, infinity)
   // The Binomial distribution is the sum of n independent, identically
   // distributed Bernoulli random variables
   // mean = np; variance = np*(1 - p) 
   //-----------------------------------------------------------------
   double drawBinomial(const int n, const double p);

   //-----------------------------------------------------------------
   // drawPoisson() - Poisson Probability Distribution Function
   // generates double floating point numbers in the half open interval 
   // [0, infinity)
   // mean = alpha; variance = alpha 
   //-----------------------------------------------------------------
   unsigned int drawPoisson(const double alpha=1.0);

   //-----------------------------------------------------------------
   // drawGamma() - Gamma Probability Distribution Function
   // generates double floating point numbers in the open interval 
   // (0, infinity)
   // mean = alpha/beta; variance = alpha/(beta^2) 
   //-----------------------------------------------------------------
   double drawGamma(const double alpha=1.0, const double beta=1.0);
   
   //-----------------------------------------------------------------
   // drawErlang - Erlang Probability Distribution Function
   // generates double floating point numbers in the half open interval 
   // [0, infinity)
   // mean = a; variance = ?? 
   //-----------------------------------------------------------------
   double drawErlang(const unsigned int m, const double a);


   //----
   // Slot functions
   //----
   bool setSlotSeed(const Number* const);

private:

   //----
   // compile time constants
   //----
   static const int n = 624, m = 397;

   //----
   // the variables below are static (no duplicates can exist)
   //----
   static unsigned int state[n];   // state vector array
   static int p;                    // position in state array
   static bool init;

   //----
   // private functions used to generate the pseudo random numbers
   //----
   unsigned int twiddle(unsigned int, unsigned int);     // used by gen_state()
   void gen_state();                                     // generate new state

};


//==============================================================================
// Inline Code
//==============================================================================
#include "openeaagles/basic/Rng.inl"


}
}

#endif


