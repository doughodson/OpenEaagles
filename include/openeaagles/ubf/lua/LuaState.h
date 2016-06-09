
#ifndef __oe_ubf_lua_LuaState_H__
#define __oe_ubf_lua_LuaState_H__

#include "openeaagles/ubf/lua/lua-5.2.3/luna.h"

#include "openeaagles/ubf/behaviors/PlaneState.h"

namespace oe {
namespace lua {

//------------------------------------------------------------------------------
// Class: LuaState
// Description: Expose parent class PlaneState to Lua state
//
// Factory name: LuaState
//------------------------------------------------------------------------------
class LuaState : public oe::behaviors::PlaneState
{
    DECLARE_SUBCLASS(LuaState, oe::behaviors::PlaneState)
public:

    // name used by Luna template class
    static const char* className;

    // list of methods to expose to Lua
    static const Luna<oe::lua::LuaState>::FunctionType methods[];

    // list of properties to expose to Lua (setters and getters)
    static const Luna<oe::lua::LuaState>::PropertyType properties[];
    
    LuaState()                 {};
    LuaState(lua_State*);
    LuaState(const PlaneState*);

    // properties
    int getRoll(lua_State*);
    int getPitch(lua_State*);
    int getHeading(lua_State*);
    int getRollRate(lua_State*);
    int getPitchRate(lua_State*);
    int getYawRate(lua_State*);
    int getAltitude(lua_State*);
    int getThrottle(lua_State*);
    int getSpeed(lua_State*);
    int getPitchTrim(lua_State*);
    int getNumTracks(lua_State*);
    int getTargetTrack(lua_State*);
    int getNumEngines(lua_State*);

    int isAlive(lua_State*);
    int isTracking(lua_State*);
    int isMissileFired(lua_State*);
    int isIncomingMissile(lua_State*);
    
    // methods
    int getPitchToTracked(lua_State*);
    int getHeadingToTracked(lua_State*);
    int getDistanceToTracked(lua_State*);
};

}
}

#endif
