
#ifndef __oe_ubf_soar_SoarArbiter_H__
#define __oe_ubf_soar_SoarArbiter_H__

#include "openeaagles/base/ubf/Arbiter.h"

namespace sml {
   class Kernel;
   class Agent;
   class Identifier;
}

namespace oe {

namespace base {
   class String;
   class List;
   namespace ubf { class State; class Action; }
}

namespace soar {

class SoarState;

//------------------------------------------------------------------------------
// Class: SoarArbiter
// Description: A meta-behavior that generates a "complex action" using the
//              Soar cogitive architecture
//
// Factory name: SoarArbiter
// Slots:
//    soarFileName <String>    !    Name of the .soar file
//------------------------------------------------------------------------------
class SoarArbiter : public oe::base::ubf::Arbiter
{
    DECLARE_SUBCLASS(SoarArbiter, oe::base::ubf::Arbiter)

public:
    SoarArbiter();
    
    base::ubf::Action* genAction(const base::ubf::State* const state, const double dt);
    base::ubf::Action* genComplexAction(base::List* const actionSet);
    void trimChangeValidation(base::ubf::Action* const complexAction);

    void Commit(const base::ubf::State* const state);
    void Run();
    void Retrieve();

protected:
    void initSoar();

    const base::String* getSoarFileName()    { return soarFileName; };

    bool setSlotSoarFileName(base::String* const);

private:
    sml::Kernel* m_Kernel;
    sml::Agent* m_smlAgent;
    SoarState* m_soarState;
    
    sml::Identifier* m_inputLink;
    sml::Identifier* m_ID;

    const base::String* soarFileName;
};

}
}

#endif
