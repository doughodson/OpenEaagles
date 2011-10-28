
#include "openeaagles/basic/Rand.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------
// class RngStream
//------------------------------------------------------------
class RngStream
{
public:
    RngStream (const char *name = "");
    static bool SetPackageSeed (const unsigned int seed[6]);
    void ResetStartStream ();
    void ResetStartSubstream ();
    void ResetNextSubstream ();
    void SetAntithetic (bool a);
    void IncreasedPrecis (bool incp);
    bool SetSeed (const unsigned int seed[6]);
    void AdvanceState (int e, int c);
    void GetState (unsigned int seed[6]) const;
    void WriteState () const;
    void WriteStateFull () const;
    double RandU01 ();
    int RandInt (int i, int j);

private:
    double Cg[6], Bg[6], Ig[6];
    bool anti, incPrec;
    std::string name;
    static double nextSeed[6];
    double U01 ();
    double U01d ();
};


//===========================================================
//
//Class: Rand
//
//===========================================================

IMPLEMENT_SUBCLASS(Rand,"Rand")
EMPTY_SERIALIZER(Rand)

// Slot Table
BEGIN_SLOTTABLE(Rand)
    "Seed",                  // 1: seed for random number generator
    "Antithetic",            // 2: boolean for antithetic operation (return 1-U instead of U) true if > 0, false if <= 0. Default is False.
    "useIncreasedPrecision"  // 3: boolean for using 53bit (double) precision instead of 32 bit - slower. same operation as slot #2.
END_SLOTTABLE(Rand)

// Map slot table to handles
BEGIN_SLOT_MAP(Rand)
    ON_SLOT(1,setSeed,Number)
    ON_SLOT(2,setSlotAntithetic,Number)
    ON_SLOT(3,setSlotIncreasedPrecision,Number)
END_SLOT_MAP()

// Constructor
Rand::Rand()
{
    STANDARD_CONSTRUCTOR()

    prng = new RngStream;
    seed = new Number;
    seed->setValue(12345); //default seed if none is specified.
    nextRand = 0;
}

void Rand::copyData(const Rand& org, const bool cc)
{
    BaseClass::copyData(org);
    //if copy constructor, init the following...
    if(cc) {
        //nothing to init.
    }
    //there is no copy constructor for RngStream - copying is
    //therefore considered dangerous and will almost certainly fail.
    //the way PRNGs will be handled in OpenEaagles is such that a copy
    //of a given PRNG should not ever be needed.
    seed->setValue(org.seed->getInt());
    nextRand = org.nextRand;
}

void Rand::deleteData()
{
    if (prng != 0) {
	      delete prng;
	      prng = 0;
    }
    if(seed != 0){
        seed->unref();
        seed = 0;
    }
}

int Rand::getSeed()
{
    return seed->getInt();
}

double Rand::getNextRand()
{
    calculateNextRand();
    return nextRand;
}

void Rand::calculateNextRand()
{
    nextRand = prng->RandU01();
}

double Rand::peekRand()
{
    return nextRand;
}

void Rand::setRand(double num)
{
    nextRand = num;
}

bool Rand::setSeed(Number* num)
{
    seed->setValue(num->getInt());
    unsigned int mySeed = seed->getInt();
    srand(mySeed);
    unsigned int seeds[6];
    for(int i=0; i<6; i++) {
        seeds[i] = (unsigned int)rand();
    }
    prng->SetPackageSeed(seeds);
    return true;
}

bool Rand::setSlotAntithetic(Number* num)
{
    bool test = false;
    if(num->getInt() > 0) { test = true; }
    prng->SetAntithetic(test);
    return true;
}

bool Rand::setSlotIncreasedPrecision(Number* num)
{
    bool test = false;
    if(num->getInt() > 0) { test = true; }
    prng->IncreasedPrecis(test);
    return true;
}

Object* Rand::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//===========================================================
//
//Class: UniformRand
//
//===========================================================

IMPLEMENT_SUBCLASS(UniformRand,"UniformRand")
EMPTY_SERIALIZER(UniformRand)

