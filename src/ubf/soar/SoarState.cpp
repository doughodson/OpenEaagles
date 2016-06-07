/**
 * @file        SoarState.cpp
 * @brief       Implementation of class SoarState
 * @details     
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
#include "openeaagles/ubf/soar/SoarState.h"

namespace oe {
namespace soar {
    
IMPLEMENT_SUBCLASS(SoarState, "SoarState")
EMPTY_COPYDATA(SoarState)
EMPTY_SERIALIZER(SoarState)
EMPTY_SLOTTABLE(SoarState)
EMPTY_DELETEDATA(SoarState)

/**
 * @brief Class constructor.
 */
SoarState::SoarState()
{
    
}

/**
 * @brief Class constructor with Agent.
 */
SoarState::SoarState( sml::Agent * agent )
{
    setAgent( agent );
}

}
}