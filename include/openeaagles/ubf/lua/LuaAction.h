/**
 * @file        LuaAction.h
 * @brief       Declaration of class LuaAction.
 * @details     It is intended to expose its parent class PlaneAction to Lua state.
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

#ifndef _LUA_ACTION_H
#define _LUA_ACTION_H

#include "openeaagles/ubf/lua/lua-5.2.3/lua.hpp"
#include "openeaagles/ubf/lua/lua-5.2.3/luna.h"

#include "openeaagles/ubf/behaviors/PlaneAction.h"

namespace oe {
namespace lua {

/** @class LuaAction
 *  @brief This class exposes PlaneAction to Lua state.
 */
class LuaAction : public oe::behaviors::PlaneAction
{
    DECLARE_SUBCLASS(LuaAction, oe::behaviors::PlaneAction)
public:
    /** @property className 
     *  @brief Name used by Luna template class.
     */
    static const char * className;
    /** @property methods 
     *  @brief List of methods to expose to Lua.
     */
    static const Luna<oe::lua::LuaAction>::FunctionType methods[];
    /** @property properties 
     *  @brief List of properties to expose to Lua (setters and getters).
     */
    static const Luna<oe::lua::LuaAction>::PropertyType properties[];
    
    LuaAction();
    LuaAction( lua_State * l );

    // properties
    int setRoll( lua_State * l );
    int getRoll( lua_State * l );
    
    int setPitch( lua_State * l );
    int getPitch( lua_State * l );
    
    int setHeading( lua_State * l );
    int getHeading( lua_State * l );
    
    int setThrottle( lua_State * l );
    int getThrottle( lua_State * l );
    
    //int setSpeed( lua_State * l );
    //int getSpeed( lua_State * l );
    
    int setPitchTrim( lua_State * l );
    int getPitchTrim( lua_State * l );
    
    int setFireMissile( lua_State * l );
    int getFireMissile( lua_State * l );
};

}
}

#endif

