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
#include "script.hpp"



namespace script
{


script::LuaState luaState;
script::LuaState* GetLua() {
	return &luaState;
}

/* Create 'lua_CFunction' */
DILUCULUM_WRAP_FUNCTION ( GetPath );
DILUCULUM_WRAP_FUNCTION ( GetLog );
DILUCULUM_WRAP_FUNCTION ( Error );


void Initialize()
{
	/* Register the functions */
	luaState ["getPath"] = DILUCULUM_WRAPPER_FUNCTION ( GetPath );
	luaState ["getLog"] = DILUCULUM_WRAPPER_FUNCTION ( GetLog );
	luaState ["error"] = DILUCULUM_WRAPPER_FUNCTION ( Error );
	
	try {
		luaState.doFile ( settings::GetPath() + "scripts/system.main" );

	} catch ( LuaRunTimeError e ) {
		GetGameClass()->Error ( e.what(), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
	} catch ( LuaSyntaxError e ) {
		GetGameClass()->Error ( e.what(), __PRETTY_FUNCTION__, __FILE__, __LINE__ );
	}
	
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