// Slot Table
BEGIN_SLOTTABLE(UniformRand)
    "Min",  // 1: the Min.
    "Max"   // 2: the Max.
END_SLOTTABLE(UniformRand)

// Map slot table to handles
BEGIN_SLOT_MAP(UniformRand)
    ON_SLOT(1,setMin,Number)
    ON_SLOT(2,setMax,Number)
END_SLOT_MAP()

// Constructor
UniformRand::UniformRand()
{
    STANDARD_CONSTRUCTOR()

    min = new Number;
    min->setValue(0);
    max = new Number;
    max->setValue(1);
}

void UniformRand::copyData(const UniformRand& org, const bool cc)
{
    BaseClass::copyData(org);
    //if copy constructor, init the following...
    if(cc) {
        //nothing to init.
    }
    //see note in Rand::copyData
    min->setValue(org.min->getDouble());
    max->setValue(org.max->getDouble());
}

void UniformRand::deleteData()
{
    if (min != 0) {
      min->unref();
      min = 0;
    }
    if (max != 0) {
      max->unref();
      max = 0;
    }
}

double UniformRand::getMin()
{
    return min->getDouble();
}

double UniformRand::getMax()
{
    return max->getDouble();
}

void UniformRand::calculateNextRand()
{
    BaseClass::calculateNextRand(); //I don't know if we can do this... 
    double modifyRand = peekRand();
    setRand(min->getDouble() + ((max->getDouble() - min->getDouble()) * modifyRand));
}

bool UniformRand::setMin(Number* num)
{
    min->setValue(num->getDouble());
    return true;
}

bool UniformRand::setMax(Number* num)
{
    max->setValue(num->getDouble());
    return true;
}

Object* UniformRand::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//===========================================================
//
//Class: NormalRand
//
//===========================================================

IMPLEMENT_SUBCLASS(NormalRand,"NormalRand")
EMPTY_SERIALIZER(NormalRand)

// Slot Table
BEGIN_SLOTTABLE(NormalRand)
    "Mean",  // 1: the Mean. (mu)
    "StdDev" // 2: the Standard Deviation. (sigma)
END_SLOTTABLE(NormalRand)

// Map slot table to handles
BEGIN_SLOT_MAP(NormalRand)
    ON_SLOT(1,setMean,Number)
    ON_SLOT(2,setStdDev,Number)
END_SLOT_MAP()

// Constructor
NormalRand::NormalRand()
{
    STANDARD_CONSTRUCTOR()

    mean = new Number;
    mean->setValue(0);
    stdDev = new Number;
    stdDev->setValue(0);
    secondRand = 0;
    secondFlag = false;
}

void NormalRand::copyData(const NormalRand& org, const bool cc)
{
    BaseClass::copyData(org);
    //if copy constructor, init the following...
    if(cc) {
        //nothing to init.
    }
    //see note in Rand::copyData
    mean->setValue(org.mean->getDouble());
    stdDev->setValue(org.stdDev->getDouble());
    secondRand = org.secondRand;
    secondFlag = org.secondFlag;
}

void NormalRand::deleteData()
{
    if (mean != 0) {
        mean->unref();
        mean = 0;
    }
    if (stdDev != 0) {
        stdDev->unref();
        stdDev = 0;
    }
}

double NormalRand::getMean()
{
    return mean->getDouble();
}

double NormalRand::getStdDev()
{
    return stdDev->getDouble();
}

