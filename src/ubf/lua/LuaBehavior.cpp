/**
 * @file        LuaBehavior.h
 * @brief       Implementation of class LuaBehavior.
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
 
#include "openeaagles/ubf/lua/LuaBehavior.h"

namespace oe {
namespace lua {

IMPLEMENT_SUBCLASS(LuaBehavior, "LuaBehavior")
EMPTY_COPYDATA(LuaBehavior)
EMPTY_SERIALIZER(LuaBehavior)

/** Slot table for this class type. */
BEGIN_SLOTTABLE(LuaBehavior)
   "luaFileName",                   /**  1) luaFileName */
   "luaFunctionName",               /**  2) luaFunctionName */
   "optPairStream"                  /**  3) luaParams */
END_SLOTTABLE(LuaBehavior)

/** Mapping of slots to handles. */
BEGIN_SLOT_MAP(LuaBehavior)
   ON_SLOT(1, setSlotLuaFileName,       base::String)
   ON_SLOT(2, setSlotLuaFunctionName,   base::String)
   ON_SLOT(3, setSlotLuaParams,         base::PairStream)
END_SLOT_MAP()

/**
 * @brief Class constructor.
 */
LuaBehavior::LuaBehavior()
{
    STANDARD_CONSTRUCTOR()
    
    m_lua_State = luaL_newstate();
    luaL_openlibs   ( m_lua_State );
    luaopen_base    ( m_lua_State );
    luaopen_math    ( m_lua_State );

    luaParams = 0;

    Luna<oe::lua::LuaState>::Register( m_lua_State );
    Luna<oe::lua::LuaAction>::Register( m_lua_State );

}
 
/**
 * @brief Loads Lua file ( >> dofile( "file" ) )
 */
bool LuaBehavior::LoadFile()
{
    int erred = luaL_dofile( m_lua_State, getLuaFileName() );
    if( erred )
    {
        std::cout << "Lua error: " << luaL_checkstring( m_lua_State, -1 ) << std::endl;
        return false;
    }
    return true;
}
 
/**
 * @brief getSlotByIndex()
 */
base::Object* LuaBehavior::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}
 
/**
 * @brief Set slot LuaFileName.
 */
bool LuaBehavior::setSlotLuaFileName( base::String * const x )
{
    if ( x != 0)
    {
        luaFileName = x -> getCopyString();
    }
    return LoadFile();
}
 
/**
 * @brief Set slot LuaFunctionName.
 */
bool LuaBehavior::setSlotLuaFunctionName( base::String * const x )
{
    if ( x != 0)
    {
        luaFunctionName = x -> getCopyString();
    }
    return true;
}
 
/**
 * @brief Set slot LuaParams.
 */
bool LuaBehavior::setSlotLuaParams( base::PairStream * const x)
{
    if ( x != 0 )
    {
        luaParams = x -> clone();
        base::List::Item* item = luaParams -> getFirstItem();
        while ( item != 0 )
        {
            base::Pair* pair = static_cast<base::Pair*>( item->getValue() );
        
            if( std::strcmp( pair -> slot() -> getString() , "float" ) == 0 )
            {
                luaParamsTypes.push_back( lua::TYPE_FLOAT );
            }
            else if( std::strcmp( pair -> slot() -> getString() , "char" ) == 0 )
            {
                luaParamsTypes.push_back( lua::TYPE_CHAR );
            }
            else if( std::strcmp( pair -> slot() -> getString() , "int" ) == 0 )
            {
                luaParamsTypes.push_back( lua::TYPE_INT );
            }
            else if( std::strcmp( pair -> slot() -> getString() , "bool" ) == 0 )
            {
                luaParamsTypes.push_back( lua::TYPE_BOOL );
            }

            item = item -> getNext();
        }
    }

    return true;
}
 
/**
 * @brief Generates an action by calling a Lua function (LuaFunctionName) written as a script (LuaFileName).
 */
base::ubf::Action* LuaBehavior::genAction(const base::ubf::State* const state, const double dt)
{
    oe::behaviors::PlaneAction* action = 0;
    const oe::behaviors::PlaneState* pState = dynamic_cast<const oe::behaviors::PlaneState*>(state->getUbfStateByType(typeid(oe::behaviors::PlaneState)));
    
    if (pState!=0)
    {
        action = new oe::behaviors::PlaneAction();

        LuaState * l_luaState = new LuaState( pState );
        
        lua_getglobal( m_lua_State, LuaBehavior::getFactoryName() );
        lua_getfield( m_lua_State, -1, getLuaFunctionName() );
        lua_pushvalue( m_lua_State, 1 );// self

        Luna<oe::lua::LuaState>::push( m_lua_State, l_luaState );
        
        int i = 0;
        if( luaParams != 0 )
        {
            base::List::Item* item = luaParams -> getFirstItem();
            while ( item != 0 )
            {
                base::Pair* pair = static_cast<base::Pair*>( item->getValue() );
                
                switch( luaParamsTypes.at( i ) )
                {
                    case lua::TYPE_FLOAT:
                    {
                        lua_pushnumber( m_lua_State, ( (base::Float*) pair -> object() ) -> getFloat() );
                        break;
                    }
                    case lua::TYPE_INT:
                    {
                        lua_pushinteger( m_lua_State, ( (base::Integer*) pair -> object() ) -> getInt() );
                        break;
                    }
                    case lua::TYPE_CHAR:
                    {
                        lua_pushstring( m_lua_State, ( (base::String*) pair -> object() ) -> getString() );
                        break;
                    }
                    case lua::TYPE_BOOL:
                    {
                        lua_pushboolean( m_lua_State, ( (base::Boolean*) pair -> object() ) -> getBoolean() );
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                item = item->getNext();
                i++;
            }
        }

        int l_error = lua_pcall( m_lua_State, i+2, 1, 0 );

        if( l_error )
        {
            static int l_lastError = 0;
            
            if( l_error != l_lastError )
            {
                std::cerr << "Lua error: " << lua_tostring( m_lua_State, -1 ) << std::endl;
                l_lastError = l_error;
            }
            lua_pop( m_lua_State, 1 );
        }
        else
        {
            action = (oe::behaviors::PlaneAction*) Luna<oe::lua::LuaAction>::check( m_lua_State, -1 );
            action -> setVote( getVote() );
        }
    }
    return action;
}
 
/**
 * @brief Closes Lua state.
 */
void LuaBehavior::deleteData()
{
    lua_close( m_lua_State );
}

}
}
