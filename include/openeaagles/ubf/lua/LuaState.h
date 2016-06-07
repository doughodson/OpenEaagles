 /**
 * @file        LuaState.h
 * @brief       Declaration of class LuaState.
 * @details     It is intended to expose its parent class PlaneState to Lua state.
 *
 * @author      Marsil de Athayde Costa e Silva,
 * @author      Instituto Tecnologico de Aeronautica - ITA
 * @author      Laboratorio de Comando e Controle - ITA LAB C2
 * 
 * @date        30/05/2014
 * @version     1.0
 * @pre         
 * @bug         
 * @copyright   Copyright (c) 2014
 *
 */

#ifndef _LUA_STATE_H
#define _LUA_STATE_H

#include "openeaagles/ubf/lua/lua-5.2.3/lua.hpp"
#include "openeaagles/ubf/lua/lua-5.2.3/luna.h"

#include "openeaagles/ubf/behaviors/PlaneState.h"

namespace oe {
namespace lua {

/** @class LuaState
 *  @brief This class exposes LuaState to Lua state.
 */
class LuaState : public oe::behaviors::PlaneState
{
    DECLARE_SUBCLASS(LuaState, oe::behaviors::PlaneState)
public:
    /** @property className 
     *  @brief Name used by Luna template class.
     */
    static const char * className;
    /** @property methods 
     *  @brief List of methods to expose to Lua.
     *  Tracking methods are put in this property because they need an input parameter.
     */
    static const Luna<oe::lua::LuaState>::FunctionType methods[];
    /** @property properties 
     *  @brief List of properties to expose to Lua (setters and getters).
     */
    static const Luna<oe::lua::LuaState>::PropertyType properties[];
    
    LuaState() {};
    LuaState( lua_State * l );
    LuaState( const PlaneState * pState );

    // properties
    int getRoll( lua_State * l );
    int getPitch( lua_State * l );
    int getHeading( lua_State * l );
    int getRollRate( lua_State * l );
    int getPitchRate( lua_State * l );
    int getYawRate( lua_State * l );
    int getAltitude( lua_State * l );
    int getThrottle( lua_State * l );
    int getSpeed( lua_State * l );
    int getPitchTrim( lua_State * l );
    int getNumTracks( lua_State * l );
    int getTargetTrack( lua_State * l );
    int getNumEngines( lua_State * l );

    int isAlive( lua_State * l );
    int isTracking( lua_State * l );
    int isMissileFired( lua_State * l );
    int isIncomingMissile( lua_State * l );
    
    // methods
    int getPitchToTracked( lua_State * l );
    int getHeadingToTracked( lua_State * l );
    int getDistanceToTracked( lua_State * l );
};

}
}

#endif
