
#ifndef __oe_ubf_lua_LuaAction_H__
#define __oe_ubf_lua_LuaAction_H__

#include "openeaagles/ubf/lua/lua-5.3.3/luna.h"

#include "openeaagles/ubf/behaviors/PlaneAction.h"

namespace oe {
namespace lua {

//------------------------------------------------------------------------------
// Class: LuaAction
// Description: Exposes parent class PlaneAction to Lua state
//
// Factory name: LuaAction
//------------------------------------------------------------------------------
class LuaAction : public oe::behaviors::PlaneAction
{
    DECLARE_SUBCLASS(LuaAction, oe::behaviors::PlaneAction)

public:
    // name used by Luna template class.
    static const char* className;

    // list of methods to expose to Lua
    static const Luna<oe::lua::LuaAction>::FunctionType methods[];

    // list of properties to expose to Lua (setters and getters)
    static const Luna<oe::lua::LuaAction>::PropertyType properties[];
    
    LuaAction();
    LuaAction(lua_State*);

    // properties
    int setRoll(lua_State*);
    int getRoll(lua_State*);
    
    int setPitch(lua_State*);
    int getPitch(lua_State*);
    
    int setHeading(lua_State*);
    int getHeading(lua_State*);
    
    int setThrottle(lua_State*);
    int getThrottle(lua_State*);
    
    //int setSpeed(lua_State*);
    //int getSpeed(lua_State*);
    
    int setPitchTrim(lua_State*);
    int getPitchTrim(lua_State*);
    
    int setFireMissile(lua_State*);
    int getFireMissile(lua_State*);
};

}
}

#endif
