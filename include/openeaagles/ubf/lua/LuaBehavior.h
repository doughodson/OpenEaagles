
#ifndef __oe_ubf_lua_LuaBehavior_H__
#define __oe_ubf_lua_LuaBehavior_H__

#include "openeaagles/ubf/behaviors/PlaneBehaviors.h"

#include "openeaagles/ubf/lua/lua-5.2.3/luna.h"

#include <vector>

namespace oe {

namespace base {
   class PairStream;
   namespace ubf { class State; }
}

namespace lua {

const int TYPE_FLOAT = 0;
const int TYPE_CHAR  = 1;
const int TYPE_INT   = 2;
const int TYPE_BOOL  = 3;

//------------------------------------------------------------------------------
// Class: LuaBehavior
// Description: Interface to Lua to allow creation of behaviors in Lua script language
//
// Factory name: LuaBehavior
//------------------------------------------------------------------------------
class LuaBehavior : public oe::behaviors::PlaneBehaviorBase
{
    DECLARE_SUBCLASS(LuaBehavior, oe::behaviors::PlaneBehaviorBase)

public:
    LuaBehavior();
    bool LoadFile();

    base::ubf::Action* genAction(const base::ubf::State* const state, const double dt) override;

    char* getLuaFileName()                                { return luaFileName;     }
    void setLuaFileName(char* str)                        { luaFileName = str;      }
    bool setSlotLuaFileName(base::String* const);

    char* getLuaFunctionName()                            { return luaFunctionName; }
    void setLuaFunctionName(char* str)                    { luaFunctionName = str;  }
    bool setSlotLuaFunctionName(base::String* const);

    base::PairStream* getLuaParams()                      { return luaParams;       }
    bool setSlotLuaParams(base::PairStream* const);

private:
    char* luaFileName;
    char* luaFunctionName;

    // luaParams
    // used to create functions with any number of input parameters
    // the parameters should be set in the EPP file by setting the identifier to float, char, int or bool.
    // For example:
    //   float: 1.0
    base::PairStream* luaParams;
    lua_State* m_lua_State;
    std::vector<char> luaParamsTypes;
};

}
}

#endif