void NormalRand::calculateNextRand()
{
    if(secondFlag == false){
        double sum = 10;
        double v1 = 0;
        double v2 = 0;
        double modifyRand = 0;
        double otherRand = 0;
        while(sum > 1){ //Step 1: generate U1 and U2 (modifyRand and otherRand) and calculate v1 and v2, go to step two when the sum of their squares is less than 1.
            BaseClass::calculateNextRand();
            modifyRand = peekRand();
            //do stuff to modifyRand right here...
            BaseClass::calculateNextRand(); //draw a second random number to use as a pair to generate the normal rands.
            otherRand = peekRand();
            v1 = (2 * modifyRand) - 1;
            v2 = (2 * otherRand) - 1;
            sum = pow(v1,2) + pow(v2,2);
        }
        //step 2: let Y = sqrt((-2* log(sum))/sum) and calculate x1 and x2 as Xi = Vi * Y. (modifyRand will be asigned the value of X1 and secondRand will be assigned the value of X2)
        //step 3: (also included: take x1 and x2 and do ((Xi* stdDev) + mean)
        double Y = sqrt((-2 * log(sum))/sum);
        modifyRand = ((v1 * Y) * stdDev->getDouble()) + mean->getDouble();
        setRand(modifyRand);
        secondRand = ((v2 * Y) * stdDev->getDouble()) + mean->getDouble();
        secondFlag = true;
    }
    else{
        setRand(secondRand);
        secondFlag = false;
    }
}

bool NormalRand::setMean(Number* num)
{
    mean->setValue(num->getDouble());
    return true;
}

bool NormalRand::setStdDev(Number* num)
{
    stdDev->setValue(num->getDouble());
    return true;
}

Object* NormalRand::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//===========================================================
//
//Class: ExponentialRand
//
//===========================================================

IMPLEMENT_SUBCLASS(ExponentialRand,"ExponentialRand")
EMPTY_SERIALIZER(ExponentialRand)

// Slot Table
BEGIN_SLOTTABLE(ExponentialRand)
    "Mean"//,  // 1: the Mean.
END_SLOTTABLE(ExponentialRand)

// Map slot table to handles
BEGIN_SLOT_MAP(ExponentialRand)
    ON_SLOT(1,setMean,Number)
END_SLOT_MAP()

// Constructor
ExponentialRand::ExponentialRand()
{
    STANDARD_CONSTRUCTOR()

    mean = new Number;
    mean->setValue(0);
}

void ExponentialRand::copyData(const ExponentialRand& org, const bool cc)
{
    BaseClass::copyData(org);
    //if copy constructor, init the following...
    if(cc) {
        //nothing to init.
    }
    //see note in Rand::copyData
    mean->setValue(org.mean->getDouble());
}

void ExponentialRand::deleteData()
{
    if (mean != 0) {
        mean->unref();
        mean = 0;
    }
}

double ExponentialRand::getMean()
{
    return mean->getDouble();
}

void ExponentialRand::calculateNextRand()
{
    BaseClass::calculateNextRand();
    double modifyRand = peekRand();
    double logModRand = -mean->getDouble() * log(modifyRand);
    setRand(logModRand);
}

bool ExponentialRand::setMean(Number* num)
{
    mean->setValue(num->getDouble());
    return true;
}

Object* ExponentialRand::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//===========================================================
//
//Class: TriangularRand ------> THIS ONE DOESN'T WORK YET!!!
//
//===========================================================

