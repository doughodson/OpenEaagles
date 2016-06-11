
#ifndef __oe_ubf_clips_ClipsArbiter_H__
#define __oe_ubf_clips_ClipsArbiter_H__

#include "openeaagles/base/ubf/Arbiter.h"

namespace oe {

namespace base {
   class String;
   class List;
   namespace ubf { class State; class Action; }
}

namespace clips {

class CLIPSCPPEnv;

//------------------------------------------------------------------------------
// Class: ClipsArbiter
// Description: A meta-behavior that generates a "complex action" using CLIPS
//              expert system.  This class defines an interface to CLIPS.
//
// Factory name: ClipsArbiter
// Slots:
//    clipsFileName <String>   !     Name of the .clp file
//------------------------------------------------------------------------------
class ClipsArbiter : public oe::base::ubf::Arbiter
{
    DECLARE_SUBCLASS(ClipsArbiter, oe::base::ubf::Arbiter)

public:
    ClipsArbiter();

    virtual base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;

protected:
    const base::String* getClipsFileName()    { return clipsFileName; };

    // evaluates a list of actions and return an optional "complex action"
    // (default: returns the action with the highest vote value)
    virtual base::ubf::Action* genComplexAction(base::List* const actionSet) override;

    void clearBehaviors();

    bool setSlotClipsFileName(base::String* const);

    void assertFacts(const base::ubf::State* state);
    void getFacts();

private:
    CLIPSCPPEnv* clipsCppEnv;

    const base::String* clipsFileName;

    unsigned int behaviorBucket;
    unsigned int behaviorNameBucket;
    unsigned int removeBucket;
    unsigned int optParamsBucket;
    unsigned int optPairStreamBucket;

    void trimChangeValidation(base::ubf::Action* const complexAction);
};

}
}

#endif
