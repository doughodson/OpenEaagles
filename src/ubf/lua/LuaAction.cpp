
#include "openeaagles/ubf/lua/LuaAction.h"

namespace oe {
namespace lua {

IMPLEMENT_SUBCLASS(LuaAction, "LuaAction")
EMPTY_COPYDATA(LuaAction)
EMPTY_SERIALIZER(LuaAction)
EMPTY_SLOTTABLE(LuaAction)
EMPTY_DELETEDATA(LuaAction)

// Luna implementation
#define method(class, name)  {#name, &class::name}

// name to use to address class in Lua
const char oe::lua::LuaAction::className[] = "LuaAction";

// list of class properties that one can set/get from Lua
const Luna<oe::lua::LuaAction>::PropertyType oe::lua::LuaAction::properties[] =   
{
    {0, 0}
};

// list of class methods to make available in Lua
const Luna<oe::lua::LuaAction>::FunctionType oe::lua::LuaAction::methods[] =      
{
    method(oe::lua::LuaAction, setRoll),
    method(oe::lua::LuaAction, getRoll),
    method(oe::lua::LuaAction, setPitch),
    method(oe::lua::LuaAction, getPitch),
    method(oe::lua::LuaAction, setHeading),
    method(oe::lua::LuaAction, getHeading),
    method(oe::lua::LuaAction, setThrottle),
    method(oe::lua::LuaAction, getThrottle),
    method(oe::lua::LuaAction, setPitchTrim),
    method(oe::lua::LuaAction, getPitchTrim),
    method(oe::lua::LuaAction, setFireMissile),
    method(oe::lua::LuaAction, getFireMissile),
    {0, 0}
};

LuaAction::LuaAction()
{
    STANDARD_CONSTRUCTOR()

    BaseClass::setPitch(0);
    BaseClass::setPitchChanged(false);

    BaseClass::setRoll(0);
    BaseClass::setRollChanged(false);

    BaseClass::setHeading(0);
    BaseClass::setHeadingChanged(false);

    BaseClass::setThrottle(0);
    BaseClass::setThrottleChanged(false);

    BaseClass::setFireMissile(0);
    BaseClass::setFireMissileChanged(false);

    BaseClass::setPitchTrim(0);
    BaseClass::setPitchTrimChanged(false);
}

LuaAction::LuaAction(lua_State*)
{}

int LuaAction::setRoll(lua_State* l)
{
    BaseClass::setRoll(luaL_checknumber(l, -1));
    BaseClass::setRollChanged( true );
    return 0;
}

int LuaAction::getRoll(lua_State* l)
{
    lua_pushnumber(l, BaseClass::getRoll());
    return 1;
}

int LuaAction::setPitch(lua_State* l)
{
    BaseClass::setPitch(luaL_checknumber(l, -1));
    BaseClass::setPitchChanged(true);
    return 0;
}

int LuaAction::getPitch(lua_State* l)
{
    lua_pushnumber(l, BaseClass::getPitch());
    return 1;
}

int LuaAction::setHeading(lua_State* l)
{
    BaseClass::setHeading(luaL_checknumber(l, -1));
    BaseClass::setHeadingChanged( true );
    return 0;
}

int LuaAction::getHeading(lua_State* l)
{
    lua_pushnumber(l, BaseClass::getHeading());
    return 1;
}

int LuaAction::setThrottle(lua_State* l)
{
    BaseClass::setThrottle(luaL_checknumber(l, -1));
    BaseClass::setThrottleChanged( true );
    return 0;
}

int LuaAction::getThrottle(lua_State* l)
{
    lua_pushnumber(l, BaseClass::getThrottle());
    return 1;
}

int LuaAction::setFireMissile(lua_State* l)
{
    BaseClass::setFireMissile(lua_toboolean(l, -1) == 1 ? true : false );
    BaseClass::setFireMissileChanged(true);
    return 0;
}

int LuaAction::getFireMissile(lua_State* l)
{
    lua_pushnumber(l, BaseClass::getFireMissile());
    return 1;
}

int LuaAction::setPitchTrim(lua_State* l)
{
    BaseClass::setPitchTrim(luaL_checknumber(l, -1));
    BaseClass::setPitchTrimChanged(true);
    return 0;
}

int LuaAction::getPitchTrim(lua_State* l)
{
    lua_pushnumber(l, BaseClass::getPitchTrim());
    return 1;
}

}
}
