/**
 * @file        LuaBehavior.h
 * @brief       Declaration of class LuaBehavior.
 * @details     It is used to interface with Lua and allow creation of behaviors in Lua script language.
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

#ifndef _LUA_BEHAVIOR_H_
#define _LUA_BEHAVIOR_H_

#include "openeaagles/ubf/behaviors/PlaneBehaviors.h"
#include "openeaagles/ubf/behaviors/PlaneAction.h"
#include "openeaagles/ubf/behaviors/PlaneState.h"

#include "openeaagles/ubf/lua/lua-5.2.3/lua.hpp"
#include "openeaagles/ubf/lua/lua-5.2.3/luna.h"

#include "openeaagles/ubf/lua/LuaState.h"
#include "openeaagles/ubf/lua/LuaAction.h"

#include "openeaagles/base/String.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/Float.h"
#include "openeaagles/base/Boolean.h"
#include "openeaagles/base/Integer.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"

#include <vector>

namespace oe {

namespace lua {

const int TYPE_FLOAT = 0;
const int TYPE_CHAR  = 1;
const int TYPE_INT   = 2;
const int TYPE_BOOL  = 3;

/** @class LuaBehavior
 *  @brief Behavior class to handle a Lua behavior script.
 */
class LuaBehavior : public oe::behaviors::PlaneBehaviorBase
{
    DECLARE_SUBCLASS(LuaBehavior, oe::behaviors::PlaneBehaviorBase)
public:
    LuaBehavior();
    bool LoadFile();

    base::ubf::Action* genAction(const base::ubf::State* const state, const double dt);

    char * getLuaFileName(){ return luaFileName; };
    void setLuaFileName( char * str ){ luaFileName = str; };
    bool setSlotLuaFileName( base::String * const );

    char * getLuaFunctionName(){ return luaFunctionName; };
    void setLuaFunctionName( char * str ){ luaFunctionName = str; };
    bool setSlotLuaFunctionName( base::String * const );
    
    base::PairStream * getLuaParams(){ return luaParams; };
    bool setSlotLuaParams( base::PairStream * const );

private:
    char * luaFileName;
    char * luaFunctionName;
    
    /** 
     * @property luaParams
     * @brief This is intended to allow user to create functions with any number of input parameter.
     * The parameters should be set in the EPP file by setting the identifier to float, char, int or bool. For example:
     * float: 1.0
     */
    base::PairStream* luaParams;
    lua_State* m_lua_State;
    std::vector<char> luaParamsTypes;
};

}
}

#endif
