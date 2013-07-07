//------------------------------------------------------------------------------
// Class: Rng
// inline functions
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Rng_Inline__
#define __Eaagles_Basic_Rng_Inline__

//-----------------------------------------------------------------
// drawClosed() -- generates double floating point numbers in the 
// closed interval [0, 1]
// mean = 1/2; variance = 1/12
// 4294967295 = 2^32 - 1
//-----------------------------------------------------------------
inline double Rng::drawClosed()
{
   return static_cast<double>(drawInt32()) * (1. / 4294967295.);
}

//-----------------------------------------------------------------
// drawOpen() -- generates double floating point numbers in the 
// open interval (0, 1)
// mean = 1/2; variance = 1/12
// 4294967296 = 2^32
//-----------------------------------------------------------------
inline double Rng::drawOpen()
{
   return (static_cast<double>(drawInt32()) + .5) * (1. / 4294967296.);
}

//-----------------------------------------------------------------
// drawHalfOpen() -- generates double floating point numbers in the
// half-open interval [0, 1)
// mean = 1/2; variance = 1/12
// 4294967296 = 2^32
//-----------------------------------------------------------------
inline double Rng::drawHalfOpen()
{
   return static_cast<double>(drawInt32()) * (1. / 4294967296.);
}

//-----------------------------------------------------------------
// drawHalfOpen53() -- generates 53 bit resolution doubles in the 
// half-open interval [0, 1)
// mean = 1/2; variance = 1/12
// 67108864 = 2^26
// 9007199254740992 = 2^53
//-----------------------------------------------------------------
inline double Rng::drawHalfOpen53()
{
   return (static_cast<double>(drawInt32() >> 5) * 67108864. +
      static_cast<double>(drawInt32() >> 6)) * (1. / 9007199254740992.);
}


//-----------------------------------------------------------------
// drawExponential - Exponential Probability Distribution Function
// generates double floating point numbers in the half open interval 
// [0, infinity)
// mean = 1/lambda; variance = 1/(lambda^2) 
//-----------------------------------------------------------------
inline double Rng::drawExponential(const double lambda)
{
   double e = 0.0;
   if (lambda != 0.0) {
      double u = drawOpen();
      e = -std::log(u) / lambda;  // u != 0
   }
   return e;
}

//-----------------------------------------------------------------
// drawGeometric - Geometric Probability Distribution Function
// generates double floating point numbers in the half open interval 
// [0, infinity)
// mean = (1-p)/p; variance = (1-p)/(p^2) 
//-----------------------------------------------------------------
inline unsigned int Rng::drawGeometric(const double p)
{
   unsigned int g = 0;
   bool ok = ( (0.0 < p) && (p < 1.0) );
   if (ok) {
      double u = drawOpen();
      double num = std::log(u);        // log(0) undefined => u in (0,1)
      double den = std::log(1.0 - p);  // log(0) undefined => p in (0,1)
      g = (unsigned int) (num/den); 
   }
   return g;
}

//-----------------------------------------------------------------
// drawUniformCont - Continuous Uniform Probability Distribution Function
// generates double floating point numbers in the interval [a, b] 
// mean = (a + b)/2; variance = ((b - a)^2)/12
//-----------------------------------------------------------------
inline double Rng::drawUniformCont(const double a, const double b)
{
   //Basic::Rng* pU = new Basic::Rng;
   double u = 0.0;
   if (a <= b) {
      u = a + (b - a)*drawClosed();
   }
   return u;
}

//-----------------------------------------------------------------
// drawUniformDisc - Discrete Uniform Probability Distribution Function
// generates double floating point numbers in the interval [a, b] 
// mean = (a + b)/2; variance = ((b - a)^2)/12
//-----------------------------------------------------------------
inline int Rng::drawUniformDisc(const int i, const int j)
{
   int k = 0;
   bool ok = (i <= j);
   if (ok) {
      k = int( (j - i + 1.0)*drawClosed() ) + i;
   }
   return k;
}

//-----------------------------------------------------------------
// drawRayleigh - Rayleigh Probability Distribution Function
// generates double floating point numbers in the half open interval 
// [0, infinity)
// mean = alpha*sqrt(PI/2); variance = (alpha^2)*(2 - PI/2)
//-----------------------------------------------------------------
inline double Rng::drawRayleigh(const double alpha)
{
   double r = 0.0;
   if (alpha > 0.0) {
      double u = drawOpen();
      r = alpha * std::sqrt(-2.0 * std::log(u));  // u in (0,1)
   }
   return r;
}

