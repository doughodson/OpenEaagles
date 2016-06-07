/**
 * @file        ClipsArbiter.h
 * @brief       Declaration of class ClipsArbiter.
 * @details     It interfaces CLIPS in order to allow the creation of an arbiter using CLIPS.
 *
 * @author      Marsil de Athayde Costa e Silva,
 * @author      Instituto Tecnologico de Aeronautica - ITA
 * @author      Laboratorio de Comando e Controle - ITA LAB C2
 * 
 * @date        15/04/2014
 * @version     1.0
 * @pre         
 * @bug         
 * @copyright   Copyright (c) 2014
 *
 */
 
#ifndef __oe_clips_ClipsArbiter_H__
#define __oe_clips_ClipsArbiter_H__

#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Integer.h"
#include "openeaagles/base/Float.h"

#include "openeaagles/base/ubf/Behavior.h"
#include "openeaagles/base/ubf/Arbiter.h"
#include "openeaagles/base/ubf/Action.h"

#include "openeaagles/ubf/behaviors/PlaneBehaviors.h"
#include "openeaagles/ubf/behaviors/PlaneState.h"
#include "openeaagles/ubf/behaviors/PlaneAction.h"

#include "openeaagles/ubf/clips/clips/clips.h"
#include "openeaagles/ubf/clips/clips/clipscpp.h"

#include "openeaagles/base/factory.h"
#include "openeaagles/ubf/lua/factory.h"
#include "openeaagles/ubf/behaviors/factory.h"

namespace oe {
namespace clips {

/** @class ClipsArbiter
 *  @brief A meta-behavior that generates a "complex action" using CLIPS Expert System.
 *    Object -> Component -> Behavior -> Arbiter -> ClipsArbiter
 * 
 * Factory name: ClipsArbiter
 * Slots:
 *    clipsFileName <String>        Name of the .CLP file
 */
class ClipsArbiter : public oe::base::ubf::Arbiter
{
    DECLARE_SUBCLASS( ClipsArbiter, oe::base::ubf::Arbiter )

public:
    ClipsArbiter();
    
    virtual base::ubf::Action * genAction( const base::ubf::State* const state, const double dt );

protected:
    const base::String  * getClipsFileName()    { return clipsFileName; };
    
    /** Evaluates a list of actions and return an optional "complex action"
     * (default: returns the action with the highest vote value) 
     */
    virtual base::ubf::Action* genComplexAction(base::List* const actionSet);
    
    // Clear behavior list.
    void clearBehaviors();
    
    // slot functions
    bool setSlotClipsFileName( base::String * const );

    void assertFacts( const base::ubf::State * state );
    void getFacts();

private:

    CLIPSCPPEnv* clipsCppEnv;
    
    const base::String* clipsFileName;
    
    unsigned int m_behaviorBucket;
    unsigned int m_behaviorNameBucket;
    unsigned int m_removeBucket;
    unsigned int m_optParamsBucket;
    unsigned int m_optPairStreamBucket;
    
    void trimChangeValidation( base::ubf::Action* const complexAction );
};

}
}

#endif

