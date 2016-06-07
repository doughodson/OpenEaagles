/**
 * @file        LuaAction.cpp
 * @brief       Implementation of class LuaAction;
 * @details     
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
#include "openeaagles/ubf/lua/LuaAction.h"

namespace oe {
namespace lua {

IMPLEMENT_SUBCLASS(LuaAction, "LuaAction")
EMPTY_COPYDATA(LuaAction)
EMPTY_SERIALIZER(LuaAction)
EMPTY_SLOTTABLE(LuaAction)
EMPTY_DELETEDATA(LuaAction)

const char * oe::lua::LuaAction::className = "LuaAction";                 
const Luna<oe::lua::LuaAction>::FunctionType oe::lua::LuaAction::methods[] =      
{
    {NULL, NULL}
};

const Luna<oe::lua::LuaAction>::PropertyType oe::lua::LuaAction::properties[] =   
{
    { "roll",               &getRoll,           &setRoll },
    { "pitch",              &getPitch,          &setPitch },
    { "heading",            &getHeading,        &setHeading },
    { "throttle",           &getThrottle,       &setThrottle },
    { "pitchtrim",          &getPitchTrim,      &setPitchTrim },
    { "firemissile",        &getFireMissile,    &setFireMissile },
    { NULL, NULL, NULL }
};

/**
 * @brief Class constructor.
 */
LuaAction::LuaAction()
{
    STANDARD_CONSTRUCTOR()

    BaseClass::setPitch( 0 );
    BaseClass::setPitchChanged( false );

    BaseClass::setRoll( 0 );
    BaseClass::setRollChanged( false );

    BaseClass::setHeading( 0 );
    BaseClass::setHeadingChanged( false );

    BaseClass::setThrottle( 0 );
    BaseClass::setThrottleChanged( false );

    BaseClass::setFireMissile( 0 );
    BaseClass::setFireMissileChanged( false );

    BaseClass::setPitchTrim( 0 );
    BaseClass::setPitchTrimChanged( false );
}

/**
 * @brief Class constructor with lua_State.
 */
LuaAction::LuaAction( lua_State * l )
{

}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::setPitch( lua_State * l )
{
    BaseClass::setPitch( luaL_checknumber( l, -1 ) );
    BaseClass::setPitchChanged( true );
    return 0;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::getPitch( lua_State * l )
{
    lua_pushnumber( l, BaseClass::getPitch() );
    return 1;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::setRoll( lua_State * l )
{
    BaseClass::setRoll( luaL_checknumber( l, -1 ) );
    BaseClass::setRollChanged( true );
    return 0;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::getRoll( lua_State * l )
{
    lua_pushnumber( l, BaseClass::getRoll() );
    return 1;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::setHeading( lua_State * l )
{
    BaseClass::setHeading( luaL_checknumber( l, -1 ) );
    BaseClass::setHeadingChanged( true );
    return 0;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::getHeading( lua_State * l )
{
    lua_pushnumber( l, BaseClass::getHeading() );
    return 1;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::setThrottle( lua_State * l )
{
    BaseClass::setThrottle( luaL_checknumber( l, -1 ) );
    BaseClass::setThrottleChanged( true );
    return 0;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::getThrottle( lua_State * l )
{
    lua_pushnumber( l, BaseClass::getThrottle() );
    return 1;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::setFireMissile( lua_State * l )
{
    BaseClass::setFireMissile( lua_toboolean( l, -1 ) == 1 ? true : false );
    BaseClass::setFireMissileChanged( true );
    return 0;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::getFireMissile( lua_State * l )
{
    lua_pushnumber( l, BaseClass::getFireMissile() );
    return 1;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::setPitchTrim( lua_State * l )
{
    BaseClass::setPitchTrim( luaL_checknumber( l, -1 ) );
    BaseClass::setPitchTrimChanged( true );
    return 0;
}

/**
 * @brief Interface to allow Lua to handle parameters.
 */
int LuaAction::getPitchTrim( lua_State * l )
{
    lua_pushnumber( l, BaseClass::getPitchTrim() );
    return 1;
}

}
}