//IMPLEMENT_SUBCLASS(TriangularRand,"TriangularRand")
//EMPTY_SERIALIZER(TriangularRand)
//
//// Slot Table
//BEGIN_SLOTTABLE(TriangularRand)
//    "Min",  // 1: the Min.
//    "Max",  // 2: the Max.
//    "Mode"  // 3: the Mode.
//END_SLOTTABLE(TriangularRand)
//
//// Map slot table to handles
//BEGIN_SLOT_MAP(TriangularRand)
//    ON_SLOT(1,setMin,Number)
//    ON_SLOT(2,setMax,Number)
//    ON_SLOT(3,setMode,Number)
//END_SLOT_MAP()
//
//// Constructor
//TriangularRand::TriangularRand()
//{
//    STANDARD_CONSTRUCTOR()
//
//    min = new Number;
//    min->setValue(0);
//    max = new Number;
//    max->setValue(0);
//    mode = new Number;
//    mode->setValue(0);
//}
//
//void TriangularRand::copyData(const TriangularRand& org, const bool cc)
//{
//    BaseClass::copyData(org);
//    //if copy constructor, init the following...
//    if(cc) {
//        //nothing to init.
//    }
//    //see note in Rand::copyData
//    min->setValue(org.min->getDouble());
//    max->setValue(org.max->getDouble());
//    mode->setValue(org.mode->getDouble());
//}
//
//void TriangularRand::deleteData()
//{
//    min->unref();
//    min = 0;
//    max->unref();
//    max = 0;
//    mode->unref();
//    mode = 0;
//}
//
//double TriangularRand::getMin()
//{
//    return min->getDouble();
//}
//
//double TriangularRand::getMax()
//{
//    return max->getDouble();
//}
//
//double TriangularRand::getMode()
//{
//    return mode->getDouble();
//}
//
//void TriangularRand::calculateNextRand()
//{
//    BaseClass::calculateNextRand();
//    double modifyRand = peekRand();
//    //do stuff to modifyRand right here...
//    modifyRand = min->getDouble() + ((max->getDouble() - min->getDouble()) * modifyRand);
//    if(modifyRand > mode->getDouble()){
//        //modifyRand = 1 - (sqrt((max->getDouble() - mode->getDouble())*(max->getDouble() - modifyRand)));
//        modifyRand = 1 - (sqrt((1 - mode->getDouble())*(1 - modifyRand)));
//    }
//    else{
//        modifyRand = sqrt(mode->getDouble() * modifyRand);
//    }
//    setRand(modifyRand);
//}
//
//bool TriangularRand::setMin(Number* num)
//{
//    min->setValue(num->getDouble());
//    return true;
//}
//
//bool TriangularRand::setMax(Number* num)
//{
//    max->setValue(num->getDouble());
//    return true;
//}
//
//bool TriangularRand::setMode(Number* num)
//{
//    mode->setValue(num->getDouble());
//    return true;
//}
//
//Object* TriangularRand::getSlotByIndex(const int si)
//{
//    return BaseClass::getSlotByIndex(si);
//}

namespace
{
const double m1   =       4294967087.0;
const double m2   =       4294944443.0;
const double norm =       1.0 / (m1 + 1.0);
const double a12  =       1403580.0;
const double a13n =       810728.0;
const double a21  =       527612.0;
const double a23n =       1370589.0;
const double two17 =      131072.0;
const double two53 =      9007199254740992.0;
const double fact =       5.9604644775390625e-8;     /* 1 / 2^24  */

// The following are the transition matrices of the two MRG components
// (in matrix form), raised to the powers -1, 1, 2^76, and 2^127, resp.

const double InvA1[3][3] = {          // Inverse of A1p0
       { 184888585.0,   0.0,  1945170933.0 },
       {         1.0,   0.0,           0.0 },
       {         0.0,   1.0,           0.0 }
       };

const double InvA2[3][3] = {          // Inverse of A2p0
       {      0.0,  360363334.0,  4225571728.0 },
       {      1.0,          0.0,           0.0 },
       {      0.0,          1.0,           0.0 }
       };

const double A1p0[3][3] = {
       {       0.0,        1.0,       0.0 },
       {       0.0,        0.0,       1.0 },
       { -810728.0,  1403580.0,       0.0 }
       };

const double A2p0[3][3] = {
       {        0.0,        1.0,       0.0 },
       {        0.0,        0.0,       1.0 },
       { -1370589.0,        0.0,  527612.0 }
       };

const double A1p76[3][3] = {
       {      82758667.0, 1871391091.0, 4127413238.0 },
       {    3672831523.0,   69195019.0, 1871391091.0 },
       {    3672091415.0, 3528743235.0,   69195019.0 }
       };

const double A2p76[3][3] = {
       {    1511326704.0, 3759209742.0, 1610795712.0 },
       {    4292754251.0, 1511326704.0, 3889917532.0 },
       {    3859662829.0, 4292754251.0, 3708466080.0 }
       };

const double A1p127[3][3] = {
       {    2427906178.0, 3580155704.0,  949770784.0 },
       {     226153695.0, 1230515664.0, 3580155704.0 },
       {    1988835001.0,  986791581.0, 1230515664.0 }
       };

const double A2p127[3][3] = {
       {    1464411153.0,  277697599.0, 1610723613.0 },
       {      32183930.0, 1464411153.0, 1022607788.0 },
       {    2824425944.0,   32183930.0, 2093834863.0 }
       };



//-------------------------------------------------------------------------
// Return (a*s + c) MOD m; a, s, c and m must be < 2^35
//
double MultModM (double a, double s, double c, double m)
{
    double v;
    int a1;

    v = a * s + c;

    if (v >= two53 || v <= -two53) {
        a1 = static_cast<int> (a / two17);    a -= a1 * two17;
        v  = a1 * s;
        a1 = static_cast<int> (v / m);     v -= a1 * m;
        v = v * two17 + a * s + c;
    }

    a1 = static_cast<int> (v / m);
    /* in case v < 0)*/
    if ((v -= a1 * m) < 0.0) return v += m;   else return v;
}


//-------------------------------------------------------------------------
// Compute the vector v = A*s MOD m. Assume that -m < s[i] < m.
// Works also when v = s.
//
void MatVecModM (const double A[3][3], const double s[3], double v[3],
                 double m)
{
    int i;
    double x[3];               // Necessary if v = s

    for (i = 0; i < 3; ++i) {
        x[i] = MultModM (A[i][0], s[0], 0.0, m);
        x[i] = MultModM (A[i][1], s[1], x[i], m);
        x[i] = MultModM (A[i][2], s[2], x[i], m);
    }
    for (i = 0; i < 3; ++i)
        v[i] = x[i];
}


//-------------------------------------------------------------------------
// Compute the matrix C = A*B MOD m. Assume that -m < s[i] < m.
// Note: works also if A = C or B = C or A = B = C.
//
void MatMatModM (const double A[3][3], const double B[3][3],
                 double C[3][3], double m)
{
    int i, j;
    double V[3], W[3][3];

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j)
            V[j] = B[j][i];
        MatVecModM (A, V, V, m);
        for (j = 0; j < 3; ++j)
            W[j][i] = V[j];
    }
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            C[i][j] = W[i][j];
}


