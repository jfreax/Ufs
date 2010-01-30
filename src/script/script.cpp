/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "LuaWrappers.hpp"

#include "../game.hpp"
#include "../settings/settings.hpp"
#include "../engine/sprite/sprite.hpp"
#include "script.hpp"
#include "../sprite/sun.hpp"

using namespace sprite;


/* Create 'lua_CFunction' */
DILUCULUM_WRAP_FUNCTION ( GetPath );
DILUCULUM_WRAP_FUNCTION ( GetLog );
DILUCULUM_WRAP_FUNCTION ( Error );

/* Create lua classes */
DILUCULUM_BEGIN_CLASS ( CMapManager );
	DILUCULUM_CLASS_METHOD ( CMapManager, GetSprite );
	DILUCULUM_CLASS_METHOD ( CMapManager, AddSprite );
DILUCULUM_END_CLASS ( CMapManager );

DILUCULUM_BEGIN_CLASS ( CSprite );
	DILUCULUM_CLASS_METHOD ( CSprite, Blub );
DILUCULUM_END_CLASS ( CSprite );


namespace Diluculum
{

Holder hold;
LuaState luaState;

LuaState* GetLua()
{
	return &luaState;
}

Holder* GetHolder()
{
	return &hold;
}


void Initialize()
{
	/* Register the functions */
	luaState ["getPath"] = DILUCULUM_WRAPPER_FUNCTION ( GetPath );
	luaState ["getLog"] = DILUCULUM_WRAPPER_FUNCTION ( GetLog );
	luaState ["error"] = DILUCULUM_WRAPPER_FUNCTION ( Error );
	
	/* Register all classes */
	DILUCULUM_REGISTER_CLASS ( luaState ["CMapManager"] , CMapManager);
	DILUCULUM_REGISTER_CLASS ( luaState ["CSprite"] , CSprite);
	
	/* Register objects */
	DILUCULUM_REGISTER_OBJECT ( luaState ["mapManager"], CMapManager, *GetGameClass()->GetMapManager() );
	
// 	hold.sprite = GetGameClass()->GetMapManager()->AddSprite ( new CSun() );
// 	RegisterObject ( "retSprite", hold.sprite );
// 	hold.sprite = new CSun();
// 	DILUCULUM_REGISTER_OBJECT ( luaState ["retSprite"], CSprite, *hold.sprite );
	
// 	hold.sprite = GetGameClass()->GetMapManager()->AddSprite ( new CSun() );
// 	hold.sprite->Blub();
// 	CMapManager* mapM = new CMapManager();
// 	mapM->Blub(*params);
// // 	hold.sprite->Blub(*params); 
	
	try {
		luaState.doFile ( settings::GetPath() + "scripts/system.main" );

	} catch ( LuaRunTimeError e ) {
		GetGameClass()->Error ( e.what(), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
	} catch ( LuaSyntaxError e ) {
		GetGameClass()->Error ( e.what(), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
	}
	

}


void RegisterObject ( std::string name, CSprite* obj)
{
	DILUCULUM_REGISTER_OBJECT ( luaState [name], CSprite, *obj );
	
}


LuaValueList GetPath ( const LuaValueList& params )
{
	LuaValueList ret;
	ret.push_back ( settings::GetPath() + "scripts/" );
		
	return ret;
}


LuaValueList GetLog ( const LuaValueList& params )
{
	int i = 0;
	if ( params.size() == 0 )
		i = 10;
	else if (params.size() != 1 || params[0].type() != LUA_TNUMBER)
		throw LuaError ( "Bad parameters!" );
	else
		i = params[0].asNumber();
	
	LuaValueList ret;
	
	std::vector< std::string >* log = GetGameClass()->GetLog();
	
	std::vector< std::string >::iterator endIter = log->end();
	std::vector< std::string >::iterator iter = log->begin();
	for ( ; iter != endIter && i; ++iter, --i ) {
		ret.push_back ( ( *iter ) );
	}
	
	
	return ret;
}


LuaValueList Error ( const LuaValueList& params )
{
	LuaValueList ret;
	
	if ( params.size() == 1 && params[0].type() == LUA_TSTRING )
		GetGameClass()->Error ( params[0].asString() );
	else
		throw LuaError ( "Bad parameters!" );
	
	
	


	return ret;
}


} /* namespace script */

