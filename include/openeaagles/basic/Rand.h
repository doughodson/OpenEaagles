//--------------------------------------------------------------------------
// Classes: Rand, UniformRand, NormalRand, ExponentialRand, TriangularRand
//
//--------------------------------------------------------------------------
#ifndef RAND_H_79425753_53BB_4640_8A21_21411B24599B__
#define RAND_H_79425753_53BB_4640_8A21_21411B24599B__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic {

class Number;
class RngStream;

//------------------------------------------------------------
// 
// Class: Rand
//
// Base Class: Object -> Rand
//
// Description: This is the base class for random number generators based
//              on specific distributions. This Pseudorandom Number Generator
//              is based on the MRG32k3a set by L'Eucyer. See attached paper
//              "An Object-Oriented Random-Number Package with many Long
//              Streams and Substreams" by L'Ecuyer, Simard, Chen, and Kelton
//
// Comments: Probably want to make this a purely virtual (purely abstract) class...
//
// Form name: Rand
// Slots:
//      Seed:               the seed to start the random number generator
//      Antithetic:         boolean (true is x>0) for antithetic operation (return 1-U instead of U)
//      IncreasedPrecision: boolean (true is x>0) for using increased precision (53bit double)
//
// Output: (0 <= x <= 1) as a double via getNextRand(); method
//------------------------------------------------------------
class Rand : public Object {
    DECLARE_SUBCLASS(Rand,Object)

public:
    Rand();
    //Rand* clone() const;

    double getNextRand();
    int getSeed();

protected:
    double peekRand();
    void setRand(double num);
    bool setSeed(Number* num);
    bool setSlotAntithetic(Number* num);
    bool setSlotIncreasedPrecision(Number* num);
    virtual void calculateNextRand();   //call BaseClass::calculateNextRand(); before manipulating "nextRand"?

private:
    RngStream*  prng;       //the PseudoRandom Number Generator
    Number*   seed;       //the seed
    double      nextRand;   //The next number to be given out.

}; //end class Rand

//------------------------------------------------------------
// Class: UniformRand
//
// Base Class: Object -> Rand -> UniformRand
//
// Description:
//      PseudoRandom Number Generator with a Uniform Distribution
//
// Form name: UniformRand
// Slots: (I: inherited)
//  (I) Seed:               the seed to start the random number generator
//  (I) Antithetic:         boolean (true is x>0) for antithetic operation (return 1-U instead of U)
//  (I) IncreasedPrecision: boolean (true is x>0) for using increased precision (53bit double)
//      Min:                The minimum bound. Default is 0.
//      Max:                The Maximum bound. Default is 1.
//
// Output: (Min <= x <= Max) as a double via getNextRand(); method
//------------------------------------------------------------
class UniformRand : public Rand {
    DECLARE_SUBCLASS(UniformRand,Rand)

public:
    UniformRand();

    double getMin();
    double getMax();

protected:
    bool setMin(Number* num);
    bool setMax(Number* num);
    virtual void calculateNextRand();   //call BaseClass::calculateNextRand(); before manipulating "nextRand"

private:
    Number*   min;    //the Min.
    Number*   max;    //the Max.

}; //end class UniformRand

//------------------------------------------------------------
// Class: NormalRand
//
// Base Class: Object -> Rand -> NormalRand
//
// Description:
//      PseudoRandom Number Generator with Normal Distribution.
//      Std. Normal Distribution generated from Uniform Distribution
//      using method in "Simulation Modeling & Analysis 2nd Ed."
//      by Law & Kelton pp. 491
//
// Form name: NormalRand
// Slots: (I: inherited)
//  (I) Seed:               the seed to start the random number generator
//  (I) Antithetic:         boolean (true is x>0) for antithetic operation (return 1-U instead of U)
//  (I) IncreasedPrecision: boolean (true is x>0) for using increased precision (53bit double)
//      Mean:               The Mean.
//      StdDev:             The Standard Deviation.
//
// Output: (x=?) as a double via getNextRand(); method
//------------------------------------------------------------
class NormalRand : public Rand {
    DECLARE_SUBCLASS(NormalRand,Rand)

public:
    NormalRand();

    double getMean();
    double getStdDev();

//protected:
    bool setMean(Number* num);
    bool setStdDev(Number* num);
    virtual void calculateNextRand();   //call BaseClass::calculateNextRand(); before manipulating "nextRand"

private:
    Number*   mean;       //the Mean.
    Number*   stdDev;     //the Standard Deviation.
    double      secondRand; //Normal randoms are made in pairs, this will store the second and the calculation will only have to be done once every 2 calls.
    bool        secondFlag; //Flag to use the secondRand. Return secondRand iff true.

}; //end class NormalRand

//------------------------------------------------------------
// Class: ExponentialRand
//
// Base Class: Object -> Rand -> ExponentialRand
//
// Description:
//      PseudoRandom Number Generator with Exponential Distribution.
//      Exponential Distribution generated from Uniform Distribution
//      using method in "Simulation Modeling & Analysis 2nd Ed."
//      by Law & Kelton pp. 486
//
// Form name: ExponentialRand
// Slots: (I: inherited)
//  (I) Seed:               the seed to start the random number generator
//  (I) Antithetic:         boolean (true is x>0) for antithetic operation (return 1-U instead of U)
//  (I) IncreasedPrecision: boolean (true is x>0) for using increased precision (53bit double)
//      Mean:               The Mean.
//
// Output: (x=?) as a double via getNextRand(); method
//------------------------------------------------------------
class ExponentialRand : public Rand {
    DECLARE_SUBCLASS(ExponentialRand,Rand)

public:
    ExponentialRand();

    double getMean();

protected:
    bool setMean(Number* num);
    virtual void calculateNextRand();   //call BaseClass::calculateNextRand(); before manipulating "nextRand"

private:
    Number*   mean;    //the Mean.

}; //end class ExponentialRand

//------------------------------------------------------------
// Class: TriangularRand
//
// Base Class: Object -> Rand -> TriangularRand
//
// Description:
//      PseudoRandom Number Generator with Triangular Distribution
//      Triangular Distribution generated from Uniform Distribution
//      using method in "Simulation Modeling & Analysis 2nd Ed."
//      by Law & Kelton pp. 494
//
// Form name: TriangularRand
// Slots: (I: inherited)
//  (I) Seed:               the seed to start the random number generator
//  (I) Antithetic:         boolean (true is x>0) for antithetic operation (return 1-U instead of U)
//  (I) IncreasedPrecision: boolean (true is x>0) for using increased precision (53bit double)
//      Min:                The minimum bound. Default is 0.
//      Max:                The Maximum bound. Default is 1.
//      Mode:               The Mode (height) of the Triangle. Default is 1.
//
// Output: (Min <= x <= Max) as a double via getNextRand(); method
//------------------------------------------------------------
//class TriangularRand : public Rand {
//    DECLARE_SUBCLASS(TriangularRand,Rand)
//
//public:
//    TriangularRand();
//
//    double getMin();
//    double getMax();
//    double getMode();
//
////protected:
//    bool setMin(Number* num);
//    bool setMax(Number* num);
//    bool setMode(Number* num);
//    virtual void calculateNextRand();   //call BaseClass::calculateNextRand(); before manipulating "nextRand"
//
//private:
//    Number*   min;    //the Min.
//    Number*   max;    //the Max.
//    Number*   mode;    //the Mode.
//
//}; //end class TriangularRand

} // End Basic namespace
} // End Eaagles namespace

#endif //RAND_H_79425753_53BB_4640_8A21_21411B24599B__
