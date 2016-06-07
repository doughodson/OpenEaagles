/**
 * @file        SoarArbiter.h
 * @brief       Declaration of class SoarArbiter.
 * @details     It is an arbiter that uses Soar to generate actions.
 *
 * @author      Marsil de Athayde Costa e Silva,
 * @author      Instituto Tecnologico de Aeronautica - ITA
 * @author      Laboratorio de Comando e Controle - ITA LAB C2
 * 
 * @date        13/06/2014
 * @version     1.0
 * @pre         
 * @bug         
 * @copyright   Copyright (c) 2014
 *
 */

#ifndef _SOAR_ARBITER_H_
#define _SOAR_ARBITER_H_

#include "soar/sml_Client.h"

#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/List.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/Integer.h"
#include "openeaagles/base/Float.h"

#include "openeaagles/base/ubf/Action.h"
#include "openeaagles/base/ubf/Arbiter.h"
#include "openeaagles/base/ubf/State.h"

#include "openeaagles/ubf/behaviors/PlaneAction.h"

#include "openeaagles/ubf/behaviors/Factory.h"
#include "openeaagles/ubf/lua/factory.h"

#include "openeaagles/ubf/soar/SoarState.h"

namespace oe {

namespace soar {

using namespace sml;

/** @class SoarArbiter
 *  @brief A meta-behavior that generates a "complex action" using Soar Cognitive Architecture.
 *    Object -> Component -> Behavior -> Arbiter -> SoarArbiter
 * 
 * Factory name: SoarArbiter
 * Slots:
 *    soarFileName <String>        Name of the .soar file
 */
class SoarArbiter : public oe::base::ubf::Arbiter
{
    DECLARE_SUBCLASS(SoarArbiter, oe::base::ubf::Arbiter)
public:
    SoarArbiter();
    
    base::ubf::Action * genAction( const base::ubf::State* const state, const double dt );
    base::ubf::Action * genComplexAction( base::List* const actionSet );
    void trimChangeValidation( base::ubf::Action * const complexAction );

    void Commit( const base::ubf::State* const state );
    void Run();
    void Retrieve();

protected:
    void initSoar();

    const base::String* getSoarFileName()    { return soarFileName; };
    // slot functions
    bool setSlotSoarFileName( base::String* const );

private:
    Kernel      * m_Kernel;
    sml::Agent  * m_smlAgent;
    SoarState   * m_soarState;
    
    sml::Identifier * m_inputLink;
    sml::Identifier * m_ID;

    const base::String * soarFileName;
};

}
}

#endif