//-------------------------------------------------------------------------
// Compute the matrix B = (A^(2^e) Mod m);  works also if A = B. 
//
void MatTwoPowModM (const double A[3][3], double B[3][3], double m, int e)
{
   int i, j;

   /* initialize: B = A */
   if (A != B) {
      for (i = 0; i < 3; ++i)
         for (j = 0; j < 3; ++j)
            B[i][j] = A[i][j];
   }
   /* Compute B = A^(2^e) mod m */
   for (i = 0; i < e; i++)
      MatMatModM (B, B, B, m);
}


//-------------------------------------------------------------------------
// Compute the matrix B = (A^n Mod m);  works even if A = B.
//
void MatPowModM (const double A[3][3], double B[3][3], double m, int n)
{
    int i, j;
    double W[3][3];

    /* initialize: W = A; B = I */
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j) {
            W[i][j] = A[i][j];
            B[i][j] = 0.0;
        }
    for (j = 0; j < 3; ++j)
        B[j][j] = 1.0;

    /* Compute B = A^n mod m using the binary decomposition of n */
    while (n > 0) {
        if (n % 2) MatMatModM (W, B, B, m);
        MatMatModM (W, W, W, m);
        n /= 2;
    }
}


//-------------------------------------------------------------------------
// Check that the seeds are legitimate values. Returns 0 if legal seeds,
// -1 otherwise.
//
int CheckSeed (const unsigned int seed[6])
{
    int i;

    for (i = 0; i < 3; ++i) {
        if (seed[i] >= m1) {
            std::cerr << "****************************************\n"
                 << "ERROR: Seed[" << i << "] >= 4294967087, Seed is not set."
                 << "\n****************************************\n\n";
            return (-1);
        }
    }
    for (i = 3; i < 6; ++i) {
        if (seed[i] >= m2) {
            std::cerr << "*****************************************\n"
                 << "ERROR: Seed[" << i << "] >= 4294944443, Seed is not set."
                 << "\n*****************************************\n\n";
            return (-1);
        }
    }
    if (seed[0] == 0 && seed[1] == 0 && seed[2] == 0) {
         std::cerr << "****************************\n"
              << "ERROR: First 3 seeds = 0.\n"
              << "****************************\n\n";
         return (-1);
    }
    if (seed[3] == 0 && seed[4] == 0 && seed[5] == 0) {
         std::cerr << "****************************\n"
              << "ERROR: Last 3 seeds = 0.\n"
              << "****************************\n\n";
         return (-1);
    }

    return 0;
}

} // end of anonymous namespace