//-----------------------------------------------------------------
// drawGauss() - Gauss (Normal) Probability Distribution Function
// generates double floating point numbers in the open interval 
// (-infinity, infinity)
// mean = mu; variance = sigma^2
//-----------------------------------------------------------------
inline double Rng::drawGauss(const double mu, const double sigma)
{
   static double z = 0.0;

   double x = 0.0;
   double y = 0.0;
   double r = 0.0;
   double g = 0.0;

//   std::cout << "z = " << z << std::endl;

   if (z == 0.0) {
      do {
         x = 2.0*drawClosed() - 1.0;         // x in [-1,1]
         y = 2.0*drawClosed() - 1.0;         // y in [-1,1]
         r = x*x + y*y;                      // r in [0,2]
      } while ( (r == 0.0) || (r >= 1.0));

      r = std::sqrt((-2.0*std::log(r)) / r); // r in [ (-inf,0) + (0,+inf) ]
      z = r*x;
      g = (mu + z*sigma);  // z = r*x calculated in this drawGauss is
      z = r*y;             // used in next drawGauss
   }
   else {
      g = (mu + z*sigma);  // z = r*y calculated in previous drawGauss 
      z = 0.0;             // is used in next drawGauss
   }

   return g; 
}

//-----------------------------------------------------------------
// drawBernoulli() - Bernoulli Probability Distribution Function
// generates double floating point numbers in the open interval 
// (0, infinity)
// mean = p; variance = p*(1 - p) 
//-----------------------------------------------------------------
inline unsigned int Rng::drawBernoulli(const double p)
{
   unsigned int q = 0;
//   bool ok = ( (0.0 <= p) && (p <= 1.0) );
   if (drawClosed() <= p) {
      q = 1;
   }
   return q;
}

//-----------------------------------------------------------------
// drawBinomial() - Binomial Probability Distribution Function
// generates double floating point numbers in the half open interval 
// [0, infinity)
// The Binomial distribution is the sum of n independent, identically
// distributed Bernoulli random variables
// mean = np; variance = np*(1 - p) 
//-----------------------------------------------------------------
inline double Rng::drawBinomial(const int n, const double p)
{
   int i = 0;
   double q = std::pow(1.0 - p, (double) n);
   double f = q;
   double u = drawOpen();
   while (f <= u) {
      q *= ( p*(n - 1) / ((i + 1.0) * (1.0 - p)) );
      f += q;
      i++;
   }
   return (i);
}

//-----------------------------------------------------------------
// drawPoisson() - Poisson Probability Distribution Function
// generates double floating point numbers in the half open interval 
// [0, infinity)
// mean = alpha; variance = alpha 
//-----------------------------------------------------------------
inline unsigned int Rng::drawPoisson(const double alpha)
{
   unsigned int i = 0;
   double p = std::exp(-alpha);
   double f = p;
   double u = drawHalfOpen();
   while (f <= u) {
      p *= ( alpha / (i + 1.0) );
      f += p;
      i++;
   }
   return i;
}

//-----------------------------------------------------------------
// drawGamma() - Gamma Probability Distribution Function
// generates double floating point numbers in the open interval 
// (0, infinity)
// mean = alpha/beta; variance = alpha/(beta^2) 
//-----------------------------------------------------------------
inline double Rng::drawGamma(const double /*alpha*/, const double /*beta*/)
{

   return 0.0;
}

//-----------------------------------------------------------------
// drawErlang - Erlang Probability Distribution Function
// generates double floating point numbers in the half open interval 
// [0, infinity)
// mean = a; variance = ?? 
//-----------------------------------------------------------------
inline double Rng::drawErlang(const unsigned int m, const double a)
{
   double w = 1.0;
   for (unsigned int i = 0; i < m; i++) {
      w *= drawOpen();
   }
   double e = -(a/m)*std::log(w);
   return e;
}

//----
// generate 32 bit random int
//----
inline unsigned int Rng::drawInt32()
{ 
   if (p == n) gen_state(); // new state vector needed
   // gen_state() is split off to be non-inline, because it is only called once
   // in every 624 calls and otherwise irand() would become too big to get inlined

   unsigned int x = state[p++];
   x ^= (x >> 11);
   x ^= (x << 7) & 0x9D2C5680UL;
   x ^= (x << 15) & 0xEFC60000UL;
   
   return ( x ^ (x >> 18) );
}

//----
// inline for speed, must therefore reside in header file
//----
inline unsigned int Rng::twiddle(unsigned int u, unsigned int v)
{
#if 1
   return (((u & 0x80000000) | (v & 0x7FFFFFFF)) >> 1)
      ^ ((v & 1) ? 0x9908B0DF : 0x0);
#else
   unsigned int n1 = u & 0x80000000;
   unsigned int n2 = v & 0x7FFFFFFF;
   unsigned int n3 = v & 1;
   unsigned int n4 = 0x9908B0DF;
   
   return ( ((n1 | n2) >> 1) ^ ((n3 ? n4 : 0x0)) );
#endif
}

#endif