//-------------------------------------------------------------------------
// Generate the next random number.
//
double RngStream::U01 ()
{
    int k;
    double p1, p2, u;

    /* Component 1 */
    p1 = a12 * Cg[1] - a13n * Cg[0];
    k = static_cast<int> (p1 / m1);
    p1 -= k * m1;
    if (p1 < 0.0) p1 += m1;
    Cg[0] = Cg[1]; Cg[1] = Cg[2]; Cg[2] = p1;

    /* Component 2 */
    p2 = a21 * Cg[5] - a23n * Cg[3];
    k = static_cast<int> (p2 / m2);
    p2 -= k * m2;
    if (p2 < 0.0) p2 += m2;
    Cg[3] = Cg[4]; Cg[4] = Cg[5]; Cg[5] = p2;

    /* Combination */
    u = ((p1 > p2) ? (p1 - p2) * norm : (p1 - p2 + m1) * norm);

    return (anti == false) ? u : (1 - u);
}


//-------------------------------------------------------------------------
// Generate the next random number with extended (53 bits) precision.
//
double RngStream::U01d ()
{
    double u;
    u = U01();
    if (anti) {
        // Don't forget that U01() returns 1 - u in the antithetic case
        u += (U01() - 1.0) * fact;
        return (u < 0.0) ? u + 1.0 : u;
    } else {
        u += U01() * fact;
        return (u < 1.0) ? u : (u - 1.0);
    }
}


//*************************************************************************
// Public members of the class start here


//-------------------------------------------------------------------------
// The default seed of the package; will be the seed of the first
// declared RngStream, unless SetPackageSeed is called.
//
double RngStream::nextSeed[6] =
{
   12345.0, 12345.0, 12345.0, 12345.0, 12345.0, 12345.0
};


//-------------------------------------------------------------------------
// constructor
//
RngStream::RngStream (const char *s) : name (s)
{
   anti = false;
   incPrec = false;

   /* Information on a stream. The arrays {Cg, Bg, Ig} contain the current
   state of the stream, the starting state of the current SubStream, and the
   starting state of the stream. This stream generates antithetic variates
   if anti = true. It also generates numbers with extended precision (53
   bits if machine follows IEEE 754 standard) if incPrec = true. nextSeed
   will be the seed of the next declared RngStream. */

   for (int i = 0; i < 6; ++i) {
      Bg[i] = Cg[i] = Ig[i] = nextSeed[i];
   }

   MatVecModM (A1p127, nextSeed, nextSeed, m1);
   MatVecModM (A2p127, &nextSeed[3], &nextSeed[3], m2);
}


//-------------------------------------------------------------------------
// Reset Stream to beginning of Stream.
//
void RngStream::ResetStartStream ()
{
   for (int i = 0; i < 6; ++i)
      Cg[i] = Bg[i] = Ig[i];
}


//-------------------------------------------------------------------------
// Reset Stream to beginning of SubStream.
//
void RngStream::ResetStartSubstream ()
{
   for (int i = 0; i < 6; ++i)
      Cg[i] = Bg[i];
}


//-------------------------------------------------------------------------
// Reset Stream to NextSubStream.
//
void RngStream::ResetNextSubstream ()
{
   MatVecModM(A1p76, Bg, Bg, m1);
   MatVecModM(A2p76, &Bg[3], &Bg[3], m2);
   for (int i = 0; i < 6; ++i)
       Cg[i] = Bg[i];
}


//-------------------------------------------------------------------------
bool RngStream::SetPackageSeed (const unsigned int seed[6])
{
   if (CheckSeed (seed))
      return false;                   // FAILURE     
   for (int i = 0; i < 6; ++i)
      nextSeed[i] = seed[i];
   return true;                       // SUCCESS
}


//-------------------------------------------------------------------------
bool RngStream::SetSeed (const unsigned int seed[6])
{
   if (CheckSeed (seed))
      return false;                   // FAILURE     
   for (int i = 0; i < 6; ++i)
      Cg[i] = Bg[i] = Ig[i] = seed[i];
   return true;                       // SUCCESS
}


//-------------------------------------------------------------------------
// if e > 0, let n = 2^e + c;
// if e < 0, let n = -2^(-e) + c;
// if e = 0, let n = c.
// Jump n steps forward if n > 0, backwards if n < 0.
//
void RngStream::AdvanceState (int e, int c)
{
    double B1[3][3], C1[3][3], B2[3][3], C2[3][3];

    if (e > 0) {
        MatTwoPowModM (A1p0, B1, m1, e);
        MatTwoPowModM (A2p0, B2, m2, e);
    } else if (e < 0) {
        MatTwoPowModM (InvA1, B1, m1, -e);
        MatTwoPowModM (InvA2, B2, m2, -e);
    }

    if (c >= 0) {
        MatPowModM (A1p0, C1, m1, c);
        MatPowModM (A2p0, C2, m2, c);
    } else {
        MatPowModM (InvA1, C1, m1, -c);
        MatPowModM (InvA2, C2, m2, -c);
    }

    if (e) {
        MatMatModM (B1, C1, C1, m1);
        MatMatModM (B2, C2, C2, m2);
    }

    MatVecModM (C1, Cg, Cg, m1);
    MatVecModM (C2, &Cg[3], &Cg[3], m2);
}


//-------------------------------------------------------------------------
void RngStream::GetState (unsigned int seed[6]) const
{
   for (int i = 0; i < 6; ++i)
      seed[i] = static_cast<unsigned int> (Cg[i]);
}


//-------------------------------------------------------------------------
void RngStream::WriteState () const
{
    std::cout << "The current state of the Rngstream";
    if (name.size() > 0)
        std::cout << " " << name;
    std::cout << ":\n   Cg = { ";

    for (int i = 0; i < 5; i++) {
        std::cout << static_cast<unsigned int> (Cg [i]) << ", ";
    }
    std::cout << static_cast<unsigned int> (Cg [5]) << " }\n\n";
}


//-------------------------------------------------------------------------
void RngStream::WriteStateFull () const
{
    int i;

    std::cout << "The RngStream";
    if (name.size() > 0)
        std::cout << " " << name;
    std::cout << ":\n   anti = " << (anti ? "true" : "false") << "\n";
    std::cout << "   incPrec = " << (incPrec ? "true" : "false") << "\n";

    std::cout << "   Ig = { ";
    for (i = 0; i < 5; i++) {
        std::cout << static_cast<unsigned int> (Ig [i]) << ", ";
    }
    std::cout << static_cast<unsigned int> (Ig [5]) << " }\n";

    std::cout << "   Bg = { ";
    for (i = 0; i < 5; i++) {
        std::cout << static_cast<unsigned int> (Bg [i]) << ", ";
    }
    std::cout << static_cast<unsigned int> (Bg [5]) << " }\n";

    std::cout << "   Cg = { ";
    for (i = 0; i < 5; i++) {
        std::cout << static_cast<unsigned int> (Cg [i]) << ", ";
    }
    std::cout << static_cast<unsigned int> (Cg [5]) << " }\n\n";
}


//-------------------------------------------------------------------------
void RngStream::IncreasedPrecis (bool incp)
{
   incPrec = incp;
}


//-------------------------------------------------------------------------
void RngStream::SetAntithetic (bool a)
{
   anti = a;
}


//-------------------------------------------------------------------------
// Generate the next random number.
//
double RngStream::RandU01 ()
{
   if (incPrec)
      return U01d();
   else
      return U01();
}


//-------------------------------------------------------------------------
// Generate the next random integer.
//
int RngStream::RandInt (int low, int high)
{
    return low + static_cast<int> ((high - low + 1.0) * RandU01 ());
};

} // End Basic namespace
} // End Eaagles namespace
